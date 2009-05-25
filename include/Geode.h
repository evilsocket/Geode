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
#pragma once

#include "bmp/bmp.h"

#include <math.h>
#include <stdarg.h>
#include <stdio.h>
#include <exception>
#include <memory.h>
#include <vector>
#include <algorithm>

using namespace std;


namespace GEngine
{

//#define GEODE_DEBUG

#ifdef GEODE_DEBUG
#	define geode_trace printf
#else
#	define geode_trace //
#endif

typedef unsigned char gcolor_t;

/* 
	Macros to define a 4 byte depth rgb color by its
	rgb values
*/
#define GEODE_RGB(r,g,b)		       { b, g, r, 0x00 }
#define GEODE_DEFINE_COLOR(name,r,g,b) gcolor_t name[] = GEODE_RGB(r,g,b)

static GEODE_DEFINE_COLOR( RED  , 0xFF, 0x00, 0x00 );
static GEODE_DEFINE_COLOR( GREEN, 0x00, 0xFF, 0x00 );
static GEODE_DEFINE_COLOR( BLUE , 0x00, 0x00, 0xFF );
static GEODE_DEFINE_COLOR( WHITE, 0xFF, 0xFF, 0xFF );
static GEODE_DEFINE_COLOR( BLACK, 0x00, 0x00, 0x00 );

/*
	Return the number of the font resource to load
	(see 'resource.h') by the character ascii code
*/
#define GEODE_CHAR_TO_FONT(c)		   101 + (c - 32)

#define GEODE_MAX(a,b,c) (a>b?(a>c?a:c):(b>c?b:c))
#define GEODE_MIN(a,b,c) (a<b?(a<c?a:c):(b<c?b:c))

#define GK_ESC        27
#define GK_F1         112
#define GK_F2         113
#define GK_F3         114
#define GK_F4         115
#define GK_F5         116
#define GK_F6         117
#define GK_F7         118
#define GK_F8         119
#define GK_F9         120
#define GK_F10        121
#define GK_F11        122
#define GK_F12        123
#define GK_PAUSE      19
#define GK_1          49
#define GK_2          50
#define GK_3          51
#define GK_4          52
#define GK_5          53
#define GK_6          54
#define GK_7          55
#define GK_8          56
#define GK_9          57
#define GK_0          48
#define GK_BACKSPACE  8
#define GK_INSERT     45
#define GK_HOME       36
#define GK_PAGEUP     33
#define GK_TAB        9
#define GK_Q          81
#define GK_W          87
#define GK_E          69
#define GK_R          82
#define GK_T          84
#define GK_Y          89
#define GK_U          85
#define GK_I          73
#define GK_O          79
#define GK_P          80
#define GK_DELETE     8
#define GK_END        35
#define GK_PAGEDOWN   34
#define GK_CAPSLOCK   20
#define GK_A          65
#define GK_S          83
#define GK_D          68
#define GK_F          70
#define GK_G          71
#define GK_H          72
#define GK_J          74
#define GK_K          75
#define GK_L          76
#define GK_ENTER      13
#define GK_SHIFTLEFT  16
#define GK_Z          90
#define GK_X          88
#define GK_C          67
#define GK_V          86
#define GK_B          66
#define GK_N          78
#define GK_M          77
#define GK_SHIFTRIGHT 16
#define GK_CTRLLEFT   17
#define GK_APPLEFT    91
#define GK_SPACE      32
#define GK_ALTGR      17
#define GK_APPRIGHT   92
#define GK_MENU       93
#define GK_CTRLRIGHT  17
#define GK_ARROWLEFT  37
#define GK_ARROWUP    38
#define GK_ARROWRIGHT 39
#define GK_ARROWDOWN  40


#define G_PI 3.14159265358979323846


class GeodeFrameBuffer;

typedef GeodeFrameBuffer GeodeMatrix;


class GeodePoint
{
	public :

		int x;
		int y;

		GeodePoint( int x, int y );
		GeodePoint();
		GeodePoint( GeodePoint& p );

		GeodePoint& operator = ( GeodePoint& p );
};

class GeodeVertex
{
	public :

		int x;
		int y;
		int z;

		GeodeVertex( int x, int y, int z );
		GeodeVertex();
		GeodeVertex( GeodeVertex& v );
};

class GeodePrimitive2D
{
	public :

		GeodePoint p1,
				   p2,
				   p3;

		GeodePrimitive2D& operator = ( GeodePrimitive2D& P2D );
};

class GeodePrimitive3D
{	
	public :
	
		GeodeVertex v1, 
					v2, 
					v3;

		GeodePrimitive3D( GeodeVertex& a, GeodeVertex& b, GeodeVertex& c );
		GeodePrimitive3D();
		GeodePrimitive3D( int x0, int y0, int z0, int x1, int y1, int z1, int x2, int y2, int z2 );
};

/*
	Simple mathematical functions wrapper class
*/
class GeodeMath
{
	public :
		
