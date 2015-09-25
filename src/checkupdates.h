/////////////////////////////////////////////////////////////////////////////
// Name:        checkupdates.h
// Purpose:     Checks in background if a new eviacam version is available 
//              and sends an event to report status
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

#ifndef CHECKUPDATES_H
#define CHECKUPDATES_H

#include <wx/event.h>

// This event provides two fields
//		GetInt: see CheckUpdates::ResultStatus
//		GetString: wxString with an appropriate message for the user
DECLARE_EVENT_TYPE(CHECKUPDATE_FINISHED_EVENT, wxCommandEvent);

namespace eviacam {

class CheckUpdates : public wxEvtHandler
{
public:
	CheckUpdates();
	virtual ~CheckUpdates();
	void Start();

	enum ResultStatus { 
		NEW_VERSION_AVAILABLE,
		NO_NEW_VERSION_AVAILABLE,
		ERROR_CHECKING_NEW_VERSION
	};

private:
	void OnThreadFinished(wxCommandEvent& event);

	// Create thread to avoid blocking the GUI while checking
	class CheckUpdatesWorker : public wxThread {
	public:
		CheckUpdatesWorker(CheckUpdates& handler);
		virtual ~CheckUpdatesWorker();
		virtual wxThread::ExitCode Entry();
	private:
		CheckUpdates* m_handler;
	};

	friend class CheckUpdatesWorker;

	volatile bool m_threadRunning;

	DECLARE_EVENT_TABLE()
};

} // namespace eviacam

#endif // CHECKUPDATES_H
