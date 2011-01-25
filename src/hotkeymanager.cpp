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
	
	m_keyCommandEnable = new CKeyCommandEnable();
	m_keyCommandWorkspace = new CKeyCommandWorkspace();
	m_keyCommandCenterMouse = new CKeyCommandCenterMouse();
	m_keyCommandIncreaseXAxisSpeed = new CKeyCommandIncreaseXAxisSpeed();
	m_keyCommandIncreaseYAxisSpeed = new CKeyCommandIncreaseYAxisSpeed();
	m_keyCommandDecreaseXAxisSpeed = new CKeyCommandDecreaseXAxisSpeed();
	m_keyCommandDecreaseYAxisSpeed = new CKeyCommandDecreaseYAxisSpeed();
	
	m_keyCommands.push_back((CKeyCommand*) m_keyCommandEnable);
	m_keyCommands.push_back((CKeyCommand*) m_keyCommandWorkspace);
	m_keyCommands.push_back((CKeyCommand*) m_keyCommandCenterMouse);
	m_keyCommands.push_back((CKeyCommand*) m_keyCommandIncreaseXAxisSpeed);
	m_keyCommands.push_back((CKeyCommand*) m_keyCommandIncreaseYAxisSpeed);
	m_keyCommands.push_back((CKeyCommand*) m_keyCommandDecreaseXAxisSpeed);
	m_keyCommands.push_back((CKeyCommand*) m_keyCommandDecreaseYAxisSpeed);

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
	pConfObj->Write(_T("enable"), m_keyCommandEnable->IsEnabled());
	pConfObj->Write(_T("enableKey"), static_cast<long>(m_keyCommandEnable->GetKey().GetRawValue()));
	
	pConfObj->Write(_T("workspace"), m_keyCommandWorkspace->IsEnabled());
	pConfObj->Write(_T("workspaceKey"), static_cast<long>(m_keyCommandWorkspace->GetKey().GetRawValue()));
	
	pConfObj->Write(_T("centerMouse"), m_keyCommandCenterMouse->IsEnabled());
	pConfObj->Write(_T("centerMouseKey"), static_cast<long>(m_keyCommandCenterMouse->GetKey().GetRawValue()));
	
	pConfObj->Write(_T("increaseX"), m_keyCommandIncreaseXAxisSpeed->IsEnabled());
	pConfObj->Write(_T("increaseXKey"), static_cast<long>(m_keyCommandIncreaseXAxisSpeed->GetKey().GetRawValue()));
	
	pConfObj->Write(_T("increaseY"), m_keyCommandIncreaseYAxisSpeed->IsEnabled());
	pConfObj->Write(_T("increaseYKey"), static_cast<long>(m_keyCommandIncreaseYAxisSpeed->GetKey().GetRawValue()));
	
	pConfObj->Write(_T("decreaseX"), m_keyCommandDecreaseXAxisSpeed->IsEnabled());
	pConfObj->Write(_T("decreaseXKey"), static_cast<long>(m_keyCommandDecreaseXAxisSpeed->GetKey().GetRawValue()));
	
	pConfObj->Write(_T("decreaseY"), m_keyCommandDecreaseYAxisSpeed->IsEnabled());
	pConfObj->Write(_T("decreaseYKey"), static_cast<long>(m_keyCommandDecreaseYAxisSpeed->GetKey().GetRawValue()));
#endif // __WXGTK___
	pConfObj->SetPath (_T(".."));
} 

