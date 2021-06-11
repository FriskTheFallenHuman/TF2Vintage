//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose: Display a list of achievements for the current game
//
//=============================================================================//

#include "achievementsdialog.h"
#include "vgui_controls/Button.h"
#include "vgui/ILocalize.h"
#include "ixboxsystem.h"
#include "iachievementmgr.h"
#include "EngineInterface.h"
#include "GameUI_Interface.h"
#include "MouseMessageForwardingPanel.h"
#include "filesystem.h"
#include "vgui_controls/ImagePanel.h"
#include "vgui_controls/ComboBox.h"
#include <vgui_controls/ScrollBar.h>
#include "BasePanel.h"
#include "fmtstr.h"

using namespace vgui;

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

// Shared helper functions so xbox and pc can share as much code as possible while coming from different bases.

//-----------------------------------------------------------------------------
// Purpose: Sets the parameter pIconPanel to display the specified achievement's icon file.
//-----------------------------------------------------------------------------
bool LoadAchievementIcon( vgui::ImagePanel* pIconPanel, IAchievement *pAchievement, const char *pszExt /*= NULL*/ )
{
	char imagePath[_MAX_PATH];
	Q_strncpy( imagePath, "achievements\\", sizeof(imagePath) );
	Q_strncat( imagePath, pAchievement->GetName(), sizeof(imagePath), COPY_ALL_CHARACTERS );
	if ( pszExt )
	{
		Q_strncat( imagePath, pszExt, sizeof(imagePath), COPY_ALL_CHARACTERS );
	}
	Q_strncat( imagePath, ".vtf", sizeof(imagePath), COPY_ALL_CHARACTERS );

	char checkFile[_MAX_PATH];
	Q_snprintf( checkFile, sizeof(checkFile), "materials\\vgui\\%s", imagePath );
	if ( !g_pFullFileSystem->FileExists( checkFile ) )
	{
		Q_snprintf( imagePath, sizeof(imagePath), "hud\\icon_locked.vtf" );
	}

	pIconPanel->SetShouldScaleImage( true );
	pIconPanel->SetImage( imagePath );
	pIconPanel->SetVisible( true );
	
	return pIconPanel->IsVisible();
}

//-----------------------------------------------------------------------------
// The bias is to ensure the percentage bar gets plenty orange before it reaches the text,
// as the white-on-grey is hard to read.
//-----------------------------------------------------------------------------
Color LerpColors ( Color cStart, Color cEnd, float flPercent )
{
	float r = (float)((float)(cStart.r()) + (float)(cEnd.r() - cStart.r()) * Bias( flPercent, 0.75 ) );
	float g = (float)((float)(cStart.g()) + (float)(cEnd.g() - cStart.g()) * Bias( flPercent, 0.75 ) );
	float b = (float)((float)(cStart.b()) + (float)(cEnd.b() - cStart.b()) * Bias( flPercent, 0.75 ) );
	float a = (float)((float)(cStart.a()) + (float)(cEnd.a() - cStart.a()) * Bias( flPercent, 0.75 ) );
	return Color( r, g, b, a );
}

//-----------------------------------------------------------------------------
// Purpose: Shares common percentage bar calculations/color settings between xbox and pc.
//			Not really intended for robustness or reuse across many panels.
// Input  : pFrame - assumed to have certain child panels (see below)
//			*pAchievement - source achievement to poll for progress. Non progress achievements will not show a percentage bar.
//-----------------------------------------------------------------------------
void UpdateProgressBar( vgui::EditablePanel* pPanel, IAchievement *pAchievement, Color clrProgressBar )
{
	if ( pAchievement->GetGoal() > 1 )
	{
		bool bShowProgress = true;

		// if this achievement gets saved with game and we're not in a level and have not achieved it, then we do not have any state 
		// for this achievement, don't show progress
		if ( ( pAchievement->GetFlags() & ACH_SAVE_WITH_GAME ) && !GameUI().IsInLevel() && !pAchievement->IsAchieved() )
		{
			bShowProgress = false;
		}

		float flCompletion = 0.0f;

		// Once achieved, we can't rely on count. If they've completed the achievement just set to 100%.
		int iCount = pAchievement->GetCount();
		if ( pAchievement->IsAchieved() )
		{
			flCompletion = 1.0f;
			iCount = pAchievement->GetGoal();
		}
		else if ( bShowProgress )
		{
			flCompletion = ( ((float)pAchievement->GetCount()) / ((float)pAchievement->GetGoal()) );
			// In rare cases count can exceed goal and not be achieved (switch local storage on X360, take saved game from different user on PC).
			// These will self-correct with continued play, but if we're in that state don't show more than 100% achieved.
			flCompletion = min( flCompletion, 1.0 );
		}

		char szPercentageText[ 256 ] = "";
		if  ( bShowProgress )
		{
			Q_snprintf( szPercentageText, 256, "%d/%d", iCount, pAchievement->GetGoal() );			
		}	

		pPanel->SetControlString( "PercentageText", szPercentageText );
		pPanel->SetControlVisible( "PercentageText", true );
		pPanel->SetControlVisible( "CompletionText", true );

		vgui::ImagePanel *pPercentageBar	= (vgui::ImagePanel*)pPanel->FindChildByName( "PercentageBar" );
		vgui::ImagePanel *pPercentageBarBkg = (vgui::ImagePanel*)pPanel->FindChildByName( "PercentageBarBackground" );

		if ( pPercentageBar && pPercentageBarBkg )
		{
			pPercentageBar->SetFillColor( clrProgressBar );
			pPercentageBar->SetWide( pPercentageBarBkg->GetWide() * flCompletion );

			pPanel->SetControlVisible( "PercentageBarBackground", IsX360() ? bShowProgress : true );
			pPanel->SetControlVisible( "PercentageBar", true );
		}
	}
}

