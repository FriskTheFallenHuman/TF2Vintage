//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose: 
//
// $NoKeywords: $
//=============================================================================//

#undef fopen

#ifdef _WIN32
#include <windows.h> // SRC only!!
#include <io.h>
#endif

#include "OptionsSubMultiplayer.h"
#include "MultiplayerAdvancedDialog.h"
#include <stdio.h>

#include <vgui_controls/Button.h>
#include <vgui_controls/CheckButton.h>
#include "tier1/KeyValues.h"
#include <vgui_controls/Label.h>
#include <vgui/ISystem.h>
#include <vgui/ISurface.h>
#include <vgui/Cursor.h>
#include <vgui_controls/RadioButton.h>
#include <vgui_controls/ComboBox.h>
#include <vgui_controls/ImagePanel.h>
#include <vgui_controls/FileOpenDialog.h>
#include <vgui_controls/MessageBox.h>
#include <vgui/IVGui.h>
#include <vgui/ILocalize.h>
#include <vgui/IPanel.h>
#include <vgui_controls/MessageBox.h>

#include "CvarTextEntry.h"
#include "CvarToggleCheckButton.h"
#include "CvarSlider.h"
#include "LabeledCommandComboBox.h"
#include "filesystem.h"
#include "EngineInterface.h"
#include "BitmapImagePanel.h"
#include "utlbuffer.h"
#include "ModInfo.h"
#include "tier1/convar.h"

#include "materialsystem/imaterial.h"
#include "materialsystem/imesh.h"
#include "materialsystem/imaterialvar.h"

#include <setjmp.h>

#include "bitmap/tgawriter.h"
#include "ivtex.h"

// memdbgon must be the last include file in a .cpp file!!!
#include <tier0/memdbgon.h>

using namespace vgui;

struct ColorItem_t
{
	char		*name;
	int			r, g, b;
};

static ColorItem_t s_crosshairColors[] = 
{
	{ "#Valve_Green",	50,		250,	50 },
	{ "#Valve_Red",		250,	50,		50 },
	{ "#Valve_Blue",	50,		50,		250 },
	{ "#Valve_Yellow",	250,	250,	50 },
	{ "#Valve_Ltblue",	50,		250,	250 }
};
static const int NumCrosshairColors = sizeof( s_crosshairColors ) / sizeof( s_crosshairColors[0] );

//-----------------------------------------------------------------------------
class CrosshairImagePanel : public ImagePanel
{
	typedef ImagePanel BaseClass;
public:
	CrosshairImagePanel( Panel *parent, const char *name, CCvarToggleCheckButton *pAdditive );

	virtual void Paint();

	void UpdateCrosshair( int r, int g, int b, int size );

protected:
	int m_R, m_G, m_B;
	int m_barSize;
	int m_barGap;
	CCvarToggleCheckButton *m_pAdditive;
	int m_iCrosshairTextureID;
};

//-----------------------------------------------------------------------------
CrosshairImagePanel::CrosshairImagePanel( Panel *parent, const char *name, CCvarToggleCheckButton *pAdditive ) : ImagePanel( parent, name )
{
	m_pAdditive = pAdditive;
	UpdateCrosshair( 50, 250, 50, 0 );

	m_iCrosshairTextureID = vgui::surface()->CreateNewTextureID();
	vgui::surface()->DrawSetTextureFile( m_iCrosshairTextureID, "vgui/white_additive" , true, false);
}

//-----------------------------------------------------------------------------
void CrosshairImagePanel::UpdateCrosshair( int r, int g, int b, int size )
{
	m_R = r;
	m_G = g;
	m_B = b;

	int screenWide, screenTall;
	surface()->GetScreenSize( screenWide, screenTall );
	if ( size == 0 )
	{
		if (screenWide <= 640)
		{
			// if the screen width is 640 or less, set the crosshair num to 3 (large)
			size = 1;
		}
		else if (screenWide < 1024)
		{
			// if the screen width is between 640 and 1024, set the crosshair num to 2 (medium)
			size = 2;
		}
		else
		{
			// if the screen width is 1024 or greater, set the crosshair num to 1 (small)
			size = 3;
		}
	}

	int scaleBase = 1024;
	switch( size )
	{
	case 3:
		scaleBase = 640;
		break;
	case 2:
		scaleBase = 800;
		break;
	default:
		scaleBase = 1024;
		break;
	}

	m_barSize = (int) 9 * screenWide / scaleBase;
	m_barGap = (int) 5 * screenWide / scaleBase;
}

//-----------------------------------------------------------------------------
static void DrawCrosshairRect( int x, int y, int w, int h, bool additive )
{
	if ( additive )
	{
		vgui::surface()->DrawTexturedRect( x, y, x+w, y+h );
	}
	else
	{
		vgui::surface()->DrawFilledRect( x, y, x+w, y+h );
	}
}

//-----------------------------------------------------------------------------
void CrosshairImagePanel::Paint()
{
	BaseClass::Paint();

	if ( !m_pAdditive )
		return;

	int wide, tall;
	GetSize( wide, tall );

	bool additive = m_pAdditive->IsSelected();

	int a = 200;
	if ( !additive )
	{
		ConVarRef cl_crosshairalpha( "cl_crosshairalpha" );
		if ( cl_crosshairalpha.IsValid() )
		{
			a = clamp( cl_crosshairalpha.GetInt(), 0, 255 );
		}
	}
	vgui::surface()->DrawSetColor( m_R, m_G, m_B, a );

	if ( additive )
	{
		vgui::surface()->DrawSetTexture( m_iCrosshairTextureID );
	}

	DrawCrosshairRect( (wide / 2 - m_barGap - m_barSize) + 1, tall / 2, m_barSize, 1, additive );
	DrawCrosshairRect( wide / 2 + m_barGap, tall / 2, m_barSize, 1, additive );
	DrawCrosshairRect( wide / 2, tall / 2 - m_barGap - m_barSize, 1, m_barSize, additive );
	DrawCrosshairRect( wide / 2, tall / 2 + m_barGap, 1, m_barSize, additive );
}

//-----------------------------------------------------------------------------
class AdvancedCrosshairImagePanel : public ImagePanel
{
	typedef ImagePanel BaseClass;
public:
			 AdvancedCrosshairImagePanel( Panel *parent, const char *name );
	virtual ~AdvancedCrosshairImagePanel();

	virtual void Paint();

	void UpdateCrosshair( int r, int g, int b, float scale, const char *crosshairname );

protected:
	int m_R, m_G, m_B;
	float m_flScale;

	// material
	int				m_iCrosshairTextureID;
	IVguiMatInfo	*m_pAdvCrosshair;

	// animation
	IVguiMatInfoVar	*m_pFrameVar;
	float			m_flNextFrameChange;
	int				m_nNumFrames;
	bool			m_bAscending;	// animating forward or in reverse?
};

//-----------------------------------------------------------------------------
AdvancedCrosshairImagePanel::AdvancedCrosshairImagePanel( Panel *parent, const char *name ) : ImagePanel( parent, name )
{
	m_pAdvCrosshair = NULL;
	m_pFrameVar = NULL;

	if ( ModInfo().AdvCrosshair() )
	{
		m_iCrosshairTextureID = vgui::surface()->CreateNewTextureID();
		UpdateCrosshair( 50, 250, 50, 32.0, "vgui/crosshairs/crosshair1" );
	}
}

AdvancedCrosshairImagePanel::~AdvancedCrosshairImagePanel()
{
	if ( m_pFrameVar )
	{
		delete m_pFrameVar;
		m_pFrameVar = NULL;
	}

	if ( m_pAdvCrosshair )
	{
		delete m_pAdvCrosshair;
		m_pAdvCrosshair = NULL;
	}
}

//-----------------------------------------------------------------------------
void AdvancedCrosshairImagePanel::UpdateCrosshair( int r, int g, int b, float scale, const char *crosshairname )
{
	m_R = r;
	m_G = g;
	m_B = b;

	m_flScale = scale;

	vgui::surface()->DrawSetTextureFile( m_iCrosshairTextureID, crosshairname, true, false );

	if ( m_pAdvCrosshair )
	{
		delete m_pAdvCrosshair;
	}

	m_pAdvCrosshair = vgui::surface()->DrawGetTextureMatInfoFactory( m_iCrosshairTextureID );

	Assert(m_pAdvCrosshair);

	m_pFrameVar = m_pAdvCrosshair->FindVarFactory( "$frame", NULL );
	m_nNumFrames = m_pAdvCrosshair->GetNumAnimationFrames();

	m_flNextFrameChange = system()->GetFrameTime() + 0.2;
	m_bAscending = true;
}

