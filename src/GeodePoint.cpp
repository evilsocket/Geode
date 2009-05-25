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

GeodePoint::GeodePoint( int x, int y )
{
	this->x = x;
	this->y = y;
}

GeodePoint::GeodePoint()
{
	this->x = -1;
	this->y = -1;
}

GeodePoint::GeodePoint( GeodePoint& p )
{
	this->x = p.x;
	this->y = p.y;
}

GeodePoint& GeodePoint::operator = ( GeodePoint& p )
{
	this->x = p.x;
	this->y = p.y;

	return *this;
}

}
