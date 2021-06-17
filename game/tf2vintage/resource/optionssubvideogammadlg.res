"resource/OptionsSubVideoGammaDlg.res"
{
	"OptionsSubVideoGammaDlg"
	{
		"ControlName"							"EditablePanel"
		"fieldName"								"OptionsSubVideoGammaDlg"
		"xpos"									"c-150"
		"ypos"									"50"
		"wide"									"290"
		"tall"									"396"
		"autoResize"							"0"
		"pinCorner"								"0"
		"visible"								"1"
		"enabled"								"1"
		"tabPosition"							"0"
		"settitlebarvisible"					"1"
		"PaintBackgroundType"					"2"
		"bgcolor_override"						"46 43 42 0"
		"infocus_bgcolor_override"				"46 43 42 0"
		"outoffocus_bgcolor_override"			"46 43 42 0"
		"border"								"GrayDialogBorder"
		"clientinsetx_override"					"0"
	}
	"Gamma"
	{
		"ControlName"		"CCvarSlider"
		"fieldName"			"Gamma"
		"xpos"				"17"
		"ypos"				"112"
		"wide"				"200"
		"tall"				"40"
		"autoResize"		"0"
		"pinCorner"			"0"
		"visible"			"1"
		"enabled"			"1"
		"tabPosition"		"1"
		"leftText"			"#GameUI_LIGHT"
		"rightText"			"#GameUI_DARK"
	}
	"GammaLabel"
	{
		"ControlName"		"CExLabel"
		"fieldName"			"GammaLabel"
		"xpos"				"16"
		"ypos"				"88"
		"wide"				"108"
		"tall"				"24"
		"autoResize"		"0"
		"pinCorner"			"0"
		"visible"			"1"
		"enabled"			"1"
		"tabPosition"		"0"
		"labelText"			"#GameUI_Gamma"
		"textAlignment"		"west"
		"dulltext"			"0"
		"brighttext"		"0"
		"wrap"				"0"
		"font"				"HudFontSmallestBold"
	}
	"GammaEntry"
	{
		"ControlName"			"TextEntry"
		"fieldName"				"GammaEntry"
		"xpos"					"228"
		"ypos"					"114"
		"wide"					"44"
		"tall"					"24"
		"autoResize"			"0"
		"pinCorner"				"0"
		"visible"				"1"
		"enabled"				"1"
		"tabPosition"			"2"
		"textHidden"			"0"
		"editable"				"1"
		"maxchars"				"-1"
		"NumericInputOnly"		"1"
		"unicode"				"0"
	}
	"OKButton"
	{
		"ControlName"		"CExButton"
		"fieldName"			"OKButton"
		"xpos"				"104"
		"ypos"				"356"
		"wide"				"80"
		"tall"				"24"
		"autoResize"		"0"
		"pinCorner"			"0"
		"visible"			"1"
		"enabled"			"1"
		"tabPosition"		"3"
		"labelText"			"#vgui_ok"
		"font"				"HudFontSmallBold"
		"textAlignment"		"center"
		"dulltext"			"0"
		"brighttext"		"0"
		"wrap"				"0"
		"Default"			"1"
		"sound_depressed"	"UI/buttonclick.wav"
		"sound_released"	"UI/buttonclickrelease.wav"
	}
	"ImagePanel1"
	{
		"ControlName"		"ImagePanel"
		"fieldName"			"ImagePanel1"
		"xpos"				"16"
		"ypos"				"156"
		"zpos"				"-1"
		"wide"				"256"
		"tall"				"252"
		"autoResize"		"0"
		"pinCorner"			"0"
		"visible"			"1"
		"enabled"			"1"
		"tabPosition"		"0"
		"image"				"gamma"
		"scaleImage"		"1"
	}
	"CancelButton"
	{
		"ControlName"		"CExButton"
		"fieldName"			"CancelButton"
		"xpos"				"192"
		"ypos"				"356"
		"wide"				"80"
		"tall"				"24"
		"autoResize"		"0"
		"pinCorner"			"0"
		"visible"			"1"
		"enabled"			"1"
		"tabPosition"		"4"
		"labelText"			"#vgui_Cancel"
		"font"				"HudFontSmallBold"
		"textAlignment"		"center"
		"dulltext"			"0"
		"brighttext"		"0"
		"wrap"				"0"
		"Command"			"Close"
		"Default"			"0"
		"sound_depressed"	"UI/buttonclick.wav"
		"sound_released"	"UI/buttonclickrelease.wav"
	}
	"GammaInfo"
	{
		"ControlName"		"CExLabel"
		"fieldName"			"GammaInfo"
		"xpos"				"16"
		"ypos"				"36"
		"wide"				"264"
		"tall"				"53"
		"autoResize"		"0"
		"pinCorner"			"0"
		"visible"			"1"
		"enabled"			"1"
		"tabPosition"		"0"
		"labelText"			"#GameUI_GammaInfo"
		"textAlignment"		"north-west"
		"font"				"HudFontSmallBold"
		"dulltext"			"0"
		"brighttext"		"0"
		"wrap"				"1"
	}
}