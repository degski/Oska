
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

#include "Text.hpp"
#include "App.hpp"


void setRightAlignedText ( sf::Text &t_, const sf::String &s_, const float y_, const float alligned_at_, sf::Font &font_, std::uint32_t p_, const sf::Color c_, const sf::Text::Style style_ ) {

	t_.setString ( s_ );
	t_.setCharacterSize ( p_ );
	t_.setFont ( font_ );
	t_.setStyle ( style_ );

	// Right align text. at alligned_at_.

	t_.setOrigin ( sf::Vector2f ( t_.getLocalBounds ( ).left + t_.getGlobalBounds ( ).width, t_.getLocalBounds ( ).top ) );
	t_.setPosition ( alligned_at_, y_ );
	t_.setFillColor ( c_ );
}

void rightAlignText ( sf::Text &t_, const sf::String &s_ ) {

	t_.setString ( s_ );

	// Right align text. at alligned_at_.

	t_.setOrigin ( sf::Vector2f ( t_.getLocalBounds ( ).left + t_.getGlobalBounds ( ).width, t_.getLocalBounds ( ).top ) );
}

void setCenteredText ( sf::RenderWindow &window, sf::Text &t_, const sf::String &s_, const float y_, sf::Font &font_, std::uint32_t p_, const sf::Color c_ ) {

	t_.setString ( s_ );
	t_.setCharacterSize ( p_ );
	t_.setFont ( font_ );
	t_.setStyle ( sf::Text::Regular );

	// Center text.

	t_.setPosition ( ( static_cast<float> ( window.getSize ( ).x ) - t_.getGlobalBounds ( ).width ) / 2.0f, y_ );
	t_.setFillColor ( c_ );
}

void centerText ( sf::RenderWindow &window, sf::Text &t_, const sf::String &s_ ) {

	t_.setString ( s_ );

	// Center text.

	t_.setPosition ( ( static_cast<float> ( window.getSize ( ).x ) - t_.getGlobalBounds ( ).width ) / 2.0f, t_.getPosition ( ).y );
}

bool validTextInput ( const sf::Event::TextEvent text_event_ ) {

	if ( text_event_.unicode == 32 ) return true; // Space.
	if ( text_event_.unicode == 45 ) return true; // Dash.
	if ( text_event_.unicode == 46 ) return true; // Dot.
	if ( text_event_.unicode == 95 ) return true; // Underscore.

	if ( text_event_.unicode > 47 && text_event_.unicode <  58 ) return true; // Numbers.
	if ( text_event_.unicode > 64 && text_event_.unicode <  91 ) return true; // Capitals.
	if ( text_event_.unicode > 96 && text_event_.unicode < 123 ) return true; // Minors.

	return false;
}
