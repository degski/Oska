
// MIT License
//
// Copyright (c) 2016-2018 degski
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

#include "Hex.hpp"


std::ostream & operator << ( std::ostream & os, const Hex & h_ ) noexcept {

	os << "[" << h_.q << ", " << h_.r << ", " << h_.s << "]";

	return os;
}


std::ostream & operator << ( std::ostream & os, const FractionalHex & h_ ) noexcept {

	os << "[" << h_.q << ", " << h_.r << ", " << h_.s << "]";

	return os;
}


std::ostream & operator << ( std::ostream & os, const OffsetCoord & oc_ ) noexcept {

	os << "[" << oc_.col << ", " << oc_.row << "]";

	return os;
}


const std::vector<Hex> hex_directions = { Hex ( 1, 0, -1 ), Hex ( 1, -1, 0 ), Hex ( 0, -1, 1 ), Hex ( -1, 0, 1 ), Hex ( -1, 1, 0 ), Hex ( 0, 1, -1 ) };
const std::vector<Hex> hex_diagonals = { Hex ( 2, -1, -1 ), Hex ( 1, -2, 1 ), Hex ( -1, -1, 2 ), Hex ( -2, 1, 1 ), Hex ( -1, 2, -1 ), Hex ( 1, 1, -2 ) };


Hex hex_round ( const FractionalHex & h_ ) noexcept {

	Hex h ( int32_t ( std::round ( h_.q ) ), int32_t ( std::round ( h_.r ) ), int32_t ( std::round ( h_.s ) ) );

	const float q_diff = std::abs ( h.q - h_.q );
	const float r_diff = std::abs ( h.r - h_.r );
	const float s_diff = std::abs ( h.s - h_.s );

	if ( q_diff > r_diff and q_diff > s_diff ) {

		h.q = -h.r - h.s;
	}

	else if ( r_diff > s_diff ) {

		h.r = -h.q - h.s;
	}

	else {

		h.s = -h.q - h.r;
	}

	return h;
}


std::vector<Hex> hex_linedraw ( Hex a, Hex b ) noexcept {

	const int32_t N = hex_distance ( a, b );

	FractionalHex a_nudge = FractionalHex ( a.q + 0.000001f, a.r + 0.000001f, a.s - 0.000002f );
	FractionalHex b_nudge = FractionalHex ( b.q + 0.000001f, b.r + 0.000001f, b.s - 0.000002f );

	std::vector<Hex> results = { };

	float step = 1.0f / std::max ( N, 1 );

	for ( int32_t i = 0; i <= N; i++ ) {

		results.push_back ( hex_round ( hex_lerp ( a_nudge, b_nudge, step * i ) ) );
	}

	return results;
}


const int32_t EVEN = +1;
const int32_t ODD  = -1;


const Orientation layout_pointy = Orientation ( std::sqrt ( 3.0f ), std::sqrt ( 3.0f ) / 2.0f, 0.0f, 3.0f / 2.0f, std::sqrt ( 3.0f ) / 3.0f, -1.0f / 3.0f, 0.0f, 2.0f / 3.0f, 0.5f );
const Orientation layout_flat = Orientation ( 3.0f / 2.0f, 0.0f, std::sqrt ( 3.0f ) / 2.0f, std::sqrt ( 3.0f ), 2.0f / 3.0f, 0.0f, -1.0f / 3.0f, std::sqrt ( 3.0f ) / 3.0f, 0.0f );


FractionalHex pixel_to_hex ( const Layout & layout, const sf::Vector2f & p ) noexcept {

	const sf::Vector2f pt = sf::Vector2f ( ( p.x - layout.origin.x ) / layout.size.x, ( p.y - layout.origin.y ) / layout.size.y );

	const Orientation M = layout.orientation;

	const float q = M.b0 * pt.x + M.b1 * pt.y;
	const float r = M.b2 * pt.x + M.b3 * pt.y;

	return FractionalHex ( q, r, -q - r );
}


sf::Vector2f hex_corner_offset ( const Layout & layout, const int32_t corner ) noexcept {

	const float angle = 1.04719758f * ( layout.orientation.start_angle - corner );

	return sf::Vector2f ( layout.size.x * std::cos ( angle ), layout.size.y * std::sin ( angle ) );
}


std::vector<sf::Vector2f> polygon_corners ( const Layout & layout, const Hex & h ) noexcept {

	std::vector<sf::Vector2f> corners;

	corners.reserve ( 6 );

	const sf::Vector2f center = hex_to_pixel ( layout, h );

	for ( int32_t i = 0; i < 6; i++ ) {

		corners.emplace_back ( hex_corner_offset ( layout, i ) + center );
	}

	return corners;
}


/*

#include <unordered_map>


std::unordered_map<Hex, int32_t, HexHash, HexEqual> map;
std::vector<Hex> vec;


template<int32_t S>
void setup ( ) noexcept {

	map.reserve ( NO_HEXAGONS ( S ) );
	vec.reserve ( NO_HEXAGONS ( S ) );

	int32_t q = 2 - S, r;

	for ( ; q < 1; ++q ) {

		for ( r = 0; r < 2 - q; ++r ) {

			map.emplace ( Hex ( q, r, -q - r ), ( int32_t ) vec.size ( ) );
			vec.emplace_back ( Hex ( q, r, -q - r ) );
		}
	}

	for ( q = 1; q < S - 1; ++q ) {

		for ( r = 0; r < q + 2; ++r ) {

			map.emplace ( Hex ( q, r, -q - r ), ( int32_t ) vec.size ( ) );
			vec.emplace_back ( Hex ( q, r, -q - r ) );
		}
	}
}

*/
