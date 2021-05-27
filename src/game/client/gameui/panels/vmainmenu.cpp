//========= Copyright  1996-2008, Valve Corporation, All rights reserved. ============//
//
// Purpose: 
//
//=====================================================================================//

#include "cbase.h"
#include "vmainmenu.h"
#include "EngineInterface.h"
#include "vhybridbutton.h"
#include "vflyoutmenu.h"
#include "vgenericconfirmation.h"
#include "basemodpanel.h"
#include "uigamedata.h"

#include "vgui/ILocalize.h"
#include "vgui_controls/Label.h"
#include "vgui_controls/Button.h"
#include "vgui_controls/Tooltip.h"
#include "vgui_controls/ImagePanel.h"
#include "vgui_controls/Image.h"

#include "materialsystem/materialsystem_config.h"

#include "ienginevgui.h"
#include "basepanel.h"
#include "vgui/ISurface.h"
#include "tier0/icommandline.h"
#include "fmtstr.h"

#include "FileSystem.h"

#include "time.h"

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

	m_pLogoImage = NULL;

	AddFrameListener( this );

	SetDeleteSelfOnClose( true );

	m_pSteamDetails = new CSteamDetailsPanel( this );
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

	bool bOpeningFlyout = false;

	if ( !Q_strcmp( command, "Achievements" ) )
	{
		CBaseModPanel::GetSingleton().OpenWindow( WT_ACHIEVEMENTS, this, true );
	}
	else if ( !Q_strcmp( command, "QuitGame" ) )
	{
		MakeGenericDialog( "#GameUI_QuitConfirmationTitle", 
						   "#GameUI_QuitConfirmationText", 
						   true, 
						   &AcceptQuitGameCallback, 
						   true,
						   this );
	}
	else if ( !Q_stricmp( command, "QuitGame_NoConfirm" ) )
	{
		engine->ClientCmd( "quit" );
	}
	else if ( !Q_strcmp( command, "GameOptions" ) )
	{
		CBaseModPanel::GetSingleton().OpenOptionsDialog( this );
	}
	else if ( !Q_strcmp( command, "Audio" ) )
	{
		CBaseModPanel::GetSingleton().OpenWindow( WT_AUDIO, this, true );
	}
	else if ( !Q_strcmp( command, "Video" ) )
	{
		CBaseModPanel::GetSingleton().OpenWindow( WT_VIDEO, this, true );
	}
	else if ( !Q_strcmp( command, "Keyboard" ) )
	{
		CBaseModPanel::GetSingleton().OpenWindow( WT_KEYBOARD, this, true );
	}
	else if ( !Q_strcmp( command, "Mouse" ) )
	{
		CBaseModPanel::GetSingleton().OpenWindow( WT_MOUSE, this, true );
	}
	else if ( !Q_strcmp( command, "ServerBrowser" ) )
	{
		CBaseModPanel::GetSingleton().OpenServerBrowser();
	}
	else if( !Q_strcmp( command, "CreateGame" ) )
	{
		CBaseModPanel::GetSingleton().OpenWindow( WT_CREATEGAME, this, true );
	}
	else if ( !Q_strcmp( command, "NewGame" ) )
	{
		CBaseModPanel::GetSingleton().OpenWindow( WT_NEWGAME, this, true );
	}
	else if( !Q_strcmp( command, "LoadGame" ) )
	{
		CBaseModPanel::GetSingleton().OpenWindow( WT_LOADGAME, this, true );
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
		const char *SteamUrl = strstr( command, "steam " ) + strlen( "steam " );
		if ( strlen( SteamUrl ) > 0 )
			steamapicontext->SteamFriends()->ActivateGameOverlay( const_cast<char *>( SteamUrl ) );
	}
	else
	{
		// does this command match a flyout menu?
		BaseModUI::FlyoutMenu *flyout = dynamic_cast< FlyoutMenu* >( FindChildByName( command ) );
		if ( flyout )
		{
			bOpeningFlyout = true;

			// If so, enumerate the buttons on the menu and find the button that issues this command.
			// (No other way to determine which button got pressed; no notion of "current" button on PC.)
			for ( int iChild = 0; iChild < GetChildCount(); iChild++ )
			{
				bool bFound = false;

				if ( !bFound )
				{
					BaseModHybridButton *hybrid = dynamic_cast<BaseModHybridButton *>( GetChild( iChild ) );
					if ( hybrid && hybrid->GetCommand() && !Q_strcmp( hybrid->GetCommand()->GetString( "command" ), command ) )
					{
						hybrid->NavigateFrom();
						// open the menu next to the button that got clicked
						flyout->OpenMenu( hybrid );
						flyout->SetListener( this );
						break;
					}
				}
			}
		}
		else
		{
			BaseClass::OnCommand( command );
		}
	}

	if( !bOpeningFlyout )
		FlyoutMenu::CloseActiveMenu(); //due to unpredictability of mouse navigation over keyboard, we should just close any flyouts that may still be open anywhere.
}

