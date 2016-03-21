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

#include "simplelog.h"

// Make sure that m_virtualKeyCode is wide enough to store a KeySym
// On windows it seems that virtual-key codes are 16bit wide, so no problem.
#if defined(__WXGTK__)
wxCOMPILE_TIME_ASSERT( sizeof(KeySym)== sizeof(unsigned long), KeySymDifferentThanLongError );
wxCOMPILE_TIME_ASSERT( sizeof(KeyCode)<= sizeof(unsigned int), KeyCodeGreaterThanIntError );
#endif

// Default constructor. Initializes internal keycode to a invalid value
KeyboardCode::KeyboardCode() : m_virtualKeyCode(0)
{	
}

// Private constructor which initializes to the internal 
// state given virtual key code or KeySym
KeyboardCode::KeyboardCode (unsigned long vkCode)
{
	m_virtualKeyCode= vkCode;	
}

const char* KeyboardCode::GetName() const
{	 
#if defined(__WXGTK__)	
	return XKeysymToString((KeySym) m_virtualKeyCode);
#else
	// Still not available for Windows
	return "";
#endif	
}

void KeyboardCode::SendKey()
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

// Reads a keycode from keyboard and returns a CKeyboardCode object
KeyboardCode KeyboardCode::ReadKeyCode()
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
	return KeyboardCode();
#endif
}

// Given an scan code returns the corresponding CKeyboardCode object
KeyboardCode KeyboardCode::FromScanCode (unsigned int scanCode)
{	
#if defined(__WXGTK__)
	return KeyboardCode (static_cast<unsigned long> (XKeycodeToKeysym(
			(Display *) wxGetDisplay(), (KeyCode) scanCode, 0)));
#else
	// Still not available for Windows
	wxUnusedVar(scanCode);
	assert(false);
	return KeyboardCode();
#endif
}

// Given a platform dependent virtual-key or KeySym returns the 
// corresponding CKeyboardCode object
KeyboardCode KeyboardCode::FromVirtualKeyCode (unsigned long vkCode)
{
	return KeyboardCode (vkCode);
}

// Given an ASCII code returns the corresponding CKeyboardCode object
KeyboardCode KeyboardCode::FromASCII (char ascii)
{
#if defined(__WXGTK__)
	char c[2]= { ascii, '\0'};
	return KeyboardCode (static_cast<unsigned long>(XStringToKeysym(c)));
#else
	// Still not available for Windows
	assert(0);
	wxUnusedVar(ascii);
	return KeyboardCode();
#endif	
}

void KeyboardCode::Dump() const
{
#if !defined(WIN32)
	slog_write (SLOG_PRIO_DEBUG,
		"KeySym: %lu	KeyCode: %u Name: %s\n", 
		m_virtualKeyCode, XKeysymToKeycode((Display *) wxGetDisplay(), m_virtualKeyCode), GetName());
#endif
}

