/////////////////////////////////////////////////////////////////////////////
// Name:        viacamcontroller.cpp
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

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

#include "viacamcontroller.h"
#include "wviacam.h"
#include "crvcamera_cv.h"
#if defined(__WXMSW__)
#include "crvcamera_wdm.h"
#else
#include "crvcamera_v4l2.h"
#endif
#include "clickwindow.h"
#include "mouseoutput.h"
#include "camwindow.h"
#include "wconfiguration.h"


//#include "icons/eviacam_mini.xpm"

#include <wx/utils.h>
#include <wx/debug.h>

// Under wxGTK we should protect calls to GUI. Under Windows is not needed
#if defined(__WXGTK__) 
#include "activationkey.h"
#include "keyboardbitmapcheck.h"
#include "cautostart.h"
#include "wx/timer.h"
#include <wx/stdpaths.h>
#include <wx/thread.h>
#define BEGIN_GUI_CALL_MUTEX() if (!wxIsMainThread()) wxMutexGuiEnter();
#define END_GUI_CALL_MUTEX() if (!wxIsMainThread()) wxMutexGuiLeave();
#else
#define BEGIN_GUI_CALL_MUTEX()
#define END_GUI_CALL_MUTEX()
#endif // __WXGTK___

#define TIMER_ID 12345

IMPLEMENT_DYNAMIC_CLASS( CViacamController, wxDialog )

BEGIN_EVENT_TABLE( CViacamController, wxDialog )
        EVT_TIMER(TIMER_ID, CViacamController::OnTimer)
END_EVENT_TABLE()

        
CViacamController::CViacamController(void) : m_timer(this, TIMER_ID)
{
	m_pMainWindow = NULL;
	m_pCamera= NULL;
	m_pCaptureThread= NULL;
	m_pClickWindowController= NULL;
	m_pMouseOutput= NULL;
	m_enabled= false;
	m_locale= new wxLocale ();
	m_configManager= new CConfigManager(this);
	m_frameRate= 0;
#if defined(__WXGTK__) 
	m_pAutostart = new CAutostart(wxT("eviacam.desktop"));
#endif        
	InitDefaults();
}

CViacamController::~CViacamController(void)
{
	assert (!m_pCaptureThread && !m_pCamera && !m_pCaptureThread);
#if defined(__WXGTK__) 
	delete m_pAutostart;
#endif
	delete m_locale;
	delete m_configManager;
}

void CViacamController::SetUpLanguage ()
{
	assert (m_locale);
	
	m_languageId= m_configManager->ReadLanguage ();

	m_locale->AddCatalogLookupPathPrefix(wxT("."));
	m_locale->Init(m_languageId, wxLOCALE_CONV_ENCODING);
	m_locale->AddCatalog(wxT("eviacam"));
}

void CViacamController::SetLanguage (const int id)
{
	// Simply store new language id
	if (id!= m_languageId)
	{
		// Check if valid
		const wxLanguageInfo *info= wxLocale::GetLanguageInfo (id);
		if (info)
		{
			m_languageId= id;
		}
		else
		{
			assert (false);
		}
	}
}

#if defined(WIN32)
#define CAM_CLASS CCameraWDM
#else 
#define CAM_CLASS CCameraCV
#endif

