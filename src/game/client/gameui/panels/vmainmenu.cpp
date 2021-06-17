//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose:
//
//===========================================================================//

#include "cbase.h"
#include "vmainmenu.h"
#include "EngineInterface.h"
#include "vgenericconfirmation.h"
#include "basemodpanel.h"

#include "vgui/ILocalize.h"
#include "vgui/IInput.h"
#include "vgui/ISurface.h"
#include "vgui_controls/Label.h"
#include "vgui_controls/Button.h"
#include "vgui_controls/Tooltip.h"
#include "vgui_controls/ImagePanel.h"
#include "vgui_controls/Image.h"
#include "vgui_controls/SectionedListPanel.h"
#include "vgui_controls/ScalableImagePanel.h"
#include "vgui_avatarimage.h"

#include "materialsystem/materialsystem_config.h"

#include "ienginevgui.h"
#include "basepanel.h"
#include "tier0/icommandline.h"
#include "fmtstr.h"

#include "FileSystem.h"
#include "GameUI_Interface.h"
#include "IGameUIFuncs.h"
#include "time.h"

#include "tf_controls.h"
#include "tf_gamerules.h"
#include "tf_notificationmanager.h"
#include "nb_header_footer.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

using namespace vgui;
using namespace BaseModUI;

//=============================================================================
MainMenu::MainMenu( Panel *parent, const char *panelName ):	BaseClass( parent, panelName, true, true, false, false )
{
	SetProportional( true );
	SetTitle( "", false );
	SetMoveable( false );
	SetSizeable( false );

	SetLowerGarnishEnabled( true );

	AddFrameListener( this );

	SetDeleteSelfOnClose( true );

	if ( steamapicontext->SteamUser() )
		m_SteamID = steamapicontext->SteamUser()->GetSteamID();

	m_pServerlistPanel = new CServerlistPanel( this, "ServerlistPanel" );
	m_pServerlistPanel->AddActionSignalTarget( this );

	vgui::input()->RegisterKeyCodeUnhandledListener( GetVPanel() );
}

//=============================================================================
MainMenu::~MainMenu()
{
	RemoveFrameListener( this );
}

//=============================================================================
void MainMenu::OnCommand( const char *command )
{
	if ( UI_IsDebug() )
		ConColorMsg( Color( 77, 116, 85, 255 ), "[GAMEUI] Handling main menu command %s\n", command );

	bool bMpOnly = GameUI().IsInMultiplayer();

	if ( bMpOnly && !Q_strcmp( command, "ReturnToGame" ) )
	{
		engine->ClientCmd( "gameui_hide" );
	}
	else if( bMpOnly && !Q_strcmp( command, "MutePlayer" ) )
	{
		CBaseModPanel::GetSingleton().OpenPlayerListDialog( this );
	}
	else if (  bMpOnly && !Q_strcmp( command, "CallVote" ) )
	{
		engine->ClientCmd( "gameui_hide" );
		engine->ClientCmd( "callvote" );
	}
	else if( bMpOnly && !Q_strcmp( command, "ExitToMainMenu" ) )
	{
		MakeGenericDialog( "#TF_MM_Disconnect_Title", 
						   "#TF_MM_Disconnect", 
						   true, 
						   &LeaveGameOkCallback,
						   true,
						   this );
	}
	else if ( !Q_strcmp( command, "QuitGame" ) )
	{
		MakeGenericDialog( "#MMenu_PromptQuit_Title", 
						   "#MMenu_PromptQuit_Body", 
						   true, 
						   &AcceptQuitGameCallback, 
						   true,
						   this );
	}
	else if ( !Q_stricmp( command, "QuitGame_NoConfirm" ) )
	{
		engine->ExecuteClientCmd( "quit" );
	}
	else if ( !Q_strcmp( command, "GameOptions" ) )
	{
		CBaseModPanel::GetSingleton().OpenOptionsDialog( this );
	}
	else if ( !Q_strcmp( command, "TF2Options" ) )
	{
		CBaseModPanel::GetSingleton().OpenTFOptionsDialog( this );
	}
	else if ( !Q_strcmp( command, "Backpack" ) )
	{
		CBaseModPanel::GetSingleton().OpenArmoryDialog( this );
	}
	else if ( !Q_strcmp( command, "ServerBrowser" ) )
	{
		CBaseModPanel::GetSingleton().OpenServerBrowser();
	}
	else if( !Q_strcmp( command, "CreateGame" ) )
	{
		CBaseModPanel::GetSingleton().OpenCreateMultiplayerGameDialog( this );
	}
	else if ( !Q_strcmp( command, "Achievements" ) )
	{
		CBaseModPanel::GetSingleton().OpenAchievementsDialog( this );
	}
	else if ( !Q_stricmp( command, "ReleaseModalWindow" ) )
	{
		vgui::surface()->RestrictPaintToSinglePanel( NULL );
	}
	else if ( Q_stristr( command, "engine " ) )
	{
		const char *engineCMD = strstr( command, "engine " ) + strlen( "engine " );
		if ( strlen( engineCMD ) > 0 )
			engine->ClientCmd_Unrestricted( const_cast<char *>( engineCMD ) );
	}
	else if ( Q_stristr( command, "steam " ) )
	{
		const char *SteamDialogUrl = strstr( command, "steam " ) + strlen( "steam " );
		if ( strlen( SteamDialogUrl ) > 0 )
		{
			if ( steamapicontext && steamapicontext->SteamFriends() )
				steamapicontext->SteamFriends()->ActivateGameOverlay( const_cast<char *>( SteamDialogUrl ) );
		}
	}
	else if ( Q_stristr( command, "website " ) )
	{
		const char *WebsiteUrl = strstr( command, "website " ) + strlen( "website " );
		if ( strlen( WebsiteUrl ) > 0 )
		{
			if ( steamapicontext && steamapicontext->SteamFriends() )
				steamapicontext->SteamFriends()->ActivateGameOverlayToWebPage( const_cast<char *>( WebsiteUrl ) );
		}
	}
	else
	{
		BaseClass::OnCommand( command );
	}
}

