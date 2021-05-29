//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose: Tip display during level loads.
//
//===========================================================================//

#include "cbase.h"
#include "LoadingTipPanel.h"
#include "vgui/isurface.h"
#include "EngineInterface.h"
#ifdef TF_VINTAGE_CLIENT
#include "tf_tips.h"
#endif

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

using namespace vgui;

//--------------------------------------------------------------------------------------------------------
CLoadingTipPanel::CLoadingTipPanel( Panel *pParent ) : EditablePanel( pParent, "LoadingTipPanel" )
{
	SetupTips();
}

//--------------------------------------------------------------------------------------------------------
void CLoadingTipPanel::ApplySchemeSettings( vgui::IScheme *pScheme )
{
	BaseClass::ApplySchemeSettings( pScheme );

	ReloadScheme();
}

//--------------------------------------------------------------------------------------------------------
void CLoadingTipPanel::ReloadScheme( void )
{
	LoadControlSettings( "Resource/UI/LoadingTipPanel.res" );

	NextTip();
}

//--------------------------------------------------------------------------------------------------------
void CLoadingTipPanel::SetupTips( void )
{
	NextTip();
}

//--------------------------------------------------------------------------------------------------------
void CLoadingTipPanel::NextTip( void )
{
	if ( !IsEnabled() )
		return;

#ifdef TF_VINTAGE_CLIENT
	int iClass = TF_CLASS_UNDEFINED;
	SetDialogVariable( "TipText", g_TFTips.GetRandomTip( iClass ) );
#endif

	// Set our control visible
	SetVisible( true );
}