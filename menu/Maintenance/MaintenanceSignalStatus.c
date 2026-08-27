#include "FLV_Cluster_APP.h"

#define SIGNAL_Y_OFFSET	63
#define SIGNAL_NO_DISPLAY		0xFF
#define SIGNAL_DISPLAY			0xFE
#define DIGITAL_INPUT_LOW			0
#define DIGITAL_INPUT_HIGH		1

#define MAX_SIGNAL			ANALOG_TOTAL

#define DISPLAY_1			0
#define DISPLAY_0_1		1
#define DISPLAY_0_4		2
#define DISPLAY_0_01		3
#define DISPLAY_0_0002		4
#define DISPLAY_SMALL_0_1		5

#define CHANGE_NEW			1
#define CHANGE_DELETE		2

USHORT		*Signal_StringList[MAX_SIGNAL];
USHORT		*Unit_StringList[ANALOG_TOTAL];
USHORT		Unit_List[DIGITAL_INPUT_TOTAL];
USHORT		Signal_OldValue[MAX_SIGNAL];
UCHAR		Signal_List[MAX_SIGNAL];
UCHAR		Signal_OldList[MAX_SIGNAL];
UCHAR		Signal_Change[MAX_SIGNAL];
UCHAR		ListCount=0;
UCHAR		nOldPageCount=0;

int nCurListCount= 0;

extern float 				LOAD_SENSOR;
extern float 				TM_OIL_PRESSURE;
extern float 				HYD_TEMP_SENSOR;
extern float 				BATTERY_VOLT;
extern float 				Calculate_Speed_Value;
extern unsigned int 		Odometer_Value;
extern unsigned int 		Odometer_Value_cm;

extern st_DOUT_DATA		DOUT_DATA;
extern st_AD_DATA 		AD_DATA;
extern FREQUENCY_DATA 	TACHO;

extern CAN_ECU_DATA_61443				RX_ECU_DATA_61443;
extern CAN_ECU_DATA_61444				RX_ECU_DATA_61444;
extern CAN_ECU_DATA_64890				RX_ECU_DATA_64890;
extern CAN_ECU_DATA_65226				RX_ECU_DATA_65226;
extern CAN_ECU_DATA_65252				RX_ECU_DATA_65252;
extern CAN_ECU_DATA_65262				RX_ECU_DATA_65262;
extern CAN_ECU_DATA_65279				RX_ECU_DATA_65279;

// ++, 200924 ctw LPG
extern CAN_ECU_DATA_65253				RX_ECU_DATA_65253;
extern CAN_ECU_DATA_65263				RX_ECU_DATA_65263;
// --, 200924 ctw LPG

extern EEPROM_MODEL_DATA1 InfoModel1;

extern unsigned char PROTO_EQUIPMENT;
extern unsigned char MAST_EQUIPMENT;
extern unsigned char ANGLE_EQUIPMENT;

extern unsigned char Engine_Type;

extern unsigned char Signal_Engine_Check_lamp;
extern unsigned char Signal_WIF_lamp;
extern unsigned char Signal_Engine_Oil_Check_lamp;
extern unsigned char Signal_TM_Oil_Check_lamp;

extern float HYD_OIL_PRESSURE; //++,--, 220829 ysm, 9A

//++, 221018 ysm, FSCU_3
extern CAN_FSCU_STATE1_65525						RX_FSCU_DATA_65525;
//--, 221018 ysm, FSCU_3


void InitAnalogList()
{

	Signal_StringList[ANALOG_EG_RPM] = (USHORT*)*StringSentence[94];
	Signal_StringList[ANALOG_DPF_LEVEL] = (USHORT*)*StringSentence[95];
	Signal_StringList[ANALOG_DPF_SOOT_LOAD_PERCENT] = (USHORT*)*StringSentence[96];
	Signal_StringList[ANALOG_TM_TEMPERATURE] = (USHORT*)*StringSentence[97];
	Signal_StringList[ANALOG_TM_TEMPERATURE_SENSOR_RESISTANCE] = (USHORT*)*StringSentence[98];
	Signal_StringList[ANALOG_TM_PRESSURE] = (USHORT*)*StringSentence[99];
	Signal_StringList[ANALOG_TM_PRESSURE_SENSOR_VOLT] = (USHORT*)*StringSentence[100];
	Signal_StringList[ANALOG_HYD_TEMPERATURE] = (USHORT*)*StringSentence[101];
	Signal_StringList[ANALOG_HYD_TEMPERATURE_SENSOR_VOLT] = (USHORT*)*StringSentence[102];
	Signal_StringList[ANALOG_HYD_PRESSURE] = (USHORT*)*StringSentence[103];
	Signal_StringList[ANALOG_HYD_PRESSURE_SENSOR_VOLT] = (USHORT*)*StringSentence[104];
	Signal_StringList[ANALOG_BREAK_PRESSURE] = (USHORT*)*StringSentence[203];
	Signal_StringList[ANALOG_BREAK_PRESSURE_SENSOR_VOLT] = (USHORT*)*StringSentence[105];
	Signal_StringList[ANALOG_WEIGHT_SENSOR_PRESSURE] = (USHORT*)*StringSentence[106];
	Signal_StringList[ANALOG_WEIGHT_SENSOR_VOLT] = (USHORT*)*StringSentence[107];
	Signal_StringList[ANALOG_ACCUMULATED_WEIGHT] = (USHORT*)*StringSentence[108];
	Signal_StringList[ANALOG_COOLANT_TEMPERATURE] = (USHORT*)*StringSentence[109];
	Signal_StringList[ANALOG_COOLANT_SENSOR_RESISTANCE] = (USHORT*)*StringSentence[110];
	Signal_StringList[ANALOG_FUEL_GAUGE] = (USHORT*)*StringSentence[111];
	Signal_StringList[ANALOG_FUEL_SENSOR_RESISTANCE] = (USHORT*)*StringSentence[112];
	Signal_StringList[ANALOG_VEHICLE_SPEED] = (USHORT*)*StringSentence[113];
	Signal_StringList[ANALOG_SPEED_SENSOR_FREQ] = (USHORT*)*StringSentence[114];
	Signal_StringList[ANALOG_VEGICLE_VOLT] = (USHORT*)*StringSentence[115];
	Signal_StringList[ANALOG_ALTERNATOR_L] = (USHORT*)*StringSentence[116];
	Signal_StringList[ANALOG_TOTAL_MILEAGE] = (USHORT*)*StringSentence[117];
	Signal_StringList[ANALOG_ACCELERATOR_PEDAL] = (USHORT*)*StringSentence[118];
	Signal_StringList[ANALOG_TILT_SENSOR_DEGREE_X] = (USHORT*)*StringSentence[119];
	Signal_StringList[ANALOG_VEHICLE_TILT_X] = (USHORT*)*StringSentence[120];
	Signal_StringList[ANALOG_VEHICLE_TILT_Y] = (USHORT*)*StringSentence[188];	
	
	
	if(((InfoModel1.ModelInfo >= MODEL_35D_9S)&&(InfoModel1.ModelInfo <= MODEL_50D_9S))||((InfoModel1.ModelInfo >= MODEL_25L_9A)&&(InfoModel1.ModelInfo <= MODEL_35LN_9A)))
		Signal_StringList[ANALOG_TM_TEMPERATURE_SENSOR_RESISTANCE] = (USHORT*)*StringSentence[202]; // TM 온도 센서 전압			
				
	
	// 단위 : rpm
	Unit_StringList[ANALOG_EG_RPM] = (USHORT*)StringEtc[SIGN_RPM];
	// 단위 : level
	Unit_StringList[ANALOG_DPF_LEVEL] = Unit_StringList[ANALOG_FUEL_GAUGE] = (USHORT*)StringEtc[SIGN_LEVEL];       
	// 단위 : %
	Unit_StringList[ANALOG_DPF_SOOT_LOAD_PERCENT] = (USHORT*)StringEtc[SIGN_PERCENT];         					// %
	Unit_StringList[ANALOG_ACCELERATOR_PEDAL] = (USHORT*)StringEtc[SIGN_PERCENT];         					// %
	// 단위 : 온도 
	Unit_StringList[ANALOG_TM_TEMPERATURE] = Unit_StringList[ANALOG_HYD_TEMPERATURE] = (USHORT*)StringEtc[SIGN_TEMP_C+InfoDisplaySetting.TemperatureUnit];	//온도
	Unit_StringList[ANALOG_COOLANT_TEMPERATURE] = (USHORT*)StringEtc[SIGN_TEMP_C+InfoDisplaySetting.TemperatureUnit];	//온도
	// 단위 : 전압 
	Unit_StringList[ANALOG_TM_PRESSURE_SENSOR_VOLT] = (USHORT*)StringEtc[SIGN_V];          					// 전압
	Unit_StringList[ANALOG_HYD_TEMPERATURE_SENSOR_VOLT] = Unit_StringList[ANALOG_HYD_PRESSURE_SENSOR_VOLT] = (USHORT*)StringEtc[SIGN_V];          					// 전압
	Unit_StringList[ANALOG_BREAK_PRESSURE_SENSOR_VOLT] = Unit_StringList[ANALOG_WEIGHT_SENSOR_VOLT] = (USHORT*)StringEtc[SIGN_V];          					// 전압
	Unit_StringList[ANALOG_VEGICLE_VOLT]  = (USHORT*)StringEtc[SIGN_V];          					// 전압
	Unit_StringList[ANALOG_ALTERNATOR_L]  = (USHORT*)StringEtc[SIGN_V];      // ++, --, 200326 bwk    					// 전압

	// 단위 : 압력
	Unit_StringList[ANALOG_TM_PRESSURE] = Unit_StringList[ANALOG_HYD_PRESSURE] = (USHORT*)StringEtc[SIGN_BAR+InfoDisplaySetting.PressureUnit];
	Unit_StringList[ANALOG_WEIGHT_SENSOR_PRESSURE] = (USHORT*)StringEtc[SIGN_BAR+InfoDisplaySetting.PressureUnit];
	Unit_StringList[ANALOG_BREAK_PRESSURE] = (USHORT*)StringEtc[SIGN_BAR+InfoDisplaySetting.PressureUnit]; //9s
	// 단위 : 무게
	Unit_StringList[ANALOG_ACCUMULATED_WEIGHT] = (USHORT*)StringEtc[SIGN_TON+InfoDisplaySetting.WeightUnit];
	// 단위 : 저항 
	if(((InfoModel1.ModelInfo >= MODEL_35D_9S)&&(InfoModel1.ModelInfo <= MODEL_50D_9S))||((InfoModel1.ModelInfo >= MODEL_25L_9A)&&(InfoModel1.ModelInfo <= MODEL_35LN_9A)))
		Unit_StringList[ANALOG_TM_TEMPERATURE_SENSOR_RESISTANCE] = (USHORT*)StringEtc[SIGN_V];        				// 전압 
	else
		Unit_StringList[ANALOG_TM_TEMPERATURE_SENSOR_RESISTANCE] = (USHORT*)StringEtc[14];         					// 저항 
		
	Unit_StringList[ANALOG_COOLANT_SENSOR_RESISTANCE] = Unit_StringList[ANALOG_FUEL_SENSOR_RESISTANCE] = (USHORT*)StringEtc[14];         					// 저항           
	// 단위 : 속도 
	Unit_StringList[ANALOG_VEHICLE_SPEED] = (USHORT*)StringEtc[SIGN_KMH+InfoDisplaySetting.SpeedUnit];         		// 속도
	// 단위 : 주파수
	Unit_StringList[ANALOG_SPEED_SENSOR_FREQ] = (USHORT*)StringEtc[SIGN_HZ];        					// Hz
	// 단위 : 거리 
	Unit_StringList[ANALOG_TOTAL_MILEAGE] = (USHORT*)StringEtc[SIGN_KM+InfoDisplaySetting.SpeedUnit];							// km
	// 단위 : 각도 
	Unit_StringList[ANALOG_TILT_SENSOR_DEGREE_X] = Unit_StringList[ANALOG_VEHICLE_TILT_X] = Unit_StringList[ANALOG_VEHICLE_TILT_Y] = (USHORT*)StringEtc[SIGN_ANGLE];        					// 각도 
	
	memset((&Signal_OldValue), 0xff, sizeof(Signal_OldValue));

	memset((UCHAR *)(&Signal_List), 0x00, sizeof( Signal_List));
	memset((UCHAR *)(&Signal_OldList), 0x00, sizeof( Signal_OldList));

	ListCount = nOldPageCount = 0;
	OldCurserIndex = 0xff;
	CurserIndex = 0;

}