void CHotkeyManager::ReadProfileData(wxConfigBase* pConfObj)
{
	pConfObj->SetPath (_T("hotKeyManager"));
#if defined(__WXGTK__) 
	bool isEnabled;
	long rawKeyCode;
	
	if(pConfObj->Read(_T("enable"), &isEnabled))
		m_keyCommandEnable->SetEnabled(isEnabled);
	if (pConfObj->Read(_T("enableKey"), &rawKeyCode))
		m_keyCommandEnable->SetKey(CKeyboardCode::FromRawValue(static_cast<unsigned long>(rawKeyCode)));
	
	if(pConfObj->Read(_T("workspace"), &isEnabled))
		  m_keyCommandWorkspace->SetEnabled(isEnabled);
	if (pConfObj->Read(_T("workspaceKey"), &rawKeyCode))
		m_keyCommandWorkspace->SetKey(CKeyboardCode::FromRawValue(static_cast<unsigned long>(rawKeyCode)));
	
	if(pConfObj->Read(_T("centerMouse"), &isEnabled))
		  m_keyCommandCenterMouse->SetEnabled(isEnabled);
	if (pConfObj->Read(_T("centerMouseKey"), &rawKeyCode))
		m_keyCommandCenterMouse->SetKey(CKeyboardCode::FromRawValue(static_cast<unsigned long>(rawKeyCode)));
	
	if(pConfObj->Read(_T("increaseX"), &isEnabled))
		  m_keyCommandIncreaseXAxisSpeed->SetEnabled(isEnabled);
	if (pConfObj->Read(_T("increaseXKey"), &rawKeyCode))
		m_keyCommandIncreaseXAxisSpeed->SetKey(CKeyboardCode::FromRawValue(static_cast<unsigned long>(rawKeyCode)));

	if(pConfObj->Read(_T("increaseY"), &isEnabled))
		  m_keyCommandIncreaseYAxisSpeed->SetEnabled(isEnabled);
	if (pConfObj->Read(_T("increaseYKey"), &rawKeyCode))
		m_keyCommandIncreaseYAxisSpeed->SetKey(CKeyboardCode::FromRawValue(static_cast<unsigned long>(rawKeyCode)));
	
	if(pConfObj->Read(_T("decreaseX"), &isEnabled))
		  m_keyCommandDecreaseXAxisSpeed->SetEnabled(isEnabled);
	if (pConfObj->Read(_T("decreaseXKey"), &rawKeyCode))
		m_keyCommandDecreaseXAxisSpeed->SetKey(CKeyboardCode::FromRawValue(static_cast<unsigned long>(rawKeyCode)));
	
	if(pConfObj->Read(_T("decreaseY"), &isEnabled))
		  m_keyCommandDecreaseYAxisSpeed->SetEnabled(isEnabled);
	if (pConfObj->Read(_T("decreaseYKey"), &rawKeyCode))
		m_keyCommandDecreaseYAxisSpeed->SetKey(CKeyboardCode::FromRawValue(static_cast<unsigned long>(rawKeyCode)));

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
	SetKey(CKeyboardCode::FromWXKeyCode (WXK_RIGHT));
	SetEnabled(true);
}

CKeyCommandIncreaseYAxisSpeed::CKeyCommandIncreaseYAxisSpeed()
{
	SetDescription(_("Increase the Y axis speed"));
	SetKey(CKeyboardCode::FromWXKeyCode (WXK_UP));
	SetEnabled(true);
}

CKeyCommandDecreaseXAxisSpeed::CKeyCommandDecreaseXAxisSpeed()
{
	SetDescription(_("Decrease the X axis speed"));
	SetKey(CKeyboardCode::FromWXKeyCode (WXK_LEFT));
	SetEnabled(true);
}

CKeyCommandDecreaseYAxisSpeed::CKeyCommandDecreaseYAxisSpeed()
{
	SetDescription(_("Decrease the Y axis speed"));
	SetKey(CKeyboardCode::FromWXKeyCode (WXK_DOWN));
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
	wxGetApp().GetController().GetPointerAction().CenterPointer();
}

void CKeyCommandIncreaseXAxisSpeed::Command()
{
	wxGetApp().GetController().GetPointerAction().SetXSpeed(wxGetApp().GetController().GetPointerAction().GetXSpeed()+1);
}

void CKeyCommandIncreaseYAxisSpeed::Command()
{
	wxGetApp().GetController().GetPointerAction().SetYSpeed(wxGetApp().GetController().GetPointerAction().GetYSpeed()+1);
}

void CKeyCommandDecreaseXAxisSpeed::Command()
{
	wxGetApp().GetController().GetPointerAction().SetXSpeed(wxGetApp().GetController().GetPointerAction().GetXSpeed()-1);
}

void CKeyCommandDecreaseYAxisSpeed::Command()
{
	wxGetApp().GetController().GetPointerAction().SetYSpeed(wxGetApp().GetController().GetPointerAction().GetYSpeed()-1);
}


