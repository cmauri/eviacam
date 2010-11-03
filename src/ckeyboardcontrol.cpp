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
//#include <wx/stdpaths.h>
#include <cmath>
#if defined(__WXGTK__)
#include <X11/extensions/XTest.h>
#include <X11/Xlib.h>
#endif

CKeyboardCode CKeyboardControl::ReadKeyCode()
{
#if defined(__WXGTK__)
	char keys_return[32];
	unsigned char keys;
	KeyCode kc = 0;
	
	XQueryKeymap(((Display *) wxGetDisplay()), keys_return);

	for(int i=0; i<32; i++) {
		keys = (unsigned char) keys_return[i];
		if (keys > 0) {
			switch (keys) {
				case 1:   kc = 0; break;
				case 2:   kc = 1; break;
				case 4:   kc = 2; break;
				case 8:   kc = 3; break;
				case 16:  kc = 4; break;
				case 32:  kc = 5; break;
				case 64:  kc = 6; break;
				case 128: kc = 7; break;
			}
			kc += 8 * i;
		}
	}
	return CKeyboardCode(kc);
#else
	return CKeyboardCode();
#endif
}

void CKeyboardControl::SendKeyboardCode(const CKeyboardCode& kc)
{
#if defined(__WXGTK__)
	XTestFakeKeyEvent((Display *) wxGetDisplay(), kc.GetKeyboardCode(), true, 0);
	XTestFakeKeyEvent((Display *) wxGetDisplay(), kc.GetKeyboardCode(), false, 0);
#else
	assert(0);
#endif
}


wxString CKeyboardControl::GetKeyboardCodeName(const CKeyboardCode& kc)
{
	wxString name = wxT("");
#if defined(__WXGTK__)
	KeySym ks = XKeycodeToKeysym((Display *) wxGetDisplay(), kc.GetKeyboardCode(), 0);
	name = wxString(XKeysymToString(ks), wxConvLocal);
#endif
	return name;
}

int CKeyboardControl::GetKeyCode(const CKeyboardCode& kc)
{
	int code = -1;
#if defined(__WXGTK__)
	code = (int) kc.GetKeyboardCode();
#endif
	return code;
}

bool CKeyboardControl::Equal(const CKeyboardCode& kc1, const CKeyboardCode& kc2)
{
	return (kc1.GetKeyboardCode() == kc2.GetKeyboardCode());
}
