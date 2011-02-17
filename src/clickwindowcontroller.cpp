/////////////////////////////////////////////////////////////////////////////
// Name:        clickwindowcontroller.cpp
// Purpose:  
// Author:      Cesar Mauri Loba (cesar at crea-si dot com)
// Modified by: 
// Created:     
// Copyright:   (C) 2008 Cesar Mauri Loba - CREA Software Systems
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

#include <wx/gdicmn.h>
#include "clickwindowcontroller.h"
#include "viacamcontroller.h"
#include "clickwindowtext.h"
#include "clickwindowbitmap.h"
#include "wviacam.h"

CClickWindowController::CClickWindowController(CViacamController & pViacamController)
{
	m_pViacamController= &pViacamController;

	// Create text window
	m_pWindowText= new CClickWindowText (NULL);	
	m_pWindowText->SetController (*this);	
	// FIXME: implement this using the observer pattern
	m_pViacamController->GetMainWindow()->Connect 
		(ID_WVIACAM, wxEVT_SHOW, wxShowEventHandler(CClickWindow::OnMainWindowShow), NULL, m_pWindowText);
	
	// Create bitmap window
	m_pWindowBitmap= new CClickWindowBitmap (NULL);
	m_pWindowBitmap->SetController (*this);	
	// FIXME: implement this using the observer pattern
	m_pViacamController->GetMainWindow()->Connect 
		(ID_WVIACAM, wxEVT_SHOW, wxShowEventHandler(CClickWindow::OnMainWindowShow), NULL, m_pWindowBitmap);
	
	// Set current window
	m_pWindow= m_pWindowBitmap;
	
	InitDefaults();

	Reset();	
}

CClickWindowController::~CClickWindowController()
{
	Finalize();
}

void CClickWindowController::Finalize ()
{
	if (m_pViacamController->GetMainWindow())
	{
		m_pViacamController->GetMainWindow()->Disconnect 
			(ID_WVIACAM, wxEVT_SHOW, wxShowEventHandler(CClickWindow::OnMainWindowShow), NULL, m_pWindowText);
		m_pViacamController->GetMainWindow()->Disconnect 
			(ID_WVIACAM, wxEVT_SHOW, wxShowEventHandler(CClickWindow::OnMainWindowShow), NULL, m_pWindowBitmap);
	}
	if (m_pWindow)
	{
		m_pWindowText->Show(false);
		m_pWindowText->Destroy ();
		m_pWindowText= NULL;
		m_pWindowBitmap->Show(false);
		m_pWindowBitmap->Destroy ();
		m_pWindowBitmap= NULL;

		m_pWindow= NULL;
	}	
}

void CClickWindowController::Show(bool show) 
{
	if (show!= m_pWindow->IsShown())
	{
		m_pWindow->SetDockingStyle((CClickWindow::EDocking)m_dockingMode, show);
		if (show) m_pWindow->UpdateButtons(GetEnabled(),GetCurrentButton(), GetLockedButton());
	}
}

//Return to default state
void CClickWindowController::Reset() 
{
	m_enabled= true;
	m_currentButton= LEFT;
	m_lockedButton= LEFT;
	m_halfDragClick= false;
}

inline
bool CClickWindowController::IsCursorOverWindow(long x, long y)
{
	// FIXME: fix this, not working properly 
	wxRect parentPos= m_pWindow->GetNoClickButton()->GetRect();
	wxRect pos= m_pWindow->GetRect();
	pos.SetX (pos.GetX() + parentPos.GetX());
	pos.SetY (pos.GetY() + parentPos.GetY());

	if (y<= pos.GetBottom() && y>= pos.GetTop() && x>= pos.GetLeft() && x<= pos.GetRight())
		return true;
	else
		return false;
}

// Get the next action that should be sent
CClickWindowController::EAction CClickWindowController::GetAction(long x, long y)
{
	CClickWindowController::EAction retval= CClickWindowController::ACT_NO_CLICK;

	//wxMutexGuiEnter();

	if (m_enabled)
	{
		if (IsCursorOverWindow(x,y))
#if defined(__WXMSW__)
			retval= CClickWindowController::ACT_LEFT_UP;
#else
			retval= CClickWindowController::ACT_LEFT_CLICK;
#endif
		else
		{
			switch (m_currentButton)
			{
			case CClickWindowController::LEFT:
				retval= CClickWindowController::ACT_LEFT_CLICK;
				break;
			case CClickWindowController::RIGHT:
				retval= CClickWindowController::ACT_RIGHT_CLICK;
				break;
			case CClickWindowController::DRAG:
				if (!m_halfDragClick) retval= CClickWindowController::ACT_LEFT_DOWN;
				else retval= CClickWindowController::ACT_LEFT_UP;
				break;
			case CClickWindowController::DBLCLICK:
				retval= CClickWindowController::ACT_DOUBLE_CLICK;
				break;
			default:
				assert (false);
			}
		}
	}	
	else
	{
		if (IsCursorOverNoClickButton(x, y))
#if defined(__WXMSW__)
			retval= CClickWindowController::ACT_LEFT_UP;
#else
			retval= CClickWindowController::ACT_LEFT_CLICK;
#endif
	}	

	//wxMutexGuiLeave();

	return retval;
}