// Given an wxKeyCode code returns the corresponding CKeyboardCode object
KeyboardCode KeyboardCode::FromWXKeyCode (wxKeyCode kc)
{
#if defined(__WXGTK__)
	switch (kc) {
		case WXK_BACK: return KeyboardCode(XK_BackSpace);
		case WXK_TAB: return KeyboardCode(XK_Tab);
		case WXK_RETURN: return KeyboardCode(XK_Return);
		case WXK_ESCAPE: return KeyboardCode(XK_Escape);		
		case WXK_DELETE: return KeyboardCode(XK_Delete);
		case WXK_CLEAR: return KeyboardCode(XK_Clear);
		case WXK_SHIFT: return KeyboardCode(XK_Shift_L);
		case WXK_ALT: return KeyboardCode(XK_Alt_L);
		case WXK_CONTROL: return KeyboardCode(XK_Control_L);
		case WXK_MENU: return KeyboardCode(XK_Menu);
		case WXK_PAUSE: return KeyboardCode(XK_Pause);
		case WXK_END: return KeyboardCode(XK_End);
		case WXK_HOME: return KeyboardCode(XK_Home);
		case WXK_LEFT: return KeyboardCode(XK_Left);
		case WXK_UP: return KeyboardCode(XK_Up);
		case WXK_RIGHT: return KeyboardCode(XK_Right);
		case WXK_DOWN: return KeyboardCode(XK_Down);
		case WXK_SELECT: return KeyboardCode(XK_Select);
		case WXK_PRINT: return KeyboardCode(XK_Print);
		case WXK_EXECUTE: return KeyboardCode(XK_Execute);
		case WXK_SNAPSHOT: return KeyboardCode(XK_Print);
		case WXK_INSERT: return KeyboardCode(XK_Insert);
		case WXK_HELP: return KeyboardCode(XK_Help);
		case WXK_NUMPAD0: return KeyboardCode(XK_KP_0);
		case WXK_NUMPAD1: return KeyboardCode(XK_KP_1);
		case WXK_NUMPAD2: return KeyboardCode(XK_KP_2);
		case WXK_NUMPAD3: return KeyboardCode(XK_KP_3);
		case WXK_NUMPAD4: return KeyboardCode(XK_KP_4);
		case WXK_NUMPAD5: return KeyboardCode(XK_KP_5);
		case WXK_NUMPAD6: return KeyboardCode(XK_KP_6);
		case WXK_NUMPAD7: return KeyboardCode(XK_KP_7);
		case WXK_NUMPAD8: return KeyboardCode(XK_KP_8);
		case WXK_NUMPAD9: return KeyboardCode(XK_KP_9);
		case WXK_MULTIPLY: return KeyboardCode(XK_KP_Multiply);
		case WXK_ADD: return KeyboardCode(XK_KP_Add);
		case WXK_SEPARATOR: return KeyboardCode(XK_KP_Separator);
		case WXK_SUBTRACT: return KeyboardCode(XK_KP_Subtract);
		case WXK_DECIMAL: return KeyboardCode(XK_KP_Decimal);
		case WXK_DIVIDE: return KeyboardCode(XK_KP_Divide);
		case WXK_F1: return KeyboardCode(XK_F1);
		case WXK_F2: return KeyboardCode(XK_F2);
		case WXK_F3: return KeyboardCode(XK_F3);
		case WXK_F4: return KeyboardCode(XK_F4);
		case WXK_F5: return KeyboardCode(XK_F5);
		case WXK_F6: return KeyboardCode(XK_F6);
		case WXK_F7: return KeyboardCode(XK_F7);
		case WXK_F8: return KeyboardCode(XK_F8);
		case WXK_F9: return KeyboardCode(XK_F9);
		case WXK_F10: return KeyboardCode(XK_F10);
		case WXK_F11: return KeyboardCode(XK_F11);
		case WXK_F12: return KeyboardCode(XK_F12);
		case WXK_F13: return KeyboardCode(XK_F13);
		case WXK_F14: return KeyboardCode(XK_F14);
		case WXK_F15: return KeyboardCode(XK_F15);
		case WXK_F16: return KeyboardCode(XK_F16);
		case WXK_F17: return KeyboardCode(XK_F17);
		case WXK_F18: return KeyboardCode(XK_F18);
		case WXK_F19: return KeyboardCode(XK_F19);
		case WXK_F20: return KeyboardCode(XK_F20);
		case WXK_F21: return KeyboardCode(XK_F21);
		case WXK_F22: return KeyboardCode(XK_F22);
		case WXK_F23: return KeyboardCode(XK_F23);
		case WXK_F24: return KeyboardCode(XK_F24);
		case WXK_NUMLOCK: return KeyboardCode(XK_Num_Lock);
		case WXK_SCROLL: return KeyboardCode(XK_Scroll_Lock);
		case WXK_PAGEUP: return KeyboardCode(XK_Page_Up);
		case WXK_PAGEDOWN: return KeyboardCode(XK_Page_Down); 
		case WXK_NUMPAD_SPACE: return KeyboardCode(XK_KP_Space);
		case WXK_NUMPAD_TAB: return KeyboardCode(XK_KP_Tab);
		case WXK_NUMPAD_ENTER: return KeyboardCode(XK_KP_Enter);
		case WXK_NUMPAD_F1: return KeyboardCode(XK_KP_F1);
		case WXK_NUMPAD_F2: return KeyboardCode(XK_KP_F2);
		case WXK_NUMPAD_F3: return KeyboardCode(XK_KP_F3);
		case WXK_NUMPAD_F4: return KeyboardCode(XK_KP_F4);
		case WXK_NUMPAD_HOME: return KeyboardCode(XK_KP_Home);
		case WXK_NUMPAD_LEFT: return KeyboardCode(XK_KP_Left);
		case WXK_NUMPAD_UP: return KeyboardCode(XK_KP_Up);
		case WXK_NUMPAD_RIGHT: return KeyboardCode(XK_KP_Right);
		case WXK_NUMPAD_DOWN: return KeyboardCode(XK_KP_Down);
		case WXK_NUMPAD_PAGEUP: return KeyboardCode(XK_KP_Page_Up);
		case WXK_NUMPAD_PAGEDOWN: return KeyboardCode(XK_KP_Page_Down);
		case WXK_NUMPAD_END: return KeyboardCode(XK_KP_End);
		case WXK_NUMPAD_BEGIN: return KeyboardCode(XK_KP_Begin);
		case WXK_NUMPAD_INSERT: return KeyboardCode(XK_KP_Insert);
		case WXK_NUMPAD_DELETE: return KeyboardCode(XK_KP_Delete);
		case WXK_NUMPAD_EQUAL: return KeyboardCode(XK_KP_Equal);
		case WXK_NUMPAD_MULTIPLY: return KeyboardCode(XK_KP_Multiply);
		case WXK_NUMPAD_ADD: return KeyboardCode(XK_KP_Add);
		case WXK_NUMPAD_SEPARATOR: return KeyboardCode(XK_KP_Separator);
		case WXK_NUMPAD_SUBTRACT: return KeyboardCode(XK_KP_Subtract);
		case WXK_NUMPAD_DECIMAL: return KeyboardCode(XK_KP_Decimal);
		case WXK_NUMPAD_DIVIDE: return KeyboardCode(XK_KP_Divide);		
		// the following key codes are only generated under Windows currently
		case WXK_WINDOWS_LEFT: return KeyboardCode(XK_Super_L);
		case WXK_WINDOWS_RIGHT: return KeyboardCode(XK_Super_R);
		case WXK_WINDOWS_MENU: return KeyboardCode(XK_Menu);

		//
		// Untranslated codes
		//
		case WXK_SPACE:
		case WXK_START:
		case WXK_LBUTTON:
		case WXK_RBUTTON:
		case WXK_CANCEL:
		case WXK_MBUTTON:

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
			assert (false);
			return KeyboardCode(0);
			break;
	}

	// If the code reaches this point it means that wxKeyCode kc carries 
	// an unicode character (see wxKeyEvent::GetUnicodeKey)
	// TODO: translate these unicode char to a keysym
	return KeyboardCode(kc);
#else
	// Still not available for Windows
//	assert(0);
	wxUnusedVar(kc);
	return KeyboardCode();
#endif
}

KeyboardCode KeyboardCode::FromRawValue (unsigned long kc)
{
	return KeyboardCode(kc);
}

// Get the internal raw value. NOTE: intended only
// for storage purposes not to work with
unsigned long KeyboardCode::GetRawValue() const
{
	return m_virtualKeyCode;
}
