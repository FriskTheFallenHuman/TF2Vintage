//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose:
//
//===========================================================================//

#include "cbase.h"
#include "VLoadingProgress.h"
#include "EngineInterface.h"
#include "vgui_controls/Label.h"
#include "vgui_controls/ProgressBar.h"
#include "vgui/ISurface.h"
#include "vgui/ILocalize.h"
#include "vgui_controls/Image.h"
#include "vgui_controls/ImagePanel.h"
#include "KeyValues.h"
#include "fmtstr.h"
#include "FileSystem.h"
#include "GameUI_Interface.h"
#include <vgui/IInput.h>
#include "FileSystem.h"
#include "time.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

using namespace vgui;
using namespace BaseModUI;

//=============================================================================
LoadingProgress::LoadingProgress(Panel *parent, const char *panelName, LoadingWindowType eLoadingType ) : BaseClass( parent, panelName, false, false, false ), m_LoadingWindowType( eLoadingType )
{
	if ( IsPC() && eLoadingType == LWT_LOADINGPLAQUE )
		MakePopup( false );

	SetDeleteSelfOnClose( true );
	SetProportional( true );

	m_flPeakProgress = 0.0f;
	m_pLoadingBar = NULL;

	m_pLoadingProgress = NULL;
	m_pCancelButton = NULL;

	m_pLoadingSpinner = NULL;
	m_LoadingType = LT_UNDEFINED;

	m_pBGImage = NULL;
	m_pFooter = NULL;

    // Listen for game events
	ListenForGameEvent ( "player_disconnect" );

	m_bDrawBackground = false;
	m_bDrawProgress = false;
	m_bDrawSpinner = false;

	m_flLastEngineTime = 0;

	// marked to indicate the controls exist
	m_bValid = false;

	MEM_ALLOC_CREDIT();

	m_pTipPanel = new CLoadingTipPanel( this );
}

//=============================================================================
LoadingProgress::~LoadingProgress()
{
}

//=============================================================================
void LoadingProgress::OnThink()
{
	UpdateLoadingSpinner();
}

//=============================================================================
void LoadingProgress::OnCommand( const char *command )
{
	if ( !stricmp( command, "cancel" ) )
	{
		// disconnect from the server
		engine->ClientCmd_Unrestricted( "disconnect\n" );

		// close
		Close();
	}
	else
	{
		BaseClass::OnCommand( command );
	}
}

//=============================================================================
void LoadingProgress::FireGameEvent( IGameEvent* event )
{
	if ( !Q_strcmp( event->GetName(), "player_disconnect" ) )
	{
		Close();
	}
}

//=============================================================================
void LoadingProgress::ApplySchemeSettings( IScheme *pScheme )
{
	// will cause the controls to be instanced
	BaseClass::ApplySchemeSettings( pScheme );

	LoadControlSettings( "Resource/UI/BaseModUI/LoadingProgress.res" );

	SetPaintBackgroundEnabled( true );
	
	// now have controls, can now do further initing
	m_bValid = true;

	m_pLoadingBar = dynamic_cast< vgui::ProgressBar* >( FindChildByName( "LoadingBar" ) );
	if ( m_pLoadingBar )
	{
		m_pLoadingBar->SetBgColor( pScheme->GetColor( "IfmMenuDark", Color( 0, 0, 0, 0 ) ) );
		m_pLoadingBar->SetPaintBorderEnabled( false );
	}

	SetupControlStates();
}

//=============================================================================
void LoadingProgress::Close()
{
	if ( m_pBGImage )
		m_pBGImage->EvictImage();

	BaseClass::Close();
}

//=============================================================================
void LoadingProgress::Activate()
{
	BaseClass::Activate();
}

//=============================================================================
// this is where the spinner gets updated.
void LoadingProgress::UpdateLoadingSpinner()
{
	if ( m_pLoadingSpinner && m_bDrawSpinner )
	{
		// clock the anim at 10hz
		float time = Plat_FloatTime();
		if ( ( m_flLastEngineTime + 0.1f ) < time )
		{
			m_flLastEngineTime = time;
			m_pLoadingSpinner->SetFrame( m_pLoadingSpinner->GetFrame() + 1 );
		}
	}
}

//=============================================================================
void LoadingProgress::SetProgress( float progress )
{
	if ( m_pLoadingBar && m_bDrawProgress )
	{
		if ( progress > m_flPeakProgress )
			m_flPeakProgress = progress;

		m_pLoadingBar->SetProgress( m_flPeakProgress );
	}

	UpdateLoadingSpinner();
}

//=============================================================================
float LoadingProgress::GetProgress()
{
	float retVal = -1.0f;

	if ( m_pLoadingBar )
		retVal = m_pLoadingBar->GetProgress();

	return retVal;
}

