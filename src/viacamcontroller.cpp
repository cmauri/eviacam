/////////////////////////////////////////////////////////////////////////////
// Name:        viacamcontroller.cpp
// Purpose:  
// Author:      Cesar Mauri Loba (cesar at crea-si dot com)
// Modified by: 
// Created:     
// Copyright:   (C) 2008-14 Cesar Mauri Loba - CREA Software Systems
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
#include "simplelog.h"
#include "newtrackerinformationdlg.h"
#include "checkupdates_manager.h"

#include <wx/msgdlg.h>
#include <wx/choicdlg.h>
#include <wx/filename.h>
#include <wx/stdpaths.h>

using namespace eviacam;

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
, m_pCheckUpdateManager(NULL)
, m_cameraName()
, m_enabled(false)
, m_enabledAtStartup(false)
, m_languageId(wxLANGUAGE_DEFAULT)
, m_onScreenKeyboardCommand()
, m_frameRate(0)
, m_motionCalibrationEnabled(false)
, m_runWizardAtStartup(false)
, m_newTrackerDialogAtStartup(true)
, m_checkUpdatesAtStartup(true)
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
	
#if defined(__WXGTK__)	
	wxString prefix= wxStandardPaths::Get().GetInstallPrefix();
	prefix+= wxT("/share/locale/");
	m_locale->AddCatalogLookupPathPrefix(prefix);
#endif

	if (!m_locale->Init(m_languageId))
		slog_write (SLOG_PRIO_WARNING, "Cannot load locale. Switching to default locale.\n");
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

bool testCamera(CCamera* cam) {
	if (cam == NULL) return false;

	IplImage* img = NULL;
	if (cam->Open()) {
		/*
			Try to capture one frame
		*/
		for (int i = 0; i < 10; i++) {
			img = cam->QueryFrame();
			if (img) break;
		}

		cam->Close();
	}

	return (img != NULL);
}

CCamera* CViacamController::SetUpCamera()
{
	// Load app local data
	ReadAppData(wxConfigBase::Get());

	/* 
		Check if cameras detected
	*/
	int driverId = 0;
	for (; driverId < CCameraEnum::NUM_DRIVERS; driverId++) {
		if (CCameraEnum::getNumDevices(driverId) > 0) break;
	}

	if (driverId == CCameraEnum::NUM_DRIVERS) {
		/*
			No cameras found, aborting.
		*/
		wxMessageDialog errorMsg (NULL, _("Not detected any camera. Aborting"), 
			_T("Enable Viacam"), wxOK | wxICON_ERROR);
		errorMsg.ShowModal();
		return NULL;
	}
	
	/*
		Try to find previously used camera with the same name
		(only check native driver)
	*/
	int camId = -1;
	const char * camName= NULL;
	if (m_cameraName.Length()> 0) {
		for (camId = 0; camId < CCameraEnum::getNumDevices(0); camId++) {
			camName = CCameraEnum::getDeviceName(0, camId);
			if (wxString(camName, wxConvLibc) == m_cameraName) {
				break;
			}
		}

		if (camId == CCameraEnum::getNumDevices(0)) camId = -1;
	}

	/*
		Show selection dialog when needed (use native driver names)
	*/
	if (camId == -1) {
		if (CCameraEnum::getNumDevices(0) > 1) {
			wxArrayString strArray;

			for (int i = 0; i < CCameraEnum::getNumDevices(0); i++) {
				strArray.Add(wxString(CCameraEnum::getDeviceName(0, i), wxConvLibc));
			}

			wxSingleChoiceDialog choiceDlg(
				NULL, _("Choose the camera to use"), _T("Enable Viacam"), strArray, 
				(char**)NULL, wxDEFAULT_DIALOG_STYLE | wxOK | wxCANCEL | wxCENTRE);

			if (choiceDlg.ShowModal ()!= wxID_OK) return NULL;

			camId= choiceDlg.GetSelection();
			m_cameraName= choiceDlg.GetStringSelection ();
		} 
		else {
			camId= 0;
			m_cameraName = wxString(CCameraEnum::getDeviceName(0, camId), wxConvLibc);
		}
	}

	/*
		Try to open the camera to make sure it works
	*/
	// native driver first
	CCamera* cam = CCameraEnum::getCamera(0, camId);
	if (!testCamera(cam)) {
		// Failed. Now try the openCV driver
		delete cam;
		cam = CCameraEnum::getCamera(1, camId);
		if (!testCamera(cam)) {
			// no way
			delete cam;
			cam = NULL;

			wxMessageDialog errorMsg(
				NULL, _("Can not initialize the camera.\nPerhaps is being used by other application."),
				_T("Enable Viacam"), wxOK | wxICON_ERROR);
			errorMsg.ShowModal();

			ChangeCamera();
		}
	}

	if (cam) cam->SetHorizontalFlip(true);
	
	WriteAppData(wxConfigBase::Get());
	wxConfigBase::Get()->Flush();

	return cam;
}

bool CViacamController::Initialize ()
{	
	bool retval= true;
	assert (!m_pMainWindow && !m_pCamera && !m_pCaptureThread);

	SetUpLanguage ();

	// Is the first time eviacam is executed on this computer?
	if (!wxConfigBase::Get()->Exists(_T("/settings/default")))
		m_newTrackerDialogAtStartup = false;

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
		m_hotKeyManager= new eviacam::HotkeyManager();
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
	if (retval) m_configManager->ReadAll ();
	
	// Enable pointeraction object
	if (retval && m_enabledAtStartup) SetEnabled(true);
	if (retval) m_pointerAction->SetEnabled(true);

#if defined(ENABLE_UPDATES_CHECK)
	// Check for updates
	if (retval && m_checkUpdatesAtStartup) {
		assert(m_pCheckUpdateManager == NULL);
		m_pCheckUpdateManager = new CheckUpdatesManager(m_pMainWindow);
		m_pCheckUpdateManager->LaunchBackground();
	}
#endif

	// Show new tracker information dialog when needed
	if (retval && m_newTrackerDialogAtStartup) {
		NewTrackerInformationDlg dlg(m_pMainWindow);
		dlg.ShowModal();
	}

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

	if (m_pCheckUpdateManager) {
		delete m_pCheckUpdateManager;
		m_pCheckUpdateManager = NULL;
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
	pConfObj->Write(_T("newTrackerDialogAtStartup"), m_newTrackerDialogAtStartup);
	pConfObj->Write(_T("checkUpdatesAtStartup"), m_checkUpdatesAtStartup);
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
	pConfObj->Read(_T("newTrackerDialogAtStartup"), &m_newTrackerDialogAtStartup);
	pConfObj->Read(_T("checkUpdatesAtStartup"), &m_checkUpdatesAtStartup);
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

void CViacamController::SetEnabled (bool value, bool silent, wxWindow* parent)
{
	if (value!= m_enabled) {
		if (value== false && !silent) {
			wxMessageDialog dlg (parent, _("This action will disable eViacam.\nAre you sure?"), _("eViacam warning"), wxICON_EXCLAMATION | wxYES_NO );
			if (dlg.ShowModal()== wxID_NO) return;
		}
		if (value) {
			// Enable, reset counters
			m_pointerAction->ResetCounters();
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
