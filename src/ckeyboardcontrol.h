/////////////////////////////////////////////////////////////////////////////
// Name:        ckeyboardcontrol.h
// Purpose:  
// Author:      Cesar Mauri Loba (cesar at crea-si dot com)
// Modified by: 
// Created:     28/09/2010
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
#ifndef KEYBOARDCONTROL_H
#define KEYBOARDCONTROL_H

#include <wx/string.h>
#include "ckeyboardcode.h"

class CKeyboardControl
{
public:
	static CKeyboardCode ReadKeyCode();
	static void SendKeyboardCode(const CKeyboardCode& kc);
	static wxString GetKeyboardCodeName(const CKeyboardCode& kc);
};

#endif
