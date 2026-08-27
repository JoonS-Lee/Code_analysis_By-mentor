#include "FLV_Cluster_APP.h"

UCHAR OldClcokSet=0xff;
UCHAR AMPMStatus=0xff;

extern RTC_HandleTypeDef hrtc;


extern RTC_DateTypeDef sDate;
extern unsigned char ChangeAMPMStatus;




unsigned char ChangeDateTime = 0;

extern st_DATA_RTC RTC_Data;
extern st_DATA_RTC RTC_Data_Old;
extern st_DATA_RTC sChangeRTC;

extern USHORT* StringSmallList[NUMBEROFSMALLLIST];

void DisplayCurrentClock()
{
	UCHAR buf[30]={0,};
	USHORT covbuf[30]={0,};
	unsigned char hour;


	read_RTC(&RTC_Data);
	
	if((RTC_Data.Year != RTC_Data_Old.Year) || (RTC_Data.Month!= RTC_Data_Old.Month) || (RTC_Data.Date!= RTC_Data_Old.Date)
		|| (RTC_Data.Hours!= RTC_Data_Old.Hours) || (RTC_Data.Minutes!= RTC_Data_Old.Minutes) || (RTC_Data.TimeFormat!= RTC_Data_Old.TimeFormat))
	{
		RTC_Data_Old = RTC_Data;
		
		if(RTC_Data_Old.Hours >= 12)
			ChangeAMPMStatus = AMPMStatus = CLOCK_PM;
		else
			ChangeAMPMStatus = AMPMStatus = CLOCK_AM;

		hour = RTC_Data_Old.Hours;
		if(hour > 12)
			hour -= 12;
		else if(hour == 0)
			hour = 12;
		
		sprintf ( (char*)buf, "%02d/%02d/%02d %s %02d:%02d",RTC_Data_Old.Year, RTC_Data_Old.Month, RTC_Data_Old.Date,
			(AMPMStatus == CLOCK_AM)?"AM":"PM",hour,RTC_Data_Old.Minutes) ; 
		String_Func(covbuf, buf);
		LCD_Draw_Color(6, 48, LCD_WIDTH, 38, COLOR_BLACK);
		GUI_DisplayCenterStringAt(&covbuf[0], 0, 48+9, LCD_WIDTH, COLOR_MENU_BLUE,23);
	}

}

void DisplayClockChange(unsigned short Number, unsigned char nList, unsigned char bEnable)
{
	USHORT y_Pos[] = {48, 90, 132};

	UCHAR buf[5]={0,};
	USHORT covbuf[5]={0,};
	
	if(OldScreenIndex != ScreenIndex)
	{
		OldCurserIndex = 0xff;
		CurserIndex = CURSER_NONE;

		if(!bEnable)
		{
			LCD_Draw_Color(6, y_Pos[nList], 31, 39, COLOR_MENU_GRAY);
			PCXtoBMP_16bit(16, y_Pos[nList]+11, 10, 15, FL_Image.menu_arrow_nor_2);

			LCD_Draw_Color(183, y_Pos[nList], 31, 39, COLOR_MENU_GRAY);
			PCXtoBMP_16bit(193, y_Pos[nList]+11, 10, 15, FL_Image.menu_arrow_nor);
		}
	}

	if((OldScreenIndex != ScreenIndex) || (OldClcokSet != Number))
	{
		if(((ScreenIndex & 0xFFF00000) == SCREEN_STATE_MENU_DISPLAYSETTING_CLOCK_TIME_TOP) && (nList == 0))
			sprintf ( (char*)buf, "%s",(Number == CLOCK_AM)?"AM":"PM") ; 
		else if(((ScreenIndex & 0xFFF00000) == SCREEN_STATE_MENU_DISPLAYSETTING_CLOCK_TIME_TOP) && (nList == 1))
		{
			unsigned char hour = Number;
			if(hour > 12)
				hour -= 12;
			else if(hour == 0)
				hour = 12;
			sprintf ( (char*)buf, "%02d",hour) ; 
		}
		else
			sprintf ( (char*)buf, "%02d",Number) ; 

		LCD_Draw_Color(74, y_Pos[nList]+9, 65, 23, COLOR_BLACK);
		String_Func(covbuf, buf);
		GUI_DisplayCenterStringAt(&covbuf[0], 0, y_Pos[nList]+9, LCD_WIDTH, bEnable?COLOR_WHITE:COLOR_MENU_LIGHTGRAY, FONTSIZE);	

		if(bEnable)
			OldClcokSet = Number;
	}


	if((OldCurserIndex != CurserIndex) && bEnable)
	{
		LCD_Draw_Color(6, y_Pos[nList], 31, 39, (CurserIndex == CURSER_UP_LEFT)?COLOR_MENU_BLUE:COLOR_MENU_GRAY);
		PCXtoBMP_16bit(16, y_Pos[nList]+11, 10, 15, (CurserIndex == CURSER_UP_LEFT)?FL_Image.menu_arrow_display_select_2:FL_Image.menu_arrow_nor_2);

		LCD_Draw_Color(183, y_Pos[nList], 31, 39, (CurserIndex == CURSER_DOWN_RIGHT)?COLOR_MENU_BLUE:COLOR_MENU_GRAY);
		PCXtoBMP_16bit(193, y_Pos[nList]+11, 10, 15, (CurserIndex == CURSER_DOWN_RIGHT)?FL_Image.menu_arrow_display_select:FL_Image.menu_arrow_nor);
		OldCurserIndex = CurserIndex;
	}
}

