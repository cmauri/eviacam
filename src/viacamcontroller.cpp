/////////////////////////////////////////////////////////////////////////////
// Name:        viacamcontroller.cpp
// Purpose:  
// Author:      Cesar Mauri Loba (cesar at crea-si dot com)
// Modified by: 
// Created:     
// Copyright:   (C) 2008-11 Cesar Mauri Loba - CREA Software Systems
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
#include "viacamcontroller.h"

#include "wviacam.h"
#include "crvcamera_enum.h"
#include "crvcamera.h"
#include "pointeraction.h"
#include "camwindow.h"
#include "wconfiguration.h"
#include "cmotioncalibration.h"
#include "wcameradialog.h"
#include "eviacamdefs.h"
#include "configmanager.h"
#include "cautostart.h"
#include "hotkeymanager.h"

#include <wx/msgdlg.h>
#include <wx/choicdlg.h>

CViacamController::CViacamController(void)
: m_pMainWindow(NULL)
, m_pCamera(NULL)
, m_pCaptureThread(NULL)
, m_pointerAction(NULL)
, m_visionPipeline()
, m_hotKeyManager(NULL)
, m_configManager(NULL)
, m_locale(NULL)
, m_pAutostart(NULL)
, m_pConfiguration(NULL)
, m_pMotionCalibration(NULL)
, m_wizardManager()
, m_pCameraDialog(NULL)
, m_wConfigurationListener(*this)
, m_wCameraDialogListener(*this)
, m_cameraName()
, m_enabled(false)
, m_enabledAtStartup(false)
, m_languageId(wxLANGUAGE_DEFAULT)
, m_onScreenKeyboardCommand()
, m_frameRate(0)
, m_motionCalibrationEnabled(false)
, m_runWizardAtStartup(false)
{
	m_locale= new wxLocale ();
	m_configManager= new CConfigManager(this);	

	InitDefaults();
}

void CViacamController::InitDefaults()
{
	m_runWizardAtStartup= true;
	m_languageId= wxLANGUAGE_DEFAULT;
	m_enabledAtStartup= false;	
#if defined(__WXMSW__)
	m_onScreenKeyboardCommand= _T("osk.exe");
#endif
}


CViacamController::~CViacamController(void)
{
	assert (!m_pCaptureThread && !m_pCamera && !m_pCaptureThread);
	delete m_pointerAction;
#if defined(__WXGTK__) 
	delete m_pAutostart;
#endif
	delete m_locale;
	delete m_configManager;
	delete m_pMotionCalibration;
	delete m_hotKeyManager;
}

void CViacamController::SetUpLanguage ()
{
	assert (m_locale);
	
	m_languageId= m_configManager->ReadLanguage ();

	m_locale->AddCatalogLookupPathPrefix(wxT("."));
	if (!m_locale->Init(m_languageId, wxLOCALE_CONV_ENCODING))
		printf ("Cannot load locale. Switching to default locale.\n");
	m_locale->AddCatalog(wxT("wxstd"));
	m_locale->AddCatalog(wxT("eviacam"));
}

void CViacamController::SetLanguage (const int id)
{
	// Simply store new language id
	if (id!= m_languageId) {
		// Check if valid
		const wxLanguageInfo *info= wxLocale::GetLanguageInfo (id);
		if (info) {
			m_languageId= id;
		}
		else {
			assert (false);
		}
	}
}

