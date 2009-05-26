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

G3DObject::G3DObject() {
	G_TRACE( "G3DObject::G3DObject()\n" );
	m_camera_x = 0;
	m_camera_y = 0;
	m_camera_z = 0;
}

void G3DObject::add_primitive( double x0, double y0, double z0, double x1, double y1, double z1, double x2, double y2, double z2 ) {
	G_TRACE( "G3DObject::add_primitive( %f, %f, %f, %f, %f, %f, %f, %f, %f )\n",  x0,  y0,  z0,  x1,  y1,  z1,  x2,  y2,  z2 );
	m_primitives.push_back( new G3DPrimitive(x0, y0, z0, x1, y1, z1, x2, y2, z2) );
}

void G3DObject::add_primitive( GVertex& v1, GVertex& v2, GVertex& v3 ) {
	G_TRACE( "G3DObject::add_primitive( v1, v2, v3 )\n" );
	m_primitives.push_back( new G3DPrimitive(v1,v2,v3) );
}

G3DPrimitive& G3DObject::operator[] ( int idx ) {
	G_TRACE( "G3DObject::operator[] ( %d )\n", idx );
	return *m_primitives[idx];
}

void G3DObject::create( int size, ... ) {
	G_TRACE( "G3DObject::create( %d, ... )\n", size );

	va_list val_list;            

	std::vector<double> tmp;

	va_start( val_list, size );
		for( int i = 0; i < size; i++ )
			tmp.push_back(va_arg(val_list,double));
	va_end( val_list );

	std::vector<double>::iterator i;
	for( i = tmp.begin(); i != tmp.end(); i += 9 ) {
		add_primitive( *(i + 0), *(i + 1), *(i + 2),
					   *(i + 3), *(i + 4), *(i + 5),
					   *(i + 6), *(i + 7), *(i + 8));
	}
}

void G3DObject::move_camera_x( int n ) {
	G_TRACE( "G3DObject::move_camera_x( %d )\n", n );
	m_camera_x += n;
}

void G3DObject::move_camera_y( int n ){
	G_TRACE( "G3DObject::move_camera_y( %d )\n", n );
	m_camera_y += n;
}

void G3DObject::move_camera_z( int n ){
	G_TRACE( "G3DObject::move_camera_z( %d )\n", n );
	m_camera_z += n;
}

bool primitive_is_less_Z( G3DPrimitive * p1, G3DPrimitive * p2 ) {
   return p1->V1.z < p2->V1.z && p1->V2.z < p2->V2.z && p1->V3.z < p2->V3.z;
}

void G3DObject::Z_sort_primitives(){
	G_TRACE( "G3DObject::Z_sort_primitives()\n" );
	sort( m_primitives.begin( ), m_primitives.end( ), primitive_is_less_Z );
}

inline GPoint G3DObject::project_vertex( GVertex& v, double X0, double Y0, double Z0, double focale /*= 900*/ ) {
	G_TRACE( "G3DObject::project_vertex( v, %f, %f, %f, %f )\n", X0, Y0, Z0, focale );

	GPoint point;

	double projz = Z0 + v.z + focale;

	point.x = X0 + focale * (v.x / projz);
	point.y = Y0 + focale * (v.y / projz);

	return point;
}

inline G2DPrimitive& G3DObject::project_primitive( GVertex& v1, GVertex& v2, GVertex& v3 ) {
	G_TRACE( "G3DObject::project_primitive( v1, v2, v3 )\n" );

	G2DPrimitive p2D;

	p2D.P1 = project_vertex( v1, m_camera_x, m_camera_y, m_camera_z );
	p2D.P2 = project_vertex( v2, m_camera_x, m_camera_y, m_camera_z );
	p2D.P3 = project_vertex( v3, m_camera_x, m_camera_y, m_camera_z );

	return p2D;
}

inline G2DPrimitive& G3DObject::project_primitive( G3DPrimitive& P3D ) {
	G_TRACE( "G3DObject::project_primitive( P3D )\n" );
	return project_primitive( P3D.V1, P3D.V2, P3D.V3 );
}

void G3DObject::render( GGraphicalContext& ggc ) {	
	G_TRACE( "G3DObject::render( ggc )\n" );

	/*Z_sort_primitives();*/

	G3DPrimitives::iterator pi;
	G2DPrimitive P2D;

	for( pi = m_primitives.begin(); pi != m_primitives.end(); pi++ ) {
		/* compute each primitive projection from 3D to 2D space */
		P2D = project_primitive( *(*pi) );
		ggc.primitive( P2D, G_GREEN );
	}
}

void G3DObject::render( GGraphicalContext* ggc ){
	render(*ggc);
}

}

