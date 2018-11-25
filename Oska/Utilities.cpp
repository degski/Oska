
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

#include "Utilities.hpp"


std::string LoadFromResource ( const std::int32_t name_ ) {

	// Loads text from a resource (.rc) file and return it as a string.

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

	return std::string ( ( char* ) first_byte, rsrc_data_size );
}


void LoadFromResource ( sf::Shader &shader_, const sf::Shader::Type type_, const std::int32_t name_ ) {

	if ( not ( shader_.loadFromMemory ( LoadFromResource ( name_ ), type_ ) ) )
		throw std::runtime_error ( "Failed to load shader from memory." );
}


void LoadFromResource ( sf::Shader &shader_, const std::int32_t vertex_name_, const std::int32_t fragment_name_ ) {

	if ( not ( shader_.loadFromMemory ( LoadFromResource ( vertex_name_ ), LoadFromResource ( fragment_name_ ) ) ) )
		throw std::runtime_error ( "Failed to load shader from memory." );
}


sf::IntRect getResourceRect ( const float square_size_, const std::array<std::int32_t, 4> &size_ ) {

	const std::size_t index = ( static_cast< std::size_t > ( square_size_ ) - 41 ) / 20;

	sf::IntRect return_value;

	switch ( index ) {

		case 0: return_value = sf::IntRect ( 0, 0, size_ [ index ], size_ [ index ] ); break;
		case 1: return_value = sf::IntRect ( size_ [ 0 ], 0, size_ [ index ], size_ [ index ] ); break;
		case 2: return_value = sf::IntRect ( size_ [ 0 ] + size_ [ 1 ], 0, size_ [ index ], size_ [ index ] ); break;
		case 3: return_value = sf::IntRect ( size_ [ 0 ] + size_ [ 1 ] + size_ [ 2 ], 0, size_ [ index ], size_ [ index ] ); break;

		default:; // Shouldn't happen.
	}
	return return_value;
}
