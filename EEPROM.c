#include "FLV_Cluster_APP.h"

#define nop()		asm("nop");

#define EEP_SCL0		HAL_GPIO_WritePin(C_EEP_SCL_GPIO_Port, C_EEP_SCL_Pin, GPIO_PIN_RESET)
#define EEP_SCL1		HAL_GPIO_WritePin(C_EEP_SCL_GPIO_Port, C_EEP_SCL_Pin, GPIO_PIN_SET)

#define EEP_SDA0		HAL_GPIO_WritePin(C_EEP_SDA_GPIO_Port, C_EEP_SDA_Pin, GPIO_PIN_RESET)
#define EEP_SDA1		HAL_GPIO_WritePin(C_EEP_SDA_GPIO_Port, C_EEP_SDA_Pin, GPIO_PIN_SET)

#define EEP_SDA_READ		HAL_GPIO_ReadPin(C_EEP_SDA_GPIO_Port, C_EEP_SDA_Pin)

#define EEP_SDAIN   		EEPROM_PortChange(0)
#define EEP_SDAOUT  		EEPROM_PortChange(1) 

#define EEPROM_CLEAR_ALL				0x01
#define EEPROM_CLEAR_HOURMETER			0x02
#define EEPROM_CLEAR_ODOMETER			0x03
#define EEPROM_CLEAR_DATA				0x04
#define EEPROM_CLEAR_MAINTENANCE		0x05

#define EEPROM_CLEAR_ANGLE_SENSOR		0x10


extern EEPROM_MODEL_DATA1	InfoModel1;
extern EEPROM_MODEL_DATA2	InfoModel2;
extern EEPROM_MODEL_DATA3	InfoModel3;
extern unsigned char			Flag_ESL;

extern CAN_MMI_DATA_43981							RX_MMI_DATA_43981;
extern CAN_MMI_DATA_63023						        RX_MMI_DATA_63023;              // ++, --, 210225 ctw Auto JIG Final Test
extern unsigned long HourMeter_Value;
extern unsigned int Odometer_Value;
extern unsigned int Odometer_Value_cm;

st_Maintenance Maintenance_Data[MAINTENANCE_TOTAL];

extern st_DATA_RTC RTC_Data;
st_DATA_RTC RTC_ESL;

extern unsigned char Engine_Type;

// ++, 210225 ctw Auto JIG Final Test
st_CANDATA_HCEPGN_63279         TX_CANDATA_HCEPGN_63279;

extern CAN_HandleTypeDef hcan1;

extern unsigned char Tab_Number;
unsigned char EepromWrite_ERR_FLAG = 0;
unsigned char Eeprom_All_Clear = 0;
// --, 210225 ctw Auto JIG Final Test

extern unsigned char Equipment_Calibration;

// ++, 210225 ctw Auto JIG Final Test
#if 0
void EEPROM_Reset(void)
{
	unsigned char Data[64];

	for(int i = 0; i < 64; i++)
		Data[i] = 0xFF;
	for(int j = 0; j < 64; j++)
		EepromWrite(j*64, Data, 64);
}
#else
void EEPROM_Reset(void)
{
    unsigned char Data[64] = {0,};

	for(int i = 0; i < 64; i++)
		Data[i] = 0xFF;
#if 0
	for(int j = 0; j < 31; j++)
		EepromWrite(j*64, &Data[0], 64);
        
	for(int j = 32; j < 46; j++)
		EepromWrite(j*64, &Data[0], 64);
        
		EepromWrite(2944, &Data[0], 56);
                
	for(int j = 49; j < 64; j++)
		EepromWrite(j*64, &Data[0], 64);
#else
    for(int j = 0; j < 28; j++)
		EepromWrite(j*64, &Data[0], 64);
	for(int j = 0; j < 32; j++)
		EepromWrite(2048+64*j, &Data[0], 64); //++,--, 220901 ysm		
#endif
}
#endif
// --, 210225 ctw Auto JIG Final Test


void Initialize_E2PROM_Hourmeter_Odometer(void)
{
	UCHAR Data[32]={0,};
	EepromWrite(ADDRESS_HOURMETER0_ERROR_COUNT, (unsigned char*)(&Data[0]), 9);	// 18~27
	EepromWrite(ADDRESS_HOURMETER1_ERROR_COUNT, (unsigned char*)(&Data[0]), 9);	// 18~27
	EepromWrite(ADDRESS_HOURMETER2_ERROR_COUNT, (unsigned char*)(&Data[0]), 9);	// 18~27
	
	TimeDelay_msec(5);
	EepromWrite(ADDRESS_HOURMETER0, (unsigned char*)(&Data[0]), ADDRESS_METER_TOTAL);		// 30 ~ 70
	EepromWrite(ADDRESS_HOURMETER1, (unsigned char*)(&Data[0]), ADDRESS_METER_TOTAL);		// 70 ~ 110
	EepromWrite(ADDRESS_HOURMETER2, (unsigned char*)(&Data[0]), ADDRESS_METER_TOTAL);		// 110 ~ 150
	TimeDelay_msec(5);
	EepromWrite(ADDRESS_ODOMETER_KM0, (unsigned char*)(&Data[0]), ADDRESS_METER_TOTAL);	// 150 ~ 190
	EepromWrite(ADDRESS_ODOMETER_KM1, (unsigned char*)(&Data[0]), ADDRESS_METER_TOTAL);	// 190 ~ 230
	EepromWrite(ADDRESS_ODOMETER_KM2, (unsigned char*)(&Data[0]), ADDRESS_METER_TOTAL);	// 230 ~ 270
	TimeDelay_msec(5);
	EepromWrite(ADDRESS_ODOMETER_CM0, (unsigned char*)(&Data[0]), ADDRESS_METER_TOTAL);	// 256 ~ 287
	EepromWrite(ADDRESS_ODOMETER_CM1, (unsigned char*)(&Data[0]), ADDRESS_METER_TOTAL);	// 288 ~ 319
	EepromWrite(ADDRESS_ODOMETER_CM2, (unsigned char*)(&Data[0]), ADDRESS_METER_TOTAL);	// 320 ~ 351
}

