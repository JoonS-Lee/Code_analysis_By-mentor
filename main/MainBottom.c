#include "FLV_Cluster_APP.h"

extern unsigned char Hour_start_flag;


UCHAR OldLoadIcon;
UCHAR OldFlag_Load_Buzzer;

UINT32	OldWeight;
UINT32	OldHourmeter;

UCHAR	cntHourmeter;
UCHAR	HourmeterFlag;
UCHAR	OldHourmeterFlag;

UCHAR MainBottomMode;
UCHAR OldMainBottomMode;

extern st_AD_DATA 		AD_DATA;
extern st_BUZZER			BUZZER_FLAG;
extern UCHAR DotCursur;


void InitialBottomValuable()
{

	OldFlag_Load_Buzzer = 0xFF;
	OldLoadIcon = 0xFF;

	OldWeight = 0xffffffff;
	OldHourmeter = 0xffffffff;


	cntHourmeter = 0;
	HourmeterFlag = 0;
	OldHourmeterFlag = 0xff;
	OldMainBottomMode = 0xff;

}

void DisplayUnit()
{
	PCXtoBMP_16bit(44, 139, 28, 20, FL_Image.bottom_icon_fork_load);
	
	UINT32 uwStartAddress;
	if(InfoDisplaySetting.WeightUnit == UNIT_WEIGHT_TON)
		uwStartAddress = FL_Image.bottom_unit_ton;
	else
		uwStartAddress = FL_Image.bottom_unit_lb;	
	
	PCXtoBMP_16bit(166, 145, 19, 23, uwStartAddress);

	if(InfoDisplaySetting.WeightUnit == UNIT_WEIGHT_TON)
	{
		if(InfoDisplaySetting.WeightDisplayUnit == WEIGHT_DISPLAYSET_100)
			DotCursur = 1;
		else
			DotCursur = 2;
	}
	else 
		DotCursur = 0;

}

void DisplayLoad()
{
	UINT32 tempRound;
	UCHAR Number[4];
	UCHAR x_Pos[]={144,123,102,81};

	if((OldWeight != AD_DATA.Weight_Current)|| (OldFlag_Load_Buzzer != BUZZER_FLAG.LoadSensor))
	{
		OldWeight = AD_DATA.Weight_Current;
		OldFlag_Load_Buzzer = BUZZER_FLAG.LoadSensor;

		memset(&Number[0], 0xff, sizeof(Number));

		if(InfoDisplaySetting.WeightUnit == UNIT_WEIGHT_TON)
		{
			if(InfoDisplaySetting.WeightDisplayUnit == WEIGHT_DISPLAYSET_100)
			{
				if((OldWeight % 10) > 4)
					tempRound = (OldWeight + (10-(OldWeight % 10)))/10;
				else
					tempRound = OldWeight/10;
			}
			else
			{
				tempRound = OldWeight;
			}
		}
		else
		{
			tempRound = (UINT32)((float)OldWeight* 22.04667);
		}

		Number[0] = tempRound % 10;
		Number[1]  = (tempRound % 100) / 10;
		Number[2]  = (tempRound % 1000) / 100;
		Number[3]  = (tempRound % 10000) / 1000;

		if(Number[3] == 0)
			Number[3] = 0xff;

		if((Number[3] == 0xff) && (Number[2] == 0) && (DotCursur != 2))
			Number[2] = 0xff;

		if((Number[3] == 0xff) && (Number[2] == 0xff) && (Number[1] == 0) && (DotCursur == 0))
			Number[1] = 0xff;

		for(int i=0;i<4;i++)
		{
			if(Number[i] == 0xff)
				LCD_Draw_Color(x_Pos[i], 133, 19, 32, COLOR_BLACK);
			else
			{
				if(OldFlag_Load_Buzzer==1)
					PCXtoBMP_16bit(x_Pos[i], 133, 19, 32, FL_Image.bottom_red_num[Number[i]]);
				else
					PCXtoBMP_16bit(x_Pos[i], 133, 19, 32, FL_Image.bottom_num[Number[i]]);
			}
		}

		if(DotCursur == 1)
			PCXtoBMP_16bit(141, 164, 4, 4, (OldFlag_Load_Buzzer==1)?FL_Image.bottom_red_dot:FL_Image.bottom_dot);
		else if(DotCursur == 2)
			PCXtoBMP_16bit(120, 164, 4, 4, (OldFlag_Load_Buzzer==1)?FL_Image.bottom_red_dot:FL_Image.bottom_dot);
	}
}

