/////////////////////////////////////////////////////////////////////////////
// Name:        wwizardmanager.cpp
// Purpose: helper class to open wizard only once
// Author:      Cesar Mauri Loba
// Modified by: 
// Created:     Mon 10 Nov 2010 09:57:22 CEST
// RCS-ID:      
// Copyright:   (C) 2010 Cesar Mauri from CREA Sistemes Informatics
// Licence:
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

#include "wwizardmanager.h"
#include "wwizard.h"

WWizardManager::WWizardManager ()
: m_wizard(NULL)
{	
}

void WWizardManager::Open(wxWindow* parent)
{
	if (m_wizard) m_wizard->Raise();
	else {
		m_wizard = new WWizard(parent);
		m_wizard->Connect (ID_WWIZARD, wxEVT_WIZARD_FINISHED, wxWizardEventHandler (WWizardManager::OnWwizardClosed), NULL, this);
		m_wizard->Connect (ID_WWIZARD, wxEVT_WIZARD_CANCEL, wxWizardEventHandler (WWizardManager::OnWwizardClosed), NULL, this);
		m_wizard->Run();
	}
}

void WWizardManager::OnWwizardClosed( wxWizardEvent& event )
{
	m_wizard= NULL;
	event.Skip(true);
}