void Initialize_E2PROM_Variable(void)
{
	unsigned char Clear[100]={0,};
	unsigned char TempData[48];

	UCHAR tmpBuf1[5];
	UCHAR tmpBuf2[5];
	UCHAR tmpBuf3[5];
	PASSWORD_DATA* Password_USER;
	PASSWORD_DATA* Password_MASTER;
	PASSWORD_DATA* Password_ESL;

	EEPROM_MODEL_DATA1		ModelData1;
	EEPROM_MODEL_DATA2		ModelData2;
	EEPROM_MODEL_DATA3		ModelData3;
	EEPROM_DISPLAY_DATA	DisplayData;

	memset(&ModelData1, 0x00, sizeof(ModelData1));
	memset(&ModelData2, 0x00, sizeof(ModelData2));
	memset(&ModelData3, 0x00, sizeof(ModelData3));
	memset(&DisplayData, 0x00, sizeof(DisplayData));

	Password_USER = (PASSWORD_DATA*)&tmpBuf1[0];
	Password_MASTER = (PASSWORD_DATA*)&tmpBuf2[0];
	Password_ESL = (PASSWORD_DATA*)&tmpBuf3[0];

	Password_USER->Data1=0;
	Password_USER->Data2=0;
	Password_USER->Data3=0;
	Password_USER->Data4=0;
	Password_USER->Data5=0;

	Password_USER->Data6=0xf;
	Password_USER->Data7=0xf;
	Password_USER->Data8=0xf;
	Password_USER->Data9=0xf;
	Password_USER->Data10=0xf;

	Password_MASTER->Data1=9;
	Password_MASTER->Data2=8;
	Password_MASTER->Data3=2;
	Password_MASTER->Data4=8;
	Password_MASTER->Data5=2;

	Password_MASTER->Data6=0xf;
	Password_MASTER->Data7=0xf;
	Password_MASTER->Data8=0xf;
	Password_MASTER->Data9=0xf;
	Password_MASTER->Data10=0xf;

	Password_ESL->Data1=0;
	Password_ESL->Data2=0;
	Password_ESL->Data3=0;
	Password_ESL->Data4=0;
	Password_ESL->Data5=0;

	Password_ESL->Data6=0xf;
	Password_ESL->Data7=0xf;
	Password_ESL->Data8=0xf;
	Password_ESL->Data9=0xf;
	Password_ESL->Data10=0xf;

	EepromWrite(ADDRESS_USER_PASSWORD, (unsigned char*)(&tmpBuf1[0]), 5);
	EepromWrite(ADDRESS_MASTER_PASSWORD, (unsigned char*)(&tmpBuf2[0]), 5);
	EepromWrite(ADDRESS_ESL_PASSWORD, (unsigned char*)(&tmpBuf3[0]), 5);

	ModelData1.TonInfo = 35;			// BASE MODEL 35L-9
	ModelData1.ModelInfo= MODEL_35L_9;	//// BASE MODEL 35L-9
	ModelData1.DCSR_Status = 0; //++,--, 210826 ysm,ASM
	ModelData1.DCSR_Speed_Block = 5;
	ModelData1.DCSR_Speed_Restore = 3;	
	EepromWrite(ADDRESS_MODEL_INFO, (unsigned char*)(&ModelData1), sizeof(ModelData1));

	ModelData2.HAC_Status = 1; //210708 ysm, oFF -> ON 	
	
	ModelData2.SpeedLimitStatus = 0;
	ModelData2.SpeedLimitValue  = 15;

	ModelData2.ESL_Status = 0;
	ModelData2.ESL_Set_Status = INACTIVE;
	ModelData2.ESL_Interval = DELAYTIME_5MIN;

	ModelData2.ClutchProtectionAlarm = 0;
	
	ModelData2.ZeroStart = 0;
	
	ModelData2.SeatBeltInterlock = 0;

	ModelData2.ZeroStart_AccelValue = 12; //++,--, 230202 ysm, FSCU 7->12
	EepromWrite(ADDRESS_HAC_STATUS, (unsigned char*)(&ModelData2), sizeof(ModelData2));
	TimeDelay_msec(5);

	ModelData3.AutoShift_Status = 1;//++,--, 210819 ysm, 9S
	ModelData3.AutoShift_Status_Speed_UP = 5;
	ModelData3.AutoShift_Status_Speed_DOWN = 3;
	ModelData3.AutoShift_Status_Overlap_Time = 45; //++,--, 210814 ysm

	ModelData3.TireInfo = 0;
	
	ModelData3.AutoShift_Status_SR_Value_UP = 7;
	ModelData3.AutoShift_Status_SR_Value_DOWN= 4;
	ModelData3.AutoShift_Status_Overlap_DownTime = 0;
	
	EepromWrite(ADDRESS_AUTOSHIFT_STATUS, (unsigned char*)(&ModelData3), sizeof(ModelData3));
	TimeDelay_msec(5);	

	DisplayData.LCDBrightnessLevel = 7;
	DisplayData.LEDBrightnessLevel = 7;
	EepromWrite(ADDRESS_LED_BRIGHTNESS_LEVEL, (unsigned char*)(&DisplayData), sizeof(DisplayData));		// brightness : 7
	TimeDelay_msec(5);
	// ADDRESS_DIAGNOSIS
	for(int i = 0 ; i < 11 ; i++)
	{
		EepromWrite((i*64)+ADDRESS_DIAGNOSIS, (unsigned char*)(&Clear[0]), 64);
	}

	for(int i = 0; i<32; i++)
		TempData[i] = 0;
	
	
	for(int i = 0 ; i < 15 ; i++) // 400 BYTES
	{
		EepromWrite(ADDRESS_RCM_START+i*32,  &TempData[0], 32);
	}

	for(int i = 0 ; i < 33 ; i++) // 800 BYTES
	{
		EepromWrite(ADDRESS_RCM3_START+i*32,  &TempData[0], 32);
	}

	//++, 220331 ysm, FINGERTIP
	EepromWrite(ADDRESS_H3510_, (unsigned char*)(&Clear[0]), 5);

	HCESPN.H3510_[0] = 0;
	HCESPN.H3510_[1] = 0;
	HCESPN.H3510_[2] = 0;
	HCESPN.H3510_[3] = 0;
	HCESPN.H3510_[4] = 0;	
	//--, 220331 ysm, FINGERTIP


	TimeDelay_msec(5);
	
	
}
void Reset_E2PROM_Maintenance(void)
{
	unsigned char i,j;

	for (i = 0; i < MAINTENANCE_TOTAL; i++)
	{
		Maintenance_Data[i].Maintenance_Alarm = 0;			
		Maintenance_Data[i].Maintenance_Replacement_Count = 0;
		Maintenance_Data[i].Maintenance_Hourmeter = 0;

		for (j = 0; j < 9; j++)
		{
			Maintenance_Data[i].Maintenance_History[j] = 0;
		}
	}


	if((InfoModel1.ModelInfo >= MODEL_35L_9)&&(InfoModel1.ModelInfo <= MODEL_50L_9))
	{
		Maintenance_Data[MAINTENANCE_ENGINE_OIL_FILTER].Maintenance_Interval = 1; // 초기 50시간
		Maintenance_Data[MAINTENANCE_TRANSMISSION_OIL_FILTER].Maintenance_Interval = 2; // 초기 100시간
	 	Maintenance_Data[MAINTENANCE_DIFFERENTIAL_GEAR_OIL].Maintenance_Interval = 2; // 초기 100시간
		Maintenance_Data[MAINTENANCE_HYD_AIR_BREATHER_ELEMENT].Maintenance_Interval = 10; // 250 -> 500 //++,--, 230104 ysm, BRAKE_OIL
		Maintenance_Data[MAINTENANCE_HYD_OIL_RETURN_FILTER].Maintenance_Interval = 5; // 1000(초기 250시간)
		Maintenance_Data[MAINTENANCE_FUEL_FILTER].Maintenance_Interval = 0xff; // 1000 -> Not Used
		Maintenance_Data[MAINTENANCE_FAN_BELT].Maintenance_Interval = 20; // 1000
		Maintenance_Data[MAINTENANCE_BRAKE_OIL].Maintenance_Interval = 20; // 1000
		Maintenance_Data[MAINTENANCE_AIR_CLEANER_ELEMENT].Maintenance_Interval = 20; // 1000
		Maintenance_Data[MAINTENANCE_HYD_SUCTION_STRAINER].Maintenance_Interval = 40; // 2000
		Maintenance_Data[MAINTENANCE_RADIATIOR_COOLANT].Maintenance_Interval = 80; // 4000
		Maintenance_Data[MAINTENANCE_HYD_OIL].Maintenance_Interval = 100; // 5000
		Maintenance_Data[MAINTENANCE_LPG_VAPORIZER].Maintenance_Interval = 80; //4000
		Maintenance_Data[MAINTENANCE_SPARK_PLUG].Maintenance_Interval = 40;     // 2000 
		Maintenance_Data[MAINTENANCE_LOCK_OFF_VALVE_FILTER].Maintenance_Interval = 40; // 2000
		
		Maintenance_Data[MAINTENANCE_DPF_ASH_CLEANING].Maintenance_Interval = 0xff; // not used
		Maintenance_Data[MAINTENANCE_BRAKE_COOLING_OIL_FILTER].Maintenance_Interval = 0xff; // not used

		
	}
	else if((InfoModel1.ModelInfo >= MODEL_35D_9S)&&(InfoModel1.ModelInfo <= MODEL_50D_9S))
	{

		Maintenance_Data[MAINTENANCE_ENGINE_OIL_FILTER].Maintenance_Interval = 1; // 초기 50시간
		Maintenance_Data[MAINTENANCE_TRANSMISSION_OIL_FILTER].Maintenance_Interval = 2; // 초기 100시간
		Maintenance_Data[MAINTENANCE_DIFFERENTIAL_GEAR_OIL].Maintenance_Interval = 2; // 초기 100시간
		Maintenance_Data[MAINTENANCE_HYD_AIR_BREATHER_ELEMENT].Maintenance_Interval = 5; // 250
		Maintenance_Data[MAINTENANCE_HYD_OIL_RETURN_FILTER].Maintenance_Interval = 5; // 1000(초기 250시간)
		Maintenance_Data[MAINTENANCE_FUEL_FILTER].Maintenance_Interval = 20; // 1000
		Maintenance_Data[MAINTENANCE_FAN_BELT].Maintenance_Interval = 10; // 500
		Maintenance_Data[MAINTENANCE_BRAKE_OIL].Maintenance_Interval = 20; // 1000
		Maintenance_Data[MAINTENANCE_AIR_CLEANER_ELEMENT].Maintenance_Interval = 20; // 1000
		Maintenance_Data[MAINTENANCE_HYD_SUCTION_STRAINER].Maintenance_Interval = 40; // 2000
		Maintenance_Data[MAINTENANCE_RADIATIOR_COOLANT].Maintenance_Interval = 40; // 2000
		Maintenance_Data[MAINTENANCE_HYD_OIL].Maintenance_Interval = 100; // 5000

		Maintenance_Data[MAINTENANCE_LPG_VAPORIZER].Maintenance_Interval = 0xff; //not used
		Maintenance_Data[MAINTENANCE_SPARK_PLUG].Maintenance_Interval = 0xff; 	// not used 
		Maintenance_Data[MAINTENANCE_LOCK_OFF_VALVE_FILTER].Maintenance_Interval = 0xff; // not used
		
		Maintenance_Data[MAINTENANCE_DPF_ASH_CLEANING].Maintenance_Interval = 0xff; // not used
		Maintenance_Data[MAINTENANCE_BRAKE_COOLING_OIL_FILTER].Maintenance_Interval = 0xff; // not used

	}
	else if((InfoModel1.ModelInfo >= MODEL_25LC_9)&&(InfoModel1.ModelInfo <= MODEL_33LC_9))
	{

		Maintenance_Data[MAINTENANCE_ENGINE_OIL_FILTER].Maintenance_Interval = 1; // 초기 50시간
		Maintenance_Data[MAINTENANCE_TRANSMISSION_OIL_FILTER].Maintenance_Interval = 2; // 초기 100시간
	 	Maintenance_Data[MAINTENANCE_DIFFERENTIAL_GEAR_OIL].Maintenance_Interval = 2; // 초기 100시간
		Maintenance_Data[MAINTENANCE_HYD_AIR_BREATHER_ELEMENT].Maintenance_Interval = 20; // 500 -> 1000 //++,--, 230104 ysm, BRAKE_OIL
		Maintenance_Data[MAINTENANCE_HYD_OIL_RETURN_FILTER].Maintenance_Interval = 5; // 1000(초기 250시간)
		
		Maintenance_Data[MAINTENANCE_FUEL_FILTER].Maintenance_Interval = 0xff; // 1000 -> Not Used
		
		Maintenance_Data[MAINTENANCE_FAN_BELT].Maintenance_Interval = 10; // 500
		Maintenance_Data[MAINTENANCE_BRAKE_OIL].Maintenance_Interval = 20; // 1000
		Maintenance_Data[MAINTENANCE_AIR_CLEANER_ELEMENT].Maintenance_Interval = 20; // 1000
		Maintenance_Data[MAINTENANCE_HYD_SUCTION_STRAINER].Maintenance_Interval = 40; // 2000
		Maintenance_Data[MAINTENANCE_RADIATIOR_COOLANT].Maintenance_Interval = 20; // 1000
		Maintenance_Data[MAINTENANCE_HYD_OIL].Maintenance_Interval = 100; // 5000
		
		Maintenance_Data[MAINTENANCE_LPG_VAPORIZER].Maintenance_Interval = 0xff; //not used
		Maintenance_Data[MAINTENANCE_SPARK_PLUG].Maintenance_Interval = 0xff; 	// not used 
		Maintenance_Data[MAINTENANCE_LOCK_OFF_VALVE_FILTER].Maintenance_Interval = 0xff; // not used
		
		Maintenance_Data[MAINTENANCE_DPF_ASH_CLEANING].Maintenance_Interval = 0xff; // not used
		Maintenance_Data[MAINTENANCE_BRAKE_COOLING_OIL_FILTER].Maintenance_Interval = 0xff; // not used

	}
	else if((InfoModel1.ModelInfo >= MODEL_25L_9A)&&(InfoModel1.ModelInfo <= MODEL_35LN_9A))
	{

		Maintenance_Data[MAINTENANCE_ENGINE_OIL_FILTER].Maintenance_Interval = 1; // 초기 50시간
		Maintenance_Data[MAINTENANCE_TRANSMISSION_OIL_FILTER].Maintenance_Interval = 2; // 초기 100시간
	 	Maintenance_Data[MAINTENANCE_DIFFERENTIAL_GEAR_OIL].Maintenance_Interval = 2; // 초기 100시간
		Maintenance_Data[MAINTENANCE_HYD_AIR_BREATHER_ELEMENT].Maintenance_Interval = 5; // 250
		Maintenance_Data[MAINTENANCE_HYD_OIL_RETURN_FILTER].Maintenance_Interval = 5; // 1000(초기 250시간)
		
		Maintenance_Data[MAINTENANCE_FUEL_FILTER].Maintenance_Interval = 20; // 1000			
		
		Maintenance_Data[MAINTENANCE_AIR_CLEANER_ELEMENT].Maintenance_Interval = 10; // 500
		Maintenance_Data[MAINTENANCE_HYD_SUCTION_STRAINER].Maintenance_Interval = 40; // 2000
		Maintenance_Data[MAINTENANCE_RADIATIOR_COOLANT].Maintenance_Interval = 40; // 2000
		Maintenance_Data[MAINTENANCE_HYD_OIL].Maintenance_Interval = 100; // 5000

		Maintenance_Data[MAINTENANCE_BRAKE_COOLING_OIL_FILTER].Maintenance_Interval = 20; // 1000	
	
		Maintenance_Data[MAINTENANCE_FAN_BELT].Maintenance_Interval = 0xff; // not Used
		Maintenance_Data[MAINTENANCE_BRAKE_OIL].Maintenance_Interval = 0xff; // not used
		Maintenance_Data[MAINTENANCE_LPG_VAPORIZER].Maintenance_Interval = 0xff; //not used
		Maintenance_Data[MAINTENANCE_SPARK_PLUG].Maintenance_Interval = 0xff; 	// not used 
		Maintenance_Data[MAINTENANCE_LOCK_OFF_VALVE_FILTER].Maintenance_Interval = 0xff; // not used		
		Maintenance_Data[MAINTENANCE_DPF_ASH_CLEANING].Maintenance_Interval = 0xff; // not used

	}
	else if((InfoModel1.ModelInfo >= MODEL_25D_9HDI)&&(InfoModel1.ModelInfo <= MODEL_50DN_9HDI))
	{

		Maintenance_Data[MAINTENANCE_ENGINE_OIL_FILTER].Maintenance_Interval = 1; // 초기 50시간
		Maintenance_Data[MAINTENANCE_TRANSMISSION_OIL_FILTER].Maintenance_Interval = 2; // 초기 100시간
	 	Maintenance_Data[MAINTENANCE_DIFFERENTIAL_GEAR_OIL].Maintenance_Interval = 2; // 초기 100시간
		Maintenance_Data[MAINTENANCE_HYD_AIR_BREATHER_ELEMENT].Maintenance_Interval = 20; // 500 -> 1000 //++,--, 230104 ysm, BRAKE_OIL
		Maintenance_Data[MAINTENANCE_HYD_OIL_RETURN_FILTER].Maintenance_Interval = 5; // 1000(초기 250시간)		

		//++, 230104 ysm, BRAKE_OIL
		if((InfoModel1.ModelInfo >= MODEL_25D_9HDI)&&(InfoModel1.ModelInfo <= MODEL_35DN_9HDI_S))
			Maintenance_Data[MAINTENANCE_FUEL_FILTER].Maintenance_Interval = 15; // 750		
		else
			Maintenance_Data[MAINTENANCE_FUEL_FILTER].Maintenance_Interval = 20; // 1000
		//--, 230104 ysm, BRAKE_OIL
	
		Maintenance_Data[MAINTENANCE_FAN_BELT].Maintenance_Interval = 40; // 2000
		Maintenance_Data[MAINTENANCE_BRAKE_OIL].Maintenance_Interval = 20; // 1000
		Maintenance_Data[MAINTENANCE_AIR_CLEANER_ELEMENT].Maintenance_Interval = 40; // 2000
		Maintenance_Data[MAINTENANCE_HYD_SUCTION_STRAINER].Maintenance_Interval = 40; // 2000
		Maintenance_Data[MAINTENANCE_RADIATIOR_COOLANT].Maintenance_Interval = 40; // 2000
		Maintenance_Data[MAINTENANCE_HYD_OIL].Maintenance_Interval = 100; // 5000
		
		Maintenance_Data[MAINTENANCE_LPG_VAPORIZER].Maintenance_Interval = 0xff; //not used
		Maintenance_Data[MAINTENANCE_SPARK_PLUG].Maintenance_Interval = 0xff; 	// not used 
		Maintenance_Data[MAINTENANCE_LOCK_OFF_VALVE_FILTER].Maintenance_Interval = 0xff; // not used
		
		Maintenance_Data[MAINTENANCE_DPF_ASH_CLEANING].Maintenance_Interval = 0xff; // not used
		Maintenance_Data[MAINTENANCE_BRAKE_COOLING_OIL_FILTER].Maintenance_Interval = 0xff; // not used


	}
	else  //9VS, 9V
	{
		Maintenance_Data[MAINTENANCE_ENGINE_OIL_FILTER].Maintenance_Interval = 1; // 초기 50시간
		Maintenance_Data[MAINTENANCE_TRANSMISSION_OIL_FILTER].Maintenance_Interval = 2; // 초기 100시간
		Maintenance_Data[MAINTENANCE_DIFFERENTIAL_GEAR_OIL].Maintenance_Interval = 2; // 초기 100시간		
		Maintenance_Data[MAINTENANCE_HYD_AIR_BREATHER_ELEMENT].Maintenance_Interval = 10; // 500
		Maintenance_Data[MAINTENANCE_HYD_OIL_RETURN_FILTER].Maintenance_Interval = 5; // 1000(초기 250시간)
		Maintenance_Data[MAINTENANCE_FUEL_FILTER].Maintenance_Interval = 15; // 750
		Maintenance_Data[MAINTENANCE_FAN_BELT].Maintenance_Interval = 40; // 2000
		Maintenance_Data[MAINTENANCE_BRAKE_OIL].Maintenance_Interval = 20; // 1000
		Maintenance_Data[MAINTENANCE_AIR_CLEANER_ELEMENT].Maintenance_Interval = 40; // 2000
		Maintenance_Data[MAINTENANCE_HYD_SUCTION_STRAINER].Maintenance_Interval = 40; // 2000
		Maintenance_Data[MAINTENANCE_RADIATIOR_COOLANT].Maintenance_Interval = 40; // 2000
		Maintenance_Data[MAINTENANCE_HYD_OIL].Maintenance_Interval = 100; // 5000
		
		Maintenance_Data[MAINTENANCE_LPG_VAPORIZER].Maintenance_Interval = 0xff; //not used
		Maintenance_Data[MAINTENANCE_SPARK_PLUG].Maintenance_Interval = 0xff; 	// not used 
		Maintenance_Data[MAINTENANCE_LOCK_OFF_VALVE_FILTER].Maintenance_Interval = 0xff; // not used

		Maintenance_Data[MAINTENANCE_DPF_ASH_CLEANING].Maintenance_Interval = 60; // 3000
		Maintenance_Data[MAINTENANCE_BRAKE_COOLING_OIL_FILTER].Maintenance_Interval = 0xff; // not used

	}


	for(i = 0 ; i < MAINTENANCE_TOTAL ; i++)
	{
		EepromWrite((i*32)+ADDRESS_MAINTENANCE, (unsigned char*)(&Maintenance_Data[i]), 32);
	}
	TimeDelay_msec(5);
}

