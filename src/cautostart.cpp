/////////////////////////////////////////////////////////////////////////////
// Name:        cautostart.cpp
// Purpose:  
// Author:      Cesar Mauri Loba (cesar at crea-si dot com)
// Modified by: 
// Created:     07/07/2010
// Copyright:   (C) 2008 Cesar Mauri Loba - CREA Software Systems
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
#include "cautostart.h"
#include <wx/string.h>
#include <wx/stdpaths.h>
#include <wx/utils.h>

CAutostart::CAutostart(wxString fileName)
{
	m_fileName = fileName;
}

CAutostart::~CAutostart()
{
}

bool CAutostart::IsEnabled()
{
#if defined(__WXGTK__) 
	wxString path;

	if (!wxGetEnv(wxT("XDG_CONFIG_HOME"), &path)) {
		path = wxStandardPaths::Get().GetUserConfigDir() + wxT("/.config/autostart/");
	}

	return wxFileExists(path + m_fileName);
#else
	assert (false);	// Win: not yet implemented
	return false;
#endif
}


void CAutostart::Enable(bool value)
{
#if defined(__WXGTK__) 
	wxString pathIn;
	wxString pathOut;

	pathIn = wxStandardPaths::Get().GetDataDir()  + wxT("/") + m_fileName;
	if (!wxGetEnv(wxT("XDG_CONFIG_HOME"), &pathOut)) {
		pathOut = wxStandardPaths::Get().GetUserConfigDir() + wxT("/.config");
		if (!wxDirExists(pathOut))
			wxMkdir(pathOut, 0777);
		pathOut += wxT("/autostart/");
	}
	
	if (value) {
		if (!wxDirExists(pathOut))
			wxMkdir(pathOut, 0777);
		wxCopyFile(pathIn, pathOut + m_fileName, false);
	} else {
		wxRemoveFile(pathOut + m_fileName);
	}
#else
	assert (false);		// Not yet implemented
#endif
}

