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
#include "clickwindowbitmapvertical.h"
#include "clickwindowvertical.h"
#include "wviacam.h"
#include "wx/window.h"

CClickWindowController::CClickWindowController(CViacamController & pViacamController)
{
	m_pViacamController= &pViacamController;

	// Create text window
	m_pWindowText= new CClickWindowText (NULL);	
	m_pWindowText->SetController (*this);	
	
	// Create bitmap window
	m_pWindowBitmap= new CClickWindowBitmap (NULL);
	m_pWindowBitmap->SetController (*this);	
	
	// Create bitmap vertical window
	m_pWindowBitmapVertical= new CClickWindowBitmapVertical (NULL);
	m_pWindowBitmapVertical->SetController (*this);	
	
	// Create text vertical window
	m_pWindowTextVertical= new CClickWindowTextVertical (NULL);
	m_pWindowTextVertical->SetController (*this);	
	
	// Set current window
	m_pWindow= m_pWindowBitmap;
	
	// FIXME: implement this using the observer pattern
	m_pViacamController->GetMainWindow()->Connect 
			(ID_WVIACAM, wxEVT_SHOW, wxShowEventHandler(CClickWindow::OnMainWindowShow), NULL, m_pWindow);
	
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
		m_pViacamController->GetMainWindow()->Disconnect 
			(ID_WVIACAM, wxEVT_SHOW, wxShowEventHandler(CClickWindow::OnMainWindowShow), NULL, m_pWindowBitmapVertical);
		m_pViacamController->GetMainWindow()->Disconnect 
			(ID_WVIACAM, wxEVT_SHOW, wxShowEventHandler(CClickWindow::OnMainWindowShow), NULL, m_pWindowTextVertical);
	
	}
	if (m_pWindow)
	{
		m_pWindowText->Show(false);
		m_pWindowText->Destroy ();
		m_pWindowText= NULL;
		m_pWindowBitmap->Show(false);
		m_pWindowBitmap->Destroy ();
		m_pWindowBitmap= NULL;
		m_pWindowBitmapVertical->Show(false);
		m_pWindowBitmapVertical->Destroy ();
		m_pWindowBitmapVertical= NULL;
		m_pWindowTextVertical->Show(false);
		m_pWindowTextVertical->Destroy ();
		m_pWindowTextVertical= NULL;
		m_pWindow= NULL;
	}	
}

