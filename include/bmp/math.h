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
#ifndef _MATH_H_
#	define _MATH_H_

typedef unsigned char  uchar;
typedef unsigned short ushort;
typedef unsigned int   uint;

typedef unsigned char  byte;
typedef unsigned short word;
typedef unsigned int   dword;

namespace math
{
	double dsquare( double n );
	int    isquare( int    n );
	int    ipower( int base, int exp );
	word   flipw( word w );
	dword  flipdw( dword dw );
	int    sgn(int x);
	int	   abs(int n);
	int	   delta(int x1, int y1, int x2, int y2);
}

#endif