//=============================================================================
void MainMenu::OnKeyCodeUnhandled( int code )
{
    const char *binding = gameuifuncs->GetBindingForButtonCode( static_cast<ButtonCode_t>( code ) );

    if ( binding && ( FStrEq( binding, "toggleconsole" ) || FStrEq( binding, "showconsole" ) ) )
        engine->ClientCmd_Unrestricted( binding );
} 

//=============================================================================
void MainMenu::OnKeyCodePressed( KeyCode code )
{
	int userId = GetJoystickForCode( code );
	BaseModUI::CBaseModPanel::GetSingleton().SetLastActiveUserId( userId );

	bool bMpOnly = GameUI().IsInMultiplayer();

	switch( GetBaseButtonCode( code ) )
	{
	case KEY_XBUTTON_START:
	case KEY_XBUTTON_B:
		if ( bMpOnly )
		{
			CBaseModPanel::GetSingleton().PlayUISound( UISOUND_BACK );
			OnCommand( "ReturnToGame" );	
		}
		break;

	default:
		BaseClass::OnKeyCodePressed( code );
		break;
	}
}

//=============================================================================
void MainMenu::OnKeyCodeTyped( KeyCode code )
{
	BaseClass::OnKeyTyped( code );
}

//=============================================================================
void MainMenu::OnThink()
{
	BaseClass::OnThink();

	bool bMpOnly = GameUI().IsInMultiplayer();
	if ( IsVisible() && bMpOnly )
	{
		// Yield to generic message box if is present
		WINDOW_TYPE arrYield[] = { WT_GENERICCONFIRMATION };
		for ( int j = 0; j < ARRAYSIZE( arrYield ); ++ j )
		{
			CBaseModFrame *pYield = CBaseModPanel::GetSingleton().GetWindow( arrYield[j] );
			if ( pYield && pYield->IsVisible() && !pYield->HasFocus() )
			{
				pYield->Activate();
				pYield->RequestFocus();
			}
		}
	}
}

//=============================================================================
void MainMenu::OnOpen()
{
	BaseClass::OnOpen();
}

//=============================================================================
void MainMenu::OnClose()
{
	bool bMpOnly = GameUI().IsInMultiplayer();
	if ( bMpOnly )
		Unpause();

	// During shutdown this calls delete this, so Unpause should occur before this call
	BaseClass::OnClose();
}

