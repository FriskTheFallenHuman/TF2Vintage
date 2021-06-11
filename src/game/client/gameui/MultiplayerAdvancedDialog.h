//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose: 
//
// $NoKeywords: $
//=============================================================================//

#ifndef MULTIPLAYERADVANCEDDIALOG_H
#define MULTIPLAYERADVANCEDDIALOG_H
#ifdef _WIN32
#pragma once
#endif

#include <vgui_controls/PropertyDialog.h>
#include "ScriptObject.h"
#include <vgui/KeyCode.h>

//-----------------------------------------------------------------------------
// Purpose: Displays a game-specific list of options
//-----------------------------------------------------------------------------
class CMultiplayerAdvancedDialog : public vgui::PropertyDialog
{
	DECLARE_CLASS_SIMPLE( CMultiplayerAdvancedDialog, vgui::PropertyDialog ); 

public:
	CMultiplayerAdvancedDialog( vgui::Panel *parent );
	~CMultiplayerAdvancedDialog();

	void Run();
	virtual void Activate();
	virtual void ApplySchemeSettings( vgui::IScheme *pScheme );

	MESSAGE_FUNC( OnGameUIHidden, "GameUIHidden" );	// called when the GameUI is hidden

private:

	void CreateControls();
	void DestroyControls();
	void GatherCurrentValues();
	void SaveValues();

	CInfoDescription *m_pDescription;

	mpcontrol_t *m_pList;

	CPanelListPanel *m_pListPanel;

	virtual void OnCommand( const char *command );
	virtual void OnClose();
	virtual bool OnOK(bool applyOnly);
	virtual void OnKeyCodeTyped(vgui::KeyCode code);
};

#endif // MULTIPLAYERADVANCEDDIALOG_H
