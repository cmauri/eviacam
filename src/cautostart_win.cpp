/////////////////////////////////////////////////////////////////////////////
// Name:        cautostart_win.cpp
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
#include "cautostart_win.h"
#include <wx/string.h>
#include "wx/stdpaths.h"
#include <wx/msw/registry.h>

constexpr wxStringCharType appName[] = wxT("eviacam");
constexpr wxStringCharType keyName[] = wxT("SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run");

CAutostart::CAutostart(wxString fileName)
{
	wxUnusedVar(fileName);
}

CAutostart::~CAutostart()
{
}

bool CAutostart::IsEnabled()
{
	wxRegKey key(wxRegKey::HKCU, "SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run");
	return key.HasValue(wxT("eviacam"));
}

void CAutostart::Enable(bool value)
{
	wxRegKey key(wxRegKey::HKCU, "SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run");
	if (value) {
		key.SetValue("eviacam", wxStandardPaths::Get().GetExecutablePath());
	}
	else {
		key.DeleteValue("eviacam");
	}
}
