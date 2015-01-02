/////////////////////////////////////////////////////////////////////////////
// Name:        checkupdates_listener.h
// Purpose:     Listener for chek for updates
// Author:      Cesar Mauri Loba (cesar at crea-si dot com)
// Copyright:   (C) 2014 Cesar Mauri Loba - CREA Software Systems
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

#ifndef CHECKUPDATES_LISTENER_H
#define CHECKUPDATES_LISTENER_H

#include "checkupdates.h"
#include <wx/event.h>

class wxWindow;
class CheckUpdatesUI;

namespace eviacam {

class CheckUpdatesManager : public wxEvtHandler
{
public:
	CheckUpdatesManager(wxWindow* parent= NULL);
	virtual ~CheckUpdatesManager();

	void LaunchBackground();
	void LaunchForeground();

private:
	void OnCheckUpdatesFinished(wxCommandEvent& event);
	
	CheckUpdates m_checker;
	wxWindow* m_parent;
	CheckUpdatesUI* m_pCheckUpdatesUI;
};

} // namespace eviacam

#endif // CHECKUPDATES_LISTENER_H
