
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

#include <Windows.h>

#include <array>
#include <vector>
#include <algorithm>
#include <numeric>

#include <SFML/Graphics.hpp>


// Load resources.

template<typename T>
void LoadFromResource ( T &destination_, const std::int32_t name_ ) {

	// Loads T (sf::Image, sf::Texture, sf::Font) with image data from a resource (.rc) file.

	HRSRC rsrc_data = FindResource ( NULL, MAKEINTRESOURCE ( name_ ), L"FILEDATA" );

	if ( not ( rsrc_data ) )
		throw std::runtime_error ( "Failed to find resource." );

	DWORD rsrc_data_size = SizeofResource ( NULL, rsrc_data );

	if ( rsrc_data_size <= 0 )
		throw std::runtime_error ( "Size of resource is 0." );

	HGLOBAL grsrc_data = LoadResource ( NULL, rsrc_data );

	if ( not ( grsrc_data ) )
		throw std::runtime_error ( "Failed to load resource." );

	LPVOID first_byte = LockResource ( grsrc_data );

	if ( not ( first_byte ) )
		throw std::runtime_error ( "Failed to lock resource." );

	if ( not ( destination_.loadFromMemory ( first_byte, rsrc_data_size ) ) )
		throw std::runtime_error ( "Failed to load resource from memory." );
}

std::string LoadFromResource ( const std::int32_t name );
void LoadFromResource ( sf::Shader &shader_, const sf::Shader::Type type_, const std::int32_t name_ );
void LoadFromResource ( sf::Shader &shader_, const std::int32_t vertex_name_, const std::int32_t fragment_name_ );


// Vector2 casting.

template<typename T>
sf::Vector2f castVector2f ( const T x_, const T y_ ) {

	return sf::Vector2f ( static_cast<float> ( x_ ), static_cast<float> ( y_ ) );
}

template<typename T>
sf::Vector2f castVector2f ( const sf::Vector2<T> &v_ ) {

	return castVector2f ( v_.x, v_.y );
}

template<typename T>
sf::Vector2i castVector2i ( const T x_, const T y_ ) {

	return sf::Vector2i ( static_cast<std::int32_t> ( x_ ), static_cast<std::int32_t> ( y_ ) );
}

template<typename T>
sf::Vector2i castVector2i ( const sf::Vector2<T> &v_ ) {

	return castVector2i ( v_.x, v_.y );
}


// Apply same sort to more than 1 vector.

template<typename T>
std::vector<std::size_t> sortPermutation ( const std::vector<T> &vector_ ) {

	std::vector<std::size_t> permutation ( vector_.size ( ) );

	std::iota ( permutation.begin ( ), permutation.end ( ), 0 );
	std::sort ( permutation.begin ( ), permutation.end ( ), [ & ] ( std::size_t i, std::size_t j ) { return vector_ [ i ] < vector_ [ j ]; } );

	return permutation;
}

template <typename T>
std::vector<T> applyPermutation ( const std::vector<T> &vector_, const std::vector<std::size_t> &permutation_ ) {

	std::vector<T> sorted_vector ( permutation_.size ( ) );

	std::transform ( permutation_.begin ( ), permutation_.end ( ), sorted_vector.begin ( ), [ & ] ( std::size_t i ) { return vector_ [ i ]; } );

	return sorted_vector;
}


template <typename T>
void negateAbs ( T &x_ ) {

	if ( x_ > 0 ) {

		x_ = -x_;
	}
}


template <typename T>
T clamp ( const T x_, const T a_, const T b_ ) {

	if ( x_ <= a_ ) {

		return a_;
	}

	if ( x_ >= b_ ) {

		return b_;
	}

	return x_;
}


sf::IntRect getResourceRect ( const float square_size_, const std::array<std::int32_t, 4> &size_ );


float min ( const float a, const float b, const float c ) {

	return std::min ( std::min ( a, b ), c );
}

float max ( const float a, const float b, const float c ) {

	return std::max ( std::max ( a, b ), c );
}
