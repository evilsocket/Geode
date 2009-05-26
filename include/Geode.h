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

namespace GEngine {

//#define GEODE_DEBUG
#ifdef GEODE_DEBUG
# define G_TRACE printf
#else
# define G_TRACE //
#endif

typedef std::string GException;

typedef struct {
	XImage        *image;
	unsigned char *data;
}
GFrameBuffer;

class GWindow; 

class GEventHandler {
public :
	
	GFrameBuffer *buffer;
	GWindow      *window;
	
	virtual void OnPaint( GFrameBuffer *buffer ){}
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
	Display          *m_display;
    Window            m_window;
    GC                m_gc;
	XWindowAttributes m_wattr;
 	XColor            m_pallete[256];
	
	GFrameBuffer	  m_frame_buffer;
	
	unsigned int      m_width,
				      m_height;
	char              m_title[0xFF];
	
	GEventHandler    *m_handler;
	
	static void * event_dispatcher( void *arg );
	
public  :

	GWindow( const char *title, unsigned int width, unsigned int height, GEventHandler *handler );
	
	void update( unsigned int x = 0, unsigned int y = 0 );
};

}

#endif 

