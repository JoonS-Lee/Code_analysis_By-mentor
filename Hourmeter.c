#include "FLV_Cluster_APP.h"

unsigned char Hour_start_flag;
unsigned long HourMeter_Value;
unsigned long HourMeter_cnt;

unsigned char MinHourmeter;
unsigned char MidHourmeter;
unsigned char MaxHourmeter;
unsigned char Hourmeter_Data[6];

unsigned char Hourmeter0_Save_Error_Count;
unsigned char Hourmeter1_Save_Error_Count;
unsigned char Hourmeter2_Save_Error_Count;

unsigned long HourMeter_Value0;
unsigned long	HourMeter_Value1;
unsigned long	HourMeter_Value2;

extern unsigned char EngineStartFlag;
extern unsigned char Flag_Engine_Start;


void Initialize_Hourmeter_Variables(void)
{
	unsigned short FRC_Right;

	Hour_start_flag 		= 0;
	HourMeter_Value 	= 0;
	HourMeter_cnt 		= 0;

	Check_Hourmeter_Address();

	FRC_Right = Hourmeter_read(0,Hourmeter0_Save_Error_Count);

	if(FRC_Right)
	{
		HourMeter_Value0 = 	(Hourmeter_Data[0] << (8*0)) + (Hourmeter_Data[1] << (8*1)) 
			+ (Hourmeter_Data[2] << (8*2)) + (Hourmeter_Data[3] << (8*3));

		if(HourMeter_Value0 == 0xffffffff )
		{
			HourMeter_Value0=0;
		}						
	}
	else		HourMeter_Value0=0;

	FRC_Right= Hourmeter_read(1,Hourmeter1_Save_Error_Count);

	if(FRC_Right)
	{
		HourMeter_Value1= 	(Hourmeter_Data[0] << (8*0)) + (Hourmeter_Data[1] << (8*1)) 
			+ (Hourmeter_Data[2] << (8*2)) + (Hourmeter_Data[3] << (8*3));

		if(HourMeter_Value1 == 0xffffffff )
		{
			HourMeter_Value1=0;
		}		
	}
	else		HourMeter_Value1=0;

	FRC_Right= Hourmeter_read(2,Hourmeter2_Save_Error_Count);

	if(FRC_Right)
	{
		HourMeter_Value2= 	(Hourmeter_Data[0] << (8*0)) + (Hourmeter_Data[1] << (8*1)) 
			+ (Hourmeter_Data[2] << (8*2)) + (Hourmeter_Data[3] << (8*3));

		if(HourMeter_Value2 == 0xffffffff )
		{
			HourMeter_Value2=0;
		}
	}
	else		HourMeter_Value2=0;

	if((HourMeter_Value0<=HourMeter_Value2) && (HourMeter_Value1<=HourMeter_Value2))	
	{
		HourMeter_Value=HourMeter_Value2;
		MaxHourmeter = 2;

		if(HourMeter_Value0>HourMeter_Value1)
		{
			MinHourmeter = 1;
			MidHourmeter = 0;
		}
		else
		{
			MinHourmeter=0;
			MidHourmeter = 1;
		}
	}	

	else if((HourMeter_Value0<=HourMeter_Value1) && (HourMeter_Value2<=HourMeter_Value1))	
	{
		HourMeter_Value=HourMeter_Value1;
		MaxHourmeter = 1;

		if(HourMeter_Value2>HourMeter_Value0)
		{
			MinHourmeter = 0;
			MidHourmeter = 2;
		}
		else
		{
			MinHourmeter = 2;
			MidHourmeter = 0;
		}

	}

	else if((HourMeter_Value1<=HourMeter_Value0) && (HourMeter_Value2<=HourMeter_Value0))	
	{
		HourMeter_Value=HourMeter_Value0;
		MaxHourmeter = 0;

		if(HourMeter_Value1>HourMeter_Value2)
		{
			MinHourmeter = 2;
			MidHourmeter = 1;
		}
		else
		{
			MinHourmeter = 1;
			MidHourmeter = 2;
		}
	}

  
//	HCESPN.H1601 = HourMeter_Value;
        HCESPN.Hourmeter_1601 = HCESPN.H1601 = HourMeter_Value;
	HCESPN.Hourmeter_Hour_701 = (unsigned short)(HourMeter_Value/3600);
}	


