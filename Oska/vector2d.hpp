
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

#include <cassert>
#include <cstddef>
#include <cstring>
#include <initializer_list>
#include <limits>
#include <type_traits>

#include <SFML/System/Vector2.hpp>


// A 2D-Vector for POD-Types in Row-Major Layout.
template<typename T>
struct Vector2D {

    static_assert ( std::is_default_constructible<T>::value and std::is_trivially_copyable<T>::value );

    explicit Vector2D ( std::size_t cols_, std::size_t rows_ ) noexcept :
        cols ( cols_ ),
        rows ( rows_ ),
        data ( new T [ cols * rows ] ( ) ) { // Zero-initialize for POD's ( parenthesis ).
    };

    Vector2D ( std::initializer_list<std::initializer_list<T>> data_ ) noexcept :
        Vector2D ( data_.begin ( )->size ( ), data_.size ( ) ) {
        std::size_t i = 0u;
        for ( const auto & row : data_ ) {
            for ( const auto e : row ) {
                data [ i++ ] = e;
            }
        }
    }

    Vector2D ( const Vector2D & v2d_ ) noexcept :
        Vector2D ( v2d_.cols, v2d_.rows ) {
        std::memcpy ( static_cast<void*> ( data ), static_cast<void*> ( v2d_.data ), cols * rows * sizeof ( T ) );
    }

    Vector2D ( Vector2D && v2d_ ) noexcept :
        cols ( v2d_.cols ),
        rows ( v2d_.rows ),
        data ( v2d_.data ) {
        v2d_.data = nullptr;
    }

    ~Vector2D ( ) noexcept {
        if ( data ) {
            delete [ ] data;
            data = nullptr;
        }
    }

    [[ maybe_unused ]] Vector2D & operator = ( const Vector2D & v2d_ ) noexcept {
        assert ( cols == v2d_.cols );
        assert ( rows == v2d_.rows ); // Will not work.
        std::memcpy ( static_cast<void*> ( data ), static_cast<void*> ( v2d_.data ), cols * rows * sizeof ( T ) );
    }

    [[ maybe_unused ]] Vector2D & operator = ( Vector2D && v2d_ ) noexcept {
        assert ( cols == v2d_.cols );
        assert ( rows == v2d_.rows ); // Will work, but probably not what you want.
        cols = v2d_.cols;
        rows = v2d_.rows;
        data = v2d_.data;
        v2d_.data = nullptr;
    }

    [[ nodiscard ]] const T & operator ( ) ( std::size_t col_, std::size_t row_ ) const noexcept {
        assert ( col_ < cols );
        assert ( row_ < rows );
        return data [ row_ * cols + col_ ];
    };

    template<typename U>
    [[ nodiscard ]] const T & operator ( ) ( const sf::Vector2<U> & p_ ) const noexcept {
        assert ( static_cast<std::size_t> ( p_.x ) >= 0u );
        assert ( static_cast<std::size_t> ( p_.x ) < cols );
        assert ( static_cast<std::size_t> ( p_.y ) >= 0u );
        assert ( static_cast<std::size_t> ( p_.y ) < rows );
        return data [ static_cast<std::size_t> ( p_.y ) * cols + static_cast<std::size_t> ( p_.x ) ];
    };

    [[ nodiscard ]] T & operator ( ) ( std::size_t col_, std::size_t row_ ) noexcept {
        assert ( col_ < cols );
        assert ( row_ < rows );
        return data [ row_ * cols + col_ ];
    };

    template<typename U>
    [[ nodiscard ]] T & operator ( ) ( const sf::Vector2<U> & p_ ) noexcept {
        assert ( static_cast<std::size_t> ( p_.x ) >= 0u );
        assert ( static_cast<std::size_t> ( p_.x ) < cols );
        assert ( static_cast<std::size_t> ( p_.y ) >= 0u );
        assert ( static_cast<std::size_t> ( p_.y ) < rows );
        return data [ static_cast<std::size_t> ( p_.y ) * cols + static_cast<std::size_t> ( p_.x ) ];
    };

    std::size_t cols, rows;
    T * data;

    template<typename Stream>
    friend Stream & operator << ( Stream & out_, const Vector2D & v2d_ ) noexcept {
        for ( std::size_t y = 0u; y < v2d_.rows; ++y ) {
            for ( std::size_t x = 0u; x < v2d_.cols; ++x ) {
                // out_ << std::setw ( std::numeric_limits<T>::digits10 + std::numeric_limits<T>::is_signed + 1 ) << static_cast<std::int64_t> ( v2d_ ( x, y ) ) << L' ';

                out_ << static_cast< std::int64_t > ( v2d_ ( x, y ) ) << L' ';
            }
            out_ << L'\n';
        }
        out_ << std::endl;
        return out_;
    }
};
