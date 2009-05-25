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

int GeodeHardware::get_screen_width() {
	Display *d;
	int width;
	
	d = XOpenDisplay(NULL);
	width = DisplayWidth( d, DefaultScreen(d);	
	XCloseDisplay(d);
	
	return width;
}

int GeodeHardware::get_screen_height() {
	Display *d;
	int height;
	
	d = XOpenDisplay(NULL);
	height = DisplayHeight( d, DefaultScreen(d);	
	XCloseDisplay(d);
	
	return height;
}

int GeodeHardware::get_screen_depth() {
	/* TODO */
	return 3; 
}

char * GeodeHardware::get_screen_name() {
	/* TODO */
	return strdup( "0:0" );
}

}
