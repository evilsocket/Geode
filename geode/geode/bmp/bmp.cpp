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

#include "bmp.h"

bool checkDataSize()
{
	if( sizeof(byte) != 1 )
		return false;

	if( sizeof(word) != 2 )
		return false;

	if( sizeof(dword) != 4 )
		return false;

	return true;
}

bool s_fread( char *ppbuffer, int size, int number, FILE *pfile )
{
	int read;

	if( feof(pfile) )
		return false;

	read = (int)fread( ppbuffer, size, number, pfile );

	return (read == number);
}


bool CBmp:: getrow32( byte* ppbuffer, int size, int row )
{
	int i = 0;

	if( _raw_data._iheader.biWidth * 4 > size )
		return false;

	for( i = 0; i < _raw_data._iheader.biWidth; i++ )
		memcpy( (char *)&(_raw_data._pdata[i][row]), (char *)ppbuffer + 4 * i, 4 );
	
	return true;
}

bool CBmp:: getrow24( byte* ppbuffer, int size, int row )
{
	int i = 0;

	if( _raw_data._iheader.biWidth * 3 > size )
		return false;

	for( i = 0; i < _raw_data._iheader.biWidth; i++ )
		memcpy( (char *)&(_raw_data._pdata[i][row]), (char *)ppbuffer + 3 * i, 3 );
	
	return true;
}
  
bool CBmp:: getrow08( byte* ppbuffer, int size, int row )
{
	int i = 0;

	if( _raw_data._iheader.biWidth > size )
		return false;

	for( i = 0; i < _raw_data._iheader.biWidth; i++ )
		*(*this)(i,row) = getColor(ppbuffer[i]);
	
	return true;
}

bool CBmp:: getrow04( byte* ppbuffer, int size, int row )
{
	int shifts[2] = {4  ,0 },
	    masks [2] = {240,15},
		i = 0,
		j,
		k = 0;

	if( _raw_data._iheader.biWidth > 2 * size )
		return false;

	while( i < _raw_data._iheader.biWidth )
	{
		j = 0;

		while( j < 2 && i < _raw_data._iheader.biWidth )
		{
			*(*this)(i++,row) = getColor( (int)((ppbuffer[k] & masks[j]) >> shifts[j]) );
			j++;
		}

		k++;
	}

	return true;
}

bool CBmp:: getrow01( byte* ppbuffer, int size, int row )
{
	int shifts[8] = {7  ,6 ,5 ,4 ,3,2,1,0},
	    masks [8] = {128,64,32,16,8,4,2,1},
		i = 0,
		j,
		k = 0;

	if( _raw_data._iheader.biWidth > 8 * size )
		return false;

	while( i < _raw_data._iheader.biWidth )
	{
		j = 0;

		while( j < 8 && i < _raw_data._iheader.biWidth )
		{
			
			*(*this)(i++,row) = getColor( (int)( (ppbuffer[k] & masks[j]) >> shifts[j]) ); 
			j++;   
		}

		k++;
	}

	return true;
}

bool CBmp:: putrow32( byte* ppbuffer, int size, int row )
{
	int i;

	if( _raw_data._iheader.biWidth * 4 > size )
		return false;

	for( i = 0; i < _raw_data._iheader.biWidth; i++ )
		memcpy( (char *)ppbuffer + 4 * i, (char *)&(_raw_data._pdata[i][row]), 4 );

	return true;
}

bool CBmp:: putrow24( byte* ppbuffer, int size, int row )
{
	int i;

	if( _raw_data._iheader.biWidth * 3 > size )
		return false;

	for( i = 0; i < _raw_data._iheader.biWidth; i++ )
		memcpy( (char *)ppbuffer + 3 * i, (char *)&(_raw_data._pdata[i][row]), 3 );

	return true;
}

bool CBmp:: putrow08( byte* ppbuffer, int size, int row )
{
	int i = 0;

	if( _raw_data._iheader.biWidth > size )
		return false;

	for( i = 0; i < _raw_data._iheader.biWidth; i++ )
		ppbuffer[i] = FindClosestColor( _raw_data._pdata[i][row] );

	return true;
}
  