//-----------------------------------------------------------------------------
// Purpose: creates child panels, passes down name to pick up any settings from res files.
//-----------------------------------------------------------------------------
CAchievementsDialog::CAchievementsDialog( vgui::Panel *parent ) : BaseClass( parent, "AchievementsDialog" )
{
	SetBounds( 0, 0, 750, 490 );
	SetDeleteSelfOnClose( true );
	SetKeyBoardInputEnabled( true );
	SetMouseInputEnabled( true );
	
	SetTitle( "", false );

	m_nUnlocked = 0;

	m_pAchievementsList = new vgui::PanelListPanel( this, "listpanel_achievements" );
	m_pAchievementsList->SetFirstColumnWidth( 0 );
	m_pAchievementsList->GetScrollbar()->UseImages( "chalkboard_scroll_up", "chalkboard_scroll_down", "chalkboard_scroll_line", "chalkboard_scroll_box" );

	m_pListBG = new vgui::ImagePanel( this, "listpanel_background" );

	m_pPercentageBarBackground = SETUP_PANEL( new ImagePanel( this, "PercentageBarBackground" ) );
	m_pPercentageBar = SETUP_PANEL( new ImagePanel( this, "PercentageBar" ) );

	m_pAchievementPackCombo = new ComboBox( this, "achievement_pack_combo", 10, false );

	// int that holds the highest number achievement id we've found
	int iHighestAchievementIDSeen = -1;
	int iNextGroupBoundary = 1000;

	Q_memset( m_AchievementGroups, 0, sizeof(m_AchievementGroups) );
	m_iNumAchievementGroups = 0;

	// Base groups
	CreateNewAchievementGroup( 0, 16000 );
	CreateNewAchievementGroup( 0, 999 );

	Assert ( achievementmgr );
	if ( achievementmgr )
	{
		int iCount = achievementmgr->GetAchievementCount();
		for ( int i = 0; i < iCount; ++i )
		{		
			IAchievement* pCur = achievementmgr->GetAchievementByIndex( i );
			if ( !pCur )
				continue;

			int iAchievementID = pCur->GetAchievementID();

			if ( iAchievementID > iHighestAchievementIDSeen )
			{
				// if its crossed the next group boundary, create a new group
				if ( iAchievementID >= iNextGroupBoundary )
				{
					int iNewGroupBoundary = 100 * ( (int)( (float)iAchievementID / 100 ) );
					CreateNewAchievementGroup( iNewGroupBoundary, iNewGroupBoundary+99 );

					iNextGroupBoundary = iNewGroupBoundary + 100;
				}

				iHighestAchievementIDSeen = iAchievementID;
			}

			// don't show hidden achievements if not achieved
			if ( pCur->ShouldHideUntilAchieved() && !pCur->IsAchieved() )
				continue;

			bool bAchieved = pCur->IsAchieved();

			if ( bAchieved )
			{
				++m_nUnlocked;
			}

			for ( int j=0;j<m_iNumAchievementGroups;j++ )
			{
				if ( iAchievementID >= m_AchievementGroups[j].m_iMinRange &&
					iAchievementID <= m_AchievementGroups[j].m_iMaxRange )
				{
					if ( bAchieved )
					{
						m_AchievementGroups[j].m_iNumUnlocked++;
					}

					m_AchievementGroups[j].m_iNumAchievements++;
				}
			}
			
			CAchievementDialogItemPanel *achievementItemPanel = new CAchievementDialogItemPanel( m_pAchievementsList, "AchievementDialogItemPanel", i );
			achievementItemPanel->SetAchievementInfo( pCur );
			m_pAchievementsList->AddItem( NULL, achievementItemPanel );
		}
	}


	for ( int i=0;i<m_iNumAchievementGroups;i++ )
	{
		char buf[128];

		if ( i == 0 )
			Q_snprintf( buf, sizeof(buf), "#Achievement_Group_All" );
		else
			Q_snprintf( buf, sizeof(buf), "#Achievement_Group_%d", m_AchievementGroups[i].m_iMinRange );

		wchar_t *wzGroupName = g_pVGuiLocalize->Find( buf );
		if ( !wzGroupName )
			wzGroupName = L"Need Title ( %s1 of %s2 )";

		wchar_t wzGroupTitle[128];
		if ( wzGroupName )
		{
			wchar_t wzNumUnlocked[8];
			_itow_s( m_AchievementGroups[i].m_iNumUnlocked, wzNumUnlocked, ARRAYSIZE( wzNumUnlocked ), 10 );

			wchar_t wzNumAchievements[8];
			_itow_s( m_AchievementGroups[i].m_iNumAchievements, wzNumAchievements, ARRAYSIZE( wzNumAchievements ), 10 );

			g_pVGuiLocalize->ConstructString( wzGroupTitle, sizeof( wzGroupTitle ), wzGroupName, 2, wzNumUnlocked, wzNumAchievements );
		}

		KeyValues *pKV = new KeyValues( "grp" );
		pKV->SetInt( "minrange", m_AchievementGroups[i].m_iMinRange );
		pKV->SetInt( "maxrange", m_AchievementGroups[i].m_iMaxRange );
		m_pAchievementPackCombo->AddItem( wzGroupTitle, pKV );
	}

	m_pAchievementPackCombo->ActivateItemByRow( 0 );
}

