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

inline int GeodeFrameBuffer::offset( int x, int y )
{ 
	return (x + m_width * y) * m_depth; 
}

inline int GeodeFrameBuffer::offset( int x, int y, int color )
{ 
	return color + offset(x,y); 
}

inline int GeodeFrameBuffer::offset( int x, int y, int z, int v )
{
	return x + y*m_width + z*m_width*m_height + v*m_width*m_height*m_depth;
}

inline bool GeodeFrameBuffer::is_valid_offset( int offset )
{
	return (offset < m_size && offset >= 0);
}

inline void GeodeFrameBuffer::set_color( int x, int y,gcolor_t * color )
{
	int off = offset(x,y);

	if( is_valid_offset(off) )
		memcpy( m_data_ptr + off, color, m_depth );
}

GeodeFrameBuffer::GeodeFrameBuffer( int width, int heigth, int depth /* = 3 */ )
{
	m_width       = width;
	m_height      = heigth;
	m_depth       = depth;
	m_size        = m_width * m_height * depth * sizeof(gcolor_t);
	m_data_ptr    = (gcolor_t *)malloc( m_size );
	m_shared_data = false;
}

GeodeFrameBuffer::GeodeFrameBuffer( GeodeFrameBuffer& gFrameBuffer ) 
{
	m_width       = 0;
	m_height      = 0;
	m_depth       = 0;
	m_size        = 0;
	m_data_ptr    = 0;
	m_shared_data = false;

	swap(gFrameBuffer);
}

GeodeFrameBuffer::~GeodeFrameBuffer()
{
	if( !m_shared_data )
		if( m_size > 3 )
			free( m_data_ptr );
}

GeodeFrameBuffer::iterator GeodeFrameBuffer::begin()
{
	return m_data_ptr;
}

GeodeFrameBuffer::iterator GeodeFrameBuffer::end()
{
	return m_data_ptr + m_size;
}

inline void GeodeFrameBuffer::swap( GeodeFrameBuffer& gFrameBuffer )
{
	if( !m_shared_data )
		if( m_size > 3 )
			free( m_data_ptr );

	m_width       = gFrameBuffer.width();
	m_height      = gFrameBuffer.height();
	m_depth       = gFrameBuffer.depth();
	m_size	      = gFrameBuffer.size();
	m_data_ptr    = (gcolor_t *)malloc(m_width * m_height * m_depth);
	m_shared_data = false;

	memcpy( m_data_ptr, gFrameBuffer.data(), m_size );
}

void GeodeFrameBuffer::load_bmp( const char * filename, bool resize/* = true*/, int x0/* = 0*/, int y0/* = 0*/ )
{
	CBmp bmp;

	bmp.load(filename);

	rgb_t ** prgbdata = bmp.getData();
	int      width    = bmp.getWidth(),
			 height   = bmp.getHeight(),
			 x,
			 y;

	GeodeFrameBuffer newbuffer( width, height, depth() );

	int off = 0;

	for( x = 0; x < width; x++ )
	{
		for( y = 0; y < height; y++ )
		{
			newbuffer(x,y,0) = prgbdata[x][y].Blue;
			newbuffer(x,y,1) = prgbdata[x][y].green;
			newbuffer(x,y,2) = prgbdata[x][y].Red;
		}
	}

	if( resize )
		swap(newbuffer);
	else
		set_portion( x0, y0, x0 + width, y0 + height, newbuffer );
}