void InitDigitalInputList()
{
	unsigned char gear_switch_flag;
	unsigned char model_flag;

	if((InfoModel1.ModelInfo >= MODEL_35D_9S)&&(InfoModel1.ModelInfo <= MODEL_50D_9S))
	{
		gear_switch_flag = 1;
		model_flag = 0;

	}
	else if((InfoModel1.ModelInfo >= MODEL_35L_9)&&(InfoModel1.ModelInfo <= MODEL_33LC_9))
	{
		gear_switch_flag = 1;
		model_flag = 1;
	}
	else
	{
		gear_switch_flag = 0;
		model_flag = 0;
	}


	for(UCHAR i=0;i<DIGITAL_INPUT_TOTAL;i++)
	{
		Signal_StringList[i] = (USHORT*)*StringSentence[121+i];


		if(i == 18)
		{
			if(((InfoModel1.ModelInfo >= MODEL_35L_9)&&(InfoModel1.ModelInfo <= MODEL_33LC_9))||
				((InfoModel1.ModelInfo >= MODEL_25D_9HDI)&&(InfoModel1.ModelInfo <= MODEL_50DN_9HDI))||
				((InfoModel1.ModelInfo >= MODEL_25L_9A)&&(InfoModel1.ModelInfo <= MODEL_35LN_9A)))
			{
				Signal_StringList[i] = (USHORT*)*StringSentence[209]; // LPG TYPE = DPF REGEN SW -> BRAKE SW
			}

		}
				
		if((Engine_Type == LPG_TYPE)&&(i == 20))
		{
			Signal_StringList[i] = (USHORT*)*StringSentence[146]; // LPG TYPE = PWD/STD SW -> LPG EMPTY SW

		}		
	
		if((gear_switch_flag==1)&&(i == 19))
		{
			Signal_StringList[i] = (USHORT*)*StringSentence[193]; //++,--, 201019 ysm, GEAR SELECT SWITCH
		}
		
		
		if(((i>=DIGITAL_LEFT_TURN_SW) && (i<=DIGITAL_REVERSE_GEAR_SW)) || (i==DIGITAL_ENGINE_PREHEAT_STATUS))
			Unit_List[i] = DIGITAL_INPUT_HIGH;
		else
			Unit_List[i] = DIGITAL_INPUT_LOW;

		if(model_flag == 1)
		{
			if(i == DIGITAL_DPF_REGEN_SW)
				Unit_List[i] = DIGITAL_INPUT_HIGH; // BRAKE SWITCH = ACTIVE HIGH(50L-9)
		}
			
	}
	
	memset((&Signal_OldValue), 0xff, sizeof(Signal_OldValue));

	memset((UCHAR *)(&Signal_List), 0x00, sizeof( Signal_List));
	memset((UCHAR *)(&Signal_OldList), 0x00, sizeof( Signal_OldList));

	ListCount = nOldPageCount = 0;
	OldCurserIndex = 0xff;
	CurserIndex = 0;
}

void InitDigitalOutputList()
{
	/*
		AUTO_PARKING_RELAY				0
		TRAVEL_CUT_RELAY				1
		ATTACH_CUT_RELAY				2
		ANTIRESTART_SW					3	
		RELAY_SHIFT_RELAY				4
		RELAY_SHIFT_RELAY2				5
		HYD_AUTO_REGENERATION			6
		Current Gear Status				7
	*/



	if((Engine_Type == LPG_TYPE)||((InfoModel1.ModelInfo >= MODEL_35D_9S)&&(InfoModel1.ModelInfo <= MODEL_50D_9S))||((InfoModel1.ModelInfo >= MODEL_35D_9HDI)&&(InfoModel1.ModelInfo <= MODEL_50DN_9HDI)))
	{

		for(UCHAR i=0;i<DIGITAL_OUTPUT_TOTAL;i++)
		{

			if(i==0) 
			{	
				//++, 221226 ysm, FSCU
				if((InfoModel1.ModelInfo >= MODEL_35D_9S)&&(InfoModel1.ModelInfo <= MODEL_50D_9S))	
					Signal_StringList[i] = (USHORT*)*StringSentence[143]; // 오토파킹 없음(9S) -> 오토 레벨
				else
				{
					
					if(COUNT_FLAG.Flag_FSCU_Enable == 1)
					{
						Signal_StringList[i] = (USHORT*)*StringSentence[228]; // 파킹 밸브
					}
					else
					{
						Signal_StringList[i] = (USHORT*)*StringSentence[147]; // 오토 파킹 릴레이
					}
				}
			}
			else if(i==2)
			{
				if(COUNT_FLAG.Flag_FSCU_Enable == 1)
				{
					Signal_StringList[i] = (USHORT*)*StringSentence[229]; // 작업장치차단 밸브
				}
				else
				{
					Signal_StringList[i] = (USHORT*)*StringSentence[149]; // 작업장치차단
				}			
				
			}
			//--, 221226 ysm, FSCU
			else if(i==4)
			{
				//++, 210617 ysm, 9S
				#if 1
				if(((InfoModel1.ModelInfo >= MODEL_35D_9S)&&(InfoModel1.ModelInfo <= MODEL_50D_9S))||((InfoModel1.ModelInfo >= MODEL_25L_9A)&&(InfoModel1.ModelInfo <= MODEL_35LN_9A)))
				{
					Signal_StringList[i] = (USHORT*)*StringSentence[144]; // 브레이크 램프 상태
				}
				else
				{
					if((InfoModel1.ModelInfo >= MODEL_25LC_9)&&(InfoModel1.ModelInfo <= MODEL_33LC_9))
						Signal_StringList[i] = (USHORT*)*StringSentence[231];  // 현재 기어 상태
					else
						Signal_StringList[i] = (USHORT*)*StringSentence[197]; // 기어 1단
				}
				#else
				Signal_StringList[i] = (USHORT*)*StringSentence[197]; // 기어 1단
				#endif
				//--, 210617 ysm, 9S

			}
			else if(i==5)
			{

				if((InfoModel1.ModelInfo >= MODEL_35D_9S)&&(InfoModel1.ModelInfo <= MODEL_50D_9S))
					Signal_StringList[i] = (USHORT*)*StringSentence[197]; // 기어 1단
				else if((InfoModel1.ModelInfo >= MODEL_25L_9A)&&(InfoModel1.ModelInfo <= MODEL_35LN_9A))
					Signal_StringList[i] = (USHORT*)*StringSentence[231];  // 현재 기어 상태
				else
					Signal_StringList[i] = (USHORT*)*StringSentence[198];  // 기어 2단

			}
			//++, 230209 ysm, FSCU
			else if(i==6)
			{
				Signal_StringList[i] = (USHORT*)*StringSentence[231];  // 현재 기어 상태

			}
			//--, 230209 ysm, FSCU
			else
				Signal_StringList[i] = (USHORT*)*StringSentence[147+i];
			
			Unit_List[i] = DIGITAL_INPUT_HIGH;

			if((InfoModel1.ModelInfo >= MODEL_35D_9S)&&(InfoModel1.ModelInfo <= MODEL_50D_9S)) // 9S, BRAKE LAMP STATUS -> ACTIVE LOW
			{
				if(i == 4)
					Unit_List[i] = DIGITAL_INPUT_LOW;
			}
		}
	}
	else
	{

		for(UCHAR i=0;i<DIGITAL_OUTPUT_TOTAL;i++)
		{
			//++, 221226 ysm, FSCU
			if(i==0)
			{			
				if(COUNT_FLAG.Flag_FSCU_Enable == 1)
				{
					Signal_StringList[i] = (USHORT*)*StringSentence[228]; // 파킹 밸브
				}
				else
				{
					Signal_StringList[i] = (USHORT*)*StringSentence[147]; // 오토 파킹 릴레이
				}

				Unit_List[i] = DIGITAL_INPUT_HIGH;
			}
			else if(i==2)
			{
				if(COUNT_FLAG.Flag_FSCU_Enable == 1)
				{
					Signal_StringList[i] = (USHORT*)*StringSentence[229]; // 작업장치차단 밸브
				}
				else
				{
					Signal_StringList[i] = (USHORT*)*StringSentence[149]; // 작업장치차단
				}

				Unit_List[i] = DIGITAL_INPUT_HIGH;
			}
			else if(i==5)
			{
				if((InfoModel1.ModelInfo >= MODEL_25D_9HDI)&&(InfoModel1.ModelInfo <= MODEL_35DN_9HDI_S))
					Signal_StringList[i] = (USHORT*)*StringSentence[231];  // 현재 기어 상태
				else
				{
					Signal_StringList[i] = (USHORT*)*StringSentence[147+i];
					Unit_List[i] = DIGITAL_INPUT_HIGH;
				}
						
			}
			//++, 230209 ysm, FSCU
			else if(i==6)
			{
				Signal_StringList[i] = (USHORT*)*StringSentence[231];  // 현재 기어 상태

			}
			//--, 230209 ysm, FSCU
			else
			{
				Signal_StringList[i] = (USHORT*)*StringSentence[147+i];
				Unit_List[i] = DIGITAL_INPUT_HIGH;
			}
			//--, 221226 ysm, FSCU			
		}

	}


	
	memset((&Signal_OldValue), 0xff, sizeof(Signal_OldValue));

	if((InfoModel1.ModelInfo >= MODEL_25LC_9)&&(InfoModel1.ModelInfo <= MODEL_33LC_9))
	{
		ListCount = 5; //++,--, 230209 ysm, FSCU
		nOldPageCount = 3; //++,--, 230209 ysm, FSCU
	}
	else if((InfoModel1.ModelInfo >= MODEL_25D_9HDI)&&(InfoModel1.ModelInfo <= MODEL_35DN_9HDI_S))
	{
		ListCount = 6; //++,--, 230209 ysm, FSCU
		nOldPageCount = 3; //++,--, 230209 ysm, FSCU
	}
	else if((InfoModel1.ModelInfo >= MODEL_25L_9A)&&(InfoModel1.ModelInfo <= MODEL_35LN_9A))
	{
		ListCount = 6; //++,--, 230209 ysm, FSCU
		nOldPageCount = 3; //++,--, 230209 ysm, FSCU
	}
	else
	{
		ListCount = 7; //++,--, 230209 ysm, FSCU
		nOldPageCount = 4; //++,--, 230209 ysm, FSCU
	}
	
	OldCurserIndex = 0xff;
	CurserIndex = 0;
}


void InitDigitalHidden()
{
	memset((&Signal_OldValue), 0xff, sizeof(Signal_OldValue));
}

