/////////////////////////////////////////////////////////////////////////////
// Name:        clickwindowtext.cpp
// Purpose:  
// Author:      Cesar Mauri Loba (cesar at crea-si dot com)
// Modified by: 
// Created:     
// Copyright:   (C) 2008-09 Cesar Mauri Loba - CREA Software Systems
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

#include "clickwindowtext.h"
#include "clickwindowcontroller.h"

/*!
 * CClickWindowText constructors
 */

CClickWindowText::CClickWindowText( wxWindow* parent,  const wxString& name)
{
    m_noClickButton = NULL;
    m_leftButton = NULL;
    m_middleButton = NULL;
    m_rightButton = NULL;
    m_dragButton = NULL;
    m_dblClickButton = NULL;
    m_btnShowFrame = NULL;

	Create(parent, name);
}


/*!
 * CClickWindowText destructor
 */

CClickWindowText::~CClickWindowText()
{
}

/*!
 * Control creation for CClickWindow
 */

void CClickWindowText::CreateControls(const wxString& name)
{  
    if (!wxXmlResource::Get()->LoadDialog(this, GetParent(), name))
        wxLogError(wxT("Missing wxXmlResource::Get()->Load() in OnInit()?"));
    m_noClickButton = XRCCTRL(*this, "ID_BUTTON_NOCLICK", wxToggleButton);
    m_leftButton = XRCCTRL(*this, "ID_BTN_LEFT", wxToggleButton);
    m_middleButton = XRCCTRL(*this, "ID_BTN_MIDDLE", wxToggleButton);
    m_rightButton = XRCCTRL(*this, "ID_BTN_RIGHT", wxToggleButton);
    m_dragButton = XRCCTRL(*this, "ID_BTN_DRAG", wxToggleButton);
    m_dblClickButton = XRCCTRL(*this, "ID_BTN_DBLCLICK", wxToggleButton);
    m_btnShowFrame = XRCCTRL(*this, "ID_BUTTON_SHOW_FRAME", wxButton);

	ConnectEvents();
}

void CClickWindowText::UpdateButtons (bool noClickStatus, CClickWindowController::EButton selected, CClickWindowController::EButton locked)
{
	// Update no click button
	m_noClickButton->SetValue (!noClickStatus);
	
	// Update selected button (foreground color)
	switch (selected)
	{
	case CClickWindowController::LEFT:
		UpdateSelectedButtons (GetLeftButton(), GetMiddleButton(), GetRightButton(), GetDragButton(), GetDblClickButton());
		break;
	case CClickWindowController::MIDDLE:
		UpdateSelectedButtons (GetMiddleButton(), GetRightButton(), GetDragButton(), GetDblClickButton(), GetLeftButton());
		break;
	case CClickWindowController::RIGHT:
		UpdateSelectedButtons (GetRightButton(), GetDragButton(), GetDblClickButton(), GetLeftButton(), GetMiddleButton());
		break;
	case CClickWindowController::DRAG:
		UpdateSelectedButtons (GetDragButton(), GetDblClickButton(), GetLeftButton(), GetMiddleButton(), GetRightButton());
		break;
	case CClickWindowController::DBLCLICK:
		UpdateSelectedButtons (GetDblClickButton(), GetLeftButton(), GetMiddleButton(), GetRightButton(), GetDragButton());
		break;
	default:
		assert (false);
	}

	// Update locked button (toggled)
	switch (locked)
	{
	case CClickWindowController::LEFT:
		UpdateLockedButtons (GetLeftButton(), GetMiddleButton(), GetRightButton(), GetDragButton(), GetDblClickButton());
		break;
	case CClickWindowController::MIDDLE:
		UpdateLockedButtons (GetMiddleButton(), GetRightButton(), GetDragButton(), GetDblClickButton(), GetLeftButton());
		break;
	case CClickWindowController::RIGHT:
		UpdateLockedButtons (GetRightButton(), GetDragButton(), GetDblClickButton(), GetLeftButton(), GetMiddleButton());
		break;
	case CClickWindowController::DRAG:
		UpdateLockedButtons (GetDragButton(), GetDblClickButton(), GetLeftButton(), GetMiddleButton(), GetRightButton());
		break;
	case CClickWindowController::DBLCLICK:
		UpdateLockedButtons (GetDblClickButton(), GetLeftButton(), GetMiddleButton(), GetRightButton(), GetDragButton());
		break;
	default:
		assert (false);
	}
}

