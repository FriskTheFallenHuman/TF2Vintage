//========= Copyright © 1996-2008, Valve Corporation, All rights reserved. ============//
//
// Purpose: 
//
//=====================================================================================//

#ifndef __VMAINMENU_H__
#define __VMAINMENU_H__

#include "basemodui.h"
#include "vflyoutmenu.h"
#include "steam/steam_api.h"
#include "vgui_avatarimage.h"
#include "ExImageMenuButton.h"

namespace BaseModUI 
{
	class CSteamDetailsPanel;

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
		virtual void RunFrame();
		virtual void PaintBackground();

	private:

		static void AcceptQuitGameCallback();
		void SetFooterState() {}

		vgui::ImagePanel	*m_pLogoImage;
		CSteamDetailsPanel	*m_pSteamDetails;
	};

	//-----------------------------------------------------------------------------
	// Purpose: 
	//-----------------------------------------------------------------------------
	class CSteamDetailsPanel : public vgui::EditablePanel
	{
		DECLARE_CLASS_SIMPLE( CSteamDetailsPanel, vgui::EditablePanel );

	public:
				CSteamDetailsPanel( vgui::Panel* parent );
		virtual ~CSteamDetailsPanel() {}

		virtual void PerformLayout();
		virtual void ApplySchemeSettings( vgui::IScheme *pScheme );

	private:
		CAvatarImagePanel	*m_pProfileAvatar; 
		CSteamID			m_SteamID;
	};

}

#endif // __VMAINMENU_H__
