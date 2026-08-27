#include "FLV_Cluster_APP.h"

extern CAN_ECU_DATA_65262						RX_ECU_DATA_65262;

unsigned char OldFuelLevel;
unsigned char OldCoolantTemp;

extern unsigned char Engine_Type;

void InitialGaugeValuable()
{
	OldFuelLevel = 0xff;
	OldCoolantTemp = 0xFF;
}

void DisplayGaugeBack()
{
	#if 0 //++,--, 201019 ysm
	PCXtoBMP_16bit(4, 7, 32, 132, FL_Image.gauge_plate);
	PCXtoBMP_16bit(5, 144, 29, 25, FL_Image.gauge_icon_fuel);

	PCXtoBMP_16bit(185, 7, 32, 132, FL_Image.gauge_plate);
	PCXtoBMP_16bit(186, 144, 29, 25, FL_Image.gauge_icon_coolant);

	#else
	/*
	if(Engine_Type == DIESEL_TYPE)
	{
		PCXtoBMP_16bit(2, 8, 34, 160, FL_Image.gauge_plate);
		PCXtoBMP_16bit(4, 11, 29, 28, FL_Image.gauge_icon_fuel_2);
	}*/
	PCXtoBMP_16bit(2, 8, 34, 160, FL_Image.gauge_plate);
	PCXtoBMP_16bit(4, 11, 29, 28, FL_Image.gauge_icon_fuel_2);
	#endif
	PCXtoBMP_16bit(185, 8, 34, 160, FL_Image.gauge_plate);
	PCXtoBMP_16bit(186, 11, 29, 28, FL_Image.gauge_icon_coolant_2);
}

void DisplayFuel()
{
	uint32_t uwStartAddress;

	if(OldFuelLevel != HCESPN.Fuel_Level)
	{
		OldFuelLevel = HCESPN.Fuel_Level;

		for(int i=0;i<7;i++) // 8->7
		{
			if(i < OldFuelLevel)
			{
				if(OldFuelLevel == 1)
					uwStartAddress = FL_Image.gauge_left_red_2;
				else
					uwStartAddress = FL_Image.gauge_left_green_2;	

				PCXtoBMP_16bit(8, 147-i*17, 22, 14, uwStartAddress);
			}			
			else
				LCD_Draw_Color(8, 147-i*17, 22, 14, COLOR_BLACK);			
			
		}
	}
}

void DisplayCoolant()
{
	uint32_t uwStartAddress;
	short CoolantTemp;
	UCHAR CoolantLevel;


	if(OldCoolantTemp != RX_ECU_DATA_65262.CoolantTemp_110)
	{
		OldCoolantTemp = RX_ECU_DATA_65262.CoolantTemp_110;

		CoolantTemp = OldCoolantTemp - 40;

		if(CoolantTemp < 50)		CoolantLevel = 1;
		else if(CoolantTemp < 60)	CoolantLevel = 2;
		else if(CoolantTemp < 70)	CoolantLevel = 3;
		else if(CoolantTemp < 80)	CoolantLevel = 4;
		else if(CoolantTemp < 106)	CoolantLevel = 5;
		else if(CoolantTemp < 109)	CoolantLevel = 6;
		else if(CoolantTemp < 112)	CoolantLevel = 7;
		else if(CoolantTemp < 115)	CoolantLevel = 8;
		else if(CoolantTemp < 120)	CoolantLevel = 9;
		else						CoolantLevel = 10;

		for(UCHAR i=0;i<10;i++)
		{
			if(i < CoolantLevel)
			{
				if(CoolantLevel == 10)
					uwStartAddress = FL_Image.gauge_right_red_2;
				else
					uwStartAddress = FL_Image.gauge_right_blue_2;

				PCXtoBMP_16bit(191, 152-i*12, 22, 10, uwStartAddress);
			}
			else
				LCD_Draw_Color(191, 152-i*12, 22, 10, COLOR_BLACK);	

			
		}
	}
}


void DisplayMainGaugeBackground()
{
	InitialGaugeValuable();

	DisplayGaugeBack();

	//if(Engine_Type == DIESEL_TYPE)
	DisplayFuel();
	DisplayCoolant();	
}

void DisplayMainGauge()
{
	//if(Engine_Type == DIESEL_TYPE)
	DisplayFuel();	
	DisplayCoolant();
}

