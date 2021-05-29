//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose: 
//
// $NoKeywords: $
//
//=============================================================================//

#include "CreateMultiplayerGameBotPage.h"

using namespace vgui;

#include <KeyValues.h>
#include <vgui_controls/ComboBox.h>
#include <vgui_controls/CheckButton.h>
#include <vgui_controls/Label.h>
#include <vgui_controls/TextEntry.h>

#include "filesystem.h"
#include "PanelListPanel.h"
#include "scriptobject.h"
#include <tier0/vcrmode.h>
#include "tier1/convar.h"
#include "EngineInterface.h"
#include "CvarToggleCheckButton.h"

// memdbgon must be the last include file in a .cpp file!!!
#include <tier0/memdbgon.h>

//-----------------------------------------------------------------------------
// Constructor
//-----------------------------------------------------------------------------
CCreateMultiplayerGameBotPage::CCreateMultiplayerGameBotPage( vgui::Panel *parent, const char *name, KeyValues *botKeys ) : PropertyPage( parent, name )
{
	m_pSavedData = botKeys;

	m_TFBotjoinAfterPlayer = new CCvarToggleCheckButton( this, "BotJoinAfterPlayerCheck", "", "tf_bot_join_after_player" );
	m_TFBotAutoVacate = new CCvarToggleCheckButton( this, "BotAutoVacateCheck", "", "tf_bot_auto_vacate" );
	m_TFBotMeleeOnly = new CCvarToggleCheckButton( this, "BotMeleeOnlyCheck", "", "tf_bot_melee_only" );
	m_TFBotPrefixDifficulty = new CCvarToggleCheckButton( this, "BotPrefixDifficulty", "", "tf_bot_prefix_name_with_difficulty" );
	m_TFBotKeepClass = new CCvarToggleCheckButton( this, "BotKeepClassCheck", "", "tf_bot_keep_class_after_death" );
	m_TFBotOfflinePratice = new CCvarToggleCheckButton( this, "BotOfflinePraticeCheck", "", "tf_bot_offline_practice" );
	m_TFBotUseItems = new CCvarToggleCheckButton( this, "BotUseItemsCheck", "", "tf_bot_use_items" );
	m_TFBotEconRandom = new CCvarToggleCheckButton( this, "BotEconRandomCheck", "", "tf_bot_random_loadouts" );
	m_TFBotKeepEcon = new CCvarToggleCheckButton( this, "BotKeepEconCheck", "", "tf_bot_keep_items_after_death" );

	// create text entry fields for quota, prefix and class
	m_TFBotQuotaMode = new TextEntry( this, "BotQuotaModeEntry" );
	m_TFBotClass = new TextEntry( this, "BotClassEntry" );

	// set positions and sizes from resources file
	LoadControlSettings( "Resource/CreateMultiplayerGameBotPage.res" );

	// get initial values from bot keys
	m_TFBotjoinAfterPlayer->SetSelected( botKeys->GetInt( "tf_bot_join_after_player", 1 ) );
	m_TFBotAutoVacate->SetSelected( botKeys->GetInt( "tf_bot_auto_vacate", 1 ) );
	m_TFBotUseItems->SetSelected( botKeys->GetInt( "tf_bot_use_items", 1 ) );

	// set tf_bot_quota_mode
	const char *quota_mode = botKeys->GetString( "tf_bot_quota_mode" );
	if ( quota_mode )
		SetControlString( "BotQuotaModeEntry", quota_mode );

	// set tf_bot_force_class
	const char *tf_class = botKeys->GetString( "tf_bot_force_class" );
	if ( tf_class )
		SetControlString( "BotClassEntry", tf_class );
}

//-----------------------------------------------------------------------------
// Destructor
//-----------------------------------------------------------------------------
CCreateMultiplayerGameBotPage::~CCreateMultiplayerGameBotPage()
{
	// vgui handles deletion of children automatically through the hierarchy
}

//-----------------------------------------------------------------------------
// Reset values
//-----------------------------------------------------------------------------
void CCreateMultiplayerGameBotPage::OnResetChanges()
{
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
void UpdateValue( KeyValues *data, const char *cvarName, int value )
{
	data->SetInt( cvarName, value );

	ConVarRef var( cvarName );
	var.SetValue( value );
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
void UpdateValue( KeyValues *data, const char *cvarName, const char *value )
{
	data->SetString( cvarName, value );

	ConVarRef var( cvarName );
	var.SetValue( value );
}

//-----------------------------------------------------------------------------
// Called to get data from the page
//-----------------------------------------------------------------------------
void CCreateMultiplayerGameBotPage::OnApplyChanges()
{
	UpdateValue( m_pSavedData, "tf_bot_join_after_player", m_TFBotjoinAfterPlayer->IsSelected() );
	UpdateValue( m_pSavedData, "tf_bot_auto_vacate", m_TFBotAutoVacate->IsSelected() );
	UpdateValue( m_pSavedData, "tf_bot_melee_only", m_TFBotMeleeOnly->IsSelected() );
	UpdateValue( m_pSavedData, "tf_bot_prefix_name_with_difficulty", m_TFBotPrefixDifficulty->IsSelected() );
	UpdateValue( m_pSavedData, "tf_bot_keep_class_after_death", m_TFBotKeepClass->IsSelected() );
	UpdateValue( m_pSavedData, "tf_bot_offline_practice", m_TFBotOfflinePratice->IsSelected() );
	UpdateValue( m_pSavedData, "tf_bot_use_items", m_TFBotUseItems->IsSelected() );
	UpdateValue( m_pSavedData, "tf_bot_random_loadouts", m_TFBotEconRandom->IsSelected() );
	UpdateValue( m_pSavedData, "tf_bot_keep_items_after_death", m_TFBotKeepEcon->IsSelected() );

	// set tf_bot_quota_mode
	#define BUF_LENGTH 256
	char entryBuffer[ BUF_LENGTH ];
	m_TFBotQuotaMode->GetText( entryBuffer, BUF_LENGTH );
	UpdateValue( m_pSavedData, "tf_bot_quota_mode", entryBuffer );

	// set tf_bot_force_class
	m_TFBotClass->GetText( entryBuffer, BUF_LENGTH );
	UpdateValue( m_pSavedData, "tf_bot_force_class", entryBuffer );
}