CCamera* CViacamController::SetUpCamera()
{
	CCamera* cam;
	int numDevices;
	int camId= -1;

	// Load app local data
	ReadAppData(wxConfigBase::Get());

	numDevices= CAM_CLASS::GetNumDevices ();
	if (numDevices== 0)
	{
		wxMessageDialog errorMsg (NULL, _("Not detected any camera. Aborting"), _T("Enable Viacam"), wxOK | wxICON_ERROR);
		errorMsg.ShowModal();

		return NULL;
	}	
	
	// Try to find previously used camera
	if (m_cameraName.Length()> 0)
	{
		for (camId= 0; camId< numDevices; camId++)
			if (wxString(CAM_CLASS::GetDeviceName (camId), wxConvLibc)== m_cameraName) break;			
		if (camId== numDevices) camId= -1;	// Not found
	}

	// Show selection dialog when needed
	if (camId== -1)
	{
		wxArrayString strArray;

		for (camId= 0; camId< numDevices; camId++)
			strArray.Add (wxString(CAM_CLASS::GetDeviceName (camId), wxConvLibc));

		wxSingleChoiceDialog choiceDlg(NULL, _("Choose the camera to use"), _T("Enable Viacam"), strArray, 
							NULL, wxDEFAULT_DIALOG_STYLE | wxOK | wxCANCEL | wxCENTRE);

		if (choiceDlg.ShowModal ()!= wxID_OK) return NULL;

		camId= choiceDlg.GetSelection();
		m_cameraName= choiceDlg.GetStringSelection ();
	}

	cam= new CAM_CLASS(camId, 320, 240);
	assert (cam);
	cam->SetHorizontalFlip (true);

	// Try to open the camera to ensure it works
	if (!cam->Open ())
	{
		wxMessageDialog errorMsg (NULL, _("Can not initialize the camera.\nPerhaps is being used by other application."), _T("Enable Viacam"), wxOK | wxICON_ERROR);
		errorMsg.ShowModal();
		delete cam;
		cam= NULL;
		ChangeCamera();

	}
	else
		cam->Close();
		

	WriteAppData(wxConfigBase::Get());

	return cam;
}

bool CViacamController::Initialize ()
{	
	bool retval= true;
	assert (!m_pMainWindow && !m_pCamera && !m_pCaptureThread);

	SetUpLanguage ();
        //m_pKeyboardBitmapCheck = new CKeyboardBitmapCheck();

	// Create camera object
	m_pCamera= SetUpCamera();	
	if (m_pCamera== NULL) retval= false;

	// Create main window
	if (retval)
	{
		m_pMainWindow = new WViacam( NULL, ID_WVIACAM );
		assert (m_pMainWindow);
		m_pMainWindow->SetController (this);
		m_pMainWindow->Show (true);	
	}

	// Create click window controller
	if (retval)
	{
		m_pClickWindowController= new CClickWindowController (*this);
		assert (m_pClickWindowController);
	}
	
	// Create mouse controller
	if (retval)
	{
		m_pMouseOutput= new CMouseOutput(*m_pClickWindowController);
		assert (m_pMouseOutput);
	}
		
	// Create and start worker thread
	if (retval)
	{
		m_pCaptureThread = new CCaptureThread (m_pCamera, m_pMainWindow->GetCamWindow(), this, wxTHREAD_JOINABLE );	
		assert (m_pCaptureThread);
		if (m_pCaptureThread->Create()!= wxTHREAD_NO_ERROR) retval= false;
		if (retval)
		{
			// Fire thread
			m_pCaptureThread->SetPriority (WXTHREAD_MAX_PRIORITY);
			if (m_pCaptureThread->Run() != wxTHREAD_NO_ERROR) retval= false;
		}
	}

	// Register track area
	if (retval)
		m_pMainWindow->GetCamWindow()->RegisterControl (m_motionTracker.GetTrackAreaControl());
	

	// Load configuration
	if (retval) m_configManager->ReadAll (true);

	return retval;
}

void CViacamController::Finalize ()
{
	SetEnabled (false, true);

	if (m_pCaptureThread)
	{
		// Save config
		m_configManager->WriteAll();

		m_pCaptureThread->Finish();
		m_pCaptureThread->Wait();
		delete m_pCaptureThread;
		m_pCaptureThread= NULL;
	}	
	if (m_pCamera)
	{
		delete m_pCamera;
		m_pCamera= NULL;
	}

	if (m_pMouseOutput)
	{
		delete m_pMouseOutput;
		m_pMouseOutput= NULL;
	}	

	if (m_pClickWindowController)
	{
		m_pClickWindowController->Finalize();
		delete m_pClickWindowController;
		m_pClickWindowController= NULL;
	}
	if (m_pMainWindow)
	{
		m_pMainWindow->GetCamWindow()->UnregisterControl (m_motionTracker.GetTrackAreaControl());
		// Main window is self-destroyed
		m_pMainWindow= NULL;
	}	
}

