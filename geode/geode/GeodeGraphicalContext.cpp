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

DWORD WINAPI GeodeGraphicalContext::msg_dispatch_thread( LPVOID lpParam )
{
	geode_trace( "GeodeGraphicalContext::msg_dispatch_thread( 0x%X )\n", lpParam );

	GeodeGraphicalContext * pGGC = (GeodeGraphicalContext *)lpParam;

	CLIENTCREATESTRUCT ccs;

	pGGC->hwnd() = ::CreateWindowA( "MDICLIENT",
					  				 pGGC->title(),
									 pGGC->style | WS_VISIBLE ,
									 pGGC->width() / 2,
									 pGGC->height() / 2,
									 pGGC->width(),
									 pGGC->height(),
									 0,
									 0,
									 0,
									 &ccs
									);

	if( !pGGC->hwnd() )
	{
		throw new GeodeException( "Invalid window handle" );
		return 0;
	}

	pGGC->hdc() = ::GetDC(pGGC->hwnd());

	if( !pGGC->hdc() )
	{
		throw new GeodeException( "Invalid device context handle" );
		return 0;
	}

	pGGC->allocate( pGGC->width(), pGGC->height(), 4 );

	::ShowWindow( pGGC->hwnd(), SW_SHOW);
	::UpdateWindow( pGGC->hwnd() );

	::SetWindowLong( pGGC->hwnd(), GWL_WNDPROC, (LONG)pGGC->wndproc() );

	if( pGGC->style == WS_POPUP )
	{
		::SetWindowPos( pGGC->hwnd(), 0, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOZORDER );
	}

	if( ::SetEvent( pGGC->ready_event() ) == 0 )
	{
		throw new GeodeException( "Cannot set synchronization event" );
		return 0;
	}

	MSG msg;

	while( GetMessage(&msg,0,0,0) )
	{
		DispatchMessage ( &msg );
	}

	if( ::SetEvent( pGGC->destroy_event() ) == 0 )
	{
		throw new GeodeException( "Cannot set destroy event" );
		return 0;
	}

	return 0;
}

GeodeGraphicalContext::GeodeGraphicalContext( const char * title, int width, int height, WNDPROC wndProc, int style /*= WS_OVERLAPPEDWINDOW*/ )
{
	geode_trace( "GeodeGraphicalContext::GeodeGraphicalContext( %s, %d, %d, 0x%X, %d )\n", title, width, height, wndProc, style );
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

GeodeGraphicalContext::GeodeGraphicalContext( HWND hwnd )
{
	geode_trace( "GeodeGraphicalContext::GeodeGraphicalContext( 0x%X )\n", hwnd );

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

GeodeGraphicalContext::~GeodeGraphicalContext()
{
	geode_trace( "GeodeGraphicalContext::~GeodeGraphicalContext()\n" );

	delete m_buffer;
}

void GeodeGraphicalContext::allocate( int width, int height, int depth )
{
	geode_trace( "GeodeGraphicalContext::allocate( %d, %d, %d )\n", width, height, depth );

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
	geode_trace( "GeodeGraphicalContext::render( obj, color )\n" );

	obj.render(this);
}

void GeodeGraphicalContext::update()
{
	//geode_trace( "GeodeGraphicalContext::update()\n" );

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

void GeodeGraphicalContext::wait( unsigned int ms /*= INFINITE*/ )
{
	geode_trace( "GeodeGraphicalContext::wait( %d )\n", ms );

	::WaitForSingleObject( m_destroy_event, ms );
}

GeodeGraphicalContext& GeodeGraphicalContext::resize( int width, int height, bool redraw /*= true*/, bool resize_buffer /*= true*/ )
{
	geode_trace( "GeodeGraphicalContext::resize( %d, %d, %d, %d )\n", width, height, redraw, resize_buffer );

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