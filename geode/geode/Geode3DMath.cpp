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