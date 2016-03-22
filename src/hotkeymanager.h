/////////////////////////////////////////////////////////////////////////////
// Name:        hotkeymanager.h
// Author:      Cesar Mauri Loba (cesar at crea-si dot com)
// Copyright:   (C) 2010-16 Cesar Mauri Loba - CREA Software Systems
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
#ifndef HOTKEYMANAGER_H
#define HOTKEYMANAGER_H

#include <vector>

#include "configbase.h"
#include "keyboardcode.h"

namespace eviacam {

/**
 * Stores an arbitrary command associated with a key along with a description
 */
class KeyCommand {
public:
	KeyCommand (const wxString& name, const wxString& desc, KeyboardCode key, bool enabled)
	: name_(name), description_(desc), key_(key), enabled_(enabled) {}
	virtual ~KeyCommand() {}

	const wxString& GetName () const { return name_; }
	const wxString& GetDescription () const { return description_; }
	const KeyboardCode& GetKey () const { return key_; }

	bool IsEnabled () const { return enabled_; }
	void SetEnabled (bool value) { enabled_= value; }

	// Command to be defined by derived classes
	virtual void Command()= 0;

private:
	friend class HotkeyManager;
	void SetKey (KeyboardCode key) { key_= key; }

	wxString name_;
	wxString description_;
	KeyboardCode key_;
	bool enabled_;
};

/**
 * Keeps the hotkeys registry and triggers actions
 */
class HotkeyManager : public CConfigBase {
public:
	HotkeyManager();
	~HotkeyManager();
	
	bool SetKeyCommand (unsigned int index, KeyboardCode kc);

	const std::vector<KeyCommand*>& GetKeyCommands() const { return m_keyCommands; }

	// This method is called from secondary thread (linux only) to poll the state
	// of the keyboard and trigger the command associated with a hotkey
	void CheckKeyboardStatus();

	// Configuration methods
	virtual void InitDefaults() override;

	virtual void ReadProfileData(wxConfigBase* pConfObj) override;

	virtual void WriteProfileData(wxConfigBase* pConfObj) override;

private:
	int IsKeyUsed (KeyboardCode kc) const;

	KeyboardCode m_lastKeyCode;
	std::vector<KeyCommand*> m_keyCommands;
};

} // namespace

#endif
