/////////////////////////////////////////////////////////////////////////////
// Name:        wcameradialog.cpp
// Purpose:     
// Author:      C�sar Mauri Loba
// Modified by: 
// Created:     Fri 22 Oct 2010 10:02:49 CEST
// RCS-ID:      
// Copyright:   (C) 2008 Cesar Mauri from CREA Sistemes Informatics
// Licence:     
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
    m_bsizerListControls = NULL;
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

    wxScrolledWindow* itemScrolledWindow3 = new wxScrolledWindow( itemDialog1, ID_SCROLLEDWINDOW1, wxDefaultPosition, wxSize(500, 1000), wxSUNKEN_BORDER|wxVSCROLL );
    itemBoxSizer2->Add(itemScrolledWindow3, 0, wxALIGN_LEFT|wxALL, 5);
    itemScrolledWindow3->SetScrollbars(10, 10, 10, 10);
    wxPanel* itemPanel4 = new wxPanel( itemScrolledWindow3, ID_PANEL2, wxDefaultPosition, wxSize(500, 1000), wxSUNKEN_BORDER|wxTAB_TRAVERSAL );
    m_bsizerListControls = new wxBoxSizer(wxVERTICAL);
    itemPanel4->SetSizer(m_bsizerListControls);

    itemScrolledWindow3->SetMinSize(wxSize(500, 1000));

////@end WCameraDialog content construction

	unsigned int numControl;
	wxSizer* sizer;
	
	for (numControl=0; numControl < m_pCamera->GetCameraControlsCount(); numControl++)
	{
		sizer = ControlNumberWidget( *(m_pCamera->GetCameraControl(numControl)), itemPanel4 );
		m_sizerList.push_back(sizer);
	}
	
}


wxSizer* WCameraDialog::ControlNumberWidget (const CCameraControl& cc, wxPanel* panel)
{
	ControlInfo ci;
	ci.cc = &cc;
			
	wxBoxSizer* itemBoxSizer = new wxBoxSizer(wxHORIZONTAL);
	m_bsizerListControls->Add(itemBoxSizer, 0, wxALIGN_LEFT|wxALL, 5);
	
	wxStaticText* itemStaticText = new wxStaticText( panel, m_lastId+1, wxString(cc.GetName(),wxConvUTF8), wxDefaultPosition, wxSize(100, -1), 0 );
	itemBoxSizer->Add(itemStaticText, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

	switch (cc.GetType())
	{
		case CCameraControl::CCTYPE_BOOLEAN:
		{	
			wxCheckBox* itemCheckBox = new wxCheckBox( panel, m_lastId+2, _(""), wxDefaultPosition, wxDefaultSize, 0 );
			itemCheckBox->SetValue(cc.GetValue());
			itemBoxSizer->Add(itemCheckBox, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
			
			ci.type.checkbox = itemCheckBox;
			itemCheckBox->Connect(m_lastId+2, wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler(WCameraDialog::OnCheckboxClick), NULL, this);						
		}		
		break;
	
		case CCameraControl::CCTYPE_CHOICE:
		{
			wxArrayString choiceStrings;
			for (int i=0; i<=cc.GetMaximumValue(); i++)
				choiceStrings.Add(wxString (cc.GetChoiceName(i), wxConvUTF8));
				
			wxChoice* itemChoice = new wxChoice( panel, m_lastId+2, wxDefaultPosition, wxDefaultSize, choiceStrings, 0 );
			itemChoice->SetSelection(cc.GetValue());		
			itemBoxSizer->Add(itemChoice, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
			
			ci.type.choice = itemChoice;
			itemChoice->Connect(m_lastId+2, wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler(WCameraDialog::OnChoiceSelected), NULL, this);						
		}
		break;	
	
		case CCameraControl::CCTYPE_NUMBER:
		{
			wxSlider* itemSlider = new wxSlider( panel, m_lastId+2, cc.GetValue(), cc.GetMinimumValue(), cc.GetMaximumValue(), wxDefaultPosition, wxSize(200, -1), wxSL_HORIZONTAL );
			itemBoxSizer->Add(itemSlider, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

			wxTextCtrl* itemTextCtrl = new wxTextCtrl( panel, m_lastId+3, wxString::Format(wxT("%i"),cc.GetValue()), wxDefaultPosition, wxDefaultSize, 0 );
			itemTextCtrl->SetEditable(false);		
			itemBoxSizer->Add(itemTextCtrl, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
			
			ci.type.slider = itemSlider;
			ci.textCtrl = itemTextCtrl;
			
			itemSlider->Connect(m_lastId+2, wxEVT_COMMAND_SLIDER_UPDATED, wxCommandEventHandler(WCameraDialog::OnSliderUpdated), NULL, this);				
		}
		break;
	
	}
	m_lastId += 4;
	m_controlList.push_back(ci);
	
    return itemBoxSizer;
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
