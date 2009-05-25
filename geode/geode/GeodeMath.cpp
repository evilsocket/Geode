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

int GeodeMath::abs( int n )
{
	return ::abs(n);
}

int GeodeMath::sgn( int n )
{
	return ( (n<0) ? -1 :( (n>0) ? 1 : 0 ) );
}

int GeodeMath::circle_next_y( int x, int r_2, int prev_y )
{
    int y  ( prev_y - 1 );
    int x_2( x * x      );

    while( x_2 + y * y > r_2 ) {
        prev_y = y--;
    }

    return prev_y;
}

double GeodeMath::degrees_to_radians( double a )
{
	return a * (G_PI / 180);
}

double GeodeMath::radians_to_degrees( double r )
{
	return (r * 180) / G_PI;
}

int	GeodeMath::pythagore( int x0, int y0, int x1, int y1 )
{
	return sqrt( (double)(::pow((double)(x1 - x0),2) + ::pow((double)(y1 - y0),2)) ); 
}

int GeodeMath::normalize( double dbl )
{
	double intpart,
		   decimals;

	decimals = modf( dbl, &intpart );

	return (int)(decimals >= 0.5 ? intpart + 1 : intpart);
}

GeodeMatrix GeodeMath::matrix( int m, int n, gcolor_t v1, ... )
{
	GeodeMatrix res(m,n,1);

	va_list val_list;            

	res(0,0,0) = v1;

	va_start( val_list, v1 );   
		for( int i = 1; i < res.size(); i++ )
			res[i] = va_arg(val_list,gcolor_t);
	va_end( val_list );     

	return res;
}

GeodeMatrix GeodeMath::vector( int n, gcolor_t v1, ... )
{
	GeodeMatrix res(1,n,1);

	va_list val_list;            

	res(0,0,0) = v1;

	va_start( val_list, v1 );   
		for( int i = 1; i < res.size(); i++ )
			res[i] = va_arg(val_list,gcolor_t);
	va_end( val_list );     

	return res;
}

}