void Initialize_E2PROM_Maintenance(void)
{
	unsigned char i,j;

    EepromWrite_ERR_FLAG = 1;       // ++, --, 210225 ctw Auto JIG Final Test
        
	for (i = 0; i < MAINTENANCE_TOTAL; i++)
	{
		Maintenance_Data[i].Maintenance_Alarm = 0;			
		Maintenance_Data[i].Maintenance_Replacement_Count = 0;
		Maintenance_Data[i].Maintenance_Hourmeter = 0;

		for (j = 0; j < 9; j++)
		{
			Maintenance_Data[i].Maintenance_History[j] = 0;
		}
	}

	//++, 230104 ysm, BRAKE_OIL
	Maintenance_Data[MAINTENANCE_ENGINE_OIL_FILTER].Maintenance_Interval = 1; // 초기 50시간
	Maintenance_Data[MAINTENANCE_TRANSMISSION_OIL_FILTER].Maintenance_Interval = 2; // 초기 100시간
	Maintenance_Data[MAINTENANCE_DIFFERENTIAL_GEAR_OIL].Maintenance_Interval = 2; // 초기 100시간
	Maintenance_Data[MAINTENANCE_HYD_AIR_BREATHER_ELEMENT].Maintenance_Interval = 10; // 250 -> 500 //++,--, 230104 ysm, BRAKE_OIL
	Maintenance_Data[MAINTENANCE_HYD_OIL_RETURN_FILTER].Maintenance_Interval = 5; // 1000(초기 250시간)
	Maintenance_Data[MAINTENANCE_FUEL_FILTER].Maintenance_Interval = 0xff; // 1000 -> Not Used
	Maintenance_Data[MAINTENANCE_FAN_BELT].Maintenance_Interval = 20; // 1000
	Maintenance_Data[MAINTENANCE_BRAKE_OIL].Maintenance_Interval = 20; // 1000
	Maintenance_Data[MAINTENANCE_AIR_CLEANER_ELEMENT].Maintenance_Interval = 20; // 1000
	Maintenance_Data[MAINTENANCE_HYD_SUCTION_STRAINER].Maintenance_Interval = 40; // 2000
	Maintenance_Data[MAINTENANCE_RADIATIOR_COOLANT].Maintenance_Interval = 80; // 4000
	Maintenance_Data[MAINTENANCE_HYD_OIL].Maintenance_Interval = 100; // 5000
	Maintenance_Data[MAINTENANCE_LPG_VAPORIZER].Maintenance_Interval = 80; //4000
	Maintenance_Data[MAINTENANCE_SPARK_PLUG].Maintenance_Interval = 40; 	// 2000 
	Maintenance_Data[MAINTENANCE_LOCK_OFF_VALVE_FILTER].Maintenance_Interval = 40; // 2000
	
	Maintenance_Data[MAINTENANCE_DPF_ASH_CLEANING].Maintenance_Interval = 0xff; // not used
	Maintenance_Data[MAINTENANCE_BRAKE_COOLING_OIL_FILTER].Maintenance_Interval = 0xff; // not used
	//--, 230104 ysm, BRAKE_OIL

	for(i = 0 ; i < MAINTENANCE_TOTAL ; i++)
	{
		EepromWrite((i*32)+ADDRESS_MAINTENANCE, (unsigned char*)(&Maintenance_Data[i]), 32);
	}
	TimeDelay_msec(5);
    
    // ++, 210225 ctw Auto JIG Final Test
    if(EepromWrite_ERR_FLAG > 1)
    {
        Eeprom_All_Clear = 2;
        EepromWrite_ERR_FLAG = 0;
    }
    else
    {
        Eeprom_All_Clear = 1;
        EepromWrite_ERR_FLAG = 0;
    }
    // --, 210225 ctw Auto JIG Final Test
        
}

