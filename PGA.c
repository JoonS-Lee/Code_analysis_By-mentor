#include "FLV_Cluster_APP.h"


unsigned short AI_DI_Low_Data0[8];
unsigned short AI_DI_Low_Data1[8];
unsigned short AI_DI_Low_Data2[8];

unsigned short Analog_Input_TimeCount;

unsigned char Fuel_Level_Sum_Count;
unsigned short Fuel_Level_buffer[10];
unsigned short Fuel_Level_rowdata;

unsigned char TM_Oil_Temp_Sum_Count;
unsigned short TM_Oil_Temp_buffer[10];
unsigned short TM_Oil_Temp_rowdata;

unsigned char BAT_level_Sum_Count;
unsigned short BAT_level_buffer[20];

unsigned char ALT_level_Sum_Count;
unsigned short ALT_level_buffer[20];

//++, 210405 ysm, 9S
unsigned char Engine_Error_Check_Count;
unsigned short Engine_Error_Check_buffer[10];
unsigned short Engine_Error_Check_rowdata;

unsigned char WIF_level_Sum_Count;
unsigned short WIF_level_buffer[20];
unsigned short WIF_Level_rowdata;

unsigned char Coolant_Level_Sum_Count;
unsigned short Coolant_Level_buffer[10];
unsigned short Coolant_Temp_rowdata;

float BRAKE_OIL_PRESSURE;
//--, 210405 ysm, 9S


float SENSOR_VOLTAGE;
float TM_OIL_TEMP_SENSOR;
float TM_OIL_PRESSURE;
 
float HYD_OIL_PRESSURE; //++,--, 220829 ysm, 9A

float LOAD_SENSOR;
float HYD_TEMP_SENSOR;
float BATTERY_VOLT;
float ALT_VOLT;
unsigned char ALT_Level_Status;

extern ADC_HandleTypeDef hadc1;

unsigned int  Line1_Value_1, Line1_Total_Value, Line1_Value_2, Line1_Total_Value2;
unsigned int  Line2_Value_1, Line2_Total_Value, Line2_Value_2, Line2_Total_Value2;

FREQUENCY_DATA TACHO;

extern st_AD_DATA	AD_DATA;
extern unsigned int 	SPEED_COUNT;

struct Digital_Input_Type Digital_Input[INDEX_DIGITIAL_INPUT_TOTAL];
unsigned char Flag_DIN[INDEX_DIGITIAL_INPUT_TOTAL];
unsigned char Flag_DIN_MMI[INDEX_DIGITIAL_INPUT_TOTAL];         // ++, --, 210830 ctw Auto JIG Final Test

extern unsigned char PROTO_EQUIPMENT;


// ++, 201017 ctw Load Ƣ? ?? ?ψ?
#define AI_DI_Low_Data2_Load_Sensor_SIZE	120
unsigned short AI_DI_Low_Data2_Load_Sensor[AI_DI_Low_Data2_Load_Sensor_SIZE];
unsigned long AI_DI_Low_Data2_Load_Sensor_Count = 0;
// --, 201017 ctw Load Ƣ? ?? ?ψ?

extern float Calculate_Speed_Value;
extern unsigned char Engine_Type;
extern EEPROM_MODEL_DATA1	InfoModel1;
extern CAN_ECU_DATA_65265 RX_ECU_DATA_65265;
extern unsigned char Signal_TM_Oil_Check_lamp;

extern float Speed_value_temp[2];

extern unsigned char Tab_Number;     // ++, --, 210622 ctw Auto JIG Final Test

//++, 220929 ysm, FSCU
extern CAN_FSCU_STATE1_65525 RX_FSCU_DATA_65525;
//--, 220929 ysm, FSCu

void Initialize_PGA_Variable(void)
{

	for(int i=0;i<INDEX_DIGITIAL_INPUT_TOTAL;i++)
	{
		Flag_DIN[i] = 0;
		if(i<20)
		{
			BAT_level_buffer[i] = 0;
			ALT_level_buffer[i]=0;
		}
		if(i<10)
		{
			Fuel_Level_buffer[i]=0;
			TM_Oil_Temp_buffer[i] = 0;
			Engine_Error_Check_buffer[i] = 0; //++,--, 210409 ysm, 9S
			Coolant_Level_buffer[i] = 0; //++,--, 210421 ysm, 9S
		}
		if(i<8)
		{
			AI_DI_Low_Data0[i] = 0;
			AI_DI_Low_Data1[i] = 0;
			AI_DI_Low_Data2[i] = 0;
		}
	}

	Analog_Input_TimeCount = 0;
	BAT_level_Sum_Count=0;
	Fuel_Level_Sum_Count=0;
	TM_Oil_Temp_Sum_Count = 0;
	
	ALT_level_Sum_Count=0;
	Fuel_Level_rowdata=0;
	TM_Oil_Temp_rowdata = 0;
	//++, 210421 ysm, 9S
	Engine_Error_Check_Count = 0;
	Engine_Error_Check_rowdata = 0;
	Coolant_Temp_rowdata=0;
	Coolant_Level_Sum_Count = 0;
	//--, 210421 ysm, 9S
	SENSOR_VOLTAGE = LOAD_SENSOR = BATTERY_VOLT = ALT_VOLT= HYD_TEMP_SENSOR = 0;
	BRAKE_OIL_PRESSURE = TM_OIL_TEMP_SENSOR = TM_OIL_PRESSURE = HYD_OIL_PRESSURE = 0;

	memset(&TACHO,0,sizeof(TACHO));

	Initialize_Digital_Input_Variable();
	
}

