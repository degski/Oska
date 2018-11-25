
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
#include "Utilities.hpp"
#include "ResourceData.hpp"
#include "Utilities.hpp"

#include "resource.h"


bool OutBox::isInside ( const Point & p_ ) const noexcept {

	// http://stackoverflow.com/questions/2049582/how-to-determine-if-a-point-is-in-a-2d-triangle

	const float s = s0 + s1 * p_.x + s2 * p_.y;

	if ( s <= 0.0f ) {

		return false;
	}

	const float t = t0 + t1 * p_.x + t2 * p_.y;

	return t > 0.0f and ( s + t ) <= a;
}

void OutBox::initialise ( const Point & p0_, const Point & p1_, const Point & p2_, const float l2radius_ ) noexcept {

	m_captured.clear ( );

	// Counter-Clock-wise.

	s0 = p0_.y * p2_.x - p0_.x * p2_.y, t0 = p0_.x * p1_.y - p0_.y * p1_.x;

	s1 = p2_.y - p0_.y, t1 = p0_.y - p1_.y;
	s2 = p0_.x - p2_.x, t2 = p1_.x - p0_.x;

	a = -p1_.y * p2_.x + p0_.y * ( p2_.x - p1_.x ) + p0_.x * ( p1_.y - p2_.y ) + p1_.x * p2_.y;

	xdist = std::uniform_real_distribution<float> ( min ( p0_.x, p1_.x, p2_.x ), max ( p0_.x, p1_.x, p2_.x ) );
	ydist = std::uniform_real_distribution<float> ( min ( p0_.y, p1_.y, p2_.y ), max ( p0_.y, p1_.y, p2_.y ) );

	m_l2radius = l2radius_ * 4;
}

Point OutBox::randomPoint ( ) const noexcept {

	Point p;

	while ( true ) {

		do {

			p.x = xdist ( g_rng ), p.y = ydist ( g_rng );

		} while ( not ( isInside ( p ) ) );

		bool is_close = false;

		for ( const auto c : m_captured ) {

			if ( isClose ( p, c ) ) {

				is_close = true;
			}
		}

		if ( not ( is_close ) ) {

			break;
		}
	}

	return p;
}