// 이미지 없을 경우 최초 1회만...
void Initialize_EEPROM_Frist()
{
    // ++, 210225 ctw Auto JIG Final Test
    CAN_TxHeaderTypeDef TxHeader1;
    uint8_t TxData1[8] = {0,};
    unsigned int TxMailBox;
    
    EepromWrite_ERR_FLAG = 1;
    // --, 210225 ctw Auto JIG Final Test
    
	EEPROM_Reset();
	//Initialize_E2PROM_Hourmeter_Odometer();
	Initialize_Hourmeter();
	HourMeter_Value = 0x00;                 // ++, --, 210225 ctw Auto JIG Final Test
	
	Initialize_Odometer();
	Odometer_Value = 0x00;                  // ++, --, 210225 ctw Auto JIG Final Test
	Odometer_Value_cm = 0x00;               // ++, --, 210225 ctw Auto JIG Final Test
        
	Initialize_E2PROM_Maintenance();
	Initialize_E2PROM_Variable();

	//++, 220901 ysm, FAN_FINAL
	Initialize_Diagnosis();
	SatelliteCommInitial();
	SatelliteCommInitial3();
	//--, 220901 ysm, FAN_FINAL
	
	// ++, 210225 ctw Auto JIG Final Test
	TxHeader1.ExtId = 0x18FB972F;
	TxHeader1.IDE = CAN_ID_EXT;
	TxHeader1.RTR = CAN_RTR_DATA;
	TxHeader1.DLC = 8;

	TxData1[0] = Eeprom_All_Clear;

	TxMailBox = HAL_CAN_GetTxMailboxesFreeLevel(&hcan1);
	HAL_CAN_AddTxMessage (&hcan1, &TxHeader1, TxData1, &TxMailBox);
	// --, 210225 ctw Auto JIG Final Test
        
}

