"Resource/UI/LoadingProgress.res"
{
	"LoadingProgress"
	{	
		"ControlName"				"Frame"
		"fieldName"					"LoadingProgress"
		"xpos"						"0"
		"ypos"						"0"
		"wide"						"f0"
		"tall"						"f0"
		"autoResize"				"0"
		"pinCorner"					"0"
		"visible"					"1"
		"enabled"					"1"
		"tabPosition"				"0"
		"setclosebuttonvisible"		"0"
	}
	
	// ---------- Decoration Content ----------
	
	"TitleBanner"
	{
		"ControlName"		"ImagePanel"
		"xpos"				"0"
		"ypos"				"0"
		"wide"				"f0"
		"tall"				"128"
		"image"				"menu_header"
		"visible_lodef"		"0"
		"enabled_lodef"		"0"
	}
	
	"MainBackground"
	{
		"ControlName"		"ImagePanel"
		"fieldName"			"MainBackground"
		"xpos"				"0"
		"ypos"				"0"
		"wide"				"f0"
		"tall"				"f0"
		"zpos"				"2"
		"scaleImage"		"1"
		"visible"			"0"
		"enabled"			"1"
	}
	
	// ---------- Center Content ----------
	
	"LoadingTipPanel"
	{
		"ControlName"			"EditablePanel"
		"fieldName"				"LoadingTipPanel"
		"xpos"					"8"
		"ypos"					"r70"
		"zpos"					"50"
		"wide"					"450"
		"tall"					"80"
		"visible"				"1"
		"enabled"				"1"
		"usetitlesafe"			"1"
		"tabPosition"			"0"
		"PaintBackgroundType"	"2"
		
		"TipText"
		{
			"ControlName"				"Label"
			"fieldName"					"TipText"
			"xpos"						"35"
			"ypos"						"20"
			"wide"						"320"
			"tall"						"36"
			"wrap"						"1"
			"pinCorner"					"0"
			"visible"					"1"
			"enabled"					"1"
			"labelText"					"%TipText%"
			"textAlignment"				"west"
			"textAlignment_hidef"		"north-west"
			"textAlignment_lodef"		"north-west"
			"font"						"ScoreboardVerySmall"
			"font_hidef"				"Default"
			"zpos"						"40"
		}
	}

	"LoadingBar"
	{
		"ControlName"			"ProgressBar"
		"fieldName"				"LoadingBar"
		"xpos"					"r300"
		"ypos"					"r38"
		"wide"					"200"
		"tall"					"15"
		"zpos"					"5"
		"autoResize"			"0"
		"pinCorner"				"0"
		"visible"				"1"
		"enabled"				"1"
		"tabPosition"			"0"
		"usetitlesafe"			"1"
	}
	
	"LoadingSpinner"
	{
		"ControlName"			"ImagePanel"
		"fieldName"				"LoadingSpinner"
		"xpos"					"0"
		"ypos"					"0"
		"zpos"					"5"
		"wide"					"40"
		"tall"					"40"
		"visible"				"0"
		"enabled"				"1"
		"tabPosition"			"0"
		"scaleImage"			"1"
		"image"					"common/swarm_cycle"
		"frame"					"0"
	}	
	
	"LoadingText"
	{
		"ControlName"			"Label"
		"fieldName"				"LoadingText"
		"xpos"					"150"
		"ypos"					"r68"
		"zpos"					"5"
		"wide"					"600"
		"tall"					"40"
		"autoResize"			"1"
		"pinCorner"				"0"
		"visible"				"1"
		"enabled"				"1"
		"tabPosition"			"0"
		"Font"					"HudFontMediumBold"
		"labelText"				"#GameUI_Loading"
		"textAlignment"			"east"
		"fgcolor_override"		"204 188 164 255"
		"usetitlesafe"			"1"
	}
	
	"LoadingProgressText"
	{
		"ControlName"			"Label"
		"fieldName"				"LoadingProgressText"
		"xpos"					"150"
		"ypos"					"r35"
		"zpos"					"5"
		"wide"					"600"
		"tall"					"40"
		"autoResize"			"1"
		"pinCorner"				"0"
		"visible"				"1"
		"enabled"				"1"
		"tabPosition"			"0"
		"Font"					"HudFontSmallBold"
		"labelText"				"#GameUI_StartingServer"
		"textAlignment"			"east"
		"fgcolor_override"		"204 188 164 255"
		"usetitlesafe"			"1"
	}
	
	// ---------- Footer Content ----------
	
	"Cancel"
	{
		"ControlName"		"CTFMenuButton"
		"fieldName"			"Cancel"
		"xpos"				"r350"
		"ypos"				"r39"
		"zpos"				"6"
		"wide"				"55"
		"tall"				"16"
		"visible"			"1"
		"enabled"			"1"
		"bordervisible"		"1"	
		"Command"			"Cancel"

		"SubButton"
		{
			"labelText" 					"#VGUI_Cancel"
			"bordervisible"					"1"
			"textAlignment"					"center"
			"font"							"HudFontSmallestBold"
			"border_default"				"OldAdvButtonDefault"
			"border_armed"					"OldAdvButtonDefaultArmed"
			"border_depressed"				"OldAdvButtonDefaultArmed"
			"depressedFgColor_override"		"46 43 42 255"	
		}
		
		"SubImage"
		{
			"imagewidth"					"14"
			"imagecolordepressed"			"MainMenuAdvButtonDepressed"
		}			
	}
	
	"Footer"
	{
		"ControlName"	"ImagePanel"
		"fieldName"		"Footer"
		"xpos"			"0"
		"ypos"			"420"
		"zpos"			"-50"
		"wide"			"f0"
		"tall"			"60"
		"visible"		"1"
		"enabled"		"1"
		"image"			"loadout_bottom_gradient"
		"tileImage"		"1"
	}

	"FooterLine"
	{
		"ControlName"	"ImagePanel"
		"fieldName"		"FooterLine"
		"xpos"			"0"
		"ypos"			"420"
		"zpos"			"-5-"
		"wide"			"f0"
		"tall"			"10"
		"visible"		"1"
		"enabled"		"1"
		"image"			"loadout_solid_line"
		"scaleImage"	"1"
	}
}