void Check_PGA_Value(unsigned char MUX)
{
	switch(MUX)
	{
	case 0:	S0_Reset;	S1_Reset;	S2_Reset;	break;
	case 1:	S0_Set;		S1_Reset;	S2_Reset;	break;
	case 2:	S0_Reset;	S1_Set;		S2_Reset;	break;
	case 3:	S0_Set;		S1_Set;		S2_Reset;	break;
	case 4:	S0_Reset;	S1_Reset;	S2_Set;		break;
	case 5:	S0_Set;		S1_Reset;	S2_Set;		break;
	case 6:	S0_Reset;	S1_Set;		S2_Set;		break;
	case 7:	S0_Set;		S1_Set;		S2_Set;		break;
	default :	S0_Reset;	S1_Reset;	S2_Reset;	break;

	}
	
	
	/*
	DIN3	BRAKE OIL LEVEL ???	Digital Low Input
	DIN4	AIR FILITER WARNING ???	Digital Low Input
	DIN5	ENG OIL PRESSURE SWITCH	Digital Low Input
	DIN2	SEAT SWITCH ???	Digital Low Input
	DIN8	PARKING PRESSURE SWITCH ???	Digital Low Input
	DIN6	T/M ???? ?μ? ??? ???	Digital Low Input
	DIN9	???? ???	Digital High Input(12V:B+)
	DIN7	BRAKE SWITCH	Digital High Input(12V:B+)
	*/
        
	HAL_ADC_Start(&hadc1);
	HAL_ADC_PollForConversion(&hadc1, 10);	
	AI_DI_Low_Data0[MUX] = HAL_ADC_GetValue(&hadc1); //Digital Input 2 to 9

	/*
	DIN11	???? ???	Digital High Input(12V:B+)
	DIN12	PWD/STD MODE SEL	Digital Low Input
	DIN13	TURN SIGNAL RH	Digital High Input(12V:B+)
	DIN10	??? ???	Digital High Input(12V:B+)
	DIN15	INHIBIT REGENERATION SIG	Digital Low Input
	x
	DIN16	PARKED REGENERATION SIG	Digital Low Input
	DIN14	TURN SIGNAL LH	Digital High Input(12V:B+)
	*/
       
	HAL_ADC_Start(&hadc1);
	HAL_ADC_PollForConversion(&hadc1, 10);  
	AI_DI_Low_Data1[MUX] = HAL_ADC_GetValue(&hadc1); //Digital Input 10 to 17
	
	/*
	AIN3	HYD OIL TEMPERATURE SIG	Analog Input(Voltage : 1~5V)
	battery
	AIN1	ALTERNATOR L ???	Analog Input(Voltage)
	AIN2	FUEL SENDER ???	Analog Input(Resistance)
	DIN18	AUTO TILT SIG	Digital Low Input
	AIN4	LOAD INDICATOR PRESSURE SIG	Analog Input(Voltage 0.5~4.5V)
	DIN19	FUEL WARMER SIG	Digital High Input(12V)
	DIN17	WATER SEPERATOR(WIF) ???	Digital Low Input
	*/
    
	HAL_ADC_Start(&hadc1);
	HAL_ADC_PollForConversion(&hadc1, 10); 
	AI_DI_Low_Data2[MUX] = HAL_ADC_GetValue(&hadc1); //Analog Input 1 to 4	

	HAL_ADC_Stop(&hadc1);

}

void WIF_level_Processing(unsigned short ADData, unsigned char Time_Count)
{
	unsigned int Sum;
	float Average;
	unsigned char i;

	if (WIF_level_Sum_Count < 10)
		WIF_level_Sum_Count++;	

	WIF_level_buffer[Time_Count] = ADData;	

	Sum = 0;

	for (i = 0; i < 10; i++)
		Sum += (unsigned int)WIF_level_buffer[i];

	Average = (float)Sum / (float)WIF_level_Sum_Count;

	WIF_Level_rowdata = (USHORT)Average;
}

void Fuel_level_Processing(unsigned short ADData, unsigned char Time_Count)
{
	unsigned int Sum;
	float Average;
	unsigned char i;

	if (Fuel_Level_Sum_Count < 10)
		Fuel_Level_Sum_Count++;	

	Fuel_Level_buffer[Time_Count] = ADData;	

	Sum = 0;

	for (i = 0; i < 10; i++)
		Sum += (unsigned int)Fuel_Level_buffer[i];

	Average = (float)Sum / (float)Fuel_Level_Sum_Count;

	Fuel_Level_rowdata = (USHORT)Average;
}

//++, 210409 ysm, 9S
void Coolant_level_Processing(unsigned short ADData, unsigned char Time_Count)
{
	unsigned int Sum;
	float Average;
	unsigned char i;

	if (Coolant_Level_Sum_Count < 10)
		Coolant_Level_Sum_Count++;	

	Coolant_Level_buffer[Time_Count] = ADData;	

	Sum = 0;

	for (i = 0; i < 10; i++)
		Sum += (unsigned int)Coolant_Level_buffer[i];

	Average = (float)Sum / (float)Coolant_Level_Sum_Count;

	Coolant_Temp_rowdata = (USHORT)Average;
}


