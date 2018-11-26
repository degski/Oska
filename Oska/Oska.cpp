
// MIT License
//
// Copyright (c) 2018 degski
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#include <SFML/Graphics.hpp>

#include "Oska.hpp"


#include <cassert>      // assert
#include <cstdint>      // uintptr_t
#include <cstdlib>      // std::malloc, std::size_t

#include <algorithm>    // std::max
#include <type_traits>  // std::alignment_of
#include <utility>      // std::forward



// Small-object allocator that uses a memory pool.
// Objects constructed in this arena *must not* have delete called on them.
// Allows all memory in the arena to be freed at once (destructors will
// be called).
// Usage:
//     SmallObjectArena arena;
//     Foo* foo = arena::construct<Foo>();
//     arena.free();        // Calls ~Foo

class SmallObjectArena {

private:

    typedef void ( * Destructor ) ( void * );

    struct Record {

        Destructor m_destructor;

        int16_t m_end_of_prev_record_offset;	// Bytes between end of previous record and beginning of this one.
        int16_t m_object_offset;				// From the end of the previous record.
    };

    struct Block {

        std::size_t m_size;
        std::int8_t * m_raw_block;
        Block * m_prev_block;
        std::int8_t * m_start_of_next_record;
    };

    template<typename T>
    static void DestructorWrapper ( void * obj ) {

        static_cast < T* > ( obj )->~T ( );
    }

public:

    explicit SmallObjectArena ( std::size_t initial_pool_size_ = 8192 ) :
        m_current_block ( nullptr ) {
        assert ( initial_pool_size_ >= sizeof ( Block ) + std::alignment_of < Block >::value );
        assert ( initial_pool_size_ >= 128 );
        createNewBlock ( initial_pool_size_ );
    }

    ~SmallObjectArena ( ) {
        this->free ( );
        std::free ( m_current_block->m_raw_block );
    }

    template<typename T, typename ... Args>
    T * emplace ( Args && ... args_ ) {
        return new ( allocate<T> ( ) ) T ( std::forward < Args > ( args_ ) ... );
    }

    // Calls the destructors of all currently allocated objects
    // then frees all allocated memory. Destructors are called in
    // the reverse order that the objects were constructed in.

    void free ( ) {
        // Destroy all objects in arena, and free all blocks except
        // for the initial block.
        do {
            std::int8_t * end_of_record = m_current_block->m_start_of_next_record;
            while ( end_of_record != reinterpret_cast<std::int8_t*> ( m_current_block ) + sizeof ( Block ) ) {
                auto start_of_record = end_of_record - sizeof ( Record );
                auto record = reinterpret_cast < Record* > ( start_of_record );
                end_of_record = start_of_record - record->m_end_of_prev_record_offset;
                record->m_destructor ( end_of_record + record->m_object_offset );
            }
            if ( m_current_block->m_prev_block != nullptr ) {
                auto mem_to_free = m_current_block->m_raw_block;
                m_current_block = m_current_block->m_prev_block;
                std::free ( mem_to_free );
            }
        } while ( m_current_block->m_prev_block != nullptr );
        m_current_block->m_start_of_next_record = reinterpret_cast<std::int8_t*> ( m_current_block ) + sizeof ( Block );
    }

private:

    template<typename T>
    static std::int8_t * alignFor ( std::int8_t * ptr_ ) {

        const std::size_t alignment = std::alignment_of<T>::value;

        return ptr_ + ( alignment - ( reinterpret_cast < uintptr_t > ( ptr_ ) % alignment ) ) % alignment;
    }

    template<typename T>
    T * allocate ( ) {

        std::int8_t * object_location = alignFor<T> ( m_current_block->m_start_of_next_record );
        std::int8_t * next_record_start = alignFor < Record > ( object_location + sizeof ( T ) );

        if ( next_record_start + sizeof ( Record ) > m_current_block->m_raw_block + m_current_block->m_size ) {

            createNewBlock ( 2 * std::max ( m_current_block->m_size, sizeof ( T ) + sizeof ( Record ) + sizeof ( Block ) + 128 ) );

            object_location = alignFor<T> ( m_current_block->m_start_of_next_record );
            next_record_start = alignFor < Record > ( object_location + sizeof ( T ) );
        }

        auto record = reinterpret_cast < Record* > ( next_record_start );

        record->m_destructor = & DestructorWrapper<T>;

        assert ( object_location - m_current_block->m_start_of_next_record < 32768 );

        record->m_object_offset = static_cast < int16_t >( object_location - m_current_block->m_start_of_next_record );

        assert ( next_record_start - m_current_block->m_start_of_next_record < 32768 );

        record->m_end_of_prev_record_offset = static_cast < int16_t >( next_record_start - m_current_block->m_start_of_next_record );

        m_current_block->m_start_of_next_record = next_record_start + sizeof ( Record );

        return reinterpret_cast < T* > ( object_location );
    }

    void createNewBlock ( const std::size_t new_block_size_ ) {

        auto raw = static_cast<std::int8_t*> ( std::malloc ( new_block_size_ ) );
        auto block_start = alignFor < Block > ( raw );

        auto new_block = reinterpret_cast < Block* > ( block_start );

        new_block->m_raw_block = raw;
        new_block->m_prev_block = m_current_block;
        new_block->m_start_of_next_record = block_start + sizeof ( Block );
        new_block->m_size = new_block_size_;

        m_current_block = new_block;
    }

private:

    Block * m_current_block;
};


struct Foo {

    Foo ( const int i_ ) : i ( i_ ) { };

    ~Foo ( ) { std::cout << i << " dest\n"; }

    int i;
};

int main456778 ( ) {

    SmallObjectArena arena;

    Foo * foo = arena.emplace < Foo > ( 3 );

    std::cout << foo->i << "\n";

    arena.free ( );

    return 0;
}