		// absolute value of n
		static int		   abs				  ( int n );
		// return -1 if n is negative, 0 if n is 0 or 1 if n is positive
		static int		   sgn				  ( int n );
		// compute nex y coordinate of a circle
		static int		   circle_next_y      ( int x, int r_2, int prev_y );
		// convert degrees value to radians
		static double	   degrees_to_radians ( double a );
		// convert radians value to degrees
		static double	   radians_to_degrees ( double r );
		// linear distance between two points
		static int		   pythagore		  ( int x0, int y0, int x1, int y1 );
		// round dbl to the nearest integer
		static int		   normalize		  ( double dbl );
		// compose a mxn matrix by its elements
		static GeodeMatrix matrix			  ( int m, int n, gcolor_t v1, ... );
		// compose a 1xn vector by its elements
		static GeodeMatrix vector			  ( int n, gcolor_t v1, ... );
};

/*
	3D space mathematical functions wrapper class
*/
class Geode3DMath
{
	public :
		
		// return a rotation matrix along the x,y,z axis
		static GeodeMatrix rotation_matrix( int x, int y, int z, double angle );
		// return the distance vector
		static GeodeMatrix distance_vector( int dx, int dy, int dz );
};

/*
	Geode engine standard exception
*/
class GeodeException
{
	private :

		int  m_error_code;			// returned from GetLastError()
		char m_error_message[0xFF]; // exception message

	public  :

		GeodeException( int error_code, char * msgFormat, ... );
		GeodeException( char * msgFormat, ... );

		int	   code();
		char * message();
};

/*
	Geode engine hardware managment class
*/
class GeodeHardware
{
	public :

		// convert a virtual key to its ascii code
		static char   vkeytoascii( int vkey, int scancode );
		// change the scree resolution
		static void   set_screen_resolution( int width, int height );
		// get the in-use screen name
		static char * get_screen_name();
		// get the in-use screen bit depth
		static int	  get_screen_depth();
		// get the in-use screen horizontal resolution
		static int    get_screen_width();
		// get the in-use screen vertical resolution
		static int    get_screen_height();
};

/*
	Fast matrix computations and drawing class
*/
class GeodeFrameBuffer
{
	private :

		int m_width,	// buffer width
			m_height,   // buffer height
			m_depth,    // buffer depth (usually 4 bytes)
			m_size;     // buffer total size

		gcolor_t * m_data_ptr; // buffer pointer

		bool m_shared_data; // is a shared pointer ?

		// x-y to linear offset
		int offset( int x, int y );
		// x-y-color to linear offset
		int offset( int x, int y, int color );

		int offset( int x , int y , int z , int v );

		// check the offset 
		bool is_valid_offset( int offset );
		
		void set_color( int x, int y,gcolor_t * color );

	public  :

		typedef gcolor_t * iterator;

		GeodeFrameBuffer( int width, int heigth, int depth = 3 );
		GeodeFrameBuffer( GeodeFrameBuffer& gFrameBuffer );
		~GeodeFrameBuffer();

		iterator begin();
		iterator end();

		void swap( GeodeFrameBuffer& gFrameBuffer );

		void load_bmp( const char * filename, bool resize = true, int x0 = 0, int y0 = 0 );

		void load_font_resource( int index, HMODULE hmod = 0, int w = 8, int h = 13, int d = 3 );

		gcolor_t&         operator () ( int x, int y, int color );
		gcolor_t&         operator () ( int x , int y , int z , int v );
		gcolor_t&         operator [] ( int offset );
		GeodeFrameBuffer& operator =  ( GeodeFrameBuffer& gFrameBuffer );
		GeodeFrameBuffer& operator *= ( GeodeFrameBuffer& gFrameBuffer );
		GeodeFrameBuffer  operator *  ( GeodeFrameBuffer& gFrameBuffer );
		GeodeFrameBuffer& operator *= ( double d );
		GeodeFrameBuffer  operator *  ( double d );
		GeodeFrameBuffer& operator += ( GeodeFrameBuffer& gFrameBuffer );
		GeodeFrameBuffer  operator +  ( GeodeFrameBuffer& gFrameBuffer );
		GeodeFrameBuffer& operator += ( gcolor_t uc );
		GeodeFrameBuffer  operator +  ( gcolor_t uc );

		inline int			   width  ();
		inline int			   height ();
		inline int			   depth  ();
		inline int			   size   ();
		inline gcolor_t *      data   ();
		inline bool			   shared ();

		GeodeFrameBuffer& line	    ( int x0, int y0, int x1, int y1, gcolor_t * color );
		GeodeFrameBuffer& rectangle ( int x0, int y0, int x1, int y1, gcolor_t * color );
		GeodeFrameBuffer& fill      ( int x0, int y0, int x1, int y1, gcolor_t * color );
		GeodeFrameBuffer& fill      ( gcolor_t * color );
		GeodeFrameBuffer& clear     ();
		GeodeFrameBuffer& circle    ( int x , int y,  int r,  gcolor_t * color );		

		GeodeFrameBuffer& primitive	( GeodePrimitive2D& pr, gcolor_t * color );

		GeodeFrameBuffer& text		( int x, int y, gcolor_t * fcolor, gcolor_t * bcolor, char * text );
		GeodeFrameBuffer& printf	( int x, int y, gcolor_t * fcolor, gcolor_t * bcolor, char * format, ... );
		GeodeFrameBuffer& printf	( int x, int y, char * format, ... );