//=============================================================================
void MainMenu::OnKeyCodePressed( KeyCode code )
{
	int userId = GetJoystickForCode( code );
	BaseModUI::CBaseModPanel::GetSingleton().SetLastActiveUserId( userId );

	switch( GetBaseButtonCode( code ) )
	{
	case KEY_XBUTTON_B:
		// Capture the B key so it doesn't play the cancel sound effect
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

	ConVarRef pDXLevel( "mat_dxlevel" );
	if( pDXLevel.GetInt() < 90 )
	{
		MakeGenericDialog( "#GameUI_Unsupported_DXLevel", 
						   "#GameUI_Unsupported_DXLevel_Msg", 
						   true, 
						   nullptr, 
						   false,
						   this );
	}
}

//=============================================================================
void MainMenu::OnOpen()
{
	BaseClass::OnOpen();
}

//=============================================================================
void MainMenu::PaintBackground() 
{
	vgui::Panel *m_pFooter = FindChildByName( "PnlBackground" );
	if ( m_pFooter )
	{
		int screenWidth, screenHeight;
		CBaseModPanel::GetSingleton().GetSize( screenWidth, screenHeight );

		int x, y, wide, tall;
		m_pFooter->GetBounds( x, y, wide, tall );
		surface()->DrawSetColor( m_pFooter->GetBgColor() );
		surface()->DrawFilledRect( 0, y, x+screenWidth, y+tall );	
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

	m_pLogoImage = dynamic_cast< vgui::ImagePanel* >( FindChildByName( "GameLogo" ) );

	LoadControlSettings( "Resource/UI/BaseModUI/MainMenu.res" );
}

//=============================================================================
void MainMenu::AcceptQuitGameCallback()
{
	if ( MainMenu *pMainMenu = static_cast< MainMenu* >( CBaseModPanel::GetSingleton().GetWindow( WT_MAINMENU ) ) )
		pMainMenu->OnCommand( "QuitGame_NoConfirm" );
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
CSteamDetailsPanel::CSteamDetailsPanel( Panel *pParent ) : EditablePanel( pParent, "SteamDetailsPanel" )
{
	if ( steamapicontext->SteamUser() )
		m_SteamID = steamapicontext->SteamUser()->GetSteamID();

	m_pProfileAvatar = NULL;

	vgui::ivgui()->AddTickSignal( GetVPanel(), 100 );
}

//=============================================================================
void CSteamDetailsPanel::ApplySchemeSettings( vgui::IScheme *pScheme )
{
	BaseClass::ApplySchemeSettings( pScheme );

	LoadControlSettings( "Resource/UI/SteamDetailsPanel.res" );

	m_pProfileAvatar = dynamic_cast<CAvatarImagePanel *>( FindChildByName( "AvatarImage" ) );
}

//=============================================================================
void CSteamDetailsPanel::PerformLayout()
{
	BaseClass::PerformLayout();

	if ( m_pProfileAvatar )
	{
		m_pProfileAvatar->SetPlayer( m_SteamID, k_EAvatarSize64x64 );
		m_pProfileAvatar->SetShouldDrawFriendIcon( false );
	}

	char szSteamUser[64];
	Q_snprintf( szSteamUser, sizeof( szSteamUser ),
		( steamapicontext->SteamFriends() ? steamapicontext->SteamFriends()->GetPersonaName() : "Unknown" ) );
	SetDialogVariable( "steamusername", szSteamUser );
};