CAchievementsDialog::~CAchievementsDialog()
{
	m_pAchievementsList->DeleteAllItems();
	delete m_pAchievementsList;
	delete m_pPercentageBarBackground;
	delete m_pPercentageBar;
}

void CAchievementsDialog::CreateNewAchievementGroup( int iMinRange, int iMaxRange )
{
	m_AchievementGroups[m_iNumAchievementGroups].m_iMinRange = iMinRange;
	m_AchievementGroups[m_iNumAchievementGroups].m_iMaxRange = iMaxRange;
	m_iNumAchievementGroups++;
}

//----------------------------------------------------------
// Get the width we're going to lock at
//----------------------------------------------------------
void CAchievementsDialog::ApplySettings( KeyValues *pResourceData )
{
	m_iFixedWidth = pResourceData->GetInt( "wide", 512 );

	BaseClass::ApplySettings( pResourceData );
}

//----------------------------------------------------------
// Preserve our width to the one in the .res file
//----------------------------------------------------------
void CAchievementsDialog::OnSizeChanged( int newWide, int newTall)
{
	// Lock the width, but allow height scaling
	if ( newWide != m_iFixedWidth )
	{
		SetSize( m_iFixedWidth, newTall );
		return;
	}

	BaseClass::OnSizeChanged( newWide, newTall );
}

//----------------------------------------------------------
// New group was selected in the dropdown, recalc what achievements to show
//----------------------------------------------------------
void CAchievementsDialog::OnTextChanged( KeyValues *data )
{
	Panel *pPanel = (Panel *)data->GetPtr( "panel", NULL );

	// first check which control had its text changed!
	if ( pPanel == m_pAchievementPackCombo )
	{
		// Re-populate the achievement list with the selected group
		m_pAchievementsList->DeleteAllItems();

		KeyValues *pData = m_pAchievementPackCombo->GetActiveItemUserData();

		if ( pData )
		{
			int iMinRange = pData->GetInt( "minrange" );
			int iMaxRange = pData->GetInt( "maxrange" );

			if ( achievementmgr )
			{
				int iCount = achievementmgr->GetAchievementCount();
				for ( int i = 0; i < iCount; ++i )
				{		
					IAchievement* pCur = achievementmgr->GetAchievementByIndex( i );

					if ( !pCur )
						continue;
					
					int iAchievementID = pCur->GetAchievementID();

					if ( iAchievementID < iMinRange || iAchievementID > iMaxRange )
						continue;

					// don't show hidden achievements if not achieved
					if ( pCur->ShouldHideUntilAchieved() && !pCur->IsAchieved() )
						continue;

					CAchievementDialogItemPanel *achievementItemPanel = new CAchievementDialogItemPanel( m_pAchievementsList, "AchievementDialogItemPanel", i );
					achievementItemPanel->SetAchievementInfo( pCur );
					m_pAchievementsList->AddItem( NULL, achievementItemPanel );
				}
			}
		}

		m_pAchievementsList->ScrollToItem( 0 );
	}
}