UCHAR CheckAnalogList()
{
	UCHAR bChagne=0;

	memset((UCHAR *)(&Signal_Change), 0x00, sizeof( Signal_Change));
	
	// E/G RPM - 0
	if(Signal_OldValue[ANALOG_EG_RPM] != HCESPN.rpm_310)
	{
		Signal_OldValue[ANALOG_EG_RPM] = HCESPN.rpm_310;		
		Signal_Change[ANALOG_EG_RPM] = 1;
		bChagne = 1;
	}
	// DPF LEVEL - 1

	if(Engine_Type == DIESEL_TYPE)
	{

		if((InfoModel1.ModelInfo <= MODEL_35DN_9VB)||((InfoModel1.ModelInfo >= MODEL_25D_9VS)&&(InfoModel1.ModelInfo <= MODEL_35DN_9VS)))
		{	
			if(Signal_OldValue[ANALOG_DPF_LEVEL] != RX_ECU_DATA_64890.DPF_Regen_Level_3800)
			{
				Signal_OldValue[ANALOG_DPF_LEVEL] = RX_ECU_DATA_64890.DPF_Regen_Level_3800;	
				Signal_Change[ANALOG_DPF_LEVEL] = 1;
				bChagne = 1;
			}
		}
	}
	
	// DPF Soot Load Percent - 2	// 구현 x
	if(Signal_OldValue[ANALOG_DPF_SOOT_LOAD_PERCENT] != 0xffff)
	{
		Signal_OldValue[ANALOG_DPF_SOOT_LOAD_PERCENT] = 0xffff;
		Signal_Change[ANALOG_DPF_SOOT_LOAD_PERCENT] = 1;
		bChagne = 1;
	}


	// TM 온도 - 3	// 구현 x
	if(Signal_OldValue[ANALOG_TM_TEMPERATURE] != HCESPN.TM_Oil_Temp)
	{
		Signal_OldValue[ANALOG_TM_TEMPERATURE] = HCESPN.TM_Oil_Temp;		
		Signal_Change[ANALOG_TM_TEMPERATURE] = 1;
		bChagne = 1;
	}
	// TM 온도 센서 전압 - 4	
	if(((InfoModel1.ModelInfo >= MODEL_35D_9S)&&(InfoModel1.ModelInfo <= MODEL_50D_9S))||((InfoModel1.ModelInfo >= MODEL_25L_9A)&&(InfoModel1.ModelInfo <= MODEL_35LN_9A)))
	{
		if(Signal_OldValue[ANALOG_TM_TEMPERATURE_SENSOR_RESISTANCE] != (unsigned short)(AD_DATA.C_TM_OIL_TEMP_TEMP[0]*100))
		{
			Signal_OldValue[ANALOG_TM_TEMPERATURE_SENSOR_RESISTANCE] = (unsigned short)(AD_DATA.C_TM_OIL_TEMP_TEMP[0]*100); 	
			Signal_Change[ANALOG_TM_TEMPERATURE_SENSOR_RESISTANCE] = 1;
			bChagne = 1;
		}
	
	}
	else
	{
		if(Signal_OldValue[ANALOG_TM_TEMPERATURE_SENSOR_RESISTANCE] != HCESPN.TM_Oil_Temp_Sensor_Resistor)
		{
			Signal_OldValue[ANALOG_TM_TEMPERATURE_SENSOR_RESISTANCE] = HCESPN.TM_Oil_Temp_Sensor_Resistor;		
			Signal_Change[ANALOG_TM_TEMPERATURE_SENSOR_RESISTANCE] = 1;
			bChagne = 1;
		}
	}


	// TM 압력 - 5
	if(Signal_OldValue[ANALOG_TM_PRESSURE] !=  (unsigned short)(AD_DATA.C_TMOilPressureBar))
	{
		Signal_OldValue[ANALOG_TM_PRESSURE] =  (unsigned short)(AD_DATA.C_TMOilPressureBar);		
		Signal_Change[ANALOG_TM_PRESSURE] = 1;
		bChagne = 1;
	}
	// TM 압력 센서 전압 - 6
	if(Signal_OldValue[ANALOG_TM_PRESSURE_SENSOR_VOLT] != (unsigned short)(TM_OIL_PRESSURE * 100))
	{
		Signal_OldValue[ANALOG_TM_PRESSURE_SENSOR_VOLT] = (unsigned short)(TM_OIL_PRESSURE * 100);		
		Signal_Change[ANALOG_TM_PRESSURE_SENSOR_VOLT] = 1;
		bChagne = 1;
	}	



	if(Engine_Type == DIESEL_TYPE)
	{
	
		if((InfoModel1.ModelInfo >= MODEL_35D_9S)&&(InfoModel1.ModelInfo <= MODEL_50D_9S))
		{
			// 브레이크 압력 - 28
			if(Signal_OldValue[ANALOG_BREAK_PRESSURE] != (unsigned short)(AD_DATA.C_BRAKE_OIL_PressureBar))
			{
				Signal_OldValue[ANALOG_BREAK_PRESSURE] = (unsigned short)(AD_DATA.C_BRAKE_OIL_PressureBar);		
				Signal_Change[ANALOG_BREAK_PRESSURE] = 1;
				bChagne = 1;
			}


			// 브레이크 압력 센서 전압 - 11
			if(Signal_OldValue[ANALOG_BREAK_PRESSURE_SENSOR_VOLT] != (unsigned short)(AD_DATA.C_BRAKE_OIL_PRESSURE_TEMP[0] * 100))
			{
				Signal_OldValue[ANALOG_BREAK_PRESSURE_SENSOR_VOLT] = (unsigned short)(AD_DATA.C_BRAKE_OIL_PRESSURE_TEMP[0] * 100);		
				Signal_Change[ANALOG_BREAK_PRESSURE_SENSOR_VOLT] = 1;
				bChagne = 1;
			}

		}
		else
		{

			if((InfoModel1.ModelInfo >= MODEL_25D_9HDI)&&(InfoModel1.ModelInfo <= MODEL_50DN_9HDI))
			{

				// 유압 온도  - 7
				if(Signal_OldValue[ANALOG_HYD_TEMPERATURE] != 0xffff)
				{
					Signal_OldValue[ANALOG_HYD_TEMPERATURE] = 0xffff;		
					Signal_Change[ANALOG_HYD_TEMPERATURE] = 1;
					bChagne = 1;
				}
				// 유압 온도 센서 전압 - 8
				if(Signal_OldValue[ANALOG_HYD_TEMPERATURE_SENSOR_VOLT] != 0xffff)
				{
					Signal_OldValue[ANALOG_HYD_TEMPERATURE_SENSOR_VOLT] = 0xffff;
					Signal_Change[ANALOG_HYD_TEMPERATURE_SENSOR_VOLT] = 1;
					bChagne = 1;
				}

				// 유압 압력  - 9
				if(Signal_OldValue[ANALOG_HYD_PRESSURE] != (unsigned short)(AD_DATA.C_HydPress))
				{
					Signal_OldValue[ANALOG_HYD_PRESSURE] = (unsigned short)(AD_DATA.C_HydPress);		
					Signal_Change[ANALOG_HYD_PRESSURE] = 1;
					bChagne = 1;
				}
				// 유압 압력 센서 전압 - 10
				if(Signal_OldValue[ANALOG_HYD_PRESSURE_SENSOR_VOLT] != (unsigned short)(HYD_OIL_PRESSURE * 100))
				{
					Signal_OldValue[ANALOG_HYD_PRESSURE_SENSOR_VOLT] = (unsigned short)(HYD_OIL_PRESSURE * 100);		
					Signal_Change[ANALOG_HYD_PRESSURE_SENSOR_VOLT] = 1;
					bChagne = 1;
				}

			}
			else
			{
				// 유압 온도  - 7
				if(Signal_OldValue[ANALOG_HYD_TEMPERATURE] != (unsigned short)(AD_DATA.C_HydTemp))
				{
					Signal_OldValue[ANALOG_HYD_TEMPERATURE] = (unsigned short)(AD_DATA.C_HydTemp);		
					Signal_Change[ANALOG_HYD_TEMPERATURE] = 1;
					bChagne = 1;
				}
				// 유압 온도 센서 전압 - 8
				if(Signal_OldValue[ANALOG_HYD_TEMPERATURE_SENSOR_VOLT] != (unsigned short)(HYD_TEMP_SENSOR * 100))
				{
					Signal_OldValue[ANALOG_HYD_TEMPERATURE_SENSOR_VOLT] = (unsigned short)(HYD_TEMP_SENSOR * 100);
					Signal_Change[ANALOG_HYD_TEMPERATURE_SENSOR_VOLT] = 1;
					bChagne = 1;
				}

				// 유압 압력  - 9	// 구현 x
				if(Signal_OldValue[ANALOG_HYD_PRESSURE] != 0xffff)
				{
					Signal_OldValue[ANALOG_HYD_PRESSURE] = 0xffff;		
					Signal_Change[ANALOG_HYD_PRESSURE] = 1;
					bChagne = 1;
				}
				// 유압 압력 센서 전압 - 10	// 구현 x
				if(Signal_OldValue[ANALOG_HYD_PRESSURE_SENSOR_VOLT] != 0xffff)
				{
					Signal_OldValue[ANALOG_HYD_PRESSURE_SENSOR_VOLT] = 0xffff;		
					Signal_Change[ANALOG_HYD_PRESSURE_SENSOR_VOLT] = 1;
					bChagne = 1;
				}


			}

		}
	
	}
	else
	{
		if((InfoModel1.ModelInfo >= MODEL_25L_9A)&&(InfoModel1.ModelInfo <= MODEL_35LN_9A))
		{

			#if 0
			// 유압 온도  - 7
			if(Signal_OldValue[ANALOG_HYD_TEMPERATURE] != (unsigned short)(AD_DATA.C_HydTemp))
			{
				Signal_OldValue[ANALOG_HYD_TEMPERATURE] = (unsigned short)(AD_DATA.C_HydTemp);		
				Signal_Change[ANALOG_HYD_TEMPERATURE] = 1;
				bChagne = 1;
			}

			// 유압 온도 센서 전압 - 8
			if(Signal_OldValue[ANALOG_HYD_TEMPERATURE_SENSOR_VOLT] != (unsigned short)(HYD_TEMP_SENSOR * 100))
			{
				Signal_OldValue[ANALOG_HYD_TEMPERATURE_SENSOR_VOLT] = (unsigned short)(HYD_TEMP_SENSOR * 100);
				Signal_Change[ANALOG_HYD_TEMPERATURE_SENSOR_VOLT] = 1;
				bChagne = 1;
			}		
			#else
			if(Signal_OldValue[ANALOG_HYD_TEMPERATURE] != 0xffff)
			{
				Signal_OldValue[ANALOG_HYD_TEMPERATURE] = 0xffff;		
				Signal_Change[ANALOG_HYD_TEMPERATURE] = 1;
				bChagne = 1;
			}

			// 유압 온도 센서 전압 - 8
			if(Signal_OldValue[ANALOG_HYD_TEMPERATURE_SENSOR_VOLT] != 0xffff)
			{
				Signal_OldValue[ANALOG_HYD_TEMPERATURE_SENSOR_VOLT] = 0xffff;		
				Signal_Change[ANALOG_HYD_TEMPERATURE_SENSOR_VOLT] = 1;
				bChagne = 1;
			}
			#endif		
	

			// 유압 압력  - 9
			if(Signal_OldValue[ANALOG_HYD_PRESSURE] != (unsigned short)(AD_DATA.C_HydPress))
			{
				Signal_OldValue[ANALOG_HYD_PRESSURE] = (unsigned short)(AD_DATA.C_HydPress);		
				Signal_Change[ANALOG_HYD_PRESSURE] = 1;
				bChagne = 1;
			}
			// 유압 압력 센서 전압 - 10

			if((HYD_OIL_PRESSURE >= 1)&&(HYD_OIL_PRESSURE <= 5))
			{
				if(Signal_OldValue[ANALOG_HYD_PRESSURE_SENSOR_VOLT] != (unsigned short)(HYD_OIL_PRESSURE * 100))
				{
					Signal_OldValue[ANALOG_HYD_PRESSURE_SENSOR_VOLT] = (unsigned short)(HYD_OIL_PRESSURE * 100);		
					Signal_Change[ANALOG_HYD_PRESSURE_SENSOR_VOLT] = 1;
					bChagne = 1;
				}
			}
			else
			{
				if(Signal_OldValue[ANALOG_HYD_PRESSURE_SENSOR_VOLT] != 0xffff)
				{
					Signal_OldValue[ANALOG_HYD_PRESSURE_SENSOR_VOLT] = 0xffff;		
					Signal_Change[ANALOG_HYD_PRESSURE_SENSOR_VOLT] = 1;
					bChagne = 1;
				}


			}
		}
	
	}


	// 중량 센서 압력 - 12
	if(Signal_OldValue[ANALOG_WEIGHT_SENSOR_PRESSURE] != (unsigned short)(AD_DATA.C_Re_Function_Bar))
	{
		Signal_OldValue[ANALOG_WEIGHT_SENSOR_PRESSURE] = (unsigned short)(AD_DATA.C_Re_Function_Bar);	
		Signal_Change[ANALOG_WEIGHT_SENSOR_PRESSURE] = 1;
		bChagne = 1;
	}
	// 중량 센서 전압- 13
	if(Signal_OldValue[ANALOG_WEIGHT_SENSOR_VOLT] != (unsigned short)(LOAD_SENSOR* 100))
	{
		Signal_OldValue[ANALOG_WEIGHT_SENSOR_VOLT] = (unsigned short)(LOAD_SENSOR * 100);				
		Signal_Change[ANALOG_WEIGHT_SENSOR_VOLT] = 1;
		bChagne = 1;
	}
	// 중량값 - 14

	if(AD_DATA.Weight_Current != 0xfbff)
	{	
		if(Signal_OldValue[ANALOG_ACCUMULATED_WEIGHT] != AD_DATA.Weight_Current)                // ++, --, 200326 bwk AD_DATA.Weight_Current 추가 
		{

			Signal_OldValue[ANALOG_ACCUMULATED_WEIGHT] = AD_DATA.Weight_Current;
					
			Signal_Change[ANALOG_ACCUMULATED_WEIGHT] = 1;
			bChagne = 1;
		}
	}


	if((InfoModel1.ModelInfo >= MODEL_35D_9S)&&(InfoModel1.ModelInfo <= MODEL_50D_9S))
	{

		// 냉각수 온도 - 15
		if(Signal_OldValue[ANALOG_COOLANT_TEMPERATURE] != RX_ECU_DATA_65262.CoolantTemp_110)
		{
			Signal_OldValue[ANALOG_COOLANT_TEMPERATURE] = RX_ECU_DATA_65262.CoolantTemp_110;		
			Signal_Change[ANALOG_COOLANT_TEMPERATURE] = 1;
			bChagne = 1;
		}
		// 냉각수 센서 저항 - 16	--> 냉각수 센서 전압 //9S
		if(Signal_OldValue[ANALOG_COOLANT_SENSOR_RESISTANCE] != AD_DATA.C_COOLANT_TEMPERATURE)
		{
			Signal_OldValue[ANALOG_COOLANT_SENSOR_RESISTANCE] = AD_DATA.C_COOLANT_TEMPERATURE; 		
			Signal_Change[ANALOG_COOLANT_SENSOR_RESISTANCE] = 1;
			bChagne = 1;
		}

	}
	else
	{
		// 냉각수 온도 - 15
		if(Signal_OldValue[ANALOG_COOLANT_TEMPERATURE] != RX_ECU_DATA_65262.CoolantTemp_110)
		{
			Signal_OldValue[ANALOG_COOLANT_TEMPERATURE] = RX_ECU_DATA_65262.CoolantTemp_110;		
			Signal_Change[ANALOG_COOLANT_TEMPERATURE] = 1;
			bChagne = 1;
		}

	}

	// 연료 게이지 - 17
	if(Engine_Type == DIESEL_TYPE)
	{
		if(Signal_OldValue[ANALOG_FUEL_GAUGE] != HCESPN.Fuel_Level)
		{
			Signal_OldValue[ANALOG_FUEL_GAUGE] = HCESPN.Fuel_Level;					
			Signal_Change[ANALOG_FUEL_GAUGE] = 1;
			bChagne = 1;
		}
		// 연료 센서 저항- 18
		if(Signal_OldValue[ANALOG_FUEL_SENSOR_RESISTANCE] != HCESPN.Fuel_Sensor_Resistor)
		{
			Signal_OldValue[ANALOG_FUEL_SENSOR_RESISTANCE] = HCESPN.Fuel_Sensor_Resistor;				
			Signal_Change[ANALOG_FUEL_SENSOR_RESISTANCE] = 1;
			bChagne = 1;
		}
	}
	// 차량 속도 - 19
	if(Signal_OldValue[ANALOG_VEHICLE_SPEED] != (unsigned short)(Calculate_Speed_Value))
	{
		Signal_OldValue[ANALOG_VEHICLE_SPEED] = (unsigned short)(Calculate_Speed_Value);	
		Signal_Change[ANALOG_VEHICLE_SPEED] = 1;
		bChagne = 1;
	}
	// 속도 센서 주파수 - 20
	if(Signal_OldValue[ANALOG_SPEED_SENSOR_FREQ] != (unsigned short)(TACHO.Value*10)) // 100
	{
		Signal_OldValue[ANALOG_SPEED_SENSOR_FREQ] = (unsigned short)(TACHO.Value*10); // 100	
		Signal_Change[ANALOG_SPEED_SENSOR_FREQ] = 1;
		bChagne = 1;
	}
	// 차량 전압 - 21
	if(Signal_OldValue[ANALOG_VEGICLE_VOLT] != (unsigned short)(BATTERY_VOLT* 100))
	{
		Signal_OldValue[ANALOG_VEGICLE_VOLT] = (unsigned short)(BATTERY_VOLT* 100);				
		Signal_Change[ANALOG_VEGICLE_VOLT] = 1;
		bChagne = 1;
	}
    // 알터 전압 - 22
	if(Signal_OldValue[ANALOG_ALTERNATOR_L] != (unsigned short)(AD_DATA.ALT_VOLT* 100))
	{
		Signal_OldValue[ANALOG_ALTERNATOR_L] = (unsigned short)(AD_DATA.ALT_VOLT* 100);			
		Signal_Change[ANALOG_ALTERNATOR_L] = 1;
		bChagne = 1;
	}
	
	// 차량 주행거리 누계 - 23
	if(Signal_OldValue[ANALOG_TOTAL_MILEAGE] != (Odometer_Value * 10) + (Odometer_Value_cm/10000))
	{
		Signal_OldValue[ANALOG_TOTAL_MILEAGE] =  (Odometer_Value * 10) + (Odometer_Value_cm/10000);
		Signal_Change[ANALOG_TOTAL_MILEAGE] = 1;
		bChagne = 1;
	}
	// 엑셀 페달 - 24
	if((InfoModel1.ModelInfo >= MODEL_35D_9S)&&(InfoModel1.ModelInfo <= MODEL_50D_9S))
	{
		//NOTHING

	}
	else
	{
		if(Signal_OldValue[ANALOG_ACCELERATOR_PEDAL] != RX_ECU_DATA_61443.AcceleratorPedalPosition_91)
		{
			Signal_OldValue[ANALOG_ACCELERATOR_PEDAL] = RX_ECU_DATA_61443.AcceleratorPedalPosition_91;			
			Signal_Change[ANALOG_ACCELERATOR_PEDAL] = 1;
			bChagne = 1;
		}
	}

	
	if(MAST_EQUIPMENT == 1)
	{
		// 기울기 센서 각도 X - 25
		if(Signal_OldValue[ANALOG_TILT_SENSOR_DEGREE_X] != (unsigned short)HCESPN.Mast_Angle_Disp)
		{				
			Signal_OldValue[ANALOG_TILT_SENSOR_DEGREE_X] = (unsigned short)HCESPN.Mast_Angle_Disp ;

			if(Signal_OldValue[ANALOG_TILT_SENSOR_DEGREE_X] == 0xffff)
				Signal_OldValue[ANALOG_TILT_SENSOR_DEGREE_X] = 0;
			
			Signal_Change[ANALOG_TILT_SENSOR_DEGREE_X] = 1;
			bChagne = 1;
		}

	}
	else
	{
		// 기울기 센서 각도 X - 25
		if(Signal_OldValue[ANALOG_TILT_SENSOR_DEGREE_X] != 0xffff)
		{
			Signal_OldValue[ANALOG_TILT_SENSOR_DEGREE_X] = 0xffff;
			Signal_Change[ANALOG_TILT_SENSOR_DEGREE_X] = 1;
			bChagne = 1;
		}

	}

	if(ANGLE_EQUIPMENT == 1)
	{
		// 차량 기울기 X - 26
		if(Signal_OldValue[ANALOG_VEHICLE_TILT_X] != (unsigned short)HCESPN.Vehicle_Angle_X_Disp )
		{
			Signal_OldValue[ANALOG_VEHICLE_TILT_X] = (unsigned short)HCESPN.Vehicle_Angle_X_Disp;		

			if(Signal_OldValue[ANALOG_VEHICLE_TILT_X] == 0xffff)
				Signal_OldValue[ANALOG_VEHICLE_TILT_X] = 0;
			
			Signal_Change[ANALOG_VEHICLE_TILT_X] = 1;
			bChagne = 1;
		}
		// 차량 기울기 Y - 27
		if(Signal_OldValue[ANALOG_VEHICLE_TILT_Y] != (unsigned short)HCESPN.Vehicle_Angle_Y )
		{
			Signal_OldValue[ANALOG_VEHICLE_TILT_Y] = (unsigned short)HCESPN.Vehicle_Angle_Y;			

			if(Signal_OldValue[ANALOG_VEHICLE_TILT_Y] == 0xffff)
				Signal_OldValue[ANALOG_VEHICLE_TILT_Y] = 0;
			
			Signal_Change[ANALOG_VEHICLE_TILT_Y] = 1;
			bChagne = 1;
		}

	}	
	else
	{

		// 차량 기울기 X - 26
		if(Signal_OldValue[ANALOG_VEHICLE_TILT_X] != 0xffff )
		{
			Signal_OldValue[ANALOG_VEHICLE_TILT_X] = 0xffff;			
			Signal_Change[ANALOG_VEHICLE_TILT_X] = 1;
			bChagne = 1;
		}
		// 차량 기울기 Y - 27
		if(Signal_OldValue[ANALOG_VEHICLE_TILT_Y] != 0xffff )
		{
			Signal_OldValue[ANALOG_VEHICLE_TILT_Y] = 0xffff;			
			Signal_Change[ANALOG_VEHICLE_TILT_Y] = 1;
			bChagne = 1;
		}
	}
        

	return bChagne;
}