CCamera* CViacamController::SetUpCamera()
{
	CCamera* cam;
	int numDevices;
	int camId= -1;

	// Load app local data
	ReadAppData(wxConfigBase::Get());

	numDevices= CCameraEnum::GetNumDevices ();
	if (numDevices== 0) {
		wxMessageDialog errorMsg (NULL, _("Not detected any camera. Aborting"), _T("Enable Viacam"), wxOK | wxICON_ERROR);
		errorMsg.ShowModal();

		return NULL;
	}	
	
	// Try to find previously used camera
	if (m_cameraName.Length()> 0) {
		for (camId= 0; camId< numDevices; camId++)
			if (wxString(CCameraEnum::GetDeviceName (camId), wxConvLibc)== m_cameraName) break;			
		if (camId== numDevices) camId= -1;	// Not found
	}

	// Show selection dialog when needed
	if (camId== -1) {
		if(numDevices > 1) {
			wxArrayString strArray;

			for (camId= 0; camId< numDevices; camId++)
				strArray.Add (wxString(CCameraEnum::GetDeviceName (camId), wxConvLibc));

			wxSingleChoiceDialog choiceDlg(NULL, _("Choose the camera to use"), _T("Enable Viacam"), strArray, 
								NULL, wxDEFAULT_DIALOG_STYLE | wxOK | wxCANCEL | wxCENTRE);

			if (choiceDlg.ShowModal ()!= wxID_OK) return NULL;

			camId= choiceDlg.GetSelection();
			m_cameraName= choiceDlg.GetStringSelection ();
		} 
		else {
			camId= 0;
			m_cameraName= wxString(CCameraEnum::GetDeviceName (camId), wxConvLibc);
		}
	}

	cam= CCameraEnum::GetCamera(camId);
	if (!cam) return NULL;
	cam->SetHorizontalFlip (true);

	// Try to open the camera to ensure it works
	if (!cam->Open ()) {
		wxMessageDialog errorMsg (NULL, _("Can not initialize the camera.\nPerhaps is being used by other application."), _T("Enable Viacam"), wxOK | wxICON_ERROR);
		errorMsg.ShowModal();
		delete cam;
		cam= NULL;
		ChangeCamera();

	}
	else
		cam->Close();
	
	WriteAppData(wxConfigBase::Get());
	wxConfigBase::Get()->Flush();

	return cam;
}