void GeodeFrameBuffer::load_font_resource( int index, HMODULE hmod, int w /*= 8*/, int h /*= 13*/, int d /*= 3*/ )
{
	HMODULE mod  = hmod ? hmod : ::GetModuleHandleA("geode.dll");
	HBITMAP hbmp = ::LoadBitmap( (HINSTANCE)mod, MAKEINTRESOURCE(index) ); 

	BITMAPINFO bmi;

	bmi.bmiHeader.biSize		  = sizeof(BITMAPINFOHEADER);
	bmi.bmiHeader.biWidth		  = w;
	bmi.bmiHeader.biHeight		  = -h;
	bmi.bmiHeader.biPlanes		  = 1;
	bmi.bmiHeader.biBitCount	  = 8 * d;
	bmi.bmiHeader.biCompression	  = BI_RGB;
	bmi.bmiHeader.biSizeImage	  = 0;
	bmi.bmiHeader.biXPelsPerMeter = 1;
	bmi.bmiHeader.biYPelsPerMeter = 1;
	bmi.bmiHeader.biClrUsed		  = 0;
	bmi.bmiHeader.biClrImportant  = 0;

	gcolor_t * bits = new gcolor_t[ w * h * d ];

	int ret = ::GetDIBits( GetDC( GetActiveWindow() ), 
					  	   hbmp, 
						   0,
						   h,
						   bits,
						   &bmi,
						   DIB_RGB_COLORS
					      );

	::DeleteObject(hbmp);

	GeodeFrameBuffer newbuffer( w, h, depth() );

	int x,
	    y,
		off = 0;

	for( x = 0; x < w; x++ )
	{
		for( y = 0; y < h; y++ )
		{
			off = (x + w * y) * d;

			newbuffer(x,y,0) = bits[off + 0];
			newbuffer(x,y,1) = bits[off + 1];
			newbuffer(x,y,2) = bits[off + 2];
		}
	}

	delete[] bits;
	
	swap(newbuffer);
}

inline gcolor_t& GeodeFrameBuffer::operator () ( int x, int y, int color )
{ 
	return m_data_ptr[offset(x,y) + color]; 
}

inline gcolor_t& GeodeFrameBuffer::operator () ( int x , int y , int z , int v )
{
	return m_data_ptr[ offset(x,y,z,v) ];
}

inline gcolor_t& GeodeFrameBuffer::operator [] ( int offset )			 
{ 
	return m_data_ptr[offset]; 
}

inline GeodeFrameBuffer& GeodeFrameBuffer::operator = ( GeodeFrameBuffer& gFrameBuffer ) 
{
	swap(gFrameBuffer);
	return *this;
}

inline GeodeFrameBuffer& GeodeFrameBuffer::operator *= ( GeodeFrameBuffer& gFrameBuffer )
{
	if( width() != gFrameBuffer.height() || depth() != gFrameBuffer.depth() )
	{
		throw new GeodeException( 0, "Height and width must be the same size for matrix multiplication" );
		return *this;
	}

	GeodeFrameBuffer result( gFrameBuffer.width(), height(), depth() );
    
	gcolor_t val;

	for( int x = 0; x < result.width(); x++ )
	{
		for( int y = 0; y < result.height(); y++ )
		{
			val = 0; 
			
			for( int k = 0; k < width(); k++ )
			{
				val += (*this)(k,y,0) * gFrameBuffer(x,k,0);
			}

			result(x,y,0) = val;
		}
	}
      
	swap(result);

	return *this;
}

inline GeodeFrameBuffer GeodeFrameBuffer::operator * ( GeodeFrameBuffer& gFrameBuffer )
{
	GeodeFrameBuffer gfb1( *this );

	return (gfb1 *= gFrameBuffer);
}

inline GeodeFrameBuffer& GeodeFrameBuffer::operator *= ( double d )
{
	for( int i = 0; i < m_size; i++ )
		m_data_ptr[i] *= d;

	return *this;
}

inline GeodeFrameBuffer GeodeFrameBuffer::operator * ( double d )
{
	GeodeFrameBuffer gfb1( *this );

	return (gfb1 *= d);
}

inline GeodeFrameBuffer& GeodeFrameBuffer::operator += ( GeodeFrameBuffer& gFrameBuffer )
{
	int smin = size() < gFrameBuffer.size() ? size() : gFrameBuffer.size();
    
	gcolor_t *ptrs = gFrameBuffer.data() + smin,
		     *ptrd;
    
	for( ptrd = m_data_ptr + smin; ptrd > m_data_ptr; --ptrd, (*ptrd) = (*ptrd)+(*(--ptrs)) );

	return *this;
}

inline GeodeFrameBuffer GeodeFrameBuffer::operator + ( GeodeFrameBuffer& gFrameBuffer )
{
	GeodeFrameBuffer gfb1( *this );

	return (gfb1 += gFrameBuffer);
}

inline GeodeFrameBuffer& GeodeFrameBuffer::operator += ( gcolor_t uc )
{
	int i;

	for( i = 0; i < m_size; i++ )
	{
		m_data_ptr[i] += uc;

		if( m_data_ptr[i] < 0 )
			m_data_ptr[i] = 0xFF;
	}

	return *this;
}

