/////////////////////////////////////////////////////////////////////////////
// Name:        checkupdates.h
// Purpose:     
// Author:      Cesar Mauri Loba (cesar at crea-si dot com)
// Copyright:   (C) 2012 Cesar Mauri Loba - CREA Software Systems
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

namespace eviacam {

// Check if a newer version of the software is available by downloading
// the /version file on server. 

// Once created an instance a new thread is spawned, caller should poll
// GetStatus() until returns something different than CHECK_IN_PROGRESS
// Then the status message could be retrieved reliably.
class CheckUpdates
{
public:
	enum ResultStatus { 
		CHECK_IN_PROGRESS= 0,	// While not finished
		NEW_VERSION_AVAILABLE,
		NO_NEW_VERSION_AVAILABLE,
		ERROR_CHECKING_NEW_VERSION
	};

	// Create new instance. Pointer should NOT be freed.
	// When finished using the object, call Release to free it.
	static CheckUpdates* GetInstance();
	void Release();
	
	ResultStatus GetStatus() const { return m_resultStatus; }
	wxString GetStatusMessage() const { return m_statusMessage; }

private:
	CheckUpdates();	// Prevent direct instance creation
	~CheckUpdates();
	
	// Contains new version name or error message
	ResultStatus m_resultStatus;
	int m_refCount;
	wxString m_statusMessage;
	wxMutex m_mutex;

	// Create thread to avoid blocking the GUI while checking
	class CheckUpdatesWorker : public wxThread {
	public:
		CheckUpdatesWorker (CheckUpdates& parent);
		virtual wxThread::ExitCode Entry();
	private:
		CheckUpdates* m_parent;
	};
};

}
#endif // 