//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose: 
//
// $NoKeywords: $
//=============================================================================//

#ifndef OPTIONS_SUB_AUDIO_H
#define OPTIONS_SUB_AUDIO_H
#ifdef _WIN32
#pragma once
#endif

#include "vgui_controls/PropertyPage.h"
#include "vgui_controls/PropertyDialog.h"
#include <language.h>
#include "vgui_controls/Frame.h"
#include "vgui/KeyCode.h"

class CLabeledCommandComboBox;
class CCvarSlider;
class CCvarToggleCheckButton;

//-----------------------------------------------------------------------------
// Purpose: Audio Details, Part of OptionsDialog
//-----------------------------------------------------------------------------
class COptionsSubAudio : public vgui::PropertyPage
{
	DECLARE_CLASS_SIMPLE( COptionsSubAudio, vgui::PropertyPage );

public:
	COptionsSubAudio(vgui::Panel *parent);
	~COptionsSubAudio();

	virtual void OnResetData();
	virtual void OnApplyChanges();
	virtual void OnCommand( const char *command );
	bool RequiresRestart();
   static char* GetUpdatedAudioLanguage() { return m_pchUpdatedAudioLanguage; }

private:
	MESSAGE_FUNC( OnControlModified, "ControlModified" );
	MESSAGE_FUNC( OnTextChanged, "TextChanged" )
	{
		OnControlModified();
	}

	MESSAGE_FUNC( RunTestSpeakers, "RunTestSpeakers" );

	vgui::ComboBox				*m_pSpeakerSetupCombo;
	vgui::ComboBox				*m_pSoundQualityCombo;
	CCvarSlider					*m_pSFXSlider;
	CCvarSlider					*m_pMusicSlider;
	vgui::ComboBox				*m_pCloseCaptionCombo;
	bool						   m_bRequireRestart;
   
   vgui::ComboBox				*m_pSpokenLanguageCombo;
   MESSAGE_FUNC( OpenThirdPartySoundCreditsDialog, "OpenThirdPartySoundCreditsDialog" );
   vgui::DHANDLE<class COptionsSubAudioThirdPartyCreditsDlg> m_OptionsSubAudioThirdPartyCreditsDlg;
   ELanguage         m_nCurrentAudioLanguage;
   static char             *m_pchUpdatedAudioLanguage;

    CCvarToggleCheckButton *m_pMuteLoseFocus;
};


//-----------------------------------------------------------------------------
// Purpose: third-party audio credits dialog
//-----------------------------------------------------------------------------
class COptionsSubAudioThirdPartyCreditsDlg : public vgui::PropertyDialog
{
	DECLARE_CLASS_SIMPLE( COptionsSubAudioThirdPartyCreditsDlg, vgui::PropertyDialog );
public:
	COptionsSubAudioThirdPartyCreditsDlg( vgui::VPANEL hParent );

	virtual void Activate();
	void OnKeyCodeTyped( vgui::KeyCode code );
	virtual void ApplySchemeSettings( vgui::IScheme *pScheme );
};


#endif // OPTIONS_SUB_AUDIO_H
