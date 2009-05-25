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

#include "math.h"

double math::dsquare( double n )
{
	return n*n;
}

int math::isquare( int    n )
{
	return n*n;
}

int math::ipower( int base, int exp )
{
	int ret = 1;

	for( int i = 0; i < exp; i++ )
		ret *= base; 

	return ret;
}

word math::flipw( word w )
{
	return ( (w >> 8) | (w << 8) );
}

dword math::flipdw( dword dw )
{
	return ( ((dw&0xFF000000)>>24) | ((dw&0x000000FF)<<24) | ((dw&0x00FF0000)>>8 ) | ((dw&0x0000FF00)<<8 ) );
}

int math::sgn(int x)
{
	return ((x<0)?-1:((x>0)?1:0));
}

int math::abs(int n)
{
	return n *= n < 0 ? -1 : 1 ;
}

int math::delta(int x1, int y1, int x2, int y2)
{
	int xd = x2-x1,
		yd = y2-y1;

	return math::isquare( xd*xd + yd*yd );
}