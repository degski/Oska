
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

#include <vector>

#include <boost/container/static_vector.hpp>

#include <SFML/Graphics.hpp>

#include "Globals.hpp"

#include "Oska.hpp"
#include "Mcts.hpp"


class OutBox {

	typedef boost::container::static_vector < Point, 8 > Captured;

	Captured m_captured;

	float s0, t0, s1, t1, s2, t2, a, m_l2radius;

	std::uniform_real_distribution<float> xdist, ydist;

	bool isClose ( const Point & point1_, const Point & point2_ ) const noexcept {

		return ( point1_.x - point2_.x ) * ( point1_.x - point2_.x ) + ( point1_.y - point2_.y ) * ( point1_.y - point2_.y ) < m_l2radius;
	}

	bool isInside ( const Point & p_ ) const noexcept;

public:

	void initialise ( const Point & p0_, const Point & p1_, const Point & p2_, const float l2radius_ ) noexcept;

	Point randomPoint ( ) const noexcept;

	void add ( const Point p_ ) noexcept {

		m_captured.push_back ( p_ );
	}

	Point back ( ) const noexcept {

		return m_captured.back ( );
	}

	Captured & operator ( ) ( ) noexcept {

		return m_captured;
	}
};


template< typename State >
struct Mover {

	State * m_state;

	Point m_point_from, m_point_move;

	sf::Time m_animation_start, m_animation_duration;

	Location::type m_id = Location::invalid;

	void initialise ( State & state_ ) {

		m_state = &state_;
	}

	bool isActive ( const Location::type id_ ) const noexcept {

		return m_id == id_;
	}

	void start ( const Move m_, const float speed_, const sf::Time delay_ = sf::seconds ( 0 ) ) noexcept {

		m_id = m_state->getIdFromLocation ( m_.m_to );

		m_state->getHexRefFromID ( m_id ).setRandomOffset ( );

		m_point_from = m_state->getHexRefFromID ( m_state->getIdFromLocation ( m_.m_from ) ).point ( );
		m_point_move = m_state->getHexRefFromID ( m_id ).point ( ) - m_point_from;

		m_animation_start = now ( ) + delay_;
		m_animation_duration = sf::milliseconds ( ( sf::Int32 ) ( std::sqrt ( m_point_move.x * m_point_move.x + m_point_move.y * m_point_move.y ) / speed_ ) );
	}

	Point move ( ) noexcept {

		const float ratio = ( now ( ) - m_animation_start ) / m_animation_duration;

		if ( ratio < 0.0f ) {

			return m_point_from;
		}

		else if ( ratio > 1.0f ) {

			m_id = Location::invalid;

			return m_point_from + m_point_move;
		}

		else {

			return m_point_from + m_point_move * ratio;
		}
	}
};


template< typename State >
struct Captor {

	State * m_state;
	OutBox * m_outbox;

	Point m_point_from, m_point_to, m_point_move;

	sf::Time m_animation_start, m_animation_duration;

	bool m_active = false;

	Location::type m_id = Location::invalid;

	void initialise ( State & state_, OutBox & outbox_ ) {

		m_state = &state_;
		m_outbox = &outbox_;
	}

	bool isActive ( ) const noexcept {

		return m_active;
	}

	void start ( const Location & f_, const float speed_, const sf::Time delay_ = sf::seconds ( 0 ) ) noexcept {

		m_id = m_state->getIdFromLocation ( f_ );

		m_point_from = m_state->getHexRefFromID ( m_id ).point ( );
		m_point_to = m_outbox->randomPoint ( );
		m_point_move = m_point_to - m_point_from; // Make a move to some point.

		m_animation_start = now ( ) + delay_;
		m_animation_duration = sf::milliseconds ( ( sf::Int32 ) ( std::sqrt ( m_point_move.x * m_point_move.x + m_point_move.y * m_point_move.y ) / speed_ ) );

		m_active = true;
	}

	Point move ( ) noexcept {

		const float ratio = ( now ( ) - m_animation_start ) / m_animation_duration;

		if ( ratio < 0.0f ) {

			return m_point_from;
		}

		else if ( ratio > 1.0f ) {

			if ( m_active ) {

				m_active = false;

				m_outbox->add ( m_point_to );
			}

			return m_point_to;
		}

		else {

			return m_point_from + m_point_move * ratio;
		}
	}
};


class App {

	os::OskaState m_state;

	std::vector < Point > m_board_polygon;

public:

	float m_window_width, m_window_height;

	sf::FloatRect m_window_bounds;
	sf::FloatRect m_drag_bounding_box;

	sf::RenderWindow m_window;

	sf::RenderTexture m_board_texture;
	sf::Sprite m_board;

private:

	sf::Font m_font_regular, m_font_bold, m_font_mono, m_font_numbers;

	sf::Texture m_background_texture;
	sf::Sprite m_background;

	sf::Texture m_agent_stone_texture;
	sf::Sprite m_agent_stone;

	sf::Texture m_human_stone_texture;
	sf::Sprite m_human_stone;

	// Drag related.

	sf::Vector2f m_mouse_point;

	bool m_is_dragging = false, m_is_go_back = false;

	sf::Texture m_drag_shape_texture;
	sf::Sprite m_drag_shape;

	float m_drag_shape_l2radius;

	Location::type m_human_id_from = Location::invalid, m_human_id_to = Location::invalid;

	OutBox m_agent_outbox, m_human_outbox;

	Mover < os::OskaState > m_agent_stone_mover;
	Captor < os::OskaState > m_agent_stone_captor, m_human_stone_captor;

	sf::FloatRect m_menu_bounds, m_exit_bounds;

public:

	void initialise ( const std::int32_t no_stones_ );

	/*

	void drawSmallLogo ( );

	template<typename TextType>
	bool enterString ( TextType &text_, const std::wstring &title_ );
	template<typename TextType>
	void textInputScreen ( const std::wstring &title_, const TextType &text_ );
	std::vector<sf::FloatRect> listSelectScreen ( const std::wstring &title_, const std::vector<std::wstring> &list_ );

	bool addUser ( );
	bool authoriseUser ( );
	void selectUser ( );

	// std::wstring getCurrentUserName ( ) { return users.name ( user_id ); }

	void highScoresScreen ( );

	*/

private:

	void setIcon ( );

	void updateBoard ( )  noexcept;

	template<typename T>
	void drawStones ( T & draw_object_ ) noexcept;

public:

	bool isPointInsidePolygon ( const Point & point_ ) const noexcept;

	bool isOnStone ( const sf::Vector2f & point_ ) const noexcept {

		return ( point_.x - m_mouse_point.x ) * ( point_.x - m_mouse_point.x ) + ( point_.y - m_mouse_point.y ) * ( point_.y - m_mouse_point.y ) < m_drag_shape_l2radius;
	}

	bool isWindowOpen ( ) const {

		return m_window.isOpen ( );
	}

	bool pollWindowEvent ( sf::Event &event_ ) {

		return m_window.pollEvent ( event_ );
	}

	void closeWindow ( ) noexcept {

		m_window.close ( );
	}

	void updateWindow ( ) noexcept;

private:

	bool doHumanMove ( const Point point_ ) noexcept;
	void doAgentRandomMove ( ) noexcept;
	void doAgentMctsMove ( ) noexcept;

public:

	void mouse ( );
};
