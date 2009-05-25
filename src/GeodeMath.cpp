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
