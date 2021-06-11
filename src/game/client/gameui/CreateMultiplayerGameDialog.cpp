//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose: 
//
// $NoKeywords: $
//=============================================================================//

#include "CreateMultiplayerGameDialog.h"
#include "CreateMultiplayerGameServerPage.h"
#include "CreateMultiplayerGameGameplayPage.h"

#include "EngineInterface.h"
#include "ModInfo.h"
#include "GameUI_Interface.h"
#include <stdio.h>
#include "vgui_controls/ComboBox.h"
#include "vgui_controls/PropertySheet.h"
#include <vgui/ILocalize.h>
#include "filesystem.h"
#include <KeyValues.h>

// memdbgon must be the last include file in a .cpp file!!!
#include <tier0/memdbgon.h>

using namespace vgui;

//-----------------------------------------------------------------------------
// Purpose: Constructor
//-----------------------------------------------------------------------------
CCreateMultiplayerGameDialog::CCreateMultiplayerGameDialog( vgui::Panel *parent ) : PropertyDialog( parent, "CreateMultiplayerGameDialog" )
{
	SetProportional( true );
	SetMoveable( false );
	SetSizeable( false );
	SetDeleteSelfOnClose( true );
	SetKeyBoardInputEnabled( true );
	SetMouseInputEnabled( true );
	
	SetTitle( "", false );

	m_pServerPage = new CCreateMultiplayerGameServerPage( this, "ServerPage" );
	m_pGameplayPage = new CCreateMultiplayerGameGameplayPage( this, "GameplayPage" );

	// create KeyValues object to load/save config options
	m_pSavedData = new KeyValues( "ServerConfig" );

	// load the config data
	if ( m_pSavedData )
	{
		m_pSavedData->LoadFromFile( g_pFullFileSystem, "ServerConfig.vdf", "GAME" ); // this is game-specific data, so it should live in GAME, not CONFIG
		const char *startMap = m_pSavedData->GetString( "map", "" );
		if ( startMap[0] )
			m_pServerPage->SetMap( startMap );
	}
}

//-----------------------------------------------------------------------------
// Purpose: Destructor
//-----------------------------------------------------------------------------
CCreateMultiplayerGameDialog::~CCreateMultiplayerGameDialog()
{
	if ( m_pSavedData )
	{
		m_pSavedData->deleteThis();
		m_pSavedData = NULL;
	}
}

//-----------------------------------------------------------------------------
// Purpose:
//-----------------------------------------------------------------------------
void CCreateMultiplayerGameDialog::SetupTabs()
{
	AddPage( m_pServerPage, "#GameUI_Server" );
	AddPage( m_pGameplayPage, "#GameUI_Game" );
}

//-----------------------------------------------------------------------------
// Purpose: runs the server when the OK button is pressed
//-----------------------------------------------------------------------------
bool CCreateMultiplayerGameDialog::OnOK( bool applyOnly )
{
	// reset server enforced cvars
	g_pCVar->RevertFlaggedConVars( FCVAR_REPLICATED );	

	// Cheats were disabled; revert all cheat cvars to their default values.
	// This must be done heading into multiplayer games because people can play
	// demos etc and set cheat cvars with sv_cheats 0.
	g_pCVar->RevertFlaggedConVars( FCVAR_CHEAT );

	DevMsg( "FCVAR_CHEAT cvars reverted to defaults.\n" );

	BaseClass::OnOK( applyOnly );

	// get these values from m_pServerPage and store them temporarily
	char szMapName[64], szHostName[64], szPassword[64];
	Q_strncpy( szMapName, m_pServerPage->GetMapName(), sizeof( szMapName ) );
	Q_strncpy( szHostName, m_pGameplayPage->GetHostName(), sizeof( szHostName ) );
	Q_strncpy( szPassword, m_pGameplayPage->GetPassword(), sizeof( szPassword ) );

	// save the config data
	if ( m_pSavedData )
	{
		if ( m_pServerPage->IsRandomMapSelected() )
			m_pSavedData->SetString( "map", "" );	// it's set to random map, just save an
		else
			m_pSavedData->SetString( "map", szMapName );

		// save config to a file
		m_pSavedData->SaveToFile( g_pFullFileSystem, "ServerConfig.vdf", "GAME" );
	}

	char szMapCommand[1024];

	// create the command to execute
	Q_snprintf( szMapCommand, sizeof( szMapCommand ), "disconnect\nwait\nwait\nsetmaster enable\nmaxplayers %i\nsv_password \"%s\"\nhostname \"%s\"\nprogress_enable\nmap %s\n",
		m_pGameplayPage->GetMaxPlayers(),
		szPassword,
		szHostName,
		szMapName
	);

	// exec
	engine->ClientCmd_Unrestricted( szMapCommand );

	return true;
}

//-----------------------------------------------------------------------------
// Purpose: Sets up the sheet
//-----------------------------------------------------------------------------
void CCreateMultiplayerGameDialog::PerformLayout()
{
	// Skip PropertyDialog baseclass has we do it ourselves 
	Frame::PerformLayout();

	int iBottom = m_iSheetInsetBottom;
	if ( IsProportional() )
		iBottom = scheme()->GetProportionalScaledValueEx( GetScheme(), iBottom );

	int x, y, wide, tall;
	GetClientArea( x, y, wide, tall );
	GetPropertySheet()->SetBounds( x, y, wide, tall - iBottom );

	GetPropertySheet()->InvalidateLayout(); // tell the propertysheet to redraw!
	Repaint();
}

//-----------------------------------------------------------------------------
// Purpose:
//-----------------------------------------------------------------------------
void CCreateMultiplayerGameDialog::ApplySchemeSettings( vgui::IScheme *pScheme )
{
	BaseClass::ApplySchemeSettings( pScheme );

	LoadControlSettings( "Resource/CreateMultiplayerGameDialog.res" );

	SetupTabs();
}