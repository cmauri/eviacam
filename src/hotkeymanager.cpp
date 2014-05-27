/////////////////////////////////////////////////////////////////////////////
// Name:        hotkeymanager.cpp
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
#include "hotkeymanager.h"
#include "eviacamdefs.h"
#include "eviacamapp.h"
#include "viacamcontroller.h"
#include "pointeraction.h"

class CKeyCommandEnable : public CKeyCommand {
public:
	CKeyCommandEnable()
	{
		SetName(_T("hotKeyEnable"));
		SetDescription(_("Enable eViacam"));
		SetKey(CKeyboardCode::FromWXKeyCode(WXK_SCROLL));
		SetEnabled(true);
	}

	void Command()
	{
		wxGetApp().GetController().SetEnabled(!wxGetApp().GetController().GetEnabled(), true);
	}
};

class CKeyCommandWorkspace : public CKeyCommand {
public:
	CKeyCommandWorkspace()
	{
		SetName(_T("hotKeyWorkspace"));
		SetDescription(_("Enable workspace limit"));
		SetKey(CKeyboardCode::FromWXKeyCode(WXK_F12));
		SetEnabled(false);
	}

	void Command()
	{
		wxGetApp().GetController().GetPointerAction().SetRestrictedWorkingArea(!wxGetApp().GetController().GetPointerAction().GetRestrictedWorkingArea());
	}
};

class CKeyCommandCenterPointer : public CKeyCommand {
public:
	CKeyCommandCenterPointer()
	{
		SetName(_T("hotKeyCenterPointer"));
		SetDescription(_("Center the pointer"));
		SetKey(CKeyboardCode::FromWXKeyCode(WXK_HOME));
		SetEnabled(true);
	}

	void Command()
	{
		wxGetApp().GetController().GetPointerAction().CenterPointer();
	}
};

class CKeyCommandIncreaseX : public CKeyCommand {
public:
	CKeyCommandIncreaseX()
	{
		SetName(_T("hotKeyIncreaseX"));
		SetDescription(_("Increase the X axis speed"));
		SetKey(CKeyboardCode::FromWXKeyCode (WXK_RIGHT));
		SetEnabled(false);
	}

	void Command()
	{
		wxGetApp().GetController().GetPointerAction().SetXSpeed(wxGetApp().GetController().GetPointerAction().GetXSpeed()+1);
	}
};

class CKeyCommandIncreaseY : public CKeyCommand {
public:
	CKeyCommandIncreaseY()
	{
		SetName(_T("hotKeyIncreaseY"));
		SetDescription(_("Increase the Y axis speed"));
		SetKey(CKeyboardCode::FromWXKeyCode (WXK_UP));
		SetEnabled(false);
	}

	void Command()
	{
		wxGetApp().GetController().GetPointerAction().SetYSpeed(wxGetApp().GetController().GetPointerAction().GetYSpeed()+1);
	}
};

class CKeyCommandDecreaseX : public CKeyCommand {
public:
	CKeyCommandDecreaseX()
	{
		SetName(_T("hotKeyDecreaseX"));
		SetDescription(_("Decrease the X axis speed"));
		SetKey(CKeyboardCode::FromWXKeyCode (WXK_LEFT));
		SetEnabled(false);	
	}

	void Command()
	{
		wxGetApp().GetController().GetPointerAction().SetXSpeed(wxGetApp().GetController().GetPointerAction().GetXSpeed()-1);
	}
};

class CKeyCommandDecreaseY : public CKeyCommand {
public:
	CKeyCommandDecreaseY()
	{
		SetName(_T("hotKeyDecreaseY"));
		SetDescription(_("Decrease the Y axis speed"));
		SetKey(CKeyboardCode::FromWXKeyCode (WXK_DOWN));
		SetEnabled(false);
	}

	void Command()
	{
		wxGetApp().GetController().GetPointerAction().SetYSpeed(wxGetApp().GetController().GetPointerAction().GetYSpeed()-1);
	}
};


// TODO: to avoid problems with synchronization, implement this by deriving 
// from wxEvtHandler and sending hot-key messages using wxPostEvent. This
// way we make sure that all hot-key driven actions are always executed from
// the main thread. This approach will also make easier the transition to 
// an implementation based on keyboard hooks

CHotkeyManager::CHotkeyManager() : m_lastKeyCode()
{
	InitDefaults();
}

CHotkeyManager::~CHotkeyManager()
{
	while (!m_keyCommands.empty()) m_keyCommands.pop_back();
}

void CHotkeyManager::InitDefaults()
{
#if defined(__WXGTK__) 
	m_keyCommands.push_back(new CKeyCommandEnable());
	m_keyCommands.push_back(new CKeyCommandWorkspace());
	m_keyCommands.push_back(new CKeyCommandCenterPointer());
	m_keyCommands.push_back(new CKeyCommandIncreaseX());
	m_keyCommands.push_back(new CKeyCommandIncreaseY());
	m_keyCommands.push_back(new CKeyCommandDecreaseX());
	m_keyCommands.push_back(new CKeyCommandDecreaseY());
#endif // __WXGTK___
}

void CHotkeyManager::CheckKeyboardStatus()
{
#if defined(__WXGTK__) 
	BEGIN_GUI_CALL_MUTEX()
	CKeyboardCode kc = CKeyboardCode::ReadKeyCode();
	if (kc!= m_lastKeyCode) {
		m_lastKeyCode= kc;
		int index= IsKeyUsed(kc);
		if (index != -1 and m_keyCommands[index]->IsEnabled())
			m_keyCommands[index]->Command();
	}
	END_GUI_CALL_MUTEX()
#endif // __WXGTK___
}


void CHotkeyManager::WriteProfileData(wxConfigBase* pConfObj)
{
	pConfObj->SetPath (_T("hotKeyManager"));
#if defined(__WXGTK__) 
	for (unsigned int i=0; i<m_keyCommands.size(); i++) {
		pConfObj->Write(m_keyCommands[i]->GetName(), m_keyCommands[i]->IsEnabled());
		pConfObj->Write(m_keyCommands[i]->GetName() + _T("Key"), static_cast<long>(m_keyCommands[i]->GetKey().GetRawValue()));
	}
#endif // __WXGTK___
	pConfObj->SetPath (_T(".."));
} 

void CHotkeyManager::ReadProfileData(wxConfigBase* pConfObj)
{
	pConfObj->SetPath (_T("hotKeyManager"));
#if defined(__WXGTK__) 
	bool isEnabled;
	long rawKeyCode;
	
	for (unsigned int i=0; i<m_keyCommands.size(); i++) {
		if(pConfObj->Read(m_keyCommands[i]->GetName(), &isEnabled))
			m_keyCommands[i]->SetEnabled(isEnabled);
		
		if (pConfObj->Read(m_keyCommands[i]->GetName() + _T("Key"), &rawKeyCode))
			m_keyCommands[i]->SetKey(CKeyboardCode::FromRawValue(static_cast<unsigned long>(rawKeyCode)));
	}
#endif // __WXGTK___
	pConfObj->SetPath (_T(".."));
}

int CHotkeyManager::IsKeyUsed (CKeyboardCode kc)
{
	for (unsigned int i=0; i<m_keyCommands.size(); i++)
	{
		if (m_keyCommands[i]->GetKey() == kc)
			return i;
	}
	return -1;
}

bool CHotkeyManager::SetKeyCommand (unsigned int index, CKeyboardCode kc)
{
	assert (index < m_keyCommands.size());
	if (IsKeyUsed(kc) == -1) {
		m_keyCommands[index]->SetKey(kc);
		return true;
	} else {
		return false;
	}
}