bool CBmp:: putrow04( byte* ppbuffer, int size, int row )
{
	int weights[2] = {16,1},
		i = 0,
		j,
		k = 0;

	if( _raw_data._iheader.biWidth > 2 * size )
		return false;

	while( i < _raw_data._iheader.biWidth )
	{
		j = 0;

		int idx = 0;

		while( j < 2 && i < _raw_data._iheader.biWidth )
		{
			idx += ( weights[j] * (int)FindClosestColor( _raw_data._pdata[i][row] ) ); 
			i++; j++;   
		}
		
		ppbuffer[k] = (byte)idx;
		k++;
	}

	return true;
}
 
bool CBmp:: putrow01( byte* ppbuffer, int size, int row )
{
	int weights[8] = {128,64,32,16,8,4,2,1},
		i = 0,
		j,
		k = 0;

	if( _raw_data._iheader.biWidth >  8 * size )
		return false;

	while( i < _raw_data._iheader.biWidth )
	{
		j = 0;

		int idx = 0;

		while( j < 8 && i < _raw_data._iheader.biWidth )
		{
			idx += ( weights[j] * (int)FindClosestColor( _raw_data._pdata[i][row] ) ); 
			i++; j++;   
		}
		ppbuffer[k] = (byte)idx;
		k++;
	}

	return true;
}

byte CBmp::FindClosestColor( rgb_t& input )
{
	int  i = 0,
	     colors = getColors(),
		 BestMatch = 999999;

	byte BestI = 0;

	while( i < colors )
	{
		rgb_t attempt = getColor( i );

		int tmp =  math::isquare( (int)attempt.Red - (int)input.Red )
				 + math::isquare( (int)attempt.green - (int)input.green )
				 + math::isquare( (int)attempt.Blue  - (int)input.Blue );

		if( tmp < BestMatch )
		{	
			BestI     = (byte)i; 
			BestMatch = tmp; 
		}
		
		if( BestMatch < 1 )
			i = colors;

		i++;
	}

	return BestI;
}


CBmp::CBmp()
{
	_raw_data._fheader.bfType     = 19778 ;
	_raw_data._fheader.bfSize     = 0     ;
	_raw_data._fheader.bfOffBits  = 54    ;

	_raw_data._iheader.biSize     = 40;
	_raw_data._iheader.biPlanes   = 1;
	_raw_data._iheader.biWidth    = 1;
	_raw_data._iheader.biHeight   = 1;
	_raw_data._iheader.biBitCount = 24;
	_raw_data._iheader.biCompression = 0;
	
	_raw_data._pdata   = NULL ;
	_raw_data._pcolors = NULL;

	_raw_data._iheader.biXPelsPerMeter = 0;
	_raw_data._iheader.biYPelsPerMeter = 0;

	_is_sobel_map = false;
}

bool CBmp::isSobelMap()
{
	return _is_sobel_map ;
}

void CBmp::setSobelMapFlag(bool flag)
{
	_is_sobel_map = flag;
}

CBmp::~CBmp()
{	
	for( int i = 0; i < _raw_data._iheader.biWidth; i++)
		delete[] _raw_data._pdata[i];

	delete[] _raw_data._pdata;

	if( _raw_data._pcolors )
		delete[] _raw_data._pcolors;
}

int CBmp::getDepth  (){ return _raw_data._iheader.biBitCount;  }
int CBmp::getWidth  (){ return _raw_data._iheader.biWidth;  }
int CBmp::getHeight (){ return _raw_data._iheader.biHeight; }

int CBmp::getColors ()
{
	int ret = math::ipower( 2, _raw_data._iheader.biBitCount );

	if( _raw_data._iheader.biBitCount == 32 )
		ret = math::ipower( 2, 24 );

	return ret;
}

int CBmp::getVDpi   ()
{
	if( !_raw_data._iheader.biYPelsPerMeter )
		_raw_data._iheader.biYPelsPerMeter = 3780;

	return (int)(_raw_data._iheader.biYPelsPerMeter / (double)39.37007874015748); 
}

int CBmp::getHDpi   ()
{
	if( !_raw_data._iheader.biXPelsPerMeter )
		_raw_data._iheader.biXPelsPerMeter = 3780;

	return (int)(_raw_data._iheader.biXPelsPerMeter / (double)39.37007874015748); 
}

