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

#include <wx/window.h>

#include "eviacamdefs.h"
#include "eviacamapp.h"
#include "viacamcontroller.h"
#include "pointeraction.h"
#include "simplelog.h"


namespace eviacam {

//
// Define the available key commands
//

class HotKeyCommandEnable : public HotKey {
public:
	HotKeyCommandEnable(int id)
	: HotKey(id, _T("hotKeyEnable"), _("Enable eViacam"), KeyboardCode::FromWXK(WXK_F11)) {}

	void Command() override {
		wxGetApp().GetController().SetEnabled(!wxGetApp().GetController().GetEnabled(), true);
	}
};

class HotKeyCommandCenterPointer : public HotKey {
public:
	HotKeyCommandCenterPointer(int id)
	: HotKey(id, _T("hotKeyCenterPointer"), _("Center the pointer"), KeyboardCode::FromWXK(WXK_F10)) {}

	void Command() override {
		wxGetApp().GetController().GetPointerAction().CenterPointer();
	}
};

class HotKeyCommandWorkspace : public HotKey {
public:
	HotKeyCommandWorkspace(int id)
	: HotKey(id, _T("hotKeyWorkspace"), _("Enable workspace limit"), KeyboardCode::FromWXK(WXK_F9)) {}

	void Command() override {
		wxGetApp().GetController().GetPointerAction().SetRestrictedWorkingArea(
			!wxGetApp().GetController().GetPointerAction().GetRestrictedWorkingArea());
	}
};

class HotKeyCommandIncreaseX : public HotKey {
public:
	HotKeyCommandIncreaseX(int id)
	: HotKey(id, ("hotKeyIncreaseX"), _("Increase the X axis speed"), KeyboardCode::FromWXK(WXK_RIGHT)) {}

	void Command() override {
		wxGetApp().GetController().GetPointerAction().SetXSpeed(wxGetApp().GetController().GetPointerAction().GetXSpeed()+1);
	}
};

class HotKeyCommandIncreaseY : public HotKey {
public:
	HotKeyCommandIncreaseY(int id)
	: HotKey(id, _T("hotKeyIncreaseY"), _("Increase the Y axis speed"), KeyboardCode::FromWXK(WXK_UP)) {}

	void Command() override {
		wxGetApp().GetController().GetPointerAction().SetYSpeed(
			wxGetApp().GetController().GetPointerAction().GetYSpeed()+1);
	}
};

class HotKeyCommandDecreaseX : public HotKey {
public:
	HotKeyCommandDecreaseX(int id)
	: HotKey(id, _T("hotKeyDecreaseX"), _("Decrease the X axis speed"), KeyboardCode::FromWXK(WXK_LEFT)) {}

	void Command() override {
		wxGetApp().GetController().GetPointerAction().SetXSpeed(
			wxGetApp().GetController().GetPointerAction().GetXSpeed()-1);
	}
};

class HotKeyCommandDecreaseY : public HotKey {
public:
	HotKeyCommandDecreaseY(int id)
	: HotKey(id, _T("hotKeyDecreaseY"), _("Decrease the Y axis speed"), KeyboardCode::FromWXK(WXK_DOWN)) {}