void CheckMeterFlag()
{
	if(Hour_start_flag == 1)
	{
		cntHourmeter++;

		if(cntHourmeter > 0 && cntHourmeter < 10)			HourmeterFlag = 1;
		else if(cntHourmeter >= 10 && cntHourmeter < 20)	HourmeterFlag = 2;
		else												cntHourmeter = 0;
	}
	else
	{
		HourmeterFlag = 0;
		cntHourmeter = 0;
	}

}
void DisplayHourmeterIcon()
{
	uint32_t uwStartAddress;

	if(OldHourmeterFlag != HourmeterFlag)
	{
		OldHourmeterFlag = HourmeterFlag;
		if(OldHourmeterFlag == 1)
			uwStartAddress = FL_Image.bottom_unit_hourglass_run;
		else if(OldHourmeterFlag == 2)
			uwStartAddress = FL_Image.bottom_unit_hourglass_act;
		else
			uwStartAddress = FL_Image.bottom_unit_hourglass;

		PCXtoBMP_16bit(166, 145, 19, 23, uwStartAddress);
	}
}

void DisplayHourmeter()
{
	UINT32 tempHourmeter_hour;
	UINT32 tempHourmeter_min;
	UCHAR hour_1,hour_2,hour_3,hour_4,hour_5;


	if(OldHourmeter != HCESPN.Hourmeter_1601)
	{
		OldHourmeter = HCESPN.Hourmeter_1601;

		tempHourmeter_hour = OldHourmeter / 3600;
		tempHourmeter_min = (OldHourmeter%3600)/360;		

		hour_1 = tempHourmeter_hour % 10;
		hour_2  = (tempHourmeter_hour % 100) / 10;
		hour_3  = (tempHourmeter_hour % 1000) / 100;
		hour_4  = (tempHourmeter_hour % 10000) / 1000;
		hour_5  = (tempHourmeter_hour % 100000) / 10000;

		PCXtoBMP_16bit(144, 133, 19, 32, FL_Image.bottom_num[tempHourmeter_min]);
		PCXtoBMP_16bit(123, 133, 19, 32, FL_Image.bottom_num[hour_1]);

		if((hour_5 != 0) || (hour_4 != 0) || (hour_3 != 0) || (hour_2 != 0))
			PCXtoBMP_16bit(102, 133, 19, 32, FL_Image.bottom_num[hour_2]);
		else
			LCD_Draw_Color(102, 133, 19, 32, COLOR_BLACK);

		if((hour_5 != 0) || (hour_4 != 0) || (hour_3 != 0))
			PCXtoBMP_16bit(81, 133, 19, 32, FL_Image.bottom_num[hour_3]);
		else
			LCD_Draw_Color(81, 133, 19, 32, COLOR_BLACK);

		if((hour_5 != 0) || (hour_4 != 0) )
			PCXtoBMP_16bit(60, 133, 19, 32, FL_Image.bottom_num[hour_4]);
		else
			LCD_Draw_Color(60, 133, 19, 32, COLOR_BLACK);

		if(hour_5 != 0)
			PCXtoBMP_16bit(39, 133, 19, 32, FL_Image.bottom_num[hour_4]);
		else
			LCD_Draw_Color(39, 133, 19, 32, COLOR_BLACK);

		PCXtoBMP_16bit(141, 164, 4, 4, FL_Image.bottom_dot);
	}
}

void CheckMainBottomMode()
{
	if((AD_DATA.Weight_Current>=10) && (AD_DATA.Weight_Current < 0xfbff))
		MainBottomMode = MAIN_DISPLAY_LOAD;
	else
		MainBottomMode = MAIN_DISPLAY_HOURMTER;

	if(OldMainBottomMode != MainBottomMode)
	{
		OldMainBottomMode = MainBottomMode;

		OldFlag_Load_Buzzer = 0xFF;
		OldLoadIcon = 0xFF;

		OldWeight = 0xffffffff;
		OldHourmeter = 0xffffffff;
	
		cntHourmeter = 0;
		HourmeterFlag = 0;
		OldHourmeterFlag = 0xff;

		LCD_Draw_Color(39, 133, 124, 35, COLOR_BLACK);
		if(OldMainBottomMode == MAIN_DISPLAY_LOAD)
		{
			DisplayUnit();
		}
	}

	switch(OldMainBottomMode)
	{
		case MAIN_DISPLAY_LOAD:
			DisplayLoad();
			break;
		case MAIN_DISPLAY_HOURMTER:
			CheckMeterFlag();
			DisplayHourmeterIcon();
			DisplayHourmeter();
			break;
	}
}

void DisplayMainBottomBackground()
{
	InitialBottomValuable();
	CheckMainBottomMode();
}

void DisplayMainBottom()
{
	CheckMainBottomMode();
}

