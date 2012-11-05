/////////////////////////////////////////////////////////////////////////////
// Name:        ckeyboardcode.h
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

#include <wx/string.h>

class CKeyboardCode
{
public:
    CKeyboardCode();
	// Default destructor, copy constructor and copy operator are fine
	bool operator==(const CKeyboardCode &other) const {
		return m_virtualKeyCode== other.m_virtualKeyCode;
	}

	bool operator!=(const CKeyboardCode &other) const {
		return !(*this== other);
	} 
	
	// Get its readable name
	const char* GetName() const;

	// Send a keystoke to the system
	void SendKey();

	bool IsValid() const { return m_virtualKeyCode!= 0; }
	
	// Reads a keycode from keyboard and returns a CKeyboardCode object
	static CKeyboardCode ReadKeyCode();

	// Given an scan code returns the corresponding CKeyboardCode object
	static CKeyboardCode FromScanCode (unsigned int scanCode);

	// Given a platform dependent virtual-key or KeySym returns the 
	// corresponding CKeyboardCode object
	static CKeyboardCode FromVirtualKeyCode (unsigned long vkCode);

	// Given an ASCII code returns the corresponding CKeyboardCode object
	static CKeyboardCode FromASCII (char ascii);

	// Given an wxKeyCode code returns the corresponding CKeyboardCode object
	static CKeyboardCode FromWXKeyCode (wxKeyCode kc);

	// Set the internal raw value. NOTE: intended only
	// for storage purposes not to work with
	static CKeyboardCode FromRawValue (unsigned long kc);

	// Get the internal raw value. NOTE: intended only
	// for storage purposes not to work with
	unsigned long GetRawValue() const;

	void Dump() const;

private:
	// Stores the virtual key code. This corresponds to a unique key
	// after taking into account the keyboard layout (the keyboard
	// returns a scan code which, using the layout, is converted to 
	// this value). In Unix systems this corresponds to KeySym
	// while for Windows systems it is called virtual-key code.
	unsigned long m_virtualKeyCode;

	// Construct object from an KeySym or virtual-key
	CKeyboardCode(unsigned long vkCode);
};

#endif