UCHAR CheckDigitalInputList()
{
	UCHAR bChagne=0;

	#if 0
	if( (InfoModel1.ModelInfo >= MODEL_25LC_9)&&(InfoModel1.ModelInfo <= MODEL_33LC_9) )
	{
	
		if(Signal_OldValue[DIGITAL_TM_TEMPERATURE_SW] != Signal_TM_Oil_Check_lamp)
		{
			Signal_OldValue[DIGITAL_TM_TEMPERATURE_SW] = Signal_TM_Oil_Check_lamp;	
			bChagne = 1;
		}
	}
	else
	{
		if(Signal_OldValue[DIGITAL_TM_TEMPERATURE_SW] != SIGNAL_NO_DISPLAY)
		{
			Signal_OldValue[DIGITAL_TM_TEMPERATURE_SW] = SIGNAL_NO_DISPLAY;	
			bChagne = 1;
		}
	}	
	#else
	if(Signal_OldValue[DIGITAL_TM_TEMPERATURE_SW] != SIGNAL_NO_DISPLAY)
	{
		Signal_OldValue[DIGITAL_TM_TEMPERATURE_SW] = SIGNAL_NO_DISPLAY; 
		bChagne = 1;
	}

	#endif
	
	if(Signal_OldValue[DIGITAL_LEFT_TURN_SW] != Flag_DIN[INDEX_TURN_SIGNAL_LH])
	{
		Signal_OldValue[DIGITAL_LEFT_TURN_SW] = Flag_DIN[INDEX_TURN_SIGNAL_LH];	
		bChagne = 1;
	}
	if(Signal_OldValue[DIGITAL_RIGHT_TURN_SW] != Flag_DIN[INDEX_TURN_SIGNAL_RH])
	{
		Signal_OldValue[DIGITAL_RIGHT_TURN_SW] = Flag_DIN[INDEX_TURN_SIGNAL_RH];	
		bChagne = 1;
	}
	if(Signal_OldValue[DIGITAL_FORWARD_GEAR_SW] != Flag_DIN[INDEX_GEAR_SIGNAL_F])
	{
		Signal_OldValue[DIGITAL_FORWARD_GEAR_SW] = Flag_DIN[INDEX_GEAR_SIGNAL_F];	
		bChagne = 1;
	}
	if(Signal_OldValue[DIGITAL_NEUTRAL_GEAR_SW] != Flag_DIN[INDEX_GEAR_SIGNAL_N])
	{
		Signal_OldValue[DIGITAL_NEUTRAL_GEAR_SW] = Flag_DIN[INDEX_GEAR_SIGNAL_N];		
		bChagne = 1;
	}
	if(Signal_OldValue[DIGITAL_REVERSE_GEAR_SW] != Flag_DIN[INDEX_GEAR_SIGNAL_R])
	{
		Signal_OldValue[DIGITAL_REVERSE_GEAR_SW] = Flag_DIN[INDEX_GEAR_SIGNAL_R];	
		bChagne = 1;
	}


	if(Signal_OldValue[DIGITAL_BUCKLE_SW] != Flag_DIN[INDEX_SW_BUCKLE])
	{
		Signal_OldValue[DIGITAL_BUCKLE_SW] = Flag_DIN[INDEX_SW_BUCKLE];
		bChagne = 1;
	}

	if(Signal_OldValue[DIGITAL_PARKING_PRESSURE_SW] != Flag_DIN[INDEX_PARKING_PRESSURE_SW])
	{
		Signal_OldValue[DIGITAL_PARKING_PRESSURE_SW] = Flag_DIN[INDEX_PARKING_PRESSURE_SW];				
		bChagne = 1;
	}

	if( ((InfoModel1.ModelInfo >= MODEL_35D_9S)&&(InfoModel1.ModelInfo <= MODEL_50D_9S))||
		((InfoModel1.ModelInfo >= MODEL_25L_9A)&&(InfoModel1.ModelInfo <= MODEL_35LN_9A)))
	{
		// NOTHING

	}
	else
	{
	
		if(Signal_OldValue[DIGITAL_ENGINE_WARMUP_SW] != RX_ECU_DATA_65252.WaittoStartLamp_1081)
		{
			Signal_OldValue[DIGITAL_ENGINE_WARMUP_SW] = RX_ECU_DATA_65252.WaittoStartLamp_1081;		
			bChagne = 1;
		}
	}


	if(Signal_OldValue[DIGITAL_ENGINE_CHECK_SW] != Signal_Engine_Check_lamp)
	{
		Signal_OldValue[DIGITAL_ENGINE_CHECK_SW] = Signal_Engine_Check_lamp; 		
		bChagne = 1;
	}
	

	if(Signal_OldValue[DIGITAL_SEAT_LAMP_SW] != Flag_DIN[INDEX_SEAT_SW])
	{
		Signal_OldValue[DIGITAL_SEAT_LAMP_SW] = Flag_DIN[INDEX_SEAT_SW];					
		bChagne = 1;
	}
	if(Signal_OldValue[DIGITAL_TILT_LEVER_SW] != Flag_DIN[INDEX_AUTO_TILT_SIG])
	{
		Signal_OldValue[DIGITAL_TILT_LEVER_SW] = Flag_DIN[INDEX_AUTO_TILT_SIG];				
		bChagne = 1;
	}
	if(Signal_OldValue[DIGITAL_DRIVING_SPEED_SW] != SIGNAL_NO_DISPLAY)
	{
		Signal_OldValue[DIGITAL_DRIVING_SPEED_SW] = SIGNAL_NO_DISPLAY;	
		bChagne = 1;
	}
	//++, 201123 ysm, ENG_OIL
	if(Signal_OldValue[DIGITAL_ENGINE_OIL_PRESSURE_SW] != Signal_Engine_Oil_Check_lamp)
	{
		Signal_OldValue[DIGITAL_ENGINE_OIL_PRESSURE_SW] = Signal_Engine_Oil_Check_lamp;	
		bChagne = 1;
	}
	//--, 201123 ysm, ENG_OIL
	if(Signal_OldValue[DIGITAL_AIR_IN_TAKE_SW] != Flag_DIN[INDEX_AIR_FILTER_WARNING])
	{
		Signal_OldValue[DIGITAL_AIR_IN_TAKE_SW] = Flag_DIN[INDEX_AIR_FILTER_WARNING];				
		bChagne = 1;
	}

	//++, 210714 ysm, 50L-9
	if((InfoModel1.ModelInfo >= MODEL_35L_9)&&(InfoModel1.ModelInfo <= MODEL_33LC_9))
	{
		//NOTHING
		if(Signal_OldValue[DIGITAL_FUELWATER_CONTAMINATION_SW] != SIGNAL_NO_DISPLAY)
		{
			Signal_OldValue[DIGITAL_FUELWATER_CONTAMINATION_SW] = SIGNAL_NO_DISPLAY;	
			bChagne = 1;
		}

	}
	else
	{
		if(Signal_OldValue[DIGITAL_FUELWATER_CONTAMINATION_SW] != Signal_WIF_lamp)
		{
			Signal_OldValue[DIGITAL_FUELWATER_CONTAMINATION_SW] = Signal_WIF_lamp;			
			bChagne = 1;
		}
	}
	//--, 210714 ysm, 50L-9

	if(Signal_OldValue[DIGITAL_BREAK_OIL_LEVEL_SW] != Flag_DIN[INDEX_BRAKE_OIL_LEVEL])
	{
		Signal_OldValue[DIGITAL_BREAK_OIL_LEVEL_SW] = Flag_DIN[INDEX_BRAKE_OIL_LEVEL];			
		bChagne = 1;
	}

	if(Engine_Type == DIESEL_TYPE)
	{
		if((InfoModel1.ModelInfo <= MODEL_35DN_9VB)||((InfoModel1.ModelInfo >= MODEL_25D_9VS)&&(InfoModel1.ModelInfo <= MODEL_35DN_9VS)))
		{	
			if(Signal_OldValue[DIGITAL_ENGINE_PREHEAT_STATUS] != Flag_DIN[INDEX_FUEL_WARMER_SIG])
			{
				Signal_OldValue[DIGITAL_ENGINE_PREHEAT_STATUS] = Flag_DIN[INDEX_FUEL_WARMER_SIG];			
				bChagne = 1;
			}

			if(Signal_OldValue[DIGITAL_DPF_REGEN_SW] != Flag_DIN[INDEX_PARKED_REGENERATION_SIG])
			{
				Signal_OldValue[DIGITAL_DPF_REGEN_SW] = Flag_DIN[INDEX_PARKED_REGENERATION_SIG];			
				bChagne = 1;
			}
		}
		

		if((InfoModel1.ModelInfo >= MODEL_35D_9S)&&(InfoModel1.ModelInfo <= MODEL_50D_9S))
		{
			//++, 210617 ysm, 9S
			#if 0
			if(Signal_OldValue[DIGITAL_DPF_REGEN_SW] != Flag_DIN[INDEX_BRAKE_SW_SIG]) //DPF REGEN -> BRAKE SW
			{
				Signal_OldValue[DIGITAL_DPF_REGEN_SW] = Flag_DIN[INDEX_BRAKE_SW_SIG];			
				bChagne = 1;
			}
			#endif
			//--, 210617 ysm, 9S	
					
			if(Signal_OldValue[DIGITAL_DPF_INHIBIT_SW] != Flag_DIN[INDEX_GEAR_SELECT_SIG])
			{
				Signal_OldValue[DIGITAL_DPF_INHIBIT_SW] = Flag_DIN[INDEX_GEAR_SELECT_SIG];			
				bChagne = 1;
			}

		}
		else
		{
			if((InfoModel1.ModelInfo >= MODEL_25D_9HDI)&&(InfoModel1.ModelInfo <= MODEL_50DN_9HDI))
			{
				// NOTHING
			}
			else
			{
				if(Signal_OldValue[DIGITAL_DPF_INHIBIT_SW] != Flag_DIN[INDEX_INHIBIT_REGENERATION_SIG])
				{
					Signal_OldValue[DIGITAL_DPF_INHIBIT_SW] = Flag_DIN[INDEX_INHIBIT_REGENERATION_SIG];			
					bChagne = 1;
				}

			}
		}
	}
	else //++,--, 201019 ysm
	{

		#if 0
		if(Signal_OldValue[DIGITAL_DPF_REGEN_SW] != Flag_DIN[INDEX_FUEL_EMPTY_SIG]) // DPF REGEN SW -> LPG EMPTY
		{
			Signal_OldValue[DIGITAL_DPF_REGEN_SW] = Flag_DIN[INDEX_FUEL_EMPTY_SIG];			
			bChagne = 1;
		}
		#else

		if(Signal_OldValue[DIGITAL_DPF_REGEN_SW] != Flag_DIN[INDEX_BRAKE_SW_SIG]) // DPF REGEN SW -> BRAKE SW
		{
			Signal_OldValue[DIGITAL_DPF_REGEN_SW] = Flag_DIN[INDEX_BRAKE_SW_SIG];
			bChagne = 1;
		}
				
		if(Signal_OldValue[DIGITAL_PWD_STD_MODEL_SEL] != Flag_DIN[INDEX_FUEL_EMPTY_SIG]) // DPF REGEN SW -> LPG EMPTY
		{
			Signal_OldValue[DIGITAL_PWD_STD_MODEL_SEL] = Flag_DIN[INDEX_FUEL_EMPTY_SIG];			
			bChagne = 1;
		}
		#endif	

	
		if(InfoModel1.ModelInfo < MODEL_25LC_9)
		{
	
			if(Signal_OldValue[DIGITAL_DPF_INHIBIT_SW] != Flag_DIN[INDEX_GEAR_SELECT_SIG])
			{
				Signal_OldValue[DIGITAL_DPF_INHIBIT_SW] = Flag_DIN[INDEX_GEAR_SELECT_SIG];			
				bChagne = 1;
			}
		}

	}
	
	if(Engine_Type == DIESEL_TYPE) //++,--, 201019 ysm
	{
		if((InfoModel1.ModelInfo <= MODEL_35DN_9VB)||(InfoModel1.ModelInfo >= MODEL_25D_9VS))
		{
		
			if(Signal_OldValue[DIGITAL_PWD_STD_MODEL_SEL] != Flag_DIN[INDEX_PWD_STD_MODE_SEL])
			{
				Signal_OldValue[DIGITAL_PWD_STD_MODEL_SEL] = Flag_DIN[INDEX_PWD_STD_MODE_SEL];			
				bChagne = 1;
			}
		}
	}
	#if 0
	if(Signal_OldValue[DIGITAL_ENGINE_WARMUP_STATUS] != SIGNAL_NO_DISPLAY)
	{
		Signal_OldValue[DIGITAL_ENGINE_WARMUP_STATUS] = SIGNAL_NO_DISPLAY;					
		bChagne = 1;
	}
	if(Signal_OldValue[DIGITAL_AUTO_LEVEL] != SIGNAL_NO_DISPLAY)
	{
		Signal_OldValue[DIGITAL_AUTO_LEVEL] = SIGNAL_NO_DISPLAY;					
		bChagne = 1;
	}
	if(Signal_OldValue[DIGITAL_BRAKE_LAMP_STATUS] != SIGNAL_NO_DISPLAY)
	{
		Signal_OldValue[DIGITAL_BRAKE_LAMP_STATUS] = SIGNAL_NO_DISPLAY;
		bChagne = 1;
	}
	if(Signal_OldValue[DIGITAL_BREAK_AUTO_SHIFT_STATUS] != SIGNAL_NO_DISPLAY)
	{
		Signal_OldValue[DIGITAL_BREAK_AUTO_SHIFT_STATUS] = SIGNAL_NO_DISPLAY;
		bChagne = 1;
	}		
	if(Signal_OldValue[DIGITAL_LPG_PRESSURE_SW] != SIGNAL_NO_DISPLAY)
	{
		Signal_OldValue[DIGITAL_LPG_PRESSURE_SW] = SIGNAL_NO_DISPLAY;				
		bChagne = 1;
	}
	#endif

	return bChagne;
}

