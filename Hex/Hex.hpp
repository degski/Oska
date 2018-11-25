
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
#pragma once

#include <cmath>
#include <cstdlib>
#include <ostream>
#include <vector>

#include <SFML/Graphics.hpp>


// Generated code -- http://www.redblobgames.com/grids/hexagons/


struct Hex {

	int32_t q, r, s;

	Hex ( const int32_t q_, const int32_t r_, const int32_t s_ ) noexcept : q ( q_ ), r ( r_ ), s ( s_ ) { }
	Hex ( const int32_t q_, const int32_t r_ ) noexcept : q ( q_ ), r ( r_ ), s ( -q - r ) { }

	Hex & operator += ( const Hex b ) noexcept {

		q += b.q, r += b.r, s += b.s;

		return *this;
	}

	Hex & operator += ( const int32_t b ) noexcept {

		q += b, r += b, s += b;

		return *this;
	}

	Hex & operator -= ( const Hex b ) noexcept {

		q -= b.q, r -= b.r, s -= b.s;

		return *this;
	}

	Hex & operator -= ( const int32_t b ) noexcept {

		q -= b, r -= b, s -= b;

		return *this;
	}

	Hex & operator *= ( const Hex b ) noexcept {

		q *= b.q, r *= b.r, s *= b.s;

		return *this;
	}

	Hex & operator *= ( const int32_t b ) noexcept {

		q *= b, r *= b, s *= b;

		return *this;
	}
};


struct HexHash {

	size_t operator ( ) ( const Hex & h_ ) const noexcept {

		return ( ( size_t ) h_.q ) << 8 or ( size_t ) h_.r;
	}
};


struct HexEqual {

	bool operator ( ) ( const Hex & lhs_, const Hex & rhs_ ) const noexcept {

		return lhs_.q == rhs_.q and lhs_.r == rhs_.r;
	}
};


std::ostream & operator << ( std::ostream & os, const Hex & h_ ) noexcept;


struct FractionalHex {

	float q, r, s;

	FractionalHex ( const float q_, const float r_, const float s_ ) noexcept : q ( q_ ), r ( r_ ), s ( s_ ) { }
	FractionalHex ( const float q_, const float r_ ) noexcept : q ( q_ ), r ( r_ ), s ( -q - r ) { }

	FractionalHex & operator += ( const FractionalHex b ) noexcept {

		q += b.q, r += b.r, s += b.s;

		return *this;
	}

	FractionalHex & operator += ( const float b ) noexcept {

		q += b, r += b, s += b;

		return *this;
	}

	FractionalHex & operator -= ( const FractionalHex b ) noexcept {

		q -= b.q, r -= b.r, s -= b.s;

		return *this;
	}

	FractionalHex & operator -= ( const float b ) noexcept {

		q -= b, r -= b, s -= b;

		return *this;
	}

	FractionalHex & operator *= ( const FractionalHex b ) noexcept {

		q *= b.q, r *= b.r, s *= b.s;

		return *this;
	}

	FractionalHex & operator *= ( const float b ) noexcept {

		q *= b, r *= b, s *= b;

		return *this;
	}
};


std::ostream & operator << ( std::ostream & os, const FractionalHex & h_ ) noexcept;


struct OffsetCoord {

	int32_t col, row;

	OffsetCoord ( const int32_t col_, const int32_t row_ ) noexcept : col ( col_ ), row ( row_ ) {}
};


std::ostream & operator << ( std::ostream & os, const OffsetCoord & oc_ ) noexcept;


struct OffsetCoordHash {

	size_t operator ( ) ( const OffsetCoord & oc_ ) const noexcept {

		return ( ( size_t ) oc_.col ) << 8 or ( size_t ) oc_.row;
	}
};


struct OffsetCoordEqual {

	bool operator ( ) ( const OffsetCoord & lhs_, const OffsetCoord & rhs_ ) const noexcept {

		return lhs_.col == rhs_.col and lhs_.row == rhs_.row;
	}
};


struct Orientation {

	float f0, f1, f2, f3, b0, b1, b2, b3;

	float start_angle;

	Orientation ( const float f0_, const float f1_, const float f2_, const float f3_, const float b0_, const float b1_, const float b2_, const float b3_, const float start_angle_ ) noexcept : f0 ( f0_ ), f1 ( f1_ ), f2 ( f2_ ), f3 ( f3_ ), b0 ( b0_ ), b1 ( b1_ ), b2 ( b2_ ), b3 ( b3_ ), start_angle ( start_angle_ ) { }
};


struct Layout {

	const Orientation orientation;

	const sf::Vector2f size, origin;