void Engine_Error_Check_Processing(unsigned short ADData, unsigned char Time_Count)
{
	unsigned int Sum;
	float Average;
	unsigned char i;

	if (Engine_Error_Check_Count < 10)
		Engine_Error_Check_Count++;	

	Engine_Error_Check_buffer[Time_Count] = ADData;	

	Sum = 0;

	for (i = 0; i < 10; i++)
		Sum += (unsigned int)Engine_Error_Check_buffer[i];

	Average = (float)Sum / (float)Engine_Error_Check_Count;

	Engine_Error_Check_rowdata = (USHORT)Average;


}
//--, 210409 ysm, 9S


void TM_Oil_Temp_Processing(unsigned short ADData, unsigned char Time_Count)
{
	unsigned int Sum;
	float Average;
	unsigned char i;

	if (TM_Oil_Temp_Sum_Count < 10)
		TM_Oil_Temp_Sum_Count++;	

	TM_Oil_Temp_buffer[Time_Count] = ADData;	

	Sum = 0;

	for (i = 0; i < 10; i++)
		Sum += (unsigned int)TM_Oil_Temp_buffer[i];

	Average = (float)Sum / (float)TM_Oil_Temp_Sum_Count;

	TM_Oil_Temp_rowdata = (USHORT)Average;
}



void BAT_level_Processing(unsigned short ADData, unsigned char Time_Count)
{
	unsigned int Sum;
	float Average;
	unsigned char i;

	float conversion_facter_12v;

	#if 0
	if((InfoModel1.ModelInfo >= MODEL_35D_9S)&&(InfoModel1.ModelInfo <= MODEL_50D_9S))
		conversion_facter_12v = 0.008864; //24V SYSTEM
	else
		conversion_facter_12v = 0.006429; //12V SYSTEM
	#else
	conversion_facter_12v = 0.008864; //24V SYSTEM
	#endif
	
	if (BAT_level_Sum_Count < 20)
		BAT_level_Sum_Count++;	
	
	BAT_level_buffer[Time_Count] = ADData;	

	Sum = 0;

	for (i = 0; i < 20; i++)
		Sum += (unsigned int)BAT_level_buffer[i];

	Average = (float)Sum / (float)BAT_level_Sum_Count;

	BATTERY_VOLT = Average * conversion_facter_12v; 
	AD_DATA.BATTERY_VOLT = BATTERY_VOLT;
}

void ALT_level_Processing(unsigned short ADData, unsigned char Time_Count)
{
	unsigned int Sum;
	float Average, alt_temp;
	unsigned char i;
	float conversion_facter_12v;
		
	static unsigned char ALT_low_Count=0;

	#if 0
	if((InfoModel1.ModelInfo >= MODEL_35D_9S)&&(InfoModel1.ModelInfo <= MODEL_50D_9S))
		conversion_facter_12v = 0.008864; //24V SYSTEM
	else
		conversion_facter_12v = 0.006429; //12V SYSTEM
	#else
	conversion_facter_12v = 0.008864; //24V SYSTEM
	#endif

	//++, 210305 ysm, BAT_LOW
	#if 0
	if(Engine_Type == DIESEL_TYPE)
		alt_temp = 10.0;
	else
		alt_temp = 1.0;
	#else
	if((InfoModel1.ModelInfo >= MODEL_35L_9)&&(InfoModel1.ModelInfo <= MODEL_50L_9))
		alt_temp = 1.0;
	else if((InfoModel1.ModelInfo >= MODEL_35D_9S)&&(InfoModel1.ModelInfo <= MODEL_50D_9S))
		alt_temp = 25.5;
	else
		alt_temp = 10.0;
	#endif
	//--, 210305 ysm, BAT_LOW	
	
	if (ALT_level_Sum_Count < 20)
		ALT_level_Sum_Count++;	
	
	ALT_level_buffer[Time_Count] = ADData;	

	Sum = 0;

	for (i = 0; i < 20; i++)
		Sum += (unsigned int)ALT_level_buffer[i];

	Average = (float)Sum / (float)ALT_level_Sum_Count;

	ALT_VOLT = (Average * conversion_facter_12v); 


	//++, 211206 ysm
	#if 0
	if(ALT_VOLT<alt_temp)  // LPG 10.0 -> 0.5, 201016 ysm, LPG   
	{
		if(++ALT_low_Count>50)
		{
			ALT_low_Count=100;
			ALT_Level_Status=0;	
		}
	}
	else
	{
		ALT_Level_Status=1;
		ALT_low_Count=0;
	}
	#else
	if(ALT_level_Sum_Count >= 20)
	{
		if(ALT_VOLT<alt_temp)  // LPG 10.0 -> 0.5, 201016 ysm, LPG   
		{
			if(++ALT_low_Count>50)
			{
				ALT_low_Count=100;
				ALT_Level_Status=0;	
			}
		}
		else
		{
			ALT_Level_Status=1;
			ALT_low_Count=0;
		}

	}
	else
	{
		ALT_Level_Status=0;
	}
	#endif
	//--, 211206 ysm
	AD_DATA.ALT_VOLT = ALT_VOLT;
}