//------------------------------------------------------------------------------
// Function Name 	: Initialize_Hourmeter
// Description 	: Hour meter data를 EEPROM에 0 값으로 저장함.
// Input 			: None
// Return 		: None
// Period 		: 초기화시 한번 사용.
//------------------------------------------------------------------------------
void Initialize_Hourmeter(void)
{
	for(int i=0;i<4;i++)
	{
		EepromWrite(ADDRESS_HOURMETER0+(i*10), 0x00, 6);
		EepromWrite(ADDRESS_HOURMETER1+(i*10), 0x00, 6);
		EepromWrite(ADDRESS_HOURMETER2+(i*10), 0x00, 6);
	}
	EepromWrite(ADDRESS_HOURMETER0_ERROR_COUNT, 0x00, 1);
	EepromWrite(ADDRESS_HOURMETER1_ERROR_COUNT, 0x00, 1);
	EepromWrite(ADDRESS_HOURMETER2_ERROR_COUNT, 0x00, 1);

	TimeDelay_msec(5);
}

unsigned short Hourmeter_read(unsigned char read_num, unsigned char err_count)
{	
	unsigned int i;
	unsigned short Right_FRC;
	unsigned char tempchar[6];

	if(err_count < 4)
	{	
		EepromRead(ADDRESS_HOURMETER0+(read_num*ADDRESS_METER_TOTAL)+(ADDRESS_METER_COUNT*err_count), (unsigned char *)(&tempchar[0]), 6);

		for( i = 0; i < 6; i++)
		{
			Hourmeter_Data[i] = tempchar[i];
		}
	}
	else
	{
		for( i = 0; i < 6; i++)
		{
			Hourmeter_Data[i] = 0;			

		}
	}

	Right_FRC= MakeCrc16(&Hourmeter_Data[0], 4);

	if( (Hourmeter_Data[4] == (Right_FRC& 0xff)) && (Hourmeter_Data[5] == ((Right_FRC>> 8) & 0xff)) )
	{
		Right_FRC=1;
	}		
	else		Right_FRC=0;

	return Right_FRC;	
}

//------------------------------------------------------------------------------
// Function Name 	: Calculate_HourMeter
// Description 	: Key on시 Hourmeter를 계산함.
// Input 			: None
// Return 		: None
// Period 		: 100ms
//------------------------------------------------------------------------------
void Calculate_HourMeter(void)
{	
	unsigned long temp_HourMeter_Value;
	unsigned long Old_HourMeter_Value;

	if((EngineStartFlag) ||(Flag_Engine_Start && COUNT_FLAG.EngineStart_cnt < 6000) )		// 조건 해지 후 10분간 동작 
	{	
		if(++HourMeter_cnt >= 10)			
		{
			HourMeter_cnt = 0;
			Old_HourMeter_Value = HourMeter_Value/360;

			HourMeter_Value++;
			temp_HourMeter_Value = HourMeter_Value/360;

			if(Old_HourMeter_Value != temp_HourMeter_Value)
				Save_hourmeter();			

			//if(HourMeter_Value > 3599996400) //999,999 hour
			// 99,999 Hour = 359,996,400
			// 					+3599
			// 99,999.9 H   = 359,999,999
			if(HourMeter_Value > 359999999) //99,999.9 hour 		359,996,400
			{
				HourMeter_Value = 0;
				Initialize_Hourmeter();
			}
		}
		Hour_start_flag = 1;
	}
	else Hour_start_flag = 0;

	HCESPN.Hourmeter_1601 = HCESPN.H1601 = HourMeter_Value;
	HCESPN.Hourmeter_Hour_701 = (unsigned short)(HourMeter_Value/3600);
}