inline GeodeFrameBuffer GeodeFrameBuffer::operator + ( gcolor_t uc )
{
	GeodeFrameBuffer gfb1( *this );

	return (gfb1 += uc);
}

inline int GeodeFrameBuffer::width() 
{ 
	return m_width;  
}

inline int GeodeFrameBuffer::height()
{ 
	return m_height; 
}

inline int GeodeFrameBuffer::depth() 
{ 
	return m_depth;  
}

inline int GeodeFrameBuffer::size()  
{ 
	return m_size;   
}

inline gcolor_t * GeodeFrameBuffer::data()  
{ 
	return m_data_ptr; 
}

inline bool GeodeFrameBuffer::shared()
{
	return m_shared_data; 
}

GeodeFrameBuffer& GeodeFrameBuffer::line( int x0, int y0, int x1, int y1, gcolor_t * color )
{
	int i,
		dx    = x1 - x0,	   /* the horizontal distance of the line */
		dy    = y1 - y0,	   /* the vertical distance of the line */
		sdx   = GeodeMath::sgn(dx),
		sdy   = GeodeMath::sgn(dy),
		dxabs = GeodeMath::abs(dx),
		dyabs = GeodeMath::abs(dy),
		x	  = dyabs >> 1,
		y	  = dxabs >> 1,
		px    = x0,
		py    = y0;

	if( dxabs >= dyabs ) /* the line is more horizontal than vertical */
	{
		for( i = 0; i < dxabs; i++ )
		{
			y += dyabs;

			if (y >= dxabs)
			{
				y  -= dxabs;
				py += sdy;
			}

			px += sdx;

			set_color(px,py,color);
		}
	}
	else /* the line is more vertical than horizontal */
	{
		for( i = 0; i < dyabs; i++ )
		{
			x += dxabs;

			if( x >= dyabs )
			{
				x  -= dyabs;
				px += sdx;
			}

			py += sdy;
			
			set_color(px,py,color);
		}
	}

	return *this;
}

GeodeFrameBuffer& GeodeFrameBuffer::rectangle( int x0, int y0, int x1, int y1, gcolor_t * color )
{
	line( x0 , y0 , x1 , y0 , color );
	line( x1 , y0 , x1 , y1 , color );
	line( x1 , y1 , x0 , y1 , color );
	line( x0 , y1 , x0 , y0 , color );

	return *this;
}

GeodeFrameBuffer& GeodeFrameBuffer::fill( gcolor_t * color )
{
	int i ;
	
	for( i = 0; i < m_size; i += m_depth )
		memcpy( m_data_ptr + i, color, m_depth );

	return *this;
}

GeodeFrameBuffer& GeodeFrameBuffer::clear()
{
	memset( m_data_ptr, 0, m_size );

	return *this;
}

GeodeFrameBuffer& GeodeFrameBuffer::circle( int x , int y,  int r,  gcolor_t * color )
{
    /*
		Distance x to eighth of circle (about which circle is symmetric).
       'Roof' guarantees that we do not miss parts of circle on approximation.
    */
	int eighth_x( (int)( r * 0.707107) ) ; // r cos( 45° ).
    int r_2     ( r * r );
    int xx      ( 0     ); // x at center.
    int yy      ( r     ); // y above center.

    while( xx <= eighth_x ) 
	{
        // Deduce 8 points from circle symmetry.
        int x_plus__xx( x + xx );
        int x_minus_xx( x - xx );
        int y_plus__yy( y + yy );
        int y_minus_yy( y - yy );

		set_color( x_plus__xx, y_minus_yy, color );
		set_color( x_minus_xx, y_minus_yy, color );
		set_color( x_plus__xx, y_plus__yy, color );
		set_color( x_minus_xx, y_plus__yy, color );
		
		set_color( y_minus_yy, x_plus__xx, color );
		set_color( y_minus_yy, x_minus_xx, color );
		set_color( y_plus__yy, x_plus__xx, color );
		set_color( y_plus__yy, x_minus_xx, color );
		
        ++xx;

		yy = GeodeMath::circle_next_y( xx, r_2, yy );
    }

	return *this;
}

