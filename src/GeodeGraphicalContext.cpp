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

void * GeodeGraphicalContext::msg_dispatch_thread( void *arg ){
	g_trace( "GeodeGraphicalContext::msg_dispatch_thread( 0x%X )\n", arg );
	
	GeodeGraphicalContext * ggc = (GeodeGraphicalContext *)arg;
	// see reference http://svn.python.org/projects/external/tk-8.5.2.0/xlib/X11/X.h
	// see reference http://tronche.com/gui/x/xlib/event-handling/manipulating-event-queue/XCheckWindowEvent.html
	long eventmask = ExposureMask        | KeyPressMask    |
				  	 PointerMotionMask   | ButtonPressMask | 
				  	 StructureNotifyMask | SubstructureRedirectMask | 
				  	 VisibilityChangeMask;
	XEvent event;	 
	
	
	XSelectInput( gcc->m_display, gcc->m_window, eventmask );
	while(1){
		// query X11 window event
		XCheckWindowEvent( gcc->m_display, gcc->m_window, eventmask, &event );

		switch( event.type ){
			case CreateNotify : 
				gcc->m_event_reciever->OnInitialize();
			break;
			
			case DestroyNotify : 
				gcc->m_event_reciever->OnClose();
			break; 
			
			case KeyPress   : 
			case KeyRelease : 
				if( event.xkey.type == KeyPress ){
					gcc->m_event_reciever->OnKeyDown( event.xkey.keycode, event.xkey.state );
				}
				else{
					gcc->m_event_reciever->OnKeyUp( event.xkey.keycode, event.xkey.state );
				}
			break;
			
			case ButtonPress   : 
			case ButtonRelease : 
				if( event.xbutton.type == ButtonPress ){
					gcc->m_event_reciever->OnMouseDown( event.xbutton.x, event.xbutton.y, event.xbutton.button );
				}
				else{
					gcc->m_event_reciever->OnMouseUp( event.xbutton.x, event.xbutton.y, event.xbutton.button );
				}
			break;
			
			case MotionNotify : 
				gcc->m_event_reciever->OnMouseMove( event.xmotion.x, event.xmotion.y );
			break;
			
			case EnterNotify : break;
			case LeaveNotify : break;
			case FocusIn : break;
			case FocusOut : break;
			case KeymapNotify : break;
			case Expose : break;
			case GraphicsExpose : break;
			case NoExpose : break;
			case VisibilityNotify : break;
			case UnmapNotify : break;
			case MapNotify : break;
			case MapRequest : break;
			case ReparentNotify : break;
			case ConfigureNotify : break;
			case ConfigureRequest : break;
			case GravityNotify : break;
			case ResizeRequest : break;
			case CirculateNotify : break;
			case CirculateRequest : break;
			case PropertyNotify : break;
			case SelectionClear : break;
			case SelectionRequest : break;
			case SelectionNotify : break;
			case ColormapNotify : break;
			case ClientMessage : break;
			case MappingNotify : break;
		}
		
		// TODO : Add poll delay ?
	}
}