void CClickWindowText::UpdateSelectedButtons (wxControl* btnSelected, wxControl* btnNoSelected1, 
								 wxControl* btnNoSelected2,wxControl* btnNoSelected3,wxControl* btnNoSelected4)
{
	if (!((wxToggleButton*)btnSelected)->GetValue()) ((wxToggleButton*)btnSelected)->SetValue (true);
	if (((wxToggleButton*)btnNoSelected1)->GetValue()) ((wxToggleButton*)btnNoSelected1)->SetValue (false);
	if (((wxToggleButton*)btnNoSelected2)->GetValue()) ((wxToggleButton*)btnNoSelected2)->SetValue (false);
	if (((wxToggleButton*)btnNoSelected3)->GetValue())((wxToggleButton*) btnNoSelected3)->SetValue (false);
	if (((wxToggleButton*)btnNoSelected4)->GetValue())((wxToggleButton*) btnNoSelected4)->SetValue (false);
	
}

void CClickWindowText::UpdateLockedButtons (wxControl* btnLocked, wxControl* btnNoLocked1, 
											 wxControl* btnNoLocked2, wxControl* btnNoLocked3, wxControl* btnNoLocked4)
{
	wxString label= ((wxToggleButton*)btnLocked)->GetLabel();
	label.SetChar (0, wxChar('['));
	label.SetChar (label.Length()-1, wxChar(']'));
	((wxToggleButton*)btnLocked)->SetLabel(label);

	label= ((wxToggleButton*)btnNoLocked1)->GetLabel();
	label.SetChar (0, wxChar(' '));
	label.SetChar (label.Length()-1, wxChar(' '));
	((wxToggleButton*)btnNoLocked1)->SetLabel(label);

	label= ((wxToggleButton*)btnNoLocked2)->GetLabel();
	label.SetChar (0, wxChar(' '));
	label.SetChar (label.Length()-1, wxChar(' '));
	((wxToggleButton*)btnNoLocked2)->SetLabel(label);

	label= ((wxToggleButton*)btnNoLocked3)->GetLabel();
	label.SetChar (0, wxChar(' '));
	label.SetChar (label.Length()-1, wxChar(' '));
	((wxToggleButton*)btnNoLocked3)->SetLabel(label);

	label= ((wxToggleButton*)btnNoLocked4)->GetLabel();
	label.SetChar (0, wxChar(' '));
	label.SetChar (label.Length()-1, wxChar(' '));
	((wxToggleButton*)btnNoLocked4)->SetLabel(label);
}

wxControl* CClickWindowText::GetNoClickButton()
{
	return m_noClickButton;
}

wxControl* CClickWindowText::GetLeftButton()
{
	return m_leftButton;
}

wxControl* CClickWindowText::GetMiddleButton()
{
	return m_middleButton;
}

wxControl* CClickWindowText::GetRightButton()
{
	return m_rightButton;
}

wxControl* CClickWindowText::GetDragButton()
{
	return m_dragButton;
}

wxControl* CClickWindowText::GetDblClickButton()
{
	return m_dblClickButton;
}

wxControl* CClickWindowText::GetShowFrame()
{
	return m_btnShowFrame;
}
	
void CClickWindowText::OnMainWindowShow ( wxShowEvent& event )
{
	if (event.IsShown())
		m_btnShowFrame->SetLabel(_("Hide main window"));
	else
		m_btnShowFrame->SetLabel(_("Show main window"));		
	event.Skip (false);
}