void App::initialise ( const std::int32_t no_stones_ ) {

	m_state.initialise ( no_stones_ );

	const ResourceData resource_data ( no_stones_ );

	// Setup parameters.

	m_window_width = resource_data.m_xara_dim.x;
	m_window_height = resource_data.m_xara_dim.y;

	m_window_bounds = sf::FloatRect ( 0.0f, 0.0f, m_window_width, m_window_height );
	m_drag_bounding_box = sf::FloatRect ( resource_data.m_margin, resource_data.m_margin, m_window_width - 2.0f * resource_data.m_margin, m_window_height - 2.0f * resource_data.m_margin );

	// Create the m_window.

	sf::ContextSettings settings;
	settings.antialiasingLevel = 8U;

	m_window.create ( sf::VideoMode ( static_cast<std::uint32_t> ( m_window_width ), static_cast<std::uint32_t> ( m_window_height ) ), L"Oska", sf::Style::Close, settings );
	m_window.setFramerateLimit ( 60U );

	m_board_texture.create ( static_cast<std::uint32_t> ( m_window_width ), static_cast<std::uint32_t> ( m_window_height ) );
	m_board_texture.setSmooth ( true );

	// The board polygon.

	m_board_polygon = std::vector < Point > {

		Point ( resource_data.m_margin, resource_data.m_margin ),
		Point ( 0.5f * resource_data.m_xara_dim.x - resource_data.m_xara_hex_dim.x, 0.5f * resource_data.m_xara_dim.y ),
		Point ( resource_data.m_margin, resource_data.m_xara_dim.y - resource_data.m_margin ),
		Point ( resource_data.m_xara_dim.x - resource_data.m_margin, resource_data.m_xara_dim.y - resource_data.m_margin ),
		Point ( 0.5f * resource_data.m_xara_dim.x + resource_data.m_xara_hex_dim.x, 0.5f * resource_data.m_xara_dim.y ),
		Point ( resource_data.m_xara_dim.x - resource_data.m_margin, resource_data.m_margin ),
		Point ( resource_data.m_margin, resource_data.m_margin )
	};

	// Set icon.

	setIcon ( );

	// Load fonts.

	LoadFromResource ( m_font_regular, __REGULAR_FONT__ );
	LoadFromResource ( m_font_bold, __BOLD_FONT__ );
	LoadFromResource ( m_font_mono, __MONO_FONT__ );
	LoadFromResource ( m_font_numbers, __NUMBERS_FONT__ );

	// Create background sprite and stones.

	LoadFromResource ( m_background_texture, resource_data.m_bg_png );
	m_background_texture.setSmooth ( true );
	m_background.setTexture ( m_background_texture );

	LoadFromResource ( m_agent_stone_texture, resource_data.m_as_png );
	m_agent_stone_texture.setSmooth ( true );
	m_agent_stone.setTexture ( m_agent_stone_texture );
	m_agent_stone.setOrigin ( m_agent_stone.getLocalBounds ( ).width / 2.0f, m_agent_stone.getLocalBounds ( ).height / 2.0f );

	LoadFromResource ( m_human_stone_texture, resource_data.m_hs_png );
	m_human_stone_texture.setSmooth ( true );
	m_human_stone.setTexture ( m_human_stone_texture );
	m_human_stone.setOrigin ( m_agent_stone.getOrigin ( ) );

	LoadFromResource ( m_drag_shape_texture, resource_data.m_hs_png );
	m_drag_shape_texture.setSmooth ( true );
	m_drag_shape.setTexture ( m_drag_shape_texture );
	m_drag_shape.setOrigin ( m_human_stone.getOrigin ( ) );

	const float radius = m_drag_shape.getLocalBounds ( ).width / 2.0f;

	m_drag_shape_l2radius = radius * radius;

	m_agent_outbox.initialise (

		Point ( 0.5f * resource_data.m_xara_dim.x - 2.75f * resource_data.m_xara_hex_dim.x, 0.5f * resource_data.m_xara_dim.y ),
		Point ( resource_data.m_margin + radius, 0.75f * resource_data.m_xara_dim.y ),
		Point ( resource_data.m_margin + radius, 0.25f * resource_data.m_xara_dim.y ),
		m_drag_shape_l2radius
	);

	m_human_outbox.initialise (

		Point ( resource_data.m_xara_dim.x - resource_data.m_margin - radius, 0.25f * resource_data.m_xara_dim.y ),
		Point ( resource_data.m_xara_dim.x - resource_data.m_margin - radius, 0.75f * resource_data.m_xara_dim.y ),
		Point ( 0.5f * resource_data.m_xara_dim.x + 2.75f * resource_data.m_xara_hex_dim.x, 0.5f * resource_data.m_xara_dim.y ),
		m_drag_shape_l2radius
	);

	m_agent_stone_mover.initialise ( m_state );

	m_agent_stone_captor.initialise ( m_state, m_agent_outbox );
	m_human_stone_captor.initialise ( m_state, m_human_outbox );
}


void App::setIcon ( ) {
	HICON hIcon = LoadIcon ( GetModuleHandle ( NULL ), MAKEINTRESOURCE ( IDI_ICON1 ) );
	if ( hIcon ) {
		SendMessage ( m_window.getSystemHandle ( ), WM_SETICON, ICON_BIG, ( LPARAM ) hIcon );
	}
}


