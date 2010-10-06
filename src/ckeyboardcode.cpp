/////////////////////////////////////////////////////////////////////////////
// Name:        ckeyboardcode.cpp
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
#include "ckeyboardcode.h"
#include <wx/utils.h>
#include <wx/stdpaths.h>
#if defined(__WXGTK__)
#include <X11/Xlib.h>
#include <X11/extensions/XTest.h>
#endif

CKeyboardCode::CKeyboardCode()
{
	m_keyboardCode = NULL;
}

CKeyboardCode::CKeyboardCode(char key)
{
#if defined(__WXGTK__)
	char c[2];
	KeySym ksym;
	KeyCode kcode;
	Display *dpy = XOpenDisplay( NULL );
	c[0] = key;
	c[1] = '\0';
	
	ksym = XStringToKeysym(c);
	kcode = XKeysymToKeycode(dpy, ksym);
	m_keyboardCode = kcode;
#else
	m_keyboardCode = NULL;
#endif	
}

//CKeyboardCode::CKeyboardCode(int key)
//{
//}

CKeyboardCode::~CKeyboardCode()
{
}

void CKeyboardCode::SendKey()
{
#if defined(__WXGTK__)
	//CKeyboardControl::SendKeyCode(m_keyboardCode);
	XTestFakeKeyEvent(((Display *) wxGetDisplay()), m_keyboardCode, true, 0);
	XTestFakeKeyEvent(((Display *) wxGetDisplay()), m_keyboardCode, false, 0);
#endif
}

wxString CKeyboardCode::GetKeyName()
{
	wxString name = wxT("");
#if defined(__WXGTK__)
	//return CKeyboardControl::GetKeyCodeName(m_keyboardCode);
	KeySym ks = XKeycodeToKeysym(((Display *) wxGetDisplay()), m_keyboardCode, 0);
	name = wxString(XKeysymToString(ks), wxConvLocal);
#endif
	return name;
}