void Run_ADC(void)
{
	unsigned char i;
	//float conversion_facter_5v = 0.006445;
	float conversion_facter_5v = 0.001611;
	float conversion_facter_Din = 0.006446;

	if (++Analog_Input_TimeCount >= 1000)
	{
		Analog_Input_TimeCount = 0;
	}


	for(i=0; i<8; i++)
	{
		Check_PGA_Value(i);
	}

	// DIN On_Off check!!
	for (i = 0; i < INDEX_DIGITIAL_INPUT_TOTAL; i++)
	{
		if(PROTO_EQUIPMENT == 1)
		{
			if((i == INDEX_NO_USE_DIN0) || (i == INDEX_NO_USE_DIN6) || ((i >= INDEX_NO_USE_DIN2) && (i <= INDEX_NO_USE_DIN5)))
				continue;
		}
		else
		{
			if((i == INDEX_NO_USE_DIN0) || (i == INDEX_NO_USE_DIN1) || (i == INDEX_NO_USE_DIN6) || ((i >= INDEX_NO_USE_DIN2) && (i <= INDEX_NO_USE_DIN5)))
				continue;
		}
		
		if(i<8)
		{
			// ++, 210622 ctw Auto JIG _ DIN Brake Switch
			if(Digital_Input[i].Parameter.Active_Value == 0)        
            {
                  if((i == 7)&&(Tab_Number == 2))
                  {
                    Set_Digital_Input(&Digital_Input[7], ((AI_DI_Low_Data0[7] < 1000) ? 1 : 0));
                  }
                  else
                  {
					Set_Digital_Input(&Digital_Input[i], ((AI_DI_Low_Data0[i] > 200) ? 1 : 0));   
                  }
            }
			// --, 210622 ctw Auto JIG _ DIN Brake Switch
                          
			else
			{
				if((InfoModel1.ModelInfo >= MODEL_35D_9S)&&(InfoModel1.ModelInfo <= MODEL_50D_9S))
					Set_Digital_Input(&Digital_Input[i], ((AI_DI_Low_Data0[i] > 2000) ? 1 : 0)); //9S : 2000, base :1000   
				else
					Set_Digital_Input(&Digital_Input[i], ((AI_DI_Low_Data0[i] > 1000) ? 1 : 0)); //9S : 2000, base :1000   
			}
		}
		else if(i<16)
		{
			if(Digital_Input[i].Parameter.Active_Value == 0)
				Set_Digital_Input(&Digital_Input[i], ((AI_DI_Low_Data1[i-8] > 200) ? 1 : 0));   
			else
			{
				if((InfoModel1.ModelInfo >= MODEL_35D_9S)&&(InfoModel1.ModelInfo <= MODEL_50D_9S))
					Set_Digital_Input(&Digital_Input[i], ((AI_DI_Low_Data1[i-8] > 2000) ? 1 : 0));
				else
				{
					if((i == INDEX_TURN_SIGNAL_LH)||(i == INDEX_TURN_SIGNAL_RH))
						Set_Digital_Input(&Digital_Input[i], ((AI_DI_Low_Data1[i-8] > 1242) ? 1 : 0));
					else
						Set_Digital_Input(&Digital_Input[i], ((AI_DI_Low_Data1[i-8] > 1000) ? 1 : 0));
				}										
				
			}
		}
		else 
		{
			if(Digital_Input[i].Parameter.Active_Value == 0)
				Set_Digital_Input(&Digital_Input[i], ((AI_DI_Low_Data2[i-16] > 200) ? 1 : 0));   
			else
			{
				if((InfoModel1.ModelInfo >= MODEL_35D_9S)&&(InfoModel1.ModelInfo <= MODEL_50D_9S))
					Set_Digital_Input(&Digital_Input[i], ((AI_DI_Low_Data2[i-16] > 2000) ? 1 : 0));  
				else
					Set_Digital_Input(&Digital_Input[i], ((AI_DI_Low_Data2[i-16] > 1000) ? 1 : 0));  

				 
			}
		}
		Flag_DIN[i] = Digital_Input[i].Data.Value;

		//++, 210528 BRAKE SW ß??
		//ACTIVE HIGH//9S : 24V, ?׿ܠ12V

		//++, 210617 ysm, 9S
		#if 0
		if((InfoModel1.ModelInfo >= MODEL_35D_9S)&&(InfoModel1.ModelInfo <= MODEL_33LC_9))
		{
			if(InfoModel1.ModelInfo <= MODEL_50D_9S) //9S
			{
				if(AI_DI_Low_Data1[6] >= 2000) 
					Flag_DIN[INDEX_BRAKE_SW_SIG] = 1;
				else
					Flag_DIN[INDEX_BRAKE_SW_SIG] = 0;
			}
			else
			{
				if(AI_DI_Low_Data1[6] >= 1000)
					Flag_DIN[INDEX_BRAKE_SW_SIG] = 1;
				else
					Flag_DIN[INDEX_BRAKE_SW_SIG] = 0;

			}
		
		}		
		#else
		if(((InfoModel1.ModelInfo >= MODEL_35L_9)&&(InfoModel1.ModelInfo <= MODEL_33LC_9))
			||((InfoModel1.ModelInfo >= MODEL_25L_9A)&&(InfoModel1.ModelInfo <= MODEL_35LN_9A))
			||((InfoModel1.ModelInfo >= MODEL_25D_9HDI)&&(InfoModel1.ModelInfo <= MODEL_50DN_9HDI)))
		{
			// ++, 210830 ctw Auto JIG Final Test
			if(AI_DI_Low_Data1[6] >= 530)
			      Flag_DIN_MMI[INDEX_BRAKE_SW_SIG] = 0;
			else
			      Flag_DIN_MMI[INDEX_BRAKE_SW_SIG] = 1;
			// --, 210830 ctw Auto JIG Final Test

			if(AI_DI_Low_Data1[6] >= 1000)
			      Flag_DIN[INDEX_BRAKE_SW_SIG] = 1;
			else
			      Flag_DIN[INDEX_BRAKE_SW_SIG] = 0;
		}
		#endif
		//--, 210617 ysm, 9S
		//--, 210528 ysm
		#if 0
		if((InfoModel1.ModelInfo >= MODEL_25LC_9)&&(InfoModel1.ModelInfo <= MODEL_33LC_9))
		{
			if(AI_DI_Low_Data1[5] > 200)
				Signal_TM_Oil_Check_lamp = 0;
			else
				Signal_TM_Oil_Check_lamp = 1;
		}
		#endif

		
	}

	if((Analog_Input_TimeCount % 100) == 0)
	{
//	if((Analog_Input_TimeCount % 10) == 0)
		Fuel_level_Processing(AI_DI_Low_Data2[3], (unsigned char)((Analog_Input_TimeCount % 1000) / 100));

		//++, 210421 ysm, 9S
		if((InfoModel1.ModelInfo >= MODEL_35D_9S)&&(InfoModel1.ModelInfo <= MODEL_50D_9S))
		{
			Coolant_level_Processing(AI_DI_Low_Data2[0], (unsigned char)((Analog_Input_TimeCount % 1000) / 100));			

			//++, 210617 ysm, 9S
			#if 0
			WIF_level_Processing(AI_DI_Low_Data0[7],  (unsigned char)((Analog_Input_TimeCount % 1000) / 100));
			#else
			WIF_level_Processing(AI_DI_Low_Data1[6],  (unsigned char)((Analog_Input_TimeCount % 1000) / 100));	
			#endif
			//--, 210617 ysm, 9S

		}
		//--, 210421 ysm, 9S

		if((InfoModel1.ModelInfo < MODEL_35D_9S)||(InfoModel1.ModelInfo > MODEL_50D_9S))
		{
			TM_Oil_Temp_Processing(AI_DI_Low_Data1[5], (unsigned char)((Analog_Input_TimeCount % 1000) / 100));
		}
		else
		{
			Engine_Error_Check_Processing(AI_DI_Low_Data1[5], (unsigned char)((Analog_Input_TimeCount % 1000) / 100));
		}


	}
	if ((Analog_Input_TimeCount % 10) == 0)
		BAT_level_Processing(AI_DI_Low_Data2[1], (unsigned char)((Analog_Input_TimeCount % 200) / 10));
	#if 0
	LOAD_SENSOR 	= (float)(AI_DI_Low_Data2[5]>>2) * conversion_facter_5v;
	HYD_TEMP_SENSOR = (float)(AI_DI_Low_Data2[0]>>2) * conversion_facter_5v;
	#else
	SENSOR_VOLTAGE 	= (float)(AI_DI_Low_Data0[5]) * conversion_facter_5v;
	LOAD_SENSOR 	= (float)(AI_DI_Low_Data2[5]) * conversion_facter_5v; 

	if((InfoModel1.ModelInfo >= MODEL_25L_9A)&&(InfoModel1.ModelInfo <= MODEL_35LN_9A))
	{
		HYD_TEMP_SENSOR = (float)(AI_DI_Low_Data2[3]) * conversion_facter_5v * 0.75; // ?àTEMP
		HYD_OIL_PRESSURE = (float)(AI_DI_Low_Data2[0]) * conversion_facter_5v;
	}
	else if((InfoModel1.ModelInfo >= MODEL_25D_9HDI)&&(InfoModel1.ModelInfo <= MODEL_50DN_9HDI))
	{
		HYD_OIL_PRESSURE = (float)(AI_DI_Low_Data2[0]) * conversion_facter_5v;
	}
	else
	{
		HYD_TEMP_SENSOR = (float)(AI_DI_Low_Data2[0]) * conversion_facter_5v;
	}
	
	TM_OIL_PRESSURE = (float)(AI_DI_Low_Data2[7]) * conversion_facter_5v; 
	//++, 210408 ysm, 9S
	if((InfoModel1.ModelInfo >= MODEL_25L_9A)&&(InfoModel1.ModelInfo <= MODEL_35LN_9A))
	{
		TM_OIL_TEMP_SENSOR = (float)(AI_DI_Low_Data2[6]) * conversion_facter_Din; 
	}
	else if((InfoModel1.ModelInfo >= MODEL_35D_9S)&&(InfoModel1.ModelInfo <= MODEL_50D_9S))
	{
		TM_OIL_TEMP_SENSOR = (float)(AI_DI_Low_Data2[6]) * conversion_facter_Din;
	}
	
	BRAKE_OIL_PRESSURE = (float)(AI_DI_Low_Data1[1]) * conversion_facter_Din; 
	//--, 210408 ysm, 9S
	#endif
	if ((Analog_Input_TimeCount % 10) == 0)
		ALT_level_Processing(AI_DI_Low_Data2[2], (unsigned char)((Analog_Input_TimeCount % 200) / 10));
	

	//++, 220929 ysm, FSCU

	if(COUNT_FLAG.Flag_FSCU_Enable == 1)
	{
		if(RX_FSCU_DATA_65525.Parking_Pressure_Switch < 3)
			Flag_DIN[INDEX_PARKING_PRESSURE_SW] = ((RX_FSCU_DATA_65525.Parking_Pressure_Switch == 0x01) ? 0x01 : 0x00);
		else
			Flag_DIN[INDEX_PARKING_PRESSURE_SW] = 0;

		if(RX_FSCU_DATA_65525.Seat_Switch_Status < 3)
			Flag_DIN[INDEX_SEAT_SW] = ((RX_FSCU_DATA_65525.Seat_Switch_Status == 0x01) ? 0x01 : 0x00);
		else
			Flag_DIN[INDEX_SEAT_SW] = 0;

		if(RX_FSCU_DATA_65525.Seatbelt_Switch_Status < 3)
			Flag_DIN[INDEX_SW_BUCKLE] = ((RX_FSCU_DATA_65525.Seatbelt_Switch_Status == 0x01) ? 0x01 : 0x00);
		else
			Flag_DIN[INDEX_SW_BUCKLE] = 0;

		//++, 221012 ysm, FSCU_2
		#if 0
		if(RX_FSCU_DATA_65525.Gear_Selector_Status_Fowwrd < 3)
			Flag_DIN[INDEX_GEAR_SIGNAL_F] = RX_FSCU_DATA_65525.Gear_Selector_Status_Fowwrd;
		else
			Flag_DIN[INDEX_GEAR_SIGNAL_F] = 0;

		if(RX_FSCU_DATA_65525.Gear_Selector_Status_Neutral < 3)
			Flag_DIN[INDEX_GEAR_SIGNAL_N] = RX_FSCU_DATA_65525.Gear_Selector_Status_Neutral;
		else
			Flag_DIN[INDEX_GEAR_SIGNAL_N] = 0;	

		if(RX_FSCU_DATA_65525.Gear_Selector_Status_Reverse < 3)
			Flag_DIN[INDEX_GEAR_SIGNAL_R] = RX_FSCU_DATA_65525.Gear_Selector_Status_Reverse;
		else
			Flag_DIN[INDEX_GEAR_SIGNAL_R] = 0;
		#endif
		//--, 221012 ysm, FSCU_2
	}
	//--, 220929 ysm, FSCU
	

}
void Check_ADC_Value(void)
{ 
	//Run_ADC();
	CheckGearStatus();
	CheckSeatBeltStatus(); //++,--, 221212 ysm, FSCU

}

