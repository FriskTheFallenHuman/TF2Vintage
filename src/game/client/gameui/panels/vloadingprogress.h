//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose:
//
//===========================================================================//

#ifndef VLOADINGPROGRESS_H
#define VLOADINGPROGRESS_H
#ifdef _WIN32
#pragma once
#endif

#include "basemodui.h"
#include "vgui/IScheme.h"
#include "const.h"
#include "igameevents.h"
#include "controls/tf_advbutton.h"
#include "vgui_controls/EditablePanel.h"

namespace BaseModUI 
{
	class CLoadingTipPanel;

	class LoadingProgress : public CBaseModFrame, public CGameEventListener
	{
		DECLARE_CLASS_SIMPLE( LoadingProgress, CBaseModFrame );

	public:
		enum LoadingType
		{
			LT_UNDEFINED = 0,
			LT_MAINMENU,
			LT_TRANSITION,
		};

		enum LoadingWindowType
		{
			LWT_LOADINGPLAQUE,
			LWT_BKGNDSCREEN,
		};

	public:
		LoadingProgress( vgui::Panel *parent, const char *panelName, LoadingWindowType eLoadingType );
		~LoadingProgress();

		virtual void		Activate();
		virtual void		Close();

		void				FireGameEvent( IGameEvent* event ) OVERRIDE;

		void				SetProgress( float progress );
		float				GetProgress();
		bool				IsDrawingProgressBar( void ) { return m_bDrawProgress; }

		void				SetLoadingType( LoadingType loadingType );
		LoadingType			GetLoadingType();

		void				SetStatusText( const char *statusText );

	protected:
		virtual void		OnThink();
		virtual void		OnCommand(const char *command);
		virtual void		ApplySchemeSettings( vgui::IScheme *pScheme );
		virtual void		PaintBackground();
		virtual void		OnKeyCodeTyped( vgui::KeyCode code );

	private:
		void				SetupControlStates( void );
		void				UpdateLoadingSpinner();

		vgui::ProgressBar	*m_pLoadingBar;
		vgui::ImagePanel	*m_pLoadingSpinner;
		vgui::ImagePanel	*m_pMainBackground;
		vgui::Panel			*m_pFooter;
		vgui::Label			*m_pLoadingProgress;

		CTFMenuButton		*m_pCancelButton;
		LoadingType			m_LoadingType;
		LoadingWindowType	m_LoadingWindowType;

		bool				m_bLoadedFully;

		bool				m_bValid;

		bool				m_bDrawBackground;
		bool				m_bDrawProgress;
		bool				m_bDrawSpinner;

		float				m_flPeakProgress;

		float				m_flLastEngineTime;

		CLoadingTipPanel	*m_pTipPanel;
	};

	//===========================================================================//
	// Purpose: Tip display during level loads.
	//===========================================================================//
	class CLoadingTipPanel : public vgui::EditablePanel
	{
		DECLARE_CLASS_SIMPLE( CLoadingTipPanel, vgui::EditablePanel )

	public:
		CLoadingTipPanel( Panel *pParent );
		~CLoadingTipPanel() {}

		virtual void ApplySchemeSettings( vgui::IScheme *pScheme );
		void ReloadScheme( void );

		void NextTip( void );

	private:
		void SetupTips( void );
	};
};
#endif // VLOADINGPROGRESS_H