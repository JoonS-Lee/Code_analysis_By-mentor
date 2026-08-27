#include "FLV_Cluster_APP.h"

extern RTC_DateTypeDef sDate;


extern RTC_HandleTypeDef hrtc;

extern st_DATA_RTC RTC_Data;
extern st_DATA_RTC RTC_Data_Old;

void DisplayFullClock()
{
	unsigned char x_Pos[]={27,47,67,87,107,112,132,152,157,177};
	unsigned char y_Pos = 43;
	
	unsigned char x1_Pos[]={26,56,91,102,132,166};
	unsigned char y1_Pos = 93;
	

	read_RTC(&RTC_Data);
	
	if(OldScreenIndex != ScreenIndex)
	{
		DisplayBackground();
		RTC_Data_Old.Minutes= 0xff;
		RTC_Data_Old.Date= 0xff;
		OldScreenIndex = ScreenIndex;
		
		LCD_Draw_Color(0, y_Pos, LCD_WIDTH, 32, COLOR_BLACK);
		PCXtoBMP_16bit(x_Pos[0], y_Pos, 19, 32, FL_Image.bottom_num[2]);
		PCXtoBMP_16bit(x_Pos[1], y_Pos, 19, 32, FL_Image.bottom_num[0]);
		PCXtoBMP_16bit(x_Pos[2], y_Pos, 19, 32, FL_Image.bottom_num[RTC_Data.Year/10]);
		PCXtoBMP_16bit(x_Pos[3], y_Pos, 19, 32, FL_Image.bottom_num[RTC_Data.Year%10]);
		PCXtoBMP_16bit(x_Pos[4], y_Pos+28, 4, 4, FL_Image.bottom_dot);
		PCXtoBMP_16bit(x_Pos[5], y_Pos, 19, 32, FL_Image.bottom_num[RTC_Data.Month/10]);
		PCXtoBMP_16bit(x_Pos[6], y_Pos, 19, 32, FL_Image.bottom_num[RTC_Data.Month%10]);
		PCXtoBMP_16bit(x_Pos[7], y_Pos+28, 4, 4, FL_Image.bottom_dot);
		PCXtoBMP_16bit(x_Pos[8], y_Pos, 19, 32, FL_Image.bottom_num[RTC_Data.Date/10]);
		PCXtoBMP_16bit(x_Pos[9], y_Pos, 19, 32, FL_Image.bottom_num[RTC_Data.Date%10]);

		unsigned char ampm, hour;
		

		hour = RTC_Data.Hours;
		if(hour>11)
		{
			ampm = CLOCK_PM;
			hour -= 12;
		}
		else
			ampm = CLOCK_AM;

		if(hour == 0)
			hour = 12;

		LCD_Draw_Color(0, y1_Pos, LCD_WIDTH, 32, COLOR_BLACK);
		PCXtoBMP_16bit(x1_Pos[0], y1_Pos, 29, 45, FL_Image.clock_num[hour/10]);
		PCXtoBMP_16bit(x1_Pos[1], y1_Pos, 29, 45, FL_Image.clock_num[hour%10]);
		PCXtoBMP_16bit(x1_Pos[2], 102, 5, 28, FL_Image.clock);
		PCXtoBMP_16bit(x1_Pos[3], y1_Pos, 29, 45, FL_Image.clock_num[RTC_Data.Minutes/10]);
		PCXtoBMP_16bit(x1_Pos[4], y1_Pos, 29, 45, FL_Image.clock_num[RTC_Data.Minutes%10]);

		PCXtoBMP_16bit(x1_Pos[5], 119, 29, 19, (ampm == CLOCK_AM)?FL_Image.clock_AM:FL_Image.clock_PM);
	}

		
}
