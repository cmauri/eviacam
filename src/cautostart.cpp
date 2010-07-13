/////////////////////////////////////////////////////////////////////////////
// Name:        keyboardbitmapcheck.h
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
	wxString path;

	if (!wxGetEnv(wxT("XDG_CONFIG_HOME"), &path)) {
		path = wxStandardPaths::Get().GetUserConfigDir() + wxT("/.config/autostart/");
	}

	return wxFileExists(path + m_fileName);
}


void CAutostart::Enable(bool value)
{
	wxString path = wxStandardPaths::Get().GetDataDir();
	char cString[1024];
	strncpy(cString, (const char*)path.mb_str(wxConvUTF8), 1023);
	printf("DIR: %s\n",cString);

	wxString pathIn;
	wxString pathOut;

	pathIn = wxT("/usr/share/applications/") + m_fileName;
	if (!wxGetEnv(wxT("XDG_CONFIG_HOME"), &pathOut)) {
		pathOut = wxStandardPaths::Get().GetUserConfigDir() + wxT("/.config/autostart/");
	}

	if (value) {
		wxMkdir(pathOut, 0777);
		wxCopyFile(pathIn, pathOut + m_fileName, false);
	} else {
		wxRemoveFile(pathOut + m_fileName);
	}
}

