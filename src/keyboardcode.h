/////////////////////////////////////////////////////////////////////////////
// Name:        keyboardcode.h
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
#ifndef KEYBOARDCODE_H
#define KEYBOARDCODE_H

#include <wx/event.h> // for wxKeyCode
#include <wx/string.h>

// Store a keyboard code which is a unique key identifier after
// taking into account the keyboard layout.
//
// The code is stored in its native format (KeySym on X11 and as
// virtual key code on Windows)
//
// Several conversion methods are provided
//
// This class has only one integer in its layout (is cheap)
//
// TODO: for X11, consider storing a KeyCode instead of a KeySym.
// https://tronche.com/gui/x/xlib/input/keyboard-encoding.html
// https://wiki.archlinux.org/index.php/extra_keyboard_keys


class KeyboardCode {

public:
    // Default constructor: initializes internal keycode to a invalid value
    KeyboardCode() : m_nativeKeyCode(0) { }

	// Construct object from its native value
	KeyboardCode(int v) : m_nativeKeyCode(v) { }

   	// Default destructor, copy constructor and copy operator are fine

	bool operator==(const KeyboardCode &other) const {
		return m_nativeKeyCode== other.m_nativeKeyCode;
	}

	bool operator!=(const KeyboardCode &other) const {
		return !(*this== other);
	} 
	
	// Get its native value (KeySym on X11, virtual key codes)
	int get_native() const { return m_nativeKeyCode; }

	bool is_valud() const { return m_nativeKeyCode!= 0; }

	// Get its readable name
	wxString GetName() const;

	// Send a keystoke to the system
	void SendKey();

	// Reads a keycode from keyboard and returns a KeyboardCode object
	static KeyboardCode ReadKeyCode();

	// Given an char returns the corresponding KeyboardCode object
	// Currently ignores shift state (SHIFT, CTRL, ALT, etc.)
	static KeyboardCode FromChar (char c);

	// Given an scan code returns the corresponding KeyboardCode object
	static KeyboardCode FromScanCode (int scanCode);

	// Given an wxKeyCode code returns the corresponding KeyboardCode object
	static KeyboardCode FromWXK (wxKeyCode kc);


#if defined(DEBUG)
	void Dump() const;
#endif

private:
	// Stores the native value for virtual key codes. This corresponds to a
	// unique key after taking into account the keyboard layout (the keyboard
	// returns a scan code which, using the layout, is converted to this
	// value). In Unix systems this corresponds to KeySym. For Windows
	// systems it is called virtual-key code. According to:
	// https://cgit.freedesktop.org/xorg/proto/x11proto/plain/keysymdef.h
	// KeySym is a 29-bit value and according to
	// https://msdn.microsoft.com/en-us//library/windows/desktop/dd375731(v=vs.85).aspx
	// it seems that virtual key codes are 8-bit values. Therefore, we store
	// it as 32-bit value
	int m_nativeKeyCode;
};

#endif // KEYBOARDCODE_H
