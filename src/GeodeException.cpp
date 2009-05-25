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

GeodeException::GeodeException( int error_code, char * msgFormat, ... )
{
	geode_trace( "GeodeException::GeodeException( %d, %s, ... )\n", error_code, msgFormat );

	va_list args;

	va_start( args, msgFormat );
		vsnprintf( m_error_message, 0xFF, msgFormat, args );
	va_end( args );

	m_error_code = error_code;
}

GeodeException::GeodeException( char * msgFormat, ... )
{
	geode_trace( "GeodeException::GeodeException( %s, ... )\n", msgFormat );

	m_error_code = ::GetLastError();

	va_list args;

	va_start( args, msgFormat );
		vsnprintf( m_error_message, 0xFF, msgFormat, args );
	va_end( args );
}

int GeodeException::code()
{ 
	geode_trace( "GeodeException::code()\n" );

	return m_error_code; 
}

char * GeodeException::message()
{ 
	geode_trace( "GeodeException::message()\n" );

	return m_error_message; 
}

}