void Set_ESL_Time(unsigned char data)
{
	unsigned int ESL_Time_Second;
	unsigned char tempchar[1];
	
	switch(data)
	{
		case 0 : // 5 minute
			ESL_Time_Second = 5 * 60;
			break;
		case 1 : // 10 minute
			ESL_Time_Second = 10 * 60;
			break;
		case 2 : // 15 minute
			ESL_Time_Second = 15 * 60;
			break;
		case 3 : // 30 minute
			ESL_Time_Second = 30 * 60;
			break;
		case 4 : // 1 Hour
			ESL_Time_Second = 60 * 60;
			break;
		case 5 : // 2 Hour
			ESL_Time_Second = 120 * 60;
			break;
		case 6 : // 4 Hour
			ESL_Time_Second = 240 * 60;	
			break;
		case 7 : // 1 Day
			ESL_Time_Second = 24*60 * 60;
			break;
		case 8 : // 2 Day
			ESL_Time_Second = 48*60 * 60;	
			break;

		default :
			ESL_Time_Second = 10;
			//ESL_Time_Second = 20;
			break;
	}

	RTC_ESL = Calculate_RTC_Offset(&RTC_Data,ESL_Time_Second,0);

	tempchar[0] = RTC_ESL.Year;
	EepromWrite(ADDRESS_ESL_RTC_YEAR, (unsigned char*)(&tempchar[0]), 1); 
	tempchar[0] = RTC_ESL.Month;
	EepromWrite(ADDRESS_ESL_RTC_MONTH, (unsigned char*)(&tempchar[0]), 1); 
	tempchar[0] = RTC_ESL.Date;
	EepromWrite(ADDRESS_ESL_RTC_DATE, (unsigned char*)(&tempchar[0]), 1); 
	tempchar[0] = RTC_ESL.WeekDay;
	EepromWrite(ADDRESS_ESL_RTC_DAY, (unsigned char*)(&tempchar[0]), 1); 
	tempchar[0] = RTC_ESL.Hours;
	EepromWrite(ADDRESS_ESL_RTC_HOUR, (unsigned char*)(&tempchar[0]), 1); 
	tempchar[0] = RTC_ESL.Minutes;
	EepromWrite(ADDRESS_ESL_RTC_MINUTE, (unsigned char*)(&tempchar[0]), 1); 
	tempchar[0] = RTC_ESL.Seconds;
	EepromWrite(ADDRESS_ESL_RTC_SECOND, (unsigned char*)(&tempchar[0]), 1); 
	
}

