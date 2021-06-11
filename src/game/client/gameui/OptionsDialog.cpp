//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose: 
//
// $NoKeywords: $
//=============================================================================//

#include "BasePanel.h"
#include "OptionsDialog.h"

#include "vgui_controls/Button.h"
#include "vgui_controls/CheckButton.h"
#include "vgui_controls/PropertySheet.h"
#include "vgui_controls/Label.h"
#include "vgui_controls/QueryBox.h"

#include "vgui/ILocalize.h"
#include "vgui/ISurface.h"
#include "vgui/ISystem.h"
#include "vgui/IVGui.h"

#include "KeyValues.h"
#include "OptionsSubKeyboard.h"
#include "OptionsSubMouse.h"
#include "OptionsSubAudio.h"
#include "OptionsSubVideo.h"
#include "OptionsSubVoice.h"
#include "OptionsSubMultiplayer.h"
#include "ModInfo.h"

using namespace vgui;

// memdbgon must be the last include file in a .cpp file!!!
#include <tier0/memdbgon.h>

//-----------------------------------------------------------------------------
// Purpose: Basic help dialog
//-----------------------------------------------------------------------------
COptionsDialog::COptionsDialog( vgui::Panel *parent ) : BaseClass( parent, "OptionsDialog" )
{
	SetProportional( true );
	SetMoveable( false );
	SetSizeable( false );
	SetDeleteSelfOnClose( true );
	SetKeyBoardInputEnabled( true );
	SetMouseInputEnabled( true );

	SetTitle( "", false );

	m_pOptionsSubAudio = new COptionsSubAudio( this );
	m_pOptionsSubVideo = new COptionsSubVideo( this );
}

//-----------------------------------------------------------------------------
// Purpose: Destructor
//-----------------------------------------------------------------------------
COptionsDialog::~COptionsDialog()
{
}

//-----------------------------------------------------------------------------
// Purpose:
//-----------------------------------------------------------------------------
void COptionsDialog::SetupTabs()
{
	AddPage( new COptionsSubKeyboard( this ), "#GameUI_Keyboard" );
	AddPage( new COptionsSubMouse( this ), "#GameUI_Mouse" );
	AddPage( m_pOptionsSubAudio, "#GameUI_Audio" );
	AddPage( m_pOptionsSubVideo, "#GameUI_Video" );
	AddPage( new COptionsSubVoice( this ), "#GameUI_Voice" );
	AddPage( new COptionsSubMultiplayer( this ), "#GameUI_Multiplayer" );
}

//-----------------------------------------------------------------------------
// Purpose: Brings the dialog to the fore
//-----------------------------------------------------------------------------
void COptionsDialog::Activate()
{
	BaseClass::Activate();
	EnableApplyButton( false );
}

//-----------------------------------------------------------------------------
// Purpose: Opens the dialog
//-----------------------------------------------------------------------------
void COptionsDialog::Run()
{
	Activate();
}

//-----------------------------------------------------------------------------
// Purpose: Sets up the sheet
//-----------------------------------------------------------------------------
void COptionsDialog::PerformLayout()
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
// Purpose: Called when the GameUI is hidden
//-----------------------------------------------------------------------------
void COptionsDialog::OnGameUIHidden()
{
	// tell our children about it
	for ( int i = 0 ; i < GetChildCount() ; i++ )
	{
		Panel *pChild = GetChild( i );
		if ( pChild )
			PostMessage( pChild, new KeyValues( "GameUIHidden" ) );
	}
}

//-----------------------------------------------------------------------------
// Purpose:
//-----------------------------------------------------------------------------
void COptionsDialog::ApplySchemeSettings( vgui::IScheme *pScheme )
{
	BaseClass::ApplySchemeSettings( pScheme );

	LoadControlSettings( "Resource/OptionsDialog.res" );

	SetupTabs();
}