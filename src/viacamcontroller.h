/////////////////////////////////////////////////////////////////////////////
// Name:        viacamcontroller.h
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

#ifndef VIACAMCONTROLLER_H
#define VIACAMCONTROLLER_H

#include "capturethread.h"
#include "visionpipeline.h"
#include "configbase.h"
#include "wwizardmanager.h"
#include "hotkeymanager.h"

class WViacam;
class CPointerAction;
class WConfiguration;
class CMotionCalibration;
class wxWindow;
class CConfigManager;
class CCamera;
class CAutostart;
class wxLocale;
class WCameraDialog;
namespace eviacam {
	class CheckUpdatesManager;
}

class CViacamController : public CProcessImage, public CConfigBase
{

public:
	CViacamController(void);
	virtual ~CViacamController(void);
		
	const bool GetEnabled () const { return m_enabled; }
	void SetEnabled (bool value, bool silent= false, wxWindow* parent= NULL);
	
	const bool GetEnabledAtStartup () const { return m_enabledAtStartup; }
	void SetEnabledAtStartup (bool value) { m_enabledAtStartup= value; }

	const bool GetMotionCalibrationEnabled () const {
		return m_motionCalibrationEnabled;
	}
	void SetMotionCalibrationEnabled (bool value) {
		m_motionCalibrationEnabled= value;
	}
	bool StartMotionCalibration (void);
	
	const bool GetRunWizardAtStartup () const {	return m_runWizardAtStartup; }
	void SetRunWizardAtStartup (bool value) { m_runWizardAtStartup= value; }
		
	const bool GetCheckUpdatesAtStartup() const { return m_checkUpdatesAtStartup; }
	void SetCheckUpdatesAtStartup(bool value) { m_checkUpdatesAtStartup = value; }

	void SetLanguage (const int id);
	const int GetLanguage () const { return m_languageId; }

	const wxString& GetOnScreenKeyboardCommand() const {
		return m_onScreenKeyboardCommand;
	}
	void SetOnScreenKeyboardCommand(const wxString& value) {
		m_onScreenKeyboardCommand= value;
	}

	const wxString& GetCameraName () const;
	const bool CameraHasSettingsDialog () const;
	void ShowCameraSettingsDialog ();
	void ChangeCamera ();
	
	void OpenConfiguration();
	void OpenOnScreenKeyboard();
	void StartWizard();

	// Getters of objects that compose the controller
	CAutostart& GetAutostart() { assert (m_pAutostart);	return *m_pAutostart; }

	CPointerAction& GetPointerAction() {
		assert (m_pointerAction); return *m_pointerAction;
	}

	CVisionPipeline& GetVisionPipeline() { return m_visionPipeline; }

	WViacam* GetMainWindow() { return m_pMainWindow; }
	
	CConfigManager& GetConfigManager() {
		assert(m_configManager); return *m_configManager;
	}

	CCamera& GetCamera() { assert(m_pCamera); return *m_pCamera; }

	eviacam::HotkeyManager& getHotkeyManager() {
		assert (m_hotKeyManager); return *m_hotKeyManager;
	}

	wxLocale* GetLocale() { return m_locale; }

	bool GetNewTrackerDialogAtStartup() const {
		return m_newTrackerDialogAtStartup;
	}
	void SetNewTrackerDialogAtStartup(bool value) {
		m_newTrackerDialogAtStartup = value;
	}

	// Configuration methods
	virtual void InitDefaults();

	virtual void ReadAppData(wxConfigBase* pConfObj);
	virtual void ReadProfileData(wxConfigBase* pConfObj);

	virtual void WriteAppData(wxConfigBase* pConfObj);
	virtual void WriteProfileData(wxConfigBase* pConfObj);  

	// Initialize application. false if failed
	bool Initialize ();
	void Finalize ();

	// Image callback
	virtual void ProcessImage (IplImage *pImage);
private:
	void ReleaseResources();
	void SetUpLanguage();		
	CCamera* SetUpCamera();

	/*
		Listeners for non-modal dialogues.
	*/
	class WConfigurationListener : public wxEvtHandler {
	public:
		WConfigurationListener(CViacamController& c) : m_controller(&c) {}
		void OnDestroy( wxWindowDestroyEvent& event ) {
			m_controller->m_pConfiguration= NULL;
			event.Skip(false);
		}
	private:
		CViacamController* m_controller;
	};

	class WCameraDialogListener : public wxEvtHandler {
	public:
		WCameraDialogListener(CViacamController& c) : m_controller(&c) {}
		void OnDestroy( wxWindowDestroyEvent& event ) {
			m_controller->m_pCameraDialog= NULL;
			event.Skip(false);
		}
	private:
		CViacamController* m_controller;
	};

	// objects that compose the controller
	WViacam* m_pMainWindow;
	CCamera* m_pCamera;
	CCaptureThread* m_pCaptureThread;	
	CPointerAction* m_pointerAction;
	CVisionPipeline m_visionPipeline;
	eviacam::HotkeyManager* m_hotKeyManager;
	CConfigManager* m_configManager;
	wxLocale* m_locale;
	CAutostart* m_pAutostart;
	WConfiguration* m_pConfiguration;
	CMotionCalibration* m_pMotionCalibration;
	WWizardManager m_wizardManager;
	WCameraDialog* m_pCameraDialog;
	WConfigurationListener m_wConfigurationListener;
	WCameraDialogListener m_wCameraDialogListener;
	eviacam::CheckUpdatesManager* m_pCheckUpdateManager;


	wxString m_cameraName;
	volatile bool m_enabled;
	bool m_enabledAtStartup;
	int m_languageId;
	wxString m_onScreenKeyboardCommand;
	float m_frameRate;
	bool m_motionCalibrationEnabled;
	bool m_runWizardAtStartup;
	bool m_newTrackerDialogAtStartup;
	bool m_checkUpdatesAtStartup;

};

#endif