void read_RTC_ESL(st_DATA_RTC *temp_RTC_Data)
{    
	EepromRead(ADDRESS_ESL_RTC_YEAR, (unsigned char*)(&temp_RTC_Data->Year), 1);   
	EepromRead(ADDRESS_ESL_RTC_MONTH, (unsigned char*)(&temp_RTC_Data->Month), 1);   
	EepromRead(ADDRESS_ESL_RTC_DATE, (unsigned char*)(&temp_RTC_Data->Date), 1);   
	EepromRead(ADDRESS_ESL_RTC_DAY, (unsigned char*)(&temp_RTC_Data->WeekDay), 1);   
	EepromRead(ADDRESS_ESL_RTC_HOUR, (unsigned char*)(&temp_RTC_Data->Hours), 1);   
	EepromRead(ADDRESS_ESL_RTC_MINUTE, (unsigned char*)(&temp_RTC_Data->Minutes), 1);   
	EepromRead(ADDRESS_ESL_RTC_SECOND, (unsigned char*)(&temp_RTC_Data->Seconds), 1);   
}


void Initialize_EEPROM(void)
{
	//Initialize_EEPROM_Frist();
	
	EepromRead(ADDRESS_MODEL_INFO, (unsigned char*)&InfoModel1, sizeof(InfoModel1)); 	
	ReadEquipmentSetting1();

	EepromRead(ADDRESS_HAC_STATUS, (unsigned char*)&InfoModel2, sizeof(InfoModel2)); 	
	ReadEquipmentSetting2();

	EepromRead(ADDRESS_AUTOSHIFT_STATUS, (unsigned char*)&InfoModel3, sizeof(InfoModel3)); 	
	ReadEquipmentSetting3();


	EepromRead(ADDRESS_LED_BRIGHTNESS_LEVEL, (unsigned char*)&InfoDisplaySetting, sizeof(InfoDisplaySetting));   
	ReadDisplaySetting();

	for (UCHAR i = 0; i < MAINTENANCE_TOTAL; i++)
	{
		EepromRead(ADDRESS_MAINTENANCE + i * 32, (unsigned char *)(&Maintenance_Data[i]), sizeof(st_Maintenance));	     	
	}

	Flag_ESL = InfoModel2.ESL_Set_Status;
#if 1
	if(InfoModel2.ESL_Set_Status==DELAYTIME) // 지정 시간 후 동작 
	{
		read_RTC(&RTC_Data);
		read_RTC_ESL(&RTC_ESL);
		Flag_ESL = Check_ESL_running(&RTC_Data,&RTC_ESL);
	}
	else if(InfoModel2.ESL_Set_Status==ACTIVE) // always ESL
	{
	#if 0
		EepromRead(ADDRESS_CHECKED_ESL, (unsigned char*)(&tempchar[0]), 1);   

		if(tempchar[0] ==1)
		{
			read_RTC(&RTC_Data);
			read_RTC_ESL(&RTC_ESL);
			Flag_ESL = Check_ESL_running(&RTC_Data,&RTC_ESL); 
		}
	#else
		Flag_ESL = ESL_ENABLE;
	#endif
	}
	else
		Flag_ESL = ESL_DISABLE;
#endif

	if(InfoDisplaySetting.Language > STATE_DISPLAY_LANGUAGE_ENGLISH)
		InfoDisplaySetting.Language = STATE_DISPLAY_LANGUAGE_KOREAN;
	if(InfoDisplaySetting.SpeedUnit > UNIT_SPEED_MPH)
		InfoDisplaySetting.SpeedUnit = UNIT_SPEED_KMH;
	if(InfoDisplaySetting.WeightUnit > UNIT_WEIGHT_LB)
		InfoDisplaySetting.WeightUnit = UNIT_WEIGHT_TON;
	if(InfoDisplaySetting.TemperatureUnit > UNIT_TEMP_F)
		InfoDisplaySetting.TemperatureUnit = UNIT_TEMP_C;
	if(InfoDisplaySetting.PressureUnit > UNIT_PRESSURE_KGCM)
		InfoDisplaySetting.PressureUnit = UNIT_PRESSURE_BAR;
	if(InfoDisplaySetting.LCDBrightnessLevel > BRIGHTNESS_MAX)
		InfoDisplaySetting.LCDBrightnessLevel = BRIGHTNESS_MAX;
	if(InfoDisplaySetting.LEDBrightnessLevel > BRIGHTNESS_MAX)
		InfoDisplaySetting.LEDBrightnessLevel = BRIGHTNESS_MAX;
 
}