UCHAR CheckDigitalOutputList()
{
	UCHAR bChagne=0;
	
	if(CurserIndex == 0)
	{
		//++, 221018 ysm, FSCU_3
		if(COUNT_FLAG.Flag_FSCU_Enable == 1)
		{		

			if(RX_FSCU_DATA_65525.Parking_Cut_SV > 1)
				RX_FSCU_DATA_65525.Parking_Cut_SV = 0;
		
			if(Signal_OldValue[0] != RX_FSCU_DATA_65525.Parking_Cut_SV)
			{
				Signal_OldValue[0] = RX_FSCU_DATA_65525.Parking_Cut_SV;
				bChagne = 1;
			}
		}
		else
		{
			if(Signal_OldValue[0] != DOUT_DATA.AUTO_PARKING_OUTPUT)
			{
				Signal_OldValue[0] = DOUT_DATA.AUTO_PARKING_OUTPUT;
				bChagne = 1;
			}
		}
		//--, 221018 ysm, FSCU_3
		

		//++, 230519 ysm, FSCU_HAC
		#if 0
		if(Signal_OldValue[1] != (DOUT_DATA.TRAVEL_CUT_OUTPUT|Flag_DIN[INDEX_PARKING_PRESSURE_SW]))
		{
			Signal_OldValue[1] = (DOUT_DATA.TRAVEL_CUT_OUTPUT|Flag_DIN[INDEX_PARKING_PRESSURE_SW]);
			bChagne = 1;
		}
		#else
		if(Signal_OldValue[1] != DOUT_DATA.TRAVEL_CUT_OUTPUT)
		{
			Signal_OldValue[1] = DOUT_DATA.TRAVEL_CUT_OUTPUT;
			bChagne = 1;
		}		
		#endif
		//--, 230519 ysm, FSCU_HAC
		
	}
	else if(CurserIndex == 1)
	{
		//++, 221018 ysm, FSCU_3
		if(COUNT_FLAG.Flag_FSCU_Enable == 1)
		{		

			if(RX_FSCU_DATA_65525.Attach_Cut_SV > 1)
				RX_FSCU_DATA_65525.Attach_Cut_SV = 0;
		
			if(Signal_OldValue[2] != RX_FSCU_DATA_65525.Attach_Cut_SV)
			{
				Signal_OldValue[2] = RX_FSCU_DATA_65525.Attach_Cut_SV;
				bChagne = 1;
			}

		}
		else
		{
			if(Signal_OldValue[2] != DOUT_DATA.ATTACH_CUT_OUTPUT)
			{
				Signal_OldValue[2] = DOUT_DATA.ATTACH_CUT_OUTPUT;
				bChagne = 1;
			}

		}	
		//--, 221018 ysm, FSCU_3
		
		if(Signal_OldValue[3] != DOUT_DATA.ANTI_RESTART_OUTPUT)
		{
			Signal_OldValue[3] = DOUT_DATA.ANTI_RESTART_OUTPUT;
			bChagne = 1;
		}
	}
	else
	{

		if((Engine_Type == LPG_TYPE)||((InfoModel1.ModelInfo >= MODEL_35D_9S)&&(InfoModel1.ModelInfo <= MODEL_50D_9S))||((InfoModel1.ModelInfo >= MODEL_35D_9HDI)&&(InfoModel1.ModelInfo <= MODEL_50DN_9HDI)))
		{

			if((InfoModel1.ModelInfo >= MODEL_25L_9A)&&(InfoModel1.ModelInfo <= MODEL_35LN_9A))
			{

				if(Signal_OldValue[4] != DOUT_DATA.HYD_AUTO_REGENERATION)
				{
					Signal_OldValue[4] = DOUT_DATA.HYD_AUTO_REGENERATION;
					bChagne = 1;
				}		

				//++, 230209 ysm, FSCU				
				if(Signal_OldValue[5] != (HCESPN.Gear_542<<4|(DOUT_DATA.TRAVEL_CUT_OUTPUT<<2)|Flag_DIN[INDEX_PARKING_PRESSURE_SW]))
				{
					Signal_OldValue[5] = (HCESPN.Gear_542<<4|(DOUT_DATA.TRAVEL_CUT_OUTPUT<<2)|Flag_DIN[INDEX_PARKING_PRESSURE_SW]);
					bChagne = 1;
				}	
				//--, 230209 ysm, FSCU

			}
			else
			{

				if((InfoModel1.ModelInfo >= MODEL_35D_9S)&&(InfoModel1.ModelInfo <= MODEL_50D_9S))
				{
					
					if(Signal_OldValue[4] != DOUT_DATA.HYD_AUTO_REGENERATION)
					{
						Signal_OldValue[4] = DOUT_DATA.HYD_AUTO_REGENERATION;
						bChagne = 1;
					}		

					if(Signal_OldValue[5] != DOUT_DATA.RELAY_SHIFT_DOUT7)
					{
						Signal_OldValue[5] = DOUT_DATA.RELAY_SHIFT_DOUT7;
						bChagne = 1;
					}

				
				}
				else if((InfoModel1.ModelInfo >= MODEL_25LC_9)&&(InfoModel1.ModelInfo <= MODEL_33LC_9))
				{
					//++, 230209 ysm, FSCU				
					if(Signal_OldValue[4] != (HCESPN.Gear_542<<4|(DOUT_DATA.TRAVEL_CUT_OUTPUT<<2)|Flag_DIN[INDEX_PARKING_PRESSURE_SW]))
					{
						Signal_OldValue[4] = (HCESPN.Gear_542<<4|(DOUT_DATA.TRAVEL_CUT_OUTPUT<<2)|Flag_DIN[INDEX_PARKING_PRESSURE_SW]);
						bChagne = 1;
					}	
					//--, 230209 ysm, FSCU

				}
				else
				{
					if(Signal_OldValue[4] != DOUT_DATA.RELAY_SHIFT_DOUT7)
					{
						Signal_OldValue[4] = DOUT_DATA.RELAY_SHIFT_DOUT7;
						bChagne = 1;
					}

					if(Signal_OldValue[5] != DOUT_DATA.RELAY_SHIFT2_DOUT5)
					{
						Signal_OldValue[5] = DOUT_DATA.RELAY_SHIFT2_DOUT5;
						bChagne = 1;
					}

				}			

				//++, 230209 ysm, FSCU				
				if(Signal_OldValue[6] != (HCESPN.Gear_542<<4|(DOUT_DATA.TRAVEL_CUT_OUTPUT<<2)|Flag_DIN[INDEX_PARKING_PRESSURE_SW]))
				{
					Signal_OldValue[6] = (HCESPN.Gear_542<<4|(DOUT_DATA.TRAVEL_CUT_OUTPUT<<2)|Flag_DIN[INDEX_PARKING_PRESSURE_SW]);
					bChagne = 1;
				}	
				//--, 230209 ysm, FSCU

			}
		}
		else
		{

			if(Signal_OldValue[4] != DOUT_DATA.WARNING_BUZZER_SIG)
			{
				Signal_OldValue[4] = DOUT_DATA.WARNING_BUZZER_SIG;
				bChagne = 1;
			}

			if((InfoModel1.ModelInfo >= MODEL_25D_9HDI)&&(InfoModel1.ModelInfo <= MODEL_35DN_9HDI_S))
			{					
				if(Signal_OldValue[5] != (HCESPN.Gear_542<<4|(DOUT_DATA.TRAVEL_CUT_OUTPUT<<2)|Flag_DIN[INDEX_PARKING_PRESSURE_SW]))
				{
					Signal_OldValue[5] = (HCESPN.Gear_542<<4|(DOUT_DATA.TRAVEL_CUT_OUTPUT<<2)|Flag_DIN[INDEX_PARKING_PRESSURE_SW]);
					bChagne = 1;
				}	

			}
			else
			{
				if(Signal_OldValue[5] != DOUT_DATA.HYD_AUTO_REGENERATION)
				{
					Signal_OldValue[5] = DOUT_DATA.HYD_AUTO_REGENERATION;
					bChagne = 1;
				}

				//++, 230209 ysm, FSCU				
				if(Signal_OldValue[6] != (HCESPN.Gear_542<<4|(DOUT_DATA.TRAVEL_CUT_OUTPUT<<2)|Flag_DIN[INDEX_PARKING_PRESSURE_SW]))
				{
					Signal_OldValue[6] = (HCESPN.Gear_542<<4|(DOUT_DATA.TRAVEL_CUT_OUTPUT<<2)|Flag_DIN[INDEX_PARKING_PRESSURE_SW]);
					bChagne = 1;
				}	

				//--, 230209 ysm, FSCU				

			}

		}

	}


	return bChagne;
}

