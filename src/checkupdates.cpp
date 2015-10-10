/////////////////////////////////////////////////////////////////////////////
// Name:        checkupdates.cpp
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

#include "checkupdates.h"
#include "config.h"

#include <wx/intl.h>
#include <wx/protocol/http.h>
#include <wx/thread.h>

#define UPDATE_HOSTNAME "eviacam.crea-si.com"
#ifdef NDEBUG		
	#define MODE_DEBUG _T("&m=r")
#else
	#define MODE_DEBUG _T("&m=d")
#endif

#if defined(__WXGTK__)
	#define UPDATE_FILE _T("/version.php?cv=") _T(VERSION) _T("&p=linux") MODE_DEBUG
#else
	#define UPDATE_FILE _T("/version.php?cv=") _T(VERSION) _T("&p=windows") MODE_DEBUG
#endif



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
	httpStream = request->GetInputStream(UPDATE_FILE);
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

DECLARE_EVENT_TYPE(THREAD_FINISHED_EVENT, wxCommandEvent);

DEFINE_EVENT_TYPE(THREAD_FINISHED_EVENT);
DEFINE_EVENT_TYPE(CHECKUPDATE_FINISHED_EVENT);

BEGIN_EVENT_TABLE(eviacam::CheckUpdates, wxEvtHandler)
	EVT_COMMAND(wxID_ANY, THREAD_FINISHED_EVENT, eviacam::CheckUpdates::OnThreadFinished)
END_EVENT_TABLE()

namespace eviacam {

void CheckUpdates::OnThreadFinished(wxCommandEvent& event)
{
	// This event handler translates the event sent from the worker thread,
	// which stores version name using a SetClientData, into a new wxCommandEvent
	// which uses SetString. We need to do so in order to maintain compatibility with
	// wx2.x. There are chances to leak some memory if some THREAD_FINISHED_EVENT are
	// discarded and thus not properly deallocated. 
	// TODO: when wx 3.0 takes over 2.x remove this stuff and send events directly 
	// from the thread using wxQueueEvent. See here for more info:
	// // http://docs.wxwidgets.org/trunk/group__group__funcmacro__events.html#ga0cf60a1ad3a5f1e659f7ae591570f58d

	assert(wxIsMainThread());

	wxCommandEvent eventNew(CHECKUPDATE_FINISHED_EVENT);

	// String
	wxString* msg = static_cast<wxString*>(event.GetClientData());
	eventNew.SetString(wxString(msg->mb_str(wxConvUTF8), wxConvUTF8)); // Take a deep copy of the string
	delete msg;
	
	// Status code
	eventNew.SetInt(event.GetInt());
	
	// Finish processing old event
	event.Skip(true); 

	// Fire new event. Use this instead of wxQueueEvent for wx2.8 compatibility
	wxPostEvent(this, eventNew); 
}

CheckUpdates::CheckUpdates()
: m_threadRunning(false)
{
}

CheckUpdates::~CheckUpdates()
{
	// Wait for the thread to finish
	while (m_threadRunning)
		wxThread::This()->Sleep(1);
}

void CheckUpdates::Start()
{
	m_threadRunning = true;
	new CheckUpdatesWorker(*this);
}

CheckUpdates::CheckUpdatesWorker::CheckUpdatesWorker(CheckUpdates& handler)
: wxThread(wxTHREAD_DETACHED)
, m_handler(&handler)
{
	wxThreadError retval= this->Create();
	if (retval== wxTHREAD_NO_ERROR) retval= this->Run();

	if (retval!= wxTHREAD_NO_ERROR) {
		m_handler->m_threadRunning = false;

		wxCommandEvent event(THREAD_FINISHED_EVENT);
		
		// Store information string using SetClientData instead of SetString
		wxString* msg = new wxString(_("Error checking for updates. Try again later."));
		event.SetClientData(msg);

		event.SetInt(CheckUpdates::ERROR_CHECKING_NEW_VERSION);
		
		// Use this instead of wxQueueEvent for wx2.8 compatibility
		wxPostEvent(m_handler, event);  
	}
}

CheckUpdates::CheckUpdatesWorker::~CheckUpdatesWorker()
{
	m_handler->m_threadRunning = false;
}

wxThread::ExitCode CheckUpdates::CheckUpdatesWorker::Entry()
{
	std::string new_version;
	int result= check_updates(&new_version);
	wxString* msg = NULL;
	wxCommandEvent event(THREAD_FINISHED_EVENT);
	
	switch (result) {
		case 1: // newer version available
			msg = new wxString(new_version.c_str(), wxConvUTF8);
			event.SetInt(CheckUpdates::NEW_VERSION_AVAILABLE);
			break;
		case 0: // no newer version available
		case -1: // local version newer than remote!
			msg = new wxString(new_version.c_str(), wxConvUTF8);
			event.SetInt(CheckUpdates::NO_NEW_VERSION_AVAILABLE);
			break;
		case -2: // cannot resolve host name
			msg = new wxString(_("Cannot resolve host name: "));
			msg->Append(_T(UPDATE_HOSTNAME));
			event.SetInt(CheckUpdates::ERROR_CHECKING_NEW_VERSION);
			break;
		case -3: // cannot connect to host
			msg = new wxString(_("Conection failed: "));
			msg->Append(_T(UPDATE_HOSTNAME));
			event.SetInt(CheckUpdates::ERROR_CHECKING_NEW_VERSION);
			break;
		case -4: // file does not exist (on server)
			msg = new wxString(_("Sorry. Version file not found. Please report us."));
			event.SetInt(CheckUpdates::ERROR_CHECKING_NEW_VERSION);
			break;
		case -5: // generic error
			msg = new wxString(_("Sorry. Something bad happened."));
			event.SetInt(CheckUpdates::ERROR_CHECKING_NEW_VERSION);
			break;
		default:
			assert (false);
	}

	// Store information string using SetClientData instead of SetString
	event.SetClientData(msg);

	// Use this instead of wxQueueEvent for wx2.8 compatibility
	wxPostEvent(m_handler, event);
	
	return ExitCode(0);
}

}
