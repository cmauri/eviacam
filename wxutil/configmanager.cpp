/////////////////////////////////////////////////////////////////////////////
// Name:        configmanager.cpp
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

#include "configmanager.h"

#include <wx/intl.h>
#include <wx/arrstr.h>

#define DEFAULT_PROFILE_KEY _T("currentProfile")
#define DEFAULT_PROFILE_GROUP_KEY _T("default")
#define DEFAULT_PROFILE_NAME _("default")
#define LANGUAGE_KEY _T("language")
#define BASE_PATH _T("/settings")

CConfigManager::CConfigManager(CConfigBase* configRoot)
{
	assert (configRoot);

	m_configRoot= configRoot;
	m_currentProfile= DEFAULT_PROFILE_GROUP_KEY;
	wxConfigBase::Get()->SetPath (BASE_PATH);
	assert (wxConfigBase::Get()->GetPath().Cmp(BASE_PATH)== 0);
}

int CConfigManager::ReadLanguage()
{
	wxString language;
	const wxLanguageInfo *info= NULL;

	assert (wxConfigBase::Get()->GetPath().Cmp(BASE_PATH)== 0);

	// Load language name
	wxConfigBase::Get()->Read(LANGUAGE_KEY, &language);
	if (language.Length()> 0)
	{
		// Check if valid
		info= wxLocale::FindLanguageInfo (language);
		if (info)
		{
			// Ok
			return info->Language;
		}		
	}
	
	return wxLANGUAGE_DEFAULT;
}

void CConfigManager::WriteLanguage (const int id)
{
	assert (wxConfigBase::Get()->GetPath().Cmp(BASE_PATH)== 0);

	// Assumes a valid language id
	if (id== wxLANGUAGE_DEFAULT)
	{
		wxConfigBase::Get()->DeleteEntry (LANGUAGE_KEY);
	}
	else
	{
		const wxLanguageInfo *info= wxLocale::GetLanguageInfo(id);
		assert (info);
		wxConfigBase::Get()->Write (LANGUAGE_KEY, info->Description);
	}
}

void CConfigManager::ReadAll (bool callStartupRun)
{
	wxConfigBase* pConfObj= wxConfigBase::Get();
	
	pConfObj->Read(DEFAULT_PROFILE_KEY, &m_currentProfile);	

	// If current profile doesn't exists set to default
	if (!pConfObj->HasGroup(m_currentProfile))
		m_currentProfile= DEFAULT_PROFILE_GROUP_KEY;
	
	// App wide data
	m_configRoot->ReadAppData (pConfObj);

	// Profile related data
	if (m_currentProfile.Length()== 0)
		pConfObj->SetPath (DEFAULT_PROFILE_GROUP_KEY);
	else
		pConfObj->SetPath (m_currentProfile);
	
	m_configRoot->ReadProfileData (pConfObj);

	pConfObj->SetPath (_T(".."));

	if (callStartupRun) m_configRoot->StartupRun();

	assert (wxConfigBase::Get()->GetPath().Cmp(BASE_PATH)== 0);
}

void CConfigManager::WriteAll()
{
	assert (wxConfigBase::Get()->GetPath().Cmp(BASE_PATH)== 0);

	wxConfigBase* pConfObj= wxConfigBase::Get();
	
	// App wide data
	pConfObj->Write(DEFAULT_PROFILE_KEY, m_currentProfile);
	m_configRoot->WriteAppData (pConfObj);

	// Profile related data
	if (m_currentProfile.Length()== 0)
		pConfObj->SetPath (DEFAULT_PROFILE_GROUP_KEY);
	else
		pConfObj->SetPath (m_currentProfile);
	
	m_configRoot->WriteProfileData (pConfObj);

	pConfObj->SetPath (_T(".."));
	
	pConfObj->Flush();

	assert (wxConfigBase::Get()->GetPath().Cmp(BASE_PATH)== 0);
}

int CConfigManager::GetCurrentProfile() const
{
	if (m_currentProfile== DEFAULT_PROFILE_GROUP_KEY) return 0;
	wxArrayString pList= GetProfileList ();
	int index= pList.Index (m_currentProfile);

	assert (index> 0 && index!= wxNOT_FOUND);
	return index;
}

void CConfigManager::ChangeCurrentProfile(int index)
{
	assert (wxConfigBase::Get()->GetPath().Cmp(BASE_PATH)== 0);
	if (index!= GetCurrentProfile())
	{
		if (index== 0) m_currentProfile= DEFAULT_PROFILE_GROUP_KEY;
		else
		{	
			wxArrayString pList= GetProfileList ();

			assert (index>0 && index< (int) pList.GetCount());

			m_currentProfile= pList[index];
		}

		// Save new profile before reading again
		wxConfigBase* pConfObj= wxConfigBase::Get();	
		pConfObj->Write(DEFAULT_PROFILE_KEY, m_currentProfile);
		pConfObj->Flush();

		ReadAll ();		
	}
	assert (wxConfigBase::Get()->GetPath().Cmp(BASE_PATH)== 0);
}

// Return 0 is ok, 1 if profile exists or 2 if profile name is invalid
int CConfigManager::AddNewProfile(const wxString& name)
{
	assert (wxConfigBase::Get()->GetPath().Cmp(BASE_PATH)== 0);
	if (name== DEFAULT_PROFILE_GROUP_KEY) return 2;
	if (name== DEFAULT_PROFILE_KEY) return 2;
	if (name== DEFAULT_PROFILE_NAME) return 2;
	if (name.Contains(_T("/"))) return 2;
	if (name.Contains(_T("\\"))) return 2;
	
	// Check existence
	wxConfigBase* pConfObj= wxConfigBase::Get();
	if (pConfObj->HasGroup(name)) return 1;
	
	// Add the new profile and change to
	m_currentProfile= name;

	WriteAll();

	assert (wxConfigBase::Get()->GetPath().Cmp(BASE_PATH)== 0);
	return 0;
}

void CConfigManager::DeleteCurrentProfile()
{
	assert (wxConfigBase::Get()->GetPath().Cmp(BASE_PATH)== 0);
	// Default profile deletion not allowed
	if (m_currentProfile!= DEFAULT_PROFILE_GROUP_KEY)
	{
		wxString oldProfile= m_currentProfile;

		// Switch to default profile
		ChangeCurrentProfile(0);

		// Delete old profile
		wxConfigBase* pConfObj= wxConfigBase::Get();
		pConfObj->DeleteGroup(oldProfile);
		pConfObj->Flush();
	}	
	assert (wxConfigBase::Get()->GetPath().Cmp(BASE_PATH)== 0);
}

wxArrayString CConfigManager::GetProfileList () const
{
	wxArrayString pList;
	wxConfigBase* pConfObj= wxConfigBase::Get();
	wxString str;
	long dummy;
	assert (wxConfigBase::Get()->GetPath().Cmp(BASE_PATH)== 0);

	// Add default profile name (is always id=0)
	pList.Add (DEFAULT_PROFILE_NAME);

	bool bCont = pConfObj->GetFirstGroup(str, dummy);
	while (bCont) {
		// FIXME: this condition ensures that:
		//  1) the default entry is not already added to the list
		//  2) another entry in the parent directory called "wxWidgets"
		//     is not added to the list. This only happens with wxConfigFile
		//     and it looks like a wxConfigFile bug
		if (str!= DEFAULT_PROFILE_GROUP_KEY && str!= _T("wxWidgets")) 
			pList.Add (str);	
		bCont = pConfObj->GetNextGroup(str, dummy);
	}
	assert (wxConfigBase::Get()->GetPath().Cmp(BASE_PATH)== 0);
  	return pList;
}
