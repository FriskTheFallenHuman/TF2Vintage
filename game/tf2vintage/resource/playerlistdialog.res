"Resource/PlayerListDialog.res"
{
	"PlayerListDialog"
	{
		"ControlName"							"EditablePanel"
		"fieldName"								"PlayerListDialog"
		"xpos"									"c-220"
		"ypos"									"60"
		"wide"									"470"
		"tall"									"370"
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
	"TitleLabel"
	{
		"ControlName"			"CExLabel"
		"fieldName"				"TitleLabel"
		"font"					"HudFontMediumBold"
		"labelText"				"#GameUI_CurrentPlayers"
		"textAlignment"			"center"
		"xpos"					"0"
		"ypos"					"0"
		"zpos"					"1"
		"wide"					"500"
		"tall"					"40"
		"autoResize"			"0"
		"pinCorner"				"0"
		"visible"				"1"
		"enabled"				"1"
		"fgcolor_override" 		"200 80 60 255"
	}
	"MuteButton"
	{
		"ControlName"		"Button"
		"fieldName"			"MuteButton"
		"xpos"				"93"
		"ypos"				"326"
		"wide"				"140"
		"tall"				"25"
		"autoResize"		"0"
		"pinCorner"			"2"
		"visible"			"1"
		"enabled"			"1"
		"tabPosition"		"3"
		"labelText"			"#GameUI_MuteIngameVoice"
		"textAlignment"		"center"
		"dulltext"			"0"
		"brighttext"		"0"
		"Command"			"Mute"
		"Default"			"1"
		"font"				"HudFontSmallBold"
		"textAlignment"		"center"
		"dulltext"			"0"
		"brighttext"		"0"
		"default"			"1"
		"Command"			"Close"
		"sound_depressed"	"UI/buttonclick.wav"
		"sound_released"	"UI/buttonclickrelease.wav"
	}
	"PlayerList"
	{
		"ControlName"		"ListPanel"
		"fieldName"			"PlayerList"
		"xpos"				"28"
		"ypos"				"38"
		"wide"				"410"
		"tall"				"279"
		"autoResize"		"3"
		"pinCorner"			"0"
		"visible"			"1"
		"enabled"			"1"
		"tabPosition"		"1"
	}
	"Close"
	{
		"ControlName"		"Button"
		"fieldName"			"Close"
		"xpos"				"276"
		"ypos"				"326"
		"wide"				"100"
		"tall"				"25"
		"autoResize"		"0"
		"pinCorner"			"3"
		"visible"			"1"
		"enabled"			"1"
		"tabPosition"		"4"
		"labelText"			"#GameUI_Close"
		"textAlignment"		"center"
		"dulltext"			"0"
		"brighttext"		"0"
		"Command"			"Close"
		"Default"			"0"
		"font"				"HudFontSmallBold"
		"textAlignment"		"center"
		"dulltext"			"0"
		"brighttext"		"0"
		"default"			"1"
		"Command"			"Close"
		"sound_depressed"	"UI/buttonclick.wav"
		"sound_released"	"UI/buttonclickrelease.wav"
	}
}