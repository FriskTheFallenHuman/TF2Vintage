"Resource/UI/BaseModUI/MainMenu.res"
{
	"MainMenu"
	{
		"ControlName"				"Frame"
		"fieldName"					"MainMenu"
		"xpos"						"0"
		"ypos"						"0"
		"wide"						"f0"
		"tall"						"f0"
		"autoResize"				"0"
		"pinCorner"					"0"
		"visible"					"1"
		"enabled"					"1"
		"tabPosition"				"0"
		"PaintBackgroundType"		"0"
		"setclosebuttonvisible"		"0"
	}
	
	// ---------- Header Content ----------
	
	"HeaderBackground"
	{
		"ControlName"		"ImagePanel"
		"fieldName"			"HeaderBackground"
		"xpos"				"0"
		"ypos"				"0"
		"zpos"				"-100"
		"wide"				"f0"
		"tall"				"90"
		"visible"			"1"
		"enabled"			"1"
		"image"				"loadout_bottom_gradient"
		"scaleImage"		"0"
		"tileImage"			"1"
	}				
	"HeaderLine"
	{
		"ControlName"		"ImagePanel"
		"fieldName"			"HeaderLine"
		"xpos"				"0"
		"ypos"				"90"
		"zpos"				"70"
		"wide"				"f0"
		"tall"				"10"
		"visible"			"1"
		"enabled"			"1"
		"image"				"loadout_solid_line"
		"scaleImage"		"1"
	}	
	
	"TopLeftDataPanel"
	{
		"ControlName"		"EditablePanel"
		"fieldName"			"TopLeftDataPanel"
		"xpos"				"10"
		"ypos"				"30"
		"zpos"				"-60"
		"wide"				"p0.48"
		"tall"				"60"
		"autoResize"		"0"
		"visible"			"1"
		"enabled"			"1"
		"border"			"MainMenuBGBorderAlpha"
		"font"				"MenuMainTitle"
		"bgcolor_override"	"46 43 42 255"
	}
	
	"WelcomeLabel"
	{
		"ControlName"		"CExLabel"
		"fieldName"			"WelcomeLabel"
		"xpos"				"70"
		"ypos"				"35"
		"zpos"				"10"
		"wide"				"p0.37"
		"tall"				"15"
		"autoResize"		"0"
		"pinCorner"			"0"
		"visible"			"1"
		"enabled"			"1"
		"alpha"				"255"
		"labelText"			"#WelcomeBack"
		"textAlignment"		"west"
		"font"				"HudFontSmallBold"
		"fgcolor"			"AdvTextDefault"
	}
	
	"VersionLabel"
	{
		"ControlName"		"CExLabel"
		"fieldName"			"VersionLabel"
		"xpos"				"70"
		"ypos"				"50"
		"zpos"				"10"
		"wide"				"140"
		"tall"				"20"
		"autoResize"		"0"
		"pinCorner"			"0"
		"visible"			"1"
		"enabled"			"1"
		"alpha"				"255"
		"textAlignment"		"north-west"
		"font"				"HudFontSmallest"
		"fgcolor_override"	"117 107 94 255"
	}
	
	"AvatarBackground"
	{
		"ControlName"		"EditablePanel"
		"fieldName"			"AvatarBackground"
		"xpos"				"15"
		"ypos"				"10"
		"zpos"				"10"
		"wide"				"50"
		"tall"				"50"
		"visible"			"1"
		"enabled"			"1"
		"border"			"MainMenuBGBorderAlpha"
		"font"				"MenuMainTitle"
		"bgcolor_override"	"117 107 94 255"
	}
	
	"AvatarImage"
	{
		"ControlName"		"CAvatarImagePanel"
		"fieldName"			"AvatarImage"
		"xpos"				"20"
		"ypos"				"15"
		"zpos"				"20"
		"wide"				"40"
		"tall"				"40"
		"visible"			"1"
		"enabled"			"1"
		"scaleImage"		"1"
		"color_outline"		"255 255 255 255"
	}
	
	"Stats"
	{
		"ControlName"		"CTFMenuButton"
		"fieldName"			"Stats"
		"xpos"				"cp-0.12"
		"ypos"				"60"
		"zpos"				"10"
		"wide"				"p0.1"
		"tall"				"20"
		"visible"			"1"
		"enabled"			"1"
		"bordervisible"		"1"
		"labelText" 		""
		"command"			"newstats"
		
		"SubButton"
		{
			"labelText" 					"#MMenu_ViewAll"
			"bordervisible"					"0"
			"font"							"HudFontSmallestBold"
			"textAlignment"					"Center"
			"border_default"				"OldAdvButtonDefault"
			"border_armed"					"OldAdvButtonDefaultArmed"
			"border_depressed"				"OldAdvButtonDefaultArmed"
			"depressedFgColor_override"		"46 43 42 255"
		}
	
	}	
	
	// ---------- Central Screen Content ----------

	"Logo"
	{
		"ControlName"		"ImagePanel"
		"fieldName"			"Logo"
		"xpos"				"c-295"
		"ypos"				"100"
		"zpos"				"-5"		
		"wide"				"450"
		"tall"				"125"
		"visible"			"1"
		"enabled"			"1"
		"image"				"../vgui/main_menu/tf2_vintage_logo"
		"alpha"				"255"
		"scaleImage"		"0"	
	}

	"CentralMenuBackground"
	{
		"ControlName"		"EditablePanel"
		"fieldName"			"MainMenuButtonsBG"
		"xpos"				"c-295"
		"ypos"				"220"
		"zpos"				"-50"
		"wide"				"260"
		"tall"				"65"
		"visible"			"1"
		"enabled"			"1"
		"border"			"MainMenuBGBorder"
		"font"				"MenuMainTitle"
	}
	
	"ServerBrowser"
	{
		"ControlName"		"CTFAdvButton"
		"fieldName"			"ServerBrowser"
		"xpos"				"c-290"
		"ypos"				"225"
		"zpos"				"5"
		"wide"				"250"
		"tall"				"26"
		"visible"			"1"
		"enabled"			"1"
		"bordervisible"		"1"		
		"command"			"ServerBrowser"		
		
		"SubButton"
		{
			"labelText" 					"#MMenu_StartPlaying"
			"textAlignment"					"west"
			"font"							"HudFontSmallBold"
			"defaultFgColor_override"		"MainMenuTextDefault"
			"armedFgColor_override"			"MainMenuTextArmed"
			"depressedFgColor_override"		"MainMenuTextDepressed"	
		}
		
		"SubImage"
		{
			"image" 			"../vgui/glyph_server"
			"imagewidth"		"18"	
		}
	}
	
	"CreateServer"
	{
		"ControlName"		"CTFAdvButton"
		"fieldName"			"CreateServer"
		"xpos"				"c-65"
		"ypos"				"228"
		"zpos"				"10"
		"wide"				"20"
		"tall"				"20"
		"visible"			"1"
		"enabled"			"1"
		"bordervisible"		"1"		
		"command"			"CreateGame"		
		
		"SubButton"
		{
			"labelText" 					""
			"tooltip" 						"#MMenu_PlayList_CreateServer_Button"
			"bordervisible"					"0"
			"textAlignment"					"west"
			"font"							"HudFontSmallBold"
			"defaultFgColor_override"		"MainMenuTextArmed"
			"armedFgColor_override"			"MainMenuTextArmed"
			"depressedFgColor_override"		"MainMenuTextDefault"
			"border_default"				"MainMenuAdvButtonDepressed"
			"border_depressed"				"MainMenuAdvButton"		
		}
		
		"SubImage"
		{
			"image" 						"../vgui/glyph_create"
			"imagewidth"					"16"	
		}
	}
	
	"Loadout"
	{
		"ControlName"		"CTFAdvButton"
		"fieldName"			"Loadout"
		"xpos"				"c-290"
		"ypos"				"255"
		"zpos"				"5"
		"wide"				"250"
		"tall"				"26"
		"visible"			"1"
		"enabled"			"1"
		"bordervisible"		"1"		
		"command"			"Backpack"
		
		"SubButton"
		{
			"labelText" 					"#MMenu_CharacterSetupOld"
			"textAlignment"					"west"
			"font"							"HudFontSmallBold"
			"defaultFgColor_override"		"MainMenuTextDefault"
			"armedFgColor_override"			"MainMenuTextArmed"
			"depressedFgColor_override"		"MainMenuTextDepressed"	
		}
		
		
		"SubImage"
		{
			"image" 						"../vgui/glyph_items"
			"imagewidth"					"16"	
		}
	}
	
	// ---------- Server Panel List Content ----------
	
	"ServerlistPanel"
	{
		"ControlName"		"EditablePanel"
		"fieldName"			"ServerlistPanel"
		"xpos"				"c-300"
		"ypos"				"280"
		"zpos"				"3"
		"wide"				"270"
		"tall"				"95"
		"visible"			"1"
		"enabled"			"1"
		"server_width" 		"50"
		"players_width" 	"35"
		"ping_width" 		"23"
		"map_width" 		"50"
		"scroll_width"	 	"23"
		
		"ServerListBackground"
		{
			"ControlName"		"EditablePanel"
			"fieldName"			"ServerListBackground"
			"xpos"				"5"
			"ypos"				"5"
			"zpos"				"-2"
			"wide"				"260"
			"tall"				"95"
			"visible"			"1"
			"enabled"			"1"
			"border"			"MainMenuBGBorder"
			"font"				"MenuMainTitle"
		}
		
		"ServerListLabel"
		{
			"ControlName"		"CExLabel"
			"fieldName"			"ServerListLabel"
			"xpos"				"95"
			"ypos"				"7"
			"zpos"				"6"
			"wide"				"170"
			"tall"				"20"
			"autoResize"		"0"
			"pinCorner"			"0"
			"visible"			"1"
			"enabled"			"1"
			"alpha"				"255"
			"labelText"			"#MMenu_Servers"
			"textAlignment"		"west"
			"font"				"HudFontSmallishBold"
			"fgcolor"			"AdvTextDefault"
		}	
		
		"ServerList"
		{
			"ControlName"	"SectionedListPanel"
			"fieldName"		"ServerList"
			"xpos"			"5"
			"ypos"			"25"
			"zpos"			"1"
			"wide"			"284"
			"tall"			"72"
			"autoResize"	"0"
			"pinCorner"		"0"
			"font"			"Link"
			"visible"		"1"
			"enabled"		"1"
			"linespacing"	"12"
			"tabPosition"	"0"
		}	
		
		"ListSlider"
		{
			"ControlName"		"CTFAdvSlider"
			"fieldName"			"ListSlider"
			"xpos"				"252"
			"ypos"				"30"
			"zpos"				"10"
			"wide"				"35"
			"tall"				"62"
			"visible"			"1"
			"enabled"			"1"
			"minvalue" 			"0"
			"maxvalue" 			"100"
			"labelwidth" 		"1"
			"bordervisible"		"0"
			"command"			""
			"vertical"			"1"
			"value_visible"		"0"
			
			"SubButton"
			{
				"labelText" 		""
				"textAlignment"		"west"
				"font"				"FontStorePromotion"
				"border_default"	""
				"border_armed"		""	
				"border_depressed"	""	
			}
		}	
		
		"ConnectButton"
		{
			"ControlName"	"CTFAdvButton"
			"fieldName"		"ConnectButton"
			"xpos"			"200"
			"ypos"			"370"
			"zpos"			"2"
			"wide"			"60"
			"tall"			"12"
			"visible"		"1"
			"enabled"		"1"
			"command"		""
			
			"SubButton"
			{
				"labelText" 					"connect"
				"textAlignment"					"center"
				"font"							"ItemFontNameLarge"
				"defaultFgColor_override"		"MainMenuTextDefault"
				"armedFgColor_override"			"MainMenuTextArmed"
				"depressedFgColor_override"		"MainMenuTextDepressed"	
			}
		}
	}

	// ---------- Footer Content ----------
	
	"FooterBackground"
	{
		"ControlName"	"ImagePanel"
		"fieldName"		"FooterBackground"
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
	
	"QuitDisconnect"
	{
		"ControlName"		"CTFAdvButton"
		"fieldName"			"QuitDisconnect"
		"xpos"				"c-290"
		"ypos"				"437"
		"zpos"				"10"
		"wide"				"150"
		"tall"				"25"
		"visible"			"1"
		"enabled"			"1"
		"bordervisible"		"1"	

		"SubButton"
		{
			"bordervisible"					"1"
			"textAlignment"					"west"
			"font"							"HudFontSmallBold"
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

	"ReportBug"
	{
		"ControlName"		"CTFAdvButton"
		"fieldName"			"ReportBug"
		"xpos"				"c-35"
		"ypos"				"437"
		"zpos"				"10"
		"wide"				"25"
		"tall"				"25"
		"visible"			"1"
		"enabled"			"1"
		"bordervisible"		"1"
		"labelText" 		""
		"command"			"website https://github.com/TF2V/TF2Vintage/issues/new"
		
		"SubButton"
		{
			"labelText" 					""
			"bordervisible"					"0"
			"tooltip" 						"Report a bug"
			"textAlignment"					"center"
			"font"							"MenuSmallFont"	
			"border_default"				"OldAdvButtonDefault"
			"border_armed"					"OldAdvButtonDefaultArmed"
			"border_depressed"				"OldAdvButtonDefaultArmed"
			"depressedFgColor_override"		"46 43 42 255"	
		}
		
		"SubImage"
		{
			"image" 						"../vgui/glyph_bug"
			"imagewidth"					"14"
		}
	}
	
	"DiscordServer"
	{
		"ControlName"		"CTFAdvButton"
		"fieldName"			"DiscordServer"
		"xpos"				"c-10"
		"ypos"				"437"
		"zpos"				"10"
		"wide"				"25"
		"tall"				"25"
		"visible"			"1"
		"enabled"			"1"
		"bordervisible"		"1"
		"labelText" 		""
		"command"			"website https://discord.gg/vU5FFq22rm"
		
		"SubButton"
		{
			"labelText" 					""
			"bordervisible"					"0"
			"tooltip" 						"Join to the discord server"
			"textAlignment"					"center"
			"font"							"MenuSmallFont"	
			"border_default"				"OldAdvButtonDefault"
			"border_armed"					"OldAdvButtonDefaultArmed"
			"border_depressed"				"OldAdvButtonDefaultArmed"
			"depressedFgColor_override"		"46 43 42 255"	
		}
		
		"SubImage"
		{
			"image" 						"../vgui/glyph_commentary"
			"imagewidth"					"14"
		}
	}

	"Achievements"
	{
		"ControlName"		"CTFAdvButton"
		"fieldName"			"Achievements"
		"xpos"				"c16"
		"ypos"				"437"
		"zpos"				"10"
		"wide"				"25"
		"tall"				"25"
		"visible"			"1"
		"enabled"			"1"
		"bordervisible"		"1"
		"labelText" 		""
		"command"			"Achievements"
		
		"SubButton"
		{
			"labelText" 					""
			"bordervisible"					"0"
			"tooltip" 						"Random Music"
			"textAlignment"					"center"
			"font"							"MenuSmallFont"	
			"border_default"				"OldAdvButtonDefault"
			"border_armed"					"OldAdvButtonDefaultArmed"
			"border_depressed"				"OldAdvButtonDefaultArmed"
			"depressedFgColor_override"		"46 43 42 255"
		}
		
		"SubImage"
		{
			"image" 						"../vgui/glyph_achievements"
			"imagewidth"					"14"
		}
	}

	"Mute"
	{
		"ControlName"		"CTFAdvButton"
		"fieldName"			"Mute"
		"xpos"				"c42"
		"ypos"				"437"
		"zpos"				"10"
		"wide"				"25"
		"tall"				"25"
		"visible"			"1"
		"enabled"			"1"
		"bordervisible"		"1"
		"labelText" 		""
		"command"			"MutePlayer"
		
		"SubButton"
		{
			"labelText" 					""
			"bordervisible"					"0"
			"tooltip" 						"#MMenu_MutePlayers"
			"textAlignment"					"center"
			"font"							"MenuSmallFont"	
			"border_default"				"OldAdvButtonDefault"
			"border_armed"					"OldAdvButtonDefaultArmed"
			"border_depressed"				"OldAdvButtonDefaultArmed"
			"depressedFgColor_override"		"46 43 42 255"
		}
		
		"SubImage"
		{
			"image" 						"../vgui/glyph_muted"
			"imagewidth"					"14"
		}
	}

	"CallVote"
	{
		"ControlName"		"CTFAdvButton"
		"fieldName"			"CallVote"
		"xpos"				"c67"
		"ypos"				"437"
		"zpos"				"10"
		"wide"				"25"
		"tall"				"25"
		"visible"			"1"
		"enabled"			"1"
		"bordervisible"		"1"
		"labelText" 		""
		"command"			"CallVote"
		
		"SubButton"
		{
			"labelText" 					""
			"bordervisible"					"0"
			"tooltip" 						"#MMenu_MutePlayers"
			"textAlignment"					"center"
			"font"							"MenuSmallFont"	
			"border_default"				"OldAdvButtonDefault"
			"border_armed"					"OldAdvButtonDefaultArmed"
			"border_depressed"				"OldAdvButtonDefaultArmed"
			"depressedFgColor_override"		"46 43 42 255"
		}
		
		"SubImage"
		{
			"image" 						"../vgui/main_menu/glyph_stats"
			"imagewidth"					"14"
		}
	}

	"TF2Options"
	{
		"ControlName"		"CTFAdvButton"
		"fieldName"			"TF2Options"
		"xpos"				"c110"
		"ypos"				"437"
		"zpos"				"10"
		"wide"				"155"
		"tall"				"25"
		"visible"			"1"
		"enabled"			"1"
		"bordervisible"		"1"	
		"Command"			"TF2Options"
		"SubButton"
		{
			"labelText" 					"#MMenu_AdvOptions"
			"bordervisible"					"1"
			"textAlignment"					"west"
			"font"							"HudFontSmallBold"	
			"border_default"				"OldAdvButtonDefault"
			"border_armed"					"OldAdvButtonDefaultArmed"
			"border_depressed"				"OldAdvButtonDefaultArmed"
			"depressedFgColor_override"		"46 43 42 255"
		}
		"SubImage"
		{
			"image" 						"../vgui/glyph_options"
			"imagewidth"					"14"
			"imagecolordepressed"			"MainMenuAdvButtonDepressed"
		}
	}

	"Options"
	{
		"ControlName"		"CTFAdvButton"
		"fieldName"			"Options"
		"xpos"				"c270"
		"ypos"				"437"
		"zpos"				"10"
		"wide"				"25"
		"tall"				"25"
		"visible"			"1"
		"enabled"			"1"
		"bordervisible"		"1"	
		"Command"			"GameOptions"
		"SubButton"
		{
			"labelText" 					""
			"tooltip"						"#GameUI_GameMenu_Options"
			"bordervisible"					"1"
			"textAlignment"					"west"
			"font"							"HudFontSmallBold"	
			"border_default"				"OldAdvButtonDefault"
			"border_armed"					"OldAdvButtonDefaultArmed"
			"border_depressed"				"OldAdvButtonDefaultArmed"
			"depressedFgColor_override"		"46 43 42 255"
		}
		
		"SubImage"
		{
			"image" 						"../vgui/glyph_steamworkshop"
			"imagewidth"					"14"
		}			
	}
}