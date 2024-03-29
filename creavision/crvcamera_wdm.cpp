/////////////////////////////////////////////////////////////////////////////
// Copyright:   (C) 2009-19 Cesar Mauri Loba - CREA Software Systems
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
#include "crvcamera_wdm.h"

#include <sys/timeb.h>
#include <sys/types.h>
#include <cassert>
#include <opencv2/opencv.hpp>
#include "crvimage.h"
#include "videoInput.h"
#include "simplelog.h"

CCameraWDM::CCameraWDM(int cameraId, unsigned int width, unsigned int height, float fr)
{
	if (cameraId>= GetNumDevices()) throw camera_exception("wrong camera id");
    
	m_Id= cameraId;
	m_Width= width;
	m_Height= height;
	m_FrameRate= fr;

	m_VI= NULL;	

	if (slog_get_priority()> SLOG_PRIO_CRIT)
		videoInput::setVerbose(true);
	else
		videoInput::setVerbose(false);
}

CCameraWDM::~CCameraWDM(void)
{
	Close();
}

bool CCameraWDM::DoOpen ()
{
	// Already open? Do nothing
	if (m_VI) return true;

	// Create a videoInput object
	m_VI= new videoInput;
	assert (m_VI);

	// BGR channel sequence assumed
	//m_pImage= new CIplImage (m_Width, m_Height, IPL_DEPTH_8U, "BGR");
	//assert (m_pImage);	

	m_VI->setIdealFramerate(m_Id, (int) m_FrameRate);
	if (!m_VI->setupDevice (m_Id, m_Width, m_Height)) {
		Close();
		return false;
	}

	return true;	
}

void CCameraWDM::DoClose ()
{
	if (m_VI) {
		m_VI->stopDevice(m_Id);
		delete m_VI;
		m_VI= NULL;
		//delete m_pImage;
	}
}

bool CCameraWDM::DoQueryFrame(cv::Mat &frame)
{
	if (!m_VI) return false;

	const int width = m_VI->getWidth(m_Id);
	const int height = m_VI->getHeight(m_Id);
	
	frame.create(height, width, CV_8UC3);

	m_VI->getPixels(m_Id, frame.data, false, false);

	return true;
}

void CCameraWDM::ShowSettingsDialog ()
{
	if (m_VI) m_VI->showSettingsWindow (m_Id);
}

int CCameraWDM::g_numDevices= -1;
char CCameraWDM::g_deviceNames[MAX_CAM_DEVICES][CAM_DEVICE_NAME_LENGTH];

int CCameraWDM::GetNumDevices() 
{ 
	// Win32: uses videoInput
	if (g_numDevices== -1) {		
		// First initialization
		g_numDevices= videoInput::listDevices (true);
		if (g_numDevices> MAX_CAM_DEVICES) g_numDevices= MAX_CAM_DEVICES;		
		for (int i= 0; i< g_numDevices; ++i) {
			// Prepend device number and append device name
			_snprintf (g_deviceNames[i], CAM_DEVICE_NAME_LENGTH, " (Id:%d) %s\0", i, videoInput::getDeviceName(i));
		}		
	}

	return g_numDevices;	
}

const char* CCameraWDM::GetDeviceName (unsigned int id)
{ 
	if ((int) id>= GetNumDevices()) return NULL;
	return g_deviceNames[id];
}