		GeodeFrameBuffer& resize	( int width, int height, int depth );

		GeodeFrameBuffer  get_portion( int x0, int y0, int x1, int y1 );
		GeodeFrameBuffer& set_portion( int x0, int y0, int x1, int y1, GeodeFrameBuffer& gFrameBuffer );
		GeodeFrameBuffer& set_portion( int x0, int y0, GeodeFrameBuffer& gFrameBuffer );

		GeodeFrameBuffer& normalize( gcolor_t& a, gcolor_t& b );

		GeodeFrameBuffer& denoise();

		gcolor_t		  linear_interpolation( int x, int y, int color );
		gcolor_t *        linear_interpolation( int x, int y );
		GeodeFrameBuffer  linear_interpolation();
};

class GeodeGraphicalContext;

class GeodeObject
{
	public :

		virtual void render( GeodeGraphicalContext& ggc ) {}
		virtual void render( GeodeGraphicalContext* ggc ) {}
};

class Geode3DObject : public GeodeObject
{
	public  :
	
		typedef vector<GeodePrimitive3D *> primitive_vector_t;

	private :

		int m_camera_x,
		    m_camera_y,
		    m_camera_z;

		primitive_vector_t m_pvector;

	public  :

		Geode3DObject();
			
    	void add_primitive( int x0, int y0, int z0, int x1, int y1, int z1, int x2, int y2, int z2 );
		void add_primitive( GeodeVertex& v1, GeodeVertex& v2, GeodeVertex& v3 );
		
		GeodePrimitive3D& operator[] ( int idx );

		void create( int size, ... );

		void move_camera_x( int n );
		void move_camera_y( int n );
		void move_camera_z( int n );

		void Z_sort_primitives ();

		GeodePoint		  project_vertex    ( GeodeVertex& v, int X0, int Y0, int Z0, double focale = 900 );

		GeodePrimitive2D& project_primitive ( GeodeVertex& v1, GeodeVertex& v2, GeodeVertex& v3 );
		GeodePrimitive2D& project_primitive ( GeodePrimitive3D& P3D );

		void render( GeodeGraphicalContext& ggc );
		void render( GeodeGraphicalContext* ggc );

		void save( const char * szFileName );
};

class GeodeGraphicalContext
{
	private :

		int				   m_width,
						   m_height;

		char			   m_title[0xFF];

		HWND			   m_hwnd;
		HDC				   m_hdc;

		WNDPROC			   m_wndproc;

		HANDLE			   m_ready_event,
		  				   m_destroy_event;

		BITMAPINFO		   m_bmpinfo;

		GeodeFrameBuffer * m_buffer;

		static DWORD WINAPI msg_dispatch_thread( LPVOID lpParam );

	public  :

		GeodeGraphicalContext( const char * title, int width, int height, WNDPROC wndProc, int style = WS_OVERLAPPEDWINDOW );
		GeodeGraphicalContext( HWND hwnd );
		~GeodeGraphicalContext();

		int style;

		void allocate( int width, int height, int depth );

		int				   width();
		int				   height();
		HWND&			   hwnd();
		HDC&			   hdc();
		char *			   title();
		WNDPROC			   wndproc();
		HANDLE&			   ready_event();
		HANDLE&			   destroy_event();
		GeodeFrameBuffer * buffer();

		void render( GeodeObject& obj, gcolor_t * color );

		void update();
		//void update( int x0, int y0, int x1, int y1 );

		void wait( unsigned int ms = INFINITE );
		
		GeodeGraphicalContext& resize( int width, int height, bool redraw = true, bool resize_buffer = true );
};	

class GeodeWindow;

class GeodeEventReceiver
{
	public :

		GeodeGraphicalContext * GCC;
		GeodeWindow			  * WND;
		
		virtual void OnPaint(){}
		virtual void OnClose(){}
		virtual void OnMouseMove( int x, int y ){}
		virtual void OnLButtonDown(){}
		virtual void OnRButtonDown(){}
		virtual void OnMButtonDown(){}
		virtual void OnLButtonUp(){}
		virtual void OnRButtonUp(){}
		virtual void OnMButtonUp(){}
		virtual void OnInitialize(){}
		virtual void OnKeyDown( int vKey, int kFlags ){}
		virtual void OnKeyUp( int vKey, int kFlags ){}
		virtual void OnCommand( int wNotifyCode, int wID, HWND hwndCtl  ){}
};

class GeodeWindow
{
	private :

		int m_screen_res_x,
			m_screen_res_y;
	
		GeodeGraphicalContext * m_gcc;

		static LRESULT APIENTRY GeodeWindowProc( HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

	public  :

		int style;

		GeodeWindow( const char * title, bool fullscreen, int width, int height, GeodeEventReceiver * eventReceiver );
		GeodeWindow( GeodeEventReceiver * eventReceiver );
		GeodeWindow( GeodeEventReceiver * eventReceiver, int screen_res_x, int screen_res_y );
		~GeodeWindow();

		void close();
};

} // end of namespace GEngine
