#include "FLV_Cluster_APP.h"

UCHAR Second = 30;
extern unsigned char		Equipment_Calibration;
extern st_AD_DATA 		AD_DATA;

void DisplayWeighSensorUnloadSetting()
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

		if(OldCurserIndex == 0)
		{
			USHORT y_Height1,y_Height2;
			USHORT y_margin, y_offset = 0;
			
			DisplayMenuBackgroundBG();
			LCD_Draw_Color(6, 48, 200, 122, COLOR_MENU_GRAY);
			DisplayListPage(1, 2);

			y_Height1 = GetStringMultiLineHeight((USHORT*)*StringSentence[40], StringEtc[SIGN_NOSTRING], StringEtc[SIGN_NOSTRING], 180,  23);
			y_Height2 = GetStringMultiLineHeight((USHORT*)*StringSentence[41], StringEtc[SIGN_NOSTRING], StringEtc[SIGN_NOSTRING], 180,  23);

			y_margin = (122 - (y_Height1+y_Height2)) / 3;
			y_offset = y_margin;
			
			for(int i=0;i<2;i++)
			{
				DisplayStringAt((USHORT*)*StringSentence[11+i], 10, 53+y_offset,  gui_font23, COLOR_WHITE);
				GUI_DisplayMultiStringAt((USHORT*)*StringSentence[40+i], 30, 53+y_offset, 180, 0, COLOR_WHITE, 23);
				y_offset+= (y_Height1+y_margin);
			}
		}
		else
		{
			DisplayMenuBackgroundBG();
			LCD_Draw_Color(6, 48, 200, 122, COLOR_MENU_GRAY);
			DisplayListPage(2, 2);

			DisplayStringAt((USHORT*)*StringSentence[13], 10, 53,  gui_font23, COLOR_WHITE);
			GUI_DisplayMultiStringAt((USHORT*)*StringSentence[42], 30, 53, 180, 0, COLOR_WHITE, 23);

		}

	}
}

void DisplayWeighSensorLoadSetting_Weight()
{
	UCHAR buf[10];
	USHORT covbuf[10];
	USHORT OldWeightTon = AD_DATA.C_Max_Ton;

	memset((&buf), 0x0, sizeof( buf));
	memset((&covbuf), 0x0, sizeof( covbuf));

	if(OldWeightTon == 0)
		sprintf ( (char*)buf, "0.0 ton");
	else
		sprintf ( (char*)buf, "%d.%d ton",OldWeightTon/10, OldWeightTon%10) ; 
	String_Func(covbuf, buf);

	DisplayStringAt(&covbuf[0], 6, 146, gui_font23,  COLOR_MENU_ORANGE);
}

void DisplayWeighSensorLoadSetting()
{
	DisplayMenuBackgroundBG();

	LCD_Draw_Color(6, 48, 210, 95, COLOR_MENU_GRAY);

	GUI_DisplayMultiStringAt((USHORT*)*StringSentence[43], 6, 48, 210, 95, COLOR_WHITE, 23);

	DisplayWeighSensorLoadSetting_Weight();	

	Second = 30;

	DisplayWeighSensorLoadSetting_Sec();

	DrawMenuFull();

}

void DisplayWeighSensorLoadSetting_Sec()
{
	UCHAR buf[10];
	USHORT covbuf[30];

	memset((&buf), 0x0, sizeof( buf));
	memset((&covbuf), 0x0, sizeof( covbuf));

	if(Equipment_Calibration == 0x00)
	{
		SetScreenIndex(SCREEN_STATE_MENU_EQUIPMENT_WEIGHTSENSOR_LOADADJUST_LOAD_COMPLETE);
	}
	else if(Second == 0)
	{
		SetScreenIndex(SCREEN_STATE_MENU_EQUIPMENT_WEIGHTSENSOR_LOADADJUST_LOAD_NOT_COMPLETE);
	}

	sprintf ( (char*)buf, "%d",Second--);
	String_Func(covbuf, buf);
	char length = strlen((char*)buf);
	USHORT *s = (USHORT*)*StringSentence[44];

	covbuf[length++] = (*s++);
	USHORT c = covbuf[length-1];

	while((c != 0))
	{
		covbuf[length++] = (*s++);
		c = covbuf[length-1];
	}	
		
	LCD_Draw_Color(148,146,72,23, COLOR_BLACK);

	GUI_DisplayCenterStringAt(&covbuf[0], 148,146,72, COLOR_WHITE, 23);
}

