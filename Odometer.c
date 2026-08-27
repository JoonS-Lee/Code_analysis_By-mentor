#include "FLV_Cluster_APP.h"

unsigned char Odo_start_flag;
unsigned int Odometer_Value;
unsigned int Odometer_Value_cm;


unsigned short Calculate_Speed_Value_100ms[10];
extern float Calculate_Speed_Value;

unsigned char MinOdometer;
unsigned char MidOdometer;
unsigned char MaxOdometer;

unsigned char Odometer_Value_temp;
unsigned char odometer_temp;

unsigned char Odometer_Save_Error_Count[6];

unsigned char Odometer_Data[6];
unsigned int Odometer_Comp_Value[3];
unsigned int Odometer_Comp_Value_cm[3];

unsigned long long Odometer_Value_comp[3];

extern unsigned char EngineStartFlag;

void Initialize_Odometer_Variables(void)
{
	unsigned char i;
	unsigned short FRC_Right;

	Odo_start_flag 		= 0;
	
	Odometer_Value		= 0;
	Odometer_Value_cm 	= 0;
	
	
	for (i = 0; i < 10; i++) 		Calculate_Speed_Value_100ms[i] 		= 0;

	Check_Odometer_Address();
	
	for(i=0 ; i<3 ; i++)
	{
		FRC_Right = Odometer_Read(i,Odometer_Save_Error_Count[i]);//km
		if(FRC_Right)
		{
			Odometer_Comp_Value[i] = 	(Odometer_Data[0] << (8*0)) + (Odometer_Data[1] << (8*1)) 
								+ (Odometer_Data[2] << (8*2)) + (Odometer_Data[3] << (8*3));

			if(Odometer_Comp_Value[i] == 0xffffffff)
			{
				Odometer_Comp_Value[i]=0;
			}
		}
		else		Odometer_Comp_Value[i]=0;
	}

	for(i=0 ; i<3 ; i++)
	{
		FRC_Right = Odometer_Read_cm(i,Odometer_Save_Error_Count[i+3]);//Cm
		if(FRC_Right)
		{
			Odometer_Comp_Value_cm[i] = 	(Odometer_Data[0] << (8*0)) + (Odometer_Data[1] << (8*1)) 
								+ (Odometer_Data[2] << (8*2)) + (Odometer_Data[3] << (8*3));

			if(Odometer_Comp_Value_cm[i] == 0xffffffff)
			{
				Odometer_Comp_Value_cm[i]=0;
			}
		}
		else		Odometer_Comp_Value_cm[i]=0;

		Odometer_Value_comp[i]= (Odometer_Comp_Value[i]*0x100000000)+Odometer_Comp_Value_cm[i];
	}
	

	if((Odometer_Value_comp[2]>=Odometer_Value_comp[1]) &&(Odometer_Value_comp[2]>=Odometer_Value_comp[0]))
	{
		Odometer_Value = Odometer_Comp_Value[2];
		Odometer_Value_cm = Odometer_Comp_Value_cm[2];
		MaxOdometer = 2;
		
		if( (Odometer_Value_comp[1]>= Odometer_Value_comp[0]))
		{
			MinOdometer = 0;
			MidOdometer = 1;
		}
		else
		{
			MinOdometer = 1;
			MidOdometer = 0;
		}
	}

	else if((Odometer_Value_comp[1]>=Odometer_Value_comp[0]) &&(Odometer_Value_comp[1]>=Odometer_Value_comp[2]))
	{
		Odometer_Value = Odometer_Comp_Value[1];
		Odometer_Value_cm = Odometer_Comp_Value_cm[1];
		MaxOdometer = 1;
		
		if( (Odometer_Value_comp[0]>= Odometer_Value_comp[2]))	
		{
			MinOdometer = 2;
			MidOdometer = 0;
		}
		else	
		{
			MinOdometer = 0;
			MidOdometer = 2;
		}
	}

	else if((Odometer_Value_comp[0]>=Odometer_Value_comp[1]) &&(Odometer_Value_comp[0]>=Odometer_Value_comp[2]))
	{
		Odometer_Value = Odometer_Comp_Value[0];
		Odometer_Value_cm = Odometer_Comp_Value_cm[0];
		MaxOdometer = 0;
		
		if( (Odometer_Value_comp[1]>= Odometer_Value_comp[2]))	
		{
			MinOdometer = 2;
			MidOdometer = 1;
		}			
		else
		{
			MinOdometer = 1;
			MidOdometer = 2;
		}
	}
}	

