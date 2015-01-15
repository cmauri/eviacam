/////////////////////////////////////////////////////////////////////////////
// Name:        website.cpp
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

#include "website.h"
#include "viacamcontroller.h"
#include "eviacamapp.h"

#include <wx/string.h>
#include <wx/intl.h>
#include <wx/utils.h>

namespace eviacam {

static
wxString GetEViacamWesiteURL()
{
	wxString url(_T("http://eviacam.crea-si.com/land.php?lang="));

	url.Append(wxGetApp().GetController().GetLocale()->GetCanonicalName());

	return url;
}

void OpenEViacamWebsiteOnBrowser()
{
	wxString url= GetEViacamWesiteURL();
	wxLaunchDefaultBrowser(url);
}

void OpenDonationsEViacamWebsiteOnBrowser()
{
	wxString url= GetEViacamWesiteURL();
	url.Append(_T("&donation=yes"));
	wxLaunchDefaultBrowser(url);
}

};
