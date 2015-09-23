/////////////////////////////////////////////////////////////////////////////
// Name:        checkupdates_listener.cpp
// Purpose:     
// Author:      Cesar Mauri Loba (cesar at crea-si dot com)
// Copyright:   (C) 2012-14 Cesar Mauri Loba - CREA Software Systems
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
#include "checkupdates_manager.h"
#include "checkupdates_ui.h"
#include "config.h"
#include <wx/window.h>

namespace eviacam {

CheckUpdatesManager::CheckUpdatesManager(wxWindow* parent)
: m_parent(parent)
, m_pCheckUpdatesUI(NULL)
{
	m_checker.Connect(
		CHECKUPDATE_FINISHED_EVENT, 
		wxCommandEventHandler(CheckUpdatesManager::OnCheckUpdatesFinished), 
		NULL, this);
}

CheckUpdatesManager::~CheckUpdatesManager()
{
	m_checker.Disconnect(
		CHECKUPDATE_FINISHED_EVENT,
		wxCommandEventHandler(CheckUpdatesManager::OnCheckUpdatesFinished),
		NULL, this);
}

static 
void SetResults(CheckUpdatesUI* dlg, CheckUpdates::ResultStatus status, wxString str)
{
	wxString txt1;
	wxString txt2;

	switch (status) {
	case CheckUpdates::NEW_VERSION_AVAILABLE:
		txt1 = _("New version available: ");
		txt1 += str;
		txt2 = _("Installed version: ");
		txt2 += _T(VERSION);
		dlg->SetResults(txt1, txt2, true);
		break;
	case CheckUpdates::NO_NEW_VERSION_AVAILABLE:
		txt1 = _("No updates available");
		dlg->SetResults(txt1, txt2, false);
		break;
	case CheckUpdates::ERROR_CHECKING_NEW_VERSION:
		txt1 = _("Error checking for updates");
		dlg->SetResults(txt1, str, false);
		break;
	default:
		assert(false);
	}
}

void CheckUpdatesManager::OnCheckUpdatesFinished(wxCommandEvent& event)
{
	event.Skip(false); // Finish processing the event

	CheckUpdates::ResultStatus status = static_cast<CheckUpdates::ResultStatus>(event.GetInt());

	if (m_pCheckUpdatesUI) {
		// Checking in foreground
		m_pCheckUpdatesUI->StopProgress();
		SetResults(m_pCheckUpdatesUI, status, event.GetString());
		return;
	}

	// Checking in background
	if (status != CheckUpdates::NEW_VERSION_AVAILABLE) return;

#if defined(ENABLE_UPDATES_CHECK)		
	m_pCheckUpdatesUI = new CheckUpdatesUI(m_parent);

	SetResults(m_pCheckUpdatesUI, status, event.GetString());
	
	m_pCheckUpdatesUI->ShowModal();
	
	wxDELETE(m_pCheckUpdatesUI);
#endif
}

void CheckUpdatesManager::LaunchBackground()
{
	assert(m_pCheckUpdatesUI == NULL);
	m_checker.Start();
}

void CheckUpdatesManager::LaunchForeground()
{
	assert(m_pCheckUpdatesUI == NULL);
	m_pCheckUpdatesUI = new CheckUpdatesUI(m_parent);
	m_checker.Start();
	m_pCheckUpdatesUI->StartProgress();
	m_pCheckUpdatesUI->ShowModal();
}

}
