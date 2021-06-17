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
#include "steam/steam_api.h"
#include "controls/tf_advslider.h"
#include "controls/tf_advbutton.h"

namespace BaseModUI 
{
	class CServerlistPanel;

	class MainMenu : public CBaseModFrame, public IBaseModFrameListener
	{
		DECLARE_CLASS_SIMPLE( MainMenu, CBaseModFrame );

	public:
		MainMenu( vgui::Panel *parent, const char *panelName );
		~MainMenu();

		void UpdateVisibility();

		virtual void SetServerlistSize( int size );
		virtual void UpdateServerInfo();

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
		virtual void PaintBackground() {}
		virtual void PerformLayout();

		MESSAGE_FUNC( OnGameUIHidden, "GameUIHidden" );	// called when the GameUI is hidden
		MESSAGE_FUNC_INT( OnKeyCodeUnhandled, "KeyCodeUnhandled", code );

	private:
		static void AcceptQuitGameCallback();
		static void LeaveGameOkCallback();

		CSteamID			m_SteamID;
		CServerlistPanel	*m_pServerlistPanel;
	};

	class CServerlistPanel : public vgui::EditablePanel
	{
		DECLARE_CLASS_SIMPLE( CServerlistPanel, vgui::EditablePanel );

	public:
				CServerlistPanel( vgui::Panel* parent, const char *panelName );
		virtual ~CServerlistPanel();

		virtual void PerformLayout();
		virtual void ApplySchemeSettings( vgui::IScheme *pScheme );
		virtual void SetServerlistSize( int size );
		virtual void UpdateServerInfo();
		virtual void OnThink();
		virtual void OnCommand( const char* command );

	private:
		static bool ServerSortFunc( vgui::SectionedListPanel *list, int itemID1, int itemID2 );

		vgui::SectionedListPanel	*m_pServerList;
		CTFAdvButton				*m_pConnectButton;
		CTFAdvSlider				*m_pListSlider;

		CPanelAnimationVarAliasType( int, m_iServerWidth, "server_width", "35", "proportional_int" );
		CPanelAnimationVarAliasType( int, m_iPlayersWidth, "players_width", "35", "proportional_int" );
		CPanelAnimationVarAliasType( int, m_iPingWidth, "ping_width", "23", "proportional_int" );
		CPanelAnimationVarAliasType( int, m_iMapWidth, "map_width", "23", "proportional_int" );
		CPanelAnimationVarAliasType( int, m_iScrollWidth, "scroll_width", "23", "proportional_int" );

		int		m_iSize;
	};
}
#endif // VMAINMENU_H