//=============================================================================
void MainMenu::PerformLayout( void )
{
	BaseClass::PerformLayout();

	bool bMpOnly = GameUI().IsInLevel();

	CAvatarImagePanel *m_pProfileAvatar = dynamic_cast< CAvatarImagePanel *>( FindChildByName( "AvatarImage" ) );
	if ( m_pProfileAvatar )
	{
		m_pProfileAvatar->SetPlayer( m_SteamID, k_EAvatarSize64x64 );
		m_pProfileAvatar->SetShouldDrawFriendIcon( false );
	}

	char szNickName[64];
	V_strcpy_safe( szNickName, ( steamapicontext->SteamFriends() ) ? steamapicontext->SteamFriends()->GetPersonaName() : "Unknown" );
	SetDialogVariable( "playername", szNickName );

	CTFAdvButton *m_pQuitDisconnect = dynamic_cast< CTFAdvButton *>( FindChildByName( "QuitDisconnect" ) );
	if ( m_pQuitDisconnect )
	{
		m_pQuitDisconnect->SetCommandString( bMpOnly ? "ExitToMainMenu" : "QuitGame" );
		m_pQuitDisconnect->SetText( bMpOnly ? "#GameUI_GameMenu_Disconnect" : "#GameUI_GameMenu_Quit" );
		m_pQuitDisconnect->SetImage( bMpOnly ? "../vgui/glyph_disconnect" : "../vgui/glyph_quit" );
	}

	CTFAdvButton *m_pMute = dynamic_cast< CTFAdvButton *>( FindChildByName( "Mute" ) );
	if ( m_pMute )
		m_pMute->SetVisible( bMpOnly ? true : false );

	CTFAdvButton *m_pVote = dynamic_cast< CTFAdvButton *>( FindChildByName( "CallVote" ) );
	if ( m_pVote )
		m_pVote->SetVisible( bMpOnly ? true : false );
}

//=============================================================================
void MainMenu::OnGameUIHidden()
{
	bool bMpOnly = GameUI().IsInMultiplayer();
	if ( bMpOnly )
	{
		Unpause();
		Close();
	}
}

//=============================================================================
void MainMenu::RunFrame()
{
	BaseClass::RunFrame();
}

//=============================================================================
void MainMenu::ApplySchemeSettings( IScheme *pScheme )
{
	BaseClass::ApplySchemeSettings( pScheme );

	LoadControlSettings( "resource/UI/BaseModUI/MainMenu.res" );

	CExLabel *m_pVersionLabel = dynamic_cast<CExLabel *>( FindChildByName( "VersionLabel" ) );
	if ( m_pVersionLabel )
	{
		char verString[64];
		Q_snprintf( verString, sizeof( verString ), "Version: %s", GetNotificationManager()->GetVersionString() );
		m_pVersionLabel->SetText( verString );
	}
}

//=============================================================================
void MainMenu::SetServerlistSize( int size )
{
	if ( m_pServerlistPanel )
		m_pServerlistPanel->SetServerlistSize( size );
}

//=============================================================================
void MainMenu::UpdateServerInfo()
{
	if ( m_pServerlistPanel )
		m_pServerlistPanel->UpdateServerInfo();
}

//=============================================================================
void MainMenu::AcceptQuitGameCallback()
{
	if ( MainMenu *pMainMenu = static_cast< MainMenu* >( CBaseModPanel::GetSingleton().GetWindow( WT_MAINMENU ) ) )
		pMainMenu->OnCommand( "QuitGame_NoConfirm" );
}

//=============================================================================
void MainMenu::LeaveGameOkCallback()
{
	COM_TimestampedLog( "Exit Game" );

	if ( MainMenu *self = static_cast< MainMenu* >( CBaseModPanel::GetSingleton().GetWindow( WT_MAINMENU ) ) )
		self->Close();

	engine->ExecuteClientCmd( "gameui_hide" );

	// On PC people can be playing via console bypassing matchmaking
	// and required session settings, so to leave game duplicate
	// session closure with an extra "disconnect" command.
	engine->ExecuteClientCmd( "disconnect" );

	engine->ExecuteClientCmd( "gameui_activate" );

	CBaseModPanel::GetSingleton().CloseAllWindows();
	CBaseModPanel::GetSingleton().OpenFrontScreen();
}

