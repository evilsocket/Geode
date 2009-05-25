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

#include "Geode.h"

namespace GEngine
{

char GeodeHardware::vkeytoascii( int vkey, int scancode )
{
	BYTE keyStateArr[256];
	WORD word;
	char ch;

	GetKeyboardState(keyStateArr);
	ToAscii(vkey, scancode, keyStateArr, &word, 0);
	ch = (char) word;
   
	if ((GetKeyState(VK_SHIFT) & 0x8000) && vkey >= 'a' && vkey <= 'z')
		ch += 'A'-'a';

	return ch;
}

void GeodeHardware::set_screen_resolution( int width, int height )
{
	DEVMODE mode;
	mode.dmSize = sizeof(DEVMODE);
	mode.dmDriverExtra = 0;
	EnumDisplaySettings(0,ENUM_CURRENT_SETTINGS,&mode);

	mode.dmPelsWidth  = width;
	mode.dmPelsHeight = height;

	::ChangeDisplaySettings( &mode, 0 );
}

int GeodeHardware::get_screen_width()
{
	DEVMODE mode;
	mode.dmSize = sizeof(DEVMODE);
	mode.dmDriverExtra = 0;
	EnumDisplaySettings(0,ENUM_CURRENT_SETTINGS,&mode);
	return mode.dmPelsWidth;
}

int GeodeHardware::get_screen_height()
{
	DEVMODE mode;
	mode.dmSize = sizeof(DEVMODE); 
	mode.dmDriverExtra = 0;
	EnumDisplaySettings(0,ENUM_CURRENT_SETTINGS,&mode);
	return mode.dmPelsHeight; 
}

int GeodeHardware::get_screen_depth()
{
	DEVMODE mode;
	mode.dmSize = sizeof(DEVMODE); 
	mode.dmDriverExtra = 0;
	EnumDisplaySettings(0,ENUM_CURRENT_SETTINGS,&mode);
	return mode.dmBitsPerPel; 
}

char * GeodeHardware::get_screen_name()
{
	DEVMODE mode;
	mode.dmSize = sizeof(DEVMODE); 
	mode.dmDriverExtra = 0;
	EnumDisplaySettings(0,ENUM_CURRENT_SETTINGS,&mode);
	return (char *)mode.dmDeviceName; 
}

}
