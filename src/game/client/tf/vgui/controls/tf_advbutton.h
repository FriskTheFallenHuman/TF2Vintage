#ifndef tf_advbutton_H
#define tf_advbutton_H
#ifdef _WIN32
#pragma once
#endif

#include <vgui_controls/EditablePanel.h>
#include "tf_imagepanel.h"
#include "tf_advbuttonbase.h"

using namespace vgui;

class CTFButton;
#define pImageColorSelected		(!m_bSelected ? pImageColorDefault : pSelectedColor) 

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
class CTFAdvButton : public CTFAdvButtonBase
{
	friend class CTFButton;
public:
	DECLARE_CLASS_SIMPLE(CTFAdvButton, CTFAdvButtonBase);

	CTFAdvButton(vgui::Panel *parent, const char *panelName, const char *text);
	~CTFAdvButton();
	void Init();

	void ApplySettings(KeyValues *inResourceData);
	void ApplySchemeSettings(vgui::IScheme *pScheme);
	void PerformLayout();

	void SendAnimation(MouseState flag);
	void SetDefaultAnimation();
	void SetText(const char *tokenName);
	void SetGlowing(bool Glowing);
	void SetShouldScaleImage(bool state) { m_bScaleImage = state; };
	bool GetShouldScaleImage() { return m_bScaleImage; };
	void SetSelected(bool bState);

	void OnThink();
	CTFButton *GetButton(){ return pButton; };

protected:
	CTFButton		*pButton;
	bool			bGlowing;
	bool			m_bAnimationIn;
	bool			m_bScaleImage;

	float			m_flActionThink;
	float			m_flAnimationThink;
};

#ifdef GAMEUI_EMBEDDED
//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
class CTFMenuButton : public CTFAdvButton
{
	friend class CTFButton;
	friend class CTFAdvButton;
public:
	DECLARE_CLASS_SIMPLE( CTFAdvButton, CTFAdvButton );

	// You can optionally pass in the panel to send the click message to and the name of the command to send to that panel.
	CTFMenuButton( vgui::Panel *parent, const char *panelName, const char *text, Panel *pActionSignalTarget=NULL, const char *pCmd=NULL );
	~CTFMenuButton();

	CTFButton *GetButton(){ return pButton; };

private:
	typedef CTFAdvButton BaseClass;

	CTFButton		*pButton;
};
#endif

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
class CTFButton : public CTFButtonBase
{
	friend class CTFAdvButton;
public:
	DECLARE_CLASS_SIMPLE(CTFButton, CTFButtonBase);

	CTFButton(vgui::Panel *parent, const char *panelName, const char *text);

	void ApplySettings(KeyValues *inResourceData);
	void ApplySchemeSettings(vgui::IScheme *pScheme);
	void PerformLayout();
	void OnCursorExited();
	void OnCursorEntered();
	void OnMousePressed(vgui::MouseCode code);
	void OnMouseReleased(vgui::MouseCode code);
	void SetMouseEnteredState(MouseState flag);
	void SetParent(CTFAdvButton *m_pButton) { m_pParent = m_pButton; };

private:
	CTFAdvButton   *m_pParent;
	float			m_fXShift;
	float			m_fYShift;
};


#endif // tf_advbutton_H
