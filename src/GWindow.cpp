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

namespace GEngine {

void * GWindow::event_dispatcher( void *arg ){ 
	G_TRACE( "GWindow::event_dispatcher( %p )\n", arg );
	
	GWindow *wnd = (GWindow *)arg;
	XEvent   event;

	while(1){
        /* get the next event - we receive only events we set the mask for */
        XNextEvent( wnd->m_display, &event );
		switch(event.type) {
			case CreateNotify : 
				wnd->m_handler->OnInitialize();
			break;
			
			case DestroyNotify : 
				wnd->m_handler->OnClose();
			break; 
			
			case Expose :
        		if( event.xexpose.count == 0 ) {
					wnd->update( event.xexpose.x, event.xexpose.y );
        		}
			break;
			
			case KeyPress   : 
			case KeyRelease : 
				if( event.xkey.type == KeyPress ){
					wnd->m_handler->OnKeyDown( event.xkey.keycode, event.xkey.state );
				}
				else{
					wnd->m_handler->OnKeyUp( event.xkey.keycode, event.xkey.state );
				}
			break;
			
			case ButtonPress   : 
			case ButtonRelease : 
				if( event.xbutton.type == ButtonPress ){
					wnd->m_handler->OnMouseDown( event.xbutton.x, event.xbutton.y, event.xbutton.button );
				}
				else{
					wnd->m_handler->OnMouseUp( event.xbutton.x, event.xbutton.y, event.xbutton.button );
				}
			break;
			
			case MotionNotify : 
				wnd->m_handler->OnMouseMove( event.xmotion.x, event.xmotion.y );
			break;
        }
    }
	
	return NULL;
}

GWindow::GWindow( const char *title, unsigned int width, unsigned int height, GEventHandler *handler ){
	strncpy( m_title, (title ? title : "Geode"), 0xFF );
	m_width   = width;
	m_height  = height;
	m_handler = handler;	
	
	/* connect to XServer (default display) */
    if( (m_display = XOpenDisplay(NULL)) == NULL ){
		throw new GException( "Could not connect to X server ." );
    }

	/* get screen capabilities */
    unsigned int screen  = DefaultScreen(m_display);

	/* create the window */
	m_window = XCreateSimpleWindow( m_display, 
									RootWindow( m_display, screen ),
                               		0 /* X */, 
									0 /* Y */, 
									m_width, 
									m_height, 
									0,
                               		BlackPixel( m_display, screen ), 
									WhitePixel( m_display, screen ) );
	
	/* set window size properties */
	XSizeHints xsizeh;
	
	xsizeh.flags      = USPosition | USSize;
    xsizeh.x          = 0;
    xsizeh.y          = 0;
    xsizeh.width      = m_width;
    xsizeh.height     = m_height;
    xsizeh.min_width  = m_width;
    xsizeh.min_height = m_height;
    xsizeh.max_width  = m_width;
    xsizeh.max_height = m_height;
	
	XSetNormalHints( m_display, m_window, &xsizeh );
	XSetWMSizeHints( m_display, m_window, &xsizeh, PSize | PMinSize | PMaxSize );
    XStoreName( m_display, m_window, m_title );

	/* create graphics context */
	XGCValues     values;
	unsigned long vmask = 0;
	
    XGetWindowAttributes( m_display, m_window, &m_wattr );
    m_gc = XCreateGC( m_display, m_window, vmask, &values );
    XSetBackground( m_display, m_gc, WhitePixel( m_display, screen ) );
    XSetForeground( m_display, m_gc, BlackPixel( m_display, screen ) );
    XSetLineAttributes( m_display, m_gc, 1, LineSolid, CapRound, JoinRound );
	
	XSetWindowAttributes attr[1];
    attr[0].backing_store  = Always;
    attr[0].backing_planes = 1;
    attr[0].backing_pixel  = BlackPixel( m_display, screen );
    XChangeWindowAttributes( m_display, m_window, CWBackingStore | CWBackingPlanes | CWBackingPixel, attr );

	/* Select input methods */
	long eventmask = ExposureMask        | 
					 KeyPressMask        | KeyReleaseMask    |
					 ButtonPressMask     | ButtonReleaseMask |
				  	 PointerMotionMask   | StructureNotifyMask | 
					 SubstructureRedirectMask | VisibilityChangeMask;
	XSelectInput( m_display, m_window, eventmask );

	/* initialize the XImage */
	unsigned int bpp; 
    switch( m_wattr.depth ){
    	case 8:
        	bpp = 1;
        break;

    	case 16:
        	bpp = 2;
        break;

    	case 24:
    	case 32:
        	bpp = 4;
        break;

    	default:
        	throw new GException( "Unsupported display depth ." );
    }

	m_ggc = new GFrameBuffer( this,
									   bpp, 
				  					   m_width, 
				  					   m_height,
				  					   m_display,
				  					   m_wattr.visual,
				  					   m_wattr.depth );
									   
	m_handler->GCC = m_ggc;
	
	/* display the window */
    XMapWindow( m_display, m_window );
    XSync( m_display, 0 );

	pthread_t tid;
	pthread_create( &tid, NULL, event_dispatcher, (void *)this );
}

GWindow::~GWindow(){
	delete m_ggc;
}

void GWindow::update( unsigned int x /*= 0*/, unsigned int y /*= 0*/ ){
	m_handler->OnPaint();
	XPutImage( m_display, 
			   m_window, 
			   m_gc, 
			   m_ggc->ximage(),
			   x, 
			   y,
			   x, 
			   y,
			   m_width, 
			   m_height );
}

}

