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
#include "website.h"

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
		<center><img src='#DATA_PATH#/eviacam.png' width='120' height='120'/></center> \
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
	if (!event.GetLinkInfo().GetHref().CmpNoCase(_T("donation")))
		eviacam::OpenDonationsEViacamWebsiteOnBrowser();
	else
		eviacam::OpenEViacamWebsiteOnBrowser();

	event.Skip(false);
}

