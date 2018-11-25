
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

#include <SFML/Graphics.hpp>

// Color definitions. Colors have to be chosen
// so that the red component has different values
// for every color.

const sf::Color Red ( 221, 36, 36, 255 ); // My Red.
const sf::Color Green ( 128, 193, 8, 255 );
const sf::Color Blue ( 2, 115, 230, 255 );
const sf::Color Purple ( 149, 34, 220, 255 );
const sf::Color Yellow ( 253, 178, 0, 255 );
const sf::Color Orange ( 220, 87, 37, 255 );
const sf::Color Transparent ( 3, 0, 0, 0 );
const sf::Color White ( 255, 255, 255, 255 );
const sf::Color LightGrey ( 210, 210, 210, 255 );
const sf::Color Grey ( 170, 170, 170, 255 );
const sf::Color DarkGrey ( 72, 72, 72, 255 );
const sf::Color Black ( 0, 0, 0, 255 );

sf::Color makeTransparent ( const sf::Color c_ );
sf::Color makeTransparent ( const sf::Color c_, const float r_ );

sf::Color makeOpaque ( const sf::Color c_ );
sf::Color makeOpaque ( const sf::Color c_, const float r_ );
