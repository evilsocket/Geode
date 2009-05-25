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
