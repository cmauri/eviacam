/////////////////////////////////////////////////////////////////////////////
// Name:        wcameradialog.cpp
// Purpose:     
// Author:      Cesar Mauri Loba
// Modified by: 
// Created:     Fri 22 Oct 2010 10:02:49 CEST
// RCS-ID:      
// Copyright:   (C) 2010-11 Cesar Mauri from CREA Sistemes Informatics
// Licence:     
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

#include "wcameradialog.h"
#include "crvcamera.h"

////@begin XPM images
////@end XPM images

//TODO: Fix specific value to create our own events.
#define FIRST_CONTROL_ID 12000


/*!
 * WCameraDialog type definition
 */

IMPLEMENT_DYNAMIC_CLASS( WCameraDialog, wxDialog )


/*!
 * WCameraDialog event table definition
 */

BEGIN_EVENT_TABLE( WCameraDialog, wxDialog )

////@begin WCameraDialog event table entries
    EVT_BUTTON( ID_BUTTON_CAMDIALOG_CLOSE, WCameraDialog::OnButtonCamdialogCloseClick )

////@end WCameraDialog event table entries

END_EVENT_TABLE()


/*!
 * WCameraDialog constructors
 */

WCameraDialog::WCameraDialog()
{
    Init();
}

WCameraDialog::WCameraDialog( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
    Init();
    Create(parent, id, caption, pos, size, style);
}

WCameraDialog::WCameraDialog( wxWindow* parent, CCamera* pCamera, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
    Init();
    m_pCamera = pCamera;
    Create(parent, id, caption, pos, size, style);
}

/*!
 * WCameraDialog creator
 */

bool WCameraDialog::Create( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
////@begin WCameraDialog creation
    SetExtraStyle(wxWS_EX_BLOCK_EVENTS);
    wxDialog::Create( parent, id, caption, pos, size, style );

    CreateControls();
    if (GetSizer())
    {
        GetSizer()->SetSizeHints(this);
    }
    Centre();
////@end WCameraDialog creation
    return true;
}


/*!
 * WCameraDialog destructor
 */

WCameraDialog::~WCameraDialog()
{
////@begin WCameraDialog destruction
////@end WCameraDialog destruction
}


/*!
 * Member initialisation
 */

void WCameraDialog::Init()
{
////@begin WCameraDialog member initialisation
    m_scrollWindow = NULL;
    m_controlsSizer = NULL;
////@end WCameraDialog member initialisation
	m_pCamera = NULL;
	m_lastId = FIRST_CONTROL_ID;
}


/*!
 * Control creation for WCameraDialog
 */

