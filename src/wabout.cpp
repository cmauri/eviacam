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
#include "version.h"


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

	wxString page (_T("<font face=\"Arial, Helvetica, sans-serif\">\n<h1><img src=\"#DATA_PATH#/viacam_cam4_html.png\" width=\"120\" height=\"96\">Enable Viacam</h1>\n<p>"));
	page.Append (_("A cross platform webcam based mouse emulator"));
	page.Append (_T("</p>\n<h4>v#APPVERSION#</p></h4>\n<p>(c) 2008-11 C&eacute;sar Mauri Loba - CREA Software Systems<br>\nhttp://viacam.org<br>\nE-Mail: Cesar Mauri &lt;cesar@crea-si.com&gt;</p>\n<p> "));
	page.Append (_("Released under GNU/GPL license. See license for details.<br>\n  With the support of: "));
	page.Append (_T("</p>\n<p><img src=\"#DATA_PATH#/guadalinfo_logo.png\" border=\"0\"><br>\n<img src=\"#DATA_PATH#/stsi_logo.png\" width=\"200\" height=\"57\" border=\"0\"><br>\n<img src=\"#DATA_PATH#/appc_logo.png\" width=\"200\" height=\"57\" border=\"0\"> </p>\n</font><br><h3>"));
	
	page.Append (_("Contributions:"));
	page.Append (_T("</h3>"));
	
	wxTextFile thanksFile(wxStandardPaths::Get().GetDataDir().Append(_T("/THANKS")));

	if (thanksFile.Open())
	{
		wxString str;

		for ( str = thanksFile.GetFirstLine(); !thanksFile.Eof(); str = thanksFile.GetNextLine() )
		{	page.Append (str);
			page.Append (_T("<br>"));
		}
	}
	
	page.Replace (_T("#DATA_PATH#"), wxStandardPaths::Get().GetDataDir(), true);
	page.Replace (_T("#APPVERSION#"), GetAppVersion(), true);
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



