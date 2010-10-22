/////////////////////////////////////////////////////////////////////////////
// Name:        viacamcontroller.h
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

#ifndef VIACAMCONTROLLER_H
#define VIACAMCONTROLLER_H

#include "capturethread.h"
//#include "dwellaction.h"
#include "waittime.h"
#include "clickwindowcontroller.h"
#include "motiontracker.h"
#include "configbase.h"
#include "configmanager.h"
#include "crvcamera.h"
#include <cv.h>
#include "wconfiguration.h"

#if defined(__WXGTK__) 
#include <X11/Xlib.h>
#include "keyboardbitmapcheck.h"
#include "activationkey.h"
#endif // __WXGTK___
#include "cautostart.h"
#include "cmotioncalibration.h"

class WViacam;
//class CCamera;
class CMouseOutput;
class WConfiguration;
class CMotionCalibration;

class CViacamController : public CProcessImage, public CConfigBase, public wxDialog
{

public:
	CViacamController(void);
	virtual ~CViacamController(void);

	// Initialize application. false if failed
	bool Initialize ();
	void Finalize ();	

	virtual void ProcessImage (IplImage *pImage);

	inline const bool GetEnabled () const;
	void SetEnabled (bool value, bool silent= false);
	
        inline const bool GetEnabledAtStartup () const;
        inline void SetEnabledAtStartup (bool value);	

        inline const bool GetEnabledActivationKey () const;
        inline void SetEnabledActivationKey (bool value);
	inline void SetActivationKeyCode (int value);
	inline const wxString GetActivationKeyName () const;
	inline const bool GetMotionCalibration () const;
	inline void SetMotionCalibration (bool value);
	bool StartMotionCalibration (void);
	
	inline const bool GetRunWizardAtStartup () const;
	inline void SetRunWizardAtStartup (bool value);	
	
	void SetLanguage (const int id);
	inline const int GetLanguage () const;

	inline CAutostart* GetAutostart();
	inline WConfiguration* GetConfiguration();
	inline CMouseOutput* GetMouseOutput();
	inline CMotionTracker* GetMotionTracker();
	inline CClickWindowController* GetClickWindowController();
	inline WViacam* GetMainWindow();
	inline CConfigManager* GetConfigManager();
	inline CCamera* GetCamera();
	
	inline const wxString& GetOnScreenKeyboardCommand() const;
	inline void SetOnScreenKeyboardCommand(const wxString& value);

	inline const wxString& GetCameraName () const;
	inline const bool CameraHasSettingsDialog () const;
	inline void ShowCameraSettingsDialog () const;
	inline void ChangeCamera ();

	void OpenConfiguration();
	void OpenOnScreenKeyboard();
	void StartWizard();

	// Configuration methods
	virtual void InitDefaults();

	virtual void ReadAppData(wxConfigBase* pConfObj);
	virtual void ReadProfileData(wxConfigBase* pConfObj);

	virtual void WriteAppData(wxConfigBase* pConfObj);
	virtual void WriteProfileData(wxConfigBase* pConfObj);  

	virtual void StartupRun();

private:
	
	void ReleaseResources();
	void SetUpLanguage();
	CCamera* SetUpCamera();
	
	WViacam* m_pMainWindow;
	CCamera* m_pCamera;
	CCaptureThread* m_pCaptureThread;	
	CClickWindowController* m_pClickWindowController;
	CMouseOutput* m_pMouseOutput;
	//CDwellAction m_dWell;	
	CWaitTime m_dWell;	
	CMotionTracker m_motionTracker;
	CConfigManager* m_configManager;
	wxLocale* m_locale;
	wxString m_cameraName;

	bool m_enabled;
	bool m_enabledAtStartup;
	int m_languageId;
	wxString m_onScreenKeyboardCommand;
	float m_frameRate;
	bool m_enabledActivationKey;
	bool m_motionCalibrationEnabled;
	bool m_runWizardAtStartup;
#if defined(__WXGTK__) 
	int m_keyCode;
	int m_lastKeyCode;
#endif
	CAutostart* m_pAutostart;
	WConfiguration* m_pConfiguration;
	CMotionCalibration* m_pMotionCalibration;
};

inline CAutostart* CViacamController::GetAutostart()
{
	return m_pAutostart;
}

inline WConfiguration* CViacamController::GetConfiguration()
{
	return m_pConfiguration;
}

inline CMouseOutput* CViacamController::GetMouseOutput()
{
	return m_pMouseOutput;
}

inline const bool CViacamController::GetEnabled () const
{
	return m_enabled;
}

inline CMotionTracker* CViacamController::GetMotionTracker()
{
	return &m_motionTracker;
}

inline CClickWindowController* CViacamController::GetClickWindowController()
{
	return m_pClickWindowController;
}

inline const bool CViacamController::GetEnabledAtStartup () const
{
	return m_enabledAtStartup;
}

inline void CViacamController::SetEnabledAtStartup (bool value)
{
	m_enabledAtStartup= value;
}

inline const int CViacamController::GetLanguage () const
{
	return m_languageId;
}

inline WViacam* CViacamController::GetMainWindow()
{
	return CViacamController::m_pMainWindow;
}

inline const wxString& CViacamController::GetOnScreenKeyboardCommand() const
{
	return m_onScreenKeyboardCommand;
}

inline const wxString& CViacamController::GetCameraName () const
{
	return m_cameraName;
}

inline const bool CViacamController::CameraHasSettingsDialog () const
{
	return m_pCamera->HasSettingsDialog();
}

inline void CViacamController::ShowCameraSettingsDialog () const
{
	m_pCamera->ShowSettingsDialog ();
}

inline void CViacamController::ChangeCamera ()
{
	m_cameraName.Clear();
}

inline void CViacamController::SetOnScreenKeyboardCommand(const wxString& value)
{
	m_onScreenKeyboardCommand= value;
}

inline CConfigManager* CViacamController::GetConfigManager()
{
	return m_configManager;
}

inline CCamera* CViacamController::GetCamera()
{
	return m_pCamera;
}

inline const bool CViacamController::GetEnabledActivationKey () const
{
    return m_enabledActivationKey;
}

inline void CViacamController::SetEnabledActivationKey (bool value)
{
	m_enabledActivationKey= value;
}

inline void CViacamController::SetActivationKeyCode (int value)
{
#if defined(__WXGTK__)
	m_keyCode= value;
	m_lastKeyCode= value;
#else
	assert (false);		// Win: not yet implemented
#endif
}

inline const wxString CViacamController::GetActivationKeyName () const
{
#if defined(__WXGTK__)
	return CKeyboardBitmapCheck::GetKeyName(m_keyCode);
#else
	return wxString(_T(""));
#endif
}

inline const bool CViacamController::GetMotionCalibration () const
{
	return m_motionCalibrationEnabled;
}

inline void CViacamController::SetMotionCalibration (bool value)
{
	m_motionCalibrationEnabled= value;
}

inline const bool CViacamController::GetRunWizardAtStartup () const
{
	return m_runWizardAtStartup;
}

inline void CViacamController::SetRunWizardAtStartup (bool value)
{
	m_runWizardAtStartup= value;
}

#endif
