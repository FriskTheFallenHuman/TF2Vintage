//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose: 
//
// $NoKeywords: $
//=============================================================================//

#ifndef CREATEMULTIPLAYERGAMEDIALOG_H
#define CREATEMULTIPLAYERGAMEDIALOG_H
#ifdef _WIN32
#pragma once
#endif

#include <vgui_controls/PropertyDialog.h>

class CCreateMultiplayerGameServerPage;
class CCreateMultiplayerGameGameplayPage;

//-----------------------------------------------------------------------------
// Purpose: dialog for launching a listenserver
//-----------------------------------------------------------------------------
class CCreateMultiplayerGameDialog : public vgui::PropertyDialog
{
	DECLARE_CLASS_SIMPLE( CCreateMultiplayerGameDialog,  vgui::PropertyDialog );

public:
	CCreateMultiplayerGameDialog( vgui::Panel *parent );
	~CCreateMultiplayerGameDialog();

	void SetupTabs();
	virtual void ApplySchemeSettings( vgui::IScheme *pScheme );

protected:

	// vgui overrides
	virtual void PerformLayout() OVERRIDE;
	virtual bool OnOK( bool applyOnly );

private:
	CCreateMultiplayerGameServerPage *m_pServerPage;
	CCreateMultiplayerGameGameplayPage *m_pGameplayPage;

	// for loading/saving game config
	KeyValues *m_pSavedData;
};

#endif // CREATEMULTIPLAYERGAMEDIALOG_H