//=============================================================================
void LoadingProgress::PaintBackground()
{
	int screenWide, screenTall;
	surface()->GetScreenSize( screenWide, screenTall );

	if ( m_bDrawBackground && m_pBGImage )
	{
		int x, y, wide, tall;
		m_pBGImage->GetBounds( x, y, wide, tall );
		surface()->DrawSetColor( Color( 255, 255, 255, 255 ) );
		surface()->DrawSetTexture( m_pBGImage->GetImage()->GetID() );
		surface()->DrawTexturedRect( x, y, x+wide, y+tall );
	}

	if ( m_pFooter )
	{
		int screenWidth, screenHeight;
		CBaseModPanel::GetSingleton().GetSize( screenWidth, screenHeight );

		int x, y, wide, tall;
		m_pFooter->GetBounds( x, y, wide, tall );
		surface()->DrawSetColor( m_pFooter->GetBgColor() );
		surface()->DrawFilledRect( 0, y, x+screenWidth, y+tall );
	}

	// this is where the spinner draws
	bool bRenderSpinner = ( m_bDrawSpinner && m_pLoadingSpinner );
	if ( bRenderSpinner )
	{
		int x, y, wide, tall;

		wide = tall = scheme()->GetProportionalScaledValue( 45 );
		x = scheme()->GetProportionalScaledValue( 45 ) - wide/2;
		y = screenTall - scheme()->GetProportionalScaledValue( 32 ) - tall/2;

		m_pLoadingSpinner->GetImage()->SetFrame( m_pLoadingSpinner->GetFrame() );

		surface()->DrawSetColor( Color( 255, 255, 255, 255 ) );
		surface()->DrawSetTexture( m_pLoadingSpinner->GetImage()->GetID() );
		surface()->DrawTexturedRect( x, y, x+wide, y+tall );
	}

	if ( m_bDrawProgress && m_pLoadingBar )
	{
		int x, y, wide, tall;
		m_pLoadingBar->GetBounds( x, y, wide, tall );
	}
}

//=============================================================================
void LoadingProgress::OnKeyCodeTyped( KeyCode code )
{
	if ( code == KEY_ESCAPE	)
		OnCommand( "cancel" );
	else
		BaseClass::OnKeyCodeTyped( code );
}


//=============================================================================
// Must be called first. Establishes the loading style
//=============================================================================
void LoadingProgress::SetLoadingType( LoadingProgress::LoadingType loadingType )
{
	m_LoadingType = loadingType;

	// the first time initing occurs during ApplySchemeSettings() or if the panel is deleted
	// if the panel is re-used, this is for the second time the panel gets used
	SetupControlStates();
}

//=============================================================================
LoadingProgress::LoadingType LoadingProgress::GetLoadingType()
{
	return m_LoadingType;
}

//=============================================================================
void LoadingProgress::SetupControlStates()
{
	m_flPeakProgress = 0.0f;

	// haven't been functionally initialized yet
	// can't set or query control states until they get established
	if ( !m_bValid )
		return;

	m_bDrawBackground = false;
	m_bDrawSpinner = false;
	m_bDrawProgress = false;

	switch( m_LoadingType )
	{
	case LT_MAINMENU:
		m_bDrawBackground = true;
		m_bDrawProgress = true;
		m_bDrawSpinner = false;
		break;
	case LT_TRANSITION:
		m_bDrawBackground = true;
		m_bDrawProgress = true;
		m_bDrawSpinner = false;
		break;
	default:
		break;
	}

	m_pLoadingProgress = dynamic_cast< vgui::Label* >( FindChildByName( "LoadingProgressText" ) );
#ifdef TF_VINTAGE_CLIENT
	m_pCancelButton = dynamic_cast< CExMenuButton* >( FindChildByName( "Cancel" ) );
#else
	m_pCancelButton = dynamic_cast< vgui::Button* >( FindChildByName( "Cancel" ) );
#endif
	m_pFooter = dynamic_cast< vgui::Panel* >( FindChildByName( "Footer" ) );

	m_pBGImage = dynamic_cast< vgui::ImagePanel* >( FindChildByName( "Background" ) );
	if ( m_pBGImage )
	{
		// set the correct background image
		int screenWide, screenTall;
		surface()->GetScreenSize( screenWide, screenTall );

		char szBGName[MAX_PATH];
		engine->GetMainMenuBackgroundName( szBGName, sizeof( szBGName ) );
		char szImage[MAX_PATH];
		Q_snprintf( szImage, sizeof( szImage ), "../console/%s", szBGName );

		float aspectRatio = (float)screenWide/(float)screenTall;
		bool bIsWidescreen = aspectRatio >= 1.5999f;
		if ( bIsWidescreen )
			Q_strcat( szImage, "_widescreen", sizeof( szImage ) );

		m_pBGImage->SetImage( szImage );

		// we will custom draw
		m_pBGImage->SetVisible( false );
	}

	// we will custom draw
	m_pLoadingBar = dynamic_cast< vgui::ProgressBar* >( FindChildByName( "LoadingBar" ) );
	if ( m_pLoadingBar )
		m_pLoadingBar->SetScheme( "SourceScheme" );

	// we will custom draw
	m_pLoadingSpinner = dynamic_cast< vgui::ImagePanel* >( FindChildByName( "LoadingSpinner" ) );
	if ( m_pLoadingSpinner )
		m_pLoadingSpinner->SetVisible( false );

	// we will custom draw
	vgui::Label *pLoadingLabel = dynamic_cast< vgui::Label *>( FindChildByName( "LoadingText" ) );
	if ( pLoadingLabel )
		pLoadingLabel->SetVisible( m_bDrawProgress );

	// Hold on to start frame slightly
	m_flLastEngineTime = Plat_FloatTime() + 0.2f;
}

//=============================================================================
void LoadingProgress::SetStatusText( const char *statusText )
{
	if ( m_pLoadingProgress )
		m_pLoadingProgress->SetText( statusText );
}