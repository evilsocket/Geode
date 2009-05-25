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

#ifndef _BMP_DATA_STRUCTURES_H_
#	define _BMP_DATA_STRUCTURES_H_

#include "math.h"

inline bool _is_bendian()
{
	short word = 0x0001;

	return ((*(char *)& word) != 0x01 );
}

typedef struct rgb_t
{
	public :
	
	byte Blue;
	byte green;
	byte Red;
	byte Alpha;

	rgb_t(byte r, byte g, byte b)
	{
		this->Red   = r ;
		this->green = g ;
		this->Blue  = b ;
		//Alpha = 0 ;
	}

	rgb_t()
	{
		this->Red   = 0 ;
		this->green = 0 ;
		this->Blue  = 0 ;
		//Alpha = 0 ;
	}

	bool isWhite()
	{
		return (Red == 255 && green == 255 && Blue == 255);
	}

	void set(byte r, byte g, byte b)
	{
		this->Red   = r ;
		this->green = g ;
		this->Blue  = b ;
		//Alpha = 0 ;
	}

	void set(rgb_t rgb)
	{
		this->Red   = rgb.Red ;
		this->green = rgb.green ;
		this->Blue  = rgb.Blue ;
		//Alpha = rgb.Alpha ;
	}

	int toInt()
	{
		return ((((byte)(this->Red)|((word)((byte)(this->green))<<8))|(((dword)(byte)(this->Blue))<<16)));
	}

	void set(int ival)
	{
		this->Red   = (ival >> 16) & 0xFF ;
		this->green = (ival >> 8 ) & 0xFF ;
		this->Blue  = ival & 0xFF ;
	}
} 
rgb_t; 

typedef struct
{
	word  bfType;
	dword bfSize;
	word  bfReserved1;
	word  bfReserved2;
	dword bfOffBits; 

	void _switchEndian()
	{
		bfType      = math::flipw ( bfType );
		bfSize      = math::flipdw( bfSize );
		bfReserved1 = math::flipw ( bfReserved1 );
		bfReserved2 = math::flipw ( bfReserved2 );
		bfOffBits   = math::flipdw( bfOffBits );
	}
} 
bmp_file_header_t;

typedef struct
{
	dword biSize;
	dword biWidth;
	dword biHeight;
	word  biPlanes;
	word  biBitCount;
	dword biCompression;
	dword biSizeImage;
	dword biXPelsPerMeter;
	dword biYPelsPerMeter;
	dword biClrUsed;
	dword biClrImportant;

	void _switchEndian()
	{
		biSize			= math::flipdw( biSize );
		biWidth			= math::flipdw( biWidth );
		biHeight		= math::flipdw( biHeight );
		biPlanes		= math::flipw ( biPlanes );
		biBitCount      = math::flipw ( biBitCount );
		biCompression   = math::flipdw( biCompression );
		biSizeImage     = math::flipdw( biSizeImage );
		biXPelsPerMeter = math::flipdw( biXPelsPerMeter );
		biYPelsPerMeter = math::flipdw( biYPelsPerMeter );
		biClrUsed       = math::flipdw( biClrUsed );
		biClrImportant  = math::flipdw( biClrImportant );
	}
}
bmp_info_header_t;


typedef struct
{
	bmp_file_header_t _fheader ;
	bmp_info_header_t _iheader ;
	rgb_t**           _pdata   ;
	rgb_t*            _pcolors ;
}  
bmp_raw_data ;

#endif