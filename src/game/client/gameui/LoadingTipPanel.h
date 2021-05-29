//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose: Tip display during level loads.
//
//===========================================================================//
#ifndef LOADINGTIPPANEL_H
#define LOADINGTIPPANEL_H
#ifdef _WIN32
#pragma once
#endif

#include "vgui_controls/EditablePanel.h"

class CLoadingTipPanel : public vgui::EditablePanel
{
	DECLARE_CLASS_SIMPLE( CLoadingTipPanel, vgui::EditablePanel )

public:
	CLoadingTipPanel( Panel *pParent );
	~CLoadingTipPanel() {}

	virtual void ApplySchemeSettings( vgui::IScheme *pScheme );
	void ReloadScheme( void );

	void NextTip( void );

private:
	void SetupTips( void );
};

#endif // LOADINGTIPPANEL_H
