#include "FLV_Cluster_APP.h"

UCHAR nBrightnessLevel;
UCHAR nOldBrightnessLevel;


#if 0
//////////////////////////////////////////////////////////////////////////////////////
#define BRIGHTNESS_MANUAL			 			 0
#define BRIGHTNESS_AUTO			 			 1
//////////////////////////////////////////////////////////////////////////////////////
#define BRIGHTNESS_MIN			 			 			 			 			0
#define BRIGHTNESS_MAX			 			 			 			 			7
//////////////////////////////////////////////////////////////////////////////////////

#endif

void DisplayBrightnessChange()
{
	USHORT covbuf[2]={0,};

	if(OldScreenIndex != ScreenIndex)
	{
		DisplayMenuBackgroundBG();
		OldCurserIndex = 0xff;
		CurserIndex = CURSER_NONE;

		covbuf[0] = '1' + nBrightnessLevel;

		LCD_Draw_Color(74, 97, 65, 22, COLOR_BLACK);
		GUI_DisplayCenterStringAt(&covbuf[0], 0, 97, LCD_WIDTH, COLOR_WHITE, FONTSIZE);	

		nOldBrightnessLevel = nBrightnessLevel;
		if(ScreenIndex == SCREEN_STATE_MENU_DISPLAYSETTING_LCD_BRIGHTNESS_CHANGE)
			InfoDisplaySetting.LCDBrightnessLevel = nBrightnessLevel;
		else
			InfoDisplaySetting.LEDBrightnessLevel = nBrightnessLevel;
	}

	if(nOldBrightnessLevel != nBrightnessLevel)
	{
		covbuf[0] = '1' + nBrightnessLevel;

		LCD_Draw_Color(74, 97, 65, 22, COLOR_BLACK);
		GUI_DisplayCenterStringAt(&covbuf[0], 0, 97, LCD_WIDTH, COLOR_WHITE, FONTSIZE);	
		
		nOldBrightnessLevel = nBrightnessLevel;

		if(ScreenIndex == SCREEN_STATE_MENU_DISPLAYSETTING_LCD_BRIGHTNESS_CHANGE)
			ControlLCDBrightnessLevel(nOldBrightnessLevel);		
		else
			ControlLEDBrightnessLevel(nOldBrightnessLevel);		
	}

	if(OldCurserIndex != CurserIndex)
	{
		LCD_Draw_Color(6, 90, 31, 39, (CurserIndex == CURSER_UP_LEFT)?COLOR_MENU_BLUE:COLOR_MENU_GRAY);
		PCXtoBMP_16bit(16, 101, 10, 15, (CurserIndex == CURSER_UP_LEFT)?FL_Image.menu_arrow_display_select_2:FL_Image.menu_arrow_nor_2);

		LCD_Draw_Color(183, 90, 31, 39, (CurserIndex == CURSER_DOWN_RIGHT)?COLOR_MENU_BLUE:COLOR_MENU_GRAY);
		PCXtoBMP_16bit(193, 101, 10, 15, (CurserIndex == CURSER_DOWN_RIGHT)?FL_Image.menu_arrow_display_select:FL_Image.menu_arrow_nor);
	}

	if(OldScreenIndex != ScreenIndex)
	{
		OldCurserIndex = CurserIndex;
		DrawMenuFull();
	}
	else if(OldCurserIndex != CurserIndex)
	{
		OldCurserIndex = CurserIndex;
	}	
}

void DisplayBrightness()
{
	if((OldScreenIndex != ScreenIndex) || (nOldBrightnessLevel != nBrightnessLevel) || (OldCurserIndex!= CurserIndex))
	{
		DisplayBrightnessChange();
	}
}

