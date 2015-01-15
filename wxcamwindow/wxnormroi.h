/////////////////////////////////////////////////////////////////////////////
// Name:        wxnormroi.h
// Purpose:  This class is used to display and interact with ROI objects
//           it's used to link between window object and CVisibleNormROI
//           object and to properly handle mouse events. 
//           Only used inside this package.
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

#ifndef WXNORMROI_H
#define WXNORMROI_H

#include <wx/event.h>

class CVisibleNormROI;
class wxWindow;
class CIplImage;

class CWXNormROI
{
public:
	CWXNormROI(CVisibleNormROI* pVisibleNormROI, wxWindow* pWindow);
	virtual ~CWXNormROI(void);

	CVisibleNormROI* GetVisibleNormROI () { return m_pVisibleNormROI; }
	
	// Returns true if event was processed
	bool OnEvtMotion ( wxMouseEvent& event, wxPoint prevCursor );
	bool OnEvtLeftDClick ( wxMouseEvent& event );	
	void OnPaint (CIplImage *pImg);
private:
	CVisibleNormROI* m_pVisibleNormROI;
	wxWindow* m_pWindow;
};

#endif
