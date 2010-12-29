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
#include <wx/defs.h>

#if defined(__WXGTK__)
#include <X11/Xlib.h>
#include <X11/extensions/XTest.h>
#define XK_MISCELLANY
#include <X11/keysymdef.h>
#endif

// Make sure that m_virtualKeyCode is wide enough to store a KeySym
// On windows it seems that virtual-key codes are 16bit wide, so no problem.
#if defined(__WXGTK__)
#include <boost/static_assert.hpp>
BOOST_STATIC_ASSERT(sizeof(KeySym)<= sizeof(unsigned int));
BOOST_STATIC_ASSERT(sizeof(KeyCode)<= sizeof(unsigned int));
#endif

// Default constructor. Initializes internal keycode to a invalid value
CKeyboardCode::CKeyboardCode() : m_virtualKeyCode(0)
{	
}

// Private constructor which initializes to the internal 
// state given a raw scan code
CKeyboardCode::CKeyboardCode (unsigned int vkCode)
{
	m_virtualKeyCode= vkCode;
}

const char* CKeyboardCode::GetName() const
{	 
#if defined(__WXGTK__)
	return XKeysymToString((KeySym) m_virtualKeyCode);
#else
	// Still not available for Windows
	return "";
#endif	
}

void CKeyboardCode::SendKey()
{
#if defined(__WXGTK__)
	KeyCode k= XKeysymToKeycode((Display *) wxGetDisplay(), (KeySym) m_virtualKeyCode);
	XTestFakeKeyEvent((Display *) wxGetDisplay(), k, true, 0);
	XTestFakeKeyEvent((Display *) wxGetDisplay(), k, false, 0);
#else
	// Still not available for Windows
	assert(0);
#endif
}

// Get the internal raw value. NOTE: intended only
// for storage purposes not to work with
unsigned int CKeyboardCode::GetRawValue() const
{
	return m_virtualKeyCode;
}

// Set the internal raw value. NOTE: intended only
// for storage purposes not to work with
void CKeyboardCode::SetRawValue(unsigned int value)
{
	m_virtualKeyCode= value;
}

// Reads a keycode from keyboard and returns a CKeyboardCode object
CKeyboardCode CKeyboardCode::ReadKeyCode()
{
#if defined(__WXGTK__)
	char keys_return[32];
	unsigned char keys;
	unsigned int kc = 0;
	
	XQueryKeymap(((Display *) wxGetDisplay()), keys_return);

	// TODO: implement this correctly
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
	return FromScanCode (kc);
#else
	// Still not available for Windows
	assert(0);
	return CKeyboardCode();
#endif
}

// Given an scan code returns the corresponding CKeyboardCode object
CKeyboardCode CKeyboardCode::FromScanCode (unsigned int scanCode)
{	
#if defined(__WXGTK__)
	return CKeyboardCode ((unsigned int) XKeycodeToKeysym(
			(Display *) wxGetDisplay(), (KeyCode) scanCode, 0));
#else
	// Still not available for Windows
	wxUnusedVar(scanCode);
	assert(false);
	return CKeyboardCode();
#endif
}

// Given an ASCII code returns the corresponding CKeyboardCode object
CKeyboardCode CKeyboardCode::FromASCII (char ascii)
{
#if defined(__WXGTK__)
	char c[2]= { ascii, '\0'};
	return CKeyboardCode ((unsigned int) XStringToKeysym(c));
#else
	// Still not available for Windows
	assert(0);
	wxUnusedVar(ascii);
	return CKeyboardCode();
#endif	
}

#if !defined(NDEBUG)
void CKeyboardCode::Dump() const
{
#if !defined(WIN32)
	printf ("KeySym: %u	KeyCode: %u Name: %s\n", m_virtualKeyCode, XKeysymToKeycode((Display *) wxGetDisplay(), m_virtualKeyCode), GetName());
#endif
}
#endif