template<typename T>
void App::drawStones ( T & draw_object_ ) noexcept {

	bool is_agent_moving = false;

	for ( const std::int32_t id : m_state.getAgentStoneIDs ( ) ) {
		if ( m_agent_stone_mover.isActive ( id ) ) {
			is_agent_moving = true;
		}
		else {
			m_agent_stone.setPosition ( m_state.getHexRefFromID ( id ).point ( ) );
			draw_object_.draw ( m_agent_stone );
		}
	}

	for ( const std::int32_t id : m_state.getHumanStoneIDs ( ) ) {
		if ( not ( m_is_dragging and id == m_human_id_from ) ) {
			m_human_stone.setPosition ( m_state.getHexRefFromID ( id ).point ( ) );
			draw_object_.draw ( m_human_stone );
		}
	}

	if ( m_human_stone_captor.isActive ( ) ) {
		m_agent_stone.setPosition ( m_human_stone_captor.move ( ) );
		draw_object_.draw ( m_agent_stone );
	}

	if ( m_agent_stone_captor.isActive ( ) ) {
		m_human_stone.setPosition ( m_agent_stone_captor.move ( ) );
		draw_object_.draw ( m_human_stone );
	}

	// Agent's moving stone.

	if ( is_agent_moving ) {
		m_agent_stone.setPosition ( m_agent_stone_mover.move ( ) );
		draw_object_.draw ( m_agent_stone );
	}

	// Human's dragging stone.

	if ( m_is_dragging ) {
		draw_object_.draw ( m_drag_shape );
	}

	// Outboxes.

	for ( const auto & point : m_human_outbox ( ) ) {
		m_agent_stone.setPosition ( point );
		draw_object_.draw ( m_agent_stone );
	}

	for ( const auto & point : m_agent_outbox ( ) ) {
		m_human_stone.setPosition ( point );
		draw_object_.draw ( m_human_stone );
	}
}


struct EvenUneven {

	enum class ValueType : std::uint32_t { Even = 0, Uneven };

	ValueType m_even_uneven;

	EvenUneven ( const ValueType eut_ ) : m_even_uneven ( eut_ ) { }

	void flip ( ) noexcept {

		* ( ( std::uint32_t * ) ( & m_even_uneven ) ) ^= 1;
	}

	bool isUneven ( ) const noexcept {

		return m_even_uneven == ValueType::Uneven;
	}

	bool isEven ( ) const noexcept {

		return m_even_uneven == ValueType::Even;
	}
};


bool App::isPointInsidePolygon ( const Point & point_ ) const noexcept {

	// https://www.ecse.rpi.edu/Homepages/wrf/Research/Short_Notes/pnpoly.html

	EvenUneven cross_counter ( EvenUneven::ValueType::Even );

	const std::int32_t last = ( std::int32_t ) m_board_polygon.size ( ) - 1;

	for ( std::int32_t i = 0, j = last - 1; i < last; j = i++ ) {

		if ( ( ( m_board_polygon [ i ].y > point_.y ) not_eq ( m_board_polygon [ j ].y > point_.y ) ) and ( point_.x < ( m_board_polygon [ j ].x - m_board_polygon [ i ].x ) * ( point_.y - m_board_polygon [ i ].y ) / ( m_board_polygon [ j ].y - m_board_polygon [ i ].y ) + m_board_polygon [ i ].x ) ) {

			cross_counter.flip ( );
		}
	}

	return cross_counter.isUneven ( );
}


void App::updateBoard ( ) noexcept {

	m_board_texture.draw ( m_background );

	drawStones ( m_board_texture );

	m_board_texture.display ( );

	// Create sprite.

	const sf::Texture & texture = m_board_texture.getTexture ( );

	m_board.setTexture ( texture );
}


void App::updateWindow ( ) noexcept {

	updateBoard ( );

	m_window.draw ( m_board );
	m_window.display ( );
}


bool App::doHumanMove ( const Point point_ ) noexcept {

	if ( m_state.getHexRefFromID ( m_human_id_to ).isClose ( point_ ) ) {

		const Move human_move = m_state.humanMove ( m_human_id_from, m_human_id_to );

		if ( human_move == Move::none ) {

			m_state.getHexRefFromID ( m_human_id_to ).setOffsetFromPoint ( point_ );

			return false;
		}

		if ( human_move == Move::invalid ) {

			return false;
		}

		if ( human_move.isCapture ( ) ) {

			m_human_stone_captor.start ( m_state.other ( human_move.captured ( ) ), 0.4f, sf::milliseconds ( 100 ) );
		}

		m_state.doMove ( human_move );
		m_state.getHexRefFromID ( m_human_id_to ).setOffsetFromPoint ( point_ );

		return true;
	}

	return false;
}