void WCameraDialog::CreateControls()
{    
////@begin WCameraDialog content construction
    WCameraDialog* itemDialog1 = this;

    wxBoxSizer* itemBoxSizer2 = new wxBoxSizer(wxVERTICAL);
    itemDialog1->SetSizer(itemBoxSizer2);

    wxPanel* itemPanel3 = new wxPanel( itemDialog1, ID_PANEL2, wxDefaultPosition, wxDefaultSize, wxSUNKEN_BORDER|wxTAB_TRAVERSAL );
    itemBoxSizer2->Add(itemPanel3, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

    m_scrollWindow = new wxScrolledWindow( itemPanel3, ID_SCROLLEDWINDOW1, wxDefaultPosition, wxSize(500, 400), wxSUNKEN_BORDER|wxVSCROLL );
    m_scrollWindow->SetScrollbars(10, 10, 10, 10);
    m_controlsSizer = new wxFlexGridSizer(0, 3, 0, 0);
    m_scrollWindow->SetSizer(m_controlsSizer);

    m_scrollWindow->FitInside();

    wxBoxSizer* itemBoxSizer6 = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer2->Add(itemBoxSizer6, 0, wxALIGN_RIGHT|wxALL, 5);

    wxButton* itemButton7 = new wxButton( itemDialog1, ID_BUTTON_CAMDIALOG_CLOSE, _("Close"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer6->Add(itemButton7, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

////@end WCameraDialog content construction
	
	// Add controls
	for (unsigned int numControl= 0; numControl < m_pCamera->GetCameraControlsCount(); numControl++)
		CreateCameraControlWidget( *(m_pCamera->GetCameraControl(numControl)), m_scrollWindow, m_controlsSizer);

	// TODO: automatically set size to fit content
//	wxSize size= m_controlsSizer->GetSize();
//	printf ("Size: %d, %d\n", size.GetWidth(), size.GetHeight());
//	size= m_scrollWindow->GetSize();	
//	printf ("Size: %d, %d\n", size.GetWidth(), size.GetHeight());
}


void WCameraDialog::CreateCameraControlWidget (const CCameraControl& cc, wxWindow* parent, wxSizer* sizer)
{
	ControlInfo ci;
	ci.cc = &cc;

	wxStaticText* itemStaticText = new wxStaticText( parent, m_lastId+1, wxString(cc.GetName(),wxConvUTF8) + _T(":"), wxDefaultPosition, wxDefaultSize, 0); // wxSize(-1, -1), 0 );
	sizer->Add(itemStaticText, 0, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

	switch (cc.GetType())
	{
		case CCameraControl::CCTYPE_BOOLEAN:
		{	
			wxCheckBox* itemCheckBox = new wxCheckBox( parent, m_lastId+2, _T(""), wxDefaultPosition, wxDefaultSize, 0 );
			itemCheckBox->SetValue( (cc.GetValue()? true : false) );
			sizer->Add(itemCheckBox, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
			sizer->Add(5, 5, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxALL, 5);
            
			ci.type.checkbox = itemCheckBox;
			itemCheckBox->Connect(m_lastId+2, wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler(WCameraDialog::OnCheckboxClick), NULL, this);						
		}		
		break;
	
		case CCameraControl::CCTYPE_CHOICE:
		{
			wxArrayString choiceStrings;
			for (int i=0; i<=cc.GetMaximumValue(); i++)
				choiceStrings.Add(wxString (cc.GetChoiceName(i), wxConvUTF8));
				
			wxChoice* itemChoice = new wxChoice( parent, m_lastId+2, wxDefaultPosition, wxDefaultSize, choiceStrings, 0 );
			itemChoice->SetSelection(cc.GetValue());		
			sizer->Add(itemChoice, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
            sizer->Add(5, 5, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxALL, 5);
			
			ci.type.choice = itemChoice;
			itemChoice->Connect(m_lastId+2, wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler(WCameraDialog::OnChoiceSelected), NULL, this);						
		}
		break;	
	
		case CCameraControl::CCTYPE_NUMBER:
		{
			wxSlider* itemSlider = new wxSlider( parent, m_lastId+2, cc.GetValue(), cc.GetMinimumValue(), cc.GetMaximumValue(), wxDefaultPosition, wxSize(100, -1), wxSL_HORIZONTAL );
			sizer->Add(itemSlider, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

			wxTextCtrl* itemTextCtrl = new wxTextCtrl( parent, m_lastId+3, wxString::Format(wxT("%i"),cc.GetValue()), wxDefaultPosition, wxDefaultSize, 0 );
			itemTextCtrl->SetEditable(false);		
			sizer->Add(itemTextCtrl, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
			
			ci.type.slider = itemSlider;
			ci.textCtrl = itemTextCtrl;
			
			itemSlider->Connect(m_lastId+2, wxEVT_COMMAND_SLIDER_UPDATED, wxCommandEventHandler(WCameraDialog::OnSliderUpdated), NULL, this);
		}
		break;
		
		case CCameraControl::CCTYPE_BUTTON:
		{
			wxButton* itemButton = new wxButton( parent, m_lastId+2);
			sizer->Add(itemButton, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

			sizer->Add(5, 5, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);
			
			ci.type.button = itemButton;
			ci.textCtrl = NULL;
			
			itemButton->Connect(m_lastId+2, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(WCameraDialog::OnButtonClick), NULL, this);
		}
		break;
		
		default: assert (false);
	
	}
	m_lastId += 4;
	m_controlList.push_back(ci);
}


/*!
 * Should we show tooltips?
 */

bool WCameraDialog::ShowToolTips()
{
    return true;
}

/*!
 * Get bitmap resources
 */

wxBitmap WCameraDialog::GetBitmapResource( const wxString& name )
{
    // Bitmap retrieval
////@begin WCameraDialog bitmap retrieval
    wxUnusedVar(name);
    return wxNullBitmap;
////@end WCameraDialog bitmap retrieval
}

/*!
 * Get icon resources
 */

wxIcon WCameraDialog::GetIconResource( const wxString& name )
{
    // Icon retrieval
////@begin WCameraDialog icon retrieval
    wxUnusedVar(name);
    return wxNullIcon;
////@end WCameraDialog icon retrieval
}


void WCameraDialog::OnCheckboxClick( wxCommandEvent& event )
{
	CCameraControl* cc = (CCameraControl*) m_controlList[(event.GetId()-FIRST_CONTROL_ID)/4].cc;	
	cc->SetValue(event.GetInt());
	event.Skip(false);
}

void WCameraDialog::OnChoiceSelected( wxCommandEvent& event )
{
	CCameraControl* cc = (CCameraControl*) m_controlList[(event.GetId()-FIRST_CONTROL_ID)/4].cc;	
	cc->SetValue(event.GetInt());
	event.Skip(false);
}

void WCameraDialog::OnSliderUpdated( wxCommandEvent& event )
{
	CCameraControl* cc = (CCameraControl*) m_controlList[(event.GetId()-FIRST_CONTROL_ID)/4].cc;	
	cc->SetValue(event.GetInt());
	m_controlList[(event.GetId()-FIRST_CONTROL_ID)/4].textCtrl->SetValue(wxString::Format(wxT("%i"), cc->GetValue()));
	event.Skip(false);
}

void WCameraDialog::OnButtonClick( wxCommandEvent& event )
{
	CCameraControl* cc = (CCameraControl*) m_controlList[(event.GetId()-FIRST_CONTROL_ID)/4].cc;	
	cc->SetValue(1);
	event.Skip(false);
}

/*!
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_BUTTON_CAMDIALOG_CLOSE
 */

void WCameraDialog::OnButtonCamdialogCloseClick( wxCommandEvent& event )
{
	Close();
	event.Skip(false);
}