	Layout ( const Orientation & orientation_, const sf::Vector2f & size_, const sf::Vector2f & origin_ ) noexcept : orientation ( orientation_ ), size ( size_ ), origin ( origin_ ) { }
};


inline Hex hex_add ( const Hex a, const Hex b ) noexcept {

	return Hex ( a.q + b.q, a.r + b.r, a.s + b.s );
}


inline Hex operator + ( const Hex a, const Hex b ) noexcept {

	return Hex ( a.q + b.q, a.r + b.r, a.s + b.s );
}


inline Hex hex_subtract ( const Hex & a, const Hex & b ) noexcept {

	return Hex ( a.q - b.q, a.r - b.r, a.s - b.s );
}


inline Hex operator - ( const Hex a, const Hex b ) noexcept {

	return Hex ( a.q - b.q, a.r - b.r, a.s - b.s );
}


inline Hex hex_scale ( const Hex & a, int32_t k ) noexcept {

	return Hex ( a.q * k, a.r * k, a.s * k );
}


inline Hex operator * ( const Hex & a, int32_t k ) noexcept {

	return Hex ( a.q * k, a.r * k, a.s * k );
}


inline Hex operator * ( int32_t k, const Hex & a ) noexcept {

	return Hex ( a.q * k, a.r * k, a.s * k );
}


inline Hex operator * ( const Hex a, const Hex b ) noexcept {

	return Hex ( a.q * b.q, a.r * b.r, a.s * b.s );
}


extern const std::vector<Hex> hex_directions;


inline Hex hex_direction ( const int32_t direction ) noexcept {

	return hex_directions [ direction ];
}


inline Hex hex_neighbor ( const Hex & hex, int32_t direction ) noexcept {

	return hex_add ( hex, hex_direction ( direction ) );
}


extern const std::vector<Hex> hex_diagonals;


inline Hex hex_diagonal_neighbor ( const Hex & hex, int32_t direction ) noexcept {

	return hex_add ( hex, hex_diagonals [ direction ] );
}


inline int32_t hex_length ( const Hex & hex ) noexcept {

	return int32_t ( ( std::abs ( hex.q ) + std::abs ( hex.r ) + std::abs ( hex.s ) ) / 2 );
}


inline int32_t hex_distance ( const Hex & a, const Hex & b ) noexcept {

	return hex_length ( hex_subtract ( a, b ) );
}


Hex hex_round ( const FractionalHex & h_ ) noexcept;


inline FractionalHex hex_lerp ( const FractionalHex & a, const FractionalHex & b, const float t ) noexcept {

	return FractionalHex ( a.q * ( 1 - t ) + b.q * t, a.r * ( 1 - t ) + b.r * t, a.s * ( 1 - t ) + b.s * t );
}


std::vector<Hex> hex_linedraw ( Hex a, Hex b ) noexcept;


extern const int32_t EVEN;
extern const int32_t ODD;


inline OffsetCoord qoffset_from_cube ( const int32_t offset, const Hex & h ) noexcept {

	return OffsetCoord ( h.q, h.r + int32_t ( ( h.q + offset * ( h.q & 1 ) ) / 2 ) );
}


inline Hex qoffset_to_cube ( const int32_t offset, const OffsetCoord & h ) noexcept {

	return Hex ( h.col, h.row - int32_t ( ( h.col + offset * ( h.col & 1 ) ) / 2 ) );
}


inline OffsetCoord roffset_from_cube ( const int32_t offset, const Hex & h ) noexcept {

	return OffsetCoord ( h.q + int32_t ( ( h.r + offset * ( h.r & 1 ) ) / 2 ), h.r );
}


inline Hex roffset_to_cube ( const int32_t offset, const OffsetCoord & h ) noexcept {

	return Hex ( h.col - int32_t ( ( h.row + offset * ( h.row & 1 ) ) / 2 ), h.row );
}


extern const Orientation layout_pointy;
extern const Orientation layout_flat;


inline sf::Vector2f hex_to_pixel ( const Layout & layout, const Hex & h ) noexcept {

	const Orientation M = layout.orientation;

	return sf::Vector2f ( ( M.f0 * h.q + M.f1 * h.r ) * layout.size.x + layout.origin.x, ( M.f2 * h.q + M.f3 * h.r ) * layout.size.y + layout.origin.y );
}


FractionalHex pixel_to_hex ( const Layout & layout, const sf::Vector2f & p ) noexcept;

sf::Vector2f hex_corner_offset ( const Layout & layout, const int32_t corner ) noexcept;

std::vector<sf::Vector2f> polygon_corners ( const Layout & layout, const Hex & h ) noexcept;