void Initialize_Odometer(void)
{
	for(int i=0;i<4;i++)
	{
		EepromWrite(ADDRESS_ODOMETER_KM0+(i*ADDRESS_METER_COUNT), 0x00, 6);
		EepromWrite(ADDRESS_ODOMETER_KM1+(i*ADDRESS_METER_COUNT), 0x00, 6);
		EepromWrite(ADDRESS_ODOMETER_KM2+(i*ADDRESS_METER_COUNT), 0x00, 6);
		
		EepromWrite(ADDRESS_ODOMETER_CM0+(i*ADDRESS_METER_COUNT), 0x00, 6);
		EepromWrite(ADDRESS_ODOMETER_CM1+(i*ADDRESS_METER_COUNT), 0x00, 6);
		EepromWrite(ADDRESS_ODOMETER_CM2+(i*ADDRESS_METER_COUNT), 0x00, 6);
	}

	TimeDelay_msec(5);

	EepromWrite(ADDRESS_ODOMETER0_ERROR_COUNT, 0x00, 1);
	EepromWrite(ADDRESS_ODOMETER1_ERROR_COUNT, 0x00, 1);
	EepromWrite(ADDRESS_ODOMETER2_ERROR_COUNT, 0x00, 1);

	EepromWrite(ADDRESS_ODOMETER0_CM_ERROR_COUNT, 0x00, 1);
	EepromWrite(ADDRESS_ODOMETER1_CM_ERROR_COUNT, 0x00, 1);
	EepromWrite(ADDRESS_ODOMETER2_CM_ERROR_COUNT, 0x00, 1);

	TimeDelay_msec(5);
	
}

unsigned short Odometer_Read(unsigned char read_num, unsigned char err_count)
{
	unsigned int i;
	unsigned short Right_FRC;
	unsigned char tempchar[6];
	
	if(err_count < 4)
	{	
		EepromRead(ADDRESS_ODOMETER_KM0+(read_num*ADDRESS_METER_TOTAL)+(ADDRESS_METER_COUNT*err_count), (unsigned char *)(&tempchar[0]), 6);

		for( i = 0; i < 6; i++)
		{
			Odometer_Data[i] = tempchar[i];
		}
	}
	else
	{
		for( i = 0; i < 6; i++)
		{
			Odometer_Data[i] = 0;			
			
		}
	}

	Right_FRC= MakeCrc16(&Odometer_Data[0], 4);
	
	if( (Odometer_Data[4] == (Right_FRC& 0xff)) && (Odometer_Data[5] == ((Right_FRC>> 8) & 0xff)) )
	{
		Right_FRC=1;
	}		
	else		Right_FRC=0;
	
	return Right_FRC;	
}

//------------------------------------------------------------------------------
// Function Name 	: Odometer_Read_cm
// Description 	: Odo meter data(cm)를 읽음.
// Input 			: read_num(저장 넘버)
// Return 		: 저장된 Odo meter 값을 읽음.
// Period 		: 초기화시 한번 사용.
//------------------------------------------------------------------------------
unsigned short Odometer_Read_cm(unsigned char read_num, unsigned char err_count)
{
	unsigned int i;
	unsigned short Right_FRC;
	unsigned char tempchar[6];
	
	if(err_count < 4)
	{	
		EepromRead(ADDRESS_ODOMETER_CM0+(read_num*ADDRESS_METER_TOTAL)+(ADDRESS_METER_COUNT*err_count), (unsigned char *)(&tempchar[0]), 6);

		for( i = 0; i < 6; i++)
		{
			Odometer_Data[i] = tempchar[i];
		}
	}
	else
	{
		for( i = 0; i < 6; i++)
		{
			Odometer_Data[i] = 0;			
			
		}
	}

	Right_FRC= MakeCrc16(&Odometer_Data[0], 4);
	
	if( (Odometer_Data[4] == (Right_FRC& 0xff)) && (Odometer_Data[5] == ((Right_FRC>> 8) & 0xff)) )
	{
		Right_FRC=1;
	}		
	else		Right_FRC=0;
	
	return Right_FRC;		
}

