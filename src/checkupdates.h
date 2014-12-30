/////////////////////////////////////////////////////////////////////////////
// Name:        checkupdates.h
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

#ifndef CHECKUPDATES_H
#define CHECKUPDATES_H

#include <wx/string.h>
#include <wx/thread.h>
#include <wx/event.h>

// This event provides two fields
// GetInt: see CheckUpdates::ResultStatus
// GetString: wxString with an appropiate message for the user
DECLARE_EVENT_TYPE(CHECKUPDATE_FINISHED_EVENT, wxCommandEvent);

namespace eviacam {

// Check if a newer version of the software is available by downloading
// the /version file on server. 
class CheckUpdates : public wxEvtHandler
{
	DECLARE_EVENT_TABLE()

public:
	CheckUpdates();
	virtual ~CheckUpdates();

	enum ResultStatus { 
		CHECK_IN_PROGRESS= 0,	// While not finished
		NEW_VERSION_AVAILABLE,
		NO_NEW_VERSION_AVAILABLE,
		ERROR_CHECKING_NEW_VERSION
	};
	
	ResultStatus GetStatus() const { return m_resultStatus; }
	wxString GetStatusMessage() const { return m_statusMessage; }

private:
	void OnThreadFinished(wxCommandEvent& event);
	
	// Contains new version name or error message
	ResultStatus m_resultStatus;
	wxString m_statusMessage;

	// Create thread to avoid blocking the GUI while checking
	class CheckUpdatesWorker : public wxThread {
	public:
		CheckUpdatesWorker(wxEvtHandler& msgDest);
		virtual wxThread::ExitCode Entry();
	private:
		wxEvtHandler* m_msgDest;
	};

	
};

}

#endif // 