void Check_Tacho_Value(void)
{

	if(((InfoModel1.ModelInfo >= MODEL_25LC_9)&&(InfoModel1.ModelInfo <= MODEL_33LC_9))||
		((InfoModel1.ModelInfo >= MODEL_25L_9A)&&(InfoModel1.ModelInfo <= MODEL_35LN_9A)))
	{

		if(RX_ECU_DATA_65265.Vehicle_Speed <= 0xFAFF)
		{
			Calculate_Speed_Value = Speed_value_temp[1] = (float)(RX_ECU_DATA_65265.Vehicle_Speed) / 256 * 10;
			HCESPN.H532 = RX_ECU_DATA_65265.Vehicle_Speed;
			
		}
		else
		{
			Calculate_Speed_Value = Speed_value_temp[1] = 0;
			HCESPN.H532 = 0;
		}

	}
	else
	{
		TACHO.AVERAGE[TACHO.ARRAY_CNT++] = SPEED_COUNT;
		if(TACHO.ARRAY_CNT == FREQUENCY_DATA_CNT)
		{
			TACHO.ARRAY_CNT = 0;
			TACHO.AVERAGE[0] = SPEED_COUNT;
		}


		for(int i = 0; i < FREQUENCY_DATA_CNT; i++)
			TACHO.CNT_AVERAGE = TACHO.CNT_AVERAGE + TACHO.AVERAGE[i];

		TACHO.Value = TACHO.CNT_AVERAGE*10;
		Make_SpeedValue(TACHO.Value);


		TACHO.CNT_AVERAGE = 0;
		SPEED_COUNT = 0;
	}
}