//-----------------------------------------------------------------------------
// Purpose: Loads settings from achievementsdialog.res in hl2/resource/ui/
//			Sets up progress bar displaying total achievement unlocking progress by the user.
//-----------------------------------------------------------------------------
void CAchievementsDialog::ApplySchemeSettings( IScheme *pScheme )
{
	BaseClass::ApplySchemeSettings( pScheme );

	LoadControlSettings( "resource/AchievementsDialog.res" );

	if ( !achievementmgr )
	{
		AssertOnce( 0 && "No achievement manager interface in GameUI.dll" );
		return;
	}

	// Set up total completion percentage bar
	float flCompletion = 0.0f;
	if ( achievementmgr->GetAchievementCount() > 0 )
	{
		flCompletion = (((float)m_nUnlocked) / ((float)achievementmgr->GetAchievementCount()));
	}

	char szPercentageText[64];
	Q_snprintf( szPercentageText, 256, "%d / %d ( %d%% )",
		m_nUnlocked, achievementmgr->GetAchievementCount(), (int)( flCompletion * 100.0f ) );

	SetControlString( "PercentageText", szPercentageText );
	SetControlVisible( "PercentageText", true );
	SetControlVisible( "CompletionText", true );

	Color clrHighlight = pScheme->GetColor( "NewGame.SelectionColor", Color(255, 255, 255, 255) );
	Color clrWhite(255, 255, 255, 255);

	Color cProgressBar = Color( static_cast<float>( clrHighlight.r() ) * ( 1.0f - flCompletion ) + static_cast<float>( clrWhite.r() ) * flCompletion,
		static_cast<float>( clrHighlight.g() ) * ( 1.0f - flCompletion ) + static_cast<float>( clrWhite.g() ) * flCompletion,
		static_cast<float>( clrHighlight.b() ) * ( 1.0f - flCompletion ) + static_cast<float>( clrWhite.b() ) * flCompletion,
		static_cast<float>( clrHighlight.a() ) * ( 1.0f - flCompletion ) + static_cast<float>( clrWhite.a() ) * flCompletion );

	m_pPercentageBar->SetFgColor( cProgressBar );
	m_pPercentageBar->SetWide( m_pPercentageBarBackground->GetWide() * flCompletion );

	SetControlVisible( "PercentageBarBackground", true );
	SetControlVisible( "PercentageBar", true );

	if ( m_iNumAchievementGroups <= 2 )
	{
		// we have no achievement packs. Hide the combo and bump the achievement list up a bit
		m_pAchievementPackCombo->SetVisible( false );

		// do some work to preserve the pincorner and resizing

		int comboX, comboY;
		m_pAchievementPackCombo->GetPos( comboX, comboY );

		int x, y, w, h;
		m_pAchievementsList->GetBounds( x, y, w, h );

		PinCorner_e corner = m_pAchievementsList->GetPinCorner();
		int pinX, pinY;
		m_pAchievementsList->GetPinOffset( pinX, pinY );

		int resizeOffsetX, resizeOffsetY;
		m_pAchievementsList->GetResizeOffset( resizeOffsetX, resizeOffsetY );

		m_pAchievementsList->SetAutoResize( corner, AUTORESIZE_DOWN, pinX, comboY, resizeOffsetX, resizeOffsetY );

		m_pAchievementsList->SetBounds( x, comboY, w, h + ( y - comboY ) );

		m_pListBG->SetAutoResize( corner, AUTORESIZE_DOWN, pinX, comboY, resizeOffsetX, resizeOffsetY );
		m_pListBG->SetBounds( x, comboY, w, h + ( y - comboY ) );
	}

	MoveToCenterOfScreen();
}

//-----------------------------------------------------------------------------
// Purpose: Each sub-panel gets its data updated
//-----------------------------------------------------------------------------
void CAchievementsDialog::UpdateAchievementDialogInfo( void )
{
	for ( int i = 0; i < m_pAchievementsList->GetItemCount(); i++ )
	{
		CAchievementDialogItemPanel *pPanel = (CAchievementDialogItemPanel*)m_pAchievementsList->GetItemPanel(i);
		if ( pPanel )
		{
			pPanel->UpdateAchievementInfo();
		}
	}
}

