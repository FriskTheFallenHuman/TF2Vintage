//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose:
//
//===========================================================================//

#ifndef VINGAMEMAINMENU_H
#define VINGAMEMAINMENU_H
#ifdef _WIN32
#pragma once
#endif

#include "basemodui.h"
#include "vflyoutmenu.h"

namespace BaseModUI 
{
	class InGameMainMenu : public CBaseModFrame, public FlyoutMenuListener
	{
		DECLARE_CLASS_SIMPLE( InGameMainMenu, CBaseModFrame );

	public:
		InGameMainMenu( vgui::Panel *parent, const char *panelName );
		~InGameMainMenu();

		// Public methods
		void OnCommand(const char *command);

		// Overrides
		virtual void OnKeyCodePressed( vgui::KeyCode code );
		virtual void OnKeyCodeTyped( vgui::KeyCode code );
		virtual void ApplySchemeSettings( vgui::IScheme *pScheme );
		virtual void PaintBackground();
		virtual void OnOpen();
		virtual void OnClose();
		virtual void OnThink();
		virtual void PerformLayout();
		virtual void Unpause() {}

		//flyout menu listener
		virtual void OnNotifyChildFocus( vgui::Panel* child ) {}
		virtual void OnFlyoutMenuClose( vgui::Panel* flyTo ) {}
		virtual void OnFlyoutMenuCancelled() {}

		MESSAGE_FUNC( OnGameUIHidden, "GameUIHidden" );	// called when the GameUI is hidden

	private:
		static void LeaveGameOkCallback();
		static void AcceptQuitGameCallback();
	};
}
#endif // VINGAMEMAINMENU_H