void Delay(unsigned int delay)
{
	while(delay--);
}


void EEPROM_Delay(void)
{
	nop () ;nop () ;nop () ;nop () ;nop () ;nop () ;nop () ; nop () ;nop () ;nop () ;nop () ;nop () ;nop () ;nop () ;
	nop () ;nop () ;nop () ;nop () ;nop () ;nop () ;nop () ; nop () ;nop () ;nop () ;nop () ;nop () ;nop () ;nop () ;
	nop () ;nop () ;nop () ;nop () ;nop () ;nop () ;nop () ; nop () ;nop () ;nop () ;nop () ;nop () ;nop () ;nop () ;
	nop () ;nop () ;nop () ;nop () ;nop () ;nop () ;nop () ; nop () ;nop () ;nop () ;nop () ;nop () ;nop () ;nop () ;
	nop () ;nop () ;nop () ;nop () ;nop () ;nop () ;nop () ; nop () ;nop () ;nop () ;nop () ;nop () ;nop () ;nop () ;
	nop () ;nop () ;nop () ;nop () ;nop () ;nop () ;nop () ; nop () ;nop () ;nop () ;nop () ;nop () ;nop () ;nop () ;
	nop () ;nop () ;nop () ;nop () ;nop () ;nop () ;nop () ; nop () ;nop () ;nop () ;nop () ;nop () ;nop () ;nop () ;
	nop () ;nop () ;nop () ;nop () ;nop () ;nop () ;nop () ; nop () ;nop () ;nop () ;nop () ;nop () ;nop () ;nop () ;
	nop () ;nop () ;nop () ;nop () ;nop () ;nop () ;nop () ; nop () ;nop () ;nop () ;nop () ;nop () ;nop () ;nop () ;
	nop () ;nop () ;nop () ;nop () ;nop () ;nop () ;nop () ; nop () ;nop () ;nop () ;nop () ;nop () ;nop () ;nop () ;
	nop () ;nop () ;nop () ;nop () ;nop () ;nop () ;nop () ; nop () ;nop () ;nop () ;nop () ;nop () ;nop () ;nop () ;
	nop () ;nop () ;nop () ;nop () ;nop () ;nop () ;nop () ; nop () ;nop () ;nop () ;nop () ;nop () ;nop () ;nop () ;
	nop () ;nop () ;nop () ;nop () ;nop () ;nop () ;nop () ; nop () ;nop () ;nop () ;nop () ;nop () ;nop () ;nop () ;
	nop () ;nop () ;nop () ;nop () ;nop () ;nop () ;nop () ; nop () ;nop () ;nop () ;nop () ;nop () ;nop () ;nop () ;
	nop () ;nop () ;nop () ;nop () ;nop () ;nop () ;nop () ; nop () ;nop () ;nop () ;nop () ;nop () ;nop () ;nop () ;
	nop () ;nop () ;nop () ;nop () ;nop () ;nop () ;nop () ; nop () ;nop () ;nop () ;nop () ;nop () ;nop () ;nop () ;
	nop () ;nop () ;nop () ;nop () ;nop () ;nop () ;nop () ; nop () ;nop () ;nop () ;nop () ;nop () ;nop ()  ;nop () ;
	nop () ;nop () ;nop () ;nop () ;nop () ;nop () ;nop () ; nop () ;nop () ;nop () ;nop () ;nop () ;nop () ;nop () ;
	nop () ;nop () ;nop () ;nop () ;nop () ;nop () ;nop () ; nop () ;nop () ;nop () ;nop () ;nop () ;nop () ;nop () ;
	nop () ;nop () ;nop () ;nop () ;nop () ;nop () ;nop () ; nop () ;nop () ;nop () ;nop () ;nop () ;nop () ;nop () ;
}


void EEPROM_PortChange(unsigned char dir)
{	
	GPIO_InitTypeDef GPIO_InitStruct;
	if(dir)	// Output
	{	
		/*Configure GPIO pins : C_EEP_SDA_Pin */
		GPIO_InitStruct.Pin = GPIO_PIN_7;
		GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
		GPIO_InitStruct.Pull = GPIO_NOPULL;
		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
		HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
	}
	else	// Input
	{	
		/*Configure GPIO pins : C_EEP_SDA_Pin */
		GPIO_InitStruct.Pin = GPIO_PIN_7;
		GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
		GPIO_InitStruct.Pull = GPIO_NOPULL;
		HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
	}
}


void EEPROM_START(void)
{
	EEP_SDA1;
	EEP_SDAOUT ;

	/* start */
	EEP_SDA1;
	EEP_SCL1 ;
	EEPROM_Delay();
	EEP_SDA0;
	EEPROM_Delay();
	EEP_SCL0 ;
}


void EEPROM_END(void)
{
	/* stop */
	EEP_SDA0;
	EEPROM_Delay();
	EEP_SCL1 ;
	EEPROM_Delay();
	EEP_SDA1;

	Delay(25);
}


