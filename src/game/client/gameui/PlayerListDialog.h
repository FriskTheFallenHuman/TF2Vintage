//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose: 
//
// $NoKeywords: $
//=============================================================================//

#ifndef PLAYERLISTDIALOG_H
#define PLAYERLISTDIALOG_H
#ifdef _WIN32
#pragma once
#endif

#include <vgui_controls/PropertyDialog.h>

//-----------------------------------------------------------------------------
// Purpose: List of players, their ingame-name and their friends-name
//-----------------------------------------------------------------------------
class CPlayerListDialog : public vgui::PropertyDialog
{
	DECLARE_CLASS_SIMPLE( CPlayerListDialog, vgui::PropertyDialog ); 

public:
	CPlayerListDialog(vgui::Panel *parent);
	~CPlayerListDialog();

	virtual void Activate();
	virtual void ApplySchemeSettings( vgui::IScheme *pScheme );

private:
  	MESSAGE_FUNC( OnItemSelected, "ItemSelected" );
	virtual void OnCommand(const char *command);

	void ToggleMuteStateOfSelectedUser();
	void RefreshPlayerProperties();

	vgui::ListPanel *m_pPlayerList;
	vgui::Button *m_pMuteButton;
};


#endif // PLAYERLISTDIALOG_H