void CBmp::setDpi  ( int hdpi, int vdpi )
{
	_raw_data._iheader.biXPelsPerMeter = (int)(hdpi * 39.37007874015748);
	_raw_data._iheader.biYPelsPerMeter = (int)(vdpi * 39.37007874015748);
}

bool CBmp::setSize ( int width, int height )
{
	int i,j; 

	if( _raw_data._pdata && _raw_data._pdata[0] )
	{
		for( i = 0; i < _raw_data._iheader.biWidth; i++)
			delete[] _raw_data._pdata[i];

		delete[] _raw_data._pdata;
	}

	_raw_data._iheader.biWidth  = width;
	_raw_data._iheader.biHeight = height;

	_raw_data._pdata = new rgb_t * [ width ]; 

	for( i = 0; i < width; i++)
		_raw_data._pdata[i] = new rgb_t [ height ];

	for( i = 0; i < width; i++ )
	{
		for( j = 0; j < height; j++ )
		{
			_raw_data._pdata[i][j].Red   = 255; 
			_raw_data._pdata[i][j].green = 255; 
			_raw_data._pdata[i][j].Blue  = 255; 
			_raw_data._pdata[i][j].Alpha = 0;    
		}
	}

	return true; 
}

bool CBmp::setDepth( int depth )
{
	_raw_data._iheader.biBitCount = depth;

	if( _raw_data._pcolors )
		delete[] _raw_data._pcolors;

	int colors = math::ipower( 2, _raw_data._iheader.biBitCount );

	if( _raw_data._iheader.biBitCount == 1 || _raw_data._iheader.biBitCount == 4 || _raw_data._iheader.biBitCount == 8 )
		_raw_data._pcolors = new rgb_t[colors];
	else
		_raw_data._pcolors = NULL;
	
	if( _raw_data._iheader.biBitCount == 1 || _raw_data._iheader.biBitCount == 4 || _raw_data._iheader.biBitCount == 8 )
		createStdColorTable();

	_raw_data._iheader.biBitCount = _raw_data._iheader.biBitCount ;

	return true;
}

rgb_t * CBmp::operator()(int x,int y)
{
	if( x >= this->getWidth() || y >= this->getHeight() )
		return NULL ;
	else
		return &_raw_data._pdata[x][y];
}

bmp_file_header_t * CBmp::getFileHeader()
{
	return &_raw_data._fheader ;
}

bmp_info_header_t * CBmp::getInfoHeader()
{
	return &_raw_data._iheader ;
}

rgb_t* CBmp::getPalette()
{
	return _raw_data._pcolors ;
}

rgb_t** CBmp::getData()
{
	return _raw_data._pdata ;
}

CBmp * CBmp::clone()
{
	CBmp * pclone = new CBmp();

	pclone->setDepth( getDepth() );
	pclone->setDpi(96,96);
	pclone->setSize( getWidth(), getHeight() );

	rgb_t ** pdata = pclone->_raw_data._pdata ;

	for( int x = 0; x < getWidth(); x++ )
		for( int y = 0; y < getHeight(); y++ )
			memcpy( &pdata[x][y], &_raw_data._pdata[x][y], sizeof(rgb_t) );

	return pclone;
}

