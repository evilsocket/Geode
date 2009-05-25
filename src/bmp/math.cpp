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
#include "math.h"

double math::dsquare( double n ) {
	return n*n;
}

int math::isquare( int n ) {
	return n*n;
}

int math::ipower( int base, int exp ) {
	int ret = 1;

	for( int i = 0; i < exp; i++ )
		ret *= base; 

	return ret;
}

word math::flipw( word w ) {
	return ( (w >> 8) | (w << 8) );
}

dword math::flipdw( dword dw ) {
	return ( ((dw&0xFF000000)>>24) | ((dw&0x000000FF)<<24) | ((dw&0x00FF0000)>>8 ) | ((dw&0x0000FF00)<<8 ) );
}

int math::sgn(int x) {
	return ((x<0)?-1:((x>0)?1:0));
}

int math::abs(int n) {
	return n *= n < 0 ? -1 : 1 ;
}

int math::delta(int x1, int y1, int x2, int y2) {
	int xd = x2-x1,
		yd = y2-y1;

	return math::isquare( xd*xd + yd*yd );
}
