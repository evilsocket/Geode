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

GeodeMatrix Geode3DMath::rotation_matrix( int x, int y, int z, double angle )
{
	geode_trace( "Geode3DMath::rotation_matrix( %d, %d, %d, %f )\n", x, y, z, angle );

	double X,Y,Z,W;

	double normalization = (double)sqrt(x*x + y*y + z*z + angle*angle);
		
	if( normalization ) 
	{ 
		X = x / normalization; 
		Y = y / normalization; 
		Z = z / normalization; 
		W = angle / normalization; 
	}
	else 
	{
		X = Y = Z = 0.0; 
		W = 1.0; 
	}

	double xx = X*X, 
		   xy = X*Y, 
		   xz = X*Z, 
		   xw = X*W, 
		   yy = Y*Y, 
		   yz = Y*Z, 
		   yw = Y*W, 
		   zz = Z*Z, 
		   zw = Z*W;

	return GeodeMath::matrix( 3, 3, 
						 1 - 2*(yy+zz) , 2*(xy+zw)     , 2*(xz-yw) ,
						 2*(xy-zw)     , 1 - 2*(xx+zz) , 2*(yz+xw) ,
						 2*(xz+yw)     , 2*(yz-xw)	   , 1 - 2*(xx+yy) 
					   );
}

GeodeMatrix Geode3DMath::distance_vector( int dx, int dy, int dz )
{
	geode_trace( "Geode3DMath::distance_vector( %d, %d, %d )\n", dx, dy, dz );

	return GeodeMath::vector(3, dx, dy, dz );
}

}
