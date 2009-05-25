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

GeodePrimitive3D::GeodePrimitive3D( GeodeVertex& a, GeodeVertex& b, GeodeVertex& c )
{
	v1 = a;
	v2 = b;
	v3 = c;
}

GeodePrimitive3D::GeodePrimitive3D()
{
	v1.x = -1;
	v1.y = -1;
	v1.z = -1;
	v2.x = -1;
	v2.y = -1;
	v2.z = -1;
	v3.x = -1;
	v3.y = -1;
	v3.z = -1;
}

GeodePrimitive3D::GeodePrimitive3D( int x0, int y0, int z0, int x1, int y1, int z1, int x2, int y2, int z2 )
{
	v1.x = x0;
	v1.y = y0;
	v1.z = z0;
	v2.x = x1;
	v2.y = y1;
	v2.z = z1;
	v3.x = x2;
	v3.y = y2;
	v3.z = z2;
}

}