bool CBmp::createStdColorTable()
{
	int i = 0;
	int j,k,ell;

	if( _raw_data._iheader.biBitCount == 1 )
	{
		for( i = 0; i < 2; i++ )
			_raw_data._pcolors[i].set( i*255, i*255, i*255 );

		return true;
	} 

	if( _raw_data._iheader.biBitCount == 4 )
	{

		for( ell = 0; ell < 2; ell++ )
			for( k = 0 ; k < 2; k++ )
				for( j = 0; j < 2; j++ )
					_raw_data._pcolors[i++].set( j*128, k*128, ell*128 ); 
				
		for( ell = 0; ell < 2; ell++ )
			for( k = 0; k < 2; k++ )
				for( j = 0; j < 2; j++ )
					_raw_data._pcolors[i++].set( j*255, k*255, ell*255 );

		_raw_data._pcolors[8].set(192, 192, 192);

		for( i = 0; i < 16; i++ )
			_raw_data._pcolors[i].Alpha = 0;

		return true;
	}

	if( _raw_data._iheader.biBitCount == 8 )
	{
		for( ell = 0; ell < 4; ell++ ) 
			for( k = 0; k < 8; k++ )
				for( j = 0; j < 8; j++ )
					_raw_data._pcolors[i++].set( j*32, k*32, ell*64 );

		i = 0;

		for( ell = 0; ell < 2; ell++ ) 
			for( k = 0; k < 2; k++ )
				for( j = 0; j < 2; j++ )
					_raw_data._pcolors[i++].set( j*128, k*128, ell*128 );

		i = 7;

		_raw_data._pcolors[i++].set(192,192,192);
		_raw_data._pcolors[i++].set(192,220,192);
		_raw_data._pcolors[i++].set(166,202,240);

		i = 246;
		
		_raw_data._pcolors[i++].set(255,251,240);
		_raw_data._pcolors[i++].set(160,160,164);
		_raw_data._pcolors[i++].set(128,128,128);
		_raw_data._pcolors[i++].set(255,  0,  0);
		_raw_data._pcolors[i++].set(0  ,255,  0);
		_raw_data._pcolors[i++].set(255,255,  0);
		_raw_data._pcolors[i++].set(  0,  0,255);
		_raw_data._pcolors[i++].set(255,  0,255);
		_raw_data._pcolors[i++].set(  0,255,255);
		_raw_data._pcolors[i++].set(255,255,255);

		return true;
	}

	return false;
}

rgb_t CBmp::getColor( int ColorNumber )
{
	rgb_t ret;

	ret.set(255,255,255);

	if( _raw_data._iheader.biBitCount != 1 && _raw_data._iheader.biBitCount != 4 && _raw_data._iheader.biBitCount != 8 )
		return ret;
	
	if( !_raw_data._pcolors )
		return ret; 

	if( ColorNumber >= getColors() )
		return ret;
	
	return _raw_data._pcolors[ColorNumber];
}

bool CBmp::setColor( int ColorNumber, rgb_t Color )
{
	if( _raw_data._iheader.biBitCount != 1 && _raw_data._iheader.biBitCount != 4 && _raw_data._iheader.biBitCount != 8 )
		return false;
	
	if( !_raw_data._pcolors )
		return false; 
	
	if( ColorNumber >= getColors() )
		return false;

	_raw_data._pcolors[ColorNumber] = Color;

	return true;
}