void CClickWindowController::SetDesign(CClickWindowController::EDesign design) 
{	
	if (m_design== design) return;

	if (design== CClickWindowController::NORMAL	|| 
		design== CClickWindowController::THIN)
	{
		m_design= design;

		bool isShown= IsShown();

		if (isShown) Show(false);

		if (design== CClickWindowController::NORMAL)
			m_pWindow= m_pWindowBitmap;
		else
			m_pWindow= m_pWindowText;

		if (isShown) Show(true);	
	}
	else
	{
		m_design= CClickWindowController::NORMAL;
	}
}

void CClickWindowController::SetDockingMode(CClickWindowController::EDocking dockingMode) 
{	
	if (m_dockingMode== dockingMode) return;
	m_dockingMode = dockingMode;
	m_pWindow->SetDockingStyle((CClickWindow::EDocking)m_dockingMode, IsShown());
}


// Called from mouse controller. Notifies click bar that the click action has 
// to be sent and where. Updates internal state.
void CClickWindowController::ActionDone(long x, long y) 
{
	//wxMutexGuiEnter();
	// If cursor is over click window the notification takes place when
	// mouse event is received otherwise update internal state
	if (!IsCursorOverWindow(x,y))
	{
		if (m_currentButton== CClickWindowController::DRAG)
		{
			if (!m_halfDragClick) m_halfDragClick= true;
			else
			{
				m_halfDragClick= false;
				m_currentButton= m_lockedButton;
			}
		}
		else
			m_currentButton= m_lockedButton;

		m_pWindow->UpdateButtons(GetEnabled(),GetCurrentButton(), GetLockedButton());
	}
	//wxMutexGuiLeave();
}

// Called from window. Notifies that button has been clicked.
void CClickWindowController::NotifyButtonClick (CClickWindowController::EButton button)
{
	if (m_enabled)
	{
		if (button== CClickWindowController::NO_CLICK)
			// Disable click
			m_enabled= false;
		else
		{
			if (m_fastMode || (m_currentButton!= m_lockedButton && button== m_currentButton))				
				m_lockedButton= button;
			m_currentButton= button;
		}

		m_pWindow->UpdateButtons(GetEnabled(),GetCurrentButton(), GetLockedButton());
	}
	else
	{
		// Disabled state. Only handle NO_CLICK button
		if (button== CClickWindowController::NO_CLICK)
		{
			m_enabled= true;

			m_pWindow->UpdateButtons(GetEnabled(),GetCurrentButton(), GetLockedButton());
		}
	}
}

// Called from window. Notifies that button has been entered
void CClickWindowController::NotifyButtonEnter (CClickWindowController::EButton button)
{
	if (m_enabled && m_fastMode)
	{
		if (button!= CClickWindowController::NO_CLICK && button!= m_currentButton)
		{
			m_currentButton= button;
			m_pWindow->UpdateButtons(GetEnabled(),GetCurrentButton(), GetLockedButton());
		}
	}	
}

void CClickWindowController::SetFastMode(bool enable) 
{
	m_fastMode= enable;
}

inline
bool CClickWindowController::IsCursorOverNoClickButton(long x, long y)
{
	wxRect pos= m_pWindow->GetNoClickButton()->GetRect();

	if (y<= pos.GetBottom() && y>= pos.GetTop() && x>= pos.GetLeft() && x<= pos.GetRight())
		return true;
	else	
		return false;	
}



void CClickWindowController::NotifyShowMainWindowClick ()
{
	m_pViacamController->GetMainWindow()->Show (!m_pViacamController->GetMainWindow()->IsShown());
}

// Configuration methods
void CClickWindowController::InitDefaults()
{
	SetFastMode (false);
	SetDesign (CClickWindowController::NORMAL);	
	SetDockingMode(CClickWindowController::NO_DOCKING);
}

void CClickWindowController::WriteProfileData(wxConfigBase* pConfObj)
{
	pConfObj->SetPath (_T("clickWindow"));	

	pConfObj->Write(_T("fastMode"), m_fastMode);
	pConfObj->Write(_T("design"), (long) m_design);
	pConfObj->Write(_T("dockingMode"), (long) m_dockingMode);

	pConfObj->SetPath (_T(".."));
}

void CClickWindowController::ReadProfileData(wxConfigBase* pConfObj)
{
	pConfObj->SetPath (_T("clickWindow"));

	pConfObj->Read(_T("fastMode"), &m_fastMode);
	long design, dockingMode;
	if (pConfObj->Read(_T("design"), &design))
		SetDesign ((CClickWindowController::EDesign) design);
	if (pConfObj->Read(_T("dockingMode"), &dockingMode))
		SetDockingMode((CClickWindowController::EDocking) dockingMode);
	
	pConfObj->SetPath (_T(".."));
}

const bool CClickWindowController::IsShown () const
{
	return m_pWindow->IsShown();
}