void CAchievementsDialog::OnCommand( const char *command )
{
	if ( !Q_strcasecmp( command, "ongameuiactivated" ) )
	{
		Msg( "Updating achievement info\n" );
		UpdateAchievementDialogInfo();
	}

	BaseClass::OnCommand( command );
}

//-----------------------------------------------------------------------------
// Purpose: creates child panels, passes down name to pick up any settings from res files.
//-----------------------------------------------------------------------------
CAchievementDialogItemPanel::CAchievementDialogItemPanel( vgui::PanelListPanel *parent, const char* name, int iListItemID ) : BaseClass( parent, name )
{
	m_iListItemID = iListItemID;
	m_pParent = parent;
	m_pSchemeSettings = NULL;

	m_pAchievementIcon = SETUP_PANEL(new vgui::ImagePanel( this, "AchievementIcon" ));
	m_pAchievementNameLabel = new vgui::Label( this, "AchievementName", "name" );
	m_pAchievementDescLabel = new vgui::Label( this, "AchievementDesc", "desc" );
	m_pPercentageBar = SETUP_PANEL( new ImagePanel( this, "PercentageBar" ) );
	m_pPercentageText = new vgui::Label( this, "PercentageText", "" );

	CMouseMessageForwardingPanel *panel = new CMouseMessageForwardingPanel(this, NULL);
	panel->SetZPos(2);
}

CAchievementDialogItemPanel::~CAchievementDialogItemPanel()
{
	delete m_pAchievementIcon;
	delete m_pAchievementNameLabel;
	delete m_pAchievementDescLabel;
	delete m_pPercentageBar;
	delete m_pPercentageText;
}

//-----------------------------------------------------------------------------
// Purpose: Updates displayed achievement data. In applyschemesettings, and when gameui activates.
//-----------------------------------------------------------------------------
void CAchievementDialogItemPanel::UpdateAchievementInfo()
{
	if ( m_pSourceAchievement && m_pSchemeSettings )
	{
		// Set name, description and unlocked state text
		m_pAchievementNameLabel->SetText( ACHIEVEMENT_LOCALIZED_NAME( m_pSourceAchievement ) );
		m_pAchievementDescLabel->SetText( ACHIEVEMENT_LOCALIZED_DESC( m_pSourceAchievement ) );

		// Setup icon
		// get the vtfFilename from the path.

		// Display percentage completion for progressive achievements
		// Set up total completion percentage bar. Goal > 1 means its a progress achievement.
		UpdateProgressBar( this, m_pSourceAchievement, m_clrProgressBar );

		if ( m_pSourceAchievement->IsAchieved() )
		{
			LoadAchievementIcon( m_pAchievementIcon, m_pSourceAchievement );
		}
		else
		{
			LoadAchievementIcon( m_pAchievementIcon, m_pSourceAchievement, "_bw" );
		}
	}
}

//-----------------------------------------------------------------------------
// Purpose: Makes a local copy of a pointer to the achievement entity stored on the client.
//-----------------------------------------------------------------------------
void CAchievementDialogItemPanel::SetAchievementInfo( IAchievement* pAchievement )
{
	if ( !pAchievement )
	{
		Assert( 0 );
		return;
	}

	m_pSourceAchievement = pAchievement;
	m_iSourceAchievementIndex = pAchievement->GetAchievementID();
}

//-----------------------------------------------------------------------------
// Purpose: Loads settings from hl2/resource/ui/achievementitem.res
//			Sets display info for this achievement item.
//-----------------------------------------------------------------------------
void CAchievementDialogItemPanel::ApplySchemeSettings( IScheme* pScheme )
{
	LoadControlSettings( "resource/ui/AchievementItem.res" );

	m_pSchemeSettings = pScheme;
	
	if ( !m_pSourceAchievement )
		return;

	BaseClass::ApplySchemeSettings( pScheme );

	// m_pSchemeSettings must be set for this.
	UpdateAchievementInfo();

	if ( m_pSourceAchievement->IsAchieved() )
	{
		SetBgColor( pScheme->GetColor( "AchievementsLightGrey", Color(255, 0, 0, 255) ) );

		m_pAchievementNameLabel->SetFgColor( pScheme->GetColor( "SteamLightGreen", Color(255, 255, 255, 255) ) );
	}
	else
	{
		SetBgColor( pScheme->GetColor( "AchievementsDarkGrey", Color(255, 0, 0, 255) ) );

		Color fgColor = pScheme->GetColor( "AchievementsInactiveFG", Color(255, 255, 255, 255) );
		m_pAchievementNameLabel->SetFgColor( fgColor );
		m_pAchievementDescLabel->SetFgColor( fgColor );
		m_pPercentageText->SetFgColor( fgColor );
	}	
}