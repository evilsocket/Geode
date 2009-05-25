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

Geode3DObject::Geode3DObject()
{
	g_trace( "Geode3DObject::Geode3DObject()\n" );

	m_camera_x = 0;
	m_camera_y = 0;
	m_camera_z = 0;
}

void Geode3DObject::add_primitive( int x0, int y0, int z0, int x1, int y1, int z1, int x2, int y2, int z2 )
{
	g_trace( "Geode3DObject::add_primitive( %d, %d, %d, %d, %d, %d, %d, %d, %d )\n",  x0,  y0,  z0,  x1,  y1,  z1,  x2,  y2,  z2 );

	m_pvector.push_back( new GeodePrimitive3D(x0, y0, z0, x1, y1, z1, x2, y2, z2) );
}

void Geode3DObject::add_primitive( GeodeVertex& v1, GeodeVertex& v2, GeodeVertex& v3 )
{
	g_trace( "Geode3DObject::add_primitive( v1, v2, v3 )\n" );

	m_pvector.push_back( new GeodePrimitive3D(v1,v2,v3) );
}

GeodePrimitive3D& Geode3DObject::operator[] ( int idx )
{
	g_trace( "Geode3DObject::operator[] ( %d )\n", idx );

	return *m_pvector[idx];
}

void Geode3DObject::create( int size, ... )
{
	g_trace( "Geode3DObject::create( %d, ... )\n", size );

	va_list val_list;            

	vector<int> tmp;

	va_start( val_list, size );
		for( int i = 0; i < size; i++ )
			tmp.push_back(va_arg(val_list,int));
	va_end( val_list );

	vector<int>::iterator i;

	for( i = tmp.begin(); i != tmp.end(); i += 9 )
	{
		add_primitive( *(i + 0), *(i + 1), *(i + 2),
					   *(i + 3), *(i + 4), *(i + 5),
					   *(i + 6), *(i + 7), *(i + 8));
	}
}

void Geode3DObject::move_camera_x( int n )
{
	g_trace( "Geode3DObject::move_camera_x( %d )\n", n );

	m_camera_x += n;
}

void Geode3DObject::move_camera_y( int n )
{
	g_trace( "Geode3DObject::move_camera_y( %d )\n", n );

	m_camera_y += n;
}

void Geode3DObject::move_camera_z( int n )
{
	g_trace( "Geode3DObject::move_camera_z( %d )\n", n );

	m_camera_z += n;
}

bool primitive_is_less_Z( GeodePrimitive3D * p1, GeodePrimitive3D * p2 )
{
   return p1->v1.z < p2->v1.z && p1->v2.z < p2->v2.z && p1->v3.z < p2->v3.z;
}

void Geode3DObject::Z_sort_primitives()
{
	g_trace( "Geode3DObject::Z_sort_primitives()\n" );

	sort( m_pvector.begin( ), m_pvector.end( ), primitive_is_less_Z );
}

inline GeodePoint Geode3DObject::project_vertex( GeodeVertex& v, int X0, int Y0, int Z0, double focale /*= 900*/ )
{
	g_trace( "Geode3DObject::project_vertex( v, %d, %d, %d, %f )\n", X0, Y0, Z0, focale );

	GeodePoint point;

	double projz = Z0 + v.z + focale;

	point.x = X0 + focale * (v.x / projz);
	point.y = Y0 + focale * (v.y / projz);

	return point;
}

inline GeodePrimitive2D& Geode3DObject::project_primitive( GeodeVertex& v1, GeodeVertex& v2, GeodeVertex& v3 )
{
	g_trace( "Geode3DObject::project_primitive( v1, v2, v3 )\n" );

	GeodePrimitive2D p2D;

	p2D.p1 = project_vertex( v1, m_camera_x, m_camera_y, m_camera_z );
	p2D.p2 = project_vertex( v2, m_camera_x, m_camera_y, m_camera_z );
	p2D.p3 = project_vertex( v3, m_camera_x, m_camera_y, m_camera_z );

	return p2D;
}

inline GeodePrimitive2D& Geode3DObject::project_primitive( GeodePrimitive3D& P3D )
{
	g_trace( "Geode3DObject::project_primitive( P3D )\n" );

	return project_primitive( P3D.v1, P3D.v2, P3D.v3 );
}

void Geode3DObject::render( GeodeGraphicalContext& ggc )
{	
	g_trace( "Geode3DObject::render( ggc )\n" );

	/*Z_sort_primitives();*/

	primitive_vector_t::iterator pi;
	
	GeodePrimitive2D P2D;

	for( pi = m_pvector.begin(); pi != m_pvector.end(); pi++ )
	{
		/* compute each primitive projection from 3D to 2D space */
		P2D = project_primitive( *(*pi) );
	
		ggc.buffer()->primitive( P2D, GREEN );
	}
}

void Geode3DObject::render( GeodeGraphicalContext* ggc )
{
	render(*ggc);
}

void Geode3DObject::save( const char * szFileName )
{
	g_trace( "Geode3DObject::save( %s )\n", szFileName );

	FILE * fd = fopen( szFileName, "w+b" );

	primitive_vector_t::iterator i;

	for( i = m_pvector.begin(); i != m_pvector.end(); i++ )
	{
		fwrite( *i, 1, sizeof(GeodePrimitive3D), fd );
	}

	fclose(fd);
}

}