void Initial_Digital_Input(struct Digital_Input_Type *Data, unsigned char Paramter1, unsigned char Paramter2, unsigned char Paramter3, unsigned char Paramter4, unsigned char Paramter5)
{
	Data->Parameter.On_Threshold = Paramter1;
	Data->Parameter.Off_Threshold = Paramter2;
	Data->Parameter.Max = Paramter3;
	Data->Parameter.Min = Paramter4;
	Data->Parameter.Active_Value = Paramter5;

	Data->Data.Value = Digital_Input_Off;
	Data->Data.Count = 0;
	Data->Data.Edge_On = 0;
	Data->Data.Edge_Off = 0;
	Data->Data.Count_On = 0;
	Data->Data.Count_Off = 0;
}

void Set_Digital_Input(struct Digital_Input_Type *Data, unsigned char Signal)
{
	if (Signal == Data->Parameter.Active_Value)
	{
		Data->Data.Edge_Off = 0;
		
		if (Data->Data.Count < Data->Parameter.Max)
			Data->Data.Count++;

		if (Data->Data.Count >= Data->Parameter.On_Threshold)
		{
			if (Data->Data.Value == Digital_Input_Off)
			{
				Data->Data.Value = Digital_Input_On;
				Data->Data.Edge_On = 1;
				Data->Data.Count_Off = 0;
			}
			else
			{
				Data->Data.Edge_On = 0;
				if (Data->Data.Count_On < 65535)
					Data->Data.Count_On++;
			}
		}
	}
	else
	{
		Data->Data.Edge_On = 0;
		
		if (Data->Data.Count > Data->Parameter.Min)
			Data->Data.Count--;

		if (Data->Data.Count <= Data->Parameter.Off_Threshold)
		{			
			if (Data->Data.Value == Digital_Input_On)
			{
				Data->Data.Value = Digital_Input_Off;
				Data->Data.Edge_Off = 1;
				Data->Data.Count_On = 0;
			}
			else
			{
				Data->Data.Edge_Off = 0;
				if (Data->Data.Count_Off < 65535)
					Data->Data.Count_Off++;
			}
		}
	}
}