GeodeFrameBuffer& GeodeFrameBuffer::primitive( GeodePrimitive2D& pr, gcolor_t * color )
{
	line( pr.p1.x, pr.p1.y, pr.p2.x, pr.p2.y, color );
	line( pr.p2.x, pr.p2.y, pr.p3.x, pr.p3.y, color );
	line( pr.p3.x, pr.p3.y, pr.p1.x, pr.p1.y, color );

	return *this;
}

GeodeFrameBuffer& GeodeFrameBuffer::resize( int width, int height, int depth )
{
	if( !m_shared_data )
		if( m_size > 3 )
			free( m_data_ptr );

	m_width       = width;
	m_height      = height;
	m_depth       = depth;
	m_size	      = width * height * depth;
	m_data_ptr    = (gcolor_t *)malloc(m_size);
	m_shared_data = false;
	
	return *this;
}

GeodeFrameBuffer& GeodeFrameBuffer::text( int x, int y, gcolor_t * fcolor, gcolor_t * bcolor, char * text )
{
	int txtlen = strlen(text);

	int sx = x,
		sy = y;

	GeodeFrameBuffer fontmask(8,13,4);

	for( int i = 0; i < txtlen; i++ )
	{
		switch(text[i])
		{
			case '\n' : 
				
				sx = x; 
				sy += 13; 
				break;
			
			case '\t' : 
				
				this->text( sx, sy, fcolor, bcolor, "    " ); 
				sx += 32;  
				break;

			default : 

				int FONT_RESOURCE_IDX = GEODE_CHAR_TO_FONT(text[i]);

				fontmask.load_font_resource(FONT_RESOURCE_IDX);
							
				int mx,
					my,
					fx,
					fy;

				for( mx = sx, fx = 0; mx < sx + 8; mx++, fx++ )
				{
					for( my = sy, fy = 0; my < sy + 13; my++, fy++ )
					{
						if( fontmask(fx,fy,0) )
						{
							if( fcolor )
								set_color(mx,my,fcolor);
						}
						else
						{
							if( bcolor )
								set_color(mx,my,bcolor);
						}
					}
				}
				
				sx += 8;
		}
	}

	return *this;
}

GeodeFrameBuffer& GeodeFrameBuffer::printf( int x, int y, gcolor_t * fcolor, gcolor_t * bcolor, char * format, ... )
{
	char txt[0xFF];

	va_list args;

	va_start( args, format );
		vsnprintf( txt, 0xFF, format, args );
	va_end( args );

	return text(x,y,fcolor,bcolor,txt);
}

GeodeFrameBuffer& GeodeFrameBuffer::printf( int x, int y, char * format, ... )
{
	char txt[0xFF];

	va_list args;

	va_start( args, format );
		vsnprintf( txt, 0xFF, format, args );
	va_end( args );

	return text(x,y,0,0,txt);
}

GeodeFrameBuffer GeodeFrameBuffer::get_portion( int x0, int y0, int x1, int y1 )
{
	GeodeFrameBuffer dest( x1 - x0, y1 - y0, m_depth );

	int sx,
		sy,
		dx,
		dy,
		soff;
	
	for( sx = x0, dx = 0; sx != x1; sx++, dx++ )
	{
		for( sy = y0, dy = 0; sy != y1; sy++, dy++ )
		{
			soff = offset(sx,sy);

			dest(dx,dy,0) = m_data_ptr[ soff + 0 ];
			dest(dx,dy,1) = m_data_ptr[ soff + 1 ];
			dest(dx,dy,2) = m_data_ptr[ soff + 2 ];
		}
	}

	return dest;
}


GeodeFrameBuffer& GeodeFrameBuffer::set_portion( int x0, int y0, int x1, int y1, GeodeFrameBuffer& gFrameBuffer )
{
	int sx,
		sy,
		dx,
		dy,
		doff;
	
	for( sx = 0, dx = x0; dx != x1; sx++, dx++ )
	{
		for( sy = 0, dy = y0; dy != y1; sy++, dy++ )
		{
			doff = offset(dx,dy);

			m_data_ptr[ doff + 0 ] = gFrameBuffer(sx,sy,0);
			m_data_ptr[ doff + 1 ] = gFrameBuffer(sx,sy,1);
			m_data_ptr[ doff + 2 ] = gFrameBuffer(sx,sy,2);
		}
	}

	return *this;
}