bool CBmp::load( const char * szBitmapFileName )
{
	if( !checkDataSize() )
	{
		throw new exception("$CBmp::load(...) : Wrong system data sizes !");
		return false; 
	}

	FILE * pfile = fopen( szBitmapFileName, "rb" );

	if( pfile == NULL )
	{
		throw new exception("$CBmp::load(...) : Cannot open bitmap file !");
		this->setDepth(1);
		this->setSize(1,1);
		return false;
	}
 
	bool ok       = true,
		 IsBitmap = false;

	ok &= s_fread( (char *)&(_raw_data._fheader.bfType), sizeof(word), 1, pfile );

	if( _is_bendian() && _raw_data._fheader.bfType == 16973 )
		IsBitmap = true;

	if( !_is_bendian() && _raw_data._fheader.bfType == 19778 )
		IsBitmap = true;

	if( !IsBitmap ) 
	{
		fclose( pfile );
		throw new exception("$CBmp::load(...) : Not a bitmap file !");		 
		return false;
	}

	ok &= s_fread( (char *)&(_raw_data._fheader.bfSize)     , sizeof(dword), 1, pfile); 
	ok &= s_fread( (char *)&(_raw_data._fheader.bfReserved1), sizeof(word) , 1, pfile);
	ok &= s_fread( (char *)&(_raw_data._fheader.bfReserved2), sizeof(word) , 1, pfile);
	ok &= s_fread( (char *)&(_raw_data._fheader.bfOffBits)  , sizeof(dword), 1, pfile);
 
	if( _is_bendian() ) 
		_raw_data._fheader._switchEndian();

	ok &= s_fread( (char *)&(_raw_data._iheader.biSize)         , sizeof(dword), 1, pfile);
	ok &= s_fread( (char *)&(_raw_data._iheader.biWidth)        , sizeof(dword), 1, pfile); 
	ok &= s_fread( (char *)&(_raw_data._iheader.biHeight)       , sizeof(dword), 1, pfile);
	ok &= s_fread( (char *)&(_raw_data._iheader.biPlanes)       , sizeof(word) , 1, pfile); 
	ok &= s_fread( (char *)&(_raw_data._iheader.biBitCount)     , sizeof(word) , 1, pfile);

	ok &= s_fread( (char *)&(_raw_data._iheader.biCompression)  , sizeof(dword), 1, pfile);
	ok &= s_fread( (char *)&(_raw_data._iheader.biSizeImage)    , sizeof(dword), 1, pfile);
	ok &= s_fread( (char *)&(_raw_data._iheader.biXPelsPerMeter), sizeof(dword), 1, pfile);
	ok &= s_fread( (char *)&(_raw_data._iheader.biYPelsPerMeter), sizeof(dword), 1, pfile);
	ok &= s_fread( (char *)&(_raw_data._iheader.biClrUsed)      , sizeof(dword), 1, pfile);
	ok &= s_fread( (char *)&(_raw_data._iheader.biClrImportant) , sizeof(dword), 1, pfile);
 
	if( _is_bendian() ) 
		_raw_data._iheader._switchEndian();
  
	if( !ok )
	{
		fclose(pfile);
		setSize(1,1);
		setDepth(1);
		throw new exception("$CBmp::load(...) : File is corrupted !");
		return false;
	} 
 
	if( _raw_data._iheader.biCompression == 1 || _raw_data._iheader.biCompression == 2 )
	{
		fclose(pfile);
		setSize(1,1);
		setDepth(1);
		throw new exception("$CBmp::load(...) : File is RLE compressed and this is not yet supported !");
		return false;
	}
 
	if( _raw_data._iheader.biCompression > 3 )
	{
		fclose(pfile);
		setSize(1,1);
		setDepth(1);
		throw new exception("$CBmp::load(...) : Unknown compression flag !");
		return false; 
	}
 
	if( _raw_data._iheader.biCompression == 3 && _raw_data._iheader.biBitCount != 16 )
	{
		fclose(pfile);
		setSize(1,1);
		setDepth(1);
		throw new exception("$CBmp::load(...) : File use bit fields but it's not a 16bit file and this is not yet supported !");
		return false; 
	}

	int tmpDepth = (int)_raw_data._iheader.biBitCount;

	if( tmpDepth != 1  && tmpDepth != 4 
		&& tmpDepth != 8  && tmpDepth != 16
		&& tmpDepth != 24 && tmpDepth != 32 
	  )
	{
		fclose(pfile);
		setSize(1,1);
		setDepth(1);
		throw new exception("$CBmp::load(...) : Unrecognized bit depth !");
		return false; 
	}


	setDepth( (int)_raw_data._iheader.biBitCount ); 

	if( (int)_raw_data._iheader.biWidth <= 0 || (int)_raw_data._iheader.biHeight <= 0 ) 
	{
		fclose(pfile);
		setSize(1,1);
		setDepth(1);
		throw new exception("$CBmp::load(...) : Non positive width or height value !");
		return false; 
	} 

	setSize( (int)_raw_data._iheader.biWidth , (int)_raw_data._iheader.biHeight );
  

	double dBytesPerPixel = ( (double)_raw_data._iheader.biBitCount ) / 8.0;
	double dBytesPerRow   = ceil(dBytesPerPixel * (_raw_data._iheader.biWidth+0.0));

	int BytePaddingPerRow = 4 - ((int)(dBytesPerRow)) % 4;

	if( BytePaddingPerRow == 4 )
		BytePaddingPerRow = 0;
 
	if( _raw_data._iheader.biBitCount < 16 )
	{
  
		int NumberOfColorsToRead = ((int)_raw_data._fheader.bfOffBits - 54 )/4; 

		if( NumberOfColorsToRead > math::ipower(2,_raw_data._iheader.biBitCount) )
			NumberOfColorsToRead = math::ipower(2,_raw_data._iheader.biBitCount);
		
		int n;

		for( n = 0; n < NumberOfColorsToRead; n++ )
			s_fread( (char *)&(_raw_data._pcolors[n]) , 4 , 1, pfile);     
		
		for( n = NumberOfColorsToRead; n < getColors() ; n++ )
		{
			rgb_t white; 
		
			white.set(255,255,255);

			setColor(n,white);
		}
	}
 
	int BytesToSkip = _raw_data._fheader.bfOffBits - 54;
 
	if( _raw_data._iheader.biBitCount < 16 )
		BytesToSkip -= 4 * math::ipower(2,_raw_data._iheader.biBitCount);
 
	if( _raw_data._iheader.biBitCount == 16 && _raw_data._iheader.biCompression == 3 )
		BytesToSkip -= 3 * 4;
 
	if( BytesToSkip < 0 )
		BytesToSkip = 0;
 
	if( BytesToSkip > 0 && _raw_data._iheader.biBitCount != 16 )
	{
		byte * tmpByte;
		tmpByte = new byte[BytesToSkip];
		s_fread( (char *)tmpByte, BytesToSkip, 1, pfile);   
		delete [] tmpByte;
	} 

	int i,j;
	
	if( _raw_data._iheader.biBitCount != 16 )
	{
		int bsize = (int)( (_raw_data._iheader.biWidth*_raw_data._iheader.biBitCount) / 8.0 );
		
		while( 8 * bsize < _raw_data._iheader.biWidth*_raw_data._iheader.biBitCount )
			bsize++ ;

		while( bsize % 4 )
			bsize++ ;

		byte * pbuffer = new byte [bsize];

		j = _raw_data._iheader.biHeight - 1;

		while( j > -1 )
		{
			int read = (int)fread( (char *)pbuffer, 1, bsize, pfile );
			
			if( read < bsize )
				j = -1; 
			else
			{
				bool Success = false;

				if( _raw_data._iheader.biBitCount == 1  )
					Success = getrow01(  pbuffer, bsize, j );

				if( _raw_data._iheader.biBitCount == 4  )
					Success = getrow04(  pbuffer, bsize, j );

				if( _raw_data._iheader.biBitCount == 8  )
					Success = getrow08(  pbuffer, bsize, j );

				if( _raw_data._iheader.biBitCount == 24 )
					Success = getrow24( pbuffer, bsize, j );

				if( _raw_data._iheader.biBitCount == 32 )
					Success = getrow32( pbuffer, bsize, j );

				if( !Success )
					j = -1;
			}   
			
			j--;
		}
		delete [] pbuffer; 
	}

	if( _raw_data._iheader.biBitCount == 16 )
	{
		int DataBytes    = _raw_data._iheader.biWidth * 2;
		int PaddingBytes = ( 4 - DataBytes % 4 ) % 4;

		word bmask = 31; 
		word gmask = 992; 
		word rmask = 31744; 

		if( _raw_data._iheader.biCompression != 0 )
		{
			word TempMaskWORD;
			word ZeroWORD = 0;

			s_fread( (char *)&rmask, 2, 1, pfile );
			
			if( _is_bendian() )
				rmask = math::flipw(rmask);
			
			s_fread( (char *)&TempMaskWORD, 2, 1, pfile );
  
			s_fread( (char *)&gmask , 2, 1, pfile );

			if( _is_bendian() )
				gmask = math::flipw(gmask);

			s_fread( (char *)&TempMaskWORD, 2, 1, pfile );

			s_fread( (char *)&bmask, 2, 1, pfile );
			
			if( _is_bendian() )
				bmask = math::flipw(bmask);

			s_fread( (char *)&TempMaskWORD, 2, 1, pfile );
		}
  
		if( BytesToSkip > 0 )
		{
			byte * tmpByte = new byte [BytesToSkip];
			s_fread( (char *)tmpByte, BytesToSkip, 1 , pfile );
			delete[] tmpByte;   
		} 
  
		int  gshift = 0; 
		word tmpshiftw = gmask;

		while( tmpshiftw > 31 )
		{
			tmpshiftw = tmpshiftw >> 1; 
			gshift++;  
		}

		int bshift = 0;
		tmpshiftw = bmask;

		while( tmpshiftw > 31 )
		{ 
			tmpshiftw = tmpshiftw >> 1; 
			bshift++; 
		} 
		
		int rshift = 0;  
		tmpshiftw = rmask;

		while( tmpshiftw > 31 )
		{
			tmpshiftw = tmpshiftw >> 1; 
			rshift++; 
		}  
   
		for( j = _raw_data._iheader.biHeight - 1; j >= 0; j-- )
		{
			i = 0;

			int ReadNumber = 0;

			while( ReadNumber < DataBytes )
			{
				word tmpw;
				
				s_fread( (char *)&tmpw, 2, 1, pfile );
				
				if( _is_bendian() )
					tmpw = math::flipw(tmpw);

				ReadNumber += 2;

				word r = rmask & tmpw;
				word g = gmask & tmpw;
				word b = bmask & tmpw;
							
				byte bb = (byte)8 *(b >> bshift);
				byte gb = (byte)8 *(g >> gshift);
				byte rb = (byte)8 *(r >> rshift);

				_raw_data._pdata[i][j].set(rb,gb,bb);

				i++;
			}

			ReadNumber = 0;
			
			while( ReadNumber < PaddingBytes )
			{
				byte tmpb;
				s_fread( (char *)&tmpb, 1, 1, pfile );
				ReadNumber++;
			}
		}

	}
 
	fclose(pfile);
	return true;
}