//++, 141230 ysm
//------------------------------------------------------------------------------
// Function Name 	: Hourmeter_Save
// Description 	: Hour meter data를  씀.
// Input 			: min(Hourmeter 최소값)
// Return 		: None
// Period 		: Key off 시 한번 사용.
//------------------------------------------------------------------------------
void Hourmeter_Save(unsigned char min)
{
	switch(min)
	{
	case 0: //minHourometer==0
		if(Hourmeter0_Save_Error_Count<4)
		{
			EepromWrite((ADDRESS_HOURMETER0+(Hourmeter0_Save_Error_Count*ADDRESS_METER_COUNT)), &Hourmeter_Data[0], 6);
		}		
		break;
	case 1: //minHourometer==1
		if(Hourmeter1_Save_Error_Count<4)
		{	
			EepromWrite((ADDRESS_HOURMETER1+(Hourmeter1_Save_Error_Count*ADDRESS_METER_COUNT)), &Hourmeter_Data[0], 6);
		}	
		break;
	case 2: //minHourometer==2
		if(Hourmeter2_Save_Error_Count<4)
		{	
			EepromWrite((ADDRESS_HOURMETER2+(Hourmeter2_Save_Error_Count*ADDRESS_METER_COUNT)), &Hourmeter_Data[0], 6);
		}	
		break;
	}

}

void Save_hourmeter(void)
{		

	unsigned char save_count_num;	
	unsigned char Save_Check_flag;

	unsigned char hourmeter_address;

	unsigned short FCS;

	unsigned long HourMeter_check_Value0;

	save_count_num = 0;	
	Save_Check_flag = 1;

	hourmeter_address = MinHourmeter;

	while(Save_Check_flag)
	{	

		HourMeter_check_Value0= 0;

		HourMeter_check_Value0 = HourMeter_Value;

		Hourmeter_Data[0]=HourMeter_check_Value0;
		Hourmeter_Data[1]=HourMeter_check_Value0>>8;
		Hourmeter_Data[2]=HourMeter_check_Value0>>16;
		Hourmeter_Data[3]=HourMeter_check_Value0>>24;

		FCS = MakeCrc16(&Hourmeter_Data[0], 4);

		Hourmeter_Data[4] = FCS & 0xff;
		Hourmeter_Data[5] = (FCS >> 8) & 0xff;

		Check_Hourmeter_ErrorCount(hourmeter_address);

		Hourmeter_Save(hourmeter_address);	

		Check_Hourmeter_Savedata(hourmeter_address);

		HourMeter_check_Value0 = Hourmeter_Data[0];
		HourMeter_check_Value0 += Hourmeter_Data[1]<<8;
		HourMeter_check_Value0 += Hourmeter_Data[2]<<16;
		HourMeter_check_Value0 += Hourmeter_Data[3]<<24;

		if(HourMeter_Value == HourMeter_check_Value0)	 Save_Check_flag = 0;
		else								Save_Check_flag = 1;

		save_count_num++;
		if(save_count_num>3)
		{

			Hourmeter_ErrorCount_Save(hourmeter_address);

			if(hourmeter_address == MinHourmeter)	
				hourmeter_address = MidHourmeter;
			else if(hourmeter_address == MidHourmeter)
				hourmeter_address = MaxHourmeter;	

			else if(hourmeter_address == MaxHourmeter)
			{
				if(Hourmeter2_Save_Error_Count > 3)
					Save_Check_flag = 0;
				else
					Save_Check_flag = 1;
			}	
			save_count_num = 0;
		}

	}
}

void Check_Hourmeter_Savedata(unsigned char min)
{
	switch(min)
	{
	case 0 :	
		EepromRead(ADDRESS_HOURMETER0+(ADDRESS_METER_COUNT*Hourmeter0_Save_Error_Count), (unsigned char *)(&Hourmeter_Data[0]), 6);
		break;
	case 1 :	
		EepromRead(ADDRESS_HOURMETER1+(ADDRESS_METER_COUNT*Hourmeter1_Save_Error_Count), (unsigned char *)(&Hourmeter_Data[0]), 6);
		break;
	case 2 :	
		EepromRead(ADDRESS_HOURMETER2+(ADDRESS_METER_COUNT*Hourmeter2_Save_Error_Count), (unsigned char *)(&Hourmeter_Data[0]), 6);
		break;
	}

}

