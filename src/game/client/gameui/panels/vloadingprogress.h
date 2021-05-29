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
#ifdef TF_VINTAGE_CLIENT
#include "tf_controls.h"
#include "LoadingTipPanel.h"
#else
#include "vgui_controls/Button.h"
#endif

namespace BaseModUI 
{
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
		vgui::ImagePanel	*m_pBGImage;
		vgui::Panel			*m_pFooter;
		vgui::Label			*m_pLoadingProgress;

#ifdef TF_VINTAGE_CLIENT
		CExMenuButton		*m_pCancelButton;
#else
		vgui::Button		*m_pCancelButton;
#endif

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
};
#endif // VLOADINGPROGRESS_H