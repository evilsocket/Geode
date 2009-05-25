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

LRESULT APIENTRY GeodeWindow::GeodeWindowProc( HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	GeodeEventReceiver * eReceiver = (GeodeEventReceiver *)GetWindowLong( hwnd, GWL_USERDATA );

	switch(uMsg) 
	{
		case WM_INITDIALOG :
			eReceiver->OnInitialize();
			return 1;
		break;

		case WM_CLOSE   :
		case WM_DESTROY :
			PostQuitMessage(0);
			eReceiver->OnClose();
		break;

		case WM_PAINT :
			eReceiver->GCC->update();
			eReceiver->OnPaint();
		break;
 
		case WM_MOUSEMOVE:
			eReceiver->OnMouseMove( LOWORD(lParam), HIWORD(lParam) );
		break;

		case WM_LBUTTONDOWN: 
			eReceiver->OnLButtonDown();
		break;

		case WM_RBUTTONDOWN:
			eReceiver->OnRButtonDown();
		break;

		case WM_MBUTTONDOWN:
			eReceiver->OnMButtonDown();
		break;

		case WM_LBUTTONUP:
			eReceiver->OnLButtonUp();
		break;

		case WM_RBUTTONUP:
			eReceiver->OnRButtonUp();
		break;

		case WM_MBUTTONUP:
			eReceiver->OnMButtonUp();
		break;

		case WM_KEYUP:
			eReceiver->OnKeyUp( wParam, lParam );
		break;

		case WM_KEYDOWN:
			eReceiver->OnKeyDown( wParam, lParam );
		break;

		case WM_COMMAND:
			eReceiver->OnCommand( HIWORD(wParam), LOWORD(wParam), (HWND)lParam );
		break;

		default : return DefWindowProc(hwnd,uMsg,wParam,lParam);
	}

	return 0;
}

GeodeWindow::GeodeWindow( const char * title, bool fullscreen, int width, int height, GeodeEventReceiver * eventReceiver )
{
	if( !eventReceiver )
	{
		throw new GeodeException( 0, "Cannot create GeodeWindow without a valid GeodeEventReceiver" );
		return;
	}

	int style = (fullscreen ? WS_POPUP : WS_OVERLAPPEDWINDOW);

	width  = (fullscreen ? GeodeHardware::get_screen_width()  : width);
	height = (fullscreen ? GeodeHardware::get_screen_height() : height);
	
	m_gcc = new GeodeGraphicalContext( title, width, height, GeodeWindowProc, style );

	::SetWindowLong( m_gcc->hwnd(), GWL_USERDATA, (LONG)eventReceiver );

	eventReceiver->GCC = m_gcc;
	eventReceiver->WND = this;

	m_gcc->wait();
}

GeodeWindow::GeodeWindow( GeodeEventReceiver * eventReceiver )
{
	GeodeWindow(0,true,0,0,eventReceiver);
}

GeodeWindow::GeodeWindow( GeodeEventReceiver * eventReceiver, int screen_res_x, int screen_res_y )
{
	m_screen_res_x = GeodeHardware::get_screen_width();
	m_screen_res_y = GeodeHardware::get_screen_height();

	GeodeHardware::set_screen_resolution(screen_res_x,screen_res_y);

	GeodeWindow(0,true,0,0,eventReceiver);
}

GeodeWindow::~GeodeWindow()
{
	if( GeodeHardware::get_screen_width()  != m_screen_res_x ||
		GeodeHardware::get_screen_height() != m_screen_res_y
	)
		GeodeHardware::set_screen_resolution(m_screen_res_x,m_screen_res_y);
}

void GeodeWindow::close()
{
	::SetEvent( m_gcc->destroy_event() );
}

}