//-----------------------------------------------------------------------------
void AdvancedCrosshairImagePanel::Paint()
{
	BaseClass::Paint();

	int wide, tall;
	GetSize( wide, tall );

	int iClipX0, iClipY0, iClipX1, iClipY1;
	ipanel()->GetClipRect(GetVPanel(), iClipX0, iClipY0, iClipX1, iClipY1 );

	// scroll through all frames
	if ( m_pFrameVar )
	{	
		float curtime = system()->GetFrameTime();

		if ( curtime >= m_flNextFrameChange )
		{
			m_flNextFrameChange = curtime + 0.2;

			int frame = m_pFrameVar->GetIntValue();

			if ( m_bAscending )
			{
				frame++;
				if ( frame >= m_nNumFrames )
				{
					m_bAscending = !m_bAscending;
					frame--;
				}
			}
			else
			{
				frame--;
				if ( frame < 0 )
				{
					m_bAscending = !m_bAscending;
					frame++;
				}
			}

			m_pFrameVar->SetIntValue(frame);
		}
	}

	float x, y;

	// assume square
	float flDrawWidth = ( m_flScale/48.0 ) * (float)wide;	
	int flHalfWidth = (int)( flDrawWidth / 2 );

	x = wide/2 - flHalfWidth;
	y = tall/2 - flHalfWidth;

	vgui::surface()->DrawSetColor( m_R, m_G, m_B, 255 );
	vgui::surface()->DrawSetTexture( m_iCrosshairTextureID );
	vgui::surface()->DrawTexturedRect( x, y, x+flDrawWidth, y+flDrawWidth );
	vgui::surface()->DrawSetTexture(0);
}