void Check_Hourmeter_Address(void)
{
	unsigned char tempchar[2];

	EepromRead(ADDRESS_HOURMETER0_ERROR_COUNT, (unsigned char *)(&tempchar[0]), 1);
	Hourmeter0_Save_Error_Count = tempchar[0];
	if(Hourmeter0_Save_Error_Count > 3)
		Hourmeter0_Save_Error_Count = 0;

	EepromRead(ADDRESS_HOURMETER1_ERROR_COUNT, (unsigned char *)(&tempchar[0]), 1);
	Hourmeter1_Save_Error_Count = tempchar[0];
	if(Hourmeter1_Save_Error_Count > 3)
		Hourmeter1_Save_Error_Count = 0;

	EepromRead(ADDRESS_HOURMETER2_ERROR_COUNT, (unsigned char *)(&tempchar[0]), 1);
	Hourmeter2_Save_Error_Count = tempchar[0];
	if(Hourmeter2_Save_Error_Count > 3)
		Hourmeter2_Save_Error_Count = 0;
}

void Hourmeter_ErrorCount_Save(unsigned char min)
{
	unsigned char tempchar[1];

	switch(min)
	{
	case 0 :
		Hourmeter0_Save_Error_Count++;			
		if(Hourmeter0_Save_Error_Count > 3)
			Hourmeter0_Save_Error_Count = 0;
		tempchar[0] = Hourmeter0_Save_Error_Count;
		EepromWrite(ADDRESS_HOURMETER0_ERROR_COUNT,(unsigned char*)(&tempchar), 1);

		break;
	case 1 :
		Hourmeter1_Save_Error_Count++;
		if(Hourmeter1_Save_Error_Count > 3)
			Hourmeter1_Save_Error_Count = 0;
		tempchar[0]  = Hourmeter1_Save_Error_Count;
		EepromWrite(ADDRESS_HOURMETER1_ERROR_COUNT,(unsigned char*)(&tempchar), 1);
		break;
	case 2 :	
		Hourmeter2_Save_Error_Count++;
		if(Hourmeter2_Save_Error_Count > 3)
			Hourmeter2_Save_Error_Count = 0;
		tempchar[0]  = Hourmeter2_Save_Error_Count;
		EepromWrite(ADDRESS_HOURMETER2_ERROR_COUNT,(unsigned char*)(&tempchar), 1);
		break;
	}

}
void Check_Hourmeter_ErrorCount(unsigned char min)
{
	unsigned char hourmeter_save_count_temp[1];

	switch(min)
	{	
	case 0 :	
		EepromRead(ADDRESS_HOURMETER0_ERROR_COUNT, (unsigned char *)(&hourmeter_save_count_temp[0]), 1);

		if(Hourmeter0_Save_Error_Count != hourmeter_save_count_temp[0])
		{
			Hourmeter0_Save_Error_Count = 0;

		}			
		break;
	case 1 :

		EepromRead(ADDRESS_HOURMETER1_ERROR_COUNT, (unsigned char *)(&hourmeter_save_count_temp[0]), 1);

		if(Hourmeter1_Save_Error_Count != hourmeter_save_count_temp[0])
		{
			Hourmeter1_Save_Error_Count = 0;

		}		

		break;
	case 2 :	

		EepromRead(ADDRESS_HOURMETER2_ERROR_COUNT, (unsigned char *)(&hourmeter_save_count_temp[0]), 1);

		if(Hourmeter2_Save_Error_Count != hourmeter_save_count_temp[0])
		{
			Hourmeter2_Save_Error_Count = 0;

		}

		break;
	}
}
