
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

#include <array>
#include <exception>
#include <iostream>
#include <stdexcept>

#include "Colors.hpp"


sf::Color makeTransparent ( const sf::Color c_ ) {

	return sf::Color { c_.r, c_.g, c_.b, 0 };
}

sf::Color makeTransparent ( const sf::Color c_, const float r_ ) {

	return sf::Color { c_.r, c_.g, c_.b, static_cast<sf::Uint8> ( 255.0f - 255.0f * r_ ) };
}

sf::Color makeOpaque ( const sf::Color c_ ) {

	return sf::Color { c_.r, c_.g, c_.b, 255 };
}

sf::Color makeOpaque ( const sf::Color c_, const float r_ ) {

	return sf::Color { c_.r, c_.g, c_.b, static_cast<sf::Uint8> ( 255.0f * r_ ) };
}