GeodeFrameBuffer& GeodeFrameBuffer::set_portion( int x0, int y0, GeodeFrameBuffer& gFrameBuffer )
{
	return set_portion( x0, y0, x0 + gFrameBuffer.width(), y0 + gFrameBuffer.height(), gFrameBuffer );
}

GeodeFrameBuffer& GeodeFrameBuffer::normalize( gcolor_t& a, gcolor_t& b )
{
	gcolor_t min = 255, max = 0, clr = 0;
	
	int i;

	for( i = 0; i < m_size; i++ )
	{
		clr = m_data_ptr[i];

		if( clr < min )
			min = clr;

		if( clr > max )
			max = clr;
	}

	for( i = 0; i < m_size; i++ )
		m_data_ptr[i] = (m_data_ptr[i]-min) / (max-min) * (b-a) + a;

	return *this;
}

GeodeFrameBuffer& GeodeFrameBuffer::denoise()
{
	float Ipp,
		  Icp,
		  Inp = 0.0,
		  Ipc,
		  Icc,
		  Inc = 0.0,
		  Ipn,
		  Icn,
		  Inn = 0.0;

	int y = 0,
		_py = 0,
		_ny = 0,
		_nx = 0,
		_px = 0,
		z = 0,
		x = 0;

	GeodeFrameBuffer mask(*this);

	for( int k = 0; k < m_size; k++ )
	{
		for( y = 0, _py = 0, _ny = 1; _ny < m_height || y == --_ny; _py = y++, _ny++ )
		{
			for( _nx = 1, _px = (int)( ( Icp = Ipp = (*this)(0,_py,0,k) ), 
										   ( Icc = Ipc = (*this)(0,  y,0,k) ), 
										   ( Icn = Ipn = (*this)(0,_ny,0,k))),
										 x = _px = 0;
										 ( _nx < (int)( m_width ) && ( ( Inp = (*this)(_nx,_py,0,k) ),
										 ( Inc = (*this)(_nx,  y,0,k) ),
										 ( Inn = (*this)(_nx,_ny,0,k) ),
										 1)) || x == -- _nx;
										 Ipp = Icp, Ipc = Icc, Ipn = Icn,
										 Icp = Inp, Icc = Inc, Icn = Inn,
										 _px = x++, _nx++ )
			{
				float ix = (Inc-Ipc)/2, 
				      iy = (Icn-Icp)/2, 
					  ng = ix*ix + iy*iy;

				mask(x,y,k,0) = (ng>1e-14)?( ix*ix * ( Icn + Icp - 2*Icc ) + iy*iy * ( Inc + Ipc - 2*Icc ) - ix*iy * ( Ipp + Inn - Ipn - Inp )/2 )/ng:0;
			}
		}
		

		(*this) += mask * 0.02;
	}

	return *this;
}

inline gcolor_t GeodeFrameBuffer::linear_interpolation( int x, int y, int color )
{
	float fx = x < 0 ? 0 : x, 
		  fy = y < 0 ? 0 : y;
	
	float dx = fx - x, 
		  dy = fy - y;

	unsigned int nx = dx > 0 ? x+1 : x, 
		         ny = dy > 0 ? y+1 : y;

	const gcolor_t &Icc = (*this)(x,y,color), 
				   &Inc = (*this)(nx,y,color), 
				   &Icn = (*this)(x,ny,color), 
				   &Inn = (*this)(nx,ny,color);

	return (gcolor_t)(Icc + dx*(Inc-Icc + dy*(Icc+Inn-Icn-Inc)) + dy*(Icn-Icc));
}

inline gcolor_t * GeodeFrameBuffer::linear_interpolation( int x, int y )
{
	gcolor_t * interpolation = new gcolor_t[ m_depth ];

	int color;

	for( color = 0; color < m_depth; color++ )
		interpolation[color] = linear_interpolation( x, y, color );

	return interpolation;
}

GeodeFrameBuffer GeodeFrameBuffer::linear_interpolation()
{
	GeodeFrameBuffer inperpolated( m_width, m_height, m_depth );

	int x,y,color;

	for( x = 0; x < m_width; x++ )
	{
		for( y = 0; y < m_height; y++ )
		{
			for( color = 0; color < m_depth; color++ )
			{
				inperpolated(x,y,color) = linear_interpolation(x,y,color);
			}
		}
	}

	return inperpolated;
}

}