void Initialize_Digital_Input_Variable(void)
{
	unsigned char i;

	// DIN Active Low
	for (i = 0; i < INDEX_GEAR_SIGNAL_F; i++)
		Initial_Digital_Input(&Digital_Input[i], DI_THRESHOLD, 0, DI_THRESHOLD, 0, 0);  

	// DIN Active High
	for (i = INDEX_GEAR_SIGNAL_F; i < INDEX_PWD_STD_MODE_SEL; i++)
	{
		if((i == INDEX_GEAR_SIGNAL_F) || (i == INDEX_GEAR_SIGNAL_R))
			Initial_Digital_Input(&Digital_Input[i], DI_GEAR_THRESHOLD, 0, DI_GEAR_THRESHOLD, 0, 1);  
		else if(i == INDEX_SW_BUCKLE)
			Initial_Digital_Input(&Digital_Input[INDEX_SW_BUCKLE], DI_THRESHOLD, 0, DI_THRESHOLD, 0, 0);
		else
			Initial_Digital_Input(&Digital_Input[i], DI_THRESHOLD, 0, DI_THRESHOLD, 0, 1);  	
	}
	

	// DIN Active Low
	Initial_Digital_Input(&Digital_Input[INDEX_PWD_STD_MODE_SEL], DI_THRESHOLD, 0, DI_THRESHOLD, 0, 0);  

	// DIN Active High
	for (i = INDEX_TURN_SIGNAL_RH; i < INDEX_INHIBIT_REGENERATION_SIG; i++)
	{
		if((i == INDEX_GEAR_SIGNAL_N))
			Initial_Digital_Input(&Digital_Input[i], DI_GEAR_THRESHOLD, 0, DI_GEAR_THRESHOLD, 0, 1);  
		else
			Initial_Digital_Input(&Digital_Input[i], DI_THRESHOLD, 0, DI_THRESHOLD, 0, 1);  	

	}
	// DIN Active Low
	// LPG Change, ysm
	for (i = INDEX_INHIBIT_REGENERATION_SIG; i < INDEX_TURN_SIGNAL_LH; i++)
		Initial_Digital_Input(&Digital_Input[i], DI_THRESHOLD, 0, DI_THRESHOLD, 0, 0);  


	// DIN Active High
	Initial_Digital_Input(&Digital_Input[INDEX_TURN_SIGNAL_LH], 10, 0, 10, 0, 1);  
	Initial_Digital_Input(&Digital_Input[i], DI_THRESHOLD, 0, DI_THRESHOLD, 0, 1);  	

	// DIN Active Low
	Initial_Digital_Input(&Digital_Input[INDEX_AUTO_TILT_SIG], DI_THRESHOLD, 0, DI_THRESHOLD, 0, 0);  

	// DIN Active High
	Initial_Digital_Input(&Digital_Input[INDEX_FUEL_WARMER_SIG], DI_THRESHOLD, 0, DI_THRESHOLD, 0, 1);  	

	// DIN Active Low
	Initial_Digital_Input(&Digital_Input[INDEX_WATER_SEPERATOR], DI_THRESHOLD, 0, DI_THRESHOLD, 0, 0);  

}