void CClickWindowController::Show(bool show) 
{
	if (show!= m_pWindow->IsShown())
	{
	/*
		if (m_autohide)
			m_pWindow->SetClickWindowStyle((CClickWindow::EClickWindowStatus)m_status,
				(CClickWindow::EDocking)m_dockingMode, show);
		else
			m_pWindow->SetClickWindowStyle(CClickWindow::DOCKED,
				(CClickWindow::EDocking)m_dockingMode, show);
	*/
		m_pWindow->Show(show);	
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
	wxRect pos= m_pWindow->GetRect();
	wxRect parentPos= m_pWindow->GetNoClickButton()->GetScreenRect();
	pos.Offset(0, parentPos.GetY() - pos.GetY());
	
	
	int top= pos.GetTop();
	int bottom= pos.GetBottom();
	int left= pos.GetLeft();
	int right= pos.GetRight();
	if (top < 0) top= 0;
	if (bottom < 0) bottom= 0;
	if (left < 0) left= 0;
	if (right < 0) right= 0;
	
	if (y<= bottom && y>= top && x>= left && x<= right)
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
		{
#if defined(__WXMSW__)
			retval= CClickWindowController::ACT_LEFT_UP;
#else
			retval= CClickWindowController::ACT_LEFT_CLICK;
#endif
		}
		else
		{
			switch (m_currentButton)
			{
			case CClickWindowController::LEFT:
				retval= CClickWindowController::ACT_LEFT_CLICK;
				break;
			case CClickWindowController::MIDDLE:
				retval= CClickWindowController::ACT_MIDDLE_CLICK;
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

// Select appropiate window taking into account design and location
void CClickWindowController::SelectAppropiateWindow (EDesign design, ELocation location)
{
	bool isAutohide= m_autohide;
	bool isHorizontal=
		(location == FLOATING_HORIZONTAL || location == TOP_DOCKED || location == BOTTOM_DOCKED);
		
	WXAppBar::EDocking oldDocking= m_pWindow->GetDockingMode();
	
	if (design == CClickWindowController::NORMAL) {
		if (isHorizontal)
			m_pWindow= m_pWindowBitmap;
		else
			m_pWindow= m_pWindowBitmapVertical;
	}
	else {
		if (isHorizontal)
			m_pWindow= m_pWindowText;
		else
			m_pWindow= m_pWindowTextVertical;
	}
	
	// FIXME: implement this using the observer pattern
	m_pViacamController->GetMainWindow()->Connect 
			(ID_WVIACAM, wxEVT_SHOW, wxShowEventHandler(CClickWindow::OnMainWindowShow), NULL, m_pWindow);
	
	m_pWindow->SetDockingMode(oldDocking);
	SetAutohide(isAutohide);
}

void CClickWindowController::SetDesign(CClickWindowController::EDesign design)
{	
	if (m_design== design) return;

	// Sanity check
	if (design != CClickWindowController::NORMAL && design != CClickWindowController::THIN) {
		SetDesign(CClickWindowController::NORMAL);
		return;
	}

	bool wasShown= IsShown();
	if (wasShown) Show(false);

	SelectAppropiateWindow (design, m_location);
		
	if (wasShown) Show(true);
	
	m_design= design;
}

void CClickWindowController::SetLocation(CClickWindowController::ELocation location)
{	
	if (m_location== location) return;

	// Sanity check
	if (location< FLOATING_HORIZONTAL || location> RIGHT_DOCKED) {
		SetLocation(TOP_DOCKED);
		return;
	}
	
	bool isShown= IsShown();	
	if (isShown) Show(false);
	
	SelectAppropiateWindow (m_design, location);
	
	switch(location) {
		case FLOATING_HORIZONTAL:
		case FLOATING_VERTICAL:
			m_pWindow->SetDockingMode(WXAppBar::NON_DOCKED);
			break;
		case TOP_DOCKED:
			m_pWindow->SetDockingMode(WXAppBar::TOP_DOCKED);
			break;
		case BOTTOM_DOCKED:
			m_pWindow->SetDockingMode(WXAppBar::BOTTOM_DOCKED);
			break;
		case LEFT_DOCKED:
			m_pWindow->SetDockingMode(WXAppBar::LEFT_DOCKED);
			break;
		case RIGHT_DOCKED:
			m_pWindow->SetDockingMode(WXAppBar::RIGHT_DOCKED);
			break;
		default:
			assert (false);
	}
		
	if (isShown) Show(true);
	
	m_location= location;	
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

void CClickWindowController::SetAutohide(bool enable) 
{
	// TODO
//	bool isShown= IsShown();
//	if (isShown) Show (false);
	
	m_autohide= enable;
	m_pWindow->SetAutohideMode(m_autohide);
//	if (m_autohide) m_status= CClickWindowController::HIDDEN;
//	else m_status= CClickWindowController::DOCKED;
	
//	if (isShown) Show (true);
}

inline
bool CClickWindowController::IsCursorOverNoClickButton(long x, long y)
{
	wxRect pos= m_pWindow->GetNoClickButton()->GetScreenRect();
	if (y<= pos.GetBottom() && y>= pos.GetTop() && x>= pos.GetLeft() && x<= pos.GetRight())
		return true;
	else	
		return false;	
}



void CClickWindowController::NotifyShowMainWindowClick ()
{
	m_pViacamController->GetMainWindow()->Show (!m_pViacamController->GetMainWindow()->IsShown());
}

void CClickWindowController::SetWarnBarOverlap (bool value)
{
	m_pWindow->SetWarnBarOverlap(value);
}

// Configuration methods
void CClickWindowController::InitDefaults()
{
	SetFastMode (false);
	SetDesign (CClickWindowController::NORMAL);	
	//SetDockingMode(CClickWindowController::TOP_DOCKING);
	SetLocation (CClickWindowController::TOP_DOCKED);
	SetAutohide(false);
	//m_status= CClickWindowController::HIDDEN;
}

void CClickWindowController::WriteProfileData(wxConfigBase* pConfObj)
{
	pConfObj->SetPath (_T("clickWindow"));	

	pConfObj->Write(_T("fastMode"), m_fastMode);
	pConfObj->Write(_T("design"), (long) m_design);
	pConfObj->Write(_T("location"), (long) m_location);
	pConfObj->Write(_T("autohide"), m_autohide);
	pConfObj->Write(_T("warnBarOverlap"), m_pWindow->GetWarnBarOverlap());
	pConfObj->SetPath (_T(".."));
}

void CClickWindowController::ReadProfileData(wxConfigBase* pConfObj)
{
	pConfObj->SetPath (_T("clickWindow"));

	long design, location;
	bool warnBarOverlap= true;
	
	pConfObj->Read(_T("fastMode"), &m_fastMode);
	if (pConfObj->Read(_T("design"), &design))
		SetDesign ((CClickWindowController::EDesign) design);
	if (pConfObj->Read(_T("location"), &location))
		SetLocation (static_cast<ELocation>(location));		
	pConfObj->Read(_T("autohide"), &m_autohide);
		SetAutohide(m_autohide);
	pConfObj->Read(_T("warnBarOverlap"), &warnBarOverlap);
		m_pWindow->SetWarnBarOverlap(warnBarOverlap);
	
	pConfObj->SetPath (_T(".."));
}

const bool CClickWindowController::IsShown () const
{
	return m_pWindow->IsShown();
}
