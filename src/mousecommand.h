/////////////////////////////////////////////////////////////////////////////
// Name:        mousecommand.h
// Purpose:     mouse command definitions  
// Author:      Cesar Mauri Loba (cesar at crea-si dot com)
// Modified by:
// Created:
// Copyright:   (C) 2012 Cesar Mauri Loba - CREA Software Systems
// 
//  This program is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with this program.  If not, see <http://www.gnu.org/licenses/>.
/////////////////////////////////////////////////////////////////////////////

#ifndef MOUSECOMMAND_H
#define MOUSECOMMAND_H

namespace mousecmd {
	enum mousecmd { 
		CMD_NO_CLICK = 0, 
		CMD_LEFT_CLICK, 
		CMD_MIDDLE_CLICK, 
		CMD_RIGHT_CLICK, 
		CMD_LEFT_DOWN, 
		CMD_LEFT_UP, 
		CMD_DOUBLE_CLICK 
	};
};

#endif // MOUSECOMMAND_H