"AchievementsDialog.res"
{	
	"AchievementsDialog"
	{
		"ControlName"							"CAchievementsDialog"
		"fieldName"								"AchievementsDialog"
		"xpos"									"272"
		"ypos"									"154"
		"wide"									"630"
		"tall"									"444"
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
		"labelText"				"#GameUI_Achievements_Title"
		"textAlignment"			"center"
		"xpos"					"c-330"
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

	"listpanel_achievements"
	{
		"ControlName"		"PanelListPanel"
		"fieldName"			"listpanel_achievements"
		"xpos"				"15"
		"ypos"				"136"
		"wide"				"600"
		"tall"				"268"
		"autoResize"		"3"
		"pinCorner"			"0"
		"visible"			"1"
		"enabled"			"1"
		"tabPosition"		"1"
	}
	
	"listpanel_background"
	{
		"ControlName"		"ImagePanel"
		"fieldName"			"listpanel_background"
		"xpos"				"15"
		"ypos"				"136"
		"wide"				"600"
		"tall"				"268"
		"fillcolor"			"32 32 32 255"
		"zpos"				"-3"
		"visible"			"1"
		"enabled"			"1"
		"pinCorner"			"0"
		"autoResize"		"3"
	}
	
	"PercentageBarBackground" //light grey overall percentage
	{
		"ControlName"		"ImagePanel"
		"fieldName"			"PercentageBarBackground"
		"xpos"				"23"
		"ypos"				"67"
		"wide"				"584"
		"tall"				"16"
		"fillcolor"			"79 79 79 255"
		"zpos"				"-2"
		"visible"			"0"
		"enabled"			"1"
	}
	
	"PercentageBar" //dark grey current completed
	{
		"ControlName"		"ImagePanel"
		"fieldName"			"PercentageBar"
		"xpos"				"23"
		"ypos"				"67"
		"wide"				"0"
		"tall"				"16"
		"fillcolor"			"157 194 80 255"
		"zpos"				"-1"
		"visible"			"0"
		"enabled"			"1"
	}
	
	"PercentageText" //Percent Text inside the percentage field
	{
		"ControlName"		"Label"
		"fieldName"			"PercentageText"
		"xpos"				"407"
		"ypos"				"46"
		"wide"				"200"
		"tall"				"20"
		"autoResize"		"0"
		"pinCorner"			"0"
		"visible"			"0"
		"enabled"			"1"
		"tabPosition"		"0"
		"labelText"			"0%"
		"textAlignment"		"east"
		"dulltext"			"0"
		"brighttext"		"0"
		"wrap"				"0"
		"fillcolor"			"255 255 255 255"
		"font"				"AchievementItemDescription"	//"defaultlarg"
	}
	
	"achievement_pack_combo"
	{
		"ControlName"	"ComboBox"
		"fieldName"		"achievement_pack_combo"
		"xpos"			"15"
		"ypos"			"102"
		"wide"			"225"
		"tall"			"24"
		"enabled"		"1"
		"visible"		"1"
		"editable"		"0"
	}
	
	"ProgressBackground" // dark box around progress bar and 'achievements earned' label
	{
		"ControlName"		"ImagePanel"
		"fieldName"			"ProgressBackground"
		"xpos"				"15"
		"ypos"				"42"
		"wide"				"600"
		"tall"				"50"
		"fillcolor"			"32 32 32 255"
		"zpos"				"-3"
		"visible"			"1"
		"enabled"			"1"
	}
	
	"AchievementsEarnedLabel" 
	{
		"ControlName"		"Label"
		"fieldName"			"AchievementsEarnedLabel"
		"xpos"				"23"
		"ypos"				"46"
		"wide"				"200"
		"tall"				"20"
		"autoResize"		"0"
		"pinCorner"			"0"
		"visible"			"1"
		"enabled"			"1"
		"tabPosition"		"0"
		"labelText"			"#GameUI_Achievements_Earned"
		"textAlignment"		"west"
		"dulltext"			"0"
		"brighttext"		"0"
		"wrap"				"0"
		"fillcolor"			"255 255 255 255"
		"font"				"AchievementItemDescription"	//"defaultlarg"
	}
	
	"HideAchieved" // Not implemented yet
	{
		"ControlName"		"CheckButton"
		"fieldName"			"HideAchieved"
		"xpos"				"255"
		"ypos"				"410"
		"wide"				"150"
		"tall"				"24"
		"autoResize"		"0"
		"pinCorner"			"2"
		"visible"			"0"
		"enabled"			"0"
		"tabPosition"		"0"
		"labelText"			"#GameUI_Achievement_Hide_Achieved"
		"textAlignment"		"west"
		"dulltext"			"0"
		"brighttext"		"0"
		"wrap"				"0"
		"font"				"AchievementItemDescription"
	}
	
	"Back" //back button
	{
		"ControlName"		"CExButton"
		"fieldName"			"Back"
		"xpos"				"c-355"
		"ypos"				"410"	
		"zpos"				"2"
		"wide"				"100"
		"tall"				"25"
		"autoResize"		"0"
		"pinCorner"			"3"
		"visible"			"1"
		"enabled"			"1"
		"tabPosition"		"0"
		"labelText"			"#TF_BackCarat"
		"font"				"HudFontSmallBold"
		"textAlignment"		"center"
		"dulltext"			"0"
		"brighttext"		"0"
		"default"			"0"
		"Command"			"Close"
		"sound_depressed"	"UI/buttonclick.wav"
		"sound_released"	"UI/buttonclickrelease.wav"
	}
	
	"FooterLine"
	{
		"ControlName"	"ImagePanel"
		"fieldName"		"FooterLine"
		"xpos"			"0"
		"ypos"			"403"
		"zpos"			"2"
		"wide"			"f0"
		"tall"			"10"
		"visible"		"1"
		"enabled"		"1"
		"image"			"loadout_solid_line"
		"scaleImage"	"1"
	}
}