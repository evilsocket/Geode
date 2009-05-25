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
