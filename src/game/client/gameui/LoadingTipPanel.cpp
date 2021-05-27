//===== Copyright © 1996-2008, Valve Corporation, All rights reserved. ======//
//
// Purpose: Tip display during level loads.
//
//===========================================================================//

#include "LoadingTipPanel.h"
#include "vgui/isurface.h"
#include "EngineInterface.h"
#ifdef VECTRONIC_DLL
#include "tip_manager.h"
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

#ifdef VECTRONIC_DLL
	SetDialogVariable( "TipText", g_TipManager.GetRandomTip() );
#endif

	// Set our control visible
	SetVisible( true );
}