void Calculate_Odometer(void)
{
	unsigned char i;
	unsigned int Speed_ave_Value_100ms;
	unsigned int Speed_ave_Value_100ms_temp;

	if(EngineStartFlag) 
	{
		for (i = 9;  i !=0; i--)		
			Calculate_Speed_Value_100ms[i] = Calculate_Speed_Value_100ms[i -1];

		Calculate_Speed_Value_100ms[0] = (unsigned short)(Calculate_Speed_Value);

		Speed_ave_Value_100ms_temp = 0;
	        
		for (i = 0; i < 10; i++) 
		{
			Speed_ave_Value_100ms_temp += Calculate_Speed_Value_100ms[i];
		}

		Speed_ave_Value_100ms = Speed_ave_Value_100ms_temp/10;
		
		Odometer_Value_cm += ((Speed_ave_Value_100ms*10000)/36000);

		odometer_temp = Odometer_Value_cm/50000;// 50000 : 500m
		
		if(Odometer_Value_temp != odometer_temp)
		{
			if(odometer_temp==1 || odometer_temp==2)
				Save_odometer();
		}

		Odometer_Value_temp=odometer_temp;
		
		if(Odometer_Value_cm /100000)
		{ 
			Odometer_Value += (Odometer_Value_cm /100000);
			Odometer_Value_cm = (Odometer_Value_cm%100000);
		}

		if ( Odometer_Value >= 1000000 )
		{
			Odometer_Value = 0; // 999,999 odo
			Initialize_Odometer();
		}

		
		if(Calculate_Speed_Value > 0)		Odo_start_flag = 1;
		else                                    Odo_start_flag = 0;
	}
	
}

//++, 150106 ysm
//------------------------------------------------------------------------------
// Function Name 	: Odometer_Save
// Description 	: Odo meter data를  씀.
// Input 			: min(Odometer 최소값), unit(Km or Cm)
// Return 		: None
// Period 		: Key off 시 한번 사용.
//------------------------------------------------------------------------------
void OdoMeter_Save(unsigned char min,unsigned char unit)
{
	switch(unit)
	{
		case 0 : //km save
			switch(min)
			{					
				case 0: //minodometer==0
					if(Odometer_Save_Error_Count[0]<4)
					{
						EepromWrite((ADDRESS_ODOMETER_KM0+(Odometer_Save_Error_Count[0]*ADDRESS_METER_COUNT)), &Odometer_Data[0], 6);
					}		
					break;
				case 1: //minodometer==0
					if(Odometer_Save_Error_Count[1]<4)
					{
						EepromWrite((ADDRESS_ODOMETER_KM1+(Odometer_Save_Error_Count[1]*ADDRESS_METER_COUNT)), &Odometer_Data[0], 6);
					}		
					break;
				case 2: //minodometer==0
					if(Odometer_Save_Error_Count[2]<4)
					{
						EepromWrite((ADDRESS_ODOMETER_KM2+(Odometer_Save_Error_Count[2]*ADDRESS_METER_COUNT)), &Odometer_Data[0], 6);
					}		
					break;	
				default :
					break;
			}				
			break;
		case 1 : //cm save
			switch(min)
			{					
				case 0: //minodometer==0
					if(Odometer_Save_Error_Count[3]<4)
					{
						EepromWrite((ADDRESS_ODOMETER_CM0+(Odometer_Save_Error_Count[3]*ADDRESS_METER_COUNT)), &Odometer_Data[0], 6);
					}		
					break;
				case 1: //minodometer==0
					if(Odometer_Save_Error_Count[4]<4)
					{
						EepromWrite((ADDRESS_ODOMETER_CM1+(Odometer_Save_Error_Count[4]*ADDRESS_METER_COUNT)), &Odometer_Data[0], 6);
					}		
					break;
				case 2: //minodometer==0
					if(Odometer_Save_Error_Count[5]<4)
					{
						EepromWrite((ADDRESS_ODOMETER_CM2+(Odometer_Save_Error_Count[5]*ADDRESS_METER_COUNT)), &Odometer_Data[0], 6);
					}		
					break;	
				default :
					break;
			}				
			break;
		default:
				break;
	}

}

