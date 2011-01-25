/////////////////////////////////////////////////////////////////////////////
// Name:        hotkeymanager.h
// Purpose:  
// Author:      Cesar Mauri Loba (cesar at crea-si dot com)
// Modified by: 
// Created:     30/12/2010
// Copyright:   (C) 2010-11 Cesar Mauri Loba - CREA Software Systems
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

#include "ckeyboardcode.h"
#include "configbase.h"

#include <vector>

using namespace std;

class CKeyCommand {
public:
	wxString GetDescription () const { return description; }
	void SetDescription (wxString s) { description= s; }
	CKeyboardCode GetKey () const { return key; }
	void SetKey (CKeyboardCode kc) { key= kc; }
	bool IsEnabled () const { return enabled; }
	void SetEnabled (bool value) { enabled= value; }
	virtual void Command() =0;

private:
	wxString description;
	CKeyboardCode key;
	bool enabled;
};

class CKeyCommandEnable : public CKeyCommand {
public:
	CKeyCommandEnable();
	void Command();
};

class CKeyCommandWorkspace : public CKeyCommand {
	public:
		CKeyCommandWorkspace();
		void Command();
};

class CKeyCommandCenterMouse : public CKeyCommand {
	public:
		CKeyCommandCenterMouse();
		void Command();
};

class CKeyCommandIncreaseXAxisSpeed : public CKeyCommand {
	public:
		CKeyCommandIncreaseXAxisSpeed();
		void Command();
};




class CHotkeyManager : public CConfigBase {
public:
	CHotkeyManager();
	
	// TODO: change the interface of this class to allow to iterate multiple
	// hot key assignements. Each hot key should carry the follwing information
	// struct CHotKey {
	//	CKeyboardCode key : key, should be always valid
	//	bool enabled :
	//  wxString description : 
	// }

	const bool GetEnabledActivationKey () const {
		return m_enabledActivationKey;
	}
	void SetEnabledActivationKey (bool value) {
		m_enabledActivationKey= value;
	}

	void SetActivationKeyCode (CKeyboardCode value) {
		m_keyCode= value;
		m_lastKeyCode= value;
	}

	const CKeyboardCode GetActivationKey () const {
		return m_keyCode;
	}
	
	const int GetNumKeyCommands () const {
		return m_keyCommands.size();
	}
	
	CKeyCommand* GetKeyCommand (int index) {
		assert (index < m_keyCommands.size());
		return m_keyCommands[index];
	}
	
	int IsKeyUsed (CKeyboardCode kc);
	
	void SetKeyCommand (int index, CKeyboardCode kc);
	
	

	// This method must be called periodically to check
	// the keyboard status and perform actions accordingly
	// TODO: switch to a hook based mechanisn instead of pulling
	void CheckKeyboardStatus();

	// Configuration methods
	virtual void InitDefaults();

//	virtual void ReadAppData(wxConfigBase* pConfObj);
	virtual void ReadProfileData(wxConfigBase* pConfObj);

//	virtual void WriteAppData(wxConfigBase* pConfObj);
	virtual void WriteProfileData(wxConfigBase* pConfObj);  

private:
	bool m_enabledActivationKey;
	CKeyboardCode m_keyCode;
	CKeyboardCode m_lastKeyCode;
	vector<CKeyCommand*> m_keyCommands;
};

#endif
