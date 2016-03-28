/////////////////////////////////////////////////////////////////////////////
// Name:        keyboardcode.cpp
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
#include "keyboardcode.h"

#include <wx/utils.h>
#include <wx/stdpaths.h>
#include <wx/defs.h>

#if defined(WIN32)
	#define _WIN32_WINNT 0x0500
	#include <windows.h>
#else
	#include <X11/Xlib.h>
	#include <X11/extensions/XTest.h>
	#define XK_MISCELLANY
	#include <X11/keysymdef.h>
	#include <X11/XKBlib.h>
#endif

#include "simplelog.h"


wxString KeyboardCode::GetName() const {
	#if defined(__WXGTK__)
		return wxString(XKeysymToString((KeySym) m_nativeKeyCode), wxConvLocal);
	#else
		UINT scanCode= ::MapVirtualKeyW(m_nativeKeyCode, MAPVK_VK_TO_VSC);
		scanCode= scanCode << 16;
		char buffer[100] = { 0 };
		::GetKeyNameTextA (scanCode, buffer, sizeof(buffer));
		buffer[sizeof(buffer)-1]= 0;
		return wxString(buffer, wxConvLocal);
	#endif
}

void KeyboardCode::SendKey() {
	#if defined(__WXGTK__)
		KeyCode k= XKeysymToKeycode((Display *) wxGetDisplay(), (KeySym) m_nativeKeyCode);
		XTestFakeKeyEvent((Display *) wxGetDisplay(), k, true, 0);
		XTestFakeKeyEvent((Display *) wxGetDisplay(), k, false, 0);
	#else
		// It seems that, to work properly, SendInput() need both the virtual-key
		// code and the scan code. Note that KEYEVENTF_EXTENDEDKEY flag is not used.
		UINT scanCode = ::MapVirtualKey(m_nativeKeyCode, MAPVK_VK_TO_VSC);

		INPUT ip;
		memset(&ip, 0, sizeof(ip));
		ip.type= INPUT_KEYBOARD;
		ip.ki.wVk= m_nativeKeyCode;
		ip.ki.wScan= (WORD) scanCode;

		// Key press
		SendInput(1, &ip, sizeof(ip));

		// Key release
		ip.ki.dwFlags|= KEYEVENTF_KEYUP;
		SendInput(1, &ip, sizeof(ip));
	#endif
}

