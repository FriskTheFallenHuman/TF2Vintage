//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose:
//
//===========================================================================//

#ifndef VMAINMENU_H
#define VMAINMENU_H
#ifdef _WIN32
#pragma once
#endif

#include "basemodui.h"
#include "vflyoutmenu.h"

namespace BaseModUI 
{
	class MainMenu : public CBaseModFrame, public IBaseModFrameListener, public FlyoutMenuListener
	{
		DECLARE_CLASS_SIMPLE( MainMenu, CBaseModFrame );

	public:
		MainMenu( vgui::Panel *parent, const char *panelName );
		~MainMenu();

		void UpdateVisibility();

		//flyout menu listener
		virtual void OnNotifyChildFocus( vgui::Panel* child ) {}
		virtual void OnFlyoutMenuClose( vgui::Panel* flyTo ) {}
		virtual void OnFlyoutMenuCancelled() {}

	protected:
		virtual void ApplySchemeSettings( vgui::IScheme *pScheme );
		virtual void OnCommand( const char *command );
		virtual void OnKeyCodePressed( vgui::KeyCode code );
		virtual void OnKeyCodeTyped( vgui::KeyCode code );
		virtual void OnThink();
		virtual void OnOpen();
		virtual void OnClose();
		virtual void Unpause() {}
		virtual void RunFrame();
		virtual void PaintBackground();
		virtual void PerformLayout();

		MESSAGE_FUNC( OnGameUIHidden, "GameUIHidden" );	// called when the GameUI is hidden

	private:

		static void AcceptQuitGameCallback();
		static void LeaveGameOkCallback();
		void SetFooterState() {}

		vgui::ImagePanel	*m_pLogoImage;
	};
}
#endif // VMAINMENU_H