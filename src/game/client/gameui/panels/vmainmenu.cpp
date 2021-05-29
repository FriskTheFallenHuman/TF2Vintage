//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose:
//
//===========================================================================//

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
#include "GameUI_Interface.h"

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
	bool bMpOnly = GameUI().IsInMultiplayer();

	if ( bMpOnly && !Q_strcmp( command, "ReturnToGame" ) )
	{
		engine->ClientCmd( "gameui_hide" );
	}
	else if( bMpOnly && !Q_strcmp( command, "MutePlayer" ) )
	{
		CBaseModPanel::GetSingleton().OpenPlayerListDialog( this );
	}
	else if( bMpOnly && !Q_strcmp( command, "ExitToMainMenu" ) )
	{
		MakeGenericDialog( "#GameUI_Disconnect", 
						   "#GameUI_DisconnectConfirmationText", 
						   true, 
						   &LeaveGameOkCallback,
						   true,
						   this );
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