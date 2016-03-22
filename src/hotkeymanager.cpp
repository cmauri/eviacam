/////////////////////////////////////////////////////////////////////////////
// Name:        hotkeymanager.cpp
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
#include "hotkeymanager.h"

#include "eviacamdefs.h"
#include "eviacamapp.h"
#include "viacamcontroller.h"
#include "pointeraction.h"

namespace eviacam {

//
// Define the available key commands
//

class KeyCommandEnable : public KeyCommand {
public:
	KeyCommandEnable()
	: KeyCommand(_T("hotKeyEnable"), _("Enable eViacam"), KeyboardCode::FromWXKeyCode(WXK_SCROLL), true) {}

	void Command() override {
		wxGetApp().GetController().SetEnabled(!wxGetApp().GetController().GetEnabled(), true);
	}
};

class KeyCommandWorkspace : public KeyCommand {
public:
	KeyCommandWorkspace()
	: KeyCommand(_T("hotKeyWorkspace"), _("Enable workspace limit"), KeyboardCode::FromWXKeyCode(WXK_F12), false) {}

	void Command() override {
		wxGetApp().GetController().GetPointerAction().SetRestrictedWorkingArea(
			!wxGetApp().GetController().GetPointerAction().GetRestrictedWorkingArea());
	}
};

class KeyCommandCenterPointer : public KeyCommand {
public:
	KeyCommandCenterPointer()
	: KeyCommand(_T("hotKeyCenterPointer"), _("Center the pointer"), KeyboardCode::FromWXKeyCode(WXK_HOME), true) {}

	void Command() override {
		wxGetApp().GetController().GetPointerAction().CenterPointer();
	}
};

class KeyCommandIncreaseX : public KeyCommand {
public:
	KeyCommandIncreaseX()
	: KeyCommand(("hotKeyIncreaseX"), _("Increase the X axis speed"), KeyboardCode::FromWXKeyCode(WXK_RIGHT), false) {}

	void Command() override {
		wxGetApp().GetController().GetPointerAction().SetXSpeed(wxGetApp().GetController().GetPointerAction().GetXSpeed()+1);
	}
};

class KeyCommandIncreaseY : public KeyCommand {
public:
	KeyCommandIncreaseY()
	: KeyCommand(_T("hotKeyIncreaseY"), _("Increase the Y axis speed"), KeyboardCode::FromWXKeyCode(WXK_UP), false) {}

	void Command() override {
		wxGetApp().GetController().GetPointerAction().SetYSpeed(
			wxGetApp().GetController().GetPointerAction().GetYSpeed()+1);
	}
};

class KeyCommandDecreaseX : public KeyCommand {
public:
	KeyCommandDecreaseX()
	: KeyCommand(_T("hotKeyDecreaseX"), _("Decrease the X axis speed"), KeyboardCode::FromWXKeyCode(WXK_LEFT), false) {}

	void Command() override {
		wxGetApp().GetController().GetPointerAction().SetXSpeed(
			wxGetApp().GetController().GetPointerAction().GetXSpeed()-1);
	}
};

class KeyCommandDecreaseY : public KeyCommand {
public:
	KeyCommandDecreaseY()
	: KeyCommand(_T("hotKeyDecreaseY"), _("Decrease the Y axis speed"), KeyboardCode::FromWXKeyCode(WXK_DOWN), false) {}

	void Command() override {
		wxGetApp().GetController().GetPointerAction().SetYSpeed(
			wxGetApp().GetController().GetPointerAction().GetYSpeed()-1);
	}
};


HotkeyManager::HotkeyManager() {
	InitDefaults();
}

HotkeyManager::~HotkeyManager() {
	for (int i= 0; i< m_keyCommands.size(); i++) {
		delete m_keyCommands[i];
	}
}

void HotkeyManager::InitDefaults() {
#if defined(__WXGTK__) 
	m_keyCommands.push_back(new KeyCommandEnable());
	m_keyCommands.push_back(new KeyCommandWorkspace());
	m_keyCommands.push_back(new KeyCommandCenterPointer());
	m_keyCommands.push_back(new KeyCommandIncreaseX());
	m_keyCommands.push_back(new KeyCommandIncreaseY());
	m_keyCommands.push_back(new KeyCommandDecreaseX());
	m_keyCommands.push_back(new KeyCommandDecreaseY());
#endif // __WXGTK___
}

// TODO: to avoid problems with synchronization, implement this by deriving
// from wxEvtHandler and sending hot-key messages using wxPostEvent. This
// way we make sure that all hot-key driven actions are always executed from
// the main thread. This approach will also make easier the transition to
// an implementation based on keyboard hooks
void HotkeyManager::CheckKeyboardStatus() {
#if defined(__WXGTK__) 
	BEGIN_GUI_CALL_MUTEX()
	KeyboardCode kc = KeyboardCode::ReadKeyCode();
	if (kc!= m_lastKeyCode) {
		m_lastKeyCode= kc;
		int index= IsKeyUsed(kc);
		if (index != -1 and m_keyCommands[index]->IsEnabled())
			m_keyCommands[index]->Command();
	}
	END_GUI_CALL_MUTEX()
#endif // __WXGTK___
}

void HotkeyManager::WriteProfileData(wxConfigBase* pConfObj) {
	pConfObj->SetPath (_T("hotKeyManager"));
#if defined(__WXGTK__) 
	for (unsigned int i=0; i<m_keyCommands.size(); i++) {
		pConfObj->Write(m_keyCommands[i]->GetName(), m_keyCommands[i]->IsEnabled());
		pConfObj->Write(m_keyCommands[i]->GetName() + _T("Key"), static_cast<long>(m_keyCommands[i]->GetKey().GetRawValue()));
	}
#endif // __WXGTK___
	pConfObj->SetPath (_T(".."));
} 

void HotkeyManager::ReadProfileData(wxConfigBase* pConfObj) {
	pConfObj->SetPath (_T("hotKeyManager"));
#if defined(__WXGTK__) 
	bool isEnabled;
	long rawKeyCode;
	
	for (unsigned int i=0; i<m_keyCommands.size(); i++) {
		if(pConfObj->Read(m_keyCommands[i]->GetName(), &isEnabled))
			m_keyCommands[i]->SetEnabled(isEnabled);
		
		if (pConfObj->Read(m_keyCommands[i]->GetName() + _T("Key"), &rawKeyCode))
			m_keyCommands[i]->SetKey(KeyboardCode::FromRawValue(static_cast<unsigned long>(rawKeyCode)));
	}
#endif // __WXGTK___
	pConfObj->SetPath (_T(".."));
}

int HotkeyManager::IsKeyUsed (KeyboardCode kc) const {
	for (unsigned int i=0; i<m_keyCommands.size(); i++) {
		if (m_keyCommands[i]->GetKey() == kc)
			return i;
	}
	return -1;
}

bool HotkeyManager::SetKeyCommand (unsigned int index, KeyboardCode kc) {
	assert (index < m_keyCommands.size());
	if (IsKeyUsed(kc) == -1) {
		m_keyCommands[index]->SetKey(kc);
		return true;
	} else {
		return false;
	}
}

} // namespace
