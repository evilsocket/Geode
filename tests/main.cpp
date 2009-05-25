/*!
 * Geode 2D/3D Rendering Engine
 * 
 * Copyright (c) 2006 by evilsocket
 *
 * http://evilsocket.altervista.org
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy of this
 * software and associated documentation files (the "Software"), to deal in the Software
 * without restriction, including without limitation the rights to use, copy, modify,
 * merge, publish, distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to the following conditions:

 * The above copyright notice and this permission notice shall be included in all copies or
 * substantial portions of the Software.

 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT
 * NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
 * DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE
 */

#include "../Geode/Geode/Geode.h"

#pragma comment( lib, "../Debug/Geode.lib" );

#include <iostream>

using namespace GEngine;

class TestEventReceiver : public GeodeEventReceiver
{
	public :
		
		bool pressed;
		bool init;

		int last_x,
			last_y;

		int char_x,
			char_y;

		Geode3DObject obj3D;

		TestEventReceiver()
		{ 
			last_x = last_y = -1; 
			init = pressed = false; 
			
			char_y = 26;
			char_x = 0; 
		}

		void OnInitialize()
		{
			cout << "OnInitialize" << endl;
		}

		void OnCommand( int wNotifyCode, int wID, HWND hwndCtl )
		{
			cout << "CONTROL " << wID << " SENT " << wNotifyCode << " NOTIFICATION" << endl;
		}

		void OnPaint()
		{
			if(!init)
			{			
				GCC->buffer()->clear();
				
				GCC->buffer()->printf( 0, 0, GREEN, BLACK, 
									   "Geode GFX Lib SCREEN(%dx%dx%d) GCC(%dx%dx%d)", 
									   GeodeHardware::get_screen_width(),
									   GeodeHardware::get_screen_height(),
									   GeodeHardware::get_screen_depth(),
									   GCC->width(),
									   GCC->height(),
									   GCC->buffer()->depth() * 8);

				obj3D.add_primitive( 400, 200, 400, 400, 600, 400, 400, 600, 800 );
				obj3D.add_primitive( 400, 200, 400, 400, 200, 800, 400, 600, 800 );
				obj3D.add_primitive( 800, 200, 400, 800, 600, 400, 800, 600, 800 );
				obj3D.add_primitive( 800, 200, 400, 800, 200, 800, 800, 600, 800 );
				obj3D.add_primitive( 400, 200, 400, 400, 600, 400, 800, 200, 400 );
				obj3D.add_primitive( 800, 200, 400, 400, 600, 400, 800, 600, 400 );
				obj3D.add_primitive( 400, 200, 800, 400, 600, 800, 800, 200, 800 );
				obj3D.add_primitive( 800, 200, 800, 400, 600, 800, 800, 600, 800 );
				obj3D.add_primitive( 400, 200, 400, 400, 200, 800, 800, 200, 400 );
				obj3D.add_primitive( 800, 200, 800, 400, 200, 800, 800, 200, 400 );
				obj3D.add_primitive( 400, 600, 400, 400, 600, 800, 800, 600, 400 );
				obj3D.add_primitive( 800, 600, 800, 400, 600, 800, 800, 600, 400 );

				GCC->render( obj3D, GREEN );
				
				GCC->update();
				
				init = true;
			}

			/*cout << "OnPaint" << endl;*/
		}

		void OnKeyDown( int vKey, int kFlags )
		{
			try
			{
				GeodeFrameBuffer bmp(1,1,3),
					             intr(1,1,3);

				char txt[0xFF] = {0};
	
				switch( vKey )
				{
					case GK_ARROWUP :
						GCC->buffer()->clear();
						obj3D.move_camera_z(-50);
						GCC->render( obj3D, GREEN );
						GCC->update();
					break;

					case GK_ARROWDOWN :
						GCC->buffer()->clear();
						obj3D.move_camera_z(+50);
						GCC->render( obj3D, GREEN );
						GCC->update();
					break;

					case GK_BACKSPACE :

						if( char_x == 0 )
						{
							char_x = GCC->width();
							char_y -= 13;
						}
						else
							char_x -= 8;

						char_x = char_x >= 0 ? char_x : 0;
						char_y = char_y >= 0 ? char_y : 0;

						txt[0] = ' ';
						GCC->buffer()->text( char_x, char_y, GREEN, BLACK, txt );
						GCC->update();			

					break;

					case GK_ENTER :

						char_x  = 0;
						char_y += 13;

					break;

					case GK_ESC :

						WND->close();

					break;

					case GK_F5 :

						GCC->buffer()->clear();

						GCC->buffer()->printf( 0, 
											   0, 
											   GREEN, 
											   BLACK, 
											   "Geode GFX Lib SCREEN(%dx%dx%d) GCC(%dx%dx%d)", 
											   GeodeHardware::get_screen_width(),
											   GeodeHardware::get_screen_height(),
											   GeodeHardware::get_screen_depth(),
											   GCC->width(),
											   GCC->height(),
											   GCC->buffer()->depth() * 8);

						char_x = 0;
						char_y = 0;

					break;

					case GK_B :

						bmp.load_bmp( "c:/images/face00.bmp" );
						intr = bmp;
						GCC->buffer()->set_portion( 30, 30, bmp );

						for( int i = 0; i < 200; i++ )
						{
							GCC->buffer()->printf( 30, 15, WHITE, BLACK, "%d", i ); 

							intr = intr.denoise();

							GCC->buffer()->set_portion( 35 + bmp.width(), 30, intr );

							GCC->update();
						}

					break;

					default :
						
						char ascii = GeodeHardware::vkeytoascii( vKey, kFlags );

						if( ascii > 0 && isprint(ascii) )
						{
							txt[0] = ascii;
							GCC->buffer()->text( char_x, char_y, WHITE, 0, txt );
							GCC->update();

							if( char_x >= GCC->width() )
							{
								char_x = 0;
								char_y += 13;
							}
							else
								char_x += 8;

							cout << ascii;
						}
				}
			}
			catch( GeodeException * pex )
			{
				cerr << "ERROR : " << pex->code() << " " << pex->message() << endl ;
				delete pex;
			}
		}

		void OnMouseMove(int x, int y)
		{
			if( pressed )
			{
				if( last_x >= 0 )
				{
					obj3D.move_camera_x( x - last_x );
					obj3D.move_camera_y( y - last_y );

					GCC->buffer()->clear();
					GCC->render( obj3D, GREEN );
					GCC->update();
				}

				last_x = x;
				last_y = y;
			}
		}

		void OnClose()
		{
			cout << "OnClose" << endl;
		}

		void OnLButtonDown()
		{
			pressed = true;
			last_x = last_y = -1;
			cout << "Left button down" << endl;
		}

		void OnRButtonDown()
		{
			
		}

		void OnMButtonDown()
		{
			cout << "Middle button down" << endl;
		}

		void OnLButtonUp()
		{
			pressed = false;
			cout << "Left button up" << endl;
		}

		void OnRButtonUp()
		{
			cout << "Right button up" << endl;
		}

		void OnMButtonUp()
		{
			cout << "Middle button up" << endl;			
		}
};

int main(int argc, char* argv[])
{
	try
	{
		TestEventReceiver eReceiver;

		GeodeWindow geodeWindow( &eReceiver /*, 800, 600*/ );

	}
	catch( GeodeException * pex )
	{
		cerr << "ERROR : " << pex->code() << " " << pex->message() << endl ;
		delete pex;
	}

	return 0;
}



