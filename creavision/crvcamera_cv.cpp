/////////////////////////////////////////////////////////////////////////////
// Name:        crvcamera_cv.cpp
// Purpose:  
// Author:      Cesar Mauri Loba (cesar at crea-si dot com)
// Modified by: 
// Created:     30/05/2008
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
#include "crvcamera_cv.h"
#include <highgui.h>
#include <sys/timeb.h>
#include <sys/types.h>
#include <stdio.h>
#include <cassert>

bool CCameraCV::g_cvInitialized= false;
int CCameraCV::g_numDevices= 0;
char CCameraCV::g_deviceNames[MAX_CV_DEVICES][50];

#if defined(WIN32)
#include <windows.h>

#define MAX_KEY_LENGTH 255
#define MAX_VALUE_NAME 16383

// Workround to enable capture at 30fps for some camera models
// Should be called with administrative rights
// Return 0 if ok, -1 if permission denied
int VfwCamFpsWorkaround ()
{
	HKEY hKey;    
	TCHAR    achKey[MAX_KEY_LENGTH];   // buffer for subkey name
    DWORD    cbName;                   // size of name string 
    TCHAR    achClass[MAX_PATH] = TEXT("");  // buffer for class name 
    DWORD    cchClassName = MAX_PATH;  // size of class string 
    DWORD    cSubKeys=0;               // number of subkeys 
    DWORD    cbMaxSubKey;              // longest subkey size 
    DWORD    cchMaxClass;              // longest class string 
    DWORD    cValues;              // number of values for key 
    DWORD    cchMaxValue;          // longest value name 
    DWORD    cbMaxValueData;       // longest value data 
    DWORD    cbSecurityDescriptor; // size of security descriptor 
    FILETIME ftLastWriteTime;      // last write time 
	DWORD i, retCode;   
   // DWORD cchValue = MAX_VALUE_NAME; 

	// Open key
	retCode= RegOpenKeyEx(HKEY_LOCAL_MACHINE, TEXT("SOFTWARE\\Microsoft\\VfWWDM Mapper"), 
							0, KEY_ALL_ACCESS, &hKey);
	if (retCode!= ERROR_SUCCESS) return -1;

	// Get the class name and the value count. 
    retCode = RegQueryInfoKey(
        hKey,                    // key handle 
        achClass,                // buffer for class name 
        &cchClassName,           // size of class string 
        NULL,                    // reserved 
        &cSubKeys,               // number of subkeys 
        &cbMaxSubKey,            // longest subkey size 
        &cchMaxClass,            // longest class string 
        &cValues,                // number of values for this key 
        &cchMaxValue,            // longest value name 
        &cbMaxValueData,         // longest value data 
        &cbSecurityDescriptor,   // security descriptor 
        &ftLastWriteTime);       // last write time 
 
	// Enumerate the subkeys
	if (cSubKeys) {
		for (i=0; i<cSubKeys; i++) { 
			cbName = MAX_KEY_LENGTH;
			retCode = RegEnumKeyEx(hKey, i,	achKey, &cbName, NULL, 
									NULL, NULL, &ftLastWriteTime); 

			if (retCode == ERROR_SUCCESS) {
				HKEY hKeyValue;

				retCode= RegOpenKeyEx(hKey, achKey, 0, KEY_WRITE, &hKeyValue);
				if (retCode== ERROR_SUCCESS) {
					DWORD value= 333330;
					retCode= RegSetValueEx(hKeyValue, TEXT("AvgTimePerFrame"), 0, REG_DWORD, 
											(const BYTE *) &value, sizeof(DWORD));
				}
				RegCloseKey(hKeyValue);
			}
		}
	} 

	// Close key
	RegCloseKey(hKey);

	return 0;
}

#endif

CCameraCV::CCameraCV(int cameraId, unsigned int width, int unsigned height, 
		float fr) throw (camera_exception)
{
	if (cameraId>= GetNumDevices()) throw camera_exception("wrong camera id");
	m_Id= cameraId;
	m_Width= width;
	m_Height= height;
	m_FrameRate= fr;
	m_pCvCapture= NULL;

#if defined(WIN32)
	VfwCamFpsWorkaround ();
#endif
}

CCameraCV::~CCameraCV(void)
{
	Close ();	
}

bool CCameraCV::DoOpen ()
{
	if (m_pCvCapture!= NULL) return true;	// Already opened
	m_pCvCapture= cvCaptureFromCAM (m_Id);
	if (m_pCvCapture== NULL) return false;
	
	// Try to set capture parameters although not always works
	cvSetCaptureProperty( m_pCvCapture, CV_CAP_PROP_FRAME_WIDTH, (double) m_Width );
	cvSetCaptureProperty( m_pCvCapture, CV_CAP_PROP_FRAME_HEIGHT, (double) m_Height );
	// The following line does nothing under MS Windows
	cvSetCaptureProperty( m_pCvCapture, CV_CAP_PROP_FPS, (double) m_FrameRate );

	return true;
}

void CCameraCV::DoClose ()
{
	if (m_pCvCapture== NULL) return;	// Already closed
	cvReleaseCapture (&m_pCvCapture);
	m_pCvCapture= NULL;
}

IplImage *CCameraCV::DoQueryFrame()
{
	assert (m_pCvCapture);
	if (m_pCvCapture== NULL) return NULL;

	IplImage *pImage= cvQueryFrame( m_pCvCapture );
	assert (pImage);
	if (pImage== NULL) return NULL;
	
#if defined(linux)
	// It seems that under Linux no proper channelSeq is reported
	// Tested with Logitech Quickcam pro 4000 
	pImage->channelSeq[0]= 'B';
	pImage->channelSeq[2]= 'R';
#endif

	return pImage;
}

int CCameraCV::GetNumDevices()
{
	if (!g_cvInitialized) {		
		cvInitSystem (0, NULL); 
		g_cvInitialized= true; 

		int i;
		CvCapture* tmpCapture;

		// Detect number of connected devices
		for (i= 0; i< MAX_CV_DEVICES; ++i) {
			tmpCapture= cvCreateCameraCapture (i);
			if (tmpCapture== NULL) break;

			cvReleaseCapture (&tmpCapture);

			// Generate device name
			sprintf (g_deviceNames[i], "Camera (Id:%d)", i);
		}		
		g_numDevices= i;
	}
	return g_numDevices;
}

char* CCameraCV::GetDeviceName (unsigned int id)
{
	if ((int) id>= GetNumDevices()) return NULL;	
	return g_deviceNames[id];
}
