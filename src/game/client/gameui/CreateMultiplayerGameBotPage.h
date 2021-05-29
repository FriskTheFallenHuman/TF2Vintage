//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose: 
//
// $NoKeywords: $
//=============================================================================//

#ifndef CREATEMULTIPLAYERGAMEBOTPAGE_H
#define CREATEMULTIPLAYERGAMEBOTPAGE_H
#ifdef _WIN32
#pragma once
#endif

#include <vgui_controls/PropertyPage.h>

class CPanelListPanel;
class CDescription;
class mpcontrol_t;
class CCvarToggleCheckButton;

//-----------------------------------------------------------------------------
// Purpose: advanced bot properties page of the create game server dialog
//-----------------------------------------------------------------------------
class CCreateMultiplayerGameBotPage : public vgui::PropertyPage
{
	DECLARE_CLASS_SIMPLE( CCreateMultiplayerGameBotPage, vgui::PropertyPage );

public:
	CCreateMultiplayerGameBotPage( vgui::Panel *parent, const char *name, KeyValues *botKeys );
	~CCreateMultiplayerGameBotPage();

protected:
	virtual void OnResetChanges();
	virtual void OnApplyChanges();

private:
	CCvarToggleCheckButton *m_TFBotjoinAfterPlayer;
	CCvarToggleCheckButton *m_TFBotAutoVacate;
	CCvarToggleCheckButton *m_TFBotOfflinePratice;
	CCvarToggleCheckButton *m_TFBotMeleeOnly;
	CCvarToggleCheckButton *m_TFBotKeepClass;
	CCvarToggleCheckButton *m_TFBotPrefixDifficulty;
	CCvarToggleCheckButton *m_TFBotUseItems;
	CCvarToggleCheckButton *m_TFBotEconRandom;
	CCvarToggleCheckButton *m_TFBotKeepEcon;

	vgui::TextEntry *m_TFBotQuotaMode;
	vgui::TextEntry *m_TFBotClass;

	KeyValues *m_pSavedData;
};


#endif // CREATEMULTIPLAYERGAMEBOTPAGE_H
