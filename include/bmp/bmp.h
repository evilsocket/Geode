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
