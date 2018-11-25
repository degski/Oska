
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

#include <iostream>
#include <exception>
#include <stdexcept>

#include "Globals.hpp"
#include "Text.hpp"
#include "App.hpp"
#include "Oska.hpp"
#include "Typedefs.hpp"


void handleEptr ( std::exception_ptr eptr ) { // Passing by value is ok.
	try {
		if ( eptr ) {
			std::rethrow_exception ( eptr );
		}
	}
	catch ( const std::exception & e ) {
		std::cout << "Caught exception \"" << e.what ( ) << "\"\n";
	}
}


std::int32_t wmain ( ) {

	std::int32_t no_stones = 8;

	std::exception_ptr eptr;

	try {

		// Create the app (contains the window).

		std::unique_ptr<App> app_uptr = std::make_unique<App> ( );
		app_uptr->initialise ( no_stones );

		// Setup window.

		//app_uptr->textInputScreen ( std::wstring ( L"Oska" ), std::wstring ( L"Starting." ) );
		//sleep ( 750 );

		//if ( app_uptr->users.empty ( ) ) { // Add this user.

			//while ( not ( app_uptr->addUser ( ) ) );
		//}

		//else {

			//app_uptr->selectUser ( );
		//}

		// Start the game loop.

		while ( app_uptr->isWindowOpen ( ) ) {

			// Process events.

			sf::Event event;

			while ( app_uptr->pollWindowEvent ( event ) ) {

				switch ( event.type ) {

					// Window closed.

					case sf::Event::Closed:
					{
						//app_uptr->textInputScreen ( std::wstring ( L"Oska" ), std::wstring ( L"Closing." ) );
						sleep ( 750 );

						app_uptr->closeWindow ( );

						return 0;
					}

					// We don't process other types of events

					default:
					{
						app_uptr->mouse ( );
					}
				}
			}

			// Clear, Draw and Display.

			app_uptr->updateWindow ( );
		}
	}

	catch ( ... ) {
		eptr = std::current_exception ( ); // Capture.
	}

	handleEptr ( eptr );

	return EXIT_SUCCESS;
}
