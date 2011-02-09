/////////////////////////////////////////////////////////////////////////////
// Name:        wmotioncalibrationy.h
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

#ifndef _WMOTIONCALIBRATIONY_H_
#define _WMOTIONCALIBRATIONY_H_


/*!
 * Includes
 */
#include "wx/wxprec.h"
#include "wx/wx.h"
#include "wx/object.h"
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
#define ID_WMOTIONCALIBRATIONY 10076
#define SYMBOL_WMOTIONCALIBRATIONY_STYLE wxCAPTION|wxSTAY_ON_TOP|wxTAB_TRAVERSAL
#define SYMBOL_WMOTIONCALIBRATIONY_TITLE _("Motion Calibration Y")
#define SYMBOL_WMOTIONCALIBRATIONY_IDNAME ID_WMOTIONCALIBRATIONY
#define SYMBOL_WMOTIONCALIBRATIONY_SIZE wxSize(400, 300)
#define SYMBOL_WMOTIONCALIBRATIONY_POSITION wxDefaultPosition
////@end control identifiers


/*!
 * WMotionCalibrationY class declaration
 */

class WMotionCalibrationY: public wxDialog
{    
    DECLARE_DYNAMIC_CLASS( WMotionCalibrationY )
    DECLARE_EVENT_TABLE()

public:
    /// Constructors
    WMotionCalibrationY();
    WMotionCalibrationY( wxWindow* parent, wxWindowID id = SYMBOL_WMOTIONCALIBRATIONY_IDNAME, const wxString& caption = SYMBOL_WMOTIONCALIBRATIONY_TITLE, const wxPoint& pos = SYMBOL_WMOTIONCALIBRATIONY_POSITION, const wxSize& size = SYMBOL_WMOTIONCALIBRATIONY_SIZE, long style = SYMBOL_WMOTIONCALIBRATIONY_STYLE );

    /// Creation
    bool Create( wxWindow* parent, wxWindowID id = SYMBOL_WMOTIONCALIBRATIONY_IDNAME, const wxString& caption = SYMBOL_WMOTIONCALIBRATIONY_TITLE, const wxPoint& pos = SYMBOL_WMOTIONCALIBRATIONY_POSITION, const wxSize& size = SYMBOL_WMOTIONCALIBRATIONY_SIZE, long style = SYMBOL_WMOTIONCALIBRATIONY_STYLE );

    /// Destructor
    ~WMotionCalibrationY();

    /// Initialises member variables
    void Init();

    /// Creates the controls and sizers
    void CreateControls();

////@begin WMotionCalibrationY event handler declarations

////@end WMotionCalibrationY event handler declarations
	void OnFaceNotDetected( wxCommandEvent& event );
	void OnFaceDetected( wxCommandEvent& event );

////@begin WMotionCalibrationY member function declarations

    /// Retrieves bitmap resources
    wxBitmap GetBitmapResource( const wxString& name );

    /// Retrieves icon resources
    wxIcon GetIconResource( const wxString& name );
////@end WMotionCalibrationY member function declarations
	void SetFaceDetected( bool isFaceDetected );
	
    /// Should we show tooltips?
    static bool ShowToolTips();

////@begin WMotionCalibrationY member variables
    wxStaticText* m_staticText;
    wxStaticText* m_staticTextFaceNotDetected;
////@end WMotionCalibrationY member variables
};

#endif
    // _WMOTIONCALIBRATIONY_H_