KeyboardCode KeyboardCode::ReadKeyCode() {
	#if defined(__WXGTK__)
		char keys_return[32];

		XQueryKeymap(((Display *) wxGetDisplay()), keys_return);
	
		// Given the keyboard state bitmap, return the scan code
		// of the key is pressed down
		// BUG: when multiple keys are pressed simultaneously
		// the scan code is undefined
		unsigned int kc = 0;
		for(int i=0; i<32; i++) {
			unsigned char keys = (unsigned char) keys_return[i];
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
		// NOT implemented for Windows. This method is used to poll
		// the keyboard to implement hotkeys. On Windows RegisterHotKey is used
		assert(0);
		return KeyboardCode();
	#endif
}

KeyboardCode KeyboardCode::FromChar (char c) {
#if defined(__WXGTK__)
		// Try to translate the one char string to a valid KeySym. If no
		// suitable conversion is available, XStringToKeysym returns
		// NoSymbol which equals to 0.
		char str[2]= { c, '\0'};
		KeySym ks = XStringToKeysym(str);
		return KeyboardCode (static_cast<int>(ks));
#else
		// VkKeyScan translates a character to the corresponding virtual-key
		// code and shift state for the current keyboard. If the function
		// succeeds, the low-order byte of the return value contains the
		// virtual-key code and the high-order byte contains the shift state
		short vkey = VkKeyScanA(c);

		// Clean shift state
		return KeyboardCode(vkey & 0xFF);
#endif
}


// Given an scan code returns the corresponding CKeyboardCode object
KeyboardCode KeyboardCode::FromScanCode (int scanCode) {
	#if defined(__WXGTK__)
		KeySym ks= XkbKeycodeToKeysym(
				static_cast<Display *>(wxGetDisplay()),
				static_cast<KeyCode>(scanCode), 0, 0);
		return KeyboardCode(static_cast<int>(ks));
	#else
		UINT vKCode= ::MapVirtualKey(scanCode, MAPVK_VSC_TO_VK_EX);
		assert (vKCode< 0xFFFF);
		return KeyboardCode(vKCode);
	#endif
}

// Given an wxKeyCode code returns the corresponding CKeyboardCode object
KeyboardCode KeyboardCode::FromWXK (wxKeyCode kc) {

	// If the value is between 0..255 corresponds to the characters of the
	// current locale, in particular the 32..127 subrange is for the ASCII
	// symbols. Therefore, try to translate as such.
	if (kc>= 0 && kc <= 255) {
		return FromChar(static_cast<char>(kc));
	}

	// Otherwise the translation is platform dependent
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

			default:
				return KeyboardCode(0);
		}

	#else
		switch (kc) {
			case WXK_BACK: return KeyboardCode(VK_BACK);
			case WXK_TAB: return KeyboardCode(VK_TAB);
			case WXK_RETURN: return KeyboardCode(VK_RETURN);
			case WXK_ESCAPE: return KeyboardCode(VK_ESCAPE);
			case WXK_DELETE: return KeyboardCode(VK_DELETE);
			case WXK_CLEAR: return KeyboardCode(VK_CLEAR);
			case WXK_SHIFT: return KeyboardCode(VK_SHIFT);
			case WXK_ALT: return KeyboardCode(VK_MENU);
			case WXK_CONTROL: return KeyboardCode(VK_CONTROL);
			case WXK_MENU: return KeyboardCode(VK_MENU);
			case WXK_PAUSE: return KeyboardCode(VK_PAUSE);
			case WXK_END: return KeyboardCode(VK_END);
			case WXK_HOME: return KeyboardCode(VK_HOME);
			case WXK_LEFT: return KeyboardCode(VK_LEFT);
			case WXK_UP: return KeyboardCode(VK_UP);
			case WXK_RIGHT: return KeyboardCode(VK_RIGHT);
			case WXK_DOWN: return KeyboardCode(VK_DOWN);
			case WXK_SELECT: return KeyboardCode(VK_SELECT);
			case WXK_PRINT: return KeyboardCode(VK_PRINT);
			case WXK_EXECUTE: return KeyboardCode(VK_EXECUTE);
			case WXK_SNAPSHOT: return KeyboardCode(VK_SNAPSHOT);
			case WXK_INSERT: return KeyboardCode(VK_INSERT);
			case WXK_HELP: return KeyboardCode(VK_HELP);
			case WXK_NUMPAD0: return KeyboardCode(VK_NUMPAD0);
			case WXK_NUMPAD1: return KeyboardCode(VK_NUMPAD1);
			case WXK_NUMPAD2: return KeyboardCode(VK_NUMPAD2);
			case WXK_NUMPAD3: return KeyboardCode(VK_NUMPAD3);
			case WXK_NUMPAD4: return KeyboardCode(VK_NUMPAD4);
			case WXK_NUMPAD5: return KeyboardCode(VK_NUMPAD5);
			case WXK_NUMPAD6: return KeyboardCode(VK_NUMPAD6);
			case WXK_NUMPAD7: return KeyboardCode(VK_NUMPAD7);
			case WXK_NUMPAD8: return KeyboardCode(VK_NUMPAD8);
			case WXK_NUMPAD9: return KeyboardCode(VK_NUMPAD9);
			case WXK_MULTIPLY: return KeyboardCode(VK_MULTIPLY);
			case WXK_ADD: return KeyboardCode(VK_ADD);
			case WXK_SEPARATOR: return KeyboardCode(VK_SEPARATOR);
			case WXK_SUBTRACT: return KeyboardCode(VK_SUBTRACT);
			case WXK_DECIMAL: return KeyboardCode(VK_DECIMAL);
			case WXK_DIVIDE: return KeyboardCode(VK_DIVIDE);
			case WXK_F1: return KeyboardCode(VK_F1);
			case WXK_F2: return KeyboardCode(VK_F2);
			case WXK_F3: return KeyboardCode(VK_F3);
			case WXK_F4: return KeyboardCode(VK_F4);
			case WXK_F5: return KeyboardCode(VK_F5);
			case WXK_F6: return KeyboardCode(VK_F6);
			case WXK_F7: return KeyboardCode(VK_F7);
			case WXK_F8: return KeyboardCode(VK_F8);
			case WXK_F9: return KeyboardCode(VK_F9);
			case WXK_F10: return KeyboardCode(VK_F10);
			case WXK_F11: return KeyboardCode(VK_F11);
			case WXK_F12: return KeyboardCode(VK_F12);
			case WXK_F13: return KeyboardCode(VK_F13);
			case WXK_F14: return KeyboardCode(VK_F14);
			case WXK_F15: return KeyboardCode(VK_F15);
			case WXK_F16: return KeyboardCode(VK_F16);
			case WXK_F17: return KeyboardCode(VK_F17);
			case WXK_F18: return KeyboardCode(VK_F18);
			case WXK_F19: return KeyboardCode(VK_F19);
			case WXK_F20: return KeyboardCode(VK_F20);
			case WXK_F21: return KeyboardCode(VK_F21);
			case WXK_F22: return KeyboardCode(VK_F22);
			case WXK_F23: return KeyboardCode(VK_F23);
			case WXK_F24: return KeyboardCode(VK_F24);
			case WXK_NUMLOCK: return KeyboardCode(VK_NUMLOCK);
			case WXK_SCROLL: return KeyboardCode(VK_SCROLL);
			case WXK_PAGEUP: return KeyboardCode(VK_PRIOR);
			case WXK_PAGEDOWN: return KeyboardCode(VK_NEXT);
			case WXK_NUMPAD_SPACE: return KeyboardCode(VK_SPACE);
			case WXK_NUMPAD_TAB: return KeyboardCode(VK_TAB);
			case WXK_NUMPAD_ENTER: return KeyboardCode(VK_RETURN);
			case WXK_NUMPAD_F1: return KeyboardCode(VK_F1);
			case WXK_NUMPAD_F2: return KeyboardCode(VK_F2);
			case WXK_NUMPAD_F3: return KeyboardCode(VK_F3);
			case WXK_NUMPAD_F4: return KeyboardCode(VK_F4);
			case WXK_NUMPAD_HOME: return KeyboardCode(VK_HOME);
			case WXK_NUMPAD_LEFT: return KeyboardCode(VK_LEFT);
			case WXK_NUMPAD_UP: return KeyboardCode(VK_UP);
			case WXK_NUMPAD_RIGHT: return KeyboardCode(VK_RIGHT);
			case WXK_NUMPAD_DOWN: return KeyboardCode(VK_DOWN);
			case WXK_NUMPAD_PAGEUP: return KeyboardCode(VK_PRIOR);
			case WXK_NUMPAD_PAGEDOWN: return KeyboardCode(VK_NEXT);
			case WXK_NUMPAD_END: return KeyboardCode(VK_END);
			case WXK_NUMPAD_BEGIN: return KeyboardCode(VK_NUMPAD5);
			case WXK_NUMPAD_INSERT: return KeyboardCode(VK_INSERT);
			case WXK_NUMPAD_DELETE: return KeyboardCode(VK_DELETE);
			case WXK_NUMPAD_MULTIPLY: return KeyboardCode(VK_MULTIPLY);
			case WXK_NUMPAD_ADD: return KeyboardCode(VK_ADD);
			case WXK_NUMPAD_SEPARATOR: return KeyboardCode(VK_SEPARATOR);
			case WXK_NUMPAD_SUBTRACT: return KeyboardCode(VK_SUBTRACT);
			case WXK_NUMPAD_DECIMAL: return KeyboardCode(VK_DECIMAL);
			case WXK_NUMPAD_DIVIDE: return KeyboardCode(VK_DIVIDE);

			// the following key codes are only generated under Windows currently
			case WXK_WINDOWS_LEFT: return KeyboardCode(VK_LWIN);
			case WXK_WINDOWS_RIGHT: return KeyboardCode(VK_RWIN);
			case WXK_WINDOWS_MENU: return KeyboardCode(VK_APPS);
			case WXK_SPACE: return KeyboardCode(VK_SPACE);
			case WXK_CANCEL: return KeyboardCode(VK_CANCEL);
			case WXK_CAPITAL: return KeyboardCode(VK_CAPITAL);

			default:
				return KeyboardCode(0);
		}
	#endif
}

#if defined(DEBUG)
void KeyboardCode::Dump() const {
	#if !defined(WIN32)
		slog_write (SLOG_PRIO_DEBUG,
			"KeySym: %d, KeyCode: %u Name: %s\n",
			m_nativeKeyCode, XKeysymToKeycode((Display *) wxGetDisplay(),
				m_nativeKeyCode),
			static_cast<const char*>(GetName().mb_str()));
	#endif
}
#endif

