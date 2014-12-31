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
#include "checkupdates_listener.h"
#include "checkupdates_ui.h"
#include <wx/window.h>

namespace eviacam {

CheckUpdatesListener::CheckUpdatesListener(wxWindow* parent)
: m_parent(parent)
{
	m_checker.Connect(
		CHECKUPDATE_FINISHED_EVENT, 
		wxCommandEventHandler(CheckUpdatesListener::OnCheckUpdatesFinished), 
		NULL, this);
	m_checker.Start();
}

CheckUpdatesListener::~CheckUpdatesListener()
{
	m_checker.Disconnect(
		CHECKUPDATE_FINISHED_EVENT,
		wxCommandEventHandler(CheckUpdatesListener::OnCheckUpdatesFinished),
		NULL, this);
}

void CheckUpdatesListener::OnCheckUpdatesFinished(wxCommandEvent& event)
{
	CheckUpdates::ResultStatus status = static_cast<CheckUpdates::ResultStatus>(event.GetInt());

	if (status == CheckUpdates::NEW_VERSION_AVAILABLE) {
		CheckUpdatesUI dlg(m_parent);
		dlg.ShowModal();
	}
	event.Skip(false); // Finish processing the event
}

}
