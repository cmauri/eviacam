/////////////////////////////////////////////////////////////////////////////
// Name:        clickwindowcontroller.h
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
#ifndef CLICKWINDOWCONTROLLER_H
#define CLICKWINDOWCONTROLLER_H

#include "configbase.h"
#include "mousecommand.h"

class CViacamController;
class CClickWindow;
class WXAppBar;

class CClickWindowController  : public CConfigBase
{
public:
	enum EButton { NO_CLICK= 0, LEFT, MIDDLE, RIGHT, DRAG, DBLCLICK };
	enum EDesign { NORMAL= 0, THIN };
	enum ELocation { FLOATING_HORIZONTAL= 0, FLOATING_VERTICAL, TOP_DOCKED, BOTTOM_DOCKED, LEFT_DOCKED, RIGHT_DOCKED };

	CClickWindowController(CViacamController & pViacamController);
	~CClickWindowController();

	void Finalize ();

	void Show (bool show);
	const bool IsShown () const;

	//Return to default state
	void Reset();

	// Get the next action that should be sent. Called by the mouse controller.
	mousecmd::mousecmd GetAction(long x, long y);
	
	// Notifies click bar that the click action has to be sent
	// and where. Updates internal state. Called by the mouse controller.
	void ActionDone(long x, long y);

	inline const EDesign GetDesign();
	void SetDesign(EDesign design);

	inline const bool GetFastMode() const;
	void SetFastMode(bool enable);
	
	ELocation GetLocation() const { return m_location; }
	void SetLocation(ELocation);

	inline const EButton GetCurrentButton () const;
	inline const EButton GetLockedButton () const;

	bool GetEnabled () const { return  m_enabled; }
	
	bool GetAutohide() const { return m_autohide; }
	void SetAutohide(bool enable);
	
	void SetWarnBarOverlap(bool enable);

//	void AutohideClickWindow(long x, long y);

	// Configuration methods
	virtual void InitDefaults();
	virtual void ReadProfileData(wxConfigBase* pConfObj);
	virtual void WriteProfileData(wxConfigBase* pConfObj);

	//
	// Called from window. 
	//
	// Notifies that button has been clicked.
	void NotifyButtonClick (EButton button);
	// Notifies that button has been entered
	void NotifyButtonEnter (EButton button);
	// Notifies that show/hide button has been clicked.
	void NotifyShowMainWindowClick ();
	
private:
	bool IsCursorOverNoClickButton(long x, long y);
	bool IsCursorOverWindow(long x, long y);
	void SelectAppropriateWindow (EDesign design, ELocation location);

	// Associations
	CClickWindow* m_pWindow;
	CClickWindow* m_pWindowText;
	CClickWindow* m_pWindowBitmap;
	CClickWindow* m_pWindowBitmapVertical;
	CClickWindow* m_pWindowTextVertical;
	
	CViacamController * m_pViacamController;
	
	bool m_enabled;
	EButton m_currentButton;
	EButton m_lockedButton;
	bool m_halfDragClick;
	
	bool m_fastMode;
	EDesign m_design;
	ELocation m_location;
	bool m_autohide;
};

inline const CClickWindowController::EDesign CClickWindowController::GetDesign()
{
	return m_design;
}

inline const bool CClickWindowController::GetFastMode() const
{
	return m_fastMode;
}

inline const CClickWindowController::EButton CClickWindowController::GetCurrentButton () const
{
	return m_currentButton;
}

inline const CClickWindowController::EButton CClickWindowController::GetLockedButton () const
{
	return m_lockedButton;
}

#endif