void SendDIN()
{
	unsigned char _Temp[8];
	struct st_CAN_Message1 Message;

	Message.Priority = 6;
	Message.Data_Page=0;
	Message.PDU_Format = 0xFF;
	Message.PDU_Specific = 0x01;
	Message.Source_Address = SA_CLUSTER;        // ++, --, 210225 ctw Auto JIG Final Test       //Message.Source_Address = SA_JIG;

	DIN_MMI_DATA*		DATA_DIN;

	memset((UCHAR*)(&_Temp[0]), 0xff,8);
	DATA_DIN = (DIN_MMI_DATA*)&_Temp[0];

	DATA_DIN->DIN1=Flag_DIN[INDEX_SEAT_SW];
	DATA_DIN->DIN2=Flag_DIN[INDEX_BRAKE_OIL_LEVEL];
	DATA_DIN->DIN3=Flag_DIN[INDEX_AIR_FILTER_WARNING];

	DATA_DIN->DIN4=Flag_DIN[INDEX_ENG_OIL_PRESSURE_SW];
//	DATA_DIN->DIN5=Flag_DIN[INDEX_TM_OIL_TEMP_WARNING];
	DATA_DIN->DIN6=Flag_DIN[INDEX_SW_BUCKLE];
	DATA_DIN->DIN7=Flag_DIN[INDEX_PARKING_PRESSURE_SW];

	DATA_DIN->DIN8=Flag_DIN[INDEX_GEAR_SIGNAL_F];
	DATA_DIN->DIN9=Flag_DIN[INDEX_GEAR_SIGNAL_N];
	DATA_DIN->DIN10=Flag_DIN[INDEX_GEAR_SIGNAL_R];
	DATA_DIN->DIN11=Flag_DIN[INDEX_PWD_STD_MODE_SEL];

	DATA_DIN->DIN12=Flag_DIN[INDEX_TURN_SIGNAL_RH];
	DATA_DIN->DIN13=Flag_DIN[INDEX_TURN_SIGNAL_LH];
	DATA_DIN->DIN14=Flag_DIN[INDEX_INHIBIT_REGENERATION_SIG];
	DATA_DIN->DIN15=Flag_DIN_MMI[INDEX_BRAKE_SW_SIG];       // ++, --, 210830 ctw Auto JIG Final Test

//	DATA_DIN->DIN16=Flag_DIN[INDEX_WATER_SEPERATOR];
	DATA_DIN->DIN16=Flag_DIN[INDEX_AUTO_TILT_SIG];
	DATA_DIN->DIN17=Flag_DIN[INDEX_FUEL_WARMER_SIG];

	memcpy(&Message.Data[0], &_Temp[0], 8); 

	CAN1_OperateRingBuffer(Message);
}


void SendAIN()
{
	unsigned char _Temp[8];
	struct st_CAN_Message1 Message;

	Message.Priority = 6;
	Message.Data_Page=0;
	Message.PDU_Format = 0xFF;
	Message.PDU_Specific = 0x02;
	Message.Source_Address = SA_JIG;

	AIN_MMI_DATA*		DATA_AIN;

	memset((UCHAR*)(&_Temp[0]), 0xff,8);
	DATA_AIN = (AIN_MMI_DATA*)&_Temp[0];

	DATA_AIN->AIN1=(unsigned char)(AD_DATA.ALT_VOLT* 10);
	DATA_AIN->AIN2=HCESPN.Fuel_Sensor_Resistor;
	DATA_AIN->AIN3=(unsigned char)(HYD_TEMP_SENSOR* 10);

	DATA_AIN->AIN4=(unsigned char)(LOAD_SENSOR* 10);
	DATA_AIN->AIN5=HCESPN.TM_Oil_Temp_Sensor_Resistor;
	DATA_AIN->AIN6=(unsigned char)(TM_OIL_PRESSURE* 10);


	memcpy(&Message.Data[0], &_Temp[0], 8); 

	CAN1_OperateRingBuffer(Message);
}

// ++, 210225 ctw Auto JIG Final Test
void SendAIN_R(void)
{
	unsigned char _Temp[8];
	struct st_CAN_Message1 Message;

	Message.Priority = 6;
	Message.Data_Page=0;
	Message.PDU_Format = 0xF6;
	Message.PDU_Specific = 0x97;
	Message.Source_Address = SA_CLUSTER;

	AIN_R_MMI_DATA*		DATA_AIN_R;

	memset((UCHAR*)(&_Temp[0]), 0xff,8);
	DATA_AIN_R = (AIN_R_MMI_DATA*)&_Temp[0];

    DATA_AIN_R->AIN2=HCESPN.Fuel_Sensor_Resistor;
    DATA_AIN_R->AIN5=HCESPN.TM_Oil_Temp_Sensor_Resistor;

	DATA_AIN_R->AIN6=(unsigned short)(AD_DATA.BATTERY_VOLT* 1000);

	memcpy(&Message.Data[0], &_Temp[0], 8); 

	CAN1_OperateRingBuffer(Message);
}


void SendAIN_V(void)
{
	unsigned char _Temp[8];
	struct st_CAN_Message1 Message;

	Message.Priority = 6;
	Message.Data_Page=0;
	Message.PDU_Format = 0xF7;
	Message.PDU_Specific = 0x97;
	Message.Source_Address = SA_CLUSTER;

	AIN_V_MMI_DATA*		DATA_AIN_V;

	memset((UCHAR*)(&_Temp[0]), 0xff,8);
        
	DATA_AIN_V = (AIN_V_MMI_DATA*)&_Temp[0];

	DATA_AIN_V->AIN1=(unsigned short)(AD_DATA.ALT_VOLT* 1000);
	DATA_AIN_V->AIN3=(unsigned short)(HYD_TEMP_SENSOR* 1000);

	DATA_AIN_V->AIN4=(unsigned short)(LOAD_SENSOR* 1000);
	DATA_AIN_V->AIN6=(unsigned short)(TM_OIL_PRESSURE* 1000);


	memcpy(&Message.Data[0], &_Temp[0], 8); 

	CAN1_OperateRingBuffer(Message);
}
// --, 210225 ctw Auto JIG Final Test