//-----------------------------------------------------------------------------
// Purpose: Constructor
//-----------------------------------------------------------------------------
CServerlistPanel::CServerlistPanel( vgui::Panel* parent, const char *panelName ) : BaseClass( parent, panelName )
{
	m_iSize = 0;
	m_pServerList = new vgui::SectionedListPanel( this, "ServerList" );
	m_pConnectButton = new CTFAdvButton( this, "ConnectButton", "Connect" );
	m_pListSlider = new CTFAdvSlider( this, "ListSlider", "" );

	vgui::ivgui()->AddTickSignal( GetVPanel(), 1000 );	// used to bring us back from invisible
}

//-----------------------------------------------------------------------------
// Purpose: Destructor
//-----------------------------------------------------------------------------
CServerlistPanel::~CServerlistPanel()
{
}

void CServerlistPanel::ApplySchemeSettings( vgui::IScheme *pScheme )
{
	BaseClass::ApplySchemeSettings( pScheme );

	m_pServerList->RemoveAll();
	m_pServerList->RemoveAllSections();
	m_pServerList->SetSectionFgColor( 0, Color( 255, 255, 255, 255 ) );
	m_pServerList->SetBgColor( Color( 0, 0, 0, 0 ) );
	m_pServerList->SetBorder( NULL );
	m_pServerList->AddSection( 0, "Servers", ServerSortFunc );
	m_pServerList->AddColumnToSection( 0, "Name", "Servers", SectionedListPanel::COLUMN_BRIGHT, m_iServerWidth );
	m_pServerList->AddColumnToSection( 0, "Players", "Players", SectionedListPanel::COLUMN_BRIGHT, m_iPlayersWidth );
	m_pServerList->AddColumnToSection( 0, "Ping", "Ping", SectionedListPanel::COLUMN_BRIGHT, m_iPingWidth );
	m_pServerList->AddColumnToSection( 0, "Map", "Map", SectionedListPanel::COLUMN_BRIGHT, m_iMapWidth );
	m_pServerList->SetSectionAlwaysVisible( 0, true );
	m_pServerList->GetScrollBar()->UseImages( "", "", "", "" ); //hack to hide the scrollbar

	m_pConnectButton->SetVisible( false );
	UpdateServerInfo();
}

void CServerlistPanel::PerformLayout()
{
	BaseClass::PerformLayout();
}

void CServerlistPanel::OnThink()
{
	m_pServerList->ClearSelection();
	m_pListSlider->SetVisible( false );
	m_pConnectButton->SetVisible( false );

	if ( !IsCursorOver() )
		return;

	m_pListSlider->SetValue( m_pServerList->GetScrollBar()->GetValue() );

	for ( int i = 0; i < m_pServerList->GetItemCount(); i++ )
	{
		int _x, _y;
		m_pServerList->GetPos( _x, _y );
		int x, y, wide, tall;
		m_pServerList->GetItemBounds( i, x, y, wide, tall );
		int cx, cy;
		surface()->SurfaceGetCursorPos( cx, cy );
		m_pServerList->ScreenToLocal( cx, cy );

		if ( cx > x && cx < x + wide && cy > y && cy < y + tall )
		{
			m_pServerList->SetSelectedItem( i );
			int by = y + _y;
			m_pConnectButton->SetPos( m_iServerWidth + m_iPlayersWidth + m_iPingWidth, by );
			m_pConnectButton->SetVisible( true );
			m_pListSlider->SetVisible( true );

			char szCommand[128];
			Q_snprintf( szCommand, sizeof( szCommand ), "connect %s", m_pServerList->GetItemData( i )->GetString( "ServerIP", "" ) );
			m_pConnectButton->SetCommandString( szCommand );
		}
	}
}

void CServerlistPanel::OnCommand( const char* command )
{
	if ( !Q_strcmp( command, "scrolled" ) )
	{
		m_pServerList->GetScrollBar()->SetValue( m_pListSlider->GetValue() );
	}
	else
	{
		BaseClass::OnCommand( command );
	}
}

//-----------------------------------------------------------------------------
// Purpose: Used for sorting servers
//-----------------------------------------------------------------------------
bool CServerlistPanel::ServerSortFunc( vgui::SectionedListPanel *list, int itemID1, int itemID2 )
{
	KeyValues *it1 = list->GetItemData( itemID1 );
	KeyValues *it2 = list->GetItemData( itemID2 );
	Assert( it1 && it2 );

	int v1 = it1->GetInt( "CurPlayers" );
	int v2 = it2->GetInt( "CurPlayers" );
	if ( v1 > v2 )
		return true;
	else if ( v1 < v2 )
		return false;

	/*
	int iOff1 = it1->GetBool("Official");
	int iOff2 = it2->GetBool("Official");
	if (iOff1 && !iOff2)
		return true;
	else if (!iOff1 && iOff2)
		return false;
	*/

	int iPing1 = it1->GetInt( "Ping" );
	if ( iPing1 == 0 )
		return false;
	int iPing2 = it2->GetInt( "Ping" );
	return ( iPing1 < iPing2 );
}

