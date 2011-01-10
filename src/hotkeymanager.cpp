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
#endif // __WXGTK___
}

void CHotkeyManager::CheckKeyboardStatus()
{
#if defined(__WXGTK__) 
	// Read keyboard
	if (m_enabledActivationKey) {
		BEGIN_GUI_CALL_MUTEX()
		CKeyboardCode kc = CKeyboardCode::ReadKeyCode();
		if (kc== m_keyCode and kc!= m_lastKeyCode) {
			wxGetApp().GetController().SetEnabled(!wxGetApp().GetController().GetEnabled(), true);
		}
		m_lastKeyCode = kc;
		END_GUI_CALL_MUTEX()
	}
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
