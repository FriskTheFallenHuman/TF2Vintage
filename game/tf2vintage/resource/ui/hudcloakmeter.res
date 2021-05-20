"Resource/UI/HudCloakMeter.res"
{
	"CloakBG"
	{
		"ControlName"	"CTFImagePanel"
		"fieldName"		"CloakBG"
		"xpos"			"12"
		"ypos"			"6"
		"zpos"			"0"
		"wide"			"76"
		"tall"			"38"
		"visible"		"1"
		"enabled"		"1"
		"image"			"../hud/misc_ammo_area_blue"
		"scaleImage"	"1"	
		"teambg_2"		"../hud/misc_ammo_area_red"
		"teambg_2_lodef"	"../hud/misc_ammo_area_red_lodef"
		"teambg_3"		"../hud/misc_ammo_area_blue"
		"teambg_3_lodef"	"../hud/misc_ammo_area_blue_lodef"		
	}
	
	"CloakLabel"
	{
		"ControlName"	"CExLabel"
		"fieldName"		"CloakLabel"
		"xpos"			"25"
		"ypos"			"27"
		"zpos"			"2"
		"wide"			"41"
		"tall"			"15"
		"autoResize"	"1"
		"pinCorner"		"2"
		"visible"		"1"

		"enabled"		"1"
		"tabPosition"	"0"
		"labelText"		"#TF_Cloak"
		"textAlignment"	"center"
		"dulltext"		"0"
		"brighttext"	"0"
		"font"			"TFFontSmall"
		
		"if_minmode"
		{
			"visible"		"0"
		}
	}

	"CloakMeter"
	{	
		"ControlName"	"ContinuousProgressBar"
		"fieldName"		"CloakMeter"
		"font"			"Default"
		"xpos"			"25"
		"ypos"			"23"
		"zpos"			"2"
		"wide"			"40"
		"tall"			"6"				
		"autoResize"	"0"
		"pinCorner"		"0"
		"visible"		"1"
		"enabled"		"1"
		"textAlignment"	"Left"
		"dulltext"		"0"
		"brighttext"	"0"
		
		"if_minmode"
		{
			"ypos"	"0"
		}
	}					
}
