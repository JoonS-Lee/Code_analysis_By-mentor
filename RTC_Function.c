#include "FLV_Cluster_APP.h"

const unsigned char Month_Day[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

RTC_TimeTypeDef sTime;
RTC_DateTypeDef sDate;
extern RTC_HandleTypeDef hrtc;

st_DATA_RTC RTC_Data;
st_DATA_RTC RTC_Data_Old;
st_DATA_RTC sChangeRTC;


extern EEPROM_MODEL_DATA2	InfoModel2;


const unsigned char Calendar[12] = { 31, 0, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 } ;

void write_RTC(st_DATA_RTC *RTC_Data)
{
    sDate.Year = RTC_Data->Year;
    sDate.Month = RTC_Data->Month;
    sDate.Date = RTC_Data->Date;
    sTime.Hours = RTC_Data->Hours;
    sTime.Minutes = RTC_Data->Minutes;
	sTime.Seconds = RTC_Data->Seconds;

    HAL_RTC_SetDate(&hrtc, &sDate, FORMAT_BIN);
    HAL_RTC_SetTime(&hrtc, &sTime, FORMAT_BIN);
}

void read_RTC(st_DATA_RTC *temp_RTC_Data)
{    
	HAL_RTC_GetTime(&hrtc, &sTime, FORMAT_BIN);
	HAL_RTC_GetDate(&hrtc, &sDate, FORMAT_BIN);

	//memcpy(&temp_RTC_Data->WeekDay, &sDate, sizeof(RTC_DateTypeDef));
	//memcpy(&temp_RTC_Data->Hours, &sTime, sizeof(RTC_TimeTypeDef));

	temp_RTC_Data->Year    = sDate.Year;
    temp_RTC_Data->Month   = sDate.Month;
    temp_RTC_Data->Date		= sDate.Date;
    temp_RTC_Data->Hours    = sTime.Hours;
    temp_RTC_Data->Minutes  = sTime.Minutes;
    temp_RTC_Data->Seconds = sTime.Seconds;

	LocalTimeSet(temp_RTC_Data);	
}


void CalculateTimeGap(st_DATA_RTC *Time1, st_DATA_RTC *Time2, unsigned int Offset1, unsigned int Offset2, unsigned int *TimeGap, unsigned char *Sign)
{
	unsigned int Time1_Sec;
	unsigned int Time2_Sec;

	unsigned int i;

        
    Time1_Sec = 0;
    Time2_Sec = 0;

    if (Time1->Year != 0) //++ 160621 LJH RTC
    {
        Time1_Sec += (Time1->Year - 1) * 365 * 24 * 60 * 60;
        Time1_Sec += ((Time1->Year - 1) / 4) * 24 * 60 * 60;
    }
    
    if (Time2->Year != 0) //++ 160621 LJH RTC
    {
        Time2_Sec += (Time2->Year - 1) * 365 * 24 * 60 * 60;
        Time2_Sec += ((Time2->Year - 1) / 4) * 24 * 60 * 60;
    }
    
    if (Time1->Month != 0)
    {
        for (i = 0; i < (Time1->Month - 1); i++)
        {
            Time1_Sec += Month_Day[i] * 24 * 60 * 60;

            if (i == 1)
            {
                if ((Time1->Year % 4) == 0)
                {
                    Time1_Sec += 24 * 60 * 60;
                }
            }
        }
    }

    if (Time2->Month != 0)
    {
        for (i = 0; i < (Time2->Month - 1); i++)
        {
            Time2_Sec += Month_Day[i] * 24 * 60 * 60;

            if (i == 1)
            {
                if ((Time2->Year % 4) == 0)
                {
                    Time2_Sec += 24 * 60 * 60;
                }
            }
        }
    }

    if (Time1->Date != 0)
        Time1_Sec += (Time1->Date -1) * 24 * 60 * 60;

    if (Time2->Date != 0)
        Time2_Sec += (Time2->Date -1) * 24* 60 * 60;

    Time1_Sec += (Time1->Hours) * 60 * 60;
    Time2_Sec += (Time2->Hours) * 60 * 60;
    Time1_Sec += (Time1->Minutes) * 60;
    Time2_Sec += (Time2->Minutes) * 60;

    Time1_Sec += Time1->Seconds;
    Time2_Sec += Time2->Seconds;

    Time1_Sec += Offset1;
    Time2_Sec += Offset2;

    if (Time1_Sec >= Time2_Sec)
    {
        *Sign = 0;
        *TimeGap = (Time1_Sec - Time2_Sec);
    }
    else
    {
        *Sign = 1;
        *TimeGap = (Time2_Sec - Time1_Sec);
    }

	
}


unsigned int RTC_To_Second(st_DATA_RTC *temp_RTC_Data)
{
	unsigned int RTC_Second;
	unsigned int i;
	unsigned int Feb_Date;

	RTC_Second = 0;

	RTC_Second += (unsigned int)(temp_RTC_Data->Year) * 365 * 24 * 60 * 60;
	RTC_Second += ((unsigned int)(temp_RTC_Data->Year - 1) / 4 + 1) * 24 * 60 * 60;

	for (i = 0; i < (temp_RTC_Data->Month - 1); i++)
	{
		if (i == 1)
		{
			if ((temp_RTC_Data->Year % 4) == 0)
			{
				Feb_Date = 29;
				RTC_Second += Feb_Date * 24 * 60 * 60;
			}
			else
			{
				Feb_Date = 28;
				RTC_Second += Feb_Date * 24 * 60 * 60;
			}
		}
		else
		{
			RTC_Second += (unsigned int)(Calendar[i]) * 24 * 60 * 60;
		}
	}

	RTC_Second += (unsigned int)(temp_RTC_Data->Date - 1) * 24 * 60 * 60;

	RTC_Second += (unsigned int)(temp_RTC_Data->Hours) * 60 * 60;

	RTC_Second += (unsigned int)(temp_RTC_Data->Minutes) * 60;

	RTC_Second += (unsigned int)(temp_RTC_Data->Seconds);

	return RTC_Second;	
}


st_DATA_RTC Second_To_RTC(unsigned int RTC_Second)
{
	st_DATA_RTC NewRTC;
	unsigned int RTC_Date;
	unsigned int RTC_Year_Date;
	unsigned int i;
	unsigned int RTC_Month;
	
	NewRTC.Seconds = RTC_Second % 60;
	NewRTC.Minutes = (RTC_Second / 60) % 60;
	NewRTC.Hours = (RTC_Second / 3600) % 24;

	NewRTC.Year = 0;
	NewRTC.Month = 1;	

	RTC_Date = RTC_Second / 86400;

	RTC_Year_Date = 366;

	while (RTC_Date >= RTC_Year_Date)
	{
		RTC_Date -= RTC_Year_Date;
		NewRTC.Year++;

		if ((NewRTC.Year % 4) == 0)
		{
			RTC_Year_Date = 366;
		}
		else
		{
			RTC_Year_Date = 365;
		}
	}

	for (i = 0; i < 12; i++)
	{
		if (i == 1)
		{
			if ((NewRTC.Year % 4) == 0)
			{
				RTC_Month = 29;
			}
			else
			{
				RTC_Month = 28;
			}
		}
		else
		{
			RTC_Month = Calendar[i];
		}

		if (RTC_Date < RTC_Month)
		{
			NewRTC.Month = i + 1;
			NewRTC.Date = RTC_Date + 1;
			break;
		}
		else
		{
			RTC_Date -= RTC_Month;
		}
	}

	return NewRTC;
}

unsigned char Check_ESL_running(st_DATA_RTC *RTC_Data_Current, st_DATA_RTC *RTC_Data_ESL)
{
	unsigned int RTC_Second;
	unsigned int RTC_ESL_Second;
	

	RTC_Second = RTC_To_Second(RTC_Data_Current);
	RTC_ESL_Second = RTC_To_Second(RTC_Data_ESL);

	if (RTC_Second >= RTC_ESL_Second)
	{
		return ESL_ENABLE;
	}
	else
	{
		return ESL_DISABLE;
	}
}

void LocalTimeSet(st_DATA_RTC *rtc_data)
{
	st_DATA_RTC NewRTC;

	NewRTC = Calculate_RTC_LHO_Applied(rtc_data, HCESPN.H951, 0);

	rtc_data->Year = HCESPN.H941[0] = HCESPN.H940[0] = NewRTC.Year;
	rtc_data->Month = HCESPN.H941[1] = HCESPN.H940[1] = NewRTC.Month;
	rtc_data->Date = HCESPN.H941[2] = HCESPN.H940[2] = NewRTC.Date;
	rtc_data->Hours = HCESPN.H941[3] = NewRTC.Hours;
	rtc_data->Minutes = HCESPN.H941[4] = NewRTC.Minutes;
	rtc_data->Seconds = HCESPN.H941[5] = NewRTC.Seconds;
	rtc_data->WeekDay= HCESPN.H942 = NewRTC.WeekDay;
}


st_DATA_RTC Calculate_RTC_Offset(st_DATA_RTC *temp_RTC_Data, unsigned int Offset, unsigned char Direction)
{
	st_DATA_RTC NewRTC;
	unsigned int RTC_Second;

	RTC_Second = RTC_To_Second(temp_RTC_Data);

	if (Direction == 0)
	{
		RTC_Second += Offset;
	}
	else
	{
		RTC_Second -= Offset;
	}

	NewRTC = Second_To_RTC(RTC_Second);


	return NewRTC;
}

void CalculateDay(st_DATA_RTC *RTC_Data)
{
	unsigned char i;
	unsigned int AddDay;

	AddDay = 0;
	AddDay += RTC_Data->Year - 1;
	AddDay += (RTC_Data->Year - 1) / 4;

	for (i = 0; i < (RTC_Data->Month - 1); i++)
	{
		if (i == 1)
		{
			if ((RTC_Data->Year % 4) == 0)
			{
				AddDay += 29;
			}
			else
			{
				AddDay += 28;
			}
		}
		else
		{
			AddDay += Calendar[i];
		}
	}

	AddDay += RTC_Data->Date;

	RTC_Data->WeekDay = (AddDay % 7) + 1;

}

unsigned char LHOAutoSetting(unsigned char *OldPosition, unsigned char *Position, unsigned char *LHO_Result)
{
	unsigned short OldPositionX;
	unsigned short OldPositionY;
	unsigned short PositionX;
	unsigned short PositionY;
	unsigned char NeedToChange;

	if (Position[3] > 90)
		return 0;

	if (Position[7] > 180)
		return 0;

	if ((OldPosition[2] & (1 << 7)) == (1 << 7))
	{
		OldPositionY = 90 - (unsigned short)(OldPosition[3]);
	}
	else
	{
		OldPositionY = 90 + (unsigned short)(OldPosition[3]);
	}

	OldPositionY = OldPositionY % 180;

	if ((Position[2] & (1 << 7)) == (1 << 7))
	{
		PositionY = 90 - (unsigned short)(Position[3]);
	}
	else
	{
		PositionY = 90 + (unsigned short)(Position[3]);
	}

	PositionY = PositionY % 180;

	if ((OldPosition[6] & (1 << 7)) == (1 << 7))
	{
		OldPositionX = 180 - (unsigned short)(OldPosition[7]);
	}
	else
	{
		OldPositionX = 180 + (unsigned short)(OldPosition[7]);
	}

	OldPositionX = OldPositionX % 360;	

	if ((Position[6] & (1 << 7)) == (1 << 7))
	{
		PositionX = 180 - (unsigned short)(Position[7]);
	}
	else
	{
		PositionX = 180 + (unsigned short)(Position[7]);
	}

	PositionX = PositionX % 360;	

	NeedToChange = 0;

	if (NeedToChange == 0)
	{
		if (OldPosition[3] == 0xff)
		{
			NeedToChange = 1;
		}
	}

	if (NeedToChange == 0)
	{
		if (OldPosition[7] == 0xff)
		{
			NeedToChange = 1;
		}
	}

	if (NeedToChange == 0)
	{
		if (OldPositionY >= PositionY)
		{
			if ((OldPositionY - PositionY) >= 10)
			{
				NeedToChange = 1;
			}
		}
		else
		{
			if ((PositionY - OldPositionY) >= 10)
			{
				NeedToChange = 1;
			}
		}
	}

	if (NeedToChange == 0)
	{
		if (OldPositionX >= PositionX)
		{
			if ((OldPositionX - PositionX) >= 10)
			{
				NeedToChange = 1;
			}
		}
		else
		{
			if ((PositionX - OldPositionX) >= 10)
			{
				NeedToChange = 1;
			}
		}
	}

	if (NeedToChange == 1)
	{
		#if 0
		*LHO_Result = LHOData[PositionY][PositionX];
		#endif

		return 1;
	}
	else
	{
		return 0;
	}
}


st_DATA_RTC Calculate_RTC_LHO_Applied(st_DATA_RTC *RTC_Data, unsigned char LHO_Value, unsigned char Direction)
{
	st_DATA_RTC NewRTC;
	unsigned char LHO_Value_Temp;

	if((LHO_Value > 200)||(LHO_Value < 1))
	{
		LHO_Value = 118; // KOREA

	}

	if (Direction == 0)
	{
		LHO_Value_Temp = LHO_Value;
	}
	else
	{
		LHO_Value_Temp = 200 - LHO_Value;
	}

	if (LHO_Value_Temp >= 100)
	{
		NewRTC = Calculate_RTC_Offset(RTC_Data, (LHO_Value_Temp - 100) * 1800, 0);
	}
	else
	{
		NewRTC = Calculate_RTC_Offset(RTC_Data, (100 -LHO_Value_Temp) * 1800, 1);
	}

	CalculateDay(&NewRTC);
	
	
	return NewRTC;
}