//------------------------------------------------------------------------------
// Function Name 	: Save_odometer
// Description 	: Odo meter data를  씀.
// Input 			: None
// Return 		: None
// Period 		: 500m 마다 한번
//------------------------------------------------------------------------------
void Save_odometer(void)
{
	unsigned char save_count_Km_num;
	unsigned char save_count_Cm_num;
	unsigned char Save_Check_flag;

	unsigned char odometer_Km_address;
	unsigned char odometer_Cm_address;
	unsigned short FCS;

	unsigned long OdoMeter_check_Value[2];
	unsigned long OdoMeter_check_Value_cm[2];

	unsigned char save_Km_Ok,save_Cm_Ok;

	save_count_Km_num = save_count_Cm_num = 0;	
	save_Km_Ok =save_Cm_Ok=0;
	Save_Check_flag = 1;

	odometer_Km_address =odometer_Cm_address  = MinOdometer;
	
	while(Save_Check_flag)
	{	
		if(save_Km_Ok !=1)
		{
			OdoMeter_check_Value[0] = 0;
			OdoMeter_check_Value[0]= Odometer_Value;
			OdoMeter_check_Value[1]= Odometer_Value;

			Odometer_Data[0]=OdoMeter_check_Value[0];
			Odometer_Data[1]=OdoMeter_check_Value[0] >>8;
			Odometer_Data[2]=OdoMeter_check_Value[0] >>16;
			Odometer_Data[3]=OdoMeter_check_Value[0] >>24;

			FCS = MakeCrc16(&Odometer_Data[0], 4);

			Odometer_Data[4] = FCS & 0xff;
			Odometer_Data[5] = (FCS >> 8) & 0xff;

			Check_Odometer_ErrorCount(odometer_Km_address,0);
			
			OdoMeter_Save(odometer_Km_address,0);

			Check_Odometer_Savedata(odometer_Km_address,0);
			OdoMeter_check_Value[1] = Odometer_Data[0] + (Odometer_Data[1]<< (8*1)) + (Odometer_Data[2]<< (8*2)) + (Odometer_Data[3]<< (8*3));
			if(OdoMeter_check_Value[1] == OdoMeter_check_Value[0]) 		save_Km_Ok=1;
			else		save_Km_Ok=0;
		}

		if(save_Cm_Ok != 1)
		{
			OdoMeter_check_Value_cm[0] = 0;
			OdoMeter_check_Value_cm[0]= Odometer_Value_cm;
			OdoMeter_check_Value_cm[1]= Odometer_Value_cm;

			Odometer_Data[0]=OdoMeter_check_Value_cm[0];
			Odometer_Data[1]=OdoMeter_check_Value_cm[0] >>8;
			Odometer_Data[2]=OdoMeter_check_Value_cm[0] >>16;
			Odometer_Data[3]=OdoMeter_check_Value_cm[0] >>24;

			FCS = MakeCrc16(&Odometer_Data[0], 4);

			Odometer_Data[4] = FCS & 0xff;
			Odometer_Data[5] = (FCS >> 8) & 0xff;

			Check_Odometer_ErrorCount(odometer_Cm_address,1);
			
			OdoMeter_Save(odometer_Cm_address,1);

			Check_Odometer_Savedata(odometer_Cm_address,1);
			OdoMeter_check_Value_cm[1] = Odometer_Data[0] + (Odometer_Data[1]<< (8*1)) + (Odometer_Data[2]<< (8*2)) + (Odometer_Data[3]<< (8*3));
			if(OdoMeter_check_Value_cm[1] == OdoMeter_check_Value_cm[0]) 		save_Cm_Ok=1;
			else		save_Cm_Ok=0;
		}
		
		if((save_Km_Ok == 1) && (save_Cm_Ok == 1))		
		{
			Save_Check_flag = 0;
		}
		else	
		{
			Save_Check_flag = 1;
			if(save_Km_Ok==0)	save_count_Km_num++;
			if(save_Cm_Ok==0)	save_count_Cm_num++;
		}
		

		if(save_count_Km_num>3)
		{
			Odometer_ErrorCount_Save(odometer_Km_address,0);
		
			if(odometer_Km_address == MinOdometer)				
				odometer_Km_address = MidOdometer;

			else if(odometer_Km_address == MidOdometer)
				odometer_Km_address = MaxOdometer;	

			else if(odometer_Km_address == MaxOdometer)
			{				
				if(Odometer_Save_Error_Count[2] < 4)
				{
					Save_Check_flag = 1;
				}
				else
					Save_Check_flag = 0;
			}
			save_count_Km_num = 0;
		}

		if(save_count_Cm_num>3)
		{
			Odometer_ErrorCount_Save(odometer_Cm_address,1);
		
			if(odometer_Cm_address == MinOdometer)				
				odometer_Cm_address = MidOdometer;

			else if(odometer_Cm_address == MidOdometer)
				odometer_Cm_address = MaxOdometer;	

			else if(odometer_Cm_address == MaxOdometer)
			{				
				if(Odometer_Save_Error_Count[5] < 4)
				{
					Save_Check_flag = 1;
				}
				else
					Save_Check_flag = 0;
			}
			save_count_Cm_num = 0;
		}
	}

}




