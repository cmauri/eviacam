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

// TODO: to avoid problems with synchronization, implement this by deriving 
// from wxEvtHandler and sending hot-key messages using wxPostEvent. This
// way we make sure that all hot-key driven actions are always executed from
// the main thread. This approach will also make easier the transition to 
// an implementation based on keyboard hooks

CHotkeyManager::CHotkeyManager()
: m_enabledActivationKey(false)
, m_keyCode()
, m_lastKeyCode()
{
	InitDefaults();
}

void CHotkeyManager::InitDefaults()
{
#if defined(__WXGTK__) 
	SetActivationKeyCode(CKeyboardCode::FromWXKeyCode (WXK_SCROLL));
	SetEnabledActivationKey (true);
	
	m_keyCommands.push_back((CKeyCommand*) new CKeyCommandEnable);
	m_keyCommands.push_back((CKeyCommand*) new CKeyCommandWorkspace);
	m_keyCommands.push_back((CKeyCommand*) new CKeyCommandCenterMouse);
	m_keyCommands.push_back((CKeyCommand*) new CKeyCommandIncreaseXAxisSpeed);

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
	
	
	
	/*// Read keyboard
	if (m_enabledActivationKey) {
		BEGIN_GUI_CALL_MUTEX()
		CKeyboardCode kc = CKeyboardCode::ReadKeyCode();
		if (kc== m_keyCode and kc!= m_lastKeyCode) {
			wxGetApp().GetController().SetEnabled(!wxGetApp().GetController().GetEnabled(), true);
		}
		m_lastKeyCode = kc;
		END_GUI_CALL_MUTEX()
	}*/
#endif // __WXGTK___
}


void CHotkeyManager::WriteProfileData(wxConfigBase* pConfObj)
{
	pConfObj->SetPath (_T("hotKeyManager"));
#if defined(__WXGTK__) 
	pConfObj->Write(_T("enabledActivationKey"), m_enabledActivationKey);
	pConfObj->Write(_T("keyCode"), static_cast<long>(GetActivationKey().GetRawValue()));
#endif // __WXGTK___
	pConfObj->SetPath (_T(".."));
} 

void CHotkeyManager::ReadProfileData(wxConfigBase* pConfObj)
{
	pConfObj->SetPath (_T("hotKeyManager"));
#if defined(__WXGTK__) 
	pConfObj->Read(_T("enabledActivationKey"), &m_enabledActivationKey);
	long rawKeyCode;
	if (pConfObj->Read(_T("keyCode"), &rawKeyCode))
		SetActivationKeyCode(CKeyboardCode::FromRawValue(static_cast<unsigned long>(rawKeyCode)));
#endif // __WXGTK___
	pConfObj->SetPath (_T(".."));
}


int CHotkeyManager::IsKeyUsed (CKeyboardCode kc)
{
	for (int i=0; i<m_keyCommands.size(); i++)
	{
		if (m_keyCommands[i]->GetKey() == kc)
			return i;
	}
	return -1;
}

void CHotkeyManager::SetKeyCommand (int index, CKeyboardCode kc)
{
	assert (index < m_keyCommands.size());
	m_keyCommands[index]->SetKey(kc);
	m_keyCommands[index]->SetEnabled(true);
}


CKeyCommandEnable::CKeyCommandEnable()
{
	SetDescription(_("Enable eViacam"));
	SetKey(CKeyboardCode::FromASCII('e'));
	SetEnabled(true);
}

CKeyCommandWorkspace::CKeyCommandWorkspace()
{
	SetDescription(_("Enable workspace limit"));
	SetKey(CKeyboardCode::FromASCII('w'));
	SetEnabled(true);
}

CKeyCommandCenterMouse::CKeyCommandCenterMouse()
{
	SetDescription(_("Center the pointer on the screen"));
	SetKey(CKeyboardCode::FromASCII('c'));
	SetEnabled(true);
}

CKeyCommandIncreaseXAxisSpeed::CKeyCommandIncreaseXAxisSpeed()
{
	SetDescription(_("Increase the X axis speed"));
	SetKey(CKeyboardCode::FromASCII('u'));
	SetEnabled(true);
}




void CKeyCommandEnable::Command()
{
	wxGetApp().GetController().SetEnabled(!wxGetApp().GetController().GetEnabled(), true);
}

void CKeyCommandWorkspace::Command()
{
	wxGetApp().GetController().GetPointerAction().SetRestrictedWorkingArea(!wxGetApp().GetController().GetPointerAction().GetRestrictedWorkingArea());
}

void CKeyCommandCenterMouse::Command()
{
	wxGetApp().GetController().GetPointerAction().GetMouseControl().CenterPointer();
}

void CKeyCommandIncreaseXAxisSpeed::Command()
{
	wxGetApp().GetController().GetPointerAction().SetXSpeed(wxGetApp().GetController().GetPointerAction().GetXSpeed()+1);
}