void DisplayClock()
{
	switch(ScreenIndex)
	{
		case SCREEN_STATE_MENU_DISPLAYSETTING_CLOCK_TIME_TOP:
		case SCREEN_STATE_MENU_DISPLAYSETTING_CLOCK_DATE_TOP:
			if((OldScreenIndex != SCREEN_STATE_MENU_DISPLAYSETTING_CLOCK_TIME_TOP)
				&& (OldScreenIndex != SCREEN_STATE_MENU_DISPLAYSETTING_CLOCK_DATE_TOP))
			{
				SetListString(3, StringEtc[SIGN_NOSTRING], (USHORT*)*StringSentence[174], (USHORT*)*StringSentence[82]);
				DisplayListAll(((ScreenIndex&0x00F00000)>>20)+1);
				DrawMenuFull();
				
				RTC_Data_Old.Year = 0xff;
				RTC_Data_Old.Hours = 0xff;
			}
			else if(OldScreenIndex != ScreenIndex)
			{
				DisplayList(((ScreenIndex&0x00F00000)>>20)+1);
			}
			DisplayCurrentClock();
			break;
		case SCREEN_STATE_MENU_DISPLAYSETTING_CLOCK_DATE_SETYEAR:
		case SCREEN_STATE_MENU_DISPLAYSETTING_CLOCK_DATE_SETMONTH:
		case SCREEN_STATE_MENU_DISPLAYSETTING_CLOCK_DATE_SETDAY:
			if(OldScreenIndex == SCREEN_STATE_MENU_DISPLAYSETTING_CLOCK_DATE_TOP)
			{
				DisplayMenuBackgroundBG();
				DisplayClockChange(sChangeRTC.Year, 0, (ScreenIndex == SCREEN_STATE_MENU_DISPLAYSETTING_CLOCK_DATE_SETYEAR));
				DisplayClockChange(sChangeRTC.Month, 1, (ScreenIndex == SCREEN_STATE_MENU_DISPLAYSETTING_CLOCK_DATE_SETMONTH));
				DisplayClockChange(sChangeRTC.Date, 2, (ScreenIndex == SCREEN_STATE_MENU_DISPLAYSETTING_CLOCK_DATE_SETDAY));
				OldScreenIndex = ScreenIndex;
			}
			else if(ScreenIndex == SCREEN_STATE_MENU_DISPLAYSETTING_CLOCK_DATE_SETYEAR)  
			{
				if((OldScreenIndex != ScreenIndex) || (OldCurserIndex != CurserIndex) || (OldClcokSet != sChangeRTC.Year))
					DisplayClockChange(sChangeRTC.Year, 0, 1);
			}
			else if(ScreenIndex == SCREEN_STATE_MENU_DISPLAYSETTING_CLOCK_DATE_SETMONTH)  
			{
				if((OldScreenIndex != ScreenIndex) || (OldCurserIndex != CurserIndex) || (OldClcokSet != sChangeRTC.Month))
					DisplayClockChange(sChangeRTC.Month, 1, 1);
			}
			else if(ScreenIndex == SCREEN_STATE_MENU_DISPLAYSETTING_CLOCK_DATE_SETDAY)  
			{
				if((OldScreenIndex != ScreenIndex) || (OldCurserIndex != CurserIndex) || (OldClcokSet != sChangeRTC.Date))
					DisplayClockChange(sChangeRTC.Date, 2, 1);
			}
			if(OldScreenIndex != ScreenIndex)
			{
				switch(OldScreenIndex)
				{
					case SCREEN_STATE_MENU_DISPLAYSETTING_CLOCK_DATE_SETYEAR:
						DisplayClockChange(sChangeRTC.Year, 0, 0);
						break;
					case SCREEN_STATE_MENU_DISPLAYSETTING_CLOCK_DATE_SETMONTH:
						DisplayClockChange(sChangeRTC.Month, 1, 0);
						break;
					case SCREEN_STATE_MENU_DISPLAYSETTING_CLOCK_DATE_SETDAY:
						DisplayClockChange(sChangeRTC.Date, 2, 0);
						break;
				}
				OldScreenIndex = ScreenIndex;
			}
			break;
		case SCREEN_STATE_MENU_DISPLAYSETTING_CLOCK_TIME_SETAMPM:
		case SCREEN_STATE_MENU_DISPLAYSETTING_CLOCK_TIME_SETHOUR:
		case SCREEN_STATE_MENU_DISPLAYSETTING_CLOCK_TIME_SETMINUTE:
			if(OldScreenIndex == SCREEN_STATE_MENU_DISPLAYSETTING_CLOCK_TIME_TOP)
			{
				DisplayMenuBackgroundBG();
				DisplayClockChange(ChangeAMPMStatus, 0, (ScreenIndex == SCREEN_STATE_MENU_DISPLAYSETTING_CLOCK_TIME_SETAMPM));
				DisplayClockChange(sChangeRTC.Hours, 1, (ScreenIndex == SCREEN_STATE_MENU_DISPLAYSETTING_CLOCK_TIME_SETHOUR));
				DisplayClockChange(sChangeRTC.Minutes, 2, (ScreenIndex == SCREEN_STATE_MENU_DISPLAYSETTING_CLOCK_TIME_SETMINUTE));
				OldScreenIndex = ScreenIndex;
			}
			else if(ScreenIndex == SCREEN_STATE_MENU_DISPLAYSETTING_CLOCK_TIME_SETAMPM)  
			{
				if((OldScreenIndex != ScreenIndex) || (OldCurserIndex != CurserIndex) || (OldClcokSet != ChangeAMPMStatus))
					DisplayClockChange(ChangeAMPMStatus, 0, 1);
			}
			else if(ScreenIndex == SCREEN_STATE_MENU_DISPLAYSETTING_CLOCK_TIME_SETHOUR)  
			{
				if((OldScreenIndex != ScreenIndex) || (OldCurserIndex != CurserIndex) || (OldClcokSet != sChangeRTC.Hours))
				DisplayClockChange(sChangeRTC.Hours, 1, 1);
			}
			else if(ScreenIndex == SCREEN_STATE_MENU_DISPLAYSETTING_CLOCK_TIME_SETMINUTE)  
			{
				if((OldScreenIndex != ScreenIndex) || (OldCurserIndex != CurserIndex) || (OldClcokSet != sChangeRTC.Minutes))
				DisplayClockChange(sChangeRTC.Minutes, 2, 1);
			}
			if(OldScreenIndex != ScreenIndex)
			{
				switch(OldScreenIndex)
				{
					case SCREEN_STATE_MENU_DISPLAYSETTING_CLOCK_TIME_SETAMPM:
						DisplayClockChange(ChangeAMPMStatus, 0, 0);
						break;
					case SCREEN_STATE_MENU_DISPLAYSETTING_CLOCK_TIME_SETHOUR:
						DisplayClockChange(sChangeRTC.Hours, 1, 0);
						break;
					case SCREEN_STATE_MENU_DISPLAYSETTING_CLOCK_TIME_SETMINUTE:
						DisplayClockChange(sChangeRTC.Minutes, 2, 0);
						break;
				}
				OldScreenIndex = ScreenIndex;
			}
			break;
	}
}
