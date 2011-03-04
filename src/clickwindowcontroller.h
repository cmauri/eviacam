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

class CViacamController;
class CClickWindow;
class WXAppBar;

class CClickWindowController  : public CConfigBase
{
  public:
	enum EButton { NO_CLICK= 0, LEFT, RIGHT, DRAG, DBLCLICK };
	enum EAction { ACT_NO_CLICK = 0, ACT_LEFT_CLICK, ACT_RIGHT_CLICK, ACT_LEFT_DOWN, ACT_LEFT_UP, ACT_DOUBLE_CLICK };
	enum EDesign { NORMAL= 0, THIN };
	enum EDocking { NO_DOCKING_HORIZONTAL= 0, NO_DOCKING_VERTICAL, TOP_DOCKING, BOTTOM_DOCKING, LEFT_DOCKING, RIGHT_DOCKING };
	enum EClickWindowStatus { VISIBLE= 0, HIDDEN, DOCKED };

    CClickWindowController(CViacamController & pViacamController);
	~CClickWindowController();

	void Finalize ();

    void Show (bool show);
	const bool IsShown () const;

    //Return to default state
    void Reset();

    // Get the next action that should be sent. Called by the mouse controller.
    EAction GetAction(long x, long y);
	
    // Notifies click bar that the click action has to be sent
    // and where. Updates internal state. Called by the mouse controller.
    void ActionDone(long x, long y);

	inline const EDesign GetDesign();
	void SetDesign(EDesign design);

	inline const bool GetFastMode() const;
    void SetFastMode(bool enable);
    
    inline const CClickWindowController::EDocking GetDockingMode() const;
    void SetDockingMode(CClickWindowController::EDocking dockingMode);

	inline const EButton GetCurrentButton () const;
	inline const EButton GetLockedButton () const;

	inline const bool GetEnabled () const;
	
	inline const bool GetAutohide() const;
	void SetAutohide(bool enable);
	void AutohideClickWindow(long x, long y);

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

	// Associations
    CClickWindow * m_pWindow;
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
	CClickWindowController::EDocking m_dockingMode;
	bool m_autohide;
	CClickWindowController::EClickWindowStatus m_status;
};

inline const CClickWindowController::EDesign CClickWindowController::GetDesign()
{
	return m_design;
}

inline const bool CClickWindowController::GetFastMode() const
{
	return m_fastMode;
}

inline const CClickWindowController::EDocking CClickWindowController::GetDockingMode() const
{
	return m_dockingMode;
}

inline const CClickWindowController::EButton CClickWindowController::GetCurrentButton () const
{
	return m_currentButton;
}

inline const CClickWindowController::EButton CClickWindowController::GetLockedButton () const
{
	return m_lockedButton;
}

inline const bool CClickWindowController::GetEnabled () const
{
	return  m_enabled;
}

inline const bool CClickWindowController::GetAutohide() const
{
	return m_autohide;
}


#endif