// Given an wxKeyCode code returns the corresponding CKeyboardCode object
CKeyboardCode CKeyboardCode::FromWXKeyCode (wxKeyCode kc)
{
#if defined(__WXGTK__)
	switch (kc) {
		case WXK_BACK: return CKeyboardCode(XK_BackSpace);
		case WXK_TAB: return CKeyboardCode(XK_Tab);
		case WXK_RETURN: return CKeyboardCode(XK_Return);
		case WXK_ESCAPE: return CKeyboardCode(XK_Escape);		
		case WXK_DELETE: return CKeyboardCode(XK_Delete);
		case WXK_CLEAR: return CKeyboardCode(XK_Clear);
		case WXK_SHIFT: return CKeyboardCode(XK_Shift_L);
		case WXK_ALT: return CKeyboardCode(XK_Alt_L);
		case WXK_CONTROL: return CKeyboardCode(XK_Control_L);
		case WXK_MENU: return CKeyboardCode(XK_Menu);
		case WXK_PAUSE: return CKeyboardCode(XK_Pause);
		case WXK_END: return CKeyboardCode(XK_End);
		case WXK_HOME: return CKeyboardCode(XK_Home);
		case WXK_LEFT: return CKeyboardCode(XK_Left);
		case WXK_UP: return CKeyboardCode(XK_Up);
		case WXK_RIGHT: return CKeyboardCode(XK_Right);
		case WXK_DOWN: return CKeyboardCode(XK_Down);
		case WXK_SELECT: return CKeyboardCode(XK_Select);
		case WXK_PRINT: return CKeyboardCode(XK_Print);
		case WXK_EXECUTE: return CKeyboardCode(XK_Execute);
		case WXK_SNAPSHOT: return CKeyboardCode(XK_Print);
		case WXK_INSERT: return CKeyboardCode(XK_Insert);
		case WXK_HELP: return CKeyboardCode(XK_Help);
		case WXK_NUMPAD0: return CKeyboardCode(XK_KP_0);
		case WXK_NUMPAD1: return CKeyboardCode(XK_KP_1);
		case WXK_NUMPAD2: return CKeyboardCode(XK_KP_2);
		case WXK_NUMPAD3: return CKeyboardCode(XK_KP_3);
		case WXK_NUMPAD4: return CKeyboardCode(XK_KP_4);
		case WXK_NUMPAD5: return CKeyboardCode(XK_KP_5);
		case WXK_NUMPAD6: return CKeyboardCode(XK_KP_6);
		case WXK_NUMPAD7: return CKeyboardCode(XK_KP_7);
		case WXK_NUMPAD8: return CKeyboardCode(XK_KP_8);
		case WXK_NUMPAD9: return CKeyboardCode(XK_KP_9);
		case WXK_MULTIPLY: return CKeyboardCode(XK_KP_Multiply);
		case WXK_ADD: return CKeyboardCode(XK_KP_Add);
		case WXK_SEPARATOR: return CKeyboardCode(XK_KP_Separator);
		case WXK_SUBTRACT: return CKeyboardCode(XK_KP_Subtract);
		case WXK_DECIMAL: return CKeyboardCode(XK_KP_Decimal);
		case WXK_DIVIDE: return CKeyboardCode(XK_KP_Divide);
		case WXK_F1: return CKeyboardCode(XK_F1);
		case WXK_F2: return CKeyboardCode(XK_F2);
		case WXK_F3: return CKeyboardCode(XK_F3);
		case WXK_F4: return CKeyboardCode(XK_F4);
		case WXK_F5: return CKeyboardCode(XK_F5);
		case WXK_F6: return CKeyboardCode(XK_F6);
		case WXK_F7: return CKeyboardCode(XK_F7);
		case WXK_F8: return CKeyboardCode(XK_F8);
		case WXK_F9: return CKeyboardCode(XK_F9);
		case WXK_F10: return CKeyboardCode(XK_F10);
		case WXK_F11: return CKeyboardCode(XK_F11);
		case WXK_F12: return CKeyboardCode(XK_F12);
		case WXK_F13: return CKeyboardCode(XK_F13);
		case WXK_F14: return CKeyboardCode(XK_F14);
		case WXK_F15: return CKeyboardCode(XK_F15);
		case WXK_F16: return CKeyboardCode(XK_F16);
		case WXK_F17: return CKeyboardCode(XK_F17);
		case WXK_F18: return CKeyboardCode(XK_F18);
		case WXK_F19: return CKeyboardCode(XK_F19);
		case WXK_F20: return CKeyboardCode(XK_F20);
		case WXK_F21: return CKeyboardCode(XK_F21);
		case WXK_F22: return CKeyboardCode(XK_F22);
		case WXK_F23: return CKeyboardCode(XK_F23);
		case WXK_F24: return CKeyboardCode(XK_F24);
		case WXK_NUMLOCK: return CKeyboardCode(XK_Num_Lock);
		case WXK_SCROLL: return CKeyboardCode(XK_Scroll_Lock);
		case WXK_PAGEUP: return CKeyboardCode(XK_Page_Up);
		case WXK_PAGEDOWN: return CKeyboardCode(XK_Page_Down); 
		case WXK_NUMPAD_SPACE: return CKeyboardCode(XK_KP_Space);
		case WXK_NUMPAD_TAB: return CKeyboardCode(XK_KP_Tab);
		case WXK_NUMPAD_ENTER: return CKeyboardCode(XK_KP_Enter);
		case WXK_NUMPAD_F1: return CKeyboardCode(XK_KP_F1);
		case WXK_NUMPAD_F2: return CKeyboardCode(XK_KP_F2);
		case WXK_NUMPAD_F3: return CKeyboardCode(XK_KP_F3);
		case WXK_NUMPAD_F4: return CKeyboardCode(XK_KP_F4);
		case WXK_NUMPAD_HOME: return CKeyboardCode(XK_KP_Home);
		case WXK_NUMPAD_LEFT: return CKeyboardCode(XK_KP_Left);
		case WXK_NUMPAD_UP: return CKeyboardCode(XK_KP_Up);
		case WXK_NUMPAD_RIGHT: return CKeyboardCode(XK_KP_Right);
		case WXK_NUMPAD_DOWN: return CKeyboardCode(XK_KP_Down);
		case WXK_NUMPAD_PAGEUP: return CKeyboardCode(XK_KP_Page_Up);
		case WXK_NUMPAD_PAGEDOWN: return CKeyboardCode(XK_KP_Page_Down);
		case WXK_NUMPAD_END: return CKeyboardCode(XK_KP_End);
		case WXK_NUMPAD_BEGIN: return CKeyboardCode(XK_KP_Begin);
		case WXK_NUMPAD_INSERT: return CKeyboardCode(XK_KP_Insert);
		case WXK_NUMPAD_DELETE: return CKeyboardCode(XK_KP_Delete);
		case WXK_NUMPAD_EQUAL: return CKeyboardCode(XK_KP_Equal);
		case WXK_NUMPAD_MULTIPLY: return CKeyboardCode(XK_KP_Multiply);
		case WXK_NUMPAD_ADD: return CKeyboardCode(XK_KP_Add);
		case WXK_NUMPAD_SEPARATOR: return CKeyboardCode(XK_KP_Separator);
		case WXK_NUMPAD_SUBTRACT: return CKeyboardCode(XK_KP_Subtract);
		case WXK_NUMPAD_DECIMAL: return CKeyboardCode(XK_KP_Decimal);
		case WXK_NUMPAD_DIVIDE: return CKeyboardCode(XK_KP_Divide);		
		// the following key codes are only generated under Windows currently
		case WXK_WINDOWS_LEFT: return CKeyboardCode(XK_Super_L);
		case WXK_WINDOWS_RIGHT: return CKeyboardCode(XK_Super_R);
		case WXK_WINDOWS_MENU: return CKeyboardCode(XK_Menu);

		//
		// Untranslated codes
		//
		case WXK_SPACE:
		case WXK_START:
		case WXK_LBUTTON:
		case WXK_RBUTTON:
		case WXK_CANCEL:
		case WXK_MBUTTON:
		case WXK_COMMAND:
		// Hardware-specific buttons
		case WXK_SPECIAL1:
		case WXK_SPECIAL2:
		case WXK_SPECIAL3:
		case WXK_SPECIAL4:
		case WXK_SPECIAL5:
		case WXK_SPECIAL6:
		case WXK_SPECIAL7:
		case WXK_SPECIAL8:
		case WXK_SPECIAL9:
		case WXK_SPECIAL10:
		case WXK_SPECIAL11:
		case WXK_SPECIAL12:
		case WXK_SPECIAL13:
		case WXK_SPECIAL14:
		case WXK_SPECIAL15:
		case WXK_SPECIAL16:
		case WXK_SPECIAL17:
		case WXK_SPECIAL18:
		case WXK_SPECIAL19:
		case WXK_SPECIAL20:
		case WXK_CAPITAL:
			break;
	}

	// If the code reaches this point it means that wxKeyCode kc carries 
	// an unicode character (see wxKeyEvent::GetUnicodeKey)
	// TODO: translate these unicode char to a keysym
	return CKeyboardCode();
#else
	// Still not available for Windows
	assert(0);
	wxUnusedVar(kc);
	return CKeyboardCode();
#endif
}