void DisplayOnOff(USHORT list, USHORT y, UCHAR ONOFF)
{
	if(ONOFF == 0)
		PCXtoBMP_16bit(178, y, 19, 19, FL_Image.signal_grey);
	else if((ONOFF == 1) || (ONOFF == 2))
	{
		PCXtoBMP_16bit(178, y, 19, 19, (list == DIGITAL_INPUT_HIGH)?FL_Image.signal_red:FL_Image.signal_green);
	}
	else
	{
		LCD_Draw_Color(178, y, 19, 19, COLOR_BLACK);
		GUI_DisplayCenterStringAt(StringEtc[SIGN_MINUS], 178, y, 19, COLOR_WHITE, 23);
	}
}

void DisplayValue(USHORT y, int Value, UCHAR DisplayType, unsigned short *strUnit)
{
	UCHAR buf[20];
	USHORT covbuf[30];
	unsigned short c = 0;

	int temp_10;

	memset((&buf), 0x0, sizeof( buf));
	memset((&covbuf), 0x0, sizeof(covbuf));

	if(DisplayType == DISPLAY_1)
		sprintf ( (char*)buf, "%d", Value) ; 
	else if((DisplayType == DISPLAY_0_1) || (DisplayType == DISPLAY_SMALL_0_1))
	{
		if(Value < 0)
			sprintf ( (char*)buf, "-%d.%d", ((-1)*Value)/10,((-1)*Value)%10) ; 
		else
			sprintf ( (char*)buf, "%d.%d", Value/10,Value%10) ; 
	}
	else if(DisplayType == DISPLAY_0_4)
		sprintf ( (char*)buf, "%d.%d", (Value*4)/10,(Value*4)%10) ; 
	else if(DisplayType == DISPLAY_0_01)
	{
		if(Value < 0)
			sprintf ( (char*)buf, "%d.%02d", ((-1)*Value)/100, ((-1)*Value)%100) ; 
		else
			sprintf ( (char*)buf, "%d.%02d", Value/100, Value%100) ; 
	}

	#if 0	
	else if(DisplayType == DISPLAY_0_0002)
		sprintf ( (char*)buf, "%d", (int)((float)(Value*0.002)-64)) ; 
	#endif

	else if(DisplayType == DISPLAY_0_0002)
	{
#if 0
		temp_10 = (int)((float)(Value*0.02)-640)/10;
		temp_1 = (char)((float)(Value*0.02)-640)%10;

		if(temp_1<0)	temp_1 = temp_1 * (-1);

		sprintf ( (char*)buf, "%d.%d", temp_10,temp_1) ;
#else
		temp_10 = (int)((float)(Value*0.2)-6400);
		
		if(temp_10 < 0)
		{
			if((( ((-1)*temp_10)/100) == 0) && ((((-1)*temp_10/10)%10) == 0))
				sprintf ( (char*)buf, "0.0"); 
			else
				sprintf ( (char*)buf, "-%d.%d", ((-1)*temp_10)/100,((-1)*temp_10/10)%10); 
		}
		else
			sprintf ( (char*)buf, "%d.%d",temp_10/100,(temp_10/10)%10) ; 
#endif		
	}

	String_Func(covbuf, buf);
	char length = strlen((char*)buf);

	covbuf[length++] = 0x20;

	covbuf[length++] = (*strUnit++);
	c = covbuf[length-1];

	while((c != 0))
	{
		covbuf[length++] = (*strUnit++);
		c = covbuf[length-1];
	}
	
	LCD_Draw_Color(6, y, 188, 23, COLOR_MENU_GRAY);
	GUI_DisplayRightStringAt(&covbuf[0], 6, y,  188, COLOR_WHITE, 23);
}


void DisplayOdometer_Data(USHORT y, unsigned short *strUnit)
{
	UCHAR buf[20];
	USHORT covbuf[30];
	UINT32 tempOdometer;
	unsigned short c = 0;
	
	memset((&buf), 0x0, sizeof( buf));
	memset((&covbuf), 0x0, sizeof( covbuf));

	tempOdometer = Signal_OldValue[ANALOG_TOTAL_MILEAGE];

	if(InfoDisplaySetting.SpeedUnit == UNIT_SPEED_KMH)
	{
		sprintf ( (char*)buf, "%d.%d",tempOdometer/10,tempOdometer%10) ; 
	}
	else
	{
		 sprintf ( (char*)buf, "%d",(int)((float)tempOdometer* 0.0621371)) ; 
	}

	String_Func(covbuf, buf);
	char length = strlen((char*)buf);

	covbuf[length++] = 0x20;

	covbuf[length++] = (*strUnit++);
	c = covbuf[length-1];

	while((c != 0))
	{
		covbuf[length++] = (*strUnit++);
		c = covbuf[length-1];
	}
		
	LCD_Draw_Color(6, y, 188, 23, COLOR_MENU_GRAY);
	GUI_DisplayRightStringAt(&covbuf[0], 6, y,  188, COLOR_WHITE, 23);
}


void DisplayTemperature(USHORT y, USHORT value, USHORT offset)
{
	int nTemp;

	if(value < 0xFA)
	{
		nTemp = value-offset;
		if(InfoDisplaySetting.TemperatureUnit== UNIT_TEMP_F){
			nTemp *= 18;
			nTemp = nTemp / 10;
			nTemp += 32;
		}		
	}
	else
		nTemp = 0;

	DisplayValue(y, nTemp, DISPLAY_1, StringEtc[6+InfoDisplaySetting.TemperatureUnit]);
}

void DisplaySpeedData(USHORT y, unsigned short *strUnit)
{
	USHORT nTemp;
	if(Calculate_Speed_Value<= 999)
	{
		nTemp = (USHORT)Calculate_Speed_Value;
		if(InfoDisplaySetting.SpeedUnit == UNIT_SPEED_MPH)
			nTemp = (int)((float)nTemp* 0.621371);
	}
	else
		nTemp = 0;
	DisplayValue(y, nTemp, DISPLAY_0_1, strUnit);
}

void DisplayPressure(USHORT y, int value)
{
	if(InfoDisplaySetting.PressureUnit == UNIT_PRESSURE_MPA)
		DisplayValue(y, value, DISPLAY_0_1, StringEtc[SIGN_BAR+InfoDisplaySetting.PressureUnit]);
	else if(InfoDisplaySetting.PressureUnit == UNIT_PRESSURE_KGCM)
	{
		if(((int)((float)value* 10.19716)) > 999)
			DisplayValue(y, (int)((float)value* 10.19716), DISPLAY_SMALL_0_1, StringEtc[SIGN_BAR+InfoDisplaySetting.PressureUnit]);
		else
			DisplayValue(y, (int)((float)value* 10.19716), DISPLAY_0_1, StringEtc[SIGN_BAR+InfoDisplaySetting.PressureUnit]);
	}
	else
		DisplayValue(y, value, DISPLAY_1, StringEtc[SIGN_BAR+InfoDisplaySetting.PressureUnit]);
}

// ++, 200326 bwk
void DisplayWeight(USHORT y, int value)
{
	UINT32 tempRound;

	if(InfoDisplaySetting.WeightUnit == UNIT_WEIGHT_TON)
	{
		if(InfoDisplaySetting.WeightDisplayUnit == WEIGHT_DISPLAYSET_100)
		{
			if((value % 10) > 4)
				tempRound = (value + (10-(value % 10)))/10;
			else
				tempRound = value/10;
			
			DisplayValue(y, tempRound, DISPLAY_0_1, StringEtc[SIGN_TON+InfoDisplaySetting.WeightUnit]);
		}
		else
		{
			tempRound = value;
			
			DisplayValue(y, tempRound, DISPLAY_0_01, StringEtc[SIGN_TON+InfoDisplaySetting.WeightUnit]);
		}
		
	}
	else
	{
		tempRound = (UINT32)((float)value* 22.04667);
		DisplayValue(y, tempRound, DISPLAY_1, StringEtc[SIGN_TON+InfoDisplaySetting.WeightUnit]);
    }
        
}
// --, 200326 bwk

void DisplayVehicleAngle(USHORT y, float value, UCHAR DisplayType, unsigned short *strUnit)
{
	UCHAR buf[20];
	USHORT covbuf[30];
	unsigned short c = 0;

	memset((&buf), 0x0, sizeof( buf));
	memset((&covbuf), 0x0, sizeof(covbuf));

	sprintf ( (char*)buf, "%.1f", value/10) ; 

	String_Func(covbuf, buf);
	char length = strlen((char*)buf);

	covbuf[length++] = 0x20;

	covbuf[length++] = (*strUnit++);
	c = covbuf[length-1];

	while((c != 0))
	{
		covbuf[length++] = (*strUnit++);
		c = covbuf[length-1];
	}
	
	LCD_Draw_Color(6, y, 188, 23, COLOR_MENU_GRAY);
	GUI_DisplayRightStringAt(&covbuf[0], 6, y,  188, COLOR_WHITE, 23);        
}


