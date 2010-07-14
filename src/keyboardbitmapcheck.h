/////////////////////////////////////////////////////////////////////////////
// Name:        keyboardbitmapcheck.h
// Purpose:  
// Author:      Cesar Mauri Loba (cesar at crea-si dot com)
// Modified by: 
// Created:     28/06/2010
// Copyright:   (C) 2008 Cesar Mauri Loba - CREA Software Systems
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
#ifndef KEYBOARDBITMAPCHECK_H
#define KEYBOARDBITMAPCHECK_H

#include <wx/string.h>
#include <X11/Xlib.h>

class CKeyboardBitmapCheck
{
public:
	static int ReadKeySym();
	static wxString GetKeyName(KeySym ks);
};

#endif
