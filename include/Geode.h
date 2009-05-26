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
#ifndef _GEODE_H_
#	define _GEODE_H_

#include <math.h>
#include <memory.h>
#include <stdarg.h>
#include <stdio.h>
#include <pthread.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xos.h>
#include <string>
#include <vector>
#include <algorithm>

namespace GEngine {

//#define GEODE_DEBUG
#ifdef GEODE_DEBUG
# define G_TRACE printf
#else
# define G_TRACE //
#endif

#define G_DEFINE_COLOR( name, value ) unsigned long name = value

static G_DEFINE_COLOR( G_RED  , 0xFF0000 );
static G_DEFINE_COLOR( G_GREEN, 0x00FF00 );
static G_DEFINE_COLOR( G_BLUE , 0x0000FF );
static G_DEFINE_COLOR( G_WHITE, 0xFFFFFF );
static G_DEFINE_COLOR( G_BLACK, 0x000000 );

typedef std::string GException;

class GWindow; 
class GFrameBuffer;

typedef GFrameBuffer GGraphicalContext;

typedef struct _GPoint {
	double x;
	double y;
	
	_GPoint(){
		this->x = 0.0;
		this->y = 0.0;
	}
	
	_GPoint( double x, double y ){
		this->x = x;
		this->y = y;
	}
}
GPoint;

typedef struct _GVertex {
	double x;
	double y;
	double z;	
	
	_GVertex(){
		this->x = 0.0;
		this->y = 0.0;
		this->z = 0.0;	
	}
	
	_GVertex( double x, double y, double z ){
		this->x = x;
		this->y = y;
		this->z = z;	
	}
}
GVertex;

typedef struct { 
	GPoint P1;
	GPoint P2;
	GPoint P3;
}
G2DPrimitive;

typedef struct _G3DPrimitive {
	GVertex V1;
	GVertex V2;
	GVertex V3;
	
	_G3DPrimitive( GVertex& a, GVertex& b, GVertex& c ){
		V1.x = a.x;
		V1.y = a.y;
		V1.z = a.z;	
		V2.x = b.x;
		V2.y = b.y;
		V2.z = b.z;	
		V3.x = c.x;
		V3.y = c.y;
		V3.z = c.z;	
	} 
	
	_G3DPrimitive( double x0, double y0, double z0, double x1, double y1, double z1, double x2, double y2, double z2 ){
		V1.x = x0;
		V1.y = y0;
		V1.z = z0;	
		V2.x = x1;
		V2.y = y1;
		V2.z = z1;	
		V3.x = x2;
		V3.y = y2;
		V3.z = z2;	
	}
}
G3DPrimitive;

class G3DObject 
{
public  :

	typedef std::vector<G3DPrimitive *> G3DPrimitives;

private :

	int m_camera_x,
		m_camera_y,
		m_camera_z;

	G3DPrimitives m_primitives;

public  :

	G3DObject();
		
	void add_primitive( double x0, double y0, double z0, double x1, double y1, double z1, double x2, double y2, double z2 );
	void add_primitive( GVertex& v1, GVertex& v2, GVertex& v3 );
	
	G3DPrimitive& operator[] ( int idx );

	void create( int size, ... );

	void move_camera_x( int n );
	void move_camera_y( int n );
	void move_camera_z( int n );

	void Z_sort_primitives();

	GPoint project_vertex( GVertex& v, double X0, double Y0, double Z0, double focale = 900 );

	G2DPrimitive& project_primitive( GVertex& v1, GVertex& v2, GVertex& v3 );
	G2DPrimitive& project_primitive( G3DPrimitive& P3D );

	void render( GGraphicalContext& ggc );
	void render( GGraphicalContext* ggc );
};

class GFrameBuffer {
private :
	GWindow       *m_window;

	unsigned int   m_bpp,
				   m_width,
				   m_height;
				   
	unsigned int   m_size;
				   
	XImage        *m_ximage;
	unsigned char *m_data;
public  :
	GFrameBuffer( GWindow *window,
				  unsigned int bpp, 
				  unsigned int width, 
				  unsigned int height,
				  Display *display,
				  Visual  *visual,
				  unsigned int depth );
				  
	~GFrameBuffer();
	
	void update();
				   
	XImage        *ximage();
	unsigned char *data();
	
	GFrameBuffer& set( unsigned int x, unsigned int y, unsigned long color );
	unsigned long get( unsigned int x, unsigned int y );
	
	GFrameBuffer& line( int x0, int y0, int x1, int y1, unsigned long color );
	GFrameBuffer& primitive( G2DPrimitive& p, unsigned long color );
	GFrameBuffer& render( G3DObject& o );
	GFrameBuffer& clear();
};



class GEventHandler {
public :
	
	GGraphicalContext *GCC;
	
	virtual void OnPaint(){}
	virtual void OnClose(){}
	virtual void OnMouseMove( unsigned int x, unsigned int y ){}
	virtual void OnMouseDown( unsigned int button, unsigned int x, unsigned int y );
	virtual void OnMouseUp( unsigned int button, unsigned int x, unsigned int y );
	virtual void OnInitialize(){}
	virtual void OnKeyDown( unsigned int key, unsigned int mask ){}
	virtual void OnKeyUp( unsigned int key, unsigned int mask ){}
};



class GWindow {
private :
	Display           *m_display;
    Window             m_window;
    GC                 m_gc;
	XWindowAttributes  m_wattr;

	GGraphicalContext *m_ggc;
	
	unsigned int       m_width,
				       m_height;
	char               m_title[0xFF];
	
	GEventHandler     *m_handler;
	
	static void * event_dispatcher( void *arg );
	
public  :

	GWindow( const char *title, unsigned int width, unsigned int height, GEventHandler *handler );
	
	~GWindow();
	
	void update( unsigned int x = 0, unsigned int y = 0 );
};

}

#endif 

