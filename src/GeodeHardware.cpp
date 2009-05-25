/***************************************************************************
 *   Geode 3D Rendering Engine	                                           *
 *   by evilsocket - evilsocket@gmail.com - http://www.evilsocket.net      *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/
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
