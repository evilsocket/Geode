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
#ifndef _BMP_DATA_STRUCTURES_H_
#	define _BMP_DATA_STRUCTURES_H_

#include "math.h"

inline bool _is_bendian(){
	short word = 0x0001;
	return ((*(char *)& word) != 0x01 );
}

typedef struct rgb_t {
	byte Blue;
	byte Green;
	byte Red;
	byte Alpha;

	rgb_t(byte r, byte g, byte b) {
		this->Red   = r ;
		this->Green = g ;
		this->Blue  = b ;
		//Alpha = 0 ;
	}

	rgb_t() {
		this->Red   = 0 ;
		this->Green = 0 ;
		this->Blue  = 0 ;
		//Alpha = 0 ;
	}

	bool isWhite() {
		return (Red == 255 && Green == 255 && Blue == 255);
	}

	void set(byte r, byte g, byte b) {
		this->Red   = r ;
		this->Green = g ;
		this->Blue  = b ;
		//Alpha = 0 ;
	}

	void set(rgb_t rgb) {
		this->Red   = rgb.Red ;
		this->Green = rgb.Green ;
		this->Blue  = rgb.Blue ;
		//Alpha = rgb.Alpha ;
	}

	int toInt() {
		return ((((byte)(this->Red)|((word)((byte)(this->Green))<<8))|(((dword)(byte)(this->Blue))<<16)));
	}

	void set(int ival) {
		this->Red   = (ival >> 16) & 0xFF ;
		this->Green = (ival >> 8 ) & 0xFF ;
		this->Blue  = ival & 0xFF ;
	}
} 
rgb_t; 

typedef struct {
	word  bfType;
	dword bfSize;
	word  bfReserved1;
	word  bfReserved2;
	dword bfOffBits; 

	void _switchEndian() {
		bfType      = math::flipw ( bfType );
		bfSize      = math::flipdw( bfSize );
		bfReserved1 = math::flipw ( bfReserved1 );
		bfReserved2 = math::flipw ( bfReserved2 );
		bfOffBits   = math::flipdw( bfOffBits );
	}
} 
bmp_file_header_t;

typedef struct {
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

	void _switchEndian() {
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


typedef struct {
	bmp_file_header_t _fheader ;
	bmp_info_header_t _iheader ;
	rgb_t**           _pdata   ;
	rgb_t*            _pcolors ;
}  
bmp_raw_data ;

#endif
