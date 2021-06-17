#ifndef TFMAINMENULOADOUTPANEL_H
#define TFMAINMENULOADOUTPANEL_H

#include <vgui_controls/Frame.h>
#include <vgui_controls/AnimationController.h>
#include "tf_inventory.h"
#include "charinfo_armory_panel.h"

class CTFAdvModelPanel;
class CTFWeaponSetPanel;
class CModelPanel;
class CTFAdvButton;
class CTFAdvItemButton;

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
class CCharInfoLoadoutPanel : public vgui::Frame
{
	DECLARE_CLASS_SIMPLE( CCharInfoLoadoutPanel, vgui::Frame );

public:
			CCharInfoLoadoutPanel( vgui::Panel* parent );
	virtual ~CCharInfoLoadoutPanel();

	virtual void PerformLayout();
	virtual void ApplySchemeSettings( vgui::IScheme *pScheme );
	virtual void OnCommand( const char* command );
	virtual void OnClose();
	virtual void Activate();

	void DefaultLayout();
	void SetWeaponPreset( int iClass, int iSlot, int iPreset );
	void SetCurrentClass( int iClass );
	void SetCurrentSlot( int iSlot ) { m_iCurrentSlot = iSlot; };
	int  GetAnimSlot( CEconItemDefinition *pItemDef, int iClass );
	const char *GetWeaponModel( CEconItemDefinition *pItemDef, int iClass );
	const char *GetExtraWearableModel( CEconItemDefinition *pItemDef );
	void UpdateModelWeapons( void );
	void UpdateMenuBodygroups(void);
	void SetModelClass(int iClass );
	void SetSlotAndPreset( int iSlot, int iPreset );
	void ResetRows();
	int GetCurrentClass( void ) { return m_iCurrentClass; }

private:
	CTFAdvModelPanel *m_pClassModelPanel;
	CModelPanel		*m_pGameModelPanel;
	CUtlVector<CTFAdvItemButton*> m_pWeaponIcons;

	MESSAGE_FUNC( UpdateModelPanels, "ControlModified" );

	int	m_iCurrentClass;
	int	m_iCurrentSlot;
	int m_iCurrentSkin;

	CCharInfoArmoryPanel *m_pItemPanel;
	CTFAdvButton *m_pPresetButtons[TF_MAX_PRESETS];
};

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
class CTFWeaponSetPanel : public vgui::EditablePanel
{
	DECLARE_CLASS_SIMPLE(CTFWeaponSetPanel, vgui::EditablePanel);

public:
	CTFWeaponSetPanel(vgui::Panel* parent, const char *panelName);
	void OnCommand(const char* command);
};

#endif // TFMAINMENULOADOUTPANEL_H