bool CViacamController::Initialize ()
{	
	bool retval= true;
	assert (!m_pMainWindow && !m_pCamera && !m_pCaptureThread);

	SetUpLanguage ();

	// Create camera object
	m_pCamera= SetUpCamera();	
	if (m_pCamera== NULL) retval= false;

	if (retval) {
		// TODO: check this!!!
		assert (!m_pMotionCalibration);
		m_pMotionCalibration= new CMotionCalibration(); //this);	
#if defined(__WXGTK__) 
		assert (!m_pAutostart);
		m_pAutostart = new CAutostart(wxT("eviacam.desktop"));
#endif
	}
	
	// Create main window
	if (retval) {
		m_pMainWindow = new WViacam( NULL, ID_WVIACAM );
		assert (m_pMainWindow);
		m_pMainWindow->Show (true);	
	}

	// Create hotkey manager
	if (retval) {
		assert (!m_hotKeyManager);
		m_hotKeyManager= new CHotkeyManager();
	}

	// Create pointer action object
	if (retval) {
		assert (!m_pointerAction);
		m_pointerAction= new CPointerAction();
	}
		
	// Create and start worker thread
	if (retval) {
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
		m_pMainWindow->GetCamWindow()->RegisterControl (m_visionPipeline.GetTrackAreaControl());

	// Load configuration
	if (retval) m_configManager->ReadAll (true);
	
	// Enable pointeraction object
	if (retval) m_pointerAction->SetEnabled(true);
	
	// Run the wizard at startup
	if (retval && m_runWizardAtStartup)
		StartWizard();
	
	return retval;
}

void CViacamController::Finalize ()
{
	SetEnabled (false, true);

	if (m_pCaptureThread) {
		// Save config
		m_configManager->WriteAll();

		m_pCaptureThread->Finish();
		m_pCaptureThread->Wait();
		delete m_pCaptureThread;
		m_pCaptureThread= NULL;
	}
	if (m_pCamera) {
		delete m_pCamera;
		m_pCamera= NULL;
	}
	
	if (m_pointerAction) {
		m_pointerAction->SetEnabled(false);
		delete m_pointerAction;
		m_pointerAction= NULL;
	}

	if (m_hotKeyManager) {
		delete m_hotKeyManager;
		m_hotKeyManager= NULL;		
	}	

	if (m_pMainWindow) {
		WViacam* mainWin= m_pMainWindow;
		m_pMainWindow= NULL;
		mainWin->GetCamWindow()->UnregisterControl (m_visionPipeline.GetTrackAreaControl());
		mainWin->Close (true);
		// Main window is self-destroyed
	}
}


void CViacamController::WriteAppData(wxConfigBase* pConfObj)
{
	// General options
	m_configManager->WriteLanguage (m_languageId);
	pConfObj->Write(_T("cameraName"), m_cameraName);
}

void CViacamController::WriteProfileData(wxConfigBase* pConfObj)
{
	pConfObj->Write(_T("enabledAtStartup"), m_enabledAtStartup);	
	pConfObj->Write(_T("onScreenKeyboardCommand"), m_onScreenKeyboardCommand);
	pConfObj->Write(_T("runWizardAtStartup"), m_runWizardAtStartup);	

	// Propagates calls
	m_pointerAction->WriteProfileData (pConfObj);
	m_visionPipeline.WriteProfileData (pConfObj);
	m_hotKeyManager->WriteProfileData (pConfObj);
} 

void CViacamController::ReadAppData(wxConfigBase* pConfObj)
{
	// General options
	SetLanguage (m_configManager->ReadLanguage());	// Only load, dont't apply
	pConfObj->Read(_T("cameraName"), &m_cameraName);
}

void CViacamController::ReadProfileData(wxConfigBase* pConfObj)
{
	pConfObj->Read(_T("enabledAtStartup"), &m_enabledAtStartup);
	pConfObj->Read(_T("onScreenKeyboardCommand"), &m_onScreenKeyboardCommand);
	pConfObj->Read(_T("runWizardAtStartup"), &m_runWizardAtStartup);

	// Propagates calls
	m_pointerAction->ReadProfileData (pConfObj);
	m_visionPipeline.ReadProfileData (pConfObj);	
	m_hotKeyManager->ReadProfileData (pConfObj);
}

void CViacamController::StartupRun()
{
	if (m_enabledAtStartup) SetEnabled (true);
}


void CViacamController::SetEnabled (bool value, bool silent, wxWindow* parent)
{
	if (value!= m_enabled) {
		if (value== false && !silent) {
			wxMessageDialog dlg (parent, _("This action will disable eViacam.\nAre you sure?"), _("eViacam warning"), wxICON_EXCLAMATION | wxYES_NO );
			if (dlg.ShowModal()== wxID_NO) return;
		}
		m_enabled= value;				
	}
}

void CViacamController::OpenConfiguration()
{
	if (!m_pConfiguration) {
		m_pConfiguration = new WConfiguration(m_pMainWindow); //, this);
		m_pConfiguration->Connect (
			wxEVT_DESTROY, 
			wxWindowDestroyEventHandler(CViacamController::WConfigurationListener::OnDestroy), 
			NULL, 
			&m_wConfigurationListener);
	}
	
	m_pConfiguration->Show(true);
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

void CViacamController::ProcessImage (IplImage *pImage)
{
	WViacam::EFPSCondition cond;
	long iFrameRate;
	float vx, vy;
	CIplImage image(pImage);
	bool allowedByFaceLocalizationSystem;

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
	allowedByFaceLocalizationSystem = m_visionPipeline.ProcessImage (image, vx, vy);

	if ((m_enabled && allowedByFaceLocalizationSystem) || m_motionCalibrationEnabled) {
		// Send mouse motion
		BEGIN_GUI_CALL_MUTEX()
		if (m_motionCalibrationEnabled)
			m_pMotionCalibration->ComputeMotionRange (-vx, vy, !(m_visionPipeline.GetTrackFace() && !m_visionPipeline.IsFaceDetected()));
		else
			m_pointerAction->ProcessMotion (-vx, vy);
		END_GUI_CALL_MUTEX()
	}
	

	m_hotKeyManager->CheckKeyboardStatus();	
}

bool CViacamController::StartMotionCalibration (void)
{
	return m_pMotionCalibration->InitMotionCalibration();
}

void CViacamController::StartWizard()
{
	m_wizardManager.Open (m_pMainWindow);
}

const wxString& CViacamController::GetCameraName () const
{
	return m_cameraName;
}

const bool CViacamController::CameraHasSettingsDialog () const
{
	return (m_pCamera->HasSettingsDialog() || m_pCamera->HasCameraControls());
}

void CViacamController::ChangeCamera ()
{
	m_cameraName.Clear();
}

void CViacamController::ShowCameraSettingsDialog ()
{
	if (m_enabled && !m_pCameraDialog) {
		wxMessageDialog dlg (
			NULL, 
			_("Note that adjusting the camera controls while eViacam\nis enabled may cause loss of pointer control.\nAre you sure you want to continue?"), 
			_T("Enable Viacam"), 
			wxICON_EXCLAMATION | wxYES_NO );
		if (dlg.ShowModal()== wxID_NO) return;
	}

	if (m_pCamera->HasSettingsDialog()) m_pCamera->ShowSettingsDialog ();
	if (m_pCamera->HasCameraControls()) {
		if (!m_pCameraDialog) {
			m_pCameraDialog = new WCameraDialog(m_pMainWindow, m_pCamera);
			m_pCameraDialog->Connect (
				wxEVT_DESTROY, 
				wxWindowDestroyEventHandler(CViacamController::WCameraDialogListener::OnDestroy), 
				NULL, 
				&m_wCameraDialogListener);
			m_pCameraDialog->Show(true);
		}
		m_pCameraDialog->Raise();
	}
}
