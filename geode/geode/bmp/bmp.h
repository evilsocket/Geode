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

#ifndef _BMP_H_
#	define _BMP_H_

#include "bmp_data_structures.h"
#include <memory.h>
#include <math.h>
#include <stdio.h>
#include <exception>

using namespace std ;

class CBmp
{
	private:

		bmp_raw_data _raw_data ;
		bool		 _is_sobel_map;
	

		bool getrow32( byte* pbuffer, int size, int row );   
		bool getrow24( byte* pbuffer, int size, int row );   
		bool getrow08( byte* pbuffer, int size, int row );  
		bool getrow04( byte* pbuffer, int size, int row );  
		bool getrow01( byte* Buffer, int size, int row );

		bool putrow32( byte* pbuffer, int size, int row );   
		bool putrow24( byte* pbuffer, int size, int row );   
		bool putrow08( byte* pbuffer, int size, int row );  
		bool putrow04( byte* pbuffer, int size, int row );  
		bool putrow01( byte* pbuffer, int size, int row );

		bool createStdColorTable();

		byte FindClosestColor( rgb_t& input );

	public: 

		CBmp();
		~CBmp();

		int		   getDepth  ();
		int		   getWidth  ();
		int		   getHeight ();
		int		   getColors ();
		int		   getVDpi   ();
		int		   getHDpi   ();
		bool	   isSobelMap();

		rgb_t getColor  ( int ColorNumber );

		bmp_file_header_t * getFileHeader();
		bmp_info_header_t * getInfoHeader();

		rgb_t*		getPalette();
		rgb_t**		getData();
		

		void setDpi  ( int hdpi, int vdpi );
		bool setSize ( int width, int height );
		bool setDepth( int depth );
		bool setColor( int ColorNumber, rgb_t Color );
		void setSobelMapFlag(bool flag);
		
		CBmp * clone();
	
		rgb_t * operator()(int x,int y);

		bool load( const char* szBitmapFileName );
		bool save( const char* szBitmapFileName );	
};





#endif