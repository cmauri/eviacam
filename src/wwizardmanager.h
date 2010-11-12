/////////////////////////////////////////////////////////////////////////////
// Name:        wwizardmanager.h
// Purpose: helper class to open wizard only once
// Author:      Cesar Mauri Loba
// Modified by: 
// Created:     Mon 10 Nov 2010 09:57:22 CEST
// RCS-ID:      
// Copyright:   (C) 2010 Cesar Mauri from CREA Sistemes Informatics
// License:
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
#ifndef _WWIZARDMANAGER_H_
#define _WWIZARDMANAGER_H_
#include <wx/event.h>

class WWizard;
class wxWizardEvent;
class CViacamController;

// Class to manage wizard open
class WWizardManager : public wxEvtHandler
{
public:
	WWizardManager (CViacamController& controller);
	virtual ~WWizardManager() {};
	void Open(wxWindow* parent= NULL);
private:
	void OnWwizardClosed( wxWizardEvent& event );
	
	WWizard* m_wizard;
	CViacamController* m_controller;
};

#endif
    // _WWIZARDMANAGER_H_
