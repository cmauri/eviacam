/////////////////////////////////////////////////////////////////////////////
// Name:        wxnormroi.cpp
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
#include "wxnormroi.h"
#include <wx/window.h>
#include "visiblenormroi.h"

CWXNormROI::CWXNormROI(CVisibleNormROI* pVisibleNormROI, wxWindow* pWindow)
{
	assert (pVisibleNormROI);
	m_pVisibleNormROI= pVisibleNormROI;

	assert (pWindow);
	m_pWindow= pWindow;
}

CWXNormROI::~CWXNormROI(void)
{
}

// Window events

bool CWXNormROI::OnEvtMotion ( wxMouseEvent& event, wxPoint prevCursor )
{
	CvSize winSize;
	bool processed;

	m_pWindow->GetClientSize (&winSize.width, &winSize.height);

	if (!event.LeftIsDown())
	{
		processed= true;

		int retval= m_pVisibleNormROI->OnMouseMovedNoClick (winSize, cvPoint (event.GetX(), event.GetY()));

		switch (retval)
		{
		case CVisibleNormROI::OVER_NONE:
			m_pWindow->SetCursor (wxNullCursor);
			processed= false;
			break;
		case CVisibleNormROI::OVER_LEFT_LINE:
		case CVisibleNormROI::OVER_RIGHT_LINE:
		{
			wxCursor cur (wxCURSOR_SIZEWE);
			m_pWindow->SetCursor (cur);
		}
			break;
		case CVisibleNormROI::OVER_BOTTOM_LINE:
		case CVisibleNormROI::OVER_UPPER_LINE:
		{
			wxCursor cur (wxCURSOR_SIZENS);
			m_pWindow->SetCursor (cur);
		}
			break;
		case CVisibleNormROI::OVER_ORIENTER:
		case CVisibleNormROI::OVER_UL_CORNER:
		case CVisibleNormROI::OVER_BR_CORNER:
		{
			wxCursor cur (wxCURSOR_HAND);
			m_pWindow->SetCursor (cur);
		}
			break;
		}
	}
	else
	{
		processed= m_pVisibleNormROI->OnMouseMovedClick (winSize, cvPoint (event.GetX(), event.GetY()), cvPoint (prevCursor.x, prevCursor.y));
	}

	return processed;	
}

bool CWXNormROI::OnEvtLeftDClick ( wxMouseEvent& WXUNUSED(event) )
{
	//m_pVisibleNormROI->SetCenterWindow (m_pWindow->GetClientSize(), event.GetX(), event.GetY());
	return true;
}

void CWXNormROI::OnPaint (CIplImage *pImg)
{
	CvSize winSize;
	m_pWindow->GetClientSize(&winSize.width, &winSize.height);
	m_pVisibleNormROI->OnPaint (winSize, pImg);
}