void AddMarqueeTextView(UCHAR nList, UCHAR index)
{
	USHORT y_Pos[]={48,111};

	UCHAR nIndex = m_MarInfo.cntMarText;
	m_MarText[nIndex].uwStartAddress = COLOR_MENU_GRAY;

	m_MarText[nIndex].PictureX = m_MarText[nIndex].StringX = 12;
	m_MarText[nIndex].PictureY = m_MarText[nIndex].StringY = y_Pos[index]+7;
	m_MarText[nIndex].PictureWidth = m_MarText[nIndex].StringWidth = 188;
	m_MarText[nIndex].PictureHeight = m_MarText[nIndex].FontSize = 23;
	m_MarText[nIndex].StringIndex = nList;
	GetStringLength(Signal_StringList[nList], gui_font23, 188, &m_MarText[nIndex].WidthLength, &m_MarText[nIndex].MaxLength);
	
	m_MarText[nIndex].StringColor = COLOR_WHITE;
	m_MarInfo.cntMarText++;
}

void DisplayAnalogList()
{
	USHORT y_Pos[]={48,111};
	
	if(OldScreenIndex != ScreenIndex)
	{
		InitAnalogList();
	}
	
	if((CheckAnalogList() == 1) || (OldCurserIndex != CurserIndex))
	{
		memset((UCHAR *)(&Signal_List), 0x00, sizeof( Signal_List));
		ListCount = 0;
		UCHAR Change = 0;
		
		for(UCHAR i=0;i<MAX_SIGNAL;i++)
		{
			switch(i)
			{
				case ANALOG_EG_RPM: 
					if(Signal_OldValue[i] <= 0xFAFF) 
						Signal_List[ListCount++] = i;
					break;
				case ANALOG_COOLANT_TEMPERATURE:	
					if(Signal_OldValue[i] != 0xFF) 
						Signal_List[ListCount++] = i;
					break;
				default:
					if(Signal_OldValue[i] != 0xFFFF)
						Signal_List[ListCount++] = i;
					break;
			}
		}

		if((CurserIndex*NUMBEROFMENULIST) == ListCount)
		{
			DisplayMenuBackgroundBG();
			CurserIndex = CurserIndex -1;
		}

		if(OldScreenIndex != ScreenIndex)
		{
			memcpy((UCHAR *)(&Signal_OldList), (UCHAR *)(&Signal_List), sizeof( Signal_OldList));
		}
		else
		{
			for(UCHAR i=0;i<MAX_SIGNAL;i++)
			{
				if(Signal_List[i] != Signal_OldList[i])
				{
					Signal_OldList[i] = Signal_List[i];
					Change = 1;
				}
			}
		}

		if((OldScreenIndex != ScreenIndex) || (OldCurserIndex != CurserIndex))
		{
			DisplayMenuBackgroundBG();
			DisplayListPage(CurserIndex+1,((ListCount-1)/NUMBEROFMENULIST)+1);
			nOldPageCount = ((ListCount-1)/NUMBEROFMENULIST)+1;

			m_MarInfo.SetInfo = 0;
			m_MarInfo.cntMarText = 0;
		}
		else if(nOldPageCount != ((ListCount-1)/NUMBEROFMENULIST)+1)
		{
			DisplayListPage(CurserIndex+1,((ListCount-1)/NUMBEROFMENULIST)+1);
		}

		for(UCHAR i=0;i<NUMBEROFMENULIST;i++)
		{
			if(i+(CurserIndex*NUMBEROFMENULIST) < ListCount)
			{
				if((OldScreenIndex != ScreenIndex) || (OldCurserIndex != CurserIndex)
					|| (Change == 1))
				{
					unsigned char SetMarquee;
					LCD_Draw_Color(6, y_Pos[i], 200, 59, COLOR_MENU_GRAY);
					SetMarquee = GUI_DisplayLeftStringAt(Signal_StringList[Signal_List[i+(CurserIndex*NUMBEROFMENULIST)]], 12, y_Pos[i]+7, 188, COLOR_WHITE, 23);
					if(((OldScreenIndex != ScreenIndex) || (OldCurserIndex != CurserIndex)) && (SetMarquee))
						AddMarqueeTextView(Signal_List[i+(CurserIndex*NUMBEROFMENULIST)], i);
				}		
				
				if((OldScreenIndex != ScreenIndex) || (OldCurserIndex != CurserIndex)
					|| (Change == 1) || (Signal_Change[Signal_List[i+(CurserIndex*NUMBEROFMENULIST)]] == 1))
				{
					switch(Signal_List[i+(CurserIndex*NUMBEROFMENULIST)])
					{
						// rpm, 저항
						case ANALOG_EG_RPM:				case ANALOG_DPF_LEVEL:		case ANALOG_COOLANT_SENSOR_RESISTANCE:
						case ANALOG_FUEL_GAUGE:			case ANALOG_FUEL_SENSOR_RESISTANCE:
							DisplayValue(y_Pos[i]+35, Signal_OldValue[Signal_List[i+(CurserIndex*NUMBEROFMENULIST)]], DISPLAY_1, Unit_StringList[Signal_List[i+(CurserIndex*NUMBEROFMENULIST)]]);
							break;
						case ANALOG_TM_TEMPERATURE_SENSOR_RESISTANCE:	
							if(((InfoModel1.ModelInfo >= MODEL_35D_9S)&&(InfoModel1.ModelInfo <= MODEL_50D_9S))||((InfoModel1.ModelInfo >= MODEL_25L_9A)&&(InfoModel1.ModelInfo <= MODEL_35LN_9A)))
								DisplayValue(y_Pos[i]+35, Signal_OldValue[Signal_List[i+(CurserIndex*NUMBEROFMENULIST)]], DISPLAY_0_01, Unit_StringList[Signal_List[i+(CurserIndex*NUMBEROFMENULIST)]]);
							else								
								DisplayValue(y_Pos[i]+35, Signal_OldValue[Signal_List[i+(CurserIndex*NUMBEROFMENULIST)]], DISPLAY_1, Unit_StringList[Signal_List[i+(CurserIndex*NUMBEROFMENULIST)]]);
							break;
						// %
						case ANALOG_DPF_SOOT_LOAD_PERCENT: 
						// %(100.0)
							DisplayValue(y_Pos[i]+35, (Signal_OldValue[Signal_List[i+(CurserIndex*NUMBEROFMENULIST)]]>1000)?0:Signal_OldValue[Signal_List[i+(CurserIndex*NUMBEROFMENULIST)]], DISPLAY_0_1, Unit_StringList[Signal_List[i+(CurserIndex*NUMBEROFMENULIST)]]);
                                                        break;
						// ++, --, 200320 bwk ANALOG_ACCELERATOR_PEDAL : x0.4 미적용 (ANALOG_DPF_SOOT_LOAD_PERCENT와 동일하게 되어 있었음)
						case ANALOG_ACCELERATOR_PEDAL:  
						// %(*0.4)
							DisplayValue(y_Pos[i]+35, Signal_OldValue[Signal_List[i+(CurserIndex*NUMBEROFMENULIST)]], DISPLAY_0_4, Unit_StringList[Signal_List[i+(CurserIndex*NUMBEROFMENULIST)]]);
							break;
						// 온도				
						case ANALOG_TM_TEMPERATURE: 		case ANALOG_COOLANT_TEMPERATURE:	// offset -40
						case ANALOG_HYD_TEMPERATURE:
							DisplayTemperature(y_Pos[i]+35, Signal_OldValue[Signal_List[i+(CurserIndex*NUMBEROFMENULIST)]], 40);
							break;
						// 전압(* 0.01)
						case ANALOG_TM_PRESSURE_SENSOR_VOLT:	
						case ANALOG_HYD_TEMPERATURE_SENSOR_VOLT:	case ANALOG_HYD_PRESSURE_SENSOR_VOLT:
						case ANALOG_BREAK_PRESSURE_SENSOR_VOLT:	case ANALOG_WEIGHT_SENSOR_VOLT:
						case ANALOG_VEGICLE_VOLT:                                  case ANALOG_ALTERNATOR_L:	// ++, --, 200326 bwk ANALOG_ALTERNATOR_L 추가
							DisplayValue(y_Pos[i]+35, Signal_OldValue[Signal_List[i+(CurserIndex*NUMBEROFMENULIST)]], DISPLAY_0_01, Unit_StringList[Signal_List[i+(CurserIndex*NUMBEROFMENULIST)]]);
							break;
						// 압력
						case ANALOG_TM_PRESSURE:	case ANALOG_HYD_PRESSURE:		case ANALOG_WEIGHT_SENSOR_PRESSURE:	case ANALOG_BREAK_PRESSURE:
							DisplayPressure(y_Pos[i]+35, Signal_OldValue[Signal_List[i+(CurserIndex*NUMBEROFMENULIST)]]);
							break;
						// 무게
						case ANALOG_ACCUMULATED_WEIGHT:
							DisplayWeight(y_Pos[i]+35, Signal_OldValue[Signal_List[i+(CurserIndex*NUMBEROFMENULIST)]]);     // ++, --, 200326 bwk
							break;
						// 차량 속도 
						case ANALOG_VEHICLE_SPEED:	
							DisplaySpeedData(y_Pos[i]+35, Unit_StringList[Signal_List[i+(CurserIndex*NUMBEROFMENULIST)]]);
							break;
						// 주파수 
						case ANALOG_SPEED_SENSOR_FREQ:
							//DisplayValue(y_Pos[i]+35, Signal_OldValue[Signal_List[i+(CurserIndex*NUMBEROFMENULIST)]], DISPLAY_0_01, Unit_StringList[Signal_List[i+(CurserIndex*NUMBEROFMENULIST)]]);
							DisplayValue(y_Pos[i]+35, Signal_OldValue[Signal_List[i+(CurserIndex*NUMBEROFMENULIST)]], DISPLAY_0_1, Unit_StringList[Signal_List[i+(CurserIndex*NUMBEROFMENULIST)]]);
							break;
						// odometer
						case ANALOG_TOTAL_MILEAGE:
							DisplayOdometer_Data(y_Pos[i]+35,Unit_StringList[Signal_List[i+(CurserIndex*NUMBEROFMENULIST)]]);
							break;
						// 기울기 센서 각도 X
						case ANALOG_TILT_SENSOR_DEGREE_X:
							DisplayVehicleAngle(y_Pos[i]+35, HCESPN.Mast_Angle_Disp, DISPLAY_0_1, Unit_StringList[Signal_List[i+(CurserIndex*NUMBEROFMENULIST)]]);
							break;
						// 차량 기울기 X, Y
						case ANALOG_VEHICLE_TILT_X:	
							DisplayVehicleAngle(y_Pos[i]+35, HCESPN.Vehicle_Angle_X_Disp, DISPLAY_0_1, Unit_StringList[Signal_List[i+(CurserIndex*NUMBEROFMENULIST)]]);
							break;
						case ANALOG_VEHICLE_TILT_Y:	
							DisplayVehicleAngle(y_Pos[i]+35, HCESPN.Vehicle_Angle_Y, DISPLAY_0_1, Unit_StringList[Signal_List[i+(CurserIndex*NUMBEROFMENULIST)]]);
							break;
					}
				}

			}
			else
				break;
		}
		if((OldScreenIndex != ScreenIndex) || (OldCurserIndex != CurserIndex))
			m_MarInfo.SetInfo = 1;
	}
	
	if((OldScreenIndex != ScreenIndex) || (OldCurserIndex != CurserIndex))
	{
		OldCurserIndex = CurserIndex;
		DrawMenuFull();
	}
}

