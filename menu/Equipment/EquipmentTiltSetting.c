#include "FLV_Cluster_APP.h"


void DisplayTiltSettingInfo()
{
	if(OldScreenIndex != ScreenIndex)
	{
		OldCurserIndex = 0xff;
		CurserIndex = 0;
		DrawMenuFull();
	}
	if(OldCurserIndex != CurserIndex)
	{
		OldCurserIndex = CurserIndex;
		
		USHORT y_Height1,y_Height2;
		USHORT y_margin, y_offset = 0;

		if(OldCurserIndex == 0)
		{
			DisplayMenuBackgroundBG();
			LCD_Draw_Color(6, 48, 200, 122, COLOR_MENU_GRAY);
			DisplayListPage(1, 2);

			y_Height1 = GetStringMultiLineHeight((USHORT*)*StringSentence[27], StringEtc[SIGN_NOSTRING], StringEtc[SIGN_NOSTRING], 180,  23);
			y_Height2 = GetStringMultiLineHeight((USHORT*)*StringSentence[28], StringEtc[SIGN_NOSTRING], StringEtc[SIGN_NOSTRING], 180,  23);

			y_margin = (122 - (y_Height1+y_Height2)) / 2;
			y_offset = y_margin;
			
			for(int i=0;i<2;i++)
			{
					DisplayStringAt((USHORT*)*StringSentence[11+i], 10, 53+y_offset,  gui_font23, COLOR_WHITE);
				GUI_DisplayMultiStringAt((USHORT*)*StringSentence[27+i], 30, 53+y_offset, 180, 0, COLOR_WHITE, 23);
				y_offset+= (y_Height1+y_margin);
			}

		}
		else
		{
			DisplayMenuBackgroundBG();
			LCD_Draw_Color(6, 48, 200, 122, COLOR_MENU_GRAY);
			DisplayListPage(2, 2);

//			y_Height1 = GetStringMultiLineHeight((USHORT*)*StringSentence[28], StringEtc[SIGN_NOSTRING], StringEtc[SIGN_NOSTRING], 180,  23);
//			y_Height2 = GetStringMultiLineHeight((USHORT*)*StringSentence[29], StringEtc[SIGN_NOSTRING], StringEtc[SIGN_NOSTRING], 180,  23);

//			y_margin = (122 - (y_Height1+y_Height2)) / 2;
//			y_offset = y_margin;

			//DisplayStringAt((USHORT*)*StringSentence[12], 10, 53,  gui_font23, COLOR_WHITE);
			
			//for(int i=0;i<2;i++)
			//{
				GUI_DisplayMultiStringAt((USHORT*)*StringSentence[29], 30, 53, 180, 0, COLOR_WHITE, 23);
			//	y_offset+= (y_Height1+y_margin);
			//}
		}

	}	
}

void DisplayTiltSetting()
{
	switch(ScreenIndex)
	{
		case SCREEN_STATE_MENU_EQUIPMENT_TILTSETTING_INITIAL:	
			if((OldScreenIndex != ScreenIndex) || (OldCurserIndex != CurserIndex))
				DisplayTiltSettingInfo();
			break;
		case SCREEN_STATE_MENU_EQUIPMENT_TILTSETTING_INITIAL_COMPLETE:	
			if(OldScreenIndex != ScreenIndex)
				DisplayPopupString2((USHORT*)*StringSentence[30], (USHORT*)*StringSentence[31]);
			break;
		case SCREEN_STATE_MENU_EQUIPMENT_TILTSETTING_INITIAL_NOT_COMPLETE:				
			if(OldScreenIndex != ScreenIndex)
				DisplayPopupString2((USHORT*)*StringSentence[30], (USHORT*)*StringSentence[32]);
			break;
	}
}