GeodeGraphicalContext::GeodeGraphicalContext( const char * title, int width, int height, GeodeEventReceiver *eventReciever ){
	g_trace( "GeodeGraphicalContext::GeodeGraphicalContext( %s, %d, %d, 0x%X )\n", title, width, height, eventReciever );
	
	char *fontname;
	XGCValues gcv;
	XSizeHints hints;
	XColor xcolor;
	Cursor cursor;
	
	strcpy( m_title, title ? title : "Geode Graphical Context Display" );
	m_width          = width;
	m_height         = height;
	m_event_reciever = eventReciever;
	
	if( (m_display = XOpenDisplay(NULL)) == NULL ){
		throw new GeodeException( "Cannot open default X display" );
		return;
	}
	
	if( (fontname = XGetDefault( m_display, "", "font")) == NULL ){
		fontname = "fixed";	
	}
	if( (m_font = XLoadQueryFont( m_display, fontname ) ) == NULL ){
		throw new GeodeException( "Cannot load default font" );
		return;
	}
	
	m_white = WhitePixel( m_display, DefaultScreen(m_display) );
	m_black = BlackPixel( m_display, DefaultScreen(m_display) );
	
	hints.flags       = (USPosition | PSize | PMinSize | PMaxSize);
	hints.height      = height;
	hints.width       = width;
	hints.base_height = height;
	hints.base_width  = width;
	hints.min_height  = height;
	hints.min_width   = width;
	hints.max_height  = height;
	hints.max_width   = width;
	// TODO: Get x-y position from args
	hints.x           = 0;
	hints.y           = 0;

	m_window   = XCreateSimpleWindow( m_display, RootWindow(m_display,0), 0, 0, width, height, 1, m_black, m_ white );
	m_colormap = DefaultColormap( m_display, DefaultScreen(m_display) );
		
	XSetNormalHints( m_display, m_window, &hints );
	XStoreName( m_display, m_window, title );
	XSynchronize( m_display, 1 );

	m_pixmap = XCreatePixmap( m_display, m_window, m_width, m_height,
							  DefaultDepth( m_display, DefaultScreen(m_display) );
	m_gc     = XCreateGC( m_display, m_pixmap, 0, (XGCValues *)0 );						  
	cursor   = XCreateFontCursor( m_display, XC_crosshair );
	XDefineCursor( m_display, m_window, cursor );
	
	XSelectInput( m_display, m_window, ExposureMask | KeyPressMask | ButtonPressMask | ButtonReleaseMask | PointerMotionHintMask );
	
	XMapRaised( m_display, m_window );
	XSetWindowBackgroundPixmap( m_display, m_window, m_pixmap );

	pthread_t tid;
	pthread_create( &tid, NULL, msg_dispatch_thread, (void *)this );
}

GeodeGraphicalContext::GeodeGraphicalContext( const char * title, int width, int height, WNDPROC wndProc, int style /*= WS_OVERLAPPEDWINDOW*/ )
{
	g_trace( "GeodeGraphicalContext::GeodeGraphicalContext( %s, %d, %d, 0x%X, %d )\n", title, width, height, wndProc, style );
	this->style = style;

	m_ready_event   = ::CreateEvent( 0, FALSE, FALSE, 0 );
	m_destroy_event = ::CreateEvent( 0, FALSE, FALSE, 0 );

	if( m_ready_event == INVALID_HANDLE_VALUE )
	{
		throw new GeodeException( "Cannot create synchronization event" );
		return;
	}

	strcpy( m_title, title ? title : "Geode Graphical Context Display" );

	m_width   = width;
	m_height  = height;
	m_wndproc = wndProc;
 
	m_bmpinfo.bmiHeader.biSize		    = sizeof(BITMAPINFOHEADER);
	m_bmpinfo.bmiHeader.biWidth		    = m_width;
	m_bmpinfo.bmiHeader.biHeight	    = -(int)m_height;
	m_bmpinfo.bmiHeader.biPlanes	    = 1;
	m_bmpinfo.bmiHeader.biBitCount	    = 32;
	m_bmpinfo.bmiHeader.biCompression   = BI_RGB;
	m_bmpinfo.bmiHeader.biSizeImage	    = 0;
	m_bmpinfo.bmiHeader.biXPelsPerMeter = 1;
	m_bmpinfo.bmiHeader.biYPelsPerMeter = 1;
	m_bmpinfo.bmiHeader.biClrUsed	    = 0;
	m_bmpinfo.bmiHeader.biClrImportant  = 0;

	::CreateThread( 0, 0, msg_dispatch_thread, (LPVOID)this, 0, 0 );

	::WaitForSingleObject( m_ready_event, INFINITE );
}
/*
GeodeGraphicalContext::GeodeGraphicalContext( HWND hwnd )
{
	g_trace( "GeodeGraphicalContext::GeodeGraphicalContext( 0x%X )\n", hwnd );

	if( !hwnd )
	{
		throw new GeodeException( "Invalid window handle" );
		return;
	}

	m_hwnd = hwnd;
	m_hdc  = ::GetDC(m_hwnd);

	if( !m_hdc )
	{
		throw new GeodeException( "Invalid device context handle" );
		return;
	}

	::GetWindowTextA( m_hwnd, m_title, 0xFF );

	m_wndproc = NULL;

	RECT r;

	::GetWindowRect( m_hwnd, &r );

	m_width  = r.right  - r.left;
	m_height = r.bottom - r.top;

	m_buffer = new GeodeFrameBuffer( m_width, m_height, 4 );

	m_bmpinfo.bmiHeader.biSize		    = sizeof(BITMAPINFOHEADER);
	m_bmpinfo.bmiHeader.biWidth		    = m_width;
	m_bmpinfo.bmiHeader.biHeight		= -(int)m_height;
	m_bmpinfo.bmiHeader.biPlanes		= 1;
	m_bmpinfo.bmiHeader.biBitCount	    = 32;
	m_bmpinfo.bmiHeader.biCompression   = BI_RGB;
	m_bmpinfo.bmiHeader.biSizeImage	    = 0;
	m_bmpinfo.bmiHeader.biXPelsPerMeter = 1;
	m_bmpinfo.bmiHeader.biYPelsPerMeter = 1;
	m_bmpinfo.bmiHeader.biClrUsed	    = 0;
	m_bmpinfo.bmiHeader.biClrImportant  = 0;
}
*/

GeodeGraphicalContext::~GeodeGraphicalContext()
{
	g_trace( "GeodeGraphicalContext::~GeodeGraphicalContext()\n" );

	delete m_buffer;
}

void GeodeGraphicalContext::allocate( int width, int height, int depth )
{
	g_trace( "GeodeGraphicalContext::allocate( %d, %d, %d )\n", width, height, depth );

	m_buffer = new GeodeFrameBuffer( width, height, depth );
}

inline int GeodeGraphicalContext::width()
{ 
	return m_width; 
}

inline int GeodeGraphicalContext::height()
{ 
	return m_height; 
}

inline HWND& GeodeGraphicalContext::hwnd()
{ 
	return m_hwnd;  
}

inline HDC& GeodeGraphicalContext::hdc()
{ 
	return m_hdc; 
}

inline char * GeodeGraphicalContext::title()
{ 
	return m_title; 
}

inline WNDPROC GeodeGraphicalContext::wndproc() 
{ 
	return m_wndproc; 
}

inline HANDLE& GeodeGraphicalContext::ready_event()
{
	return m_ready_event;
}

inline HANDLE& GeodeGraphicalContext::destroy_event()
{
	return m_destroy_event;
}

inline GeodeFrameBuffer * GeodeGraphicalContext::buffer()
{
	return m_buffer;
}

void GeodeGraphicalContext::render( GeodeObject& obj, gcolor_t * color )
{
	g_trace( "GeodeGraphicalContext::render( obj, color )\n" );

	obj.render(this);
}

void GeodeGraphicalContext::update()
{
	//g_trace( "GeodeGraphicalContext::update()\n" );

	int ret;

	ret = ::SetDIBitsToDevice( m_hdc,
							   0,
							   0,
							   m_width,
							   m_height,
							   0,
							   0,
							   0,
							   m_height,
							   m_buffer->data(),
							   &m_bmpinfo,
							   DIB_RGB_COLORS
							 );

	if( ret == 0 )
	{
		throw new GeodeException( "Error during frame buffer rendering" );
	}
}

void GeodeGraphicalContext::wait( unsigned int ms ) {
	g_trace( "GeodeGraphicalContext::wait( %d )\n", ms );
	sleep(ms);
}

GeodeGraphicalContext& GeodeGraphicalContext::resize( int width, int height, bool redraw /*= true*/, bool resize_buffer /*= true*/ )
{
	g_trace( "GeodeGraphicalContext::resize( %d, %d, %d, %d )\n", width, height, redraw, resize_buffer );

	m_width  = width;
	m_height = height;

	::SetWindowPos( m_hwnd,
					0,
					0,
					0,
					width,
					height,
					SWP_NOMOVE | SWP_NOZORDER | SWP_NOCOPYBITS
				  );

	if( resize_buffer )
	{
		GeodeFrameBuffer frame( width, height, 4 );

		buffer()->swap(frame);
	}

	if( redraw )
		update();

	return *this;
}

}
