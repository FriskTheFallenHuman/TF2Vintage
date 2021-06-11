"Resource/UI/GenericConfirmation.res"
{
	"LblTitle"
	{
		"ControlName"		"Label"
		"fieldName"			"LblTitle"
		"fgcolor_override"	"200 80 60 255"
	}
	
	"LblMessage"
	{
		"ControlName"		"Label"
		"fieldName"			"LblMessage"
		"fgcolor_override"	"215 215 215 255"
	}
	
	"GenericConfirmation"
	{
		"ControlName"		"Frame"
		"fieldName"			"GenericConfirmation"
		"autoResize"		"0"
		"pinCorner"			"0"
		"visible"			"1"
		"enabled"			"1"
		"tabPosition"		"0"
	}

	"BtnOK"
	{
		"ControlName"		"CTFMenuButton"
		"fieldName"			"BtnOK"
		"xpos"				"135"
		"ypos"				"80"
		"wide"				"117"
		"tall"				"27"
		"visible"			"1"
		"autoResize"		"0"
		"enabled"			"1"
		"tabPosition"		"0"
		"AllCaps"			"1"
		"bordervisible"		"1"	
		"Command"			"OK"
		"SubButton"
		{
			"labelText" 					"#GameUI_OK"
			"bordervisible"					"1"
			"textAlignment"					"center"
			"font"							"HudFontSmallBold"
			"border_default"				"OldAdvButtonDefault"
			"border_armed"					"OldAdvButtonDefaultArmed"
			"border_depressed"				"OldAdvButtonDefaultArmed"
			"depressedFgColor_override"		"46 43 42 255"	
		}
	}

	"BtnCancel"
	{
		"ControlName"		"CTFMenuButton"
		"fieldName"			"BtnCancel"
		"xpos"				"195"
		"ypos"				"80"
		"wide"				"117"
		"tall"				"27"
		"autoResize"		"0"
		"enabled"			"1"
		"tabPosition"		"0"
		"AllCaps"			"1"
		"bordervisible"		"1"	
		"Command"			"cancel"
		"SubButton"
		{
			"labelText" 					"#GameUI_Cancel"
			"bordervisible"					"1"
			"textAlignment"					"center"
			"font"							"HudFontSmallBold"
			"border_default"				"OldAdvButtonDefault"
			"border_armed"					"OldAdvButtonDefaultArmed"
			"border_depressed"				"OldAdvButtonDefaultArmed"
			"depressedFgColor_override"		"46 43 42 255"	
		}
	}
}