	void Command() override {
		wxGetApp().GetController().GetPointerAction().SetYSpeed(
			wxGetApp().GetController().GetPointerAction().GetYSpeed()-1);
	}
};

static const KeyboardCode g_banned_hotkeys[]= {
#if defined(__WXMSW__)
	KeyboardCode(VK_SHIFT),
	KeyboardCode(VK_CONTROL),
	KeyboardCode(VK_MENU),
	KeyboardCode(VK_LWIN),
	KeyboardCode(VK_RWIN),
	KeyboardCode(VK_APPS),
	KeyboardCode(VK_LSHIFT),
	KeyboardCode(VK_RSHIFT),
	KeyboardCode(VK_LCONTROL),
	KeyboardCode(VK_RCONTROL),
	KeyboardCode(VK_LMENU),
	// For Windows the F12 key is reserved for use by the debugger at all
	// times, so it should not be registered as a hot key. See:
	// https://msdn.microsoft.com/en-us//library/windows/desktop/ms646309(v=vs.85).aspx
	KeyboardCode(VK_F12)
#endif
};


HotkeyManager::HotkeyManager() {
	// Create the hotkeys
	m_HotKeys.push_back(new HotKeyCommandEnable(m_HotKeys.size()));
	m_HotKeys.push_back(new HotKeyCommandCenterPointer(m_HotKeys.size()));
	m_HotKeys.push_back(new HotKeyCommandWorkspace(m_HotKeys.size()));
	m_HotKeys.push_back(new HotKeyCommandIncreaseX(m_HotKeys.size()));
	m_HotKeys.push_back(new HotKeyCommandIncreaseY(m_HotKeys.size()));
	m_HotKeys.push_back(new HotKeyCommandDecreaseX(m_HotKeys.size()));
	m_HotKeys.push_back(new HotKeyCommandDecreaseY(m_HotKeys.size()));

	InitDefaults();

	// Bind hotkey handler
	#if defined(__WXMSW__)
		wxWindow* mainWin = wxGetApp().GetController().GetMainWindow();
		mainWin->Bind(wxEVT_HOTKEY,	[this](wxKeyEvent& e) {	this->HotkeyEventHandler(e); });
	#endif
}

HotkeyManager::~HotkeyManager() {
	// free memory
	for (size_t i= 0; i< m_HotKeys.size(); i++) {
		delete m_HotKeys[i];
	}

#if defined(__WXMSW__)
	// TODO: unbind handler. See:
	// http://docs.wxwidgets.org/trunk/classwx_evt_handler.html#a2b7df8272075a96daea78cdd799c00da
#endif
}

void HotkeyManager::ResetHotKeys() {
	// Disable hot keys
	for (size_t i= 0; i< m_HotKeys.size(); i++) {
		DisableHotKey(*m_HotKeys[i]);
	}
}

bool HotkeyManager::SetHotKeyKeyboardCode (HotKey& hk, KeyboardCode kc) {
	if (hk.GetKey()== kc) return true; 		// Do nothing if is the same
	//if (FindByKeyboardCode(kc) != -1) return false; 	// Already in use

	// Is banned?
	for (size_t i= 0; i< sizeof(g_banned_hotkeys)/sizeof(KeyboardCode); ++i) {
		if (g_banned_hotkeys[i] == kc) return false;
	}

	bool wasEnabled= hk.IsEnabled();
	if (wasEnabled) DisableHotKey(hk);
	hk.SetKey(kc);
	bool result= true;
	if (wasEnabled) result= EnableHotKey(hk);
	return result;
}

int HotkeyManager::FindByKeyboardCode (KeyboardCode kc) const {
	for (size_t i=0; i< m_HotKeys.size(); i++) {
		if (m_HotKeys[i]->GetKey() == kc) return i;
	}
	return -1; // not found
}

bool HotkeyManager::EnableHotKey(HotKey& hk) {
	bool result= true;
	if (!hk.IsEnabled()) {
		#if defined(__WXMSW__)
			wxWindow* mainWin = wxGetApp().GetController().GetMainWindow();
			result = mainWin->RegisterHotKey(hk.id_, 0, hk.GetKey().get_native());
		#endif

		if (result) hk.SetEnabled(true);
	}
	return result;
}

void HotkeyManager::DisableHotKey(HotKey& hk) {
	if (hk.IsEnabled()) {
		#if defined(__WXMSW__)
			wxWindow* mainWin = wxGetApp().GetController().GetMainWindow();
			mainWin->UnregisterHotKey(hk.id_);
		#endif

		hk.SetEnabled(false);
	}
}

void HotkeyManager::HotkeyEventHandler(wxKeyEvent& event) {
	KeyboardCode kc(event.GetRawKeyCode());

	SLOG_DEBUG("HotKey Handler called, KeyboardCode: %d (%s)", kc.get_native(),
		static_cast<const char*>(kc.GetName().mb_str()));

	int i= FindByKeyboardCode(kc);
	if (i== -1) return;
	if (m_HotKeys[i]->IsEnabled()) m_HotKeys[i]->Command();
}


// TODO: to avoid synchronization problems, implement this by deriving
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
		int index= FindByKeyboardCode(kc);
		if (index != -1 and m_HotKeys[index]->IsEnabled())
			m_HotKeys[index]->Command();
	}
	END_GUI_CALL_MUTEX()
#endif // __WXGTK___
}

void HotkeyManager::InitDefaults() {
	// Hotkeys disabled by default
	for (size_t i= 0; i< m_HotKeys.size(); i++) {
		DisableHotKey(*m_HotKeys[i]);
	}
}

void HotkeyManager::WriteProfileData(wxConfigBase* pConfObj) {
	pConfObj->SetPath (_T("hotKeyManager"));

	for (unsigned int i=0; i<m_HotKeys.size(); i++) {
		HotKey* hk= m_HotKeys[i];
		pConfObj->Write(hk->GetName(), hk->IsEnabled());
		pConfObj->Write(hk->GetName() + _T("Key"),
				static_cast<int>(hk->GetKey().get_native()));
	}

	pConfObj->SetPath (_T(".."));
} 

void HotkeyManager::ReadProfileData(wxConfigBase* pConfObj) {
	pConfObj->SetPath (_T("hotKeyManager"));

	for (unsigned int i=0; i<m_HotKeys.size(); i++) {
		HotKey* hk= m_HotKeys[i];

		bool isEnabled= hk->IsEnabled();
		pConfObj->Read(hk->GetName(), &isEnabled);

		int rawKeyCode= hk->GetKey().get_native();
		pConfObj->Read(m_HotKeys[i]->GetName() + _T("Key"), &rawKeyCode);

		DisableHotKey(*hk);
		SetHotKeyKeyboardCode (*hk, KeyboardCode(rawKeyCode));
		if (isEnabled) EnableHotKey(*hk);
	}

	pConfObj->SetPath (_T(".."));
}


} // namespace