//------------------------------------------------------------------------------
// Function Name 	: Check_Odometer_Savedata
// Description 	: EEPROM에 저장된 Odo meter data를 읽음.
// Input 			: min(Odometer 최소값), unit(Km or Cm)
// Return 		: 저장된 Odo meter 값을 읽음.
// Period 		: Power Off시 한번 사용.
//------------------------------------------------------------------------------
void Check_Odometer_Savedata(unsigned char min,unsigned char unit)
{
	switch(unit)
	{
		case 0 : //km save
			switch(min)
			{
				case 0: //minodometer==0
					EepromRead(ADDRESS_ODOMETER_KM0+(ADDRESS_METER_COUNT*Odometer_Save_Error_Count[0]), (unsigned char *)(&Odometer_Data[0]), 6);
					break;
				case 1: //minodometer==1
					EepromRead(ADDRESS_ODOMETER_KM1+(ADDRESS_METER_COUNT*Odometer_Save_Error_Count[1]), (unsigned char *)(&Odometer_Data[0]), 6);
					break;
				case 2: //minodometer==2
					EepromRead(ADDRESS_ODOMETER_KM2+(ADDRESS_METER_COUNT*Odometer_Save_Error_Count[2]), (unsigned char *)(&Odometer_Data[0]), 6);
					break;
			}
			break;
			
		case 1: //Cm save
			switch(min)
			{
				case 0: //minodometer==0
					EepromRead(ADDRESS_ODOMETER_CM0+(ADDRESS_METER_COUNT*Odometer_Save_Error_Count[3]), (unsigned char *)(&Odometer_Data[0]), 6);
					break;
				case 1: //minodometer==1
					EepromRead(ADDRESS_ODOMETER_CM1+(ADDRESS_METER_COUNT*Odometer_Save_Error_Count[4]), (unsigned char *)(&Odometer_Data[0]), 6);
					break;
				case 2: //minodometer==2
					EepromRead(ADDRESS_ODOMETER_CM2+(ADDRESS_METER_COUNT*Odometer_Save_Error_Count[5]), (unsigned char *)(&Odometer_Data[0]), 6);
					break;
			}
			break;
		default:
				break;
	}
}

void Check_Odometer_Address(void)
{
	unsigned char tempchar[2];
	
	EepromRead(ADDRESS_ODOMETER0_ERROR_COUNT, (unsigned char *)(&tempchar[0]), 1);
	Odometer_Save_Error_Count[0] = tempchar[0];
	if(Odometer_Save_Error_Count[0] > 3)
		Odometer_Save_Error_Count[0] = 0;
	
	EepromRead(ADDRESS_ODOMETER1_ERROR_COUNT, (unsigned char *)(&tempchar[0]), 1);
	Odometer_Save_Error_Count[1] = tempchar[0];
	if(Odometer_Save_Error_Count[1] > 3)
		Odometer_Save_Error_Count[1] = 0;
		
	EepromRead(ADDRESS_ODOMETER2_ERROR_COUNT, (unsigned char *)(&tempchar[0]), 1);
	Odometer_Save_Error_Count[2] = tempchar[0];
	if(Odometer_Save_Error_Count[2] > 3)
		Odometer_Save_Error_Count[2] = 0;

	EepromRead(ADDRESS_ODOMETER0_CM_ERROR_COUNT, (unsigned char *)(&tempchar[0]), 1);
	Odometer_Save_Error_Count[3] = tempchar[0];
	if(Odometer_Save_Error_Count[3] > 3)
		Odometer_Save_Error_Count[3] = 0;
	
	EepromRead(ADDRESS_ODOMETER1_CM_ERROR_COUNT, (unsigned char *)(&tempchar[0]), 1);
	Odometer_Save_Error_Count[4] = tempchar[0];
	if(Odometer_Save_Error_Count[4] > 3)
		Odometer_Save_Error_Count[4] = 0;
		
	EepromRead(ADDRESS_ODOMETER2_CM_ERROR_COUNT, (unsigned char *)(&tempchar[0]), 1);
	Odometer_Save_Error_Count[5] = tempchar[0];
	if(Odometer_Save_Error_Count[5] > 3)
		Odometer_Save_Error_Count[5] = 0;
}