void CServerlistPanel::SetServerlistSize( int size )
{
	m_iSize = size;
};

void CServerlistPanel::UpdateServerInfo()
{
	m_pServerList->RemoveAll();
	HFont Font = GETSCHEME()->GetFont( "FontStoreOriginalPrice", true );

	for ( int i = 0; i < m_iSize; i++ )
	{
		gameserveritem_t m_Server = GetNotificationManager()->GetServerInfo( i );
		if ( m_Server.m_steamID.GetAccountID() == 0 )
			continue;

		// Don't show passworded/locked servers.
		if ( m_Server.m_bPassword )
			continue;

		// Hide servers with zero human players.
		if ( ( m_Server.m_nPlayers - m_Server.m_nBotPlayers ) < 1 )
			continue;

		char szServerName[128];
		char szServerIP[32];
		char szServerPlayers[16];
		int szServerCurPlayers;
		int szServerPing;
		char szServerMap[32];

		Q_snprintf( szServerName, sizeof( szServerName ), "%s", m_Server.GetName() );
		Q_snprintf( szServerIP, sizeof( szServerIP ), "%s", m_Server.m_NetAdr.GetQueryAddressString() );
		Q_snprintf( szServerPlayers, sizeof( szServerPlayers ), "%i/%i", m_Server.m_nPlayers, m_Server.m_nMaxPlayers );
		szServerCurPlayers = m_Server.m_nPlayers - m_Server.m_nBotPlayers; // Current HUMAN Players.
		szServerPing = m_Server.m_nPing;
		Q_snprintf( szServerMap, sizeof( szServerMap ), "%s", m_Server.m_szMap );

		KeyValues *curitem = new KeyValues( "data" );

		curitem->SetString( "Name", szServerName );
		curitem->SetString( "ServerIP", szServerIP );
		curitem->SetString( "Players", szServerPlayers );
		curitem->SetInt( "Ping", szServerPing );
		curitem->SetInt( "CurPlayers", szServerCurPlayers );
		curitem->SetString( "Map", szServerMap );

		int itemID = m_pServerList->AddItem( 0, curitem );

		m_pServerList->SetItemFgColor( itemID, GETSCHEME()->GetColor( "AdvTextDefault", Color( 255, 255, 255, 255 ) ) );

		m_pServerList->SetItemFont( itemID, Font );
		curitem->deleteThis();
	}
	
#ifdef _DEBUG
	if ( m_pServerList->GetItemCount() < 1 )
	{
		// If we don't have any servers listed, make a dummy server for the debugger.
		KeyValues *curitemDEBUG = new KeyValues( "data" );

		curitemDEBUG->SetString( "Name", "DEBUG NAME" );
		curitemDEBUG->SetString( "ServerIP", "127.0.0.1:27015" );
		curitemDEBUG->SetString( "Players", "0/0" );
		curitemDEBUG->SetInt( "Ping", 000 );
		curitemDEBUG->SetInt( "CurPlayers", 0 );
		curitemDEBUG->SetString( "Map", "DEBUG MAP" );

		int itemID = m_pServerList->AddItem( 0, curitemDEBUG );

		m_pServerList->SetItemFgColor( itemID, GETSCHEME()->GetColor( "AdvTextDefault", Color( 255, 255, 255, 255 ) ) );

		m_pServerList->SetItemFont( itemID, Font );
		curitemDEBUG->deleteThis();
	}
	SetVisible( true );
#else
	if ( m_pServerList->GetItemCount() > 0 )
	{
		SetVisible( true );
	}
	else
	{
		SetVisible( false );
	}
#endif

	int min, max;
	m_pServerList->InvalidateLayout( 1, 0 );
	m_pServerList->GetScrollBar()->GetRange( min, max );
	m_pListSlider->SetRange( min, max - m_pServerList->GetScrollBar()->GetButton( 0 )->GetTall() * 4 );
}