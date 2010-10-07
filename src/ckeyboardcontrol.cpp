/////////////////////////////////////////////////////////////////////////////
// Name:        ckeyboardcontrol.cpp
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
#include "ckeyboardcontrol.h"
#include <wx/utils.h>
#include <cmath>
#include <wx/stdpaths.h>
#if defined(__WXGTK__)
#include <X11/extensions/XTest.h>
#include <X11/Xlib.h>
#endif

void CKeyboardControl::SendKeyCode(KeyCode kc)
{
#if defined(__WXGTK__)
	XTestFakeKeyEvent((Display *) wxGetDisplay(), kc, true, 0);
	XTestFakeKeyEvent((Display *) wxGetDisplay(), kc, false, 0);
#endif
}

KeyCode CKeyboardControl::ReadKeyCode()
{
	char keys_return[32];
	unsigned char keys;
	KeyCode kc = 0;
#if defined(__WXGTK__)
	XQueryKeymap(((Display *) wxGetDisplay()), keys_return);

	for(int i=0; i<32; i++) {
		keys = (unsigned char) keys_return[i];
		if (keys > 0) {
			kc = (log(keys) / log(2)) + (8 * i);
		}
	}
#endif
	return kc;
}

wxString CKeyboardControl::GetKeyCodeName(KeyCode kc)
{
	wxString name = wxT("");
#if defined(__WXGTK__)
	KeySym ks = XKeycodeToKeysym((Display *) wxGetDisplay(), kc, 0);
	name = wxString(XKeysymToString(ks), wxConvLocal);
#endif
	return name;
}



/*void CKeyboardControl::SendChar(char* c)
{
	KeyCode kc = XKeysymToKeycode((Display *) wxGetDisplay(), XStringToKeysym(c));
	SendKeyCode(kc);
}*/

/*void CKeyboardControl::SendKeyboardCode(CKeyboardCode kc)
{
	XTestFakeKeyEvent((Display *) wxGetDisplay(), kc.GetKeyboardCode(), true, 0);
	XTestFakeKeyEvent((Display *) wxGetDisplay(), kc.GetKeyboardCode(), false, 0);
}*/
	
/*char* CKeyboardControl::ReadChar()
{
	KeyCode kc = ReadKeyCode();
	if (kc != 0)
		return GetKeyCodeName(kc).mb_str();
	else
		return NULL;
}*/
