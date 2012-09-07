/////////////////////////////////////////////////////////////////////////////
// Name:        wabout.cpp
// Purpose:  
// Author:      Cesar Mauri Loba (cesar at crea-si dot com)
// Modified by: 
// Created:     
// Copyright:   (C) 2008-11 Cesar Mauri Loba - CREA Software Systems
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

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

////@begin includes
////@end includes

#include <wx/stdpaths.h>
#include <wx/textfile.h>
#include "wabout.h"
#include "eviacamapp.h"
#include "viacamcontroller.h"
#include "version.h"
#include "paths.h"

////@begin XPM images
////@end XPM images


/*!
 * WAbout type definition
 */

IMPLEMENT_DYNAMIC_CLASS( WAbout, wxDialog )


/*!
 * WAbout event table definition
 */

BEGIN_EVENT_TABLE( WAbout, wxDialog )

////@begin WAbout event table entries
    EVT_HTML_LINK_CLICKED( ID_HTMLWINDOW, WAbout::OnHtmlwindowLinkClicked )

    EVT_BUTTON( ID_BUTTON_CLOSE, WAbout::OnButtonCloseClick )

////@end WAbout event table entries

END_EVENT_TABLE()


/*!
 * WAbout constructors
 */

WAbout::WAbout()
{
    Init();
}

WAbout::WAbout( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
    Init();
    Create(parent, id, caption, pos, size, style);
}


/*!
 * WAbout creator
 */

bool WAbout::Create( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
////@begin WAbout creation
    SetExtraStyle(wxWS_EX_BLOCK_EVENTS);
    wxDialog::Create( parent, id, caption, pos, size, style );

    CreateControls();
    if (GetSizer())
    {
        GetSizer()->SetSizeHints(this);
    }
    Centre();
////@end WAbout creation
    return true;
}


/*!
 * WAbout destructor
 */

WAbout::~WAbout()
{
////@begin WAbout destruction
////@end WAbout destruction
}


/*!
 * Member initialisation
 */

void WAbout::Init()
{
////@begin WAbout member initialisation
////@end WAbout member initialisation
}


/*!
 * Control creation for WAbout
 */

void WAbout::CreateControls()
{    
////@begin WAbout content construction
    WAbout* itemDialog1 = this;

    wxBoxSizer* itemBoxSizer2 = new wxBoxSizer(wxVERTICAL);
    itemDialog1->SetSizer(itemBoxSizer2);

    wxHtmlWindow* itemHtmlWindow3 = new wxHtmlWindow( itemDialog1, ID_HTMLWINDOW, wxDefaultPosition, wxSize(600, 450), wxHW_SCROLLBAR_AUTO|wxSUNKEN_BORDER|wxHSCROLL|wxVSCROLL );
    itemBoxSizer2->Add(itemHtmlWindow3, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

    wxButton* itemButton4 = new wxButton( itemDialog1, ID_BUTTON_CLOSE, _("Close"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer2->Add(itemButton4, 0, wxALIGN_RIGHT|wxALL, 5);

////@end WAbout content construction

	wxString page(_T("\
		<body bgcolor='#F8F8F8'> \
		<font face='Arial, Helvetica, sans-serif' size='+1'> \
		<center><img src='#DATA_PATH#/eviacam.png' width='150' height='120'/></center> \
		<hr align='center' size='1'/> \
		<strong>Enable Viacam &mdash; v#APPVERSION#</strong><br> \
	"));
	page.Append(_("A webcam based mouse emulator"));
	page.Append(_T("<p>"));
	page.Append(wxString(COPYRIGHT, wxConvUTF8));
	page.Append(_T(" "));
	page.Append(wxString(PUBLISHER, wxConvUTF8));
	page.Append(_T("<p>"));
	page.Append(_("If you find this software useful then please consider \
		<a href='donation'>making a small donation</a> to show your appreciation \
		and help support the continued development. Thanks!\
	"));
	page.Append(_T("</p><p><a href='website'>"));
	page.Append(_("Visit website"));
	page.Append(_T("</a></p> \
		<hr align='center' size='1'/> \
		<strong> \
	"));
	page.Append(_("Thanks"));
	page.Append(_T("</strong> \
		<center><p><img src='#DATA_PATH#/fve.png' border='1' align='middle'> &nbsp;&nbsp; \
		<img src='#DATA_PATH#/guadalinfo_logo.png' border='1' align='middle'></p></center> \
		<center><p><img src='#DATA_PATH#/stsi_logo.png' width='200' height='57' border='1' align='middle'>&nbsp;&nbsp; \
		<img src='#DATA_PATH#/appc_logo.png' width='200' height='57' border='1' align='middle'> </p></center> \
		</font> \
		<font size='+0'><p> \
	"));

	wxTextFile thanksFile(eviacam::GetDataDir() + wxString(_T("/THANKS")));

	if (thanksFile.Open())
	{
		wxString str;

		for ( str = thanksFile.GetFirstLine(); !thanksFile.Eof(); str = thanksFile.GetNextLine() )
		{	page.Append (str);
			page.Append (_T("<br>"));
		}
	}
	
	page.Append(_T("</p></font> \
		<hr align='center' size='1'/> \
		<font face='Arial, Helvetica, sans-serif' size='+1'> \
		<strong> \
	"));
	page.Append(_("License"));
	page.Append(_T("</strong> \
		<p>\
	"));
	page.Append(_("Released under the GNU/GPL v3 license. See COPYING file for details."));
	page.Append(_T("</p> \
		</font> \
		</body> \
	"));
	
	page.Replace (_T("#DATA_PATH#"), eviacam::GetDataDir(), true);
	page.Replace (_T("#APPVERSION#"), _T(VERSION), true);

	itemHtmlWindow3->SetPage(page);
}


/*!
 * Should we show tooltips?
 */

bool WAbout::ShowToolTips()
{
    return true;
}

/*!
 * Get bitmap resources
 */

wxBitmap WAbout::GetBitmapResource( const wxString& name )
{
    // Bitmap retrieval
////@begin WAbout bitmap retrieval
    wxUnusedVar(name);
    return wxNullBitmap;
////@end WAbout bitmap retrieval
}

/*!
 * Get icon resources
 */

wxIcon WAbout::GetIconResource( const wxString& name )
{
    // Icon retrieval
////@begin WAbout icon retrieval
    wxUnusedVar(name);
    return wxNullIcon;
////@end WAbout icon retrieval
}


/*!
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_BUTTON_CLOSE
 */

void WAbout::OnButtonCloseClick( wxCommandEvent& WXUNUSED(event) )
{
////@begin wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_BUTTON_CLOSE in WAbout.
    // Before editing this code, remove the block markers.
    EndModal(ID_BUTTON_CLOSE);
////@end wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_BUTTON_CLOSE in WAbout. 
}




/*!
 * wxEVT_COMMAND_HTML_LINK_CLICKED event handler for ID_HTMLWINDOW
 */

void WAbout::OnHtmlwindowLinkClicked( wxHtmlLinkEvent& event )
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

	if (!event.GetLinkInfo().GetHref().CmpNoCase(_T("donation")))
		url.Append(_T("#contribute"));
	wxLaunchDefaultBrowser(url);

	event.Skip(false);
}

