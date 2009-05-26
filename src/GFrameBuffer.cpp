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

GFrameBuffer::GFrameBuffer( GWindow *window,
						    unsigned int bpp, 
				  			unsigned int width, 
				  			unsigned int height,
				  			Display *display,
				  			Visual  *visual,
				  			unsigned int depth ){
	m_window = window;
	m_bpp    = bpp;
	m_width  = width;
	m_height = height;		
	m_size   = m_width * m_height * m_bpp;
	m_data   = new unsigned char[ m_size ];
	m_ximage = XCreateImage( display, 
							 visual, 
							 depth, 
							 ZPixmap, 
							 0,
							 (char *)m_data, 
							 m_width, 
							 m_height, 
							 8,
							 /* let X guess scanline size */ 
							 0 );
										 
	if( XInitImage(m_ximage) == 0 ){
		throw new GException( "Could not initialize frame buffer ximage ." );	
	}						
}

GFrameBuffer::~GFrameBuffer(){
	XFree(m_ximage);	
}

void GFrameBuffer::update(){
	m_window->update();	
}


XImage * GFrameBuffer::ximage(){
	return m_ximage;	
}

unsigned char *GFrameBuffer::data(){
	return m_data;	
}

GFrameBuffer& GFrameBuffer::set( unsigned int x, unsigned int y, unsigned long color ){
	memcpy( &m_data[ (x + m_width * y) * m_bpp ], &color, 3 );
	return *this;
}

unsigned long GFrameBuffer::get( unsigned int x, unsigned int y ){
	return m_data[ (x + m_width * y) * m_bpp ];
}

GFrameBuffer& GFrameBuffer::line( int x0, int y0, int x1, int y1, unsigned long color ){
	#define sgn(n) ( (n<0) ? -1 :( (n>0) ? 1 : 0 ) )
	
	int i,
		dx    = x1 - x0,	   /* the horizontal distance of the line */
		dy    = y1 - y0,	   /* the vertical distance of the line */
		sdx   = sgn(dx),
		sdy   = sgn(dy),
		dxabs = abs(dx),
		dyabs = abs(dy),
		x	  = dyabs >> 1,
		y	  = dxabs >> 1,
		px    = x0,
		py    = y0;

	if( dxabs >= dyabs ) { /* the line is more horizontal than vertical */
		for( i = 0; i < dxabs; i++ ) {
			y += dyabs;
			if (y >= dxabs) {
				y  -= dxabs;
				py += sdy;
			}
			px += sdx;
			memcpy( &m_data[ (px + m_width * py) * m_bpp ], &color, 3 );
		}
	}
	else { /* the line is more vertical than horizontal */
		for( i = 0; i < dyabs; i++ ) {
			x += dxabs;
			if( x >= dyabs ) {
				x  -= dyabs;
				px += sdx;
			}
			py += sdy;
			memcpy( &m_data[ (px + m_width * py) * m_bpp ], &color, 3 );
		}
	}

	return *this;	
}

GFrameBuffer& GFrameBuffer::primitive( G2DPrimitive& p, unsigned long color ){
	line( p.P1.x, p.P1.y, p.P2.x, p.P2.y, color );
	line( p.P2.x, p.P2.y, p.P3.x, p.P3.y, color );
	line( p.P3.x, p.P3.y, p.P1.x, p.P1.y, color );
		
	return *this;
}

GFrameBuffer& GFrameBuffer::render( G3DObject& o ){
	o.render(this);
	return *this;	
}

GFrameBuffer& GFrameBuffer::clear(){
	memset( m_data, 0x00, m_size );
	return *this;	
}

}