void App::doAgentRandomMove ( ) noexcept {

	const Move agent_move = m_state.randomMove ( );

	if ( agent_move not_eq Move::invalid ) {

		if ( agent_move.isCapture ( ) ) {

			m_agent_stone_captor.start ( agent_move.captured ( ), 0.4f, sf::milliseconds ( 350 ) );
		}

		m_agent_stone_mover.start ( agent_move, 0.25f );
		m_state.doMove ( agent_move );

		return;
	}

	std::cout << "Invalid random move.\n";
	exit ( 0 );
}


void App::doAgentMctsMove ( ) noexcept {

	const Move agent_move = mcts::Mcts<os::OskaState>::compute ( m_state );

	if ( agent_move not_eq Move::invalid ) {

		if ( agent_move.isCapture ( ) ) {

			m_agent_stone_captor.start ( agent_move.captured ( ), 0.4f, sf::milliseconds ( 350 ) );
		}

		m_agent_stone_mover.start ( agent_move, 0.25f );
		m_state.doMove ( agent_move );

		return;
	}

	std::cout << "Invalid mcts move.\n";
	exit ( EXIT_FAILURE );
}


void App::mouse ( ) {

	static Point point, offset, previous_mouse_point;
	static bool is_done_once = false;

	previous_mouse_point = m_mouse_point;
	m_mouse_point = castVector2f ( sf::Mouse::getPosition ( m_window ) );

	if ( not ( m_window_bounds.contains ( m_mouse_point ) ) ) { // Not in m_window.

		if ( m_is_dragging ) {

			m_mouse_point = point = previous_mouse_point;

			m_human_id_from = m_human_id_to = Location::invalid;

			m_is_dragging = false;

			return;
		}
	}

	else if ( sf::Mouse::isButtonPressed ( sf::Mouse::Left ) ) {

		/* if ( is_user_select ) {

			m_is_selection_done = true;

			return;
		}

		else */ if ( not ( m_drag_bounding_box.contains ( m_mouse_point ) ) ) { // Notice area.

			if ( m_exit_bounds.contains ( m_mouse_point ) ) {

				// textInputScreen ( std::wstring ( L"Oska Closing." ), std::wstring ( ) );

				closeWindow ( );
			}

			return;
		}

		// Pick up stone.

		if ( not ( m_is_dragging ) ) {

			m_human_id_from = m_state.pointToHumanID ( m_mouse_point );

			if ( not ( m_state.isValidID ( m_human_id_from ) ) ) {

				return;
			}

			point = m_state.getHexRefFromID ( m_human_id_from ).point ( );

			if ( not ( isOnStone ( point ) ) ) {

				return;
			}

			m_drag_shape.setPosition ( point );

			is_done_once = true;
			m_is_dragging = true;
		}

		// Dragging it.

		if ( m_is_dragging or ( m_mouse_point.x > point.x and m_mouse_point.y > point.y and m_mouse_point.x - m_drag_shape.getLocalBounds ( ).width < point.x and m_mouse_point.y - m_drag_shape.getLocalBounds ( ).height < point.y ) ) {

			if ( is_done_once ) {

				offset = m_mouse_point - point;

				is_done_once = false;
			}

			point = m_mouse_point - offset;

			if ( not ( isPointInsidePolygon ( point ) ) ) {

				m_mouse_point = previous_mouse_point;
				point = previous_mouse_point - offset;
			}

			m_drag_shape.setPosition ( point );
		}
	}

	// Mouse-button released.

	else {

		// Move stones.

		if ( m_is_dragging ) {

			m_human_id_to = m_state.pointToHexID ( point );

			if ( m_state.isValidID ( m_human_id_to ) and doHumanMove ( point ) ) { // If Human move successfull do agent move.

				doAgentMctsMove ( );
			}

			m_human_id_from = m_human_id_to = Location::invalid;

			m_is_dragging = false;
		}
	}
}
