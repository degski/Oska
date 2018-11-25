
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

#include "Colors.hpp"
#include "SecureBuffer.hpp"
#include "App.hpp"


void setRightAlignedText ( sf::Text &t_, const sf::String &s_, const float y_, const float alligned_at_, sf::Font &font_, std::uint32_t p_ = 20, const sf::Color c_ = Black, const sf::Text::Style = sf::Text::Regular );
void rightAlignText ( sf::Text &t_, const sf::String &s_ );

void setCenteredText ( sf::RenderWindow &window, sf::Text &t_, const sf::String &s_, const float y_, sf::Font &font_, std::uint32_t p_ = 20, const sf::Color c_ = Black );

template<typename T, std::size_t Capacity>
void setCenteredText ( sf::RenderWindow &window, sf::Text &t_, const SecureBuffer<T, Capacity> &s_, const float y_, sf::Font &font_, std::uint32_t p_ = 20, const sf::Color c_ = Black ) {

	t_.setString ( std::wstring ( s_.length ( ), L'*' ) );
	t_.setCharacterSize ( p_ );
	t_.setFont ( font_ );
	t_.setStyle ( sf::Text::Regular );
	t_.setOrigin ( sf::Vector2f ( std::round ( t_.getLocalBounds ( ).width * 0.5f ), std::round ( t_.getLocalBounds ( ).height * 0.5f ) ) );

	// Center text.

	t_.setPosition ( std::round ( static_cast<float> ( window.getSize ( ).x ) / 2.0f ), y_ );
	t_.setFillColor ( c_ );
}

template<typename T>
sf::Text setCenteredText ( sf::RenderWindow &window, const T &s_, const float y_, sf::Font &font_, std::uint32_t p_ = 20, const sf::Color c_ = Black ) {

	sf::Text return_value;

	setCenteredText ( window, return_value, s_, y_, font_, p_, c_ );

	return return_value;
}


void centerText ( sf::RenderWindow &window, sf::Text &t_, const sf::String &s_ );
bool validTextInput ( const sf::Event::TextEvent text_event_ );