//-----------------------------------------------------------------------------
// Purpose: Basic help dialog
//-----------------------------------------------------------------------------
COptionsSubMultiplayer::COptionsSubMultiplayer(vgui::Panel *parent) : vgui::PropertyPage(parent, "OptionsSubMultiplayer") 
{
	Button *cancel = new Button( this, "Cancel", "#GameUI_Cancel" );
	cancel->SetCommand( "Close" );

	Button *ok = new Button( this, "OK", "#GameUI_OK" );
	ok->SetCommand( "Ok" );

	Button *apply = new Button( this, "Apply", "#GameUI_Apply" );
	apply->SetCommand( "Apply" );

	Button *advanced = new Button( this, "Advanced", "#GameUI_AdvancedEllipsis" );
	advanced->SetCommand( "Advanced" );

	Button *importSprayImage = new Button( this, "ImportSprayImage", "#GameUI_ImportSprayEllipsis" );
	importSprayImage->SetCommand("ImportSprayImage");

	m_hImportSprayDialog = NULL;

	m_pLogoList = new CLabeledCommandComboBox( this, "SpraypaintList" );
	m_LogoName[0] = 0;
	InitLogoList( m_pLogoList );

	m_pLogoImage = new ImagePanel( this, "LogoImage" );
	m_pLogoImage->AddActionSignalTarget( this );
	
	m_nLogoR = 255;
	m_nLogoG = 255;
	m_nLogoB = 255;

	// crosshair controls
	m_pCrosshairColorComboBox = new ComboBox(this, "CrosshairColorComboBox", 6, false);
	m_pCrosshairSize = new CLabeledCommandComboBox(this, "CrosshairSizeComboBox");
	m_pCrosshairTranslucencyCheckbox = new CCvarToggleCheckButton(this, "CrosshairTranslucencyCheckbox", "#GameUI_Translucent", "cl_crosshairusealpha");
	m_pCrosshairImage = new CrosshairImagePanel( this, "CrosshairImage", m_pCrosshairTranslucencyCheckbox );

	// advanced crosshair controls
	//==========
	m_pAdvCrosshairRedSlider = new CCvarSlider( this, "Red Color Slider", "#GameUI_CrosshairColor_Red",
		0.0f, 255.0f, "cl_crosshair_red" );
	m_pAdvCrosshairGreenSlider = new CCvarSlider( this, "Green Color Slider", "#GameUI_CrosshairColor_Green",
		0.0f, 255.0f, "cl_crosshair_green" );
	m_pAdvCrosshairBlueSlider = new CCvarSlider( this, "Blue Color Slider", "#GameUI_CrosshairColor_Blue",
		0.0f, 255.0f, "cl_crosshair_blue" );

	m_pAdvCrosshairScaleSlider = new CCvarSlider( this, "Scale Slider", "#GameUI_CrosshairScale",
		16.0f, 48.0f, "cl_crosshair_scale" );

	m_pAdvCrosshairRedSlider->AddActionSignalTarget( this );
	m_pAdvCrosshairGreenSlider->AddActionSignalTarget( this );
	m_pAdvCrosshairBlueSlider->AddActionSignalTarget( this );
	m_pAdvCrosshairScaleSlider->AddActionSignalTarget( this );

	m_pAdvCrosshairStyle = new CLabeledCommandComboBox( this, "AdvCrosshairList" );
	m_pAdvCrosshairImage = new AdvancedCrosshairImagePanel( this, "AdvCrosshairImage" );

	InitAdvCrosshairStyleList(m_pAdvCrosshairStyle);
	RedrawAdvCrosshairImage();
	//=========
	
	m_pDownloadFilterCombo = new ComboBox( this, "DownloadFilterCheck", 3, false );
	m_pDownloadFilterCombo->AddItem( "#GameUI_DownloadFilter_ALL", NULL );
	m_pDownloadFilterCombo->AddItem( "#GameUI_DownloadFilter_NoSounds", NULL );
	m_pDownloadFilterCombo->AddItem( "#GameUI_DownloadFilter_None", NULL );

	//=========

	LoadControlSettings("Resource/OptionsSubMultiplayer.res");

	InitCrosshairColorEntries();
	InitCrosshairSizeList(m_pCrosshairSize);
	RedrawCrosshairImage();

	// turn off the crosshair stuff if the mod specifies "nocrosshair" in the gameinfo.txt file
	if ( ModInfo().NoCrosshair() )
	{
		m_pCrosshairColorComboBox->SetVisible( false );
		m_pCrosshairSize->SetVisible( false );
		m_pCrosshairTranslucencyCheckbox->SetVisible( false );
		m_pCrosshairImage->SetVisible( false );

		Panel *pTempPanel = NULL;

		// #GameUI_CrosshairDescription (from "Resource/OptionsSubMultiplayer.res")
		pTempPanel = FindChildByName( "CrosshairLabel" );

		if ( pTempPanel )
		{
			pTempPanel->SetVisible( false );
		}
	}

	// Advanced crosshair selection
	if  ( !ModInfo().AdvCrosshair() )
	{
		m_pAdvCrosshairImage->SetVisible( false );

		m_pAdvCrosshairRedSlider->SetVisible( false );		
		m_pAdvCrosshairBlueSlider->SetVisible( false );
		m_pAdvCrosshairGreenSlider->SetVisible( false );
		m_pAdvCrosshairScaleSlider->SetVisible( false );
		m_pAdvCrosshairStyle->SetVisible( false );

		Panel *pTempPanel = NULL;

		// #GameUI_AdvCrosshairDescription (from "Resource/OptionsSubMultiplayer.res")
		pTempPanel = FindChildByName( "AdvCrosshairLabel" );
		if ( pTempPanel )
		{
			pTempPanel->SetVisible( false );
		}
	}
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
COptionsSubMultiplayer::~COptionsSubMultiplayer()
{
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void COptionsSubMultiplayer::OnCommand( const char *command )
{
	if ( !stricmp( command, "Advanced" ) )
	{
		if (!m_hMultiplayerAdvancedDialog.Get())
			m_hMultiplayerAdvancedDialog = new CMultiplayerAdvancedDialog( this );

		m_hMultiplayerAdvancedDialog->Activate();
	}
	else if (!stricmp( command, "ImportSprayImage" ) )
	{
		if (m_hImportSprayDialog == NULL)
		{
			m_hImportSprayDialog = new FileOpenDialog(NULL, "#GameUI_ImportSprayImage", true);
			m_hImportSprayDialog->AddFilter("*.tga,*.jpg,*.bmp,*.vtf", "#GameUI_All_Images", true);
			m_hImportSprayDialog->AddFilter("*.tga", "#GameUI_TGA_Images", false);
			m_hImportSprayDialog->AddFilter("*.bmp", "#GameUI_BMP_Images", false);
			m_hImportSprayDialog->AddFilter("*.vtf", "#GameUI_VTF_Images", false);
			m_hImportSprayDialog->AddActionSignalTarget(this);
		}
		m_hImportSprayDialog->DoModal(false);
		m_hImportSprayDialog->Activate();
	}

	BaseClass::OnCommand( command );
}

// file selected.  This can only happen when someone selects an image to be imported as a spray logo.
void COptionsSubMultiplayer::OnFileSelected(const char *fullpath)
{
	if ((fullpath == NULL) || (fullpath[0] == 0))
	{
		return;
	}

	ConversionErrorType errcode;

	// this can take a while, put up a waiting cursor
	surface()->SetCursor(dc_hourglass);

	// get the extension of the file we're to convert
	char extension[MAX_PATH];
	const char *constchar = fullpath + strlen(fullpath);
	while ((constchar > fullpath) && (*(constchar-1) != '.'))
	{
		--constchar;
	}
	Q_strncpy(extension, constchar, MAX_PATH);

	bool deleteIntermediateTGA = false;
	bool deleteIntermediateVTF = false;
	bool convertTGAToVTF = true;
	char tgaPath[MAX_PATH*2];
	char *c;
	bool failed = false;

	Q_strncpy(tgaPath, fullpath, sizeof(tgaPath));

	if (stricmp(extension, "tga"))
	{
		// construct a .tga version of this file path.

		c = tgaPath + strlen(tgaPath);
		while ((c > tgaPath) && (*(c-1) != '\\') && (*(c-1) != '/'))
		{
			--c;
		}
		*c = 0;

		char origpath[MAX_PATH*2];
		Q_strncpy(origpath, tgaPath, sizeof(origpath));

		int index = 0;
		do {
			Q_snprintf(tgaPath, sizeof(tgaPath), "%stemp%d.tga", origpath, index);
			++index;
		} while (_access(tgaPath, 0) != -1);

		if (!stricmp(extension, "bmp"))
		{
			// convert from the bmp file format to the TGA file format
			errcode = ConvertBMPToTGA(fullpath, tgaPath);

			if (errcode == CE_SUCCESS)
			{
				deleteIntermediateTGA = true;
			}
			else
			{
				failed = true;
				vgui::MessageBox *errorDialog = NULL;

				if (errcode == CE_MEMORY_ERROR)
				{
					errorDialog = new vgui::MessageBox("#GameUI_Spray_Import_Error_Title", "#GameUI_Spray_Import_Error_Memory");
				}
				else if (errcode == CE_CANT_OPEN_SOURCE_FILE)
				{
					errorDialog = new vgui::MessageBox("#GameUI_Spray_Import_Error_Title", "#GameUI_Spray_Import_Error_Reading_Image");
				}
				else if (errcode == CE_ERROR_PARSING_SOURCE)
				{
					errorDialog = new vgui::MessageBox("#GameUI_Spray_Import_Error_Title", "#GameUI_Spray_Import_Error_Image_File_Corrupt");
				}
				else if (errcode == CE_SOURCE_FILE_FORMAT_NOT_SUPPORTED)
				{
					errorDialog = new vgui::MessageBox("#GameUI_Spray_Import_Error_Title", "#GameUI_Spray_Import_Error_BMP_Format_Not_Supported");
				}
				else if (errcode == CE_ERROR_WRITING_OUTPUT_FILE)
				{
					errorDialog = new vgui::MessageBox("#GameUI_Spray_Import_Error_Title", "#GameUI_Spray_Import_Error_Writing_Temp_Output");
				}

				if (errorDialog != NULL)
				{
					errorDialog->DoModal();
				}
			}
		}
		else if (!stricmp(extension, "vtf"))
		{
			// if the file is already in the vtf format there's no need to convert it.
			convertTGAToVTF = false;
		}
	}

	if (convertTGAToVTF && !failed)
	{
		// convert the TGA file to the VTF format.
		errcode = ConvertTGA(tgaPath); // resize TGA so that it has power-of-two dimensions with a max size of 256x256.
		if (errcode != CE_SUCCESS)
		{
			failed = true;
			vgui::MessageBox *errorDialog = NULL;

			if (errcode == CE_MEMORY_ERROR)
			{
				errorDialog = new vgui::MessageBox("#GameUI_Spray_Import_Error_Title", "#GameUI_Spray_Import_Error_Memory");
			}
			else if (errcode == CE_CANT_OPEN_SOURCE_FILE)
			{
				errorDialog = new vgui::MessageBox("#GameUI_Spray_Import_Error_Title", "#GameUI_Spray_Import_Error_Reading_Image");
			}
			else if (errcode == CE_ERROR_PARSING_SOURCE)
			{
				errorDialog = new vgui::MessageBox("#GameUI_Spray_Import_Error_Title", "#GameUI_Spray_Import_Error_Image_File_Corrupt");
			}
			else if (errcode == CE_SOURCE_FILE_FORMAT_NOT_SUPPORTED)
			{
				errorDialog = new vgui::MessageBox("#GameUI_Spray_Import_Error_Title", "#GameUI_Spray_Import_Error_TGA_Format_Not_Supported");
			}
			else if (errcode == CE_ERROR_WRITING_OUTPUT_FILE)
			{
				errorDialog = new vgui::MessageBox("#GameUI_Spray_Import_Error_Title", "#GameUI_Spray_Import_Error_Writing_Temp_Output");
			}

			if (errorDialog != NULL)
			{
				errorDialog->DoModal();
			}
		}

		if (!failed)
		{
			char tempPath[MAX_PATH*2];
			Q_strncpy(tempPath, tgaPath, sizeof(tempPath));

			errcode = ConvertTGAToVTF(tempPath);
			if (errcode == CE_SUCCESS)
			{
				deleteIntermediateVTF = true;
			}
			else
			{
				failed = true;
				vgui::MessageBox *errorDialog = NULL;

				if (errcode == CE_MEMORY_ERROR)
				{
					errorDialog = new vgui::MessageBox("#GameUI_Spray_Import_Error_Title", "#GameUI_Spray_Import_Error_Memory");
				}
				else if (errcode == CE_CANT_OPEN_SOURCE_FILE)
				{
					errorDialog = new vgui::MessageBox("#GameUI_Spray_Import_Error_Title", "#GameUI_Spray_Import_Error_Reading_Image");
				}
				else if (errcode == CE_ERROR_PARSING_SOURCE)
				{
					errorDialog = new vgui::MessageBox("#GameUI_Spray_Import_Error_Title", "#GameUI_Spray_Import_Error_Image_File_Corrupt");
				}
				else if (errcode == CE_SOURCE_FILE_FORMAT_NOT_SUPPORTED)
				{
					errorDialog = new vgui::MessageBox("#GameUI_Spray_Import_Error_Title", "#GameUI_Spray_Import_Error_TGA_Format_Not_Supported");
				}
				else if (errcode == CE_ERROR_WRITING_OUTPUT_FILE)
				{
					errorDialog = new vgui::MessageBox("#GameUI_Spray_Import_Error_Title", "#GameUI_Spray_Import_Error_Writing_Temp_Output");
				}
				else if (errcode == CE_ERROR_LOADING_DLL)
				{
					errorDialog = new vgui::MessageBox("#GameUI_Spray_Import_Error_Title", "#GameUI_Spray_Import_Error_Cant_Load_VTEX_DLL");
				}

				if (errorDialog != NULL)
				{
					errorDialog->DoModal();
				}
			}
		}
	}

	char finalPath[MAX_PATH*2];
	finalPath[0] = 0;
	char vtfPath[MAX_PATH*2];
	vtfPath[0] = 0;

	if (!failed)
	{
		Q_strncpy(vtfPath, tgaPath, sizeof(vtfPath));

		// rename the tga file to be a vtf file.
		c = vtfPath + strlen(vtfPath);
		while ((c > vtfPath) && (*(c-1) != '.'))
		{
			--c;
		}
		*c = 0;
		Q_strncat(vtfPath, "vtf", sizeof(vtfPath), COPY_ALL_CHARACTERS);

		// get the vtfFilename from the path.
		const char *vtfFilename = fullpath + strlen(fullpath);
		while ((vtfFilename > fullpath) && (*(vtfFilename-1) != '\\') && (*(vtfFilename-1) != '/'))
		{
			--vtfFilename;
		}

		Q_strncpy(finalPath, engine->GetGameDirectory(), sizeof(finalPath));
		Q_strncat(finalPath, "\\materials\\VGUI\\logos\\", sizeof(finalPath), COPY_ALL_CHARACTERS);
		Q_strncat(finalPath, vtfFilename, sizeof(finalPath), COPY_ALL_CHARACTERS);

		c = finalPath + strlen(finalPath);
		while ((c > finalPath) && (*(c-1) != '.'))
		{
			--c;
		}
		*c = 0;
		Q_strncat(finalPath,"vtf", sizeof(finalPath), COPY_ALL_CHARACTERS);

		// make sure the directory exists before we try to copy the file.
		g_pFullFileSystem->CreateDirHierarchy("materials/VGUI/logos/", "GAME");

		// write out the spray VMT file.
		errcode = WriteSprayVMT(finalPath);
		if (errcode != CE_SUCCESS)
		{
			failed = true;

			vgui::MessageBox *errorDialog = new vgui::MessageBox("#GameUI_Spray_Import_Error_Title", "#GameUI_Spray_Import_Error_Writing_Output");
			errorDialog->DoModal();
		}

		if (!failed)
		{
			// copy vtf file to the final location.
			CopyFile(vtfPath, finalPath, true);

			// refresh the logo list so the new spray shows up.
			InitLogoList(m_pLogoList);

			char rootFilename[MAX_PATH];
			Q_strncpy(rootFilename, vtfFilename, MAX_PATH);

			// get the root filename so we can select in the spray list.
			rootFilename[strlen(rootFilename) - 4] = 0;

			// automatically select the logo that was just imported.
			SelectLogo(rootFilename);
		}
	}

	// delete the intermediate VTF file if one was made.
	if (deleteIntermediateVTF)
	{
		DeleteFile(vtfPath);

		// the TGA->VTF conversion process generates a .txt file if one wasn't already there.
		// in this case, delete the .txt file.
		c = vtfPath + strlen(vtfPath);
		while ((c > vtfPath) && (*(c-1) != '.'))
		{
			--c;
		}
		Q_strncpy(c, "txt", sizeof(vtfPath)-(c-vtfPath));
		DeleteFile(vtfPath);
	}

	// delete the intermediate TGA file if one was made.
	if (deleteIntermediateTGA)
	{
		DeleteFile(tgaPath);
	}

	// change the cursor back to normal
	surface()->SetCursor(dc_user);
}

// convert the bmp file given to a TGA file at the given destination path.
ConversionErrorType COptionsSubMultiplayer::ConvertBMPToTGA(const char *bmpPath, const char *tgaPath)
{
	if ( !IsPC() )
		return CE_SOURCE_FILE_FORMAT_NOT_SUPPORTED;

	HBITMAP hBitmap = (HBITMAP)LoadImage(NULL, bmpPath, IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION | LR_LOADFROMFILE | LR_DEFAULTSIZE);
	BITMAP bitmap;

	GetObject(hBitmap, sizeof(bitmap), &bitmap);

	BITMAPINFO *bitmapInfo;

	if (bitmap.bmBitsPixel == 24)
	{
		bitmapInfo = (BITMAPINFO *)malloc(sizeof(BITMAPINFO));
	}
	else
	{
		int colorsUsed = 1 << bitmap.bmBitsPixel;
		bitmapInfo = (BITMAPINFO *)malloc(colorsUsed * sizeof(RGBQUAD) + sizeof(BITMAPINFO));
	}

	memset(bitmapInfo, 0, sizeof(BITMAPINFO));
	bitmapInfo->bmiHeader.biSize = sizeof(bitmapInfo->bmiHeader);
	if (bitmap.bmBitsPixel != 24)
	{
		bitmapInfo->bmiHeader.biBitCount = bitmap.bmBitsPixel; // need to specify the bits per pixel so GDI will generate a color table for us.
	}

	HDC dc = CreateCompatibleDC(NULL);

	int retcode = GetDIBits(dc, hBitmap, 0, bitmap.bmHeight, NULL, bitmapInfo, DIB_RGB_COLORS);

	DeleteDC(dc);

	if (retcode == 0)
	{
		// error getting the bitmap info for some reason.
		free(bitmapInfo);
		return CE_ERROR_PARSING_SOURCE;
	}

	int mem_required = 3 * bitmap.bmWidth * bitmap.bmHeight;  // mem required for copying the data out into RGB format.

	unsigned char *buf = (unsigned char *)malloc(mem_required);
	if (buf == NULL)
	{
		free(bitmapInfo);
		return CE_MEMORY_ERROR;
	}

	if (bitmapInfo->bmiHeader.biBitCount == 24)
	{
		if ((bitmap.bmWidth * 3) == bitmap.bmWidthBytes)
		{
			// 24-bit BGR color bitmap that is word aligned.
			memcpy(buf, bitmap.bmBits, mem_required);
		}
		else
		{
			// 24-bit BGR color bitmap that is not word aligned.
			// need to read it in one row at a time since it isn't word aligned.
			int row;
			for (row = 0; row < bitmap.bmHeight; ++row)
			{
				memcpy(buf + (row * 3 * bitmap.bmWidth), (unsigned char *)(bitmap.bmBits) + (row * bitmap.bmWidthBytes), 3 * bitmap.bmWidth);
			}
		}

		// bitmaps are loaded as BGR, need to convert it to RGB.

		int numPixels = bitmap.bmHeight * bitmap.bmWidth;

		// convert to RGB
		int index;
		for (index = 0; index < numPixels; ++index)
		{
			unsigned char blue = buf[3 * index]; // save blue value
			buf[3 * index] = buf[3 * index + 2]; // copy red value.
			buf[3 * index + 2] = blue; // copy blue value.
		}
	}
	else if (bitmapInfo->bmiHeader.biBitCount == 8)
	{
		// 8-bit 256 color bitmap.
		int y, x, index, colorTableEntry;
		for (y = 0; y < bitmap.bmHeight; ++y)
		{
			for (x = 0; x < bitmap.bmWidth; ++x)
			{
				// compute the color map entry for this pixel
				index = y * bitmap.bmWidthBytes + x;
				colorTableEntry = ((unsigned char *)bitmap.bmBits)[index];

				// get the color for this color map entry.
				RGBQUAD *rgbQuad = &(bitmapInfo->bmiColors[colorTableEntry]);

				// copy the color values for this pixel to the destination buffer.
				buf[(y * bitmap.bmWidth * 3) + (x * 3)] = rgbQuad->rgbRed;
				buf[(y * bitmap.bmWidth * 3) + (x * 3) + 1] = rgbQuad->rgbGreen;
				buf[(y * bitmap.bmWidth * 3) + (x * 3) + 2] = rgbQuad->rgbBlue;
			}
		}
	}
	else if (bitmapInfo->bmiHeader.biBitCount == 4)
	{
		// 4-bit 16 color bitmap.
		int y, x, index, colorTableEntry;
		for (y = 0; y < bitmap.bmHeight; ++y)
		{
			for (x = 0; x < bitmap.bmWidth; x += 2)
			{
				// two color table entries per byte
				index = y * bitmap.bmWidthBytes + x / 2;

				// get the color table entry for this pixel
				colorTableEntry = (0xf0 & ((unsigned char *)bitmap.bmBits)[index]) >> 4;

				// get the color values for this pixel's color table entry.
				RGBQUAD *rgbQuad = &(bitmapInfo->bmiColors[colorTableEntry]);

				// copy the pixel's color values to the destination buffer.
				buf[(y * bitmap.bmWidth * 3) + (x * 3)] = rgbQuad->rgbRed;
				buf[(y * bitmap.bmWidth * 3) + (x * 3) + 1] = rgbQuad->rgbGreen;
				buf[(y * bitmap.bmWidth * 3) + (x * 3) + 2] = rgbQuad->rgbBlue;

				// make sure we haven't reached the end of the row.
				if ((x + 1) > bitmap.bmWidth)
				{
					break;
				}

				// get the color table entry for this pixel.
				colorTableEntry = 0x0f & ((unsigned char *)bitmap.bmBits)[index];

				// get the color values for this pixel's color table entry.
				rgbQuad = &(bitmapInfo->bmiColors[colorTableEntry]);

				// copy the pixel's color values to the destination buffer.
				buf[(y * bitmap.bmWidth * 3) + ((x+1) * 3)] = rgbQuad->rgbRed;
				buf[(y * bitmap.bmWidth * 3) + ((x+1) * 3) + 1] = rgbQuad->rgbGreen;
				buf[(y * bitmap.bmWidth * 3) + ((x+1) * 3) + 2] = rgbQuad->rgbBlue;
			}
		}
	}
	else if (bitmapInfo->bmiHeader.biBitCount == 1)
	{
		// 1-bit monochrome bitmap.
		int y, x, index, bit, bitMask;
		for (y = 0; y < bitmap.bmHeight; ++y)
		{
			x = 0;
			while (x < bitmap.bmWidth)
			{
				RGBQUAD *rgbQuad = NULL;
				bitMask = 0x80;

				// get the index into the bitmap data for the next 8 pixels.
				index = y * bitmap.bmWidthBytes + x / 8;

				// go through all 8 bits in this byte to get all 8 pixel colors.
				do
				{
					// get the value of the bit for this pixel.
					bit = ((unsigned char *)bitmap.bmBits)[index] & bitMask;

					// bit will either be 0 or non-zero since there are only two colors.
					if (bit == 0)
					{
						rgbQuad = &(bitmapInfo->bmiColors[0]);
					}
					else
					{
						rgbQuad = &(bitmapInfo->bmiColors[1]);
					}

					// copy this pixel's color values into the destination buffer.
					buf[(y * bitmap.bmWidth * 3) + (x * 3)] = rgbQuad->rgbRed;
					buf[(y * bitmap.bmWidth * 3) + (x * 3) + 1] = rgbQuad->rgbGreen;
					buf[(y * bitmap.bmWidth * 3) + (x * 3) + 2] = rgbQuad->rgbBlue;

					// go to the next pixel.
					++x;
					bitMask = bitMask >> 1;
				} while ((x < bitmap.bmWidth) && (bitMask > 0));
			}
		}
	}
	else
	{
		free(bitmapInfo);
		free(buf);
		DeleteObject(hBitmap);

		return CE_SOURCE_FILE_FORMAT_NOT_SUPPORTED;
	}

	free(bitmapInfo);

	// bitmaps are stored upside down, so flip the image vertically
	int index;
	int rowWidth = 3 * bitmap.bmWidth;
	unsigned char *row = (unsigned char *)malloc(rowWidth);
	for (index = 0; index < (bitmap.bmHeight / 2); ++index)
	{
		memcpy(row, buf + (index * rowWidth), rowWidth);
		memcpy(buf + (index * rowWidth), buf + (((bitmap.bmHeight - 1) - index) * rowWidth), rowWidth);
		memcpy(buf + (((bitmap.bmHeight - 1) - index) * rowWidth), row, rowWidth);
	}

	// write out the TGA file using the RGB data buffer.
	CUtlBuffer outBuf;
	bool retval = TGAWriter::WriteToBuffer(buf, outBuf, bitmap.bmWidth, bitmap.bmHeight, IMAGE_FORMAT_RGB888, IMAGE_FORMAT_RGB888);
	if ( retval )
	{
		if ( !g_pFullFileSystem->WriteFile( tgaPath, NULL, outBuf ) )
		{
			retval = false;
		}
	}
	DeleteObject(hBitmap);
	return retval ? CE_SUCCESS : CE_ERROR_WRITING_OUTPUT_FILE;
}

// read a TGA header from the current point in the file stream.
static void ReadTGAHeader(FILE *infile, TGAHeader &header)
{
	if (infile == NULL)
	{
		return;
	}

	fread(&header.identsize, sizeof(header.identsize), 1, infile);
	fread(&header.colourmaptype, sizeof(header.colourmaptype), 1, infile);
	fread(&header.imagetype, sizeof(header.imagetype), 1, infile);
	fread(&header.colourmapstart, sizeof(header.colourmapstart), 1, infile);
	fread(&header.colourmaplength, sizeof(header.colourmaplength), 1, infile);
	fread(&header.colourmapbits, sizeof(header.colourmapbits), 1, infile);
	fread(&header.xstart, sizeof(header.xstart), 1, infile);
	fread(&header.ystart, sizeof(header.ystart), 1, infile);
	fread(&header.width, sizeof(header.width), 1, infile);
	fread(&header.height, sizeof(header.height), 1, infile);
	fread(&header.bits, sizeof(header.bits), 1, infile);
	fread(&header.descriptor, sizeof(header.descriptor), 1, infile);
}

// write a TGA header to the current point in the file stream.
static void WriteTGAHeader(FILE *outfile, TGAHeader &header)
{
	if (outfile == NULL)
	{
		return;
	}

	fwrite(&header.identsize, sizeof(header.identsize), 1, outfile);
	fwrite(&header.colourmaptype, sizeof(header.colourmaptype), 1, outfile);
	fwrite(&header.imagetype, sizeof(header.imagetype), 1, outfile);
	fwrite(&header.colourmapstart, sizeof(header.colourmapstart), 1, outfile);
	fwrite(&header.colourmaplength, sizeof(header.colourmaplength), 1, outfile);
	fwrite(&header.colourmapbits, sizeof(header.colourmapbits), 1, outfile);
	fwrite(&header.xstart, sizeof(header.xstart), 1, outfile);
	fwrite(&header.ystart, sizeof(header.ystart), 1, outfile);
	fwrite(&header.width, sizeof(header.width), 1, outfile);
	fwrite(&header.height, sizeof(header.height), 1, outfile);
	fwrite(&header.bits, sizeof(header.bits), 1, outfile);
	fwrite(&header.descriptor, sizeof(header.descriptor), 1, outfile);
}

// reads in a TGA file and converts it to 32 bit RGBA color values in a memory buffer.
unsigned char * COptionsSubMultiplayer::ReadTGAAsRGBA(const char *tgaPath, int &width, int &height, ConversionErrorType &errcode, TGAHeader &tgaHeader )
{
	FILE *tgaFile = fopen(tgaPath, "rb");
	if (tgaFile == NULL)
	{
		errcode = CE_CANT_OPEN_SOURCE_FILE;
		return NULL;
	}

	// read header for TGA file.
	ReadTGAHeader(tgaFile, tgaHeader);

	// image type 2 is RGB, other types not supported.
	if (tgaHeader.imagetype != 2)
	{
		fclose(tgaFile);

		errcode = CE_SOURCE_FILE_FORMAT_NOT_SUPPORTED;
		return NULL;
	}

	int tgaDataSize = tgaHeader.width * tgaHeader.height * tgaHeader.bits / 8;
	unsigned char *tgaData = (unsigned char *)malloc(tgaDataSize);
	if (tgaData == NULL)
	{
		fclose(tgaFile);

		errcode = CE_MEMORY_ERROR;
		return NULL;
	}

	fread(tgaData, 1, tgaDataSize, tgaFile);

	fclose(tgaFile);

	width = tgaHeader.width;
	height = tgaHeader.height;

	if (tgaHeader.bits == 24)
	{
		// image needs to be converted to a 32-bit image.
		int numPixels = tgaHeader.width * tgaHeader.height;

		unsigned char *retBuf = (unsigned char *)malloc(numPixels * 4);
		if (retBuf == NULL)
		{
			free(tgaData);

			errcode = CE_MEMORY_ERROR;
			return NULL;
		}

		// convert from RGB to RGBA color format.
		int index;
		for (index = 0; index < numPixels; ++index)
		{
			retBuf[index * 4] = tgaData[index * 3];
			retBuf[index * 4 + 1] = tgaData[index * 3 + 1];
			retBuf[index * 4 + 2] = tgaData[index * 3 + 2];
			retBuf[index * 4 + 3] = 0xff;
		}

		free(tgaData);

		tgaHeader.bits = 32;

		errcode = CE_SUCCESS;
		return retBuf;
	}
	else if (tgaHeader.bits == 32)
	{
		// already in RGBA format so just return it.
		errcode = CE_SUCCESS;
		return tgaData;
	}

	// format not supported, fail.
	free(tgaData);

	errcode = CE_SOURCE_FILE_FORMAT_NOT_SUPPORTED;
	return NULL;
}

// resizes the file specified by tgaPath so that it has dimensions that are
// powers-of-two and is equal to or smaller than 256x256.
// also converts from 24-bit RGB to 32-bit RGB (with 8-bit alpha)
ConversionErrorType COptionsSubMultiplayer::ConvertTGA(const char *tgaPath)
{
	int tgaWidth = 0, tgaHeight = 0;
	ConversionErrorType errcode;
	TGAHeader tgaHeader;
	unsigned char *srcBuffer = ReadTGAAsRGBA(tgaPath, tgaWidth, tgaHeight, errcode, tgaHeader);

	if (srcBuffer == NULL)
	{
		return errcode;
	}

	int paddedImageWidth, paddedImageHeight;

	if ((tgaWidth <= 0) || (tgaHeight <= 0))
	{
		free(srcBuffer);
		return CE_ERROR_PARSING_SOURCE;
	}

	// get the nearest power of two that is greater than the width of the image.
	paddedImageWidth = tgaWidth;
	if (!IsPowerOfTwo(paddedImageWidth))
	{
		// width is not a power of two, calculate the next highest power of two value.
		int i = 1;
		while (paddedImageWidth > 1)
		{
			paddedImageWidth = paddedImageWidth >> 1;
			++i;
		}

		paddedImageWidth = paddedImageWidth << i;
	}

	// make sure the width is less than or equal to 256
	if (paddedImageWidth > 256)
	{
		paddedImageWidth = 256;
	}

	// get the nearest power of two that is greater than the height of the image
	paddedImageHeight = tgaHeight;
	if (!IsPowerOfTwo(paddedImageHeight))
	{
		// height is not a power of two, calculate the next highest power of two value.
		int i = 1;
		while (paddedImageHeight > 1)
		{
			paddedImageHeight = paddedImageHeight >> 1;
			++i;
		}

		paddedImageHeight = paddedImageHeight << i;
	}

	// make sure the height is less than or equal to 256
	if (paddedImageHeight > 256)
	{
		paddedImageHeight = 256;
	}

	// compute the amount of stretching that needs to be done to both width and height to get the image to fit.
	float widthRatio = (float)paddedImageWidth / tgaWidth;
	float heightRatio = (float)paddedImageHeight / tgaHeight;

	int finalWidth;
	int finalHeight;

	// compute the final dimensions of the stretched image.
	if (widthRatio < heightRatio)
	{
		finalWidth = paddedImageWidth;
		finalHeight = (int)(tgaHeight * widthRatio + 0.5f);
											// i.e.  for 1x1 size pixels in the resized image we will take color from sourceRatio x sourceRatio sized pixels in the source image.
	}
	else if (heightRatio < widthRatio)
	{
		finalHeight = paddedImageHeight;
		finalWidth = (int)(tgaWidth * heightRatio + 0.5f);
	}
	else
	{
		finalHeight = paddedImageHeight;
		finalWidth = paddedImageWidth;
	}

	unsigned char *resizeBuffer = (unsigned char *)malloc(finalWidth * finalHeight * 4);

	// do the actual stretching
	StretchRGBAImage(srcBuffer, tgaWidth, tgaHeight, resizeBuffer, finalWidth, finalHeight);

	free(srcBuffer);  // don't need this anymore.

	///////////////////////////////////////////////////////////////////////
	///// need to pad the image so both dimensions are power of two's /////
	///////////////////////////////////////////////////////////////////////
	unsigned char *finalBuffer = (unsigned char *)malloc(paddedImageWidth * paddedImageHeight * 4);
	PadRGBAImage(resizeBuffer, finalWidth, finalHeight, finalBuffer, paddedImageWidth, paddedImageHeight);

	FILE *outfile = fopen(tgaPath, "wb");
	if (outfile == NULL)
	{
		free(resizeBuffer);
		free(finalBuffer);

		return CE_ERROR_WRITING_OUTPUT_FILE;
	}

	tgaHeader.width = paddedImageWidth;
	tgaHeader.height = paddedImageHeight;

	WriteTGAHeader(outfile, tgaHeader);

	fwrite(finalBuffer, 1, paddedImageWidth * paddedImageHeight * 4, outfile);

	fclose(outfile);

	free(resizeBuffer);
	free(finalBuffer);

	return CE_SUCCESS;
}

// resize by stretching (or compressing) an RGBA image pointed to by srcBuf into the buffer pointed to by destBuf.
// the buffers are assumed to be sized appropriately to accomidate RGBA images of the given widths and heights.
ConversionErrorType COptionsSubMultiplayer::StretchRGBAImage(const unsigned char *srcBuf, const int srcWidth, const int srcHeight,
											  unsigned char *destBuf, const int destWidth, const int destHeight)
{
	if ((srcBuf == NULL) || (destBuf == NULL))
	{
		return CE_CANT_OPEN_SOURCE_FILE;
	}

	int destRow,destColumn;

	float ratioX = (float)srcWidth / (float)destWidth;
	float ratioY = (float)srcHeight / (float)destHeight;

	// loop through all the pixels in the destination image.
	for (destRow = 0; destRow < destHeight; ++destRow)
	{
		for (destColumn = 0; destColumn < destWidth; ++destColumn)
		{
			// calculate the center of the pixel in the source image.
			float srcCenterX = ratioX * (destColumn + 0.5f);
			float srcCenterY = ratioY * (destRow + 0.5f);

			// calculate the starting and ending coords for this destination pixel in the source image.
			float srcStartX = srcCenterX - (ratioX / 2.0f);
			if (srcStartX < 0.0f)
			{
				srcStartX = 0.0f; // this should never happen, but just in case.
			}

			float srcStartY = srcCenterY - (ratioY / 2.0f);
			if (srcStartY < 0.0f)
			{
				srcStartY = 0.0f; // this should never happen, but just in case.
			}

			float srcEndX = srcCenterX + (ratioX / 2.0f);
			if (srcEndX > srcWidth)
			{
				srcEndX = srcWidth; // this should never happen, but just in case.
			}

			float srcEndY = srcCenterY + (ratioY / 2.0f);
			if (srcEndY > srcHeight)
			{
				srcEndY = srcHeight; // this should never happen, but just in case.
			}

			// Calculate the percentage of each source pixels' contribution to the destination pixel color.

			float srcCurrentX; // initialized at the start of the y loop.
			float srcCurrentY = srcStartY;

			float destRed = 0.0f;
			float destGreen = 0.0f;
			float destBlue = 0.0f;
			float destAlpha = 0.0f;

			//// loop for the parts of the source image that will contribute color to the destination pixel.
			while (srcCurrentY < srcEndY)
			{
				float srcCurrentEndY = (float)((int)srcCurrentY + 1);
				if (srcCurrentEndY > srcEndY)
				{
					srcCurrentEndY = srcEndY;
				}

				float srcCurrentHeight = srcCurrentEndY - srcCurrentY;

				srcCurrentX = srcStartX;

				while (srcCurrentX < srcEndX)
				{
					float srcCurrentEndX = (float)((int)srcCurrentX + 1);
					if (srcCurrentEndX > srcEndX)
					{
						srcCurrentEndX = srcEndX;
					}
					float srcCurrentWidth = srcCurrentEndX - srcCurrentX;

					// compute the percentage of the destination pixel's color this source pixel will contribute.
					float srcColorPercentage = (srcCurrentWidth / ratioX) * (srcCurrentHeight / ratioY);

					int srcCurrentPixelX = (int)srcCurrentX;
					int srcCurrentPixelY = (int)srcCurrentY;

					// get the color values for this source pixel.
					unsigned char srcCurrentRed = srcBuf[(srcCurrentPixelY * srcWidth * 4) + (srcCurrentPixelX * 4)];
					unsigned char srcCurrentGreen = srcBuf[(srcCurrentPixelY * srcWidth * 4) + (srcCurrentPixelX * 4) + 1];
					unsigned char srcCurrentBlue = srcBuf[(srcCurrentPixelY * srcWidth * 4) + (srcCurrentPixelX * 4) + 2];
					unsigned char srcCurrentAlpha = srcBuf[(srcCurrentPixelY * srcWidth * 4) + (srcCurrentPixelX * 4) + 3];

					// add the color contribution from this source pixel to the destination pixel.
					destRed += srcCurrentRed * srcColorPercentage;
					destGreen += srcCurrentGreen * srcColorPercentage;
					destBlue += srcCurrentBlue * srcColorPercentage;
					destAlpha += srcCurrentAlpha * srcColorPercentage;

					srcCurrentX = srcCurrentEndX;
				}

				srcCurrentY = srcCurrentEndY;
			}

			// assign the computed color to the destination pixel, round to the nearest value.  Make sure the value doesn't exceed 255.
			destBuf[(destRow * destWidth * 4) + (destColumn * 4)] = min((int)(destRed + 0.5f), 255);
			destBuf[(destRow * destWidth * 4) + (destColumn * 4) + 1] = min((int)(destGreen + 0.5f), 255);
			destBuf[(destRow * destWidth * 4) + (destColumn * 4) + 2] = min((int)(destBlue + 0.5f), 255);
			destBuf[(destRow * destWidth * 4) + (destColumn * 4) + 3] = min((int)(destAlpha + 0.5f), 255);
		} // column loop
	} // row loop

	return CE_SUCCESS;
}

ConversionErrorType COptionsSubMultiplayer::PadRGBAImage(const unsigned char *srcBuf, const int srcWidth, const int srcHeight,
										  unsigned char *destBuf, const int destWidth, const int destHeight)
{
	if ((srcBuf == NULL) || (destBuf == NULL))
	{
		return CE_CANT_OPEN_SOURCE_FILE;
	}

	memset(destBuf, 0, destWidth * destHeight * 4);

	if ((destWidth < srcWidth) || (destHeight < srcHeight))
	{
		return CE_ERROR_PARSING_SOURCE;
	}

	if ((srcWidth == destWidth) && (srcHeight == destHeight))
	{
		// no padding is needed, just copy the buffer straight over and call it done.
		memcpy(destBuf, srcBuf, destWidth * destHeight * 4);
		return CE_SUCCESS;
	}

	if (destWidth == srcWidth)
	{
		// only the top and bottom of the image need padding.
		// do this separately since we can do this more efficiently than the other cases.
		int numRowsToPad = (destHeight - srcHeight) / 2;
		memcpy(destBuf + (numRowsToPad * destWidth * 4), srcBuf, srcWidth * srcHeight * 4);
	}
	else
	{
		int numColumnsToPad = (destWidth - srcWidth) / 2;
		int numRowsToPad = (destHeight - srcHeight) / 2;
		int lastRow = numRowsToPad + srcHeight;
		int row;
		for (row = numRowsToPad; row < lastRow; ++row)
		{
			unsigned char * destOffset = destBuf + (row * destWidth * 4) + (numColumnsToPad * 4);
			const unsigned char * srcOffset = srcBuf + ((row - numRowsToPad) * srcWidth * 4);
			memcpy(destOffset, srcOffset, srcWidth * 4);
		}
	}

	return CE_SUCCESS;
}

// convert TGA file at the given location to a VTF file of the same root name at the same location.
ConversionErrorType COptionsSubMultiplayer::ConvertTGAToVTF(const char *tgaPath)
{
	FILE *infile = fopen(tgaPath, "rb");
	if (infile == NULL)
	{
		return CE_CANT_OPEN_SOURCE_FILE;
	}

	// read out the header of the image.
	TGAHeader header;
	ReadTGAHeader(infile, header);

	// check to make sure that the TGA has the proper dimensions and size.
	if (!IsPowerOfTwo(header.width) || !IsPowerOfTwo(header.height))
	{
		fclose(infile);
		return CE_SOURCE_FILE_FORMAT_NOT_SUPPORTED;
	}
	
	// check to make sure that the TGA isn't too big.
	if ((header.width > 256) || (header.height > 256))
	{
		fclose(infile);
		return CE_SOURCE_FILE_FORMAT_NOT_SUPPORTED;
	}

	int imageMemoryFootprint = header.width * header.height * header.bits / 8;

	CUtlBuffer inbuf(0, imageMemoryFootprint);

	// read in the image
	int nBytesRead = fread(inbuf.Base(), imageMemoryFootprint, 1, infile);

	fclose(infile);
	inbuf.SeekPut( CUtlBuffer::SEEK_HEAD, nBytesRead );

	// load vtex_dll.dll and get the interface to it.
	CSysModule *vtexmod = Sys_LoadModule("vtex_dll");
	if (vtexmod == NULL)
	{
		return CE_ERROR_LOADING_DLL;
	}

	CreateInterfaceFn factory = Sys_GetFactory(vtexmod);
	if (factory == NULL)
	{
		Sys_UnloadModule(vtexmod);
		return CE_ERROR_LOADING_DLL;
	}

	IVTex *vtex = (IVTex *)factory(IVTEX_VERSION_STRING, NULL);
	if (vtex == NULL)
	{
		Sys_UnloadModule(vtexmod);
		return CE_ERROR_LOADING_DLL;
	}

	char *vtfParams[4];

	// the 0th entry is skipped cause normally thats the program name.
	vtfParams[0] = "";
	vtfParams[1] = "-quiet";
	vtfParams[2] = "-dontusegamedir";
	vtfParams[3] = (char *)tgaPath;

	// call vtex to do the conversion.
	vtex->VTex(4, vtfParams);

	Sys_UnloadModule(vtexmod);

	return CE_SUCCESS;
}

// write a VMT file for the spray VTF file at the given path.
ConversionErrorType COptionsSubMultiplayer::WriteSprayVMT(const char *vtfPath)
{
	if (vtfPath == NULL)
	{
		return CE_ERROR_WRITING_OUTPUT_FILE;
	}

	// make the vmt filename
	char vmtPath[MAX_PATH*4];
	Q_strncpy(vmtPath, vtfPath, sizeof(vmtPath));
	char *c = vmtPath + strlen(vmtPath);
	while ((c > vmtPath) && (*(c-1) != '.'))
	{
		--c;
	}
	Q_strncpy(c, "vmt", sizeof(vmtPath) - (c - vmtPath));

	// get the root filename for the vtf file
	char filename[MAX_PATH];
	while ((c > vmtPath) && (*(c-1) != '/') && (*(c-1) != '\\'))
	{
		--c;
	}

	int i = 0;
	while ((*c != 0) && (*c != '.'))
	{
		filename[i++] = *(c++);
	}
	filename[i] = 0;

	// create the vmt file.
	FILE *vmtFile = fopen(vmtPath, "w");
	if (vmtFile == NULL)
	{
		return CE_ERROR_WRITING_OUTPUT_FILE;
	}

	// write the contents of the file.
	fprintf(vmtFile, "LightmappedGeneric\n{\n\t\"$basetexture\"	\"vgui\\logos\\%s\"\n\t\"$translucent\" \"1\"\n\t\"$decal\" \"1\"\n\t\"$decalscale\" \"0.250\"\n}\n", filename);

	fclose(vmtFile);

	return CE_SUCCESS;
}


//-----------------------------------------------------------------------------
// Purpose: Builds the list of logos
//-----------------------------------------------------------------------------
void COptionsSubMultiplayer::InitLogoList( CLabeledCommandComboBox *cb )
{
	// Find out images
	FileFindHandle_t fh;
	char directory[ 512 ];

	ConVarRef cl_logofile( "cl_logofile" );
	if ( !cl_logofile.IsValid() )
		return;

	cb->DeleteAllItems();

	const char *logofile = cl_logofile.GetString();
	Q_snprintf( directory, sizeof( directory ), "materials/vgui/logos/*.vtf" );
	const char *fn = g_pFullFileSystem->FindFirst( directory, &fh );
	int i = 0, initialItem = 0; 
	while (fn)
	{
		char filename[ 512 ];
		Q_snprintf( filename, sizeof(filename), "materials/vgui/logos/%s", fn );
		if ( strlen( filename ) >= 4 )
		{
			filename[ strlen( filename ) - 4 ] = 0;
			Q_strncat( filename, ".vmt", sizeof( filename ), COPY_ALL_CHARACTERS );
			if ( g_pFullFileSystem->FileExists( filename ) )
			{
				// strip off the extension
				Q_strncpy( filename, fn, sizeof( filename ) );
				filename[ strlen( filename ) - 4 ] = 0;
				cb->AddItem( filename, "" );

				// check to see if this is the one we have set
				Q_snprintf( filename, sizeof(filename), "materials/vgui/logos/%s", fn );
				if (!Q_stricmp(filename, logofile))
				{
					initialItem = i;
				}

				++i;
			}
		}

		fn = g_pFullFileSystem->FindNext( fh );
	}

	g_pFullFileSystem->FindClose( fh );
	cb->SetInitialItem(initialItem);
}


//-----------------------------------------------------------------------------
// Purpose: Selects the given logo in the logo list.
//-----------------------------------------------------------------------------
void COptionsSubMultiplayer::SelectLogo(const char *logoName)
{
	int numEntries = m_pLogoList->GetItemCount();
	int index;
	wchar_t itemText[MAX_PATH];
	wchar_t itemToSelectText[MAX_PATH];

	// convert the logo filename to unicode
	g_pVGuiLocalize->ConvertANSIToUnicode(logoName, itemToSelectText, sizeof(itemToSelectText));

	// find the index of the spray we want.
	for (index = 0; index < numEntries; ++index)
	{
		m_pLogoList->GetItemText(index, itemText, sizeof(itemText));
		if (!wcscmp(itemText, itemToSelectText))
		{
			break;
		}
	}

	if (index < numEntries)
	{
		// select the logo.
		m_pLogoList->ActivateItem(index);
	}
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void COptionsSubMultiplayer::InitCrosshairColorEntries()
{
	// parse the string for the custom color settings and get the initial settings.
	ConVarRef cl_crosshaircolor( "cl_crosshaircolor" );
	int index = 0;
	if ( cl_crosshaircolor.IsValid() )
	{
		index = clamp( cl_crosshaircolor.GetInt(), 0, NumCrosshairColors );
	}

	if (m_pCrosshairColorComboBox != NULL)
	{
		KeyValues *data = new KeyValues("data");

		// add in the "Default" selection
		data->Clear();

		// add in the colors for the color list
		for ( int i = 0; i < NumCrosshairColors; i++ )
		{
			data->SetInt("color", i);
			m_pCrosshairColorComboBox->AddItem( s_crosshairColors[ i ].name, data);
		}

		m_pCrosshairColorComboBox->ActivateItemByRow(index);

		data->deleteThis();
	}

	// force the crosshair to redraw.
	RedrawCrosshairImage();  // don't need to do this since we're not drawing anything.
}

//-----------------------------------------------------------------------------
// Purpose: takes the settings from the crosshair settings combo boxes and sliders
//          and apply it to the crosshair illustrations.
//-----------------------------------------------------------------------------
void COptionsSubMultiplayer::RedrawCrosshairImage()
{
	if (m_pCrosshairColorComboBox == NULL)
	{
		return;
	}

	bool enableApplyButton = false;

	// get the color selected in the combo box.
	KeyValues *data = m_pCrosshairColorComboBox->GetActiveItemUserData();
	int colorIndex = data->GetInt("color");
	colorIndex = clamp( colorIndex, 0, NumCrosshairColors );

	int selectedVal = 0;
	int actualVal = 0;
	if (m_pCrosshairColorComboBox != NULL)
	{
		selectedVal = m_pCrosshairColorComboBox->GetActiveItem();
	}

	ConVarRef cl_crosshaircolor( "cl_crosshaircolor" );
	if ( cl_crosshaircolor.IsValid() )
	{
		actualVal = clamp( cl_crosshaircolor.GetInt(), 0, NumCrosshairColors );
	}

	if ( selectedVal != actualVal )
	{
		enableApplyButton = true;
	}

	if (enableApplyButton)
	{
		OnApplyButtonEnable();
	}

	if ( m_pCrosshairImage && m_pCrosshairSize )
	{
		int size = m_pCrosshairSize->GetActiveItem();
		m_pCrosshairImage->UpdateCrosshair(
			s_crosshairColors[selectedVal].r,
			s_crosshairColors[selectedVal].g,
			s_crosshairColors[selectedVal].b,
			size );
	}
}

//-----------------------------------------------------------------------------
// Purpose: takes the settings from the crosshair settings combo boxes and sliders
//          and apply it to the crosshair illustrations.
//-----------------------------------------------------------------------------
void COptionsSubMultiplayer::RedrawAdvCrosshairImage()
{
	if ( !ModInfo().AdvCrosshair() )
	{
		return;
	}

	// get the color selected in the combo box.
	int r,g,b;

	r = clamp( m_pAdvCrosshairRedSlider->GetSliderValue(), 0, 255 );
	g = clamp( m_pAdvCrosshairGreenSlider->GetSliderValue(), 0, 255 );
	b = clamp( m_pAdvCrosshairBlueSlider->GetSliderValue(), 0, 255 );

	float scale = m_pAdvCrosshairScaleSlider->GetSliderValue();

	if ( m_pAdvCrosshairImage && m_pAdvCrosshairStyle )
	{
		char crosshairname[256];
		m_pAdvCrosshairStyle->GetText( crosshairname, sizeof(crosshairname)	);

		char texture[ 256 ];
		Q_snprintf ( texture, sizeof( texture ), "vgui/crosshairs/%s", crosshairname );

		m_pAdvCrosshairImage->UpdateCrosshair( r, g, b, scale, texture );
	}
}

//-----------------------------------------------------------------------------
// Purpose: initialize the crosshair size list.
//-----------------------------------------------------------------------------
void COptionsSubMultiplayer::InitCrosshairSizeList(CLabeledCommandComboBox *cb)
{
	if (cb == NULL)
	{
		return;
	}

	cb->Reset();

	// add in the auto, small, medium, and large size selections.
	cb->AddItem("#GameUI_Auto", "cl_crosshairscale 0");
	cb->AddItem("#GameUI_Small", "cl_crosshairscale 1200");
	cb->AddItem("#GameUI_Medium", "cl_crosshairscale 768");
	cb->AddItem("#GameUI_Large", "cl_crosshairscale 600");

	// parse out the size value from the cvar and set the initial value.
	int initialScale = 0;
	ConVarRef cl_crosshairscale( "cl_crosshairscale" );
	if ( cl_crosshairscale.IsValid() )
	{
		initialScale = cl_crosshairscale.GetInt();
		if ( initialScale <= 0 )
		{
			initialScale = 0;
		}
		else if ( initialScale <= 600 )
		{
			initialScale = 3;
		}
		else if ( initialScale <= 768 )
		{
			initialScale = 2;
		}
		else
		{
			initialScale = 1;
		}
	}
	cb->SetInitialItem( initialScale );
}

//-----------------------------------------------------------------------------
// Purpose: initialize the crosshair style list
//-----------------------------------------------------------------------------
void COptionsSubMultiplayer::InitAdvCrosshairStyleList(CLabeledCommandComboBox *cb)
{
	// Find out images
	FileFindHandle_t fh;
	char directory[ 512 ];

	ConVarRef cl_crosshair_file( "cl_crosshair_file" );
	if ( !cl_crosshair_file.IsValid() )
		return;

	cb->DeleteAllItems();

	char crosshairfile[256];
	Q_snprintf( crosshairfile, sizeof(crosshairfile), "materials/vgui/crosshairs/%s.vtf", cl_crosshair_file.GetString() );

	Q_snprintf( directory, sizeof( directory ), "materials/vgui/crosshairs/*.vtf" );
	const char *fn = g_pFullFileSystem->FindFirst( directory, &fh );
	int i = 0, initialItem = 0; 
	while (fn)
	{
		char filename[ 512 ];
		Q_snprintf( filename, sizeof(filename), "materials/vgui/crosshairs/%s", fn );
		if ( strlen( filename ) >= 4 )
		{
			filename[ strlen( filename ) - 4 ] = 0;
			Q_strncat( filename, ".vmt", sizeof( filename ), COPY_ALL_CHARACTERS );
			if ( g_pFullFileSystem->FileExists( filename ) )
			{
				// strip off the extension
				Q_strncpy( filename, fn, sizeof( filename ) );
				filename[ strlen( filename ) - 4 ] = 0;
				cb->AddItem( filename, "" );

				// check to see if this is the one we have set
				Q_snprintf( filename, sizeof(filename), "materials/vgui/crosshairs/%s", fn );
				if (!stricmp(filename, crosshairfile))
				{
					initialItem = i;
				}

				++i;
			}
		}

		fn = g_pFullFileSystem->FindNext( fh );
	}

	g_pFullFileSystem->FindClose( fh );
	cb->SetInitialItem(initialItem);
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void COptionsSubMultiplayer::RemapLogo()
{
	char logoname[256];

	m_pLogoList->GetText( logoname, sizeof( logoname ) );
	if( !logoname[ 0 ] )
		return;

	char fullLogoName[512];

	// make sure there is a version with the proper shader
	g_pFullFileSystem->CreateDirHierarchy( "materials/VGUI/logos/UI", "GAME" );
	Q_snprintf( fullLogoName, sizeof( fullLogoName ), "materials/VGUI/logos/UI/%s.vmt", logoname );
	if ( !g_pFullFileSystem->FileExists( fullLogoName ) )
	{
		FileHandle_t fp = g_pFullFileSystem->Open( fullLogoName, "wb" );
		if ( !fp )
			return;

		char data[1024];
		Q_snprintf( data, sizeof( data ), "\"UnlitGeneric\"\n\
{\n\
	// Original shader: BaseTimesVertexColorAlphaBlendNoOverbright\n\
	\"$translucent\" 1\n\
	\"$basetexture\" \"VGUI\\logos\\%s\"\n\
	\"$vertexcolor\" 1\n\
	\"$vertexalpha\" 1\n\
	\"$no_fullbright\" 1\n\
	\"$ignorez\" 1\n\
}\n\
", logoname );

		g_pFullFileSystem->Write( data, strlen( data ), fp );
		g_pFullFileSystem->Close( fp );
	}

	Q_snprintf( fullLogoName, sizeof( fullLogoName ), "logos/UI/%s", logoname );
	m_pLogoImage->SetImage( fullLogoName );
}

//-----------------------------------------------------------------------------
// Purpose: Called whenever model name changes
//-----------------------------------------------------------------------------
void COptionsSubMultiplayer::OnTextChanged(vgui::Panel *panel)
{
	RemapLogo();

	RedrawCrosshairImage(); // redraw the crosshair.

	RedrawAdvCrosshairImage();
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void COptionsSubMultiplayer::OnSliderMoved(KeyValues *data)
{
	RedrawAdvCrosshairImage();
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void COptionsSubMultiplayer::OnApplyButtonEnable()
{
	PostMessage(GetParent(), new KeyValues("ApplyButtonEnable"));
	InvalidateLayout();
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void COptionsSubMultiplayer::OnResetData()
{
	// reset the DownloadFilter combo box
	if ( m_pDownloadFilterCombo )
	{
		// cl_downloadfilter
		ConVarRef  cl_downloadfilter( "cl_downloadfilter" );

		if ( Q_stricmp( cl_downloadfilter.GetString(), "none" ) == 0 )
		{
			m_pDownloadFilterCombo->ActivateItem( 2 );
		}
		else if ( Q_stricmp( cl_downloadfilter.GetString(), "nosounds" ) == 0 )
		{
			m_pDownloadFilterCombo->ActivateItem( 1 );
		}
		else
		{
			m_pDownloadFilterCombo->ActivateItem( 0 );
		}
	}
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void COptionsSubMultiplayer::OnApplyChanges()
{
//	m_pModelList->ApplyChanges();
	m_pLogoList->ApplyChanges();
	m_pLogoList->GetText(m_LogoName, sizeof(m_LogoName));

	for ( int i=0; i<m_cvarToggleCheckButtons.GetCount(); ++i )
	{
		CCvarToggleCheckButton *toggleButton = m_cvarToggleCheckButtons[i];
		if( toggleButton->IsVisible() && toggleButton->IsEnabled() )
		{
			toggleButton->ApplyChanges();
		}
	}

	if ( !ModInfo().NoCrosshair() )
	{
		if (m_pCrosshairSize != NULL)
		{
			m_pCrosshairSize->ApplyChanges();
		}

		if (m_pCrosshairTranslucencyCheckbox != NULL)
		{
			m_pCrosshairTranslucencyCheckbox->ApplyChanges();
		}

		ApplyCrosshairColorChanges();
	}

	if ( ModInfo().AdvCrosshair() )
	{
		m_pAdvCrosshairRedSlider->ApplyChanges();
		m_pAdvCrosshairGreenSlider->ApplyChanges();
		m_pAdvCrosshairBlueSlider->ApplyChanges();
		m_pAdvCrosshairScaleSlider->ApplyChanges();
		m_pAdvCrosshairStyle->ApplyChanges();

		// save the crosshair
		char cmd[512];
		char crosshair[256];
		m_pAdvCrosshairStyle->GetText(crosshair, sizeof(crosshair));
		Q_snprintf(cmd, sizeof(cmd), "cl_crosshair_file %s\n", crosshair);
		engine->ClientCmd_Unrestricted(cmd);
	}

	// save the logo name
	char cmd[512];
	if ( m_LogoName[ 0 ] )
	{
		Q_snprintf(cmd, sizeof(cmd), "cl_logofile materials/vgui/logos/%s.vtf\n", m_LogoName);
	}
	else
	{
		Q_strncpy( cmd, "cl_logofile \"\"\n", sizeof( cmd ) );
	}
	engine->ClientCmd_Unrestricted(cmd);

	// set the DownloadFilter cvar
	if ( m_pDownloadFilterCombo )
	{
		ConVarRef  cl_downloadfilter( "cl_downloadfilter" );
		
		switch ( m_pDownloadFilterCombo->GetActiveItem() )
		{
		default:
		case 0:
			cl_downloadfilter.SetValue( "all" );
			break;
		case 1:
			cl_downloadfilter.SetValue( "nosounds" );
			break;
		case 2:
			cl_downloadfilter.SetValue( "none" );
			break;
		}
	}
}

//-----------------------------------------------------------------------------
// Purpose: apply the crosshair color values to the cvar.
//          also set the slider values to match the new value.
//-----------------------------------------------------------------------------
void COptionsSubMultiplayer::ApplyCrosshairColorChanges()
{
	char cmd[256];
	cmd[0] = 0;

	if (m_pCrosshairColorComboBox != NULL)
	{
		int val = m_pCrosshairColorComboBox->GetActiveItem();
		Q_snprintf( cmd, sizeof(cmd), "cl_crosshaircolor %d\n", val );
		engine->ClientCmd_Unrestricted( cmd );
	}
}

//-----------------------------------------------------------------------------
// Purpose: Allow the res file to create controls on per-mod basis
//-----------------------------------------------------------------------------
Panel *COptionsSubMultiplayer::CreateControlByName( const char *controlName )
{
	if( !Q_stricmp( "CCvarToggleCheckButton", controlName ) )
	{
		CCvarToggleCheckButton *newButton = new CCvarToggleCheckButton( this, controlName, "", "" );
		m_cvarToggleCheckButtons.AddElement( newButton );
		return newButton;
	}
	else
	{
		return BaseClass::CreateControlByName( controlName );
	}
}