void EepromByteWrite(unsigned char WData)
{
	unsigned char i, j;

	EEPROM_Delay();

	for(i = 0; i < 8; i++)
	{
		j = ( WData & 0x80 ) ? 1 : 0;
		(j) ? EEP_SDA1 : EEP_SDA0;
		EEPROM_Delay();
		EEP_SCL1;
		EEPROM_Delay();
		WData <<= 1;
		EEP_SCL0;
		EEPROM_Delay();
	}

	EEP_SDA0;
	EEPROM_Delay();
	EEP_SCL1;
	EEPROM_Delay();
	EEP_SCL0;
}


unsigned char EepromByteRead(void)
{
	unsigned char i;

	//CPPCHECK ERROR
	unsigned char RData = 0;

	EEPROM_Delay();
	EEP_SCL0;
	EEPROM_Delay();
	EEP_SDA1;
	EEP_SDAIN;

	for(i = 0; i < 8; i++)
	{
		EEP_SCL1;
		EEPROM_Delay();
		RData = (RData << 1) | EEP_SDA_READ;
		EEP_SCL0;
		EEPROM_Delay();
	}
	EEP_SDA0;
	EEP_SDAOUT;
	return(RData);
}


void EepromWrite(unsigned short Add,  unsigned char *Data, unsigned short size)
{
	unsigned char CompareData[256];
	unsigned char Error_cnt = 5;

	while(Error_cnt)
	{
		EEPROM_START();

		EepromByteWrite(0xa0);
		EepromByteWrite((unsigned char)(Add >> 8));
		EepromByteWrite((unsigned char)Add);

		for(int i = 0; i < size; i++)
			EepromByteWrite(Data[i]);

		EEPROM_END();

		for(unsigned int i = 0; i < 1000; i++);

		EepromRead(Add, CompareData, size);

		if(memcmp(Data, CompareData, size) == 0)
			break;

		Error_cnt--;
                
                // ++, 210225 ctw Auto JIG Final Test
                if(Error_cnt == 0)
                {
                      EepromWrite_ERR_FLAG++;
                }
                // --, 210225 ctw Auto JIG Final Test
                
	}
	Delay(600000);
}


void EEPROM_Write_Byte(unsigned short Add,  unsigned int Data, unsigned short size)
{
	unsigned char CompareData;
	unsigned char Error_cnt = 5;
	unsigned char Position;

	while(Error_cnt)
	{
		EEPROM_START();

		EepromByteWrite(0xa0);
		EepromByteWrite((unsigned char)(Add >> 8));
		EepromByteWrite((unsigned char)Add);

		for(unsigned short i = 0; i < size; i++)
		{
			Position = i * 8;
			EepromByteWrite((Data>>Position)&0xFF);
		}

		EEPROM_END();

		for(unsigned int i = 0; i < 1000; i++);
		EepromRead(Add, &CompareData, 1);

		if(CompareData == Data)
			break;

		Error_cnt--;
	}
	Delay(600000);
}


void EepromRead(unsigned short Add, unsigned char *Data, unsigned short size)
{
	EEPROM_START();

	EepromByteWrite(0xa0);
	EepromByteWrite((unsigned char)(Add >> 8));
	EepromByteWrite((unsigned char)Add);

	EEPROM_Delay();

	/* start */
	EEP_SDA1;
	EEPROM_Delay();
	EEP_SCL1;
	EEPROM_Delay();

	EEP_SDA0;
	EEPROM_Delay();
	EEP_SCL0;
	/* start end */

	EepromByteWrite(0xa1);

	for(int i = 0; i < (size - 1); i++)
	{
		Data[i] = EepromByteRead();
		/* ack */
		EEP_SDA0;
		EEPROM_Delay();
		EEP_SCL1;
		EEPROM_Delay();
		EEP_SCL0;
		EEPROM_Delay();
	}

	Data[size - 1] = EepromByteRead();
	/* nak */
	EEP_SDA1;
	EEPROM_Delay();
	EEP_SCL1;
	EEPROM_Delay();
	EEP_SCL0;
	EEPROM_Delay();

	/* stop */
	EEPROM_END();

	EEPROM_Delay();
}

void ResetEEPROM()
{
  
	// ++, 210225 ctw Auto JIG Final Test //if(Tab_Number == 4)추가, 기존 코드는 else문 내부만

	if(RX_MMI_DATA_43981.EEPROM_CLEAR == EEPROM_CLEAR_ALL)
	{
	        Initialize_EEPROM_Frist();
	        Initialize_EEPROM();
	}
	else if(RX_MMI_DATA_43981.EEPROM_CLEAR == EEPROM_CLEAR_HOURMETER)
	{
	        Initialize_Hourmeter();
	        HourMeter_Value = RX_MMI_DATA_43981.EEPROM_DATA;
	}
	else if(RX_MMI_DATA_43981.EEPROM_CLEAR == EEPROM_CLEAR_ODOMETER)
	{
	        Initialize_Odometer();
	        Odometer_Value = (RX_MMI_DATA_43981.EEPROM_DATA /100000);
	        Odometer_Value_cm = (RX_MMI_DATA_43981.EEPROM_DATA%100000);
	}
	else if(RX_MMI_DATA_43981.EEPROM_CLEAR == EEPROM_CLEAR_DATA)
	{
	        Initialize_E2PROM_Maintenance();
	        Initialize_E2PROM_Variable();
	        Initialize_EEPROM();
	}
	else if(RX_MMI_DATA_43981.EEPROM_CLEAR == EEPROM_CLEAR_MAINTENANCE)
	{
	        Initialize_E2PROM_Maintenance();
	}
	else if(RX_MMI_DATA_43981.EEPROM_CLEAR == EEPROM_CLEAR_ANGLE_SENSOR)
	{
		Equipment_Calibration = CALIBRATION_ANGLE_INITIAL;
	}
	// --, 210225 ctw Auto JIG Final Test //if(Tab_Number == 4)추가, 기존 코드는 else문 내부만
	
}

#if 0
unsigned char Confirm_EEPROM(void)
{
	unsigned char Data1[4];
	unsigned char Data2[4];
	unsigned char Data3[4];

	Data2[0] = 0xaa;
	Data2[1] = 0x44;
	Data2[2] = 0xc0;
	Data2[3] = 0x0c;
	
	Data3[0] = 0x55;
	Data3[1] = 0xbb;
	Data3[2] = 0x0c;
	Data3[3] = 0xc0;
	
	read_EEPROM(ADDRESS_CONFIRM_DATA, Data1, 4); 

	if ((memcmp(Data1, Data3, 4)) == 0)
	{		
		write_EEPROM(ADDRESS_CONFIRM_DATA, Data2, 4);
		return 1;
	}
	else
	{
		return 0;
	}   
}
#endif

