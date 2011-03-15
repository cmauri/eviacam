/////////////////////////////////////////////////////////////////////////////
// Name:        wmotioncalibrationx.h
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

#ifndef _WMOTIONCALIBRATIONX_H_
#define _WMOTIONCALIBRATIONX_H_


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
#define ID_WMOTIONCALIBRATIONX 10075
#define SYMBOL_WMOTIONCALIBRATIONX_STYLE wxCAPTION|wxSTAY_ON_TOP|wxTAB_TRAVERSAL
#define SYMBOL_WMOTIONCALIBRATIONX_TITLE _("Motion Calibration X")
#define SYMBOL_WMOTIONCALIBRATIONX_IDNAME ID_WMOTIONCALIBRATIONX
#define SYMBOL_WMOTIONCALIBRATIONX_SIZE wxSize(400, 300)
#define SYMBOL_WMOTIONCALIBRATIONX_POSITION wxDefaultPosition
////@end control identifiers


/*!
 * WMotionCalibrationX class declaration
 */

class WMotionCalibrationX: public wxDialog
{    
    DECLARE_DYNAMIC_CLASS( WMotionCalibrationX )
    DECLARE_EVENT_TABLE()

public:
    /// Constructors
    WMotionCalibrationX();
    WMotionCalibrationX( wxWindow* parent, wxWindowID id = SYMBOL_WMOTIONCALIBRATIONX_IDNAME, const wxString& caption = SYMBOL_WMOTIONCALIBRATIONX_TITLE, const wxPoint& pos = SYMBOL_WMOTIONCALIBRATIONX_POSITION, const wxSize& size = SYMBOL_WMOTIONCALIBRATIONX_SIZE, long style = SYMBOL_WMOTIONCALIBRATIONX_STYLE );

    /// Creation
    bool Create( wxWindow* parent, wxWindowID id = SYMBOL_WMOTIONCALIBRATIONX_IDNAME, const wxString& caption = SYMBOL_WMOTIONCALIBRATIONX_TITLE, const wxPoint& pos = SYMBOL_WMOTIONCALIBRATIONX_POSITION, const wxSize& size = SYMBOL_WMOTIONCALIBRATIONX_SIZE, long style = SYMBOL_WMOTIONCALIBRATIONX_STYLE );

    /// Destructor
    ~WMotionCalibrationX();

    /// Initialises member variables
    void Init();

    /// Creates the controls and sizers
    void CreateControls();

////@begin WMotionCalibrationX event handler declarations

////@end WMotionCalibrationX event handler declarations
	void OnFaceNotDetected( wxCommandEvent& event );
	void OnFaceDetected( wxCommandEvent& event );
	
////@begin WMotionCalibrationX member function declarations

    /// Retrieves bitmap resources
    wxBitmap GetBitmapResource( const wxString& name );

    /// Retrieves icon resources
    wxIcon GetIconResource( const wxString& name );
////@end WMotionCalibrationX member function declarations
	void SetFaceDetected( bool isFaceDetected );

    /// Should we show tooltips?
    static bool ShowToolTips();

////@begin WMotionCalibrationX member variables
    wxStaticText* m_staticText;
    wxStaticText* m_staticTextFaceNotDetected;
////@end WMotionCalibrationX member variables
};

#endif
    // _WMOTIONCALIBRATIONX_H_
