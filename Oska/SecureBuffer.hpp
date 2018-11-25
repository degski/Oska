
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

#pragma once

#include <cstdint>
#include <cstdlib>
#include <string>

#include <sodium.h>


template<typename T, std::size_t ChunkSize = 8>
class SecureBuffer {

    // Memory-secure buffer class.

    std::size_t m_length = 0, m_capacity = 0;
    T * m_data = nullptr;

public:

    SecureBuffer ( ) {
    }

    ~SecureBuffer ( ) {
        clear ( );
    }

    [[ nodiscard ]] std::size_t capacityInBytes ( ) const noexcept {
        return m_capacity * sizeof ( T );
    }
    [[ nodiscard ]] std::size_t capacity ( ) const noexcept {
        return m_capacity;
    }

    [[ nodiscard ]] std::size_t lengthInBytes ( ) const noexcept {
        return m_length * sizeof ( T );
    }
    [[ nodiscard ]] std::size_t length ( ) const noexcept {
        return m_length;
    }

    [[ nodiscard ]] bool empty ( ) const noexcept {
        return m_length == 0;
    }

    [[ nodiscard ]] T * data ( ) const noexcept {
        return m_data;
    }  // Raw data access.

    void clear ( ) noexcept {
        if ( m_data != nullptr ) {
            for ( std::size_t i = 0; i < m_length; ++i ) {
                ( m_data + i )->~T ( );
            }
            sodium_munlock ( ( void* ) m_data, m_capacity * sizeof ( T ) );
            free ( m_data );
            m_data = nullptr;
            m_length = 0;
            m_capacity = 0;
        }
    }

private:

    void extend ( ) {
        const std::size_t c = m_capacity * sizeof ( T );
        m_capacity += ChunkSize;
        T* data = ( T* ) calloc ( m_capacity, sizeof ( T ) );
        sodium_mlock ( ( void* ) data, m_capacity * sizeof ( T ) );
        if ( m_data != nullptr ) {
            memcpy ( ( void* ) data, ( void* ) m_data, c );
        }
        memset ( ( void* ) ( ( std::uint8_t* ) data + c ), 0, m_capacity * sizeof ( T ) - c );
        std::swap ( m_data, data );
        if ( data != nullptr ) {
            sodium_munlock ( ( void* ) data, c );
            free ( data );
        }
    }

public:

    void emplace_back ( T && v_ ) {
        if ( m_length == m_capacity ) {
            extend ( );
        }
        new ( m_data + m_length++ ) T ( v_ );
    }

    [[ maybe_unused ]] SecureBuffer & operator += ( const T & rhs_ ) noexcept {
        if ( m_length == m_capacity ) {
            extend ( );
        }
        new ( m_data + m_length++ ) T ( rhs_ );
        return * this;
    }

    void pop_back ( ) noexcept {
        if ( m_length > 0 ) {
            ( m_data + --m_length )->~T ( );
            memset ( ( void* ) &m_data [ m_length ], 0, sizeof ( T ) );
        }
    }

    void shrink_to_fit ( ) noexcept {
        if ( m_length < m_capacity && m_length > 0 ) {
            const std::size_t l = m_length * sizeof ( T );
            T* data = ( T* ) malloc ( l );
            sodium_mlock ( ( void* ) data, l );
            memcpy ( ( void* ) data, ( void* ) m_data, l );
            std::swap ( m_data, data );
            sodium_munlock ( ( void* ) data, m_capacity * sizeof ( T ) );
            free ( data );
            m_capacity = m_length;
        }
        else if ( m_data != nullptr && m_length == 0 ) {
            sodium_munlock ( ( void* ) m_data, m_capacity * sizeof ( T ) );
            free ( m_data );
            m_data = nullptr;
            m_capacity = 0;
        }
    }

    [[ nodiscard ]] bool operator == ( const SecureBuffer & rhs_ ) const noexcept {
        if ( m_length != rhs_.m_length ) {
            return false;
        }
        for ( std::size_t i = 0; i < m_length; ++i ) {
            if ( m_data [ i ] != rhs_.m_data [ i ] ) {
                return false;
            }
        }
        return true;
    }

    [[ nodiscard ]] bool operator != ( const SecureBuffer &rhs_ ) const noexcept {
        return not ( *this == rhs_ );
    }
};

using Password = SecureBuffer<wchar_t, 8>;