void Odometer_ErrorCount_Save(unsigned char min, unsigned char unit)
{
	unsigned char tempchar[1];

	if(unit==0) // km error
	{
		switch(min)
		{
			case 0 :
					Odometer_Save_Error_Count[0]++;			
					if(Odometer_Save_Error_Count[0] > 3)
						Odometer_Save_Error_Count[0] = 0;
					tempchar[0] = Odometer_Save_Error_Count[0];
					EepromWrite(ADDRESS_ODOMETER0_ERROR_COUNT,(unsigned char*)(&tempchar), 1);
					
					break;
			case 1 :
					Odometer_Save_Error_Count[1]++;
					if(Odometer_Save_Error_Count[1] > 3)
						Odometer_Save_Error_Count[1] = 0;
					tempchar[0]  = Odometer_Save_Error_Count[1];
					EepromWrite(ADDRESS_ODOMETER1_ERROR_COUNT,(unsigned char*)(&tempchar), 1);
					break;
			case 2 :	
					Odometer_Save_Error_Count[2]++;
					if(Odometer_Save_Error_Count[2] > 3)
						Odometer_Save_Error_Count[2] = 0;
					tempchar[0]  = Odometer_Save_Error_Count[2];
					EepromWrite(ADDRESS_ODOMETER2_ERROR_COUNT,(unsigned char*)(&tempchar), 1);
					break;
		}
	}
	else  // cm error
	{
		switch(min)
		{
			case 0 :
					Odometer_Save_Error_Count[3]++;			
					if(Odometer_Save_Error_Count[3] > 3)
						Odometer_Save_Error_Count[3] = 0;
					tempchar[0] = Odometer_Save_Error_Count[3];
					EepromWrite(ADDRESS_ODOMETER0_CM_ERROR_COUNT,(unsigned char*)(&tempchar), 1);
					
					break;
			case 1 :
					Odometer_Save_Error_Count[4]++;
					if(Odometer_Save_Error_Count[4] > 3)
						Odometer_Save_Error_Count[4] = 0;
					tempchar[0]  = Odometer_Save_Error_Count[4];
					EepromWrite(ADDRESS_ODOMETER1_CM_ERROR_COUNT,(unsigned char*)(&tempchar), 1);
					break;
			case 2 :	
					Odometer_Save_Error_Count[5]++;
					if(Odometer_Save_Error_Count[5] > 3)
						Odometer_Save_Error_Count[5] = 0;
					tempchar[0]  = Odometer_Save_Error_Count[5];
					EepromWrite(ADDRESS_ODOMETER2_CM_ERROR_COUNT,(unsigned char*)(&tempchar), 1);
					break;
		}
	}
}

void Check_Odometer_ErrorCount(unsigned char min, unsigned char unit)
{
	unsigned char tempchar[1];

	if(unit==0)
	{
		switch(min)
		{	
			case 0 :	
				EepromRead(ADDRESS_ODOMETER0_ERROR_COUNT, (unsigned char *)(&tempchar[0]), 1);
				if(Odometer_Save_Error_Count[0] != tempchar[0])	Odometer_Save_Error_Count[0] = 0;
				break;
			case 1 :	
				EepromRead(ADDRESS_ODOMETER1_ERROR_COUNT, (unsigned char *)(&tempchar[0]), 1);
				if(Odometer_Save_Error_Count[1] != tempchar[0])	Odometer_Save_Error_Count[2] = 0;
				break;
			case 2 :	
				EepromRead(ADDRESS_ODOMETER2_ERROR_COUNT, (unsigned char *)(&tempchar[0]), 1);
				if(Odometer_Save_Error_Count[1] != tempchar[0])	Odometer_Save_Error_Count[2] = 0;
				break;
		}
	}
	else
	{
		switch(min)
		{	
			case 0 :	
				EepromRead(ADDRESS_ODOMETER0_CM_ERROR_COUNT, (unsigned char *)(&tempchar[0]), 1);
				if(Odometer_Save_Error_Count[3] != tempchar[0])	Odometer_Save_Error_Count[3] = 0;
				break;
			case 1 :	
				EepromRead(ADDRESS_ODOMETER1_CM_ERROR_COUNT, (unsigned char *)(&tempchar[0]), 1);
				if(Odometer_Save_Error_Count[4] != tempchar[0])	Odometer_Save_Error_Count[4] = 0;
				break;
			case 2 :	
				EepromRead(ADDRESS_ODOMETER2_CM_ERROR_COUNT, (unsigned char *)(&tempchar[0]), 1);
				if(Odometer_Save_Error_Count[5] != tempchar[0])	Odometer_Save_Error_Count[5] = 0;
				break;
		}
	}
}