void DisplayDigitalInputList()
{
	USHORT y_Pos[]={48,111};
	if(OldScreenIndex != ScreenIndex)
	{
		InitDigitalInputList();
	}
	
	if((CheckDigitalInputList() == 1) || (OldCurserIndex != CurserIndex))
	{
		UCHAR Change = 0;
		memset((UCHAR *)(&Signal_List), 0x00, sizeof( Signal_List));
		ListCount = 0;
		
		for(UCHAR i=0;i<DIGITAL_INPUT_TOTAL;i++)
		{
			if(i == DIGITAL_ENGINE_CHECK_SW)
			{
				if(Signal_OldValue[i] <= 2)
				{
					Signal_List[ListCount++] = i;
				}
			}
			else if(Signal_OldValue[i] <= 1)
			{
				Signal_List[ListCount++] = i;
			}
		}

		if((CurserIndex*NUMBEROFMENULIST) == ListCount)
		{
			DisplayMenuBackgroundBG();
			CurserIndex = CurserIndex -1;
		}

		if(OldScreenIndex != ScreenIndex) 
		{
			memcpy((UCHAR *)(&Signal_OldList), (UCHAR *)(&Signal_List), sizeof( Signal_OldList));
		}
		else
		{
			for(UCHAR i=0;i<MAX_SIGNAL;i++)
			{
				if(Signal_List[i] != Signal_OldList[i])
				{
					Signal_OldList[i] = Signal_List[i];
					Change = 1;
				}
			}
		}

		if((OldScreenIndex != ScreenIndex) || (OldCurserIndex != CurserIndex))
		{
			DisplayMenuBackgroundBG();
			DisplayListPage(CurserIndex+1,((ListCount-1)/NUMBEROFMENULIST)+1);
			nOldPageCount = ((ListCount-1)/NUMBEROFMENULIST)+1;

			m_MarInfo.SetInfo = 0;
			m_MarInfo.cntMarText = 0;
		}

		
		for(UCHAR i=0;i<NUMBEROFMENULIST;i++)
		{
			if(i+(CurserIndex*NUMBEROFMENULIST) < ListCount)
			{
				if((OldScreenIndex != ScreenIndex) || (OldCurserIndex != CurserIndex) || (Change == 1))
				{
					unsigned char SetMarquee;
					LCD_Draw_Color(6, y_Pos[i], 200, 59, COLOR_MENU_GRAY);
					SetMarquee = GUI_DisplayLeftStringAt(Signal_StringList[Signal_List[i+(CurserIndex*NUMBEROFMENULIST)]], 12, y_Pos[i]+7, 188, COLOR_WHITE, 23);
					if(((OldScreenIndex != ScreenIndex) || (OldCurserIndex != CurserIndex)) && (SetMarquee))
						AddMarqueeTextView(Signal_List[i+(CurserIndex*NUMBEROFMENULIST)], i);
				}
				DisplayOnOff( Unit_List[Signal_List[i+(CurserIndex*NUMBEROFMENULIST)]], y_Pos[i]+35, Signal_OldValue[Signal_List[i+(CurserIndex*NUMBEROFMENULIST)]]);
			}
			else
				break;
		}
		if((OldScreenIndex != ScreenIndex) || (OldCurserIndex != CurserIndex))
			m_MarInfo.SetInfo = 1;
	}

	if((OldScreenIndex != ScreenIndex) || (OldCurserIndex != CurserIndex))
	{
		OldCurserIndex = CurserIndex;
		DrawMenuFull();
	}
}


void DisplayDigitalOutputList()
{
	USHORT y_Pos[]={48,111};
	
	if(OldScreenIndex != ScreenIndex)
		InitDigitalOutputList();
	
	if((CheckDigitalOutputList() == 1) || ((OldScreenIndex != ScreenIndex) || (OldCurserIndex != CurserIndex)))
	{
		if((OldScreenIndex != ScreenIndex) || (OldCurserIndex != CurserIndex))
		{
			DisplayMenuBackgroundBG();
			DisplayListPage(CurserIndex+1,((ListCount-1)/NUMBEROFMENULIST)+1);

			m_MarInfo.SetInfo = 0;
			m_MarInfo.cntMarText = 0;
		}

		//++, 230209 ysm, FSCU
		if((InfoModel1.ModelInfo >= MODEL_25LC_9)&&(InfoModel1.ModelInfo <= MODEL_33LC_9)&&(CurserIndex == 2))
		{
					
				if((OldScreenIndex != ScreenIndex) || (OldCurserIndex != CurserIndex))
				{
					unsigned char SetMarquee;
					LCD_Draw_Color(6, y_Pos[0], 200, 59, COLOR_MENU_GRAY);
					SetMarquee = GUI_DisplayLeftStringAt(Signal_StringList[(CurserIndex*NUMBEROFMENULIST)], 12, y_Pos[0]+7, 188, COLOR_WHITE, 23);
					if(((OldScreenIndex != ScreenIndex) || (OldCurserIndex != CurserIndex)) && (SetMarquee))
							AddMarqueeTextView((CurserIndex*NUMBEROFMENULIST), 0);
				}

				LCD_Draw_Color(6, y_Pos[0]+35, 200, 23, COLOR_MENU_GRAY);
				
				if((Flag_DIN[INDEX_PARKING_PRESSURE_SW] == 1)||(HCESPN.Gear_542 == 0)||(DOUT_DATA.TRAVEL_CUT_OUTPUT == 1))
					GUI_DisplayCenterStringAt(StringEtc[SIGN_NEUTRAL], 50, y_Pos[0]+35, 200, COLOR_WHITE, 23);
				else
				{
					if(HCESPN.Gear_542 == 1)
						GUI_DisplayCenterStringAt(StringEtc[SIGN_FORWARD], 50, y_Pos[0]+35, 200, COLOR_WHITE, 23);
					else if(HCESPN.Gear_542 == 2)
						GUI_DisplayCenterStringAt(StringEtc[SIGN_REVERSE], 50, y_Pos[0]+35, 200, COLOR_WHITE, 23);
					else
						GUI_DisplayCenterStringAt(StringEtc[SIGN_NEUTRAL], 50, y_Pos[0]+35, 200, COLOR_WHITE, 23);
				
				}		

		}
		else if(CurserIndex == 3)
		{

			if((OldScreenIndex != ScreenIndex) || (OldCurserIndex != CurserIndex))
			{
				unsigned char SetMarquee;
				LCD_Draw_Color(6, y_Pos[0], 200, 59, COLOR_MENU_GRAY);
				SetMarquee = GUI_DisplayLeftStringAt(Signal_StringList[(CurserIndex*NUMBEROFMENULIST)], 12, y_Pos[0]+7, 188, COLOR_WHITE, 23);
				if(((OldScreenIndex != ScreenIndex) || (OldCurserIndex != CurserIndex)) && (SetMarquee))
						AddMarqueeTextView((CurserIndex*NUMBEROFMENULIST), 0);
			}
		
			LCD_Draw_Color(6, y_Pos[0]+35, 200, 23, COLOR_MENU_GRAY);		
			
			if((Flag_DIN[INDEX_PARKING_PRESSURE_SW] == 1)||(HCESPN.Gear_542 == 0)||(DOUT_DATA.TRAVEL_CUT_OUTPUT == 1))
				GUI_DisplayCenterStringAt(StringEtc[SIGN_NEUTRAL], 50, y_Pos[0]+35, 200, COLOR_WHITE, 23);
			else
			{
				if(HCESPN.Gear_542 == 1)
					GUI_DisplayCenterStringAt(StringEtc[SIGN_FORWARD], 50, y_Pos[0]+35, 200, COLOR_WHITE, 23);
				else if(HCESPN.Gear_542 == 2)
					GUI_DisplayCenterStringAt(StringEtc[SIGN_REVERSE], 50, y_Pos[0]+35, 200, COLOR_WHITE, 23);
				else
					GUI_DisplayCenterStringAt(StringEtc[SIGN_NEUTRAL], 50, y_Pos[0]+35, 200, COLOR_WHITE, 23);
			
			}

		}
		else
		{
			
			for(UCHAR i=0;i<NUMBEROFMENULIST;i++)
			{
				if((OldScreenIndex != ScreenIndex) || (OldCurserIndex != CurserIndex))
				{
			
					unsigned char SetMarquee;
					LCD_Draw_Color(6, y_Pos[i], 200, 59, COLOR_MENU_GRAY);
					SetMarquee = GUI_DisplayLeftStringAt(Signal_StringList[i+(CurserIndex*NUMBEROFMENULIST)], 12, y_Pos[i]+7, 188, COLOR_WHITE, 23);
					if(((OldScreenIndex != ScreenIndex) || (OldCurserIndex != CurserIndex)) && (SetMarquee))
							AddMarqueeTextView(i+(CurserIndex*NUMBEROFMENULIST), i);
				}
				
				if((InfoModel1.ModelInfo >= MODEL_25D_9HDI)&&(InfoModel1.ModelInfo <= MODEL_35DN_9HDI_S)&&(CurserIndex==2)&&(i==1))
				{
			
					LCD_Draw_Color(6, y_Pos[i]+35, 200, 23, COLOR_MENU_GRAY);			
					
					if((Flag_DIN[INDEX_PARKING_PRESSURE_SW] == 1)||(HCESPN.Gear_542 == 0)||(DOUT_DATA.TRAVEL_CUT_OUTPUT == 1))
						GUI_DisplayCenterStringAt(StringEtc[SIGN_NEUTRAL], 50, y_Pos[i]+35, 200, COLOR_WHITE, 23);
					else
					{
						if(HCESPN.Gear_542 == 1)
							GUI_DisplayCenterStringAt(StringEtc[SIGN_FORWARD], 50, y_Pos[i]+35, 200, COLOR_WHITE, 23);
						else if(HCESPN.Gear_542 == 2)
							GUI_DisplayCenterStringAt(StringEtc[SIGN_REVERSE], 50, y_Pos[i]+35, 200, COLOR_WHITE, 23);
						else
							GUI_DisplayCenterStringAt(StringEtc[SIGN_NEUTRAL], 50, y_Pos[i]+35, 200, COLOR_WHITE, 23);
					
					}
			
				}
				else if((InfoModel1.ModelInfo >= MODEL_25L_9A)&&(InfoModel1.ModelInfo <= MODEL_35LN_9A)&&(CurserIndex==2)&&(i==1))
				{
					LCD_Draw_Color(6, y_Pos[i]+35, 200, 23, COLOR_MENU_GRAY);			
					
					if((Flag_DIN[INDEX_PARKING_PRESSURE_SW] == 1)||(HCESPN.Gear_542 == 0)||(DOUT_DATA.TRAVEL_CUT_OUTPUT == 1))
						GUI_DisplayCenterStringAt(StringEtc[SIGN_NEUTRAL], 50, y_Pos[i]+35, 200, COLOR_WHITE, 23);
					else
					{
						if(HCESPN.Gear_542 == 1)
							GUI_DisplayCenterStringAt(StringEtc[SIGN_FORWARD], 50, y_Pos[i]+35, 200, COLOR_WHITE, 23);
						else if(HCESPN.Gear_542 == 2)
							GUI_DisplayCenterStringAt(StringEtc[SIGN_REVERSE], 50, y_Pos[i]+35, 200, COLOR_WHITE, 23);
						else
							GUI_DisplayCenterStringAt(StringEtc[SIGN_NEUTRAL], 50, y_Pos[i]+35, 200, COLOR_WHITE, 23);
					
					}
			
				}
				else						
				{				
					DisplayOnOff( Unit_List[i+(CurserIndex*NUMBEROFMENULIST)], y_Pos[i]+35, Signal_OldValue[i+(CurserIndex*NUMBEROFMENULIST)]);		
				}
			}
		}
		//--, 230209 ysm, FSCU

		if((OldScreenIndex != ScreenIndex) || (OldCurserIndex != CurserIndex))
			m_MarInfo.SetInfo = 1;
	}

	if((OldScreenIndex != ScreenIndex) || (OldCurserIndex != CurserIndex))
	{
		OldCurserIndex = CurserIndex;
		DrawMenuFull();
	}	
}

void DisplaySignalStatus()
{
	switch(ScreenIndex)
	{
		case SCREEN_STATE_MENU_MAINTENANCE_SIGNALSTATUS_ANALOG_TOP:
		case SCREEN_STATE_MENU_MAINTENANCE_SIGNALSTATUS_DIGITALINPUT_TOP:	
		case SCREEN_STATE_MENU_MAINTENANCE_SIGNALSTATUS_DIGITALOUTPUT_TOP:
			if(OldScreenIndex != ScreenIndex)
			{
				if((OldScreenIndex != SCREEN_STATE_MENU_MAINTENANCE_SIGNALSTATUS_ANALOG_TOP)
					&& (OldScreenIndex != SCREEN_STATE_MENU_MAINTENANCE_SIGNALSTATUS_DIGITALINPUT_TOP)
					&& (OldScreenIndex != SCREEN_STATE_MENU_MAINTENANCE_SIGNALSTATUS_DIGITALOUTPUT_TOP))
				{
					SetListString3(79, 80, 81);
					DisplayListAll(((ScreenIndex&0x00F00000)>>20));
					DrawMenuFull();
				}
				else if(OldScreenIndex != ScreenIndex)
				{
					DisplayList(((ScreenIndex&0x00F00000)>>20));
				}
			}
			break;
		case SCREEN_STATE_MENU_MAINTENANCE_SIGNALSTATUS_ANALOG_LIST:
			DisplayAnalogList();
			break;
		case SCREEN_STATE_MENU_MAINTENANCE_SIGNALSTATUS_DIGITALINPUT_LIST:	
			DisplayDigitalInputList();
			break;
		case SCREEN_STATE_MENU_MAINTENANCE_SIGNALSTATUS_DIGITALOUTPUT_LIST:
			DisplayDigitalOutputList();
			break;
	}
}
