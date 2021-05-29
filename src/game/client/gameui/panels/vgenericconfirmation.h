//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose:
//
//===========================================================================//

#ifndef VGENERICCONFIRMATION_H
#define VGENERICCONFIRMATION_H
#ifdef _WIN32
#pragma once
#endif

#include "vgui_controls/cvartogglecheckbutton.h"

#include "basemodui.h"

#ifdef TF_VINTAGE_CLIENT
#include "tf_controls.h"
#else
#include "vgui_controls/Button.h"
#endif

namespace BaseModUI 
{
	class GenericConfirmation : public CBaseModFrame
	{
		DECLARE_CLASS_SIMPLE( GenericConfirmation, CBaseModFrame );
	public:
		GenericConfirmation(vgui::Panel *parent, const char *panelName);
		~GenericConfirmation();

		// 
		// Public types
		//
		typedef void (*Callback_t)(void);

		struct Data_t
		{
			const char* pWindowTitle;
			const char* pMessageText;
			const wchar_t* pMessageTextW;
			
			bool        bOkButtonEnabled;
			Callback_t	pfnOkCallback;

			bool        bCancelButtonEnabled;
			Callback_t	pfnCancelCallback;

			bool		bCheckBoxEnabled;
			const char *pCheckBoxLabelText;
			const char *pCheckBoxCvarName;

			Data_t();
		};

		int  SetUsageData( const Data_t & data );     // returns the usageId, different number each time this is called
		int  GetUsageId() const { return m_usageId; }

		// 
		// Accessors
		//
		const Data_t & GetUsageData() const { return m_data; }

	protected:
		virtual void ApplySchemeSettings(vgui::IScheme *pScheme);
		virtual void OnCommand(const char *command);
		virtual void OnKeyCodePressed(vgui::KeyCode code);
		virtual void OnKeyCodeTyped( vgui::KeyCode code );
		virtual void OnOpen();
		virtual void LoadLayout();
		virtual void PaintBackground();

		vgui::CvarToggleCheckButton<ConVarRef> *m_pCheckBox;

	#ifdef TF_VINTAGE_CLIENT
		CExMenuButton *m_pBtnOK;
		CExMenuButton *m_pBtnCancel;
	#else
		vgui::Button *m_pBtnOK;
		vgui::Button *m_pBtnCancel;
	#endif

	private:
		vgui::Label *m_pLblMessage;
		vgui::Label *m_pLblCheckBox;

		Data_t		 m_data;
		int			 m_usageId;

		static int sm_currentUsageId;

		bool	m_bNeedsMoveToFront;

		vgui::HFont		m_hTitleFont;
		vgui::HFont		m_hMessageFont;
	};
};
#endif // VGENERICCONFIRMATION_H