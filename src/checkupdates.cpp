/////////////////////////////////////////////////////////////////////////////
// Name:        checkupdates.cpp
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

#include "checkupdates.h"
#include "version.h"

#include <wx/intl.h>
#include <wx/protocol/http.h>
#include <wx/thread.h>

#define UPDATE_HOSTNAME "eviacam.sourceforge.net"
#define UPDATE_FILE "/version"

namespace eviacam {

// Compare two version strings
// Return:
// -1: s1< s2
// 0: s1== s2
// 1: s1> s2
static
int versionstrcmp(const char* s1, const char* s2)
{
	int compo_s1[4], compo_s2[4];

	for (int i= 0; i< 4; i++) compo_s1[i]= compo_s2[i]= 0;

	sscanf (s1, "%d.%d.%d.%d", &compo_s1[0], &compo_s1[1], &compo_s1[2], &compo_s1[3]);
	sscanf (s2, "%d.%d.%d.%d", &compo_s2[0], &compo_s2[1], &compo_s2[2], &compo_s2[3]);

	for (int i= 0; i< 4; i++) {
		if (compo_s1[i]< compo_s2[i]) return -1;
		if (compo_s1[i]> compo_s2[i]) return 1;
	}

	return 0;
}

#define VERSION_STRING_MAX_SIZE	20

// Check if a new version of the software is available by 
// checking the contents of  
// http://UPDATE_HOSTNAME/UPDATE_FILE
//
// Return:
//  1: newer version available 
//  0: no newer version available
//	-1: local version newer than remote!
//	-2: cannot resolve host name
//	-3: cannot connect to host
//  -4: file does not exist (on server)
//  -5: generic error

static 
int check_updates(std::string* new_version)
{
	// Create request on the heap as recommended in the documentation
	wxHTTP* request= new wxHTTP();
	if (!request) return -5;

	wxInputStream *httpStream = NULL;
    
	int retval= 0;

	request->SetHeader(_T("Content-type"), _T("text/html; charset=utf-8"));
	request->SetTimeout(10); // 10 seconds of timeout

	// Start connection request (actually only resolve hostname)
	if (!request->Connect(_T(UPDATE_HOSTNAME))) {
		retval= -2;
		goto check_update_error;
	}

	// Set file to request & make connection
	httpStream = request->GetInputStream(_T(UPDATE_FILE));
	if (!httpStream) {
		// Due to a buggy wxHTTP implementation, many errors are reported
		// this way. User only sees "connection error".
		retval= -3;
		goto check_update_error;
	}

	if (request->GetError()!= wxPROTO_NOERR) {
		switch (request->GetError()) {
			case wxPROTO_CONNERR: retval= -3; break;
			case wxPROTO_NOFILE: retval= -4; break;
			default: retval= -5;
		}
		goto check_update_error;
	}

	// Read file contents up to VERSION_STRING_MAX_SIZE bytes
	char buffer[VERSION_STRING_MAX_SIZE + 1];
	{
		unsigned int read_count= 0;
		bool is_eof= false;

		while (read_count< VERSION_STRING_MAX_SIZE && !is_eof) {
			httpStream->Read(&buffer[read_count], VERSION_STRING_MAX_SIZE - read_count);
			read_count+= static_cast<unsigned int>(httpStream->LastRead());
			is_eof= httpStream->Eof();
		}
		assert (read_count<= VERSION_STRING_MAX_SIZE);
		buffer[read_count]= 0;	// String terminator
	}

	retval= versionstrcmp(buffer, VERSION);

	if (new_version) (*new_version)= buffer;
	 
check_update_error:
	if (httpStream) wxDELETE(httpStream);
	if (request) request->Destroy();
	return retval;	
}

CheckUpdates* CheckUpdates::GetInstance()
{
	return new CheckUpdates();
}

void CheckUpdates::Release()
{
	m_mutex.Lock();
	m_refCount--;
	int cur_refCount= m_refCount;
	m_mutex.Unlock();

	assert (cur_refCount>= 0);
	if (cur_refCount== 0) delete this;
}

CheckUpdates::CheckUpdates() 
: m_resultStatus(CheckUpdates::CHECK_IN_PROGRESS)
, m_refCount(2)	// One for the caller and other for the internal thread
{
	new CheckUpdates::CheckUpdatesWorker(*this);
}

CheckUpdates::~CheckUpdates()
{
	assert (m_refCount== 0);
}

CheckUpdates::CheckUpdatesWorker::CheckUpdatesWorker (CheckUpdates& parent) 
: wxThread(wxTHREAD_DETACHED)
, m_parent(&parent)
{
	wxThreadError retval= this->Create();
	if (retval== wxTHREAD_NO_ERROR) retval= this->Run();

	if (retval!= wxTHREAD_NO_ERROR) {
		m_parent->m_statusMessage= _("Error checking for updates. Try again later.");
		m_parent->m_resultStatus= CheckUpdates::ERROR_CHECKING_NEW_VERSION;
		m_parent->Release();
	}
}

wxThread::ExitCode CheckUpdates::CheckUpdatesWorker::Entry()
{
	std::string new_version;
	int result= check_updates(&new_version);

	m_parent->m_statusMessage= wxString(new_version.c_str(), wxConvUTF8);
	
	switch (result) {
		case 1: // newer version available
			m_parent->m_resultStatus= CheckUpdates::NEW_VERSION_AVAILABLE;
			break;
		case 0: // no newer version available
			m_parent->m_resultStatus= CheckUpdates::NO_NEW_VERSION_AVAILABLE;
			break;
		case -1: // local version newer than remote!
			m_parent->m_statusMessage= _T("Your version is newer than the one on the server!");
			m_parent->m_resultStatus= CheckUpdates::ERROR_CHECKING_NEW_VERSION;
			break;
		case -2: // cannot resolve host name
			m_parent->m_statusMessage= _("Cannot resolve host name: ");
			m_parent->m_statusMessage+= _T(UPDATE_HOSTNAME);
			m_parent->m_resultStatus= CheckUpdates::ERROR_CHECKING_NEW_VERSION;
			break;
		case -3: // cannot connect to host
			m_parent->m_statusMessage= _("Conection failed: ");
			m_parent->m_statusMessage+= _T(UPDATE_HOSTNAME);
			m_parent->m_resultStatus= CheckUpdates::ERROR_CHECKING_NEW_VERSION;
			break;
		case -4: // file does not exist (on server)
			m_parent->m_statusMessage= _("Sorry. Version file not found. Please report us.");
			m_parent->m_resultStatus= CheckUpdates::ERROR_CHECKING_NEW_VERSION;
			break;
		case -5: // generic error
			m_parent->m_statusMessage= _("Sorry. Something bad happened.");
			m_parent->m_resultStatus= CheckUpdates::ERROR_CHECKING_NEW_VERSION;
			break;
		default:
			assert (false);
	}

	m_parent->Release();

	return ExitCode(0);
}


};