void CViacamController::InitDefaults()
{
	m_languageId= wxLANGUAGE_DEFAULT;
	m_enabledAtStartup= false;	
#if defined(__WXMSW__)  
	m_onScreenKeyboardCommand= _T("osk.exe");
#endif
}

void CViacamController::WriteAppData(wxConfigBase* pConfObj)
{
	// General options
	//pConfObj->Write(_T("onScreenKeyboardCommand"), m_onScreenKeyboardCommand);
	m_configManager->WriteLanguage (m_languageId);
	pConfObj->Write(_T("cameraName"), m_cameraName);
}

void CViacamController::WriteProfileData(wxConfigBase* pConfObj)
{
	pConfObj->Write(_T("enabledAtStartup"), m_enabledAtStartup);	
	pConfObj->Write(_T("onScreenKeyboardCommand"), m_onScreenKeyboardCommand);

#if defined(__WXGTK__) 
	pConfObj->Write(_T("enabledActivationKey"), m_enabledActivationKey);
	pConfObj->Write(_T("keySym"), (int)m_keySym);
#endif

	// Propagates calls
	m_pMouseOutput->WriteProfileData (pConfObj);
	m_pClickWindowController->WriteProfileData (pConfObj);
	m_motionTracker.WriteProfileData (pConfObj);	
} 

void CViacamController::ReadAppData(wxConfigBase* pConfObj)
{
	// General options
	//pConfObj->Read(_T("onScreenKeyboardCommand"), &m_onScreenKeyboardCommand);
	SetLanguage (m_configManager->ReadLanguage());	// Only load, dont't apply
	pConfObj->Read(_T("cameraName"), &m_cameraName);
}

void CViacamController::ReadProfileData(wxConfigBase* pConfObj)
{
	pConfObj->Read(_T("enabledAtStartup"), &m_enabledAtStartup);
	pConfObj->Read(_T("onScreenKeyboardCommand"), &m_onScreenKeyboardCommand);
#if defined(__WXGTK__) 
	int keyCode = 0;
	pConfObj->Read(_T("enabledActivationKey"), &m_enabledActivationKey);
	pConfObj->Read(_T("keySym"), &keyCode);
        
	m_keySym = (KeySym) keyCode;
        
	// Set default activation key (JORDI: compte amb els números "màgics", defineix una constant)
	if (m_keySym == 0) {
		m_keySym = 65300;
	}
#endif
	// Propagates calls
	m_pMouseOutput->ReadProfileData (pConfObj);
	m_pClickWindowController->ReadProfileData (pConfObj);
	m_motionTracker.ReadProfileData (pConfObj);	
}

void CViacamController::StartupRun()
{
	if (m_enabledAtStartup) SetEnabled (true);
	GetClickWindowController()->StartupRun();	
}


void CViacamController::SetEnabled (bool value, bool silent)
{
	if (value!= m_enabled)
	{
		if (value== false && !silent)
		{
			wxMessageDialog dlg (NULL, _("This action will disable eViacam.\nAre you sure?"), _("eViacam warning"), wxICON_EXCLAMATION | wxYES_NO );
			if (dlg.ShowModal()== wxID_YES)
			{
				m_enabled= value;
				m_pMouseOutput->SetEnabled (m_enabled);
			}
		}
		else
		{
			m_enabled= value;
			m_pMouseOutput->SetEnabled (m_enabled);
		}
	}
}

void CViacamController::OpenConfiguration()
{   
#if defined(__WXGTK__) 
    WConfiguration* window = new WConfiguration(m_pMainWindow, this, m_pAutostart);
#else
	WConfiguration* window = new WConfiguration(m_pMainWindow, this);
#endif

    int returnValue = window->ShowModal();
	window->Destroy();
	if (returnValue== ID_BUTTON_OK)
		// Save changes
		m_configManager->WriteAll();
	else
		// Discard changes
		m_configManager->ReadAll();		
}

void CViacamController::OpenOnScreenKeyboard()
{
	if (m_onScreenKeyboardCommand.IsEmpty())
	{
		wxMessageDialog dlg (NULL, _("On-screen keyboard command not set.\nPlease, configure it through options dialog."), _("eViacam error"), wxOK | wxICON_ERROR);
		dlg.ShowModal();
	}
	else
	{
		if (wxExecute (m_onScreenKeyboardCommand)== 0)
		{
			wxString errMsg;


			errMsg.Printf (_("Can't execute on-screen keyboard program.\nCommand %s failed.\nPlease, configure it through options dialog."), m_onScreenKeyboardCommand.c_str());
#if defined(__WXMSW__)
			errMsg.Append (_("\n\nOn Windows Vista/7 several problems have been reported\nwith the system's default on-screen keyboard (osk). These\nissues are under investigation. Meanwhile you might want\nto disable User Account Control (UAC) to avoid such problems\nor try another osk, e.g. Click-N-Type."));
#endif
			wxMessageDialog dlg (NULL, errMsg, _("eViacam error"), wxOK | wxICON_ERROR);
			dlg.ShowModal();
		}
	}
}

void CViacamController::OnTimer(wxTimerEvent& event)
{
    bool isEnabled = GetEnabled();
    //m_pConfiguration = new WConfiguration(m_pMainWindow, this);
#if defined(__WXGTK__)         
    KeySym keyCode = 0;
    keyCode = CKeyboardBitmapCheck::ReadKeySym();
    
    if (keyCode != 0) {
        if (keyCode != 65307) {
            m_lastKeySym = keyCode;
            m_keySym = keyCode;
            SetEnabled(isEnabled,isEnabled);
            //m_pConfiguration->SetActivationKey(GetActivationKeyName());            
        }
        CloseActivationKey();
    }
#endif
}

void CViacamController::OpenActivationKey()
{
#if defined(__WXGTK__)
    m_window = new Activationkey(m_pMainWindow);
    m_window->Show();
    m_timer.Start (33);
#endif
}

void CViacamController::CloseActivationKey()
{
#if defined(__WXGTK__)
    m_window->Destroy();
    m_timer.Stop();
#endif
}

void CViacamController::ProcessImage (IplImage *pImage)
{
	WViacam::EFPSCondition cond;
	long iFrameRate;
	float vx, vy;
	CIplImage image(pImage);

	// Refresh fps meter
	m_frameRate= 0.5f * m_pCamera->GetRealFrameRate() + 0.5f * m_frameRate;
	iFrameRate= (long) (m_frameRate + 0.5f);	
	if (iFrameRate< 20) 
		cond= WViacam::POOR;
	else if (iFrameRate< 25)
		cond= WViacam::FAIR;
	else 
		cond= WViacam::GOOD;
	m_pMainWindow->SetFPS (iFrameRate, cond);

	// Proces frame
	m_motionTracker.ProcessImage (image, vx, vy);

	// Send mouse motion
	BEGIN_GUI_CALL_MUTEX()
	m_pMouseOutput->ProcessRelativePointerMove (-vx, vy);
	END_GUI_CALL_MUTEX()
                
#if defined(__WXGTK__) 
        //Read keyboard        
        if (m_enabledActivationKey) {
            KeySym keyCode = CKeyboardBitmapCheck::ReadKeySym();
            if (keyCode == m_keySym and keyCode != m_lastKeySym) {
                SetEnabled(!m_pMouseOutput->GetEnabled(),true);
            }
            m_lastKeySym = keyCode;
        }
#endif // __WXGTK___
}