bool CBmp::save( const char* szBitmapFileName )
{
	if( !checkDataSize() )
	{
		throw new exception("$CBmp::save(...) : Wrong system data sizes !");
		return false; 
	}
 
	FILE * pfile = fopen( szBitmapFileName, "wb" );
		
	if( pfile == NULL )
	{
		throw new exception("$CBmp::save(...) : Couldn't open file for saving !");
		return false;
	}

	double dBytesPerPixel = ( (double) _raw_data._iheader.biBitCount ) / 8.0;
	double dBytesPerRow = ceil(dBytesPerPixel * (_raw_data._iheader.biWidth+0.0));

	int BytePaddingPerRow = 4 - ( (int) (dBytesPerRow) )% 4;
	
	if( BytePaddingPerRow == 4 )
		BytePaddingPerRow = 0;

	fwrite( (char *)&(_raw_data._fheader.bfType)     , sizeof(word) , 1, pfile );
	fwrite( (char *)&(_raw_data._fheader.bfSize)     , sizeof(dword), 1, pfile );
	fwrite( (char *)&(_raw_data._fheader.bfReserved1), sizeof(word) , 1, pfile );
	fwrite( (char *)&(_raw_data._fheader.bfReserved2), sizeof(word) , 1, pfile );
	fwrite( (char *)&(_raw_data._fheader.bfOffBits)  , sizeof(dword), 1, pfile );

	fwrite( (char *)&(_raw_data._iheader.biSize)		   , sizeof(dword), 1, pfile );
	fwrite( (char *)&(_raw_data._iheader.biWidth)		   , sizeof(dword), 1, pfile );
	fwrite( (char *)&(_raw_data._iheader.biHeight)	   , sizeof(dword), 1, pfile );
	fwrite( (char *)&(_raw_data._iheader.biPlanes)	   , sizeof(word) , 1, pfile );
	fwrite( (char *)&(_raw_data._iheader.biBitCount)	   , sizeof(word) , 1, pfile );
	fwrite( (char *)&(_raw_data._iheader.biCompression)  , sizeof(dword), 1, pfile );
	fwrite( (char *)&(_raw_data._iheader.biSizeImage)    , sizeof(dword), 1, pfile );
	fwrite( (char *)&(_raw_data._iheader.biXPelsPerMeter), sizeof(dword), 1, pfile );
	fwrite( (char *)&(_raw_data._iheader.biYPelsPerMeter), sizeof(dword), 1, pfile ); 
	fwrite( (char *)&(_raw_data._iheader.biClrUsed)      , sizeof(dword), 1, pfile );
	fwrite( (char *)&(_raw_data._iheader.biClrImportant) , sizeof(dword), 1, pfile );
  
	if( _raw_data._iheader.biBitCount == 1 || _raw_data._iheader.biBitCount == 4 || _raw_data._iheader.biBitCount == 8 )
	{
		int NumberOfColors = math::ipower(2,_raw_data._iheader.biBitCount);

		if( !_raw_data._pcolors )
		{
			if( !_raw_data._pcolors )
				_raw_data._pcolors = new rgb_t[NumberOfColors];

			createStdColorTable(); 
		}

		int n;

		for( n = 0; n < NumberOfColors; n++ )
			fwrite( (char *)&(_raw_data._pcolors[n]), 4 , 1, pfile );
	}
 
	int i,j;

	if( _raw_data._iheader.biBitCount != 16 )
	{  
		byte * pbuffer;

		int bsize = (int)( (_raw_data._iheader.biWidth * _raw_data._iheader.biBitCount) / 8.0 );
		
		while( 8 * bsize < _raw_data._iheader.biWidth * _raw_data._iheader.biBitCount )
			bsize++;

		while( bsize % 4 )
			bsize++;

		pbuffer = new byte[bsize];

		for( j = 0; j < bsize; j++ )
			pbuffer[j] = 0;

		j = _raw_data._iheader.biHeight - 1;
  
		while( j > -1 )
		{
			bool Success = false;

			if( _raw_data._iheader.biBitCount == 32 )
				Success = putrow32( pbuffer, bsize, j );

			if( _raw_data._iheader.biBitCount == 24 )
				Success = putrow24( pbuffer, bsize, j );

			if( _raw_data._iheader.biBitCount == 8  )
				Success = putrow08( pbuffer, bsize, j );

			if( _raw_data._iheader.biBitCount == 4  )
				Success = putrow04( pbuffer, bsize, j ); 

			if( _raw_data._iheader.biBitCount == 1  )
				Success = putrow01( pbuffer, bsize, j );

			if( Success )
			{
				int BytesWritten = (int)fwrite( (char*)pbuffer, 1, bsize, pfile );
			
				if( BytesWritten != bsize )
					Success = false;
			}

			if( !Success )
				j = -1; 
			
			j--; 
		}
  
		delete [] pbuffer;
	}
 
	if( _raw_data._iheader.biBitCount == 16 )
	{
		word bmask = 31; 
		word gmask = 2016; 
		word rmask = 63488; 
		word ZeroWORD = 0;

		if( _is_bendian() )
			rmask = math::flipw( rmask );

		fwrite( (char *)&rmask   , 2, 1, pfile );
		fwrite( (char *)&ZeroWORD, 2, 1, pfile );

		if( _is_bendian() )
			gmask = math::flipw( gmask );

		fwrite( (char *)&gmask   , 2, 1, pfile );
		fwrite( (char *)&ZeroWORD, 2, 1, pfile );

		if( _is_bendian() )
			bmask = math::flipw( bmask );

		fwrite( (char *)&bmask   , 2, 1, pfile );
		fwrite( (char *)&ZeroWORD, 2, 1, pfile );

		int DataBytes = _raw_data._iheader.biWidth * 2;
		int PaddingBytes = ( 4 - DataBytes % 4 ) % 4;
  
		for( j = _raw_data._iheader.biHeight - 1; j >= 0; j-- )
		{
			i = 0;

			int WriteNumber = 0;

			while( WriteNumber < DataBytes )
			{
				word tmpw;

				word rw = (word)((_raw_data._pdata[i][j]).Red   / 8);
				word gw = (word)((_raw_data._pdata[i][j]).green / 4);
				word bw = (word)((_raw_data._pdata[i][j]).Blue  / 8);

				tmpw = (rw << 11) + (gw << 5) + bw;
				
				if( _is_bendian() )
					tmpw = math::flipw( tmpw );
	
				fwrite( (char *)&tmpw, 2, 1, pfile);
				WriteNumber += 2;
				i++;
			}

			WriteNumber = 0;
			
			while( WriteNumber < PaddingBytes )
			{
				byte tmpb;
				fwrite( (char *)&tmpb, 1, 1, pfile);
				WriteNumber++;
			}
		}
  
	 }

	fclose(pfile);
	return true;	
}

