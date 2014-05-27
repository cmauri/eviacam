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
	wxString url(_T("http://eviacam.sourceforge.net/"));

	switch(wxGetApp().GetController().GetLocale()->GetLanguage()) {		
	case (wxLANGUAGE_CATALAN):
	case (wxLANGUAGE_VALENCIAN):
		url.Append(_T("index_ca.php"));
		break;
	case (wxLANGUAGE_SPANISH):
	case (wxLANGUAGE_SPANISH_ARGENTINA):
	case (wxLANGUAGE_SPANISH_BOLIVIA):
	case (wxLANGUAGE_SPANISH_CHILE):
	case (wxLANGUAGE_SPANISH_COLOMBIA):
	case (wxLANGUAGE_SPANISH_COSTA_RICA):
	case (wxLANGUAGE_SPANISH_DOMINICAN_REPUBLIC):
	case (wxLANGUAGE_SPANISH_ECUADOR):
	case (wxLANGUAGE_SPANISH_EL_SALVADOR):
	case (wxLANGUAGE_SPANISH_GUATEMALA):
	case (wxLANGUAGE_SPANISH_HONDURAS):
	case (wxLANGUAGE_SPANISH_MEXICAN):
	case (wxLANGUAGE_SPANISH_MODERN):
	case (wxLANGUAGE_SPANISH_NICARAGUA):
	case (wxLANGUAGE_SPANISH_PANAMA):
	case (wxLANGUAGE_SPANISH_PARAGUAY):
	case (wxLANGUAGE_SPANISH_PERU):
	case (wxLANGUAGE_SPANISH_PUERTO_RICO):
	case (wxLANGUAGE_SPANISH_URUGUAY):
	case (wxLANGUAGE_SPANISH_US):
	case (wxLANGUAGE_SPANISH_VENEZUELA):
		url.Append(_T("index_es.php"));
		break;
	case (wxLANGUAGE_GERMAN):
	case (wxLANGUAGE_GERMAN_AUSTRIAN):
	case (wxLANGUAGE_GERMAN_BELGIUM):
	case (wxLANGUAGE_GERMAN_LIECHTENSTEIN):
	case (wxLANGUAGE_GERMAN_LUXEMBOURG):
	case (wxLANGUAGE_GERMAN_SWISS):
		url.Append(_T("index_de.php"));
		break;
	case (wxLANGUAGE_GALICIAN):
		url.Append(_T("index_gl.php"));
		break;
	case (wxLANGUAGE_FRENCH):
	case (wxLANGUAGE_FRENCH_BELGIAN):
	case (wxLANGUAGE_FRENCH_CANADIAN):
	case (wxLANGUAGE_FRENCH_LUXEMBOURG):	
	case (wxLANGUAGE_FRENCH_MONACO):
	case (wxLANGUAGE_FRENCH_SWISS):
		url.Append(_T("index_fr.php"));
		break;
	case (wxLANGUAGE_ITALIAN):
	case (wxLANGUAGE_ITALIAN_SWISS):
		url.Append(_T("index_it.php"));
		break;
	default:
		break;
	}

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
	url.Append(_T("#contribute"));
	wxLaunchDefaultBrowser(url);
}

};