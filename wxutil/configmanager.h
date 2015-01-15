/////////////////////////////////////////////////////////////////////////////
// Name:        configmanager.h
// Purpose:  
// Author:      Cesar Mauri Loba (cesar at crea-si dot com)
// Modified by: 
// Created:     
// Copyright:   (C) 2009 Cesar Mauri Loba - CREA Software Systems
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

#ifndef CONFIGMANAGER_H
#define CONFIGMANAGER_H

#include "configbase.h"

class CConfigManager
{
public:
	CConfigManager(CConfigBase* configRoot);

	// Read/write both app wide and specific profile config
	void ReadAll ();
	void WriteAll ();

	// For language setting only
	int ReadLanguage();
	void WriteLanguage (const int id);

	int GetCurrentProfile() const;
	void ChangeCurrentProfile(int index);
	void DeleteCurrentProfile();
	// Add and change to new profile. 
	// Return 0 is ok, 1 if profile exists or 2 if profile name is invalid
	int AddNewProfile(const wxString& name);
	wxArrayString GetProfileList () const;

private:
	wxString m_currentProfile;
	CConfigBase* m_configRoot;
};

#endif
