/////////////////////////////////////////////////////////////////////////////
// Name:        wconfirmcalibration.h
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

#ifndef _WCONFIRMCALIBRATION_H_
#define _WCONFIRMCALIBRATION_H_


/*!
 * Includes
 */
#include <wx/timer.h>
////@begin includes
////@end includes

/*!
 * Forward declarations
 */

////@begin forward declarations
////@end forward declarations

/*!
 * Control identifiers
 */

////@begin control identifiers
#define ID_WCONFIRMCALIBRATION 10077
#define ID_BUTTON1 10078
#define ID_BUTTON2 10079
#define ID_BUTTON3 10080
#define SYMBOL_WCONFIRMCALIBRATION_STYLE wxCAPTION|wxSTAY_ON_TOP|wxTAB_TRAVERSAL
#define SYMBOL_WCONFIRMCALIBRATION_TITLE _("Confirm Calibration")
#define SYMBOL_WCONFIRMCALIBRATION_IDNAME ID_WCONFIRMCALIBRATION
#define SYMBOL_WCONFIRMCALIBRATION_SIZE wxSize(400, 300)
#define SYMBOL_WCONFIRMCALIBRATION_POSITION wxDefaultPosition
////@end control identifiers


/*!
 * WConfirmCalibration class declaration
 */

class WConfirmCalibration: public wxDialog
{    
    DECLARE_DYNAMIC_CLASS( WConfirmCalibration )
    DECLARE_EVENT_TABLE()

public:

	enum EButton { OK, CANCEL, REPEAT };

    /// Constructors
    WConfirmCalibration();
    WConfirmCalibration( wxWindow* parent, wxWindowID id = SYMBOL_WCONFIRMCALIBRATION_IDNAME, const wxString& caption = SYMBOL_WCONFIRMCALIBRATION_TITLE, const wxPoint& pos = SYMBOL_WCONFIRMCALIBRATION_POSITION, const wxSize& size = SYMBOL_WCONFIRMCALIBRATION_SIZE, long style = SYMBOL_WCONFIRMCALIBRATION_STYLE );

    /// Creation
    bool Create( wxWindow* parent, wxWindowID id = SYMBOL_WCONFIRMCALIBRATION_IDNAME, const wxString& caption = SYMBOL_WCONFIRMCALIBRATION_TITLE, const wxPoint& pos = SYMBOL_WCONFIRMCALIBRATION_POSITION, const wxSize& size = SYMBOL_WCONFIRMCALIBRATION_SIZE, long style = SYMBOL_WCONFIRMCALIBRATION_STYLE );

    /// Destructor
    ~WConfirmCalibration();

    /// Initialises member variables
    void Init();

    /// Creates the controls and sizers
    void CreateControls();

////@begin WConfirmCalibration event handler declarations

    /// wxEVT_MOTION event handler for ID_WCONFIRMCALIBRATION
    void OnWindowMotion( wxMouseEvent& event );

    /// wxEVT_LEAVE_WINDOW event handler for ID_WCONFIRMCALIBRATION
    void OnLeaveWindow( wxMouseEvent& event );

    /// wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_BUTTON1
    void OnButton1Click( wxCommandEvent& event );

    /// wxEVT_MOTION event handler for ID_BUTTON1
    void OnOkButtonMotion( wxMouseEvent& event );

    /// wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_BUTTON2
    void OnButton2Click( wxCommandEvent& event );

    /// wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_BUTTON3
    void OnButton3Click( wxCommandEvent& event );

    /// wxEVT_MOTION event handler for ID_BUTTON3
    void OnRepeatButtonMotion( wxMouseEvent& event );

////@end WConfirmCalibration event handler declarations

	void OnTimer(wxTimerEvent& event);

////@begin WConfirmCalibration member function declarations

    /// Retrieves bitmap resources
    wxBitmap GetBitmapResource( const wxString& name );

    /// Retrieves icon resources
    wxIcon GetIconResource( const wxString& name );
////@end WConfirmCalibration member function declarations

    /// Should we show tooltips?
    static bool ShowToolTips();

////@begin WConfirmCalibration member variables
    wxButton* m_buttonOk;
    wxButton* m_buttonCancel;
    wxButton* m_buttonRepeat;
////@end WConfirmCalibration member variables

private:
	wxTimer m_timer;
	int m_okCountdown;
	int m_cancelCountdown;
	int m_repeatCountdown;
	EButton m_activeButton;
};

#endif
    // _WCONFIRMCALIBRATION_H_
