#include "FLV_Cluster_APP.h"
#include "math.h"
// ++, 200407 bwk
#if 1
#define SPEED_LIMIT_33D9H_P		30
#define SPEED_LIMIT_33D9H_I 	10
#define SPEED_LIMIT_33D9H_D 	500.0

#else
#define SPEED_LIMIT_33D9H_P	30
#define SPEED_LIMIT_33D9H_I 	10
#define SPEED_LIMIT_33D9H_D 	500.0

unsigned short speed_limit_1 = 20;
unsigned short speed_limit_2 = 30;
float SPEED_LIMIT_33D9H_P	=20;
float SPEED_LIMIT_33D9H_I 	=7;
float SPEED_LIMIT_33D9H_D 	=80.0;
#endif
// --, 200407 bwk
st_HCESPN 		HCESPN;
st_SPN 			SPN;
st_COUNT_FLAG 	COUNT_FLAG;
st_AD_DATA 		AD_DATA;
st_BUZZER		BUZZER_FLAG;
st_Key_Status	RMS_Key_Status;

EEPROM_MODEL_DATA1	OldInfoModel1;
EEPROM_MODEL_DATA1	InfoModel1;
EEPROM_MODEL_DATA2	OldInfoModel2;
EEPROM_MODEL_DATA2	InfoModel2;
EEPROM_MODEL_DATA3	OldInfoModel3;
EEPROM_MODEL_DATA3	InfoModel3;


EEPROM_DISPLAY_DATA OldInfoDisplaySetting;
EEPROM_DISPLAY_DATA InfoDisplaySetting;

extern UCHAR gMaintenanceItem;
extern USHORT temp_interval;
extern st_Maintenance Maintenance_Data[];

unsigned short Load_data[3];

unsigned char Nominal_Friction;
unsigned char Torque_limit;
unsigned short Desire_torque;
unsigned short Temp_Desire_torque;
unsigned short Engine_Speed_Limit;

unsigned short EngineStartCount;
unsigned char EngineStartFlag;
unsigned char Flag_Engine_Start;

unsigned char FSCU_EngineStartFlag; //++,--, 230209 ysm, FSCU


unsigned char OPSS_Status;

unsigned char Engine_Type;

float diffSpeed;
unsigned char diffSpeed_sign;
float OlddiffSpeed;
float TotaldiffSpeed;
float MovingAverage[10];
uint8_t speed_limit_index;
uint8_t PID_limit_ON;
uint16_t PID_exit_cnt;
float result_SPN518_torq_limit;
float result_SPN518_torq_limit_old;

//unsigned short result_SPN518_torq_limit;
//unsigned short result_SPN518_torq_limit_old;

unsigned char SpeedLimitOverrideControlMode;
unsigned char SpeedLimitTorque;

unsigned char Speed_Movingaverage_flag;
unsigned int Speed_Buffer[10];
unsigned int Speed_Buffer_cnt;
// ++, 200819 bwk
#if 0
unsigned short Speed_value_temp[2];
unsigned short Calculate_Speed_Value;
#else
float Speed_value_temp[2];
float Calculate_Speed_Value;

#endif
// --, 200819 bwk

unsigned char AutoShiftMode_Gear = 1;
unsigned char Old_AutoShiftMode_Gear = 1;
unsigned char AutoShiftMode_Gear_flag = 0;

float SR_Temp;
float Trpm_Temp;

extern unsigned char MAST_EQUIPMENT;
extern unsigned char ANGLE_EQUIPMENT;

extern PASSWORD_DATA	InfoPassword;
extern st_DOUT_DATA		DOUT_DATA;

extern FREQUENCY_DATA TACHO;

extern CAN_ECU_DATA_65266 RX_ECU_DATA_65266;


extern unsigned short 		Fuel_Level_rowdata;
extern unsigned short 		TM_Oil_Temp_rowdata;
extern unsigned char 		ALT_Level_Status;
extern float 				LOAD_SENSOR;
extern float				HYD_TEMP_SENSOR;
extern float				TM_OIL_PRESSURE;

extern float HYD_OIL_PRESSURE; //++,--, 220829 ysm, 9A



unsigned short SPEED_LIMIT_P_GAIN = 30;
unsigned short SPEED_LIMIT_I_GAIN = 10;
unsigned short SPEED_LIMIT_D_GAIN = 500;



unsigned char				Equipment_Calibration;
unsigned char				Flag_ESL, Save_ESL_Interval;
unsigned char				WeightBuzzerOn;

unsigned char				OldGear;

unsigned char				TMOilTemp_Warning;
unsigned char				ClutchProtection;

unsigned char Init_SeatBeltInterlock_Count;

//++, 211125 ysm
unsigned char OPSS_Travel_Cut_Relay = 0;
unsigned char OPSS_Attach_Cut_Relay = 0;
unsigned char APC_Parking_Relay = 0;
unsigned char DCSR_Travel_Cut_Relay = 0;
unsigned char ZEROSTART_Travel_Cut_Relay = 0;
unsigned char AUTOLEVELING_Attach_Cut_Relay = 0;
unsigned char HAC_Parking_Relay = 0;
//--, 211125 ysm

//++, 220222 ysm, AUTO_TILT
unsigned char Auto_Tilt_Upper_Limit_Angle;
unsigned char Auto_Tilt_Lower_Limit_Angle;
unsigned char Auto_Tilt_Temp_Angle;
unsigned char Auto_Tilt_Mode_Flag = 0;
extern float temp_filter_val;

//--, 220222 ysm, AUTO_TILT

//++, 230503 ysm, FSCU_HAC
unsigned char FSCU_HAC_OFF_Flag = 0;
//--, 230503 ysm, FSCU_HAC


//++, 210409 ysm, 9S
extern float TM_OIL_TEMP_SENSOR;
extern float BRAKE_OIL_PRESSURE;

extern unsigned short Coolant_Temp_rowdata;


extern unsigned short Engine_Error_Check_rowdata;
extern CAN_ECU_DATA_65262 RX_ECU_DATA_65262;
//--, 210409 ysm, 9S

//++, 210821 ysm, 9S
extern unsigned short Parking_Buzzer_Count;
extern unsigned short Parking_Buzzer_Count2;
extern unsigned char Parking_Buzzer_flag;
//--, 210821 ysm, 9S

unsigned char Speed_Limit_Test_Flag; //++,--, 220901 ysm


extern CAN_ECU_DATA_64890							RX_ECU_DATA_64890;
extern CAN_ECU_DATA_61443							RX_ECU_DATA_61443;


extern unsigned char RCMData_MaintenanceChange;
extern unsigned char RCMData_MaintenanceItem;

extern unsigned char ActualGearStatus;

#if MENU_SHOW_SPEEDLIMIT
extern SPEED_LIMIT_SET_STATUS_65452				RX_SPEED_LIMIT_SET_STATUS_65452;
#endif

extern unsigned char temp_ModelInfo;
extern unsigned char temp_TireInfo;

extern UCHAR CurrDisplayMast;
extern unsigned char RCMData_UserPasswordChange;
extern unsigned char RCMData_ESLStatusChange;

extern unsigned char Tab_Number;     // ++, --, 210714 ctw Auto JIG Final Test
//++, 211124 ysm
extern unsigned char MSS_ESL_Flag;
//--, 211124 ysm


//++, 220222 ysm, AUTO_TILT
extern unsigned char Auto_Tilt_Upper_Limit_Angle;
extern unsigned char Auto_Tilt_Lower_Limit_Angle;
extern unsigned char Auto_Tilt_Filter_Value;
extern unsigned char Auto_Tilt_Temp_Angle;
//--, 220222 ysm, AUTO_TILT

//++, 220428 ysm, 9V_DPF
extern unsigned char Tilt_Alarm_flag;
//--, 220428 ysm, 9V_DPF

//++, 220818 ysm, ACC_TEST2
extern ACC_TUNE									ACCS;
//++, 220818 ysm, ACC_TEST2

//++, 221226 ysm, FSCU
extern unsigned char FLT_ERROR_Travel_Cut_Relay;
extern CAN_FSCU_STATE1_65525						RX_FSCU_DATA_65525;
extern CAN_FLT_SUB_65291							RX_FLT_SUB_65291;

extern unsigned char BrakeOilLevelLow_BuzzerStop;
//--, 221226 ysm, FSCU

void InitSpeedLimitProcess()
{
	memset(&MovingAverage,0,sizeof(MovingAverage));
	Speed_Buffer_cnt = 0;
	Speed_value_temp[0] = Speed_value_temp[1] = Calculate_Speed_Value = 0.0;
	PID_exit_cnt = PID_limit_ON = speed_limit_index = 0;
	OlddiffSpeed = diffSpeed = TotaldiffSpeed = 0;
	result_SPN518_torq_limit = 0xff;
	result_SPN518_torq_limit_old = 40; //++, 201208 ysm, SPEED_LIMIT
	SpeedLimitOverrideControlMode = SpeedLimitTorque = 0;
}

void Initialize_Algorithm_Variable(void)
{
	unsigned short tempshort[2];
	unsigned char tempchar[2];
	float tempfloat;

	memset(&COUNT_FLAG,0,sizeof(COUNT_FLAG));
	memset(&HCESPN,0xff,sizeof(HCESPN));
	memset(&AD_DATA,0,sizeof(AD_DATA));
	AD_DATA.OldWeight_Current = 0xffff;
	memset(&BUZZER_FLAG,0,sizeof(BUZZER_FLAG));
	memset(&RMS_Key_Status,0,sizeof(RMS_Key_Status));
	memset(&Load_data,0,sizeof(Load_data));
	memset(&Speed_Buffer,0,sizeof(Speed_Buffer));

	InitSpeedLimitProcess();
	
	Nominal_Friction=Torque_limit=Desire_torque=Temp_Desire_torque=0;
	Engine_Speed_Limit=0;

	EngineStartCount=EngineStartFlag=Flag_Engine_Start=FSCU_EngineStartFlag = 0; //++,--, 230209 ysm, FSCU
	Calculate_Speed_Value	= 0.0;
	OPSS_Status = 0;
	Equipment_Calibration = CALIBRATION_NO;
	Flag_ESL = 1;		// ++, --, 200323 bwk 시동제한 시 시동 걸리는 부분 방지 
	Save_ESL_Interval = 0;
	WeightBuzzerOn = 0;
	OldGear = 0xff;

	TMOilTemp_Warning = ClutchProtection = 0;

	Init_SeatBeltInterlock_Count = 0;

	HCESPN.Fuel_Rate_LPG = 0;

	//WARNING LAMP INIT
	HCESPN.H706 = 0;	
	HCESPN.H504 = 0;
	HCESPN.H305 = 0;
	HCESPN.H313 = 0;
	HCESPN.H537 = 0;
	HCESPN.H320 = 0;
	HCESPN.H303 = 0;
	HCESPN.H317 = 0;
	HCESPN.H360 = 0;

	Make_HCESPN_UserPassword();
	Make_HCESPN_MasterPassword();

	EepromRead(ADDRESS_TARE_VOLT, (unsigned char*)(&tempshort[0]), 2);
	AD_DATA.C_Tare_Volt = tempshort[0];
	EepromRead(ADDRESS_MAX_VOLT, (unsigned char*)(&tempshort[0]), 2);
	AD_DATA.C_Max_Volt = tempshort[0];
	EepromRead(ADDRESS_MAX_TON, (unsigned char*)(&tempchar[0]), 1);     
	AD_DATA.C_Max_Ton = tempchar[0];

	EepromRead(ADDRESS2_USER_MAST_INTEGER, (unsigned char*)(&tempshort[0]), 2);
	AD_DATA.C_A = AD_DATA.C_A_IntegerValue = tempshort[0];
	
	EepromRead(ADDRESS2_USER_MAST_POINT, (unsigned char*)(&tempchar[0]), 1);   
	tempfloat=AD_DATA.C_A_PointValue=tempchar[0];

	AD_DATA.C_A +=(tempfloat/100);

	if ( AD_DATA.C_Tare_Volt >= 4500 )    AD_DATA.C_Bar_Tare = (( 350./4. ) * (((float)(AD_DATA.C_Tare_Volt) /10000) - 0.5 ));
	else                                  AD_DATA.C_Bar_Tare = 0;

	AD_DATA.C_D = (( AD_DATA.C_A / 2)*  AD_DATA.C_Bar_Tare );
      
	if((AD_DATA.C_Max_Ton != 0) && (AD_DATA.C_Max_Volt != 0))
	{
		AD_DATA.C_B = (2 * (( AD_DATA.C_Max_Ton * 100 ) + AD_DATA.C_D ))/((( 350./4. ) * (((float)(AD_DATA.C_Max_Volt) /10000) - 0.5 )));	
	}
	
	//++, 221226 ysm, FSCU
	EepromRead(ADDRESS_FSCU_ENABLE, (unsigned char*)(&tempchar[0]), 1);
	if(tempchar[0] > 1)
		tempchar[0] = 0;
	COUNT_FLAG.Flag_FSCU_Enable = tempchar[0];

	COUNT_FLAG.Count_FSCU_AliveCount = 0;
	COUNT_FLAG.Count_MCU_AliveCount = 0;


	EepromRead(ADDRESS_SEATBELT_STATUS, (unsigned char*)(&tempchar[0]), 1);   
	if(tempchar[0] > 1)
		tempchar[0] = 0;
	
	COUNT_FLAG.SeatBeltStatus = tempchar[0];
	COUNT_FLAG.SeatBeltCount = 0;
	//--, 221226 ysm, FSCU
	
}

void ReadEquipmentSetting1()
{
	EepromRead(ADDRESS_MODEL_INFO, (unsigned char*)&OldInfoModel1, sizeof(OldInfoModel1)); 

	if(OldInfoModel1.ModelInfo > MODEL_35LN_9A)
	{
		 OldInfoModel1.ModelInfo = MODEL_35L_9;
		 OldInfoModel1.TonInfo = 35;


		
	}

	if(InfoModel1.ModelInfo > MODEL_35LN_9A)
	{
		 InfoModel1.ModelInfo = MODEL_35L_9;
		 InfoModel1.TonInfo = 35;
		
	}

	if(((OldInfoModel1.ModelInfo >= MODEL_35L_9)&&(OldInfoModel1.ModelInfo <= MODEL_33LC_9))||
		((OldInfoModel1.ModelInfo >= MODEL_25L_9A)&&(OldInfoModel1.ModelInfo <= MODEL_35LN_9A)))
	{
		Engine_Type = LPG_TYPE;
	}
	else
	{
		Engine_Type = DIESEL_TYPE;
	}

	if(InfoModel1.DCSR_Status > 1)
	{
		InfoModel1.DCSR_Status = 0;
	}
			
	if((InfoModel1.DCSR_Speed_Block < MIN_BLOCK_SPEED_LIMIT)||(InfoModel1.DCSR_Speed_Block > MAX_BLOCK_SPEED_LIMIT))
	{
		InfoModel1.DCSR_Speed_Block = MIN_BLOCK_SPEED_LIMIT;
	}

	if((InfoModel1.DCSR_Speed_Restore < MIN_RESTORE_SPEED_LIMIT)||(InfoModel1.DCSR_Speed_Restore > MAX_RESTORE_SPEED_LIMIT))
	{
		InfoModel1.DCSR_Speed_Restore = MIN_RESTORE_SPEED_LIMIT;
	}

	temp_ModelInfo = InfoModel1.ModelInfo;
	
}

void ReadEquipmentSetting2()
{
	EepromRead(ADDRESS_HAC_STATUS, (unsigned char*)&OldInfoModel2, sizeof(OldInfoModel2)); 

	if(OldInfoModel2.ESL_Interval>DELAYTIME_2DAY)
		OldInfoModel2.ESL_Interval = DELAYTIME_5MIN;

	if(InfoModel2.HAC_Status > 1)
	{
		InfoModel2.HAC_Status = 1; //210708 ysm, oFF -> ON 	
	}

	if(InfoModel2.SpeedLimitStatus > 1)
	{
		InfoModel2.SpeedLimitStatus = 0;
	}

	if((InfoModel1.ModelInfo >= MODEL_35D_9S)&&(InfoModel1.ModelInfo <= MODEL_50D_9S))
	{
		if((InfoModel2.SpeedLimitValue < 5)||(InfoModel2.SpeedLimitValue > 25))
		{
			InfoModel2.SpeedLimitValue = 5;
		}

	}
	else
	{
						
		if((InfoModel2.SpeedLimitValue < MIN_SPEED_LIMIT)||(InfoModel2.SpeedLimitValue > MAX_SPEED_LIMIT))
		{
			InfoModel2.SpeedLimitValue = MIN_SPEED_LIMIT;
		}
	}

	if((InfoModel2.ESL_Set_Status < INACTIVE) || (InfoModel2.ESL_Set_Status > DELAYTIME))
	{
		InfoModel2.ESL_Set_Status = INACTIVE;
	}

	if(InfoModel2.ESL_Set_Status == INACTIVE)
		HCESPN.H820 = 0;
	else if(InfoModel2.ESL_Set_Status == ACTIVE)
		HCESPN.H820 = 1;
	else if(InfoModel2.ESL_Set_Status == DELAYTIME)
		HCESPN.H820 = 2;
	else
		HCESPN.H820 = 0;

	if(InfoModel2.ESL_Interval > 8)
	{
		InfoModel2.ESL_Interval = 0;
	}

	if(InfoModel2.ClutchProtectionAlarm > 1)
	{
		InfoModel2.ClutchProtectionAlarm = 0;
	}

	if(InfoModel2.ZeroStart > 1)
	{
		InfoModel2.ZeroStart = 0;
	}

	if(InfoModel2.ZeroStart_AccelValue > 12)  //++,--, 230202 ysm, FSCU 75->25->12
	{
		InfoModel2.ZeroStart_AccelValue = 1; //++,--, 230202 ysm, FSCU 7->13->1
	}
	else if(InfoModel2.ZeroStart_AccelValue < 1)
	{
		InfoModel2.ZeroStart_AccelValue = 1;
	}

	if(InfoModel2.SeatBeltInterlock > 1)
	{
		InfoModel2.SeatBeltInterlock = 0;
	}

}

void ReadEquipmentSetting3()
{
	EepromRead(ADDRESS_AUTOSHIFT_STATUS, (unsigned char*)&OldInfoModel3, sizeof(OldInfoModel3)); 

	if(OldInfoModel3.AutoShift_Status > 1)
	{
		OldInfoModel3.AutoShift_Status = 1;
	}

	if(InfoModel3.AutoShift_Status > 1)
	{
		InfoModel3.AutoShift_Status = 1;
	}
	
	if((InfoModel3.AutoShift_Status_Speed_UP < MIN_AUTOSHIFT_UP_LIMIT)||(InfoModel3.AutoShift_Status_Speed_UP > MAX_AUTOSHIFT_UP_LIMIT))
	{
		InfoModel3.AutoShift_Status_Speed_UP = MIN_AUTOSHIFT_UP_LIMIT;
	}

	if((InfoModel3.AutoShift_Status_Speed_DOWN < MIN_AUTOSHIFT_DOWN_LIMIT)||(InfoModel3.AutoShift_Status_Speed_DOWN > MAX_AUTOSHIFT_DOWN_LIMIT))
	{
		InfoModel3.AutoShift_Status_Speed_DOWN = MIN_AUTOSHIFT_DOWN_LIMIT;
	}

	if(InfoModel3.AutoShift_Status_Overlap_Time > 200)
	{
		InfoModel3.AutoShift_Status_Overlap_Time = 45; // 30->45 //++,--, 210814 ysm
	}

	//++, 230425 ysm, HDI_INT
	#if 0
	if(((InfoModel1.ModelInfo >= MODEL_35D_9S)&&(InfoModel1.ModelInfo <= MODEL_50D_9S))
	    ||((InfoModel1.ModelInfo >= MODEL_35L_9)&&(InfoModel1.ModelInfo <= MODEL_50L_9))
	    ||((InfoModel1.ModelInfo >= MODEL_35D_9HDI)&&(InfoModel1.ModelInfo <= MODEL_50DN_9HDI)))
	{	
		if(InfoModel3.TireInfo > 1)
		{
			InfoModel3.TireInfo = 0;
		}
	}
	else
	{
		InfoModel3.TireInfo = 0;
	}
	#else
	if(InfoModel3.TireInfo > 1)
	{
		InfoModel3.TireInfo = 0;
	}
	#endif

	//--, 230425 ysm, HDI_INT

	
	if(InfoModel3.AutoShift_Status_SR_Value_UP > 10)
	{
		InfoModel3.AutoShift_Status_SR_Value_UP = 7;
	}

	if(InfoModel3.AutoShift_Status_SR_Value_DOWN> 10)
	{
		InfoModel3.AutoShift_Status_SR_Value_DOWN = 4;
	}

	if(InfoModel3.AutoShift_Status_Overlap_DownTime> 200)
	{
		InfoModel3.AutoShift_Status_Overlap_DownTime = 0;
	}

	temp_TireInfo = InfoModel3.TireInfo;
}

							


void ReadDisplaySetting()
{
	EepromRead(ADDRESS_LED_BRIGHTNESS_LEVEL, (unsigned char*)&OldInfoDisplaySetting, sizeof(OldInfoDisplaySetting)); 

	if(OldInfoDisplaySetting.Language > STATE_DISPLAY_LANGUAGE_ENGLISH)
		OldInfoDisplaySetting.Language = STATE_DISPLAY_LANGUAGE_KOREAN;
	if(OldInfoDisplaySetting.SpeedUnit > UNIT_SPEED_MPH)
		OldInfoDisplaySetting.SpeedUnit = UNIT_SPEED_KMH;
	if(OldInfoDisplaySetting.WeightUnit > UNIT_WEIGHT_LB)
		OldInfoDisplaySetting.WeightUnit = UNIT_WEIGHT_TON;
	if(OldInfoDisplaySetting.TemperatureUnit > UNIT_TEMP_F)
		OldInfoDisplaySetting.TemperatureUnit = UNIT_TEMP_C;
	if(OldInfoDisplaySetting.PressureUnit > UNIT_PRESSURE_KGCM)
		OldInfoDisplaySetting.PressureUnit = UNIT_PRESSURE_BAR;
	if(OldInfoDisplaySetting.LCDBrightnessLevel > BRIGHTNESS_MAX)
		OldInfoDisplaySetting.LCDBrightnessLevel = BRIGHTNESS_MAX;
	if(OldInfoDisplaySetting.LEDBrightnessLevel > BRIGHTNESS_MAX)
		OldInfoDisplaySetting.LEDBrightnessLevel = BRIGHTNESS_MAX;
}

void SaveDisplaySetting()
{
	unsigned char tempchar[1];

	if(InfoDisplaySetting.LCDBrightnessLevel != 0xf)
	{
		if(OldInfoDisplaySetting.LCDBrightnessLevel != InfoDisplaySetting.LCDBrightnessLevel)
		{
			tempchar[0] = InfoDisplaySetting.LCDBrightnessLevel;
			EepromWrite(ADDRESS_LCD_BRIGHTNESS_LEVEL, (unsigned char*)(&tempchar[0]), 1); 
		}
	}

	if(InfoDisplaySetting.LEDBrightnessLevel != 0xf)
	{
		if(OldInfoDisplaySetting.LEDBrightnessLevel != InfoDisplaySetting.LEDBrightnessLevel)
		{
			tempchar[0] = InfoDisplaySetting.LEDBrightnessLevel;
			EepromWrite(ADDRESS_LED_BRIGHTNESS_LEVEL, (unsigned char*)(&tempchar[0]), 1); 
		}
	}

	if(InfoDisplaySetting.SpeedUnit != 0xf)
	{
		if(OldInfoDisplaySetting.SpeedUnit != InfoDisplaySetting.SpeedUnit)
		{		
			tempchar[0] = InfoDisplaySetting.SpeedUnit;
			EepromWrite(ADDRESS_SPEED_UNIT, (unsigned char*)(&tempchar[0]), 1); 
		}
	}
	if(InfoDisplaySetting.WeightUnit != 0xf)
	{
		if(OldInfoDisplaySetting.WeightUnit != InfoDisplaySetting.WeightUnit)
		{		
			tempchar[0] = InfoDisplaySetting.WeightUnit;
			EepromWrite(ADDRESS_WEIGHT_UNIT, (unsigned char*)(&tempchar[0]), 1); 
		}
	}
	if(InfoDisplaySetting.TemperatureUnit != 0xf)
	{
		if(OldInfoDisplaySetting.TemperatureUnit != InfoDisplaySetting.TemperatureUnit)
		{		
			tempchar[0] = InfoDisplaySetting.TemperatureUnit;
			EepromWrite(ADDRESS_TEMPERATURE_UNIT, (unsigned char*)(&tempchar[0]), 1); 
		}
	}
	if(InfoDisplaySetting.WeightDisplayUnit != 0xf)
	{
		if(OldInfoDisplaySetting.WeightDisplayUnit != InfoDisplaySetting.WeightDisplayUnit)
		{
			tempchar[0] = InfoDisplaySetting.WeightDisplayUnit;
			EepromWrite(ADDRESS_WEIGHT_DISPLAY_UNIT, (unsigned char*)(&tempchar[0]), 1); 
		}
	}
	if(InfoDisplaySetting.PressureUnit != 0xf)
	{
		if(OldInfoDisplaySetting.PressureUnit != InfoDisplaySetting.PressureUnit)
		{
			tempchar[0] = InfoDisplaySetting.PressureUnit;
			EepromWrite(ADDRESS_PRESSURE_UNIT, (unsigned char*)(&tempchar[0]), 1); 
		}
	}
	if(InfoDisplaySetting.Language != 0xf)
	{
		if(OldInfoDisplaySetting.Language != InfoDisplaySetting.Language)
		{
			tempchar[0] = InfoDisplaySetting.Language;
			EepromWrite(ADDRESS_LANGUAGE_SET, (unsigned char*)(&tempchar[0]), 1); 
		}
	}
	if(InfoDisplaySetting.WeightDisplayUnit != 0xf)
	{
		if(OldInfoDisplaySetting.WeightDisplayUnit != InfoDisplaySetting.WeightDisplayUnit)
		{
			tempchar[0] = InfoDisplaySetting.WeightDisplayUnit;
			EepromWrite(ADDRESS_WEIGHT_DISPLAY_UNIT, (unsigned char*)(&tempchar[0]), 1); 
		}
	}
	if(InfoDisplaySetting.LoadBuzzerSet!= 0xf)
	{
		if(OldInfoDisplaySetting.LoadBuzzerSet != InfoDisplaySetting.LoadBuzzerSet)
		{
			tempchar[0] = InfoDisplaySetting.LoadBuzzerSet;
			EepromWrite(ADDRESS_LOAD_BUZZER, (unsigned char*)(&tempchar[0]), 1); 
		}
	}

	CalibrationFlag = DISPLAY_INFO_CHANGE;
	Checkcnt100ms = 0;	

	ReadDisplaySetting();
}

//++, 221226 ysm, FSCU
void CheckSeatBeltStatus()
{
	unsigned char tempchar[1];
	unsigned char seat_belt_flag;

	seat_belt_flag = Flag_DIN[INDEX_SW_BUCKLE];

	if(seat_belt_flag == 1)
	{	
		COUNT_FLAG.SeatBeltCount++;
		if(COUNT_FLAG.SeatBeltCount >= 5)
		{
			COUNT_FLAG.SeatBeltCount = 10;
			if(COUNT_FLAG.SeatBeltStatus != 1)
			{
				COUNT_FLAG.SeatBeltStatus = 1;
				
				tempchar[0] = COUNT_FLAG.SeatBeltStatus;
				EepromWrite(ADDRESS_SEATBELT_STATUS, (unsigned char*)(&tempchar[0]), 1); 		
			}
		}		
		
	}
	else
	{
		COUNT_FLAG.SeatBeltCount = 0;		
	}

	if((COUNT_FLAG.SeatBeltStatus == 1)&&(seat_belt_flag == 0))
	{		
		if(Calculate_Speed_Value > 40)
		{
			BUZZER_FLAG.SeatBeltAlarm = 1;
		}
		else
		{

			BUZZER_FLAG.SeatBeltAlarm = 0;
		}

	}
	else
	{
		BUZZER_FLAG.SeatBeltAlarm = 0;
	}

}
//--, 221226 ysm, FSCU

void CheckGearStatus()
{
	//++, 221024 ysm, FSCU
	#if 0
	if(Flag_DIN[INDEX_GEAR_SIGNAL_N] == 1)
		HCESPN.Gear_542 = GEAR_NEUTRAL;
	else if(Flag_DIN[INDEX_GEAR_SIGNAL_R] == 1)
		HCESPN.Gear_542 = GEAR_REVERSE;
	else if(Flag_DIN[INDEX_GEAR_SIGNAL_F] == 1) 
		HCESPN.Gear_542 = GEAR_FORWARD;
	else
		HCESPN.Gear_542 = 3;
	#else			
	if(COUNT_FLAG.Flag_FLT_SUB == 1)
	{
		COUNT_FLAG.Comm_Error_FLT++;
		
		if(COUNT_FLAG.Comm_Error_FLT > 100)
		{
			COUNT_FLAG.Comm_Error_FLT = 200;
			
			HCESPN.Gear_542 = GEAR_NEUTRAL;

			FLT_ERROR_Travel_Cut_Relay = 1;
			COUNT_FLAG.Flag_FLTCommError = 1; //++,--, 221226 ysm, FSCU

		}
		else
		{	
			if(RX_FLT_SUB_65291.FNR_STATUS < 3)
			{			
				HCESPN.Gear_542 = RX_FLT_SUB_65291.FNR_STATUS;

				if(FLT_ERROR_Travel_Cut_Relay == 1)
				{
					if(HCESPN.Gear_542 == GEAR_NEUTRAL)
						FLT_ERROR_Travel_Cut_Relay = 0;
				}
				else
					FLT_ERROR_Travel_Cut_Relay = 0;

				COUNT_FLAG.Flag_FLTCommError = 0; //++,--, 221226 ysm, FSCU
			}
			else
			{
				HCESPN.Gear_542 = GEAR_NEUTRAL;
			}	
		}

	}
	else
	{
		if(Flag_DIN[INDEX_GEAR_SIGNAL_N] == 1)
		{
			HCESPN.Gear_542 = GEAR_NEUTRAL;
			COUNT_FLAG.Count_Gear_Fail = 0; //++,--, 230519 ysm, FSCU_HAC
		}
		else if(Flag_DIN[INDEX_GEAR_SIGNAL_R] == 1)
		{
			HCESPN.Gear_542 = GEAR_REVERSE;
			COUNT_FLAG.Count_Gear_Fail = 0; //++,--, 230519 ysm, FSCU_HAC
		}
		else if(Flag_DIN[INDEX_GEAR_SIGNAL_F] == 1)
		{
			HCESPN.Gear_542 = GEAR_FORWARD;
			COUNT_FLAG.Count_Gear_Fail = 0; //++,--, 230519 ysm, FSCU_HAC
		}
		else
		{
			//++, 230519 ysm, FSCU_HAC
			if(HCESPN.Gear_542 < 3)
			{
				if(COUNT_FLAG.Count_Gear_Fail++ > 100)
				{
					COUNT_FLAG.Count_Gear_Fail = 0;
					HCESPN.Gear_542 = 3;
				}
			}
			else
			{
				COUNT_FLAG.Count_Gear_Fail = 0;
				HCESPN.Gear_542 = 3;			
			}
			//--, 230519 ysm, FSCU_HAC
		}

		FLT_ERROR_Travel_Cut_Relay = 0;

		COUNT_FLAG.Flag_FLTCommError = 0; //++,--, 221226 ysm, FSCU
	}	
	#endif
	//--, 221024 ysm, FSCU

}
// --, 200407 bwk


void Translate_Torque_limit_Pwr(void) 
{
#if 0
	if(HCESPN.rpm_310< 700)
		Temp_Desire_torque =542;
	
	else if ( 700<=HCESPN.rpm_310&&HCESPN.rpm_310<800)
		Temp_Desire_torque = 542 + ((610-542)*(HCESPN.rpm_310%700)/100);

	else if ( 800<=HCESPN.rpm_310&&HCESPN.rpm_310<900)
		Temp_Desire_torque = 610 + ((652-610)*(HCESPN.rpm_310%800)/100);

	else if ( 900<=HCESPN.rpm_310&&HCESPN.rpm_310<1000)
		Temp_Desire_torque = 652 + ((693-652)*(HCESPN.rpm_310%900)/100);

	else if ( 1000<=HCESPN.rpm_310&&HCESPN.rpm_310<1100)
		Temp_Desire_torque = 693 + ((701-693)*(HCESPN.rpm_310%1000)/100);

	else if ( 1100<=HCESPN.rpm_310&&HCESPN.rpm_310<1200)
		Temp_Desire_torque = 701 - ((701-688)*(HCESPN.rpm_310%1100)/100);

	else if ( 1200<=HCESPN.rpm_310&&HCESPN.rpm_310<1300)
		Temp_Desire_torque = 688 - ((688-675)*(HCESPN.rpm_310%1200)/100);

	else if ( 1300<=HCESPN.rpm_310&&HCESPN.rpm_310<1400)
		Temp_Desire_torque = 675 - ((675-667)*(HCESPN.rpm_310%1300)/100);

	else if ( 1400<=HCESPN.rpm_310&&HCESPN.rpm_310<1500)
		Temp_Desire_torque = 667 - ((667-645)*(HCESPN.rpm_310%1400)/100);

	else if ( 1500<=HCESPN.rpm_310&&HCESPN.rpm_310<1600)
		Temp_Desire_torque = 645 - ((645-634)*(HCESPN.rpm_310%1500)/100);

	else if ( 1600<=HCESPN.rpm_310&&HCESPN.rpm_310<1800)
		Temp_Desire_torque = 634 - ((634-624)*(HCESPN.rpm_310%1600)/200);

	else if ( 1800<=HCESPN.rpm_310&&HCESPN.rpm_310<2000)
		Temp_Desire_torque = 624 - ((624-598)*(HCESPN.rpm_310%1800)/200);

	else if ( 2000<=HCESPN.rpm_310&&HCESPN.rpm_310<2100)
		Temp_Desire_torque = 598 - ((598-570)*(HCESPN.rpm_310%2000)/100);

	else if ( 2100<=HCESPN.rpm_310&&HCESPN.rpm_310<2200)
		Temp_Desire_torque = 570 - ((570-534)*(HCESPN.rpm_310%2100)/100);
	//++, 170324 ysm
	else if ( HCESPN.rpm_310>=2200)
		Temp_Desire_torque = 534;
#else
	if(HCESPN.rpm_310< 850)
		Temp_Desire_torque =97;
	else if(HCESPN.rpm_310<900)
		Temp_Desire_torque = 106;
	else if(HCESPN.rpm_310<1001)
		Temp_Desire_torque = 116;
	else if(HCESPN.rpm_310<1101)
		Temp_Desire_torque = 116;
	else if(HCESPN.rpm_310<1201)
		Temp_Desire_torque = 116;
	else if(HCESPN.rpm_310<1301)
		Temp_Desire_torque = 116;
	else if(HCESPN.rpm_310<1401)
		Temp_Desire_torque = 116;
	else if(HCESPN.rpm_310<1501)
		Temp_Desire_torque = 118;
	else if(HCESPN.rpm_310<1601)
		Temp_Desire_torque = 120;
	else if(HCESPN.rpm_310<1701)
		Temp_Desire_torque = 118;
	else if(HCESPN.rpm_310<1798)
		Temp_Desire_torque = 118;
	else if(HCESPN.rpm_310<1900)
		Temp_Desire_torque = 115;
	else if(HCESPN.rpm_310<1950)
		Temp_Desire_torque = 114;
	else if(HCESPN.rpm_310<2001)
		Temp_Desire_torque = 112;
	else if(HCESPN.rpm_310<2050)
		Temp_Desire_torque = 111;
	else if(HCESPN.rpm_310<2100)
		Temp_Desire_torque = 109;
	else if(HCESPN.rpm_310<2203)
		Temp_Desire_torque = 105;
	else if(HCESPN.rpm_310<2302)
		Temp_Desire_torque = 101;
	else //if(HCESPN.rpm_310<2402)
		Temp_Desire_torque = 64;
#endif
	Desire_torque =Temp_Desire_torque;

}

void Translate_Torque_limit_Std(void) 
{
#if 0
	if(HCESPN.rpm_310< 700)
		Temp_Desire_torque =542;
	
	else if ( 700<=HCESPN.rpm_310&&HCESPN.rpm_310<800)
		Temp_Desire_torque = 542 + ((610-542)*(HCESPN.rpm_310%700)/100);

	else if ( 800<=HCESPN.rpm_310&&HCESPN.rpm_310<900)
		Temp_Desire_torque = 610 + ((652-610)*(HCESPN.rpm_310%800)/100);

	else if ( 900<=HCESPN.rpm_310&&HCESPN.rpm_310<1000)
		Temp_Desire_torque = 652 + ((693-652)*(HCESPN.rpm_310%900)/100);

	else if ( 1000<=HCESPN.rpm_310&&HCESPN.rpm_310<1100)
		Temp_Desire_torque = 693 + ((701-693)*(HCESPN.rpm_310%1000)/100);

	else if ( 1100<=HCESPN.rpm_310&&HCESPN.rpm_310<1200)
		Temp_Desire_torque = 701 - ((701-688)*(HCESPN.rpm_310%1100)/100);

	else if ( 1200<=HCESPN.rpm_310&&HCESPN.rpm_310<1300)
		Temp_Desire_torque = 688 - ((688-675)*(HCESPN.rpm_310%1200)/100);

	else if ( 1300<=HCESPN.rpm_310&&HCESPN.rpm_310<1400)
		Temp_Desire_torque = 675 - ((675-667)*(HCESPN.rpm_310%1300)/100);

	else if ( 1400<=HCESPN.rpm_310&&HCESPN.rpm_310<1500)
		Temp_Desire_torque = 667 - ((667-645)*(HCESPN.rpm_310%1400)/100);

	else if ( 1500<=HCESPN.rpm_310&&HCESPN.rpm_310<1600)
		Temp_Desire_torque = 645 - ((645-634)*(HCESPN.rpm_310%1500)/100);

	else if ( 1600<=HCESPN.rpm_310&&HCESPN.rpm_310<1800)
		Temp_Desire_torque = 634 - ((634-624)*(HCESPN.rpm_310%1600)/200);

	else if ( 1800<=HCESPN.rpm_310&&HCESPN.rpm_310<2000)
		Temp_Desire_torque = 624 - ((624-470)*(HCESPN.rpm_310%1800)/200);

	else if ( 2000<=HCESPN.rpm_310&&HCESPN.rpm_310<2100)
		Temp_Desire_torque = 470 - ((470-450)*(HCESPN.rpm_310%2000)/100);
	
	else if ( HCESPN.rpm_310>=2100)
		Temp_Desire_torque = 450;
#else
	if(HCESPN.rpm_310< 850)
		Temp_Desire_torque =98;
	else if(HCESPN.rpm_310<900)
		Temp_Desire_torque = 107;
	else if(HCESPN.rpm_310<1001)
		Temp_Desire_torque = 116;
	else if(HCESPN.rpm_310<1101)
		Temp_Desire_torque = 116;
	else if(HCESPN.rpm_310<1201)
		Temp_Desire_torque = 116;
	else if(HCESPN.rpm_310<1301)
		Temp_Desire_torque = 110;
	else if(HCESPN.rpm_310<1401)
		Temp_Desire_torque = 104;
	else if(HCESPN.rpm_310<1501)
		Temp_Desire_torque = 95;
	else if(HCESPN.rpm_310<1601)
		Temp_Desire_torque = 96;
	else if(HCESPN.rpm_310<1701)
		Temp_Desire_torque = 96;
	else if(HCESPN.rpm_310<1798)
		Temp_Desire_torque = 93;
	else if(HCESPN.rpm_310<1900)
		Temp_Desire_torque = 90;
	else if(HCESPN.rpm_310<1950)
		Temp_Desire_torque = 90;
	else if(HCESPN.rpm_310<2001)
		Temp_Desire_torque = 85;
	else if(HCESPN.rpm_310<2050)
		Temp_Desire_torque = 83;
	else if(HCESPN.rpm_310<2100)
		Temp_Desire_torque = 82;
	else if(HCESPN.rpm_310<2203)
		Temp_Desire_torque = 81;
	else if(HCESPN.rpm_310<2302)
		Temp_Desire_torque = 80;
	else //if(HCESPN.rpm_310<2402)
		Temp_Desire_torque = 80;
#endif

	Desire_torque =Temp_Desire_torque;
}


void Power_Standard_Mode(void)
{
	// ++, --, 200326 bwk Std <-> Power 잘못 제어 하는 부분 수정 완료 
	if( Flag_DIN[INDEX_PWD_STD_MODE_SEL] == 1 )   //Std Mode
	{
		if ( HCESPN.rpm_310 >= 850 )	
		{
			Translate_Torque_limit_Std();
			
			Torque_limit =  (Desire_torque*100/990)  + Nominal_Friction; 
			
			Torque_limit += 125;
			Engine_Speed_Limit = 0x45b0; 
			if(Nominal_Friction>0x3b)
				Torque_limit=250;
		}
		else
		{
			Torque_limit = 250;
			Engine_Speed_Limit = 0xFAFF;
		}
  	}
	else//Power Mode
	{
		if ( HCESPN.rpm_310 >= 850 ) 
		{
			Translate_Torque_limit_Pwr();
			
			Torque_limit =  (Desire_torque*100/990)  + Nominal_Friction;  
			
			Torque_limit += 125;

			if(Nominal_Friction>0x3b)
				Torque_limit=250;
		}
		else
		{
			Torque_limit = 250;
			Engine_Speed_Limit = 0xFAFF;
		}
	}
}
/*
// DIESEL TYPE
#define MODEL_25D_9VB			0
#define MODEL_30D_9VB			1	
#define MODEL_35DN_9VB			2
#define MODEL_35D_9S			3
#define MODEL_40D_9S			4
#define MODEL_45D_9S			5
#define MODEL_50D_9S			6
#define MODEL_35L_9				7
#define MODEL_40L_9				8
#define MODEL_45L_9				9	
#define MODEL_50L_9				10
#define MODEL_25LC_9			11
#define MODEL_30LC_9			12
#define MODEL_33LC_9			13	
#define MODEL_25L_9A_X			14
#define MODEL_30L_9A_X			15
#define MODEL_33L_9A_X			16
#define MODEL_25D_9VS			17
#define MODEL_30D_9VS			18	
#define MODEL_35DN_9VS			19
#define MODEL_25D_9HDI			20
#define MODEL_30D_9HDI			21	
#define MODEL_35DN_9HDI			22
#define MODEL_25D_9HDI_S		23
#define MODEL_30D_9HDI_S		24
#define MODEL_35DN_9HDI_S		25
#define MODEL_35D_9HDI			26
#define MODEL_40D_9HDI			27
#define MODEL_45D_9HDI			28
#define MODEL_50DN_9HDI			29
#define MODEL_25L_9A			30
#define MODEL_30L_9A			31	
#define MODEL_33L_9A			32
#define MODEL_35LN_9A			33


*/
unsigned char Model_Change(void)
{
	unsigned char tempchar[1];
	unsigned char reset_maintenance_flag;
	
	//TON               0  1  2  3  4  5  6  7  8  9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 32 33 
	UCHAR Ton_Index[]={25,30,35,35,40,45,50,35,40,45,50,25,30,33,25,30,33,25,30,35,25,30,35,25,30,35,35,40,45,50,25,30,33,35};

	if(InfoModel1.ModelInfo != 0xff)
	{
		reset_maintenance_flag = 0;
		
		if(OldInfoModel1.ModelInfo != InfoModel1.ModelInfo)
		{
			reset_maintenance_flag = 1;
			tempchar[0] = InfoModel1.ModelInfo;
			EepromWrite(ADDRESS_MODEL_INFO, (unsigned char*)(&tempchar[0]), 1); 
			tempchar[0] = InfoModel1.TonInfo = Ton_Index[InfoModel1.ModelInfo];
			EepromWrite(ADDRESS_TON_INFO, (unsigned char*)(&tempchar[0]), 1); 
		}

		if(OldInfoModel3.TireInfo != InfoModel3.TireInfo)
		{
			tempchar[0] = InfoModel3.TireInfo;
			EepromWrite(ADDRESS_TIRE_INFO, (unsigned char*)(&tempchar[0]), 1); 

			temp_TireInfo = InfoModel3.TireInfo;

			OldInfoModel3.TireInfo = InfoModel3.TireInfo;

		}

		
	}
	ReadEquipmentSetting1();
	
	if(OldInfoModel1.ModelInfo == InfoModel1.ModelInfo)
	{		
		if(reset_maintenance_flag == 1)
			Reset_E2PROM_Maintenance();

		//++, 221212 ysm, FSCU	
		COUNT_FLAG.Flag_FLT_SUB = 0;	
	
		COUNT_FLAG.SeatBeltStatus = tempchar[0] = 0;		
		COUNT_FLAG.SeatBeltCount = 0;		
		
		EepromWrite(ADDRESS_SEATBELT_STATUS, (unsigned char*)(&tempchar[0]), 1); 
		//--, 221212 ysm, FSCU

		//++, 221226 ysm, FSCU
		COUNT_FLAG.Flag_FSCU_Enable = 0;
		//--, 221226 ysm, FSCU
		
		return SAVE_SUCCESS;
	}
	else
	{
		InfoModel1.ModelInfo = OldInfoModel1.ModelInfo;		
		return SAVE_FAILURE;
	}

}

unsigned char ESL_Setting(void)
{
	unsigned char tempchar[1];
	
	if((InfoModel2.ESL_Set_Status >= INACTIVE) && (InfoModel2.ESL_Set_Status <= DELAYTIME))
	{
		tempchar[0] = InfoModel2.ESL_Set_Status;
		EepromWrite(ADDRESS_ESL_SET_STATUS, (unsigned char*)(&tempchar[0]), 1); 
		tempchar[1] = ((tempchar[0]==ACTIVE)?1:0);
		EepromWrite(ADDRESS_ESL_STATUS, (unsigned char*)(&tempchar[1]), 1); 

		if(tempchar[0]==DELAYTIME) // 지정 시간 후 동작 
		{
			tempchar[1] = InfoModel2.ESL_Interval;
			EepromWrite(ADDRESS_ESL_INTERVAL, (unsigned char*)(&tempchar[1]), 1); 
			
			Save_ESL_Interval=1;
		}
		else if(tempchar[0]==ACTIVE)
		{
			Save_ESL_Interval=1;
		}
	}

	if(OldInfoModel2.ESL_Set_Status != InfoModel2.ESL_Set_Status)
	{
		RCMData_ESLStatusChange = 1;

	}
	
	ReadEquipmentSetting2();
	#if 0
	if((OldInfoModel2.ESL_Set_Status == InfoModel2.ESL_Set_Status)
		&& (OldInfoModel2.ESL_Status == InfoModel2.ESL_Status))
	#else
	if(OldInfoModel2.ESL_Set_Status == InfoModel2.ESL_Set_Status)
	#endif
	{
		if(InfoModel2.ESL_Set_Status == DELAYTIME)
		{
			if(OldInfoModel2.ESL_Interval == InfoModel2.ESL_Interval)
				return SAVE_SUCCESS;
			else
			{
				InfoModel2.ESL_Set_Status = OldInfoModel2.ESL_Set_Status;
				InfoModel2.ESL_Status = OldInfoModel2.ESL_Status;
				InfoModel2.ESL_Interval = OldInfoModel2.ESL_Interval;
				return SAVE_FAILURE;
			}
		}
		else 		
			return SAVE_SUCCESS;
	}
	else
	{
		InfoModel2.ESL_Set_Status = OldInfoModel2.ESL_Set_Status;
		InfoModel2.ESL_Status = OldInfoModel2.ESL_Status;
		InfoModel2.ESL_Interval = OldInfoModel2.ESL_Interval;
		return SAVE_FAILURE;
	}
}

unsigned char AutoShift_Setting()
{
	unsigned char tempchar[1];

	if(ScreenIndex == SCREEN_STATE_MENU_EQUIPMENT_AUTOSHIFT_MODE_SET)
	{
		if(InfoModel3.AutoShift_Status < 2)
		{
			tempchar[0] = InfoModel3.AutoShift_Status;
			EepromWrite(ADDRESS_AUTOSHIFT_STATUS, (unsigned char*)(&tempchar[0]), 1); 
		}
		ReadEquipmentSetting3();
		if((OldInfoModel3.AutoShift_Status == InfoModel3.AutoShift_Status))
		{
			return SAVE_SUCCESS;
		}
		else
		{
			InfoModel3.AutoShift_Status = OldInfoModel3.AutoShift_Status;
			return SAVE_FAILURE;
		}
	}
	else if(ScreenIndex == SCREEN_STATE_MENU_EQUIPMENT_AUTOSHIFT_SPEED_UP_SET)
	{
		if((InfoModel3.AutoShift_Status_Speed_UP >= MIN_AUTOSHIFT_UP_LIMIT)
			&&(InfoModel3.AutoShift_Status_Speed_UP <= MAX_AUTOSHIFT_UP_LIMIT))
		{
			tempchar[0] = InfoModel3.AutoShift_Status_Speed_UP;
			EepromWrite(ADDRESS_AUTOSHIFT_STATUS_SPEED_UP, (unsigned char*)(&tempchar[0]), 1); 
		}
		ReadEquipmentSetting3();
		if((OldInfoModel3.AutoShift_Status_Speed_UP == InfoModel3.AutoShift_Status_Speed_UP))
		{
			return SAVE_SUCCESS;
		}
		else
		{
			InfoModel3.AutoShift_Status_Speed_UP = OldInfoModel3.AutoShift_Status_Speed_UP;
			return SAVE_FAILURE;
		}
	}
	else if(ScreenIndex == SCREEN_STATE_MENU_EQUIPMENT_AUTOSHIFT_SPEED_DOWN_SET)
	{
		if((InfoModel3.AutoShift_Status_Speed_DOWN >= MIN_AUTOSHIFT_DOWN_LIMIT)
			&&(InfoModel3.AutoShift_Status_Speed_DOWN <= MAX_AUTOSHIFT_DOWN_LIMIT))
		{
			tempchar[0] = InfoModel3.AutoShift_Status_Speed_DOWN;
			EepromWrite(ADDRESS_AUTOSHIFT_STATUS_SPEED_DOWN, (unsigned char*)(&tempchar[0]), 1); 
		}
		ReadEquipmentSetting3();
		if((OldInfoModel3.AutoShift_Status_Speed_DOWN == InfoModel3.AutoShift_Status_Speed_DOWN))
		{
			return SAVE_SUCCESS;
		}
		else
		{
			InfoModel3.AutoShift_Status_Speed_DOWN = OldInfoModel3.AutoShift_Status_Speed_DOWN;
			return SAVE_FAILURE;
		}

	}
	else if(ScreenIndex == SCREEN_STATE_MENU_EQUIPMENT_AUTOSHIFT_OVERLAP_UP_SET)
	{
		if(InfoModel3.AutoShift_Status_Overlap_Time <= 200)
		{
			tempchar[0] = InfoModel3.AutoShift_Status_Overlap_Time;
			EepromWrite(ADDRESS_AUTOSHIFT_STATUS_OVERLAP_TIME, (unsigned char*)(&tempchar[0]), 1); 
		}
		ReadEquipmentSetting3();
		if((OldInfoModel3.AutoShift_Status_Overlap_Time == InfoModel3.AutoShift_Status_Overlap_Time))
		{			
			return SAVE_SUCCESS;
		}
		else
		{
			InfoModel3.AutoShift_Status_Overlap_Time = OldInfoModel3.AutoShift_Status_Overlap_Time;
			return SAVE_FAILURE;
		}
	}
	else if(ScreenIndex == SCREEN_STATE_MENU_EQUIPMENT_AUTOSHIFT_OVERLAP_DOWN_SET)
	{
		if(InfoModel3.AutoShift_Status_Overlap_DownTime <= 200)
		{
			tempchar[0] = InfoModel3.AutoShift_Status_Overlap_DownTime;
			EepromWrite(ADDRESS_AUTOSHIFT_STATUS_OVERLAP_DOWNTIME, (unsigned char*)(&tempchar[0]), 1); 
		}
		ReadEquipmentSetting3();
		if((OldInfoModel3.AutoShift_Status_Overlap_DownTime == InfoModel3.AutoShift_Status_Overlap_DownTime))
		{			
			return SAVE_SUCCESS;
		}
		else
		{
			InfoModel3.AutoShift_Status_Overlap_DownTime = OldInfoModel3.AutoShift_Status_Overlap_DownTime;
			return SAVE_FAILURE;
		}
	}
	else if(ScreenIndex == SCREEN_STATE_MENU_EQUIPMENT_AUTOSHIFT_SR_UP_SET)
	{
		if(InfoModel3.AutoShift_Status_SR_Value_UP <= 10)
		{
			tempchar[0] = InfoModel3.AutoShift_Status_SR_Value_UP;
			EepromWrite(ADDRESS_AUTOSHIFT_STATUS_SR_UP, (unsigned char*)(&tempchar[0]), 1); 
		}
		ReadEquipmentSetting3();
		if((OldInfoModel3.AutoShift_Status_SR_Value_UP == InfoModel3.AutoShift_Status_SR_Value_UP))
		{			
			return SAVE_SUCCESS;
		}
		else
		{
			InfoModel3.AutoShift_Status_SR_Value_UP = OldInfoModel3.AutoShift_Status_SR_Value_UP;
			return SAVE_FAILURE;
		}
	}
	else if(ScreenIndex == SCREEN_STATE_MENU_EQUIPMENT_AUTOSHIFT_SR_DOWN_SET)
	{
		if(InfoModel3.AutoShift_Status_SR_Value_DOWN  <= 10)
		{
			tempchar[0] = InfoModel3.AutoShift_Status_SR_Value_DOWN;
			EepromWrite(ADDRESS_AUTOSHIFT_STATUS_SR_DOWN, (unsigned char*)(&tempchar[0]), 1); 
		}
		ReadEquipmentSetting3();
		if((OldInfoModel3.AutoShift_Status_SR_Value_DOWN == InfoModel3.AutoShift_Status_SR_Value_DOWN))
		{			
			return SAVE_SUCCESS;
		}
		else
		{
			InfoModel3.AutoShift_Status_SR_Value_DOWN = OldInfoModel3.AutoShift_Status_SR_Value_DOWN;
			return SAVE_FAILURE;
		}
	}
	else
		return SAVE_FAILURE;

	
}


unsigned char DCSR_Setting()
{
	unsigned char tempchar[1];

	if(ScreenIndex == SCREEN_STATE_MENU_EQUIPMENT_DCSR_MODE_SET)
	{
		if(InfoModel1.DCSR_Status < 2)
		{
			tempchar[0] = InfoModel1.DCSR_Status;
			EepromWrite(ADDRESS_DCSR_STATUS, (unsigned char*)(&tempchar[0]), 1); 
		}
		ReadEquipmentSetting1();
		if((OldInfoModel1.DCSR_Status == InfoModel1.DCSR_Status))
		{
			return SAVE_SUCCESS;
		}
		else
		{
			InfoModel1.DCSR_Status = OldInfoModel1.DCSR_Status;
			return SAVE_FAILURE;
		}
	}
	else if(ScreenIndex == SCREEN_STATE_MENU_EQUIPMENT_DCSR_SPEED_BLOCKING_SET)
	{
		if((InfoModel1.DCSR_Speed_Block >= MIN_BLOCK_SPEED_LIMIT)
			&&(InfoModel1.DCSR_Speed_Block <= MAX_BLOCK_SPEED_LIMIT))
		{
			tempchar[0] = InfoModel1.DCSR_Speed_Block;
			EepromWrite(ADDRESS_DCSR_SPEED_BLOCK, (unsigned char*)(&tempchar[0]), 1); 
		}
		ReadEquipmentSetting1();
		if((OldInfoModel1.DCSR_Speed_Block == InfoModel1.DCSR_Speed_Block))
		{
			return SAVE_SUCCESS;
		}
		else
		{
			InfoModel1.DCSR_Speed_Block = OldInfoModel1.DCSR_Speed_Block;
			return SAVE_FAILURE;
		}
	}
	else if(ScreenIndex == SCREEN_STATE_MENU_EQUIPMENT_DCSR_SPEED_RESTORE_SET)
	{
		if((InfoModel1.DCSR_Speed_Restore >= MIN_RESTORE_SPEED_LIMIT)
			&& (InfoModel1.DCSR_Speed_Restore <= MAX_RESTORE_SPEED_LIMIT))
		{
			tempchar[0] = InfoModel1.DCSR_Speed_Restore;
			EepromWrite(ADDRESS_DCSR_SPEED_RESTORE, (unsigned char*)(&tempchar[0]), 1); 
		}
		ReadEquipmentSetting1();
		if((OldInfoModel1.DCSR_Speed_Restore == InfoModel1.DCSR_Speed_Restore))
		{
			return SAVE_SUCCESS;
		}
		else
		{
			InfoModel1.DCSR_Speed_Restore = OldInfoModel1.DCSR_Speed_Restore;
			return SAVE_FAILURE;
		}
	}	
	else
		return SAVE_FAILURE;

}

unsigned char SpeedLimit_Setting(void)
{
	unsigned char tempchar[2];
	
	if(ScreenIndex == SCREEN_STATE_MENU_EQUIPMENT_MAXSPEEDLIMIT_FUNC_SET)
	{
		if(InfoModel2.SpeedLimitStatus < 2)
		{
			tempchar[0] = InfoModel2.SpeedLimitStatus;
			EepromWrite(ADDRESS_SPEED_LIMIT_STATUS, (unsigned char*)(&tempchar[0]), 1); 
		}
		ReadEquipmentSetting2();
		if((OldInfoModel2.SpeedLimitStatus == InfoModel2.SpeedLimitStatus))
		{
			InitSpeedLimitProcess();
			return SAVE_SUCCESS;
		}
		else
		{
			InfoModel2.SpeedLimitStatus = OldInfoModel2.SpeedLimitStatus;
			return SAVE_FAILURE;
		}
	}
	else if(ScreenIndex == SCREEN_STATE_MENU_EQUIPMENT_MAXSPEEDLIMIT_SPEED_SET)
	{

		if((InfoModel1.ModelInfo >= MODEL_35D_9S)&&(InfoModel1.ModelInfo <= MODEL_50D_9S))
		{
			if((InfoModel2.SpeedLimitValue >= 5)
				&& (InfoModel2.SpeedLimitValue <= 25))
			{
				tempchar[0] = InfoModel2.SpeedLimitValue;
				EepromWrite(ADDRESS_SPEED_LIMIT_VALUE, (unsigned char*)(&tempchar[0]), 1); 
			}

		}
		else
		{
	
			if((InfoModel2.SpeedLimitValue >= MIN_SPEED_LIMIT)
				&& (InfoModel2.SpeedLimitValue <= MAX_SPEED_LIMIT))
			{
				tempchar[0] = InfoModel2.SpeedLimitValue;
				EepromWrite(ADDRESS_SPEED_LIMIT_VALUE, (unsigned char*)(&tempchar[0]), 1); 
			}
		}
		ReadEquipmentSetting2();
		if((OldInfoModel2.SpeedLimitValue == InfoModel2.SpeedLimitValue))
		{
			InitSpeedLimitProcess();		
			return SAVE_SUCCESS;
		}
		else
		{
			InfoModel2.SpeedLimitValue = OldInfoModel2.SpeedLimitValue;
			return SAVE_FAILURE;
		}
	}
	else
		return SAVE_FAILURE;
}

unsigned char HAC_Setting(void)
{
	unsigned char tempchar[2];
	
	if(InfoModel2.HAC_Status < 2)
	{
		tempchar[0] = InfoModel2.HAC_Status;
		EepromWrite(ADDRESS_HAC_STATUS, (unsigned char*)(&tempchar[0]), 1); 
	}
	ReadEquipmentSetting2();
	if((OldInfoModel2.HAC_Status == InfoModel2.HAC_Status))
	{
		return SAVE_SUCCESS;
	}
	else
	{
		InfoModel2.HAC_Status = OldInfoModel2.HAC_Status;
		return SAVE_FAILURE;
	}
}

unsigned char EquipmentEtc_Save(void)
{
	unsigned char tempchar[2];

	if(ScreenIndex == SCREEN_STATE_MENU_EQUIPMENT_CLUTCHPROTECTION_SET)
	{
		if(InfoModel2.ClutchProtectionAlarm< 2)
		{
			tempchar[0] = InfoModel2.ClutchProtectionAlarm;
			EepromWrite(ADDRESS_CLUTCH_PROTECTION, (unsigned char*)(&tempchar[0]), 1); 
		}
		ReadEquipmentSetting2();
		if((OldInfoModel2.ClutchProtectionAlarm == InfoModel2.ClutchProtectionAlarm))
		{
			return SAVE_SUCCESS;
		}
		else
		{
			InfoModel2.ClutchProtectionAlarm = OldInfoModel2.ClutchProtectionAlarm;
			return SAVE_FAILURE;
		}
	}
	else if(ScreenIndex == SCREEN_STATE_MENU_EQUIPMENT_ZEROSTART_MODE_SET)
	{
		if(InfoModel2.ZeroStart< 2)
		{
			tempchar[0] = InfoModel2.ZeroStart;
			EepromWrite(ADDRESS_ZERO_START, (unsigned char*)(&tempchar[0]), 1); 
		}
		ReadEquipmentSetting2();
		if((OldInfoModel2.ZeroStart == InfoModel2.ZeroStart))
		{
			return SAVE_SUCCESS;
		}
		else
		{
			InfoModel2.ZeroStart = OldInfoModel2.ZeroStart;
			return SAVE_FAILURE;
		}
	}
	else if(ScreenIndex == SCREEN_STATE_MENU_EQUIPMENT_ZEROSTART_ACCEL_SET)
	{
		if(InfoModel2.ZeroStart_AccelValue <= 12) //++,--, 230202 ysm, FSCU 75->25->12
		{
			tempchar[0] = InfoModel2.ZeroStart_AccelValue;
			EepromWrite(ADDRESS_ZERO_START_ACCEL_VALUE, (unsigned char*)(&tempchar[0]), 1); 
		}
		ReadEquipmentSetting2();
		if((OldInfoModel2.ZeroStart_AccelValue== InfoModel2.ZeroStart_AccelValue))
		{
			return SAVE_SUCCESS;
		}
		else
		{
			InfoModel2.ZeroStart_AccelValue = OldInfoModel2.ZeroStart_AccelValue;
			return SAVE_FAILURE;
		}
	}
	else
	{
		if(InfoModel2.SeatBeltInterlock< 2)
		{
			tempchar[0] = InfoModel2.SeatBeltInterlock;
			EepromWrite(ADDRESS_SEAT_BELT_INTERLOCK, (unsigned char*)(&tempchar[0]), 1); 
		}
		ReadEquipmentSetting2();
		if((OldInfoModel2.SeatBeltInterlock == InfoModel2.SeatBeltInterlock))
		{
			return SAVE_SUCCESS;
		}
		else
		{
			InfoModel2.SeatBeltInterlock = OldInfoModel2.SeatBeltInterlock;
			return SAVE_FAILURE;
		}
	}
}



void Change_MasterPassword()
{
	unsigned char i;	
	unsigned char tempchar[5] = {0,};

	PASSWORD_DATA* Password_MASTER;

	Password_MASTER = (PASSWORD_DATA*)&tempchar[0];
	
	Password_MASTER->Data1=HCESPN.H954_[0]&0x0f;
	Password_MASTER->Data2=HCESPN.H954_[1]&0x0f;
	Password_MASTER->Data3=HCESPN.H954_[2]&0x0f;
	Password_MASTER->Data4=HCESPN.H954_[3]&0x0f;
	Password_MASTER->Data5=HCESPN.H954_[4]&0x0f;

	Password_MASTER->Data6=HCESPN.H954_[5]&0x0f;
	Password_MASTER->Data7=HCESPN.H954_[6]&0x0f;
	Password_MASTER->Data8=HCESPN.H954_[7]&0x0f;
	Password_MASTER->Data9=HCESPN.H954_[8]&0x0f;
	Password_MASTER->Data10=HCESPN.H954_[9]&0x0f;
	
	EepromWrite(ADDRESS_MASTER_PASSWORD, (unsigned char*)(&tempchar[0]), 5);

	for(i=0; i<11; i++)
	{
		HCESPN.H954[i] = 0xff;

	}	
	
	for(i=0; i<10; i++)
	{
		if(HCESPN.H954_[i] == 0xff)
		{
			HCESPN.H954[i] = 0x2a;
			break;
		}
		else
		{
			HCESPN.H954[i] = HCESPN.H954_[i];
		}
							
	}	
	
}

void Make_HCESPN_MasterPassword()
{
	unsigned char i;
	unsigned char tempchar[5] = {0,};
	unsigned char tempchar1[11];
	
	EepromRead(ADDRESS_MASTER_PASSWORD, (unsigned char*)(&tempchar[0]), 5);

	memset(&tempchar1,0xff,sizeof(tempchar1));
	memset(&HCESPN.H954,0xff,sizeof(HCESPN.H954));

	for(i=0; i<5; i++)
	{
		tempchar1[2*i] = tempchar[i]&0xf;

		if(tempchar1[2*i] == 0xf)
		{
			tempchar1[2*i] = 0x2a;
			break;
		}
		
		tempchar1[2*i+1] = (tempchar[i]&0xf0)>>4;

		if(tempchar1[2*i+1] == 0xf)
		{
			tempchar1[2*i+1] = 0x2a;
			break;
		}			

	}

	for(i=0; i<10; i++)
	{
		HCESPN.H954[i] = tempchar1[i];

		if(tempchar1[i] == 0x2a)
			break;
		else
		{
			HCESPN.H954[i] = tempchar1[i] + 0x30;
			
		}

	}

	if(i>=10)
		HCESPN.H954[10] = 0x2a;	

}


void Make_HCESPN_UserPassword()
{
	unsigned char i;
	unsigned char tempchar[5] = {0,};
	unsigned char tempchar1[11];
	
	EepromRead(ADDRESS_USER_PASSWORD, (unsigned char*)(&tempchar[0]), 5);

	memset(&tempchar1,0xff,sizeof(tempchar1));
	memset(&HCESPN.H955,0xff,sizeof(HCESPN.H955));

	for(i=0; i<5; i++)
	{
		tempchar1[2*i] = tempchar[i]&0xf;

		if(tempchar1[2*i] == 0xf)
		{
			tempchar1[2*i] = 0x2a;
			break;
		}
		
		tempchar1[2*i+1] = (tempchar[i]&0xf0)>>4;

		if(tempchar1[2*i+1] == 0xf)
		{
			tempchar1[2*i+1] = 0x2a;
			break;
		}			

	}

	for(i=0; i<10; i++)
	{
		HCESPN.H955[i] = tempchar1[i];

		if(tempchar1[i] == 0x2a)
			break;
		else
		{
			HCESPN.H955[i] = tempchar1[i] + 0x30;
			
		}

	}

	if(i>=10)
		HCESPN.H955[10] = 0x2a;	

}

void Reset_UserPassword()
{
	UCHAR tmpBuf[5];
	PASSWORD_DATA* Password_USER;

	Password_USER = (PASSWORD_DATA*)&tmpBuf[0];
	
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
	
	EepromWrite(ADDRESS_USER_PASSWORD, (unsigned char*)(&tmpBuf[0]), 5);

	Make_HCESPN_UserPassword();
	RCMData_UserPasswordChange = 1;

}


unsigned char User_Master_password_check()
{
	unsigned char tempchar[5];

	EepromRead(ADDRESS_USER_PASSWORD, (unsigned char*)(&tempchar[0]), 5);

	COUNT_FLAG.ShowMenuMaster = 0;
	
	if(memcmp(&tempchar[0],&InfoPassword,5) ==0) // user password correct
	{
		
		return USER_PASSWORD_CHECK_OK;
	}
	else  // user password incorrect
	{
		return Master_password_check(USER_PASSWORD_CHECK_REQUEST);
	}
}

unsigned char ESL_Master_password_check()
{
	unsigned char tempchar[5];
	unsigned char tempchar1[5];

	EepromRead(ADDRESS_ESL_PASSWORD, (unsigned char*)(&tempchar[0]), 5); 

	memcpy(&tempchar1[0],&InfoPassword,5);

	COUNT_FLAG.ShowMenuMaster = 0; //++,--, 210705 ysm, MASTER_PASS

	if(memcmp(&tempchar[0],&tempchar1[0],5) ==0) // ESL password correct
	{
		if(Flag_ESL)	
		{
			Flag_ESL=0;
		}		
		
		return ESL_PASSWORD_CHECK_OK;
	}
	else  // ELS password incorrect
	{
		return Master_password_check(ESL_PASSWORD_CHECK_REQUEST);
	}
}

unsigned char Master_password_check(unsigned char mode)
{
	unsigned char tempchar[5];
	unsigned char tempchar1[5];

	EepromRead(ADDRESS_MASTER_PASSWORD, (unsigned char*)(&tempchar[0]), 5); 

	memcpy(&tempchar1[0],&InfoPassword,5);

	if(memcmp(&tempchar[0],&tempchar1[0],5) ==0) // master password correct
	{
		if(mode==ESL_PASSWORD_CHECK_REQUEST) //ESL_mode
		{
			if(Flag_ESL)	
			{
				Flag_ESL=0;
			}
		}
		
		COUNT_FLAG.ShowMenuMaster = 1; //++,--, 210705 ysm, MASTER_PASS
		
		return MASTER_PASSWORD_CHECK_OK;
	}
	else  // user password & Master password incorrect
	{
		COUNT_FLAG.ShowMenuMaster = 0; //++,--, 210705 ysm, MASTER_PASS
	
		if(mode==ESL_PASSWORD_CHECK_REQUEST) //ESL_mode
			return ESL_PASSWORD_CHECK_NOT_OK;
		else
			return USER_PASSWORD_CHECK_NOT_OK;
	}
}

unsigned char User_password_Change(void)
{
	unsigned char tempchar[5];
	unsigned char tempchar1[5];

	memcpy(&tempchar,&InfoPassword,5);

	EepromWrite(ADDRESS_USER_PASSWORD, (unsigned char*)(&tempchar[0]), 5); 
	EepromRead(ADDRESS_USER_PASSWORD, (unsigned char*)(&tempchar1[0]), 5); 

	COUNT_FLAG.ShowMenuMaster = 0;

	if(memcmp(&tempchar[0],&tempchar1[0],5) ==0) // user password changed
	{
		Make_HCESPN_UserPassword();
		RCMData_UserPasswordChange = 1;
		return USER_PASSWORD_CHANGE_OK;// user password changed
	}
	else 
	{
		return USER_PASSWORD_CHANGE_NOT_OK; // user password not changed
	}
}

unsigned char ESL_password_Change(void)
{
	unsigned char tempchar[5];
	unsigned char tempchar1[5];

	memcpy(&tempchar[0],&InfoPassword,5);

	EepromWrite(ADDRESS_ESL_PASSWORD, (unsigned char*)(&tempchar[0]), 5); 
	EepromRead(ADDRESS_ESL_PASSWORD, (unsigned char*)(&tempchar1[0]), 5); 

	COUNT_FLAG.ShowMenuMaster = 0; //++,--, 210705 ysm, MASTER_PASS

	if(memcmp(&tempchar[0],&tempchar1[0],5) ==0) // ESL password changed
	{
		return ESL_PASSWORD_CHANGE_OK; // ESL password changed
	}
	else 
	{
		return ESL_PASSWORD_CHANGE_NOT_OK; // ESL password not changed
	}
}
unsigned char Unset_H1098(unsigned char maintenance_item)
{
	unsigned char return_value;

	switch(maintenance_item)
	{	
		case 50 :			return_value = MAINTENANCE_ENGINE_OIL_FILTER ;		break;
		case 51 :			return_value = MAINTENANCE_TRANSMISSION_OIL_FILTER ;break;
		case 52 :			return_value = MAINTENANCE_DIFFERENTIAL_GEAR_OIL;	break;
		case 11 :			return_value = MAINTENANCE_HYD_AIR_BREATHER_ELEMENT;break;
		case 7:				return_value = MAINTENANCE_HYD_OIL_RETURN_FILTER;	break;
		case 53:			return_value = MAINTENANCE_FUEL_FILTER;				break;
		case 54 :			return_value = MAINTENANCE_FAN_BELT;				break;
		case 55 :			return_value = MAINTENANCE_BRAKE_OIL ;				break;
		case 12:			return_value = MAINTENANCE_AIR_CLEANER_ELEMENT ;	break;
		case 21:			return_value = MAINTENANCE_HYD_SUCTION_STRAINER ;	break;
		case 13:			return_value = MAINTENANCE_RADIATIOR_COOLANT ;		break;
		case 4:				return_value = MAINTENANCE_HYD_OIL ;				break;
		case 61 :			return_value = MAINTENANCE_LPG_VAPORIZER;			break;
		case 62 :			return_value = MAINTENANCE_SPARK_PLUG;				break;
		case 63 :			return_value = MAINTENANCE_LOCK_OFF_VALVE_FILTER;	break;		
		case 59 :			return_value = MAINTENANCE_DPF_ASH_CLEANING;		break;
		case 20 :			return_value = MAINTENANCE_BRAKE_COOLING_OIL_FILTER;break;
		default:			return_value = MAINTENANCE_ENGINE_OIL_FILTER;		break;	
	}

	return return_value;


}

unsigned char Set_H1098(unsigned char maintenance_item)
{
	unsigned char return_value;

	switch(maintenance_item)
	{	
		case MAINTENANCE_ENGINE_OIL_FILTER:				return_value = 50;	break;
		case MAINTENANCE_TRANSMISSION_OIL_FILTER:		return_value = 51;	break;
		case MAINTENANCE_DIFFERENTIAL_GEAR_OIL:			return_value = 52;	break;
		case MAINTENANCE_HYD_AIR_BREATHER_ELEMENT:		return_value = 11;	break;
		case MAINTENANCE_HYD_OIL_RETURN_FILTER:			return_value = 7;	break;
		case MAINTENANCE_FUEL_FILTER:					return_value = 53;	break;
		case MAINTENANCE_FAN_BELT:						return_value = 54;	break;
		case MAINTENANCE_BRAKE_OIL:						return_value = 55;	break;
		case MAINTENANCE_AIR_CLEANER_ELEMENT:			return_value = 12;	break;
		case MAINTENANCE_HYD_SUCTION_STRAINER:			return_value = 21;	break;
		case MAINTENANCE_RADIATIOR_COOLANT:				return_value = 13;	break;
		case MAINTENANCE_HYD_OIL:						return_value = 4;	break;
		case MAINTENANCE_LPG_VAPORIZER:					return_value = 61;	break;
		case MAINTENANCE_SPARK_PLUG:					return_value = 62;	break;
		case MAINTENANCE_LOCK_OFF_VALVE_FILTER:			return_value = 63;	break;
		case MAINTENANCE_DPF_ASH_CLEANING:				return_value = 59;	break;
		case MAINTENANCE_BRAKE_COOLING_OIL_FILTER:		return_value = 20;	break;
		default:										return_value = 50;	break;		
	}

	return return_value;


}

void Check_Maintenance_Info(unsigned char request)
{
	unsigned char i;
	unsigned short interval = temp_interval/50;

	switch(request)
	{
		case REQUEST_REPLACMENT_CONFIRMATION: // Replacement 

			if(gMaintenanceItem <= MAINTENANCE_TOTAL)
			{
				Maintenance_Data[gMaintenanceItem].Maintenance_Replacement_Count++;


				if(gMaintenanceItem == MAINTENANCE_ENGINE_OIL_FILTER)
				{
					if(Maintenance_Data[gMaintenanceItem].Maintenance_Replacement_Count==1)
					{
						if((InfoModel1.ModelInfo >= MODEL_35L_9)&&(InfoModel1.ModelInfo <= MODEL_50L_9))
							Maintenance_Data[gMaintenanceItem].Maintenance_Interval=8; // 초기 50 이후 400 시간
						else if((InfoModel1.ModelInfo >= MODEL_35D_9S)&&(InfoModel1.ModelInfo <= MODEL_50D_9S))
							Maintenance_Data[gMaintenanceItem].Maintenance_Interval=5; // 초기 50 이후 250시간
						else if((InfoModel1.ModelInfo >= MODEL_25LC_9)&&(InfoModel1.ModelInfo <= MODEL_33LC_9))
							Maintenance_Data[gMaintenanceItem].Maintenance_Interval=5; // 초기 50 이후 250시간
						else if((InfoModel1.ModelInfo >= MODEL_25L_9A)&&(InfoModel1.ModelInfo <= MODEL_35LN_9A))
							Maintenance_Data[gMaintenanceItem].Maintenance_Interval=10; // 초기 50 이후 500시간
						else
							Maintenance_Data[gMaintenanceItem].Maintenance_Interval=10; // 초기 50 이후 500시간
					}
				}
				else if((gMaintenanceItem==MAINTENANCE_TRANSMISSION_OIL_FILTER) 
					||(gMaintenanceItem==MAINTENANCE_DIFFERENTIAL_GEAR_OIL))
				{
					if(Maintenance_Data[gMaintenanceItem].Maintenance_Replacement_Count==1)
					{
						Maintenance_Data[gMaintenanceItem].Maintenance_Interval=20; // 초기 100 이후 1000으로 변경 
					}
				}
				else if(gMaintenanceItem==MAINTENANCE_HYD_OIL_RETURN_FILTER)
				{
					if(Maintenance_Data[gMaintenanceItem].Maintenance_Replacement_Count==1)
					{
						Maintenance_Data[gMaintenanceItem].Maintenance_Interval=20; // 초기 250 이후 1000으로 변경 
					}
				}

				for (i = 8; i >= 1; i--)
				{
					Maintenance_Data[gMaintenanceItem].Maintenance_History[i] = Maintenance_Data[gMaintenanceItem].Maintenance_History[i - 1];
				}

				Maintenance_Data[gMaintenanceItem].Maintenance_Hourmeter = HCESPN.Hourmeter_Hour_701;
				
				Maintenance_Data[gMaintenanceItem].Maintenance_History[0] = Maintenance_Data[gMaintenanceItem].Maintenance_Hourmeter;

				RCMData_MaintenanceChange = 1;
				
				RCMData_MaintenanceItem = HCESPN.H1098 = Set_H1098(gMaintenanceItem);					
			}		
			break;	

		case REQUEST_INTERVAL_CHANGE:	// Interval Change 			
            
			if ((gMaintenanceItem <= MAINTENANCE_TOTAL) && (interval <= 250))
			{
				Maintenance_Data[gMaintenanceItem].Maintenance_Interval = interval;     				
				RCMData_MaintenanceChange = 2;
				
                RCMData_MaintenanceItem = HCESPN.H1098 = Set_H1098(gMaintenanceItem);
			}
		break;	
	}
}

//++, 221226 ysm, FSCU
void Save_FSCU_Status(void)
{
	unsigned char tempchar[1];	

	tempchar[0] = COUNT_FLAG.Flag_FSCU_Enable;
	EepromWrite(ADDRESS_FSCU_ENABLE, (unsigned char*)(&tempchar[0]), 1); 

	COUNT_FLAG.Comm_Error_FSCU = 0; //++,--, 230118 ysm, FSCU_BUG_FIX

}

//--, 221226 ysm, FSCU

void Save_maintenance(void)
{
	unsigned char i;	

	for (i = 0; i < MAINTENANCE_TOTAL; i++)
	{
		EepromWrite((i*32)+ADDRESS_MAINTENANCE, (unsigned char*)(&Maintenance_Data[i]), sizeof(st_Maintenance));
	}

	
}
//-----------------------------------------------------------------------------------------------------------
//++, 220829 ysm, 9A
void AD_HYD_OIL_PRESSURE(void)
{
	if(((InfoModel1.ModelInfo >= MODEL_25L_9A)&&(InfoModel1.ModelInfo <= MODEL_35LN_9A))||
		((InfoModel1.ModelInfo >= MODEL_25D_9HDI)&&(InfoModel1.ModelInfo <= MODEL_50DN_9HDI)))
	{

		AD_DATA.C_HYD_PRESURE_TEMP[1] = (unsigned short)(HYD_OIL_PRESSURE*10000.0); 
		AD_DATA.C_HYD_PRESSURE = (USHORT)((B_0 * AD_DATA.C_HYD_PRESURE_TEMP[1]) + (B_1 * AD_DATA.C_HYD_PRESURE_TEMP[0]) - (A_1 * AD_DATA.C_HYD_PRESSURE));
	
		AD_DATA.C_HYD_PRESURE_TEMP[0] = AD_DATA.C_HYD_PRESURE_TEMP[1];

		if(AD_DATA.C_HYD_PRESSURE >= 10000)
		{
			AD_DATA.C_HydPressBar = (( 250./4. ) * (((float)(AD_DATA.C_HYD_PRESSURE) /10000) - 1 )) * 1.019716; 
			AD_DATA.C_HydPress = (( 250./4. ) * (((float)(AD_DATA.C_HYD_PRESSURE) /10000) - 1 )); 
		}
		else
		{
			AD_DATA.C_HydPressBar = 0xffff; 
			AD_DATA.C_HydPress = 0xffff;				
		}
	}
}
//--, 220829 ysm, 9A


void AD_HYD_OIL_TEMPERATURE ( void )
{

	AD_DATA.C_HYD_TEMP_TEMP[1] = (unsigned short)(HYD_TEMP_SENSOR*10000.0); 
	AD_DATA.C_HYD_TEMPERATURE = (USHORT)((B_0 * AD_DATA.C_HYD_TEMP_TEMP[1]) + (B_1 * AD_DATA.C_HYD_TEMP_TEMP[0]) - (A_1 * AD_DATA.C_HYD_TEMPERATURE));
	
	AD_DATA.C_HYD_TEMP_TEMP[0] = AD_DATA.C_HYD_TEMP_TEMP[1];
	
	if(AD_DATA.C_HYD_TEMPERATURE >= 10000)
	{
		AD_DATA.C_HydTempBar = (( 250./4. ) * (((float)(AD_DATA.C_HYD_TEMPERATURE) /10000) - 1 )) * 1.019716; 
		AD_DATA.C_HydTemp = (( 165./4. ) * (((float)(AD_DATA.C_HYD_TEMPERATURE) /10000) - 1 )); 
	}
	else
	{
		AD_DATA.C_HydTempBar = 0xffff; 
		AD_DATA.C_HydTemp = 0xffff;
	}
}

//++, 210421 ysm, 9S
void AD_COOLANT_TEMPERATURE(void)
{

	float coolant_R;
	float level_R1 = 151.9;
	float level_R2 = 51.9;
	float level_R3 = 27.4;
	float level_R4 = 16.0;
	//Fuel_R=(((3.3*(10+20))*Ref_R*Fuel_Level_rowdata*3)/(30*((4095*2*5)-(3.3*(1+2)*Fuel_Level_rowdata))-(3.3*Fuel_Level_rowdata*3*Ref_R)))*1000;
	
	if((InfoModel1.ModelInfo >= MODEL_35D_9S)&&(InfoModel1.ModelInfo <= MODEL_50D_9S))
	{

		//coolant_R=(((3.3*(10+10))*ref_R*Coolant_Temp_rowdata*2)/(20*((4095*2*5)-(3.3*(1+1)*Coolant_Temp_rowdata))-(3.3*Coolant_Temp_rowdata*2*ref_R)))*1000;

		coolant_R = (6.6*Coolant_Temp_rowdata*50/4095) / (5-6.6*Coolant_Temp_rowdata/4095);

		AD_DATA.C_COOLANT_TEMPERATURE = (unsigned short)coolant_R;
		
		if(coolant_R > level_R1)
		{
			RX_ECU_DATA_65262.CoolantTemp_110 = 0 + 40;
		
		}
		else if((coolant_R <= level_R1)&&(coolant_R > level_R2)) //151.9 ~ 51.9
		{
			RX_ECU_DATA_65262.CoolantTemp_110 = (unsigned char)(((-0.3)*coolant_R) + 95.57) + 40;
		}
		else if((coolant_R <= level_R2)&&(coolant_R > level_R3)) //51.9 ~ 27.4
		{
			RX_ECU_DATA_65262.CoolantTemp_110 = (unsigned char)(((2.4)*coolant_R) - 44.5 ) + 40;
		}
		else if((coolant_R <= level_R3)&&(coolant_R > level_R4)) //27.4 ~ 16.0
		{
			RX_ECU_DATA_65262.CoolantTemp_110 = (unsigned char)(((-1.75)*coolant_R) + 148 ) + 40;

		}
		else
		{
			RX_ECU_DATA_65262.CoolantTemp_110 = 120 + 40;

		}

		
	}
}

void AD_BRAKE_OIL_PRESSURE ( void )
{

	float a_value = 25;
	float b_value = 25;
	float brake_pressure;


	if((InfoModel1.ModelInfo >= MODEL_35D_9S)&&(InfoModel1.ModelInfo <= MODEL_50D_9S))
	{

		AD_DATA.C_BRAKE_OIL_PRESSURE_TEMP[1] = BRAKE_OIL_PRESSURE;
		AD_DATA.C_BRAKE_OIL_PRESSURE = (USHORT)((B_0 * AD_DATA.C_BRAKE_OIL_PRESSURE_TEMP[1]) + (B_1 * AD_DATA.C_BRAKE_OIL_PRESSURE_TEMP[0]) - (A_1 * AD_DATA.C_BRAKE_OIL_PRESSURE));

		AD_DATA.C_BRAKE_OIL_PRESSURE_TEMP[0] = AD_DATA.C_BRAKE_OIL_PRESSURE_TEMP[1];

		//++, 210819 ysm, 9S
		if((AD_DATA.C_BRAKE_OIL_PRESSURE_TEMP[0] >= 1)&&(AD_DATA.C_BRAKE_OIL_PRESSURE_TEMP[0] <= 5)) // 1~5V -> 0~100 BAR
		{
			brake_pressure = a_value * AD_DATA.C_BRAKE_OIL_PRESSURE_TEMP[0] - b_value;
		}
		else if(AD_DATA.C_BRAKE_OIL_PRESSURE_TEMP[0] < 1)
		{	
			AD_DATA.C_BRAKE_OIL_PRESSURE_TEMP[0] = 0;
			brake_pressure = 0;

		}
		else
		{
			AD_DATA.C_BRAKE_OIL_PRESSURE_TEMP[0] = 5;
			brake_pressure = 100;

		}
		//--, 210819 ysm, 9S

		if(brake_pressure < 0)
		{
			brake_pressure = 0;
		}

		AD_DATA.C_BRAKE_OIL_PressureBar = brake_pressure;

	}

}
//--, 210421 ysm, 9S


void AD_TM_OIL_PRESSURE ( void )
{
	AD_DATA.C_TM_OIL_PRESSURE_TEMP[1] = (unsigned short)(TM_OIL_PRESSURE*10000.0); 
	AD_DATA.C_TM_LIL_PRESSURE = (USHORT)((B_0 * AD_DATA.C_TM_OIL_PRESSURE_TEMP[1]) + (B_1 * AD_DATA.C_TM_OIL_PRESSURE_TEMP[0]) - (A_1 * AD_DATA.C_TM_LIL_PRESSURE));

	AD_DATA.C_TM_OIL_PRESSURE_TEMP[0] = AD_DATA.C_TM_OIL_PRESSURE_TEMP[1];
// ++, 200326 bwk
#if 0
	AD_DATA.C_HydTempBar = (( 250./4. ) * (((float)(AD_DATA.C_LOAD_SENSOR) /10000) - 1 )) * 1.019716; 
	AD_DATA.C_HydTemp = (( 165./4. ) * (((float)(AD_DATA.C_LOAD_SENSOR) /10000) - 1 )); 
#else
	if(AD_DATA.C_TM_LIL_PRESSURE >= 10000)
	{
		AD_DATA.C_TMOilPressureBar = (( 25./2. ) * (((float)(AD_DATA.C_TM_LIL_PRESSURE) /10000) - 1 )) * 1.019716; 
	}
	else
	{
		AD_DATA.C_TMOilPressureBar = 0xffff; 
	}

#endif
}

/**************************************************************************/
/*                              LOAD SENSER                               */
/**************************************************************************/
void Check_Load_buzzer(void)
{
	UINT32 tempRound;

	// ++, 200401 bwk
	#if 0
	if(EngineStartFlag && InfoDisplaySetting.LoadBuzzerSet)
	#else
	if(EngineStartFlag)
	#endif
	// --, 200401 bwk
	{
		if(AD_DATA.OldWeight_Current != AD_DATA.Weight_Current)
		{
			AD_DATA.OldWeight_Current = AD_DATA.Weight_Current;

			if(AD_DATA.OldWeight_Current>=0xfbff)		WeightBuzzerOn=0;
			else
			{
				if(InfoDisplaySetting.WeightDisplayUnit == WEIGHT_DISPLAYSET_100)
				{
					if((AD_DATA.OldWeight_Current % 10) > 4)
						tempRound = AD_DATA.OldWeight_Current + (10-(AD_DATA.OldWeight_Current % 10));
					else
						tempRound = AD_DATA.OldWeight_Current;

					if((tempRound/10) >= InfoModel1.TonInfo)	WeightBuzzerOn=1;
					else		WeightBuzzerOn=0;
				}
				else
				{
					if((AD_DATA.OldWeight_Current/10) >= InfoModel1.TonInfo)		WeightBuzzerOn=1;
					else		WeightBuzzerOn=0;
				}
			}
		}
	}
	else
		WeightBuzzerOn = 0;
}

unsigned char Check_load_Sensor(unsigned short Data)
{
	if(COUNT_FLAG.Check_load_sensor++ > 3000)
	{
		COUNT_FLAG.Check_load_sensor=0;
		return 2;
	}
	else if(COUNT_FLAG.Check_load_sensor < 500) // 5s 동안 average 하여 얻은 값을 Ref 로 한다. 
	{
		Load_data[0] = Data;	
	}
	else
	{
		if(Load_data[0] >= Data)
		{
			if(Load_data[0]-Data < 500)
			{
				COUNT_FLAG.Check_load_sensor_OK++;
			}
			else
			{
				Load_data[0] = Data;
				COUNT_FLAG.Check_load_sensor_OK=0;
			}
		}
		else if(Data > Load_data[0])
		{
			if((Data - Load_data[0]) < 500) 
			{
				COUNT_FLAG.Check_load_sensor_OK++;
			}
			else
			{
				Load_data[0] = Data;
				COUNT_FLAG.Check_load_sensor_OK=0;
			}
		}
	}

	if(COUNT_FLAG.Check_load_sensor_OK>300)
	{
		COUNT_FLAG.Check_load_sensor_OK=0;
		AD_DATA.C_Max_Volt = Load_data[0];
		return 1;
	}
	else
	{
		return 0;
	}
}

void AD_LOAD_SENSER ( void )
{
	unsigned short tempshort = 0;
	unsigned char tempchar = 0;
	unsigned char Save_Load_Sensor = 0;

	AD_DATA.C_LOAD_SENSOR_TEMP[1] = (unsigned short)(LOAD_SENSOR*10000.0); 
	AD_DATA.C_LOAD_SENSOR = (USHORT)((B_0 * AD_DATA.C_LOAD_SENSOR_TEMP[1]) + (B_1 * AD_DATA.C_LOAD_SENSOR_TEMP[0]) - (A_1 * AD_DATA.C_LOAD_SENSOR));

	AD_DATA.C_LOAD_SENSOR_TEMP[0] = AD_DATA.C_LOAD_SENSOR_TEMP[1];

	//////////// 43PP8-15 /////////////////
	//////////// MLH350BST12E /////////////////
	// 21HS-10160 	// Type : MBS1250
	// 1 kgf/cm2 = 0.980665 bar
	// 1 bar = 1.019716 kgf/cm2		==> 다를수도 있음 
	// Zero Output = 0.5 Volt
	// Full Scale Span 4 Volt ( 0.5 to 4.5 Volt )
	// Pressure Range : 0 to 350 bar
	// <=> 1m Volt = 0.0875 bar
	// 1bar = ap.  11.3 mVolt

	// Tare : 0.8502 Volt
	// Max_Ton : 17.0 ton
	// Max_Ton Volt : 2.2771 Volt

	// 1bar = ap.  11.3 mVolt                
	// 부하 무게 조정 - 무 부하 조정
        // 200326 무게가 무부하가 0.5V가 안나와서 보정안됨ㅋ임시로 0.5 V -> 0.45V로 수정 
	if ( Equipment_Calibration == CALIBRATION_WEIGHT_UNLOAD ) // Tare(1)
	{
		AD_DATA.C_Tare_Volt = AD_DATA.C_LOAD_SENSOR;

		if ( AD_DATA.C_Tare_Volt >= 4500 )    AD_DATA.C_Bar_Tare = (( 350./4. ) * (((float)(AD_DATA.C_Tare_Volt) /10000) - 0.5 ));  // ++, --, 200326 bwk 5000 -> 4500, //++,--, 210812 ysm, 1.019716 -> 1
		else                                  AD_DATA.C_Bar_Tare = 0;
		AD_DATA.C_D = (( AD_DATA.C_A / 2)*  AD_DATA.C_Bar_Tare );                          

		tempshort =  AD_DATA.C_Tare_Volt;
		EepromWrite(ADDRESS_TARE_VOLT, (unsigned char*)(&tempshort), 2);

		Equipment_Calibration = CALIBRATION_NO;           
	}
	// 부하 무게 조정 - 유 부하 조정 
	else if ( Equipment_Calibration == CALIBRATION_WEIGHT_LOAD ) // Input Max_Volt(2)
	{
		Save_Load_Sensor = Check_load_Sensor(AD_DATA.C_LOAD_SENSOR);

		if(Save_Load_Sensor==1)
		{
			//AD_DATA.C_Max_Ton = RX_MCU_DATA_65451.WeightMaxTon;
			AD_DATA.C_B = (2 * (( AD_DATA.C_Max_Ton * 100 ) + AD_DATA.C_D ))/((( 350./4. ) * (((float)(AD_DATA.C_Max_Volt) /10000) - 0.5 )));	//++,--, 210812 ysm, 1.019716 -> 1
			
			tempshort =  AD_DATA.C_Max_Volt;
			EepromWrite(ADDRESS_MAX_VOLT, (unsigned char*)(&tempshort), 2);

			tempchar =  AD_DATA.C_Max_Ton;
			EepromWrite(ADDRESS_MAX_TON, (unsigned char*)(&tempchar), 1);           

			Equipment_Calibration = CALIBRATION_NO; 
			Save_Load_Sensor=0;
			COUNT_FLAG.Check_load_sensor=0;
		}
		else if(Save_Load_Sensor==2)
		{
			Equipment_Calibration = CALIBRATION_NO; 
			Save_Load_Sensor=0;
		}
	}    	
	// 부하무게 조정 - 초기화 
	else if ( Equipment_Calibration == CALIBRATION_WEIGHT_INITIAL ) // Init Load sensor value
	{
		AD_DATA.C_Bar_Tare=0;
		AD_DATA.C_Max_Ton = 0;
		AD_DATA.C_Max_Volt = 0;
		AD_DATA.C_B = 0;
		tempshort=0;
		EepromWrite(ADDRESS_TARE_VOLT, (unsigned char*)(&tempshort), 2);
		EepromWrite(ADDRESS_MAX_VOLT, (unsigned char*)(&tempshort), 2);
		
		tempchar =  0x00;
		EepromWrite(ADDRESS_MAX_TON, (unsigned char*)(&tempchar), 1);           
		
		Equipment_Calibration = CALIBRATION_NO;  
	} 

	if ( AD_DATA.C_LOAD_SENSOR >= 5000 && AD_DATA.C_LOAD_SENSOR <= 45000 && AD_DATA.C_Bar_Tare != 0 ) //0.5 ~ 4.5 Volt
	{
		// 1bar = ap.  11.3 mVolt                
		AD_DATA.C_Re_Function_Bar = (( 350./4. ) * (((float)(AD_DATA.C_LOAD_SENSOR) /10000) - 0.5 )); //++,--, 210812 ysm, 1.019716 -> 1
		// ++, 200407 bwk
#if 1	//++,--, 210812 ysm, 1.019716 -> 1
		if ( AD_DATA.C_B == 0 )   AD_DATA.C_Compensation_Function_A = ( AD_DATA.C_A / 2) *  AD_DATA.C_Re_Function_Bar ;
#else
		if ( AD_DATA.C_B == 0 )   AD_DATA.C_Compensation_Function_A = ( AD_DATA.C_A * 2) *  AD_DATA.C_Re_Function_Bar /9.8;
#endif
		// --, 200407 bwk
		else                      AD_DATA.C_Compensation_Function_A = ( AD_DATA.C_B / 2) *  AD_DATA.C_Re_Function_Bar ;
		if ( AD_DATA.C_Compensation_Function_A >= AD_DATA.C_D )      AD_DATA.C_Load_Indicator = (unsigned long)(AD_DATA.C_Compensation_Function_A - AD_DATA.C_D);
		else AD_DATA.C_Load_Indicator = 0;

		if ( AD_DATA.C_Load_Indicator >= 65530)   AD_DATA.C_Load_Indicator = 65520;

		COUNT_FLAG.C_10ms=0;

	}
	else if(AD_DATA.C_LOAD_SENSOR >= 5000 && AD_DATA.C_LOAD_SENSOR <= 45000 && AD_DATA.C_Bar_Tare == 0)
	{
		AD_DATA.C_Load_Indicator = 0xffff ;
		AD_DATA.C_Re_Function_Bar = (( 350./4. ) * (((float)(AD_DATA.C_LOAD_SENSOR) /10000) - 0.5 )) ;                // ++, --, 200407 bwk 무게 보정 안되도 표시되게 ,//++,--, 210812 ysm, 1.019716 -> 1
	}
	else
	{
		if(AD_DATA.C_LOAD_SENSOR <= 3000 || AD_DATA.C_LOAD_SENSOR >= 47000 )
		{
			if(COUNT_FLAG.C_10ms++<500)
			{	
				AD_DATA.C_Load_Indicator = 0;
			}
			else
			{
				COUNT_FLAG.C_10ms=600;
				
				if(AD_DATA.C_LOAD_SENSOR <= 3000 || AD_DATA.C_LOAD_SENSOR >= 47000 )
					AD_DATA.C_Load_Indicator = 0xfbff;  //sensor error 
					
				if(AD_DATA.C_Load_Indicator != 0xfbff)		AD_DATA.C_Load_Indicator = 0;
			}
		}
		else
			AD_DATA.C_Load_Indicator=0;
	}

	if(AD_DATA.C_Load_Indicator < 0xfaff)
	{
		AD_DATA.Weight_Current = AD_DATA.C_Load_Indicator/10;
		Check_Load_buzzer();
	}
	else if(AD_DATA.C_Load_Indicator == 0xfbff)
	{
		AD_DATA.Weight_Current = 0xfbff; // load sensor NA
		WeightBuzzerOn = 0;
	}
	else
	{
		AD_DATA.Weight_Current = 0xffff; //설정 요망 
		WeightBuzzerOn = 0;
	}
	
	HCESPN.Weight_Current = AD_DATA.Weight_Current;


} 
//-----------------------------------------------------------------------------------------------------------

void Check_Engine_Start(void)		// 100ms
{
	// 1초 이상 유지 시 시동 조건 
	if(((HCESPN.rpm_310 >= 500) && (HCESPN.rpm_310 < 8192)) || (ALT_Level_Status == 1))
	{
		if(EngineStartCount++ > 10)
		{
			//++,--, 230209 ysm, FSCU
			#if 0	
			EngineStartCount = 1000;
			#else
			if(EngineStartCount > 100)	
			{
				EngineStartCount = 200;
				FSCU_EngineStartFlag = 1;
			}
			#endif
			EngineStartFlag = 1;
			Flag_Engine_Start=1;

		}       
		else
			COUNT_FLAG.EngineStart_cnt = 0;
	}
	else
	{	
		if(Flag_Engine_Start && (++COUNT_FLAG.EngineStart_cnt > (10*60*10)))
		{
			COUNT_FLAG.EngineStart_cnt = 0xf000;
		}
		EngineStartCount = 0;
		EngineStartFlag = 0;
		FSCU_EngineStartFlag = 0; //++,--, 230209 ysm, FSCU

		//++, 220627 ysm, DPF_ALARM
		if(COUNT_FLAG.Flag_DPF_ESC_Buzzer_Alarm == 1)
			COUNT_FLAG.Flag_DPF_ESC_Buzzer_Alarm = 0;
		//--, 220627 ysm, DPF_ALARM
	}	
}



void Anti_Restart_Control(void)
{
	Check_Engine_Start();

	if(EngineStartFlag)
	{
		COUNT_FLAG.Antirestart_cnt2=0;

		if ( ++COUNT_FLAG.Antirestart_cnt > 5 )
		{
			COUNT_FLAG.Antirestart_cnt = 0xf0 ;
			DOUT_DATA.ANTI_RESTART_OUTPUT=0;
		}
	}
	else
	{
		COUNT_FLAG.Antirestart_cnt = 0 ;
		if( ++COUNT_FLAG.Antirestart_cnt2 > 5)
		{
			COUNT_FLAG.Antirestart_cnt2 = 0xf0;

			//++, 211124 ysm
			#if 0
			if((HCESPN.Gear_542 == GEAR_NEUTRAL) && (Flag_ESL==0))		// 시동 걸림
				DOUT_DATA.ANTI_RESTART_OUTPUT=1;
			else
				DOUT_DATA.ANTI_RESTART_OUTPUT=0;
			#else
			if((HCESPN.Gear_542 == GEAR_NEUTRAL) && (Flag_ESL==0) && (MSS_ESL_Flag != 1))		// 시동 걸림
				DOUT_DATA.ANTI_RESTART_OUTPUT=1;
			else
				DOUT_DATA.ANTI_RESTART_OUTPUT=0;			
			#endif
			//--, 211124 ysm

		}
	}
}

void OPSS_Control(void)  // Travel_CUT , Attach-CUT Control 
{
	unsigned char ops_flag;
	
	HCESPN.Seat_SW_1504 = Flag_DIN[INDEX_SEAT_SW];	//seat on : 1  seat off : 0
	HCESPN.Seat_Belt  = Flag_DIN[INDEX_SW_BUCKLE];

	//++, 220929 ysm, FSCU	
	if(COUNT_FLAG.Flag_FSCU_Enable == 1)
	{
		Init_SeatBeltInterlock_Count = 0;
		COUNT_FLAG.SeatBeltInterlock_On = 0;
		#if 0
		BUZZER_FLAG.OPSS = RX_FSCU_DATA_65525.Warning_Buzzer_Demand;
		#else		
		BUZZER_FLAG.OPSS = 0;
		#endif
		OPSS_Status = RX_FSCU_DATA_65525.OPSS_Symbol_Demand;

		if(InfoModel2.SeatBeltInterlock == CURSER_ON) //SEAT BELT INTERLOCK FUNCTION ON
		{
			if((HCESPN.Seat_SW_1504 == 1)&&(HCESPN.Seat_Belt == 1))
				ops_flag = 1;
			else
				ops_flag = 0;
		}
		else
			ops_flag = HCESPN.Seat_SW_1504;

		if(ops_flag==0)
		{
			if(++COUNT_FLAG.Seat_off>=20)
			{
				COUNT_FLAG.Seat_off=20;
			}
						

		}
		else
		{
			COUNT_FLAG.Seat_off = 0;

		}		

	}
	else
	{
		
		if(Init_SeatBeltInterlock_Count < 5) // KEY ON 후 초기 상태
		{
			Init_SeatBeltInterlock_Count++;
			
			if((HCESPN.Seat_SW_1504 == 1)&&(HCESPN.Seat_Belt == 1))
			{
				COUNT_FLAG.SeatBeltInterlock_On = SEAT_OFF_BELT_OFF; //++,--, 211125 ysm SEAT_ON_BELT_ON -> SEAT_ON_BELT_OFF
				Init_SeatBeltInterlock_Count = 10;
			}
		
		}
					
		if((HCESPN.Seat_SW_1504 == 1)&&(HCESPN.Seat_Belt == 0)&&(COUNT_FLAG.SeatBeltInterlock_On == SEAT_OFF_BELT_OFF))
		{
			COUNT_FLAG.SeatBeltInterlock_On = SEAT_ON_BELT_OFF;
		  
		}			
		else if((COUNT_FLAG.SeatBeltInterlock_On == SEAT_ON_BELT_OFF)&&(HCESPN.Seat_Belt == 1))
		{
			COUNT_FLAG.SeatBeltInterlock_On = SEAT_ON_BELT_ON; 
		  
		}
		else
		{
			if((COUNT_FLAG.SeatBeltInterlock_On == SEAT_ON_BELT_ON)&&(HCESPN.Seat_SW_1504 == 1)&&(HCESPN.Seat_Belt == 1))
			{	
				COUNT_FLAG.SeatBeltInterlock_On = SEAT_ON_BELT_ON; 
			  
			}
			else if((COUNT_FLAG.SeatBeltInterlock_On == SEAT_ON_BELT_ON)&&(HCESPN.Seat_SW_1504 == 1)&&(HCESPN.Seat_Belt == 0))
			{	
				COUNT_FLAG.SeatBeltInterlock_On = SEAT_ON_BELT_OFF; 
			  
			}
			else if((COUNT_FLAG.SeatBeltInterlock_On == SEAT_ON_BELT_OFF)&&(HCESPN.Seat_SW_1504 == 1)&&(HCESPN.Seat_Belt == 0))
			{	
				COUNT_FLAG.SeatBeltInterlock_On = SEAT_ON_BELT_OFF; 
			  
			}
			else
			{			   
			  COUNT_FLAG.SeatBeltInterlock_On = SEAT_OFF_BELT_OFF; 
			}
		}
		
		
		if(InfoModel2.SeatBeltInterlock == CURSER_OFF) //SEAT BELT INTERLOCK FUNCTION OFF
		{
			if(HCESPN.Seat_SW_1504==0)
			{
					if(++COUNT_FLAG.Seat_off>=20)
					{
							COUNT_FLAG.Seat_off=20;
							COUNT_FLAG.SeatBeltInterlock_Cnt = 20;
							OPSS_Status=1;
		
							//++, 211125 ysm
							//DOUT_DATA.ATTACH_CUT_OUTPUT=1;
							OPSS_Attach_Cut_Relay = 1;
							//--, 211125 ysm
					}
							
					if(OPSS_Status==1 )
					{
							if((HCESPN.Gear_542==GEAR_FORWARD) || (HCESPN.Gear_542==GEAR_REVERSE)) 
							{
									//++, 211125 ysm
								   // DOUT_DATA.TRAVEL_CUT_OUTPUT=1;
									OPSS_Travel_Cut_Relay = 1;
									//--, 211125 ys,
									BUZZER_FLAG.OPSS = 1;
							}
					}
			}
			else
			{
					COUNT_FLAG.Seat_off = 0;
					COUNT_FLAG.SeatBeltInterlock_Cnt = 0;
					BUZZER_FLAG.OPSS = 0;
			}
		
			if(OPSS_Attach_Cut_Relay==1)
			{
					if(HCESPN.Seat_SW_1504==1)
					{
							OPSS_Attach_Cut_Relay=0;
							if((OPSS_Status==1) && (OPSS_Travel_Cut_Relay==0))
									OPSS_Status=0;
					}
			}
		
			if(OPSS_Travel_Cut_Relay==1)
			{
					if((HCESPN.Gear_542==GEAR_NEUTRAL) && (HCESPN.Seat_SW_1504==1))
					{
						   OPSS_Travel_Cut_Relay=0;
							if(OPSS_Status==1)
									OPSS_Status=0;
					}
			}
		
		}
		else // SEAT BELT INTERLOCK FUNCTION ON
		{
			if(COUNT_FLAG.SeatBeltInterlock_On < SEAT_ON_BELT_ON)
			{
					if(++COUNT_FLAG.SeatBeltInterlock_Cnt>=20)
					{
							COUNT_FLAG.SeatBeltInterlock_Cnt=20;
							COUNT_FLAG.Seat_off=20;
							OPSS_Status=1;
		
							OPSS_Attach_Cut_Relay = 1;
		
					}
							
					if(OPSS_Status==1 )
					{
							if((HCESPN.Gear_542==GEAR_FORWARD) || (HCESPN.Gear_542==GEAR_REVERSE)) 
							{
								OPSS_Travel_Cut_Relay = 1;
								BUZZER_FLAG.OPSS = 1;
							}
					}
			}
			else
			{
					COUNT_FLAG.SeatBeltInterlock_Cnt = 0;
					COUNT_FLAG.Seat_off=0;
					BUZZER_FLAG.OPSS = 0;
			}
		
			if(OPSS_Attach_Cut_Relay==1)
			{
					if(COUNT_FLAG.SeatBeltInterlock_On == SEAT_ON_BELT_ON)
					{
							OPSS_Attach_Cut_Relay=0;
							if((OPSS_Status==1) && (OPSS_Travel_Cut_Relay==0))	
									OPSS_Status=0;
					}
			}
		
			if(OPSS_Travel_Cut_Relay==1)
			{
					if((HCESPN.Gear_542==GEAR_NEUTRAL) && (COUNT_FLAG.SeatBeltInterlock_On==SEAT_ON_BELT_ON))
					{
							OPSS_Travel_Cut_Relay=0;
							if(OPSS_Status==1)	
									OPSS_Status=0;
					}
			}
		
		}

	}
}


void Auto_Parking_Control(void)
{
	unsigned char model_flag;
	unsigned char ops_flag; //++,--, 221019 ysm, FSCU_3

	if((InfoModel1.ModelInfo >= MODEL_35D_9S)&&(InfoModel1.ModelInfo <= MODEL_50D_9S))
	{
		model_flag = 0;
		
	}
	else
		model_flag = 1;

	if(model_flag == 1)
	{
		if(InfoModel2.SeatBeltInterlock == CURSER_ON) //SEAT BELT INTERLOCK FUNCTION ON
		{
			if((HCESPN.Seat_SW_1504 == 1)&&(HCESPN.Seat_Belt == 1))
				ops_flag = 1;
			else
				ops_flag = 0;
		}
		else
			ops_flag = HCESPN.Seat_SW_1504;

	
		if(ops_flag==0)
		{
			if(COUNT_FLAG.Seat_off >=20)
			{
				if((HCESPN.Gear_542==GEAR_NEUTRAL) && (Calculate_Speed_Value <= 30))
				{
					//++, 211125 ysm
					//DOUT_DATA.AUTO_PARKING_OUTPUT=1;
					APC_Parking_Relay = 1;
					//--, 211125 ysm
					COUNT_FLAG.Flag_AutoParking = 1;
					// ++, 200323 bwk
					// Parking 이 잡혀 있을 경우에는 보지 않음. 
					#if 0
					if((BUZZER_FLAG.AutoParking == 0)
					#else
					if((Flag_DIN[INDEX_PARKING_PRESSURE_SW] == 0) && (BUZZER_FLAG.AutoParking == 0))
					#endif
					// --, 200320 bwk
						BUZZER_FLAG.AutoParking = 1;
				}
			}
			else
			{
				COUNT_FLAG.Auto_parking = 0;
				BUZZER_FLAG.AutoParking = 0;
			}
		}
		else
		{
			COUNT_FLAG.Auto_parking = 0;
			BUZZER_FLAG.AutoParking = 0;
		}

		//++, 211125 ysm
		#if 0
		if((DOUT_DATA.AUTO_PARKING_OUTPUT==1) && (COUNT_FLAG.Flag_AutoParking == 1))
		{
			if(++COUNT_FLAG.Auto_parking>=10)
			{
				COUNT_FLAG.Auto_parking = 10;
				BUZZER_FLAG.AutoParking = 0;
			}
			
			if(HCESPN.Seat_SW_1504==1)	
			{
				COUNT_FLAG.Flag_AutoParking = 0;
				DOUT_DATA.AUTO_PARKING_OUTPUT=0;
			}
		}
		#else
		if((APC_Parking_Relay==1) && (COUNT_FLAG.Flag_AutoParking == 1))
		{
			if(++COUNT_FLAG.Auto_parking>=10)
			{
				COUNT_FLAG.Auto_parking = 10;
				BUZZER_FLAG.AutoParking = 0;
			}
			
			if(ops_flag==1)	
			{
				COUNT_FLAG.Flag_AutoParking = 0;
				APC_Parking_Relay=0;
			}
		}
		#endif
		//--, 211125 ysm
	}
}

float Calculate_Speed_Value_temp;
unsigned char Current_Gear;
unsigned char Current_Gear_Old;
unsigned short Gear_Count;

#if 1
void AutoShiftMode_Control()
{
	unsigned short erpm;
	unsigned char model_flag;

	unsigned char out_on,out_off;

	float sr_temp_up, sr_temp_down;

	out_on = 1;
	out_off = 0;

	//Current_Gear = Flag_DIN[INDEX_GEAR_SIGNAL_F];
	
	if((InfoModel1.ModelInfo >= MODEL_35L_9)&&(InfoModel1.ModelInfo <= MODEL_50L_9))
	{
		model_flag = 1; // 사용 

	}
	else if((InfoModel1.ModelInfo >= MODEL_35D_9HDI)&&(InfoModel1.ModelInfo <= MODEL_50DN_9HDI))
	{
		model_flag = 1; // 사용 

	}
	else if((InfoModel1.ModelInfo >= MODEL_35D_9S)&&(InfoModel1.ModelInfo <= MODEL_50D_9S))
	{
		model_flag = 1; // 사용
	}
	else
	{
		model_flag = 0; // 미사용 
	}	

	erpm= HCESPN.rpm_310;

	if(erpm == 0)
	{
		erpm = 1;
	}

	if(model_flag == 1)
	{
	
		if(AutoShiftMode_Gear == 2)
			Trpm_Temp = 60*(TACHO.Value/39)*1.024;
		else if(AutoShiftMode_Gear == 1)
			Trpm_Temp = 60*(TACHO.Value/39)*2.667;
		
		SR_Temp = Trpm_Temp/erpm;

		//++, 210814 ysm
		#if 0
		sr_temp_up = (float)(InfoModel3.AutoShift_Status_SR_Value_UP)/10;
		sr_temp_down = (float)(InfoModel3.AutoShift_Status_SR_Value_DOWN)/10;
		#else
		sr_temp_up = 0.7;
		sr_temp_down = 0.4;

		#endif
		//--, 210814 ysm

		if((InfoModel1.ModelInfo >= MODEL_35D_9S)&&(InfoModel1.ModelInfo <= MODEL_50D_9S))
		{
			SR_Temp = sr_temp_up = sr_temp_down = 0; // RPM 받을 수 없음 

		}

		if(InfoModel3.AutoShift_Status == 1) // AUTO
		{

			if((InfoModel1.ModelInfo >= MODEL_35D_9S)&&(InfoModel1.ModelInfo <= MODEL_50D_9S))
			{
				COUNT_FLAG.AutoShift_Cnt = 0;

				if((AutoShiftMode_Gear == 1)  && (Calculate_Speed_Value >= (10*InfoModel3.AutoShift_Status_Speed_UP)))
				{
				
					AutoShiftMode_Gear = 2;
					AutoShiftMode_Gear_flag = 0;
						
				}
				else if((AutoShiftMode_Gear == 2) && (Calculate_Speed_Value <= (10*InfoModel3.AutoShift_Status_Speed_DOWN)))
				{			

					
					AutoShiftMode_Gear = 1;
					AutoShiftMode_Gear_flag = 0;
				
				}
				
				if(AutoShiftMode_Gear == 1)
				{
					DOUT_DATA.RELAY_SHIFT_DOUT7 = out_on;
					DOUT_DATA.RELAY_SHIFT2_DOUT5 = out_off;


				}
				else
				{
					DOUT_DATA.RELAY_SHIFT_DOUT7 = out_off;
					DOUT_DATA.RELAY_SHIFT2_DOUT5 = out_off;

				}


			}
			else
			{
				//(InfoModel3.AutoShift_Status_Speed_UP)
				if(Flag_DIN[INDEX_GEAR_SIGNAL_F] == 1)
				{
					if(Current_Gear_Old == 0)
					{		
						DOUT_DATA.RELAY_SHIFT_DOUT7 = out_on;				
						DOUT_DATA.RELAY_SHIFT2_DOUT5 = out_off;  //2단 GND, 1단 OPEN
						COUNT_FLAG.AutoShift_Cnt = 0;
						AutoShiftMode_Gear = 1;	
						AutoShiftMode_Gear_flag = 0;
					
						if(Calculate_Speed_Value >= (10*(3)))
						{
							if(++Gear_Count >= 300)
							{
								Gear_Count = 0;
								Current_Gear_Old = 1;
							}
						}
						else
						{
							Gear_Count = 0;
							Current_Gear_Old = 1;
						}

					}
					else
					{
						if((AutoShiftMode_Gear == 1) && (SR_Temp >= sr_temp_up) && (Calculate_Speed_Value >= (10*InfoModel3.AutoShift_Status_Speed_UP)))
						{
														
							DOUT_DATA.RELAY_SHIFT_DOUT7 = out_on;
							DOUT_DATA.RELAY_SHIFT2_DOUT5 = out_on;
						
							AutoShiftMode_Gear_flag = 1; // 오버랩 구간
							
						}
						else if((AutoShiftMode_Gear == 2) && (SR_Temp <= sr_temp_down) &&  (Calculate_Speed_Value <= (10*InfoModel3.AutoShift_Status_Speed_DOWN)))
						{			
							DOUT_DATA.RELAY_SHIFT_DOUT7 = out_on;
							//DOUT_DATA.RELAY_SHIFT2_DOUT5 = out_off;
							DOUT_DATA.RELAY_SHIFT2_DOUT5 = out_on;
							
							AutoShiftMode_Gear_flag = 2; // 오버랩 구간 
						
						}
						else
						{
						
							if((AutoShiftMode_Gear_flag == 1)||(AutoShiftMode_Gear_flag == 2)) // 오버랩 구간 
							{
								DOUT_DATA.RELAY_SHIFT_DOUT7 = out_on;
								DOUT_DATA.RELAY_SHIFT2_DOUT5 = out_on;
							}
							else
							{
								COUNT_FLAG.AutoShift_Cnt = 0;
								
								if(AutoShiftMode_Gear == 2) // 2단 상태
								{
									DOUT_DATA.RELAY_SHIFT_DOUT7 = out_off;
									DOUT_DATA.RELAY_SHIFT2_DOUT5 = out_on;
								}
								else if(AutoShiftMode_Gear == 1) // 1단 상태
								{
									DOUT_DATA.RELAY_SHIFT_DOUT7 = out_on;
									DOUT_DATA.RELAY_SHIFT2_DOUT5 = out_off;
								}
						
							}			
						
						
						}			
					
						if(AutoShiftMode_Gear_flag == 1) // 오버랩 구간
						{
							COUNT_FLAG.AutoShift_Cnt++;
							//if(COUNT_FLAG.AutoShift_Cnt > InfoModel3.AutoShift_Status_Overlap_Time) // 1->2
							if(COUNT_FLAG.AutoShift_Cnt > 45) // 1->2
							{
								COUNT_FLAG.AutoShift_Cnt = 0;
								DOUT_DATA.RELAY_SHIFT_DOUT7 = out_off;
						
								// 2단
								AutoShiftMode_Gear = 2;
								AutoShiftMode_Gear_flag = 0;
							}
								
						}
						else if(AutoShiftMode_Gear_flag == 2) //오버랩 구간
						{
							COUNT_FLAG.AutoShift_Cnt++;
							//if(COUNT_FLAG.AutoShift_Cnt > InfoModel3.AutoShift_Status_Overlap_DownTime) // 2->1
							if(COUNT_FLAG.AutoShift_Cnt > 0) // 2->1
							{
								COUNT_FLAG.AutoShift_Cnt = 0;
								DOUT_DATA.RELAY_SHIFT2_DOUT5 = out_off;
							
								AutoShiftMode_Gear = 1;
								AutoShiftMode_Gear_flag = 0;
							}
								
						}
						else
						{
							COUNT_FLAG.AutoShift_Cnt = 0;
						
						}
					}
				}
				else
				{
					DOUT_DATA.RELAY_SHIFT_DOUT7 = out_off;
					DOUT_DATA.RELAY_SHIFT2_DOUT5 = out_off;
					AutoShiftMode_Gear = 1;
					AutoShiftMode_Gear_flag = 0;
					COUNT_FLAG.AutoShift_Cnt = 0;
					Gear_Count = 0;

					Current_Gear_Old = 0;

				}

				
				
			}
		
				
		}
		else // MANUAL
		{
			COUNT_FLAG.AutoShift_Cnt = 0;
			AutoShiftMode_Gear_flag = 0;
			Current_Gear_Old = Current_Gear;
			Gear_Count = 0;
			
			if(Flag_DIN[INDEX_GEAR_SELECT_SIG] == 1)  //1단 GND, 2단 OPEN
			{
				DOUT_DATA.RELAY_SHIFT_DOUT7 = out_on;				
				DOUT_DATA.RELAY_SHIFT2_DOUT5 = out_off;  //2단 GND, 1단 OPEN
				AutoShiftMode_Gear = 1;			
			}
			else
			{
				DOUT_DATA.RELAY_SHIFT_DOUT7 = out_off;

				if((InfoModel1.ModelInfo >= MODEL_35D_9S)&&(InfoModel1.ModelInfo <= MODEL_50D_9S))
					DOUT_DATA.RELAY_SHIFT2_DOUT5 = out_off;
				else
					DOUT_DATA.RELAY_SHIFT2_DOUT5 = out_on;
				AutoShiftMode_Gear = 2;
			}			

		}


	}
	else
	{
		COUNT_FLAG.AutoShift_Cnt = 0;
	}


	//++, 210119 ysm, LPG_LC
	if(model_flag == 0)
	{
		AutoShiftMode_Gear = 1;
		AutoShiftMode_Gear_flag = 0;
		COUNT_FLAG.AutoShift_Cnt = 0;

	}
	
	if(model_flag == 2)
	{
		DOUT_DATA.RELAY_SHIFT_DOUT7 = out_off;
		DOUT_DATA.RELAY_SHIFT2_DOUT5 = out_off;
		AutoShiftMode_Gear = 1;
		AutoShiftMode_Gear_flag = 0;
		COUNT_FLAG.AutoShift_Cnt = 0;
	}
	//--, 210119 ysm, LPG_LC

	if(((InfoModel1.ModelInfo >= MODEL_35L_9)&&(InfoModel1.ModelInfo <= MODEL_50L_9)||(InfoModel1.ModelInfo >= MODEL_35D_9HDI)&&(InfoModel1.ModelInfo <= MODEL_50DN_9HDI))
		&&(InfoModel2.SpeedLimitStatus == 1)&&(InfoModel2.SpeedLimitValue <= 9))
	{
		AutoShiftMode_Gear = 1;
		AutoShiftMode_Gear_flag = 0;
		COUNT_FLAG.AutoShift_Cnt = 0;
		DOUT_DATA.RELAY_SHIFT_DOUT7 = out_on;				
		DOUT_DATA.RELAY_SHIFT2_DOUT5 = out_off;  //2단 GND, 1단 OPEN

	}

	if(HCESPN.Gear_542 != 0xff)
	{
		if(HCESPN.Gear_542 == GEAR_FORWARD)
		{	
			ActualGearStatus = AutoShiftMode_Gear;																			
											
		}
		else if(HCESPN.Gear_542 == GEAR_REVERSE)
		{
			if((InfoModel1.ModelInfo >= MODEL_35D_9S)&&(InfoModel1.ModelInfo <= MODEL_50D_9S))
			{
				if( (AutoShiftMode_Gear >= 1) && (AutoShiftMode_Gear <= 2) ) 
					ActualGearStatus = AutoShiftMode_Gear + 6;
				else
					ActualGearStatus = 0;
			}
			else
			  ActualGearStatus = 7;
		}
		else if(HCESPN.Gear_542 == GEAR_NEUTRAL)
		{
			ActualGearStatus = 0;
		}
	}
}
#endif

void DCSR_Control()
{
	unsigned char dcsr_block_speed, dcsr_restore_speed;

	//++, 220208 ysm, 30LC-9
	if((InfoModel1.DCSR_Speed_Block>=MIN_BLOCK_SPEED_LIMIT)&&(InfoModel1.DCSR_Speed_Block<=MAX_BLOCK_SPEED_LIMIT))
		dcsr_block_speed = 10*(InfoModel1.DCSR_Speed_Block);
	else
		dcsr_block_speed = 10*MIN_BLOCK_SPEED_LIMIT;


	if((InfoModel1.DCSR_Speed_Restore>=MIN_RESTORE_SPEED_LIMIT)&&(InfoModel1.DCSR_Speed_Restore<=MAX_RESTORE_SPEED_LIMIT))
		dcsr_restore_speed = 10*(InfoModel1.DCSR_Speed_Restore);
	else
		dcsr_restore_speed = 10*MIN_RESTORE_SPEED_LIMIT;
	
	
	if(((InfoModel1.ModelInfo >= MODEL_25LC_9)&&(InfoModel1.ModelInfo <= MODEL_33LC_9))||

		((InfoModel1.ModelInfo >= MODEL_25L_9A)&&(InfoModel1.ModelInfo <= MODEL_35LN_9A)))
	{
		dcsr_block_speed += 4;	
		dcsr_restore_speed += 4;
	}
	//--, 220208 ysm, 30LC-9
	
	if(InfoModel1.DCSR_Status == CURSER_ON)
	{
		if(OldGear != HCESPN.Gear_542)
		{
			// 200626 전진만 dcsr 되는 버그 수정 (후진도 되게 적용 함)
			if(((OldGear == GEAR_FORWARD) || (OldGear == GEAR_REVERSE)) && (dcsr_block_speed < Calculate_Speed_Value))
			{
				//++, 211125 ysm
				//DOUT_DATA.TRAVEL_CUT_OUTPUT = 1;
				DCSR_Travel_Cut_Relay = 1;
				//--, 211125 ysm
				COUNT_FLAG.DCSR_On = 1;
			}
			else
				OldGear = HCESPN.Gear_542;
		}

		//++, 211125 ysm
		#if 0
		if((COUNT_FLAG.DCSR_On ==  1) && (DOUT_DATA.TRAVEL_CUT_OUTPUT == 1))
		{
			if((InfoModel1.DCSR_Speed_Restore*10 > Calculate_Speed_Value))
			{
				COUNT_FLAG.DCSR_On = 0;
				DOUT_DATA.TRAVEL_CUT_OUTPUT = 0;
			}
		}
		#else
		if((COUNT_FLAG.DCSR_On ==  1) && (DCSR_Travel_Cut_Relay == 1))
		{
			if((dcsr_restore_speed > Calculate_Speed_Value))
			{
				COUNT_FLAG.DCSR_On = 0;
				DCSR_Travel_Cut_Relay = 0;
			}
		}
		#endif
		//--, 211125 ysm
	}
	else
	{
		COUNT_FLAG.DCSR_On = 0;
		DCSR_Travel_Cut_Relay = 0;
		OldGear = 0xff;
	}
}

//LPG
//Speed Limit Value(km/h)      	 8    9    10   11   12   13   14   15
unsigned char Speed_P1[] =	 	{110, 110, 110, 110, 30,  30,  30,  30};
unsigned char Speed_I1[] =	 	{10,  10,  10,  10,  20,  20,  20,  20};
unsigned short Speed_D1[] =	 	{1100,1400,1700,1700,800, 800, 800, 800};
unsigned char Speed_Low_Tq1[]= 	{16,  16,  18,  22,  13,  13,  13,  13};

#if 1
unsigned char Speed_P2[] =	 	{40,  15,  10,  5, 	 20,  20,  30,  40};
unsigned char Speed_I2[] =	 	{10,  10,  10,  10,  10,  10,  10,  10};
unsigned short Speed_D2[] =	 	{2000,2000,2000,2000,3500,3500,3500,3500};
unsigned char Speed_Low_Tq2[]=	{18,  15,  13,  20,  18,  18,  18,  18};
#else
unsigned char Speed_P2[] =		{80,  15,  10,	5,	 20,  20,  30,	40};
unsigned char Speed_I2[] =		{10,  10,  10,	10,  10,  10,  10,	10};
unsigned short Speed_D2[] = 	{2000,2000,2000,2000,3500,3500,3500,3500};
unsigned char Speed_Low_Tq2[]=	{15,  15,  13,	20,  18,  18,  18,	18};
#endif
//DIESEL
//Speed Limit Value(km/h)         8     9    10    11    12    13    14    15
unsigned char Speed_P0[] =	 	{80,   80,   80,   80,   80,   80,   80,   80};
unsigned char Speed_I0[] =	 	{20,   20,   20,   20,   20,   20,   20,   20};
unsigned short Speed_D0[] =	 	{800, 800,  800,  800,  800,  800,  800,  800};
//unsigned char Speed_Low_Tq0[]=	{90,   95,  120,  140,  160,  180,  200,  220};
//unsigned char Speed_Low_Tq0[]=	{30,   30,  30,  30,  30,  30,  30,  30};
//unsigned char Speed_Low_Tq0[]=	{50,   50,  50,  50,  50,  50,  50,  50};
unsigned char Speed_Low_Tq0[]=	{80,   90,  90,  90,  90,  90,  90,  90};

//9HDI
unsigned char Speed_P3[] =	 	{100, 100, 100, 100, 100,  100,  100,  100};
unsigned char Speed_I3[] =	 	{10,  10,  10,  10,  10,  10,  10,  10};
unsigned short Speed_D3[] =	 	{900,900,900,900,900, 900, 900, 900};
unsigned char Speed_Low_Tq3[]=	{4,   4,  5,  5,  5,  5,  5,  5};



unsigned short Low_tq_count = 0; //++,--, 221005 ysm, Speed_limit_hdi
unsigned char Low_tq = 5;
float Speed_value_temp_Old;
float result_SPN518_torq_limit_temp;


float Engine_Speed_Old;
float result_engine_Speed;
unsigned short SpeedLimitSpeed;
unsigned char SpeedLimitStep = 0;
void Speed_limit_process(void)
{
	unsigned char limit_speed_value;
	float upper_limit_torque;
	unsigned char lower_limit_torque;

	unsigned char model_flag;
	float speedlimit_value;
	float currnet_torque;
	
	speedlimit_value = (float)(InfoModel2.SpeedLimitValue*10);
	currnet_torque = (float)HCESPN.Actual_Engine_Percent_Torque_513;
	

	if(InfoModel2.SpeedLimitStatus == 1)
	{
		if(Speed_value_temp[1] >= speedlimit_value )
		{
			COUNT_FLAG.Flag_SpeedLimit = 1;
		}
		else
			COUNT_FLAG.Flag_SpeedLimit = 0;

	}
	else
	{
		COUNT_FLAG.Flag_SpeedLimit = 0;
	}

	if(((InfoModel1.ModelInfo >= MODEL_25LC_9)&&(InfoModel1.ModelInfo <= MODEL_33LC_9))||

		((InfoModel1.ModelInfo >= MODEL_25L_9A)&&(InfoModel1.ModelInfo <= MODEL_35LN_9A))||
		((InfoModel1.ModelInfo >= MODEL_35D_9S)&&(InfoModel1.ModelInfo <= MODEL_50D_9S)))
	{
		if(InfoModel2.SpeedLimitStatus == 1)
		{
			if(Speed_value_temp[1] >= speedlimit_value )
			{
				BUZZER_FLAG.SpeedLimit = 1;
				COUNT_FLAG.Count_SpeedLimit = 0;

			}
			else
			{
				BUZZER_FLAG.SpeedLimit = 0;
			}			
		}
		else
		{
			BUZZER_FLAG.SpeedLimit = 0;
			COUNT_FLAG.Count_SpeedLimit = 0;
		}

		SpeedLimitOverrideControlMode = 0x00 ;
		SpeedLimitTorque = 0xFF;
	}
	else
	{
		BUZZER_FLAG.SpeedLimit = 0;
		COUNT_FLAG.Count_SpeedLimit = 0;

		//if(Engine_Type == LPG_TYPE)
		if((Engine_Type == LPG_TYPE)||((InfoModel1.ModelInfo >= MODEL_25D_9HDI)&&(InfoModel1.ModelInfo <= MODEL_50DN_9HDI)))
		{
			model_flag = LPG_TYPE;
		}
		else
		{
			model_flag = DIESEL_TYPE;
		}
		
		if((InfoModel2.SpeedLimitValue >= 8)&&(InfoModel2.SpeedLimitValue <= 15))
		{
			limit_speed_value = InfoModel2.SpeedLimitValue - 8;
		}
		else
			limit_speed_value = 0;
		
		
		if(model_flag == LPG_TYPE)
		{

			upper_limit_torque = 125.0;
			if((InfoModel1.ModelInfo >= MODEL_35L_9)&&(InfoModel1.ModelInfo <= MODEL_50L_9))
			{
				
				if(AutoShiftMode_Gear == 1)
					lower_limit_torque = Speed_Low_Tq1[limit_speed_value];
				else
					lower_limit_torque = Speed_Low_Tq2[limit_speed_value];

			}
			else if((InfoModel1.ModelInfo >= MODEL_25D_9HDI)&&(InfoModel1.ModelInfo <= MODEL_50DN_9HDI))
			{
				
				if(Speed_Limit_Test_Flag == 1)
					lower_limit_torque = RX_SPEED_LIMIT_SET_STATUS_65452.SpeedLimit_LowTq;
				else
					lower_limit_torque = Speed_Low_Tq3[limit_speed_value];

			}
			else
			{	
				if(Speed_Limit_Test_Flag == 1)
					lower_limit_torque = RX_SPEED_LIMIT_SET_STATUS_65452.SpeedLimit_LowTq;
				else
					lower_limit_torque = Speed_Low_Tq3[limit_speed_value];
			}
		
		

			
		}
		else
		{
			upper_limit_torque = 500.0;
			lower_limit_torque = Speed_Low_Tq0[limit_speed_value];
		}
		
	
		if(InfoModel2.SpeedLimitStatus == 1)
		{
			if(PID_limit_ON == 0)
			{
				#if 1
				if(AutoShiftMode_Gear == 2)
					diffSpeed=(speedlimit_value - 15) - Speed_value_temp[1];
				else
				{
					if(model_flag  == LPG_TYPE)
					{
						if((InfoModel1.ModelInfo >= MODEL_25D_9HDI)&&(InfoModel1.ModelInfo <= MODEL_35DN_9HDI_S))
							diffSpeed= speedlimit_value - Speed_value_temp[1] - 15;
						else
							diffSpeed= speedlimit_value- Speed_value_temp[1] - 5;
					}
					else
						diffSpeed= speedlimit_value - Speed_value_temp[1] - 15; // 10->15
				}
				#else
				diffSpeed=(InfoModel2.SpeedLimitValue*10) - Speed_value_temp[1];
				#endif
			}
			else
			{

				diffSpeed= speedlimit_value - Speed_value_temp[1] - 5;

			}
			//무빙에버러지를 통한 PID계산시 error계산
			TotaldiffSpeed -= MovingAverage[speed_limit_index];
			MovingAverage[speed_limit_index] = diffSpeed;
			TotaldiffSpeed += MovingAverage[speed_limit_index];
		
			if(++speed_limit_index>= 10)	//무빙에버러지 배열이 다차면 다시 처음부터 하기 위해 필터계수는 10개정도
			{
				speed_limit_index = 0;
			}
			
			if(PID_limit_ON == 0)
			{
				// 에러가 1키로 이내일 때 그리고 악셀페달값이 100%이내일 때 면서 0이아닐 때 PID 제어
				//if(  (diffSpeed <= (InfoModel2.SpeedLimitValue*10 - 10) ) && (HCESPN.Accel_pedal_position_91 <= 250) && (HCESPN.Accel_pedal_position_91 != 0) )
				if( (diffSpeed <= 0)&&(HCESPN.Accel_pedal_position_91 <= 250) && (HCESPN.Accel_pedal_position_91 != 0) )
				{
					PID_limit_ON = 1;

		
					if(model_flag  == LPG_TYPE)
					{
						if(currnet_torque <= 50) 
							result_SPN518_torq_limit_old = currnet_torque ;
						else						
							result_SPN518_torq_limit_old = 25;
					}
					else
					{
						if(currnet_torque <= 50)
							result_SPN518_torq_limit_old = currnet_torque;
						else					
							result_SPN518_torq_limit_old = 50;
		
					}
		
					result_SPN518_torq_limit = result_SPN518_torq_limit_old;
				}
				PID_exit_cnt = 0;
			}
			else
			{	
				
				//if((HCESPN.Accel_pedal_position_91==0)||(HCESPN.Accel_pedal_position_91==0xFF) )
				if( (diffSpeed >= 0) || (HCESPN.Accel_pedal_position_91==0)||(HCESPN.Accel_pedal_position_91==0xFF) )
					PID_exit_cnt++;
				else
					PID_exit_cnt = 0;
		
				if(PID_exit_cnt >= 10)		// 20 -> 10
				{
					PID_limit_ON = 0;
					PID_exit_cnt = 0;

				}
			}
		
			if(PID_limit_ON == 1)
			{
			//PID제어 시작, 518값에 PID 컨트롤값을 대응하여 제어할수 있도록 함 P=1.6, I=0, D=25.0 으로 세팅 ->실차테스트후 변경예정
		
		
				if(model_flag  == LPG_TYPE)
				{

					if((InfoModel1.ModelInfo >= MODEL_35L_9)&&(InfoModel1.ModelInfo <= MODEL_50L_9))
					{
						if(AutoShiftMode_Gear == 1)
						{			
							result_SPN518_torq_limit = result_SPN518_torq_limit_old + Speed_P1[limit_speed_value] * (diffSpeed)/((float)10)
												+ Speed_I1[limit_speed_value] * (TotaldiffSpeed/((float)10))
												+ (Speed_D1[limit_speed_value]/((float)10)) * (diffSpeed - OlddiffSpeed);					
						}
						else
						{
							result_SPN518_torq_limit = result_SPN518_torq_limit_old + Speed_P2[limit_speed_value] * (diffSpeed)/((float)10)
												+ Speed_I2[limit_speed_value] * (TotaldiffSpeed/((float)10))
												+ (Speed_D2[limit_speed_value]/((float)10)) * (diffSpeed - OlddiffSpeed);	
			
						}
					}
					else
					{

						if(Speed_Limit_Test_Flag == 1)
						{
							result_SPN518_torq_limit = result_SPN518_torq_limit_old + RX_SPEED_LIMIT_SET_STATUS_65452.SpeedLimit_P * (diffSpeed)/((float)10)
												+ RX_SPEED_LIMIT_SET_STATUS_65452.SpeedLimit_I  * (TotaldiffSpeed/((float)10))
												+ ( RX_SPEED_LIMIT_SET_STATUS_65452.SpeedLimit_D/((float)10)) * (diffSpeed - OlddiffSpeed);

						}
						else
						{

							#if 1
							/*
							result_SPN518_torq_limit = result_SPN518_torq_limit_old + Speed_P3[limit_speed_value] * (diffSpeed)/((float)10)
													+ Speed_I3[limit_speed_value] * (TotaldiffSpeed/((float)10))
													+ (Speed_D3[limit_speed_value]/((float)10)) * (diffSpeed - OlddiffSpeed);
							*/
							if(AutoShiftMode_Gear == 1)
							{			
								result_SPN518_torq_limit = result_SPN518_torq_limit_old + Speed_P1[limit_speed_value] * (diffSpeed)/((float)10)
													+ Speed_I1[limit_speed_value] * (TotaldiffSpeed/((float)10))
													+ (Speed_D1[limit_speed_value]/((float)10)) * (diffSpeed - OlddiffSpeed);					
							}
							else
							{
								result_SPN518_torq_limit = result_SPN518_torq_limit_old + Speed_P2[limit_speed_value] * (diffSpeed)/((float)10)
													+ Speed_I2[limit_speed_value] * (TotaldiffSpeed/((float)10))
													+ (Speed_D2[limit_speed_value]/((float)10)) * (diffSpeed - OlddiffSpeed);	
							
							}
							
							#else


							#if 0
							if(diffSpeed <= 0)  // spd >= 7.5  // 0->5
							{
								result_SPN518_torq_limit = 5;
								SpeedLimitStep = 1;
								
								Speed_value_temp_Old = Speed_value_temp[1];
							}						
							else if(diffSpeed >= 5)
							{
								if(SpeedLimitStep == 1)
								{

									if(Speed_value_temp[1] <= ((InfoModel2.SpeedLimitValue*10)-5))									
									{
										result_SPN518_torq_limit_old = HCESPN.Actual_Engine_Percent_Torque_513;
										SpeedLimitStep = 2;
									}

								}									
						
							}

							if(SpeedLimitStep == 2)
							{
								if( Speed_value_temp[1] > (InfoModel2.SpeedLimitValue*10))
								{
									result_SPN518_torq_limit = result_SPN518_torq_limit_old - 1;
								}
								else if( Speed_value_temp[1] < ((InfoModel2.SpeedLimitValue*10)+5))
								{						
									result_SPN518_torq_limit = result_SPN518_torq_limit_old + 1;
								}

							}
							#else // TEST
							if(SpeedLimitStep == 1)
							{
								result_SPN518_torq_limit = 5;
								SpeedLimitStep = 2;
							}
							else if(SpeedLimitStep == 2)
							{

							}
								
							if(diffSpeed <= 0) 
							{
								result_SPN518_torq_limit = 5;

								if(HCESPN.Actual_Engine_Percent_Torque_513 >= 5)
									result_SPN518_torq_limit_temp = (float)HCESPN.Actual_Engine_Percent_Torque_513;

								SpeedLimitStep = 1;
							}						
							else if(diffSpeed >= 10)
							{
								result_SPN518_torq_limit = 100;
								SpeedLimitStep = 4;

							}
							else
							{
								result_SPN518_torq_limit = result_SPN518_torq_limit_temp;
								SpeedLimitStep = 2;
							}
	
							#endif

	

							#endif
						}


					}
				}
				else
				{
					result_SPN518_torq_limit = result_SPN518_torq_limit_old + Speed_P0[limit_speed_value] * (diffSpeed)/((float)10)
										+ Speed_I0[limit_speed_value] * (TotaldiffSpeed/((float)10))
										+ (Speed_D0[limit_speed_value]/((float)10)) * (diffSpeed - OlddiffSpeed);
		
				}
		
				if(model_flag  == LPG_TYPE)
				{
					if(result_SPN518_torq_limit >= (currnet_torque +5))	//실제토크보다 제어할 토크가 일정수준 이상 크면 일정수준으로 제어하게 //25
						result_SPN518_torq_limit = currnet_torque +5;

				}
				else
				{
					if(result_SPN518_torq_limit >=	(currnet_torque + 25))	//실제토크보다 제어할 토크가 일정수준 이상 크면 일정수준으로 제어하게 //25
						result_SPN518_torq_limit = currnet_torque + 25;
		
				}		
		
		
				if( result_SPN518_torq_limit > upper_limit_torque)
				{				
					result_SPN518_torq_limit = upper_limit_torque;
				}
				
		
				if(result_SPN518_torq_limit < lower_limit_torque)
				{
					result_SPN518_torq_limit = lower_limit_torque;
					
				}				
				result_SPN518_torq_limit_old = result_SPN518_torq_limit;

				SpeedLimitOverrideControlMode = 0x03;
		
				if(model_flag  == LPG_TYPE)
					SpeedLimitTorque = (unsigned char)(result_SPN518_torq_limit+125);
				else
					SpeedLimitTorque = (unsigned char)(result_SPN518_torq_limit/2);	
				
			}
			else
			{
				SpeedLimitOverrideControlMode = 0x00 ;
				SpeedLimitTorque = 0xFF;

				SpeedLimitStep = 0;	
		
			}
			
			OlddiffSpeed= diffSpeed;
		}	
		else
		{
			SpeedLimitOverrideControlMode = 0x00 ;
			SpeedLimitTorque = 0xFF;

		
		}
	}

	SpeedLimitStep = PID_limit_ON;
	
} 
// --, 200326 bwk

                                    
void DPF_Automatic_active_regeneration_Control()
{
	if((InfoModel1.ModelInfo <= MODEL_35DN_9VB) || ((InfoModel1.ModelInfo >= MODEL_25D_9VS)&&(InfoModel1.ModelInfo <= MODEL_35DN_9VS)))
	{
		if((RX_ECU_DATA_64890.DPF_Regen_Status_3801 == 1) && (Flag_DIN[INDEX_INHIBIT_REGENERATION_SIG] == 0)
			&& (Flag_DIN[INDEX_PARKED_REGENERATION_SIG] == 0) && (AD_DATA.C_HydTemp <= (60+40)))
			DOUT_DATA.HYD_AUTO_REGENERATION = 1;
		else
			DOUT_DATA.HYD_AUTO_REGENERATION = 0;
	}
	else
		DOUT_DATA.HYD_AUTO_REGENERATION = 0;
}

void ZeroStart()	// 200626 추가
{
	unsigned char accel_pos;	

	
	if((InfoModel1.ModelInfo >= MODEL_35D_9S) && (InfoModel1.ModelInfo <= MODEL_50D_9S)) //++,--, 230208 ysm, FSCU
	{
		COUNT_FLAG.ZeroStart_Cnt = 0;
		COUNT_FLAG.ZeroStart_On = 0;
		ZEROSTART_Travel_Cut_Relay = 0; //++,--, 220621 ysm, LCD_DELAY
	}
	else
	{

		if((InfoModel1.ModelInfo >= MODEL_25D_9HDI) && (InfoModel1.ModelInfo <= MODEL_50DN_9HDI))
		{
			if((HCESPN.Accel_pedal_position_91 == 0)||(HCESPN.Accel_Pedal1_Low_Idle_Sw == 1))
			{
				accel_pos = 0;
			}
			else
			{
				accel_pos = HCESPN.Accel_pedal_position_91;
			}
		}
		else
			accel_pos = HCESPN.Accel_pedal_position_91;	
	
		//++, 230125 ysm, FSCU
		if(COUNT_FLAG.Flag_FSCU_Enable == 1)
		{
			InfoModel2.ZeroStart = CURSER_ON;
		}
		//--, 230125 ysm, FSCU


		if(InfoModel2.ZeroStart == CURSER_ON)
		{
			if((HCESPN.Gear_542==GEAR_FORWARD) || (HCESPN.Gear_542==GEAR_REVERSE))
			{                  
				//if((Flag_DIN[INDEX_PARKING_PRESSURE_SW] == 0) && (accel_pos == 0)&&(COUNT_FLAG.Flag_HAC == 0)) //++,--, 230503 ysm, TEST_ZEROSTART
				if((Flag_DIN[INDEX_PARKING_PRESSURE_SW] == 0) && (accel_pos == 0)) //++,--, 230503 ysm, TEST_ZEROSTART
				{
					if((++COUNT_FLAG.ZeroStart_Cnt >= 5) || (Calculate_Speed_Value == 0))
					{
						COUNT_FLAG.ZeroStart_Cnt = 5;
						//++, 211125 ysm
						//DOUT_DATA.TRAVEL_CUT_OUTPUT=1;
						ZEROSTART_Travel_Cut_Relay = 1;
						//--, 211125 ysm
						COUNT_FLAG.ZeroStart_On = 1;
					}
				}
				else
				{
					COUNT_FLAG.ZeroStart_Cnt = 0;
				}
			}
			else
				COUNT_FLAG.ZeroStart_Cnt = 0;


			if((COUNT_FLAG.ZeroStart_On == 1) && (ZEROSTART_Travel_Cut_Relay== 1))
			{
				if((HCESPN.Accel_pedal_position_91 >= InfoModel2.ZeroStart_AccelValue)/* && (Calculate_Speed_Value == 0)*/)	// 5% 초과 
				//if((accel_pos != 0)&&(HCESPN.Accel_pedal_position_91 >= 13)) // 5.2% //++,--, 220621 ysm, LCD_DELAY
				{
					COUNT_FLAG.ZeroStart_On = 0;
					ZEROSTART_Travel_Cut_Relay = 0;
				}
			}

		}
		else
		{
			//++, 220621 ysm, LCD_DELAY
			COUNT_FLAG.ZeroStart_Cnt = 0;
			COUNT_FLAG.ZeroStart_On = 0;
			ZEROSTART_Travel_Cut_Relay = 0;
			//--, 220621 ysm, LCD_DELAY

		}
	}
}

unsigned char Rear_Count;
void HillStartAssistControl_Control(void)
{
	unsigned char model_flag;
	unsigned char accel_pos;
	unsigned char hac_count;

	if(ACCS.TUNE_Enable == 1)
		hac_count = ACCS.HAC_Count_Value;
	else
		hac_count = 5;
	

	if(((InfoModel1.ModelInfo >= MODEL_35D_9S)&&(InfoModel1.ModelInfo <= MODEL_50D_9S))||

		((InfoModel1.ModelInfo >= MODEL_25D_9VS)&&(InfoModel1.ModelInfo <= MODEL_35DN_9VS))||
		(InfoModel1.ModelInfo <= MODEL_35DN_9VB))
	{
		model_flag = 0;
	}
	else
		model_flag = 1;



	if((InfoModel1.ModelInfo >= MODEL_25D_9HDI) && (InfoModel1.ModelInfo <= MODEL_50DN_9HDI))
	{
		if((HCESPN.Accel_pedal_position_91 == 0)||(HCESPN.Accel_Pedal1_Low_Idle_Sw == 1))
		{
			accel_pos = 0;
		}
		else
		{
			accel_pos = HCESPN.Accel_pedal_position_91;
		}
	}
	else
		accel_pos = HCESPN.Accel_pedal_position_91; 

	

	//if((MAST_EQUIPMENT==1)&&(ANGLE_EQUIPMENT==1)&&(model_flag==1)) //210708 ysm, HAC
	if((ANGLE_EQUIPMENT==1)&&(model_flag==1)) 
	{

		if(InfoModel2.HAC_Status == 1)
		{
			#if 1
			if((HCESPN.Gear_542==GEAR_REVERSE)&&(Calculate_Speed_Value >= 30))
			{
				Rear_Count = 30;
			}
			else
			{	
				if(Rear_Count > 0)
					Rear_Count--;
			}
			#else
			Rear_Count = 0;
			#endif
			

			if(gHardware_Version == RevC_01)
			{
		
				if((COUNT_FLAG.Flag_HAC_Check == 1)&&(HCESPN.Vehicle_Angle_X >= 30)&&(Rear_Count==0))
				{				
					if(++COUNT_FLAG.HAC_On_Cnt >= hac_count)
					{
						COUNT_FLAG.HAC_On_Cnt = hac_count;
						COUNT_FLAG.Flag_HAC= 1;

						HAC_Parking_Relay = 1;
					}
				}
				else
				{
					COUNT_FLAG.HAC_On_Cnt = 0;
				}
			}
			else
			{
				if((HCESPN.Gear_542==GEAR_FORWARD) && (Flag_DIN[INDEX_BRAKE_SW_SIG] == 1) && (accel_pos == 0) && (HCESPN.Vehicle_Angle_X >= 30) && (Calculate_Speed_Value == 0))
				{				
					if(++COUNT_FLAG.HAC_On_Cnt >= 6)
					{
						COUNT_FLAG.HAC_On_Cnt = 6;
						COUNT_FLAG.Flag_HAC= 1;

						HAC_Parking_Relay = 1;
					}
				}
				else
				{
					COUNT_FLAG.HAC_On_Cnt = 0;
				}			

			
			}

			if((COUNT_FLAG.Flag_HAC == 1) && (HAC_Parking_Relay== 1))
			{
				if(((HCESPN.Accel_pedal_position_91 >= 38) && (HCESPN.Gear_542==GEAR_FORWARD))||(HCESPN.Gear_542==GEAR_REVERSE)||(HCESPN.Gear_542==GEAR_NEUTRAL))
				{
					if((++COUNT_FLAG.HAC_Off_Cnt >= 3))
					{
						COUNT_FLAG.HAC_Off_Cnt = 3;
						COUNT_FLAG.Flag_HAC= 0;
						HAC_Parking_Relay=0;
					}
				}
				else
				{
					COUNT_FLAG.HAC_Off_Cnt = 0;
				}
			}

			//++, 230509 ysm, FSCU_HAC
			if(HAC_Parking_Relay == 1)
			{
				FSCU_HAC_OFF_Flag = 1;
			}
			else
			{

				if(FSCU_HAC_OFF_Flag != 0)
				{

					if(FSCU_HAC_OFF_Flag == 1)
					{
						if(RX_FSCU_DATA_65525.Parking_Cut_SV == 0)
						{
							FSCU_HAC_OFF_Flag = 2; // 정상 해제
						}
						else
						{
							if(APC_Parking_Relay== 1) // AUTO PARKING 이 걸려서 PARKING 해제가 안되는 상태 
							{
								FSCU_HAC_OFF_Flag = 0; // 즉시 TRAVEL CUT
							}
							else  // PARKING 스위치 채결되어 PARKING이 해제가 안된 상태
							{
								if(RX_FSCU_DATA_65525.Parking_Switch == 1)
								{
									FSCU_HAC_OFF_Flag = 0; // 즉시 TRAVEL CUT
								}
							}

						}

					}
					else if(FSCU_HAC_OFF_Flag == 2)
					{
						if((RX_FSCU_DATA_65525.Warning_Buzzer_Demand == 1)&&(RX_FSCU_DATA_65525.Travel_Cut_Relay_Demand == 1))
						{
							FSCU_HAC_OFF_Flag = 0;
						}
						else if((RX_FSCU_DATA_65525.Warning_Buzzer_Demand == 0)&&(RX_FSCU_DATA_65525.Travel_Cut_Relay_Demand == 1))
						{
							
							if(RX_FSCU_DATA_65525.OPSS_Symbol_Demand == 1)
							{
								FSCU_HAC_OFF_Flag = 0;
							}
							else // IGNORE
							{
								FSCU_HAC_OFF_Flag = 2;
							}
						}
						else if((RX_FSCU_DATA_65525.Warning_Buzzer_Demand == 1)&&(RX_FSCU_DATA_65525.Travel_Cut_Relay_Demand == 0))
						{
							// IGNORE
							FSCU_HAC_OFF_Flag = 2;
						}
						else
						{
							FSCU_HAC_OFF_Flag = 0;
						}

					}
				}
				else
				{
					FSCU_HAC_OFF_Flag = 0;
				}	
			}
			//--, 230509 ysm, FSCU_HAC
		}
		else
		{
			HAC_Parking_Relay = 0;	

			COUNT_FLAG.HAC_On_Cnt = 0;
			COUNT_FLAG.HAC_Off_Cnt = 0;
			COUNT_FLAG.Flag_HAC= 0;

			//++, 230503 ysm, FSCU_HAC
			FSCU_HAC_OFF_Flag = 0;
			//--, 230503 ysm, FSCU_HAC
		}
	}
	else
	{
		HAC_Parking_Relay = 0;

		COUNT_FLAG.HAC_On_Cnt = 0;
		COUNT_FLAG.HAC_Off_Cnt = 0;
		COUNT_FLAG.Flag_HAC= 0;

		//++, 230503 ysm, FSCU_HAC
		FSCU_HAC_OFF_Flag = 0;
		//--, 230503 ysm, FSCU_HAC

	}
}

//++, 220317 ysm, AUTO_TILT
float upper_limit_angle,lower_limit_angle, temp_angle;

void Cal_AutoLevel_Angle()
{
	float upper_angle_max, upper_angle_min;
	float lower_angle_max, lower_angle_min;
	float upper_rpm, lower_rpm;
	float currnet_rpm;


	currnet_rpm = HCESPN.rpm_310;

	if(((InfoModel1.ModelInfo >= MODEL_25LC_9)&&(InfoModel1.ModelInfo <= MODEL_33LC_9))||
		((InfoModel1.ModelInfo >= MODEL_25L_9A)&&(InfoModel1.ModelInfo <= MODEL_35LN_9A)))
	{
		if(currnet_rpm < 800)
			currnet_rpm = 800;
		else if(currnet_rpm > 2600)
			currnet_rpm = 2600;
		
		if((currnet_rpm >= 800)&&(currnet_rpm < 1200))
		{
			lower_rpm = 800; upper_rpm = 1200; upper_angle_max = 32; upper_angle_min = 21; lower_angle_max = 29; lower_angle_min = 29;
		}
		else if((currnet_rpm >= 1200)&&(currnet_rpm < 1600))
		{
			lower_rpm = 1200; upper_rpm = 1600; upper_angle_max = 32; upper_angle_min = 32; lower_angle_max = 37; lower_angle_min = 29;
		}
		else if((currnet_rpm >= 1600)&&(currnet_rpm < 1800))
		{
			lower_rpm = 1600; upper_rpm = 1800; upper_angle_max = 32; upper_angle_min = 32; lower_angle_max = 37; lower_angle_min = 37;
		}
		else if((currnet_rpm >= 1800)&&(currnet_rpm < 2000))
		{
			lower_rpm = 1800; upper_rpm = 2000; upper_angle_max = 32; upper_angle_min = 32; lower_angle_max = 37; lower_angle_min = 37;
		}
		else if((currnet_rpm >= 2000)&&(currnet_rpm < 2200))
		{
			lower_rpm = 2000; upper_rpm = 2200; upper_angle_max = 32; upper_angle_min = 32; lower_angle_max = 38; lower_angle_min = 37;
		}
		else if((currnet_rpm >= 2200)&&(currnet_rpm < 2400))
		{
			lower_rpm = 2200; upper_rpm = 2400; upper_angle_max = 34; upper_angle_min = 32; lower_angle_max = 42; lower_angle_min = 38;
		}
		else
		{
			lower_rpm = 2400; upper_rpm = 2600; upper_angle_max = 35; upper_angle_min = 34; lower_angle_max = 42; lower_angle_min = 42;
		}

	}
	else
	{
		if(currnet_rpm < 850)
			currnet_rpm = 850;
		else if(currnet_rpm > 2450)
			currnet_rpm = 2450;

		if((currnet_rpm >= 850)&&(currnet_rpm < 1000))
		{
			lower_rpm = 850; upper_rpm = 1000; upper_angle_max = 22; upper_angle_min = 16; lower_angle_max = 22; lower_angle_min = 20;
		}
		else if((currnet_rpm >= 1000)&&(currnet_rpm < 1400))
		{
			lower_rpm = 1000; upper_rpm = 1400; upper_angle_max = 22; upper_angle_min = 22; lower_angle_max = 25; lower_angle_min = 22;
		}
		else if((currnet_rpm >= 1400)&&(currnet_rpm < 2000))
		{
			lower_rpm = 1400; upper_rpm = 2000; upper_angle_max = 22; upper_angle_min = 22; lower_angle_max = 28; lower_angle_min = 25;
		}
		else if((currnet_rpm >= 2000)&&(currnet_rpm < 2450))
		{
			lower_rpm = 2000; upper_rpm = 2450; upper_angle_max = 22; upper_angle_min = 22; lower_angle_max = 35; lower_angle_min = 28;
		}
		else
		{	
			lower_rpm = 2000; upper_rpm = 2450; upper_angle_max = 22; upper_angle_min = 22; lower_angle_max = 35; lower_angle_min = 28;
			
		}
	}

	upper_limit_angle = ((upper_angle_max - upper_angle_min)*(currnet_rpm)/(upper_rpm-lower_rpm)) + upper_angle_max - ((upper_angle_max - upper_angle_min)*(upper_rpm)/(upper_rpm-lower_rpm));
	lower_limit_angle = ((lower_angle_max - lower_angle_min)*(currnet_rpm)/(upper_rpm-lower_rpm)) + lower_angle_max - ((lower_angle_max - lower_angle_min)*(upper_rpm)/(upper_rpm-lower_rpm));

	lower_limit_angle = (-1)*lower_limit_angle;
}
//--, 220317 ysm, AUTO_TILT

float AutoTilt_Start_Angle;
void AutoLeveling(void)
{
	float max_angle_positive,max_angle_negative;
	float min_angle_positive,min_negative;

	//++, 220317 ysm, AUTO_TILT
	temp_angle = (float)Auto_Tilt_Temp_Angle; //++,--, 220317 ysm, AUTO_TILT
	#if 0
	upper_limit_angle = (float)Auto_Tilt_Upper_Limit_Angle;
	lower_limit_angle = (-1)*(float)Auto_Tilt_Lower_Limit_Angle;	
	#else
	Cal_AutoLevel_Angle();
	#endif

	if(temp_angle < upper_limit_angle)
		temp_angle = upper_limit_angle;
	//--, 220317 ysm, AUTO_TILT

	if(((InfoModel1.ModelInfo >= MODEL_25LC_9)&&(InfoModel1.ModelInfo <= MODEL_33LC_9))||
		((InfoModel1.ModelInfo >= MODEL_25L_9A)&&(InfoModel1.ModelInfo <= MODEL_35LN_9A)))
	{		
		max_angle_positive = 50;
		max_angle_negative = -50;

		//min_angle_positive = 20;
		//min_negative = -30;

		min_angle_positive = 15;
		min_negative = -15;
	}
	else
	{
		max_angle_positive = 50;
		max_angle_negative = -50;
		
		//min_angle_positive = 20;
		//min_negative = -30;	

		min_angle_positive = 15;
		min_negative = -15;
	}

	if((MAST_EQUIPMENT == 1)&&(ANGLE_EQUIPMENT == 1))
	{	

		if(Flag_DIN[INDEX_AUTO_TILT_SIG] == 1)
		{	
			COUNT_FLAG.Flag_Mast_Display = 1;

			if(Auto_Tilt_Mode_Flag == 0)
			{
				AutoTilt_Start_Angle = HCESPN.Mast_Angle;
				Auto_Tilt_Mode_Flag = 1;
			}
			
			if(Auto_Tilt_Mode_Flag == 1)
			{

				if((AutoTilt_Start_Angle >= max_angle_positive)||(AutoTilt_Start_Angle <= max_angle_negative))
				{

			        if((HCESPN.Mast_Angle <= upper_limit_angle) && (HCESPN.Mast_Angle >= lower_limit_angle))  // 15 ~ -15
					{
			
						AUTOLEVELING_Attach_Cut_Relay = 1;
						COUNT_FLAG.Flag_AutoLeveling = 1;

					}
					#if 0
					else
					{
						AUTOLEVELING_Attach_Cut_Relay = 0;
						COUNT_FLAG.Flag_AutoLeveling = 0;

					}
					#endif
				}
				else
				{		
			        if((HCESPN.Mast_Angle <= min_angle_positive)&& (HCESPN.Mast_Angle >= min_negative)) 
					{
			
						AUTOLEVELING_Attach_Cut_Relay = 1;
						COUNT_FLAG.Flag_AutoLeveling = 1;

					}
					#if 0
					else
					{
						AUTOLEVELING_Attach_Cut_Relay = 0;
						COUNT_FLAG.Flag_AutoLeveling = 0;
					}
					#endif
					
				}

				if(AUTOLEVELING_Attach_Cut_Relay == 1)
				{
					#if 0
					if((HCESPN.Mast_Angle > 15)|| (HCESPN.Mast_Angle < -15)) 
					{	
						COUNT_FLAG.Count_Tilt_Lock_Fail++;
						if(COUNT_FLAG.Count_Tilt_Lock_Fail > 100)
						{
							COUNT_FLAG.Count_Tilt_Lock_Fail = 0xF0;	
							AUTOLEVELING_Attach_Cut_Relay = 0;
							COUNT_FLAG.Flag_AutoLeveling = 0;
						
						}
					
					}
					else
					{
						COUNT_FLAG.Count_Tilt_Lock_Fail = 0;

					}
					#else

					#endif
				}
				
			}
 		}
		else
		{
			COUNT_FLAG.Flag_Mast_Display = 0;

			Auto_Tilt_Mode_Flag = 0;

			AUTOLEVELING_Attach_Cut_Relay = 0;
			COUNT_FLAG.Flag_AutoLeveling = 0;

		}

	}
}
extern unsigned short AI_DI_Low_Data1[8];
void BrakeLamp_Control(void)
{
	if((InfoModel1.ModelInfo >= MODEL_35D_9S)&&(InfoModel1.ModelInfo <= MODEL_50D_9S))
	{

		#if 0
		//if(Flag_DIN[INDEX_PARKED_REGENERATION_SIG] == 1)
		//if(AI_DI_Low_Data1[6] >= 2000)
		if(Flag_DIN[INDEX_BRAKE_SW_SIG] == 1)
			DOUT_DATA.HYD_AUTO_REGENERATION = 1;
		else
			DOUT_DATA.HYD_AUTO_REGENERATION = 0;
		#else		

		if(AD_DATA.C_BRAKE_OIL_PressureBar >= 5)
			DOUT_DATA.HYD_AUTO_REGENERATION = 1;
		else
			DOUT_DATA.HYD_AUTO_REGENERATION = 0;

		#endif
	}
	else if((InfoModel1.ModelInfo >= MODEL_25L_9A)&&(InfoModel1.ModelInfo <= MODEL_35LN_9A))
	{
	
		if(Flag_DIN[INDEX_BRAKE_SW_SIG] == 1)
			DOUT_DATA.HYD_AUTO_REGENERATION = 1;
		else
			DOUT_DATA.HYD_AUTO_REGENERATION = 0;

	}

}

void Relay_Control(void)		// 100ms
{

	Anti_Restart_Control();
	OPSS_Control();
	Auto_Parking_Control();
	DCSR_Control();
	//AutoShiftMode_Control();
	DPF_Automatic_active_regeneration_Control();
	ZeroStart();	
	
	HillStartAssistControl_Control();
	BrakeLamp_Control();

	
}

//-----------------------------------------------------------------------------------------------------------
#if TEST_MODE
void Make_FuelSensorValue(void)
{
	float Fuel_R;
	float Ref_R = 0.180;	// 180 -> 0.180 200624 //510; //++,--, 200114 ysm

	//Fuel_R =(Ref_R *Fuel_Level_rowdata)/(4095-Fuel_Level_rowdata);
	//Fuel_R =((Ref_R*3 *Fuel_Level_rowdata)*3.3)/((4096*5.2*2)-(Fuel_Level_rowdata*3.3*3));
	//Fuel_R=(3.3*(10000+20000))*Ref_R*Fuel_Level_rowdata*3/(4095*20000*5.1-(3.3*(10000+20000))*Fuel_Level_rowdata-3.3*Fuel_Level_rowdata*Ref_R);
	Fuel_R=(((3.3*(10+20))*Ref_R*Fuel_Level_rowdata*3)/(30*((4095*2*5)-(3.3*(1+2)*Fuel_Level_rowdata))-(3.3*Fuel_Level_rowdata*3*Ref_R)))*1000;
	
	HCESPN.Fuel_Sensor_Resistor = (unsigned short)Fuel_R;
}
#else
void Make_FuelSensorValue(void)
{
	float Fuel_R;
	float Ref_R = 0.180;	// 180 -> 0.180 200624 //510; //++,--, 200114 ysm

	//Fuel_R =(Ref_R *Fuel_Level_rowdata)/(4095-Fuel_Level_rowdata);
	//Fuel_R =((Ref_R*3 *Fuel_Level_rowdata)*3.3)/((4096*5.2*2)-(Fuel_Level_rowdata*3.3*3));
	//Fuel_R=(3.3*(10000+20000))*Ref_R*Fuel_Level_rowdata*3/(4095*20000*5.1-(3.3*(10000+20000))*Fuel_Level_rowdata-3.3*Fuel_Level_rowdata*Ref_R);
	Fuel_R=(((3.3*(10+20))*Ref_R*Fuel_Level_rowdata*3)/(30*((4095*2*5)-(3.3*(1+2)*Fuel_Level_rowdata))-(3.3*Fuel_Level_rowdata*3*Ref_R)))*1000;
	
	

	if((Engine_Type == DIESEL_TYPE)||(Tab_Number == 0x01))          // ++, --, 210714 ctw Auto JIG Final Test       //if(Engine_Type == DIESEL_TYPE)
	{		
		HCESPN.Fuel_Sensor_Resistor = (unsigned short)Fuel_R;
	
		if(Fuel_R < (50))
		{
			HCESPN.Fuel_Level=8;
			HCESPN.Cluster_Fuel_Gauge = 1000;   // MAX
		}
		else if((Fuel_R>= (50)) && (Fuel_R < (100)))
		{
			HCESPN.Fuel_Level=7;
			HCESPN.Cluster_Fuel_Gauge = 875;   // 7/8
		}
		else if((Fuel_R>= (100)) && (Fuel_R < (200)))
		{
			HCESPN.Fuel_Level=6;
			HCESPN.Cluster_Fuel_Gauge = 750;   // 6/8
		}
		else if((Fuel_R>= (200)) && (Fuel_R < (300)))
		{	
			HCESPN.Fuel_Level=5;
			HCESPN.Cluster_Fuel_Gauge = 625;   // 6/8
		}
		else if((Fuel_R>= (300)) && (Fuel_R < (350)))
		{
			HCESPN.Fuel_Level=4;
			HCESPN.Cluster_Fuel_Gauge = 500;   // 5/8
		}
		else if((Fuel_R>= (350)) && (Fuel_R < (400)))
		{
			HCESPN.Fuel_Level=3;
			HCESPN.Cluster_Fuel_Gauge = 375;   // 3/8
		}
		else if((Fuel_R>= (400)) && (Fuel_R < (500)))
		{
			HCESPN.Fuel_Level=2;
			HCESPN.Cluster_Fuel_Gauge = 250;   // 2/8
		}
		else if((Fuel_R>= (500)) && (Fuel_R < (600)))
		{
			HCESPN.Fuel_Level=1;
			HCESPN.Cluster_Fuel_Gauge = 125;   // 1/8
		}
		else if((Fuel_R>= (600)) && (Fuel_R < (700)))
		{
			HCESPN.Fuel_Level=0;
			HCESPN.Cluster_Fuel_Gauge = 0;   // 0/8
		}
		else if(Fuel_R >= (700))
		{
			HCESPN.Fuel_Level=0;
			HCESPN.Cluster_Fuel_Gauge = 0;   // 0/8
		}
		HCESPN.H301 = (unsigned char)((HCESPN.Cluster_Fuel_Gauge)/10);
	}
	else // LPG FUEL RATE
	{
		#if 0 //++,--, 201019 ysm
		if(RX_ECU_DATA_65266.Fuel_Rate <= 0xFAFF)
		{
			HCESPN.Fuel_Rate_LPG = RX_ECU_DATA_65266.Fuel_Rate;

		}
		else
			HCESPN.Fuel_Rate_LPG = 0;
		
		HCESPN.Fuel_Level = (unsigned char)((HCESPN.Fuel_Rate_LPG)/8);//8000
		#else
		HCESPN.Fuel_Level=0;
		HCESPN.Cluster_Fuel_Gauge = 0xff;   // 모름
		
		#endif

	}
	//HCESPN.H302 = FuelFactor[Machine_Model][HCESPN.Fuel_Level];
	
	HCESPN.H302 = 0xffff; //추후 구현
}
#endif

//++, 210409 ysm, 9S
unsigned short Make_EngineCheckLamp()
{
		float temp_R;		
		float Ref_R = 2.7; // 0.499;	//4700	// 200624

		unsigned short return_value;
	
		temp_R =((3.3*(10+20))*Ref_R*Engine_Error_Check_rowdata*3)/(30*((4095*2*5)-(3.3*(1+2)*Engine_Error_Check_rowdata))-(3.3*Engine_Error_Check_rowdata*3*Ref_R))*1000;			
	
        return_value =  (unsigned short)temp_R;

		return return_value;

}
//--, 210409 ysm, 9S

void Make_TMOilTemp()
{
		float TM_Oil_Temp_R;
#if 1
		//unsigned short Ref_R = 499;	//4700	// 200624
	
		
		float Ref_R = 2.7; // 0.499;	//4700	// 200624
	
		TM_Oil_Temp_R=((3.3*(10+20))*Ref_R*TM_Oil_Temp_rowdata*3)/(30*((4095*2*5)-(3.3*(1+2)*TM_Oil_Temp_rowdata))-(3.3*TM_Oil_Temp_rowdata*3*Ref_R))*1000;
	
		//TM_Oil_Temp_R=(3.3*(10000+20000))*Ref_R*TM_Oil_Temp_rowdata/(4095*20000*5.1-(3.3*(10000+20000))*TM_Oil_Temp_rowdata-3.3*TM_Oil_Temp_rowdata*Ref_R);
		
		HCESPN.TM_Oil_Temp_Sensor_Resistor = (unsigned short)TM_Oil_Temp_R;
	CHECK_TEMP :
		if(TM_Oil_Temp_R < (76))
		{
			HCESPN.TM_Oil_Temp=130+40;
		}
		else if((TM_Oil_Temp_R >= (76)) && (TM_Oil_Temp_R <= (96)))
		{
			HCESPN.TM_Oil_Temp=130+40;
		}
		else if((TM_Oil_Temp_R > (96)) && (TM_Oil_Temp_R <= (127)))
		{
			HCESPN.TM_Oil_Temp=120+40;
		}
		else if((TM_Oil_Temp_R >= (130)) && (TM_Oil_Temp_R <= (143)))
		{	
			HCESPN.TM_Oil_Temp=110+40;
		}
		else if((TM_Oil_Temp_R>= (151)) && (TM_Oil_Temp_R <= (164)))
		{
			HCESPN.TM_Oil_Temp=105+40;
		}
		else if((TM_Oil_Temp_R >= (167)) && (TM_Oil_Temp_R <= (185)))
		{
			HCESPN.TM_Oil_Temp=100+40;
		}
		else if((TM_Oil_Temp_R >= (196)) && (TM_Oil_Temp_R <= (214)))
		{
			HCESPN.TM_Oil_Temp=95+40;
		}
		else if((TM_Oil_Temp_R >= (219)) && (TM_Oil_Temp_R <= (243)))
		{
			HCESPN.TM_Oil_Temp=90+40;
		}
		else if((TM_Oil_Temp_R >= (256)) && (TM_Oil_Temp_R <= (282)))
		{
			HCESPN.TM_Oil_Temp=85+40;
		}
		else if((TM_Oil_Temp_R >= (298)) && (TM_Oil_Temp_R <= (322)))
		{
			HCESPN.TM_Oil_Temp=80+40;
		}
		else if((TM_Oil_Temp_R >= (392)) && (TM_Oil_Temp_R <= (476)))
		{
			HCESPN.TM_Oil_Temp=70+40;
		}
		else if((TM_Oil_Temp_R >= (538)) && (TM_Oil_Temp_R <= (650)))
		{
			HCESPN.TM_Oil_Temp=60+40;
		}
		else if((TM_Oil_Temp_R >= (748)) && (TM_Oil_Temp_R <= (904)))
		{
			HCESPN.TM_Oil_Temp=50+40;
		}
		else if((TM_Oil_Temp_R >= (1059)) && (TM_Oil_Temp_R <= (1281)))
		{
			HCESPN.TM_Oil_Temp=40+40;
		}
		else if((TM_Oil_Temp_R >= (2270)) && (TM_Oil_Temp_R <= (2730)))
		{
			HCESPN.TM_Oil_Temp=20+40;
		}
		else if((TM_Oil_Temp_R >= (5180)) && (TM_Oil_Temp_R <= (6660)))
		{
			HCESPN.TM_Oil_Temp=0+40;
		}
		else if((TM_Oil_Temp_R>= (8160)) && (TM_Oil_Temp_R <= (10740)))
		{
			HCESPN.TM_Oil_Temp=(-10)+40;
		}
		else if((TM_Oil_Temp_R>= (13240)) && (TM_Oil_Temp_R <= (18100)))
		{
			HCESPN.TM_Oil_Temp=(-20)+40;
		}
		else if((TM_Oil_Temp_R>= (22220)) && (TM_Oil_Temp_R <= (31780)))
		{
			HCESPN.TM_Oil_Temp=(-30)+40;
		}
		else if((TM_Oil_Temp_R > (31780)))
		{
			HCESPN.TM_Oil_Temp=(-30)+40;
		}
		else 
		{
			unsigned short shortData[] = {127, 130, 143, 151, 164, 167, 185, 196, 214, 219, 243, 256, 282, 298, 322, 392, 476, 538, 650, 748, 904, 1059, 1281, 2270, 2730, 5180, 6660, 8160, 10740, 13240, 18100, 22220};
	
			short Diff = 0;
			short diffMin = shortData[31];
			int Near = 0;
			short i=0;
	
			for(i=0;i<32;i++)
			{
				Diff = shortData[i] - (unsigned short)TM_Oil_Temp_R;
				if(abs(diffMin) > abs(Diff))
				{
					diffMin = Diff;
					Near = shortData[i];
				}
				
			}
			TM_Oil_Temp_R =  Near;
			goto CHECK_TEMP;
			
		}
#else
		float Ref_R = 2.7; // 0.499;	//4700	// 200624
	
		TM_Oil_Temp_R=((3.3*(10+20))*Ref_R*TM_Oil_Temp_rowdata*3)/(30*((4095*2*5)-(3.3*(1+2)*TM_Oil_Temp_rowdata))-(3.3*TM_Oil_Temp_rowdata*3*Ref_R))*1000;
		
		HCESPN.TM_Oil_Temp_Sensor_Resistor = (unsigned short)TM_Oil_Temp_R;
	
	#if 0
		if(TM_Oil_Temp_rowdata < (545)) 		//	if(TM_Oil_Temp_R < (76))
		{
			HCESPN.TM_Oil_Temp=130+40;
		}
		else if(TM_Oil_Temp_rowdata < (668))		// else if((TM_Oil_Temp_R>= (76)) && (TM_Oil_Temp_R <= (96)))
		{
			HCESPN.TM_Oil_Temp=130+40;
		}
		else if(TM_Oil_Temp_rowdata < (844))		// else if((TM_Oil_Temp_R> (96)) && (TM_Oil_Temp_R <= (127)))
		{
			HCESPN.TM_Oil_Temp=120+40;
		}
		else if(TM_Oil_Temp_rowdata < (938))		// else if((TM_Oil_Temp_R>= (130)) && (TM_Oil_Temp_R <= (143)))
		{	
			HCESPN.TM_Oil_Temp=110+40;
		}
		else if(TM_Oil_Temp_rowdata < (1026))	// else if((TM_Oil_Temp_R>= (151)) && (TM_Oil_Temp_R <= (164)))
		{
			HCESPN.TM_Oil_Temp=105+40;
		}
		else if(TM_Oil_Temp_rowdata < (1137))	// else if((TM_Oil_Temp_R>= (167)) && (TM_Oil_Temp_R <= (185)))
		{
			HCESPN.TM_Oil_Temp=100+40;
		}
		else if(TM_Oil_Temp_rowdata < (1245))	// else if((TM_Oil_Temp_R>= (196)) && (TM_Oil_Temp_R <= (214)))
		{
			HCESPN.TM_Oil_Temp=95+40;
		}
		else if(TM_Oil_Temp_rowdata < (1371))	// else if((TM_Oil_Temp_R>= (219)) && (TM_Oil_Temp_R <= (243)))
		{
			HCESPN.TM_Oil_Temp=90+40;
		}
		else if(TM_Oil_Temp_rowdata < (1511))	// else if((TM_Oil_Temp_R>= (256)) && (TM_Oil_Temp_R <= (282)))
		{
			HCESPN.TM_Oil_Temp=85+40;
		}
		else if(TM_Oil_Temp_rowdata < (1709))	// else if((TM_Oil_Temp_R>= (298)) && (TM_Oil_Temp_R <= (322)))
		{
			HCESPN.TM_Oil_Temp=80+40;
		}
		else if(TM_Oil_Temp_rowdata < (2065))	// else if((TM_Oil_Temp_R>= (392)) && (TM_Oil_Temp_R <= (476)))
		{
			HCESPN.TM_Oil_Temp=70+40;
		}
		else if(TM_Oil_Temp_rowdata < (2387))	// else if((TM_Oil_Temp_R>= (538)) && (TM_Oil_Temp_R <= (650)))
		{
			HCESPN.TM_Oil_Temp=60+40;
		}
		else if(TM_Oil_Temp_rowdata < (2709))	// else if((TM_Oil_Temp_R>= (748)) && (TM_Oil_Temp_R <= (904)))
		{
			HCESPN.TM_Oil_Temp=50+40;
		}
		else if(TM_Oil_Temp_rowdata < (3143))	// else if((TM_Oil_Temp_R>= (1059)) && (TM_Oil_Temp_R <= (1281)))
		{
			HCESPN.TM_Oil_Temp=40+40;
		}
		else if(TM_Oil_Temp_rowdata < (3583))	// else if((TM_Oil_Temp_R>= (2270)) && (TM_Oil_Temp_R <= (2730)))
		{
			HCESPN.TM_Oil_Temp=20+40;
		}
		else if(TM_Oil_Temp_rowdata < (3812))	// else if((TM_Oil_Temp_R>= (5180)) && (TM_Oil_Temp_R <= (6660)))
		{
			HCESPN.TM_Oil_Temp=0+40;
		}
		else if(TM_Oil_Temp_rowdata < (3907))	// else if((TM_Oil_Temp_R>= (8160)) && (TM_Oil_Temp_R <= (10740)))
		{
			HCESPN.TM_Oil_Temp=(-10)+40;
		}
		else if(TM_Oil_Temp_rowdata < (3971))	// else if((TM_Oil_Temp_R>= (13240)) && (TM_Oil_Temp_R <= (18100)))
		{
			HCESPN.TM_Oil_Temp=(-20)+40;
		}
		else if(TM_Oil_Temp_rowdata < (4007))	// else if((TM_Oil_Temp_R>= (22220)) && (TM_Oil_Temp_R <= (31780)))
		{
			HCESPN.TM_Oil_Temp=(-30)+40;
		}
		else 
		{
			HCESPN.TM_Oil_Temp=(-30)+40;
		}
	#else //++,--, 201019 ysm
		if(TM_Oil_Temp_R < (76))		//	if(TM_Oil_Temp_R < (76))
		{
			HCESPN.TM_Oil_Temp=130+40;
		}
		else if((TM_Oil_Temp_R>= (76)) && (TM_Oil_Temp_R <= (100))) 	// else if((TM_Oil_Temp_R>= (76)) && (TM_Oil_Temp_R <= (96)))
		{
			HCESPN.TM_Oil_Temp=130+40;
		}
		else if((TM_Oil_Temp_R> (96)) && (TM_Oil_Temp_R <= (127)))	// else if((TM_Oil_Temp_R> (96)) && (TM_Oil_Temp_R <= (127)))
		{
			HCESPN.TM_Oil_Temp=120+40;
		}
		else if((TM_Oil_Temp_R>= (130)) && (TM_Oil_Temp_R <= (143)))		// else if((TM_Oil_Temp_R>= (130)) && (TM_Oil_Temp_R <= (143)))
		{	
			HCESPN.TM_Oil_Temp=110+40;
		}
		else if((TM_Oil_Temp_R>= (151)) && (TM_Oil_Temp_R <= (164)))	// else if((TM_Oil_Temp_R>= (151)) && (TM_Oil_Temp_R <= (164)))
		{
			HCESPN.TM_Oil_Temp=105+40;
		}
		else if((TM_Oil_Temp_R>= (167)) && (TM_Oil_Temp_R <= (185)))	// else if((TM_Oil_Temp_R>= (167)) && (TM_Oil_Temp_R <= (185)))
		{
			HCESPN.TM_Oil_Temp=100+40;
		}
		else if((TM_Oil_Temp_R>= (196)) && (TM_Oil_Temp_R <= (214)))	// else if((TM_Oil_Temp_R>= (196)) && (TM_Oil_Temp_R <= (214)))
		{
			HCESPN.TM_Oil_Temp=95+40;
		}
		else if((TM_Oil_Temp_R>= (219)) && (TM_Oil_Temp_R <= (243)))	// else if((TM_Oil_Temp_R>= (219)) && (TM_Oil_Temp_R <= (243)))
		{
			HCESPN.TM_Oil_Temp=90+40;
		}
		else if((TM_Oil_Temp_R>= (256)) && (TM_Oil_Temp_R <= (282)))	// else if((TM_Oil_Temp_R>= (256)) && (TM_Oil_Temp_R <= (282)))
		{
			HCESPN.TM_Oil_Temp=85+40;
		}
		else if((TM_Oil_Temp_R>= (298)) && (TM_Oil_Temp_R <= (322)))	// else if((TM_Oil_Temp_R>= (298)) && (TM_Oil_Temp_R <= (322)))
		{
			HCESPN.TM_Oil_Temp=80+40;
		}
		else if((TM_Oil_Temp_R>= (392)) && (TM_Oil_Temp_R <= (476)))	// else if((TM_Oil_Temp_R>= (392)) && (TM_Oil_Temp_R <= (476)))
		{
			HCESPN.TM_Oil_Temp=70+40;
		}
		else if((TM_Oil_Temp_R>= (538)) && (TM_Oil_Temp_R <= (650)))	// else if((TM_Oil_Temp_R>= (538)) && (TM_Oil_Temp_R <= (650)))
		{
			HCESPN.TM_Oil_Temp=60+40;
		}
		else if((TM_Oil_Temp_R>= (748)) && (TM_Oil_Temp_R <= (904)))	// else if((TM_Oil_Temp_R>= (748)) && (TM_Oil_Temp_R <= (904)))
		{
			HCESPN.TM_Oil_Temp=50+40;
		}
		else if((TM_Oil_Temp_R>= (1059)) && (TM_Oil_Temp_R <= (1281)))	// else if((TM_Oil_Temp_R>= (1059)) && (TM_Oil_Temp_R <= (1281)))
		{
			HCESPN.TM_Oil_Temp=40+40;
		}
		else if((TM_Oil_Temp_R>= (2270)) && (TM_Oil_Temp_R <= (2730)))	// else if((TM_Oil_Temp_R>= (2270)) && (TM_Oil_Temp_R <= (2730)))
		{
			HCESPN.TM_Oil_Temp=20+40;
		}
		else if((TM_Oil_Temp_R>= (5180)) && (TM_Oil_Temp_R <= (6660)))	// else if((TM_Oil_Temp_R>= (5180)) && (TM_Oil_Temp_R <= (6660)))
		{
			HCESPN.TM_Oil_Temp=0+40;
		}
		else if((TM_Oil_Temp_R>= (8160)) && (TM_Oil_Temp_R <= (10740))) // else if((TM_Oil_Temp_R>= (8160)) && (TM_Oil_Temp_R <= (10740)))
		{
			HCESPN.TM_Oil_Temp=(-10)+40;
		}
		else if((TM_Oil_Temp_R>= (13240)) && (TM_Oil_Temp_R <= (18100)))	// else if((TM_Oil_Temp_R>= (13240)) && (TM_Oil_Temp_R <= (18100)))
		{
			HCESPN.TM_Oil_Temp=(-20)+40;
		}
		else if((TM_Oil_Temp_R>= (22220)) && (TM_Oil_Temp_R <= (31780)))	// else if((TM_Oil_Temp_R>= (22220)) && (TM_Oil_Temp_R <= (31780)))
		{
			HCESPN.TM_Oil_Temp=(-30)+40;
		}
		else 
		{
			HCESPN.TM_Oil_Temp=(-30)+40;
		}
	
	
	
	
	
	#endif
#endif


}

void Make_TMOilTemp_9S()
{
	float a_value = 41.25;
	float b_value = -81.25;
	float tm_temp;

	AD_DATA.C_TM_OIL_TEMP_TEMP[1] = TM_OIL_TEMP_SENSOR;
	AD_DATA.C_TM_OIL_TEMPERATURE = (USHORT)((B_0 * AD_DATA.C_TM_OIL_TEMP_TEMP[1]) + (B_1 * AD_DATA.C_TM_OIL_TEMP_TEMP[0]) - (A_1 * AD_DATA.C_TM_OIL_TEMPERATURE));

	AD_DATA.C_TM_OIL_TEMP_TEMP[0] = AD_DATA.C_TM_OIL_TEMP_TEMP[1];

	if((AD_DATA.C_TM_OIL_TEMP_TEMP[0] >= 1)&&(AD_DATA.C_TM_OIL_TEMP_TEMP[0] <= 5)) // 1~5V
	{
		tm_temp = a_value * AD_DATA.C_TM_OIL_TEMP_TEMP[0] + b_value;
	}
	else if(AD_DATA.C_TM_OIL_TEMP_TEMP[0] > 5)
	{
		//AD_DATA.C_TM_OIL_TEMP_TEMP[0] = 0;
		tm_temp = 125;
	}
	else
	{
		tm_temp = (-1)*40;

	}

	if(tm_temp < 0)
	{
		tm_temp = 0;
	}

	HCESPN.TM_Oil_Temp = (unsigned short)(tm_temp + 40);
	
	
}


void Make_TMOilTemperatureValue(void)
{
	if((InfoModel1.ModelInfo >= MODEL_35D_9S)&&(InfoModel1.ModelInfo <= MODEL_50D_9S))
	{
		Make_TMOilTemp_9S();
	}
	else if((InfoModel1.ModelInfo >= MODEL_25LC_9)&&(InfoModel1.ModelInfo <= MODEL_33LC_9)) //PILOT.. WIF
	{
		Make_TMOilTemp();
	}
	else if((InfoModel1.ModelInfo >= MODEL_25L_9A)&&(InfoModel1.ModelInfo <= MODEL_35LN_9A)) //++,--, 220829 ysm, 9A
	{
		Make_TMOilTemp_9S();
	}
	else
	{
		Make_TMOilTemp();
	}

}

//------------------------------------------------------------------------------
// Function Name  	: Make_SpeedValue
// Description    	: Speed gauge 구동 조건 판별.

// Speed = (Output gear 잇수 * TCU1.Output_shaft_speed ) / (60 * 8 * Output Speed Pulse(Hz))
//     50= (36*20.83333*Speed_temp)/407353;
// 20.83333 = 10000/(60*8)

// F = (2.65259 * t * i) / (60 * r)

// F : Frequency [Hz], 차량이 1km/h로 주행할때 output gear의 pulse 값
// t : output gear 잇수, (T/M의 output gear 잇수) 36  47
// i : axle gear ratio (axle 입력/출력 기어비) 10.545 11.73
// r : tire rolling radius [ m ] ( 동하중 반경, 타이어 센터에서 눌림면까지의 거리 ) 0.412 0.55

//모  델				Tire		미션구분		Z	r		R		F [Hz]
// 15/18/20D-9		 		MS 정밀		45 	14.20 	0.291 	97.246 
// 25D-9		 				MS 정밀		48 	14.20 	0.325 	92.718 
// 30/33D-9/35DA-9			MS 정밀		48 	14.20 	0.352 	85.606 
// 35D-9				 		DIC			51 	11.69 	0.405 	65.080 
// 40/45D-9,50DA-9			DIC			51 	11.69 	0.392 	67.238 
// 35D-9		 				TNA			37 	11.69 	0.405 	47.215 
// 40/45D-9,50DA-9			TNA			37 	11.69 	0.392 	48.781 
// 25D-9V			single	TNA			26 	11.57 	0.325 	40.914 
//					double	TNA			26 	11.57 	0.344 	38.654 
// 30D-9V			single	TNA			26 	11.57 	0.344 	38.654 
//					double	TNA			26 	11.57 	0.344 	38.654 
// 35DN-9V			single	TNA			26 	11.57 	0.344 	38.654 
//					double	TNA			26 	11.57 	0.344 	38.654 
// 25D-9V(B)			single	TNA			32	16.629	0.325	72.386
//					double	TNA			32	16.629	0.344	68.387
// 30D-9V(B)			single	TNA			32	16.629	0.344	68.387
//					double	TNA			32	16.629	0.344	68.387
// 35DN-9V(B)		single	TNA			32	16.629	0.344	68.387
//					double	TNA			32	16.629	0.344	68.387
// 35/40/45D-9VB, 50DN-9VB 	TNA			39 	11.69 	0.385 	52.352 

// double tire 

// Input          		: None
// Return         	: None
// Period			: Move_Gauge함수 사용시.
//------------------------------------------------------------------------------
// ++, 200819 bwk
#if 0
void Make_SpeedValue(unsigned int speed)
{
	unsigned int Speed_temp;

	// ++, 200407 bwk
#if 0
	float fflt;
	unsigned int i;
	
	Speed_Buffer[Speed_Buffer_cnt] = speed;
	if( ++Speed_Buffer_cnt > 9 )
	{
		Speed_Movingaverage_flag = 1;			Speed_Buffer_cnt = 0;
	}

	fflt = 0;

	for(i = 0; i < 10; i++)
	{
		fflt = fflt + Speed_Buffer[i];
	}

	if(Speed_Movingaverage_flag == 0)
	{
		if(Speed_Buffer_cnt == 0) 	Speed_Buffer_cnt = 1;
	}

	if(Speed_Movingaverage_flag == 1) 		fflt = fflt / 10 ; 
	else 									fflt = fflt / Speed_Buffer_cnt ; 
#endif
	// --, 200407 bwk
	Speed_temp = (unsigned int)(speed);
#if TEST_MODE
	Speed_value_temp[1] = (unsigned short)((Speed_temp/68.387)*10);	//KM/H
	if(Speed_value_temp[1]  > 999) Speed_value_temp[1]  = 999 ;
	Speed_value_temp[0] = Speed_value_temp[1];
#else
	Speed_value_temp[1] = (unsigned short)((Speed_temp/68.387)*10);	//KM/H
	if(Speed_value_temp[1]  > 999) Speed_value_temp[1]  = 999 ;
	Calculate_Speed_Value = (USHORT)((B_0 * Speed_value_temp[1]) + (B_1 * Speed_value_temp[0]) - (A_1 * Calculate_Speed_Value));
	Speed_value_temp[0] = Speed_value_temp[1];
#endif
}
#else
float Test_Speed_value_temp;
void Make_SpeedValue(float Speed_temp)
{
	float f_value;	
#if 0

	Speed_value_temp[1] = (Speed_temp/68.387)*10;	//KM/H
	if(Speed_value_temp[1]  > 999) Speed_value_temp[1]  = 999 ;
	Speed_value_temp[0] = Speed_value_temp[1];
	Test_Speed_value_temp = (60 * Speed_temp / 39) / 11.692 * 2 * 3.14159 * 0.385 * 60 / 1000;
#else

	if(Engine_Type == DIESEL_TYPE)
	{
		if((InfoModel1.ModelInfo >= MODEL_35D_9S)&&(InfoModel1.ModelInfo <= MODEL_50D_9S))
		{
			if(InfoModel3.TireInfo == 0)
			{
				f_value = 58.942; //SINGLE TIRE
			}
			else
			{	
				f_value = 61.498; //DOUBLE TIRE
			}
		}
		else
		{

			if((InfoModel1.ModelInfo >= MODEL_35D_9HDI)&&(InfoModel1.ModelInfo <= MODEL_50DN_9HDI))
			{
			
				if(InfoModel3.TireInfo == 0) // SINGLE TIRE
				{
					f_value = 51.401; // //210305 ysm, F_ +2up, ori = 49.401
				}
				else // DOUBLE TIRE
					f_value = 53.549;	//210305 ysm, F_ +2up ori = 51.549
			}
			else // 9V, 9VS
				f_value = 68.593;//SINGLE & DOUBLE TIRE	//210517 ysm, 68.387 -> 68.593
				
		}
	}
	else	
	{

		if((InfoModel1.ModelInfo >= MODEL_35L_9)&&(InfoModel1.ModelInfo <= MODEL_50L_9))
		{
		
			if(InfoModel3.TireInfo == 0) // SINGLE TIRE
			{
				f_value = 51.401; // //210305 ysm, F_ +2up, ori = 49.401
			}
			else // DOUBLE TIRE
				f_value = 53.549;	//210305 ysm, F_ +2up ori = 51.549
		}
		else // LCO
			f_value = 86.490;	//25~33LC-9  => BASE = 90.490 -> 86.490 
	}

	Speed_value_temp[1] = (Speed_temp/f_value)*10;
		
	if(Speed_value_temp[1]  > 999) Speed_value_temp[1]  = 999 ;
// ++, 200819 bwk
#if 0
	Calculate_Speed_Value = (float)((B_0 * Speed_value_temp[1]) + (B_1 * Speed_value_temp[0]) - (A_1 * Calculate_Speed_Value));
#else
	Calculate_Speed_Value = Speed_value_temp[1];

#endif
// --, 200819 bwk
	Speed_value_temp[0] = Speed_value_temp[1];
#endif

	HCESPN.H532 = (unsigned short)(Calculate_Speed_Value/10*256);
}

#endif
// --, 200819 bwk

void Make_AIN()
{
	AD_LOAD_SENSER();
	AD_HYD_OIL_TEMPERATURE();
	AD_COOLANT_TEMPERATURE(); //9S
	AD_BRAKE_OIL_PRESSURE(); //9S
	AD_HYD_OIL_PRESSURE(); //9A
	AD_TM_OIL_PRESSURE();
	Make_FuelSensorValue();
	Make_TMOilTemperatureValue();
}

void CheckClutchProtection(void)
{
	
	if(HCESPN.TM_Oil_Temp>=140)// TM OFFSET 적용 100->140(LPG, 9VB와 비교 필요), ++, 201016 YSM
	{
		if(++COUNT_FLAG.TM_Oil_Temp>=20)
		{
			COUNT_FLAG.TM_Oil_Temp=20;
			TMOilTemp_Warning = 1;
		}

		if(TMOilTemp_Warning == 1)
		{
			if(((HCESPN.Gear_542==GEAR_FORWARD) || (HCESPN.Gear_542==GEAR_REVERSE)) 
				&& (DOUT_DATA.TRAVEL_CUT_OUTPUT == 0)
				&& ((AD_DATA.C_TMOilPressureBar >= 1.3) && (AD_DATA.C_TMOilPressureBar <= 9.3)))
			{
				ClutchProtection = 1;
			}
			
		}
	}
	else
	{
		TMOilTemp_Warning = 0;
		COUNT_FLAG.TM_Oil_Temp = 0;
		
	}

	if(ClutchProtection == 1)
	{
		if(RX_ECU_DATA_61443.AcceleratorPedalPosition_91 >= 10)
		{
			if(++COUNT_FLAG.AccelPedal>=30)
			{
				COUNT_FLAG.AccelPedal=30;
				BUZZER_FLAG.ClutchProtection = 1;
			}
		}
		else
		{
			COUNT_FLAG.AccelPedal = 0;
			BUZZER_FLAG.ClutchProtection = 0;
		}
		
		if((HCESPN.TM_Oil_Temp<140) // TM OFFSET 적용 100->140(LPG, 9VB와 비교 필요), ++, 201016 YSM
			|| (AD_DATA.C_TMOilPressureBar < 1.3) || (AD_DATA.C_TMOilPressureBar > 9.3)
			|| (HCESPN.Gear_542==GEAR_NEUTRAL) 
			|| (DOUT_DATA.TRAVEL_CUT_OUTPUT == 1))
		{
			ClutchProtection = 0;
			BUZZER_FLAG.ClutchProtection = 0;
			COUNT_FLAG.AccelPedal = 0;
		}
	}
	else
	{
		BUZZER_FLAG.ClutchProtection = 0;
		COUNT_FLAG.AccelPedal = 0;
	}

	
}

//------------------------------------------------------------------------------
void Buzzer_Operating(void)		// 10ms
{
	if(WeightBuzzerOn == 1) // 1s on , 300ms off
	{
		COUNT_FLAG.Load_Buzzer++;
		if(COUNT_FLAG.Load_Buzzer<100)
			BUZZER_FLAG.LoadSensor = 1;
		else if(COUNT_FLAG.Load_Buzzer<130)
			BUZZER_FLAG.LoadSensor=0;
		else
		{
 			BUZZER_FLAG.LoadSensor=0;
 			COUNT_FLAG.Load_Buzzer=0;
		}
	}
	else
	{
		BUZZER_FLAG.LoadSensor = 0;
		COUNT_FLAG.Load_Buzzer = 0;
	}	

	//++, 210821 ysm, 9S
	if(Parking_Buzzer_flag == 1)
	{
		Parking_Buzzer_Count2++;
		Parking_Buzzer_Count++;
		if(Parking_Buzzer_Count >= 3000)
			Parking_Buzzer_Count = 3000;


		if(Parking_Buzzer_Count2>=200)
		{
			Parking_Buzzer_Count2 = 200;
		
			COUNT_FLAG.Parking_Buzzer++;
			if(COUNT_FLAG.Parking_Buzzer<50)
				BUZZER_FLAG.OPSS = 1;
			else if(COUNT_FLAG.Parking_Buzzer<100)
				BUZZER_FLAG.OPSS = 0;
			else
			{
	 			BUZZER_FLAG.OPSS = 0;
	 			COUNT_FLAG.Parking_Buzzer = 0;
			}
		}	
		
		
	}
	else if(Parking_Buzzer_flag == 2)
	{		
		Parking_Buzzer_Count2 = 0;
		Parking_Buzzer_Count++;
		if(Parking_Buzzer_Count >= 3000)
			Parking_Buzzer_Count = 3000;
		
		COUNT_FLAG.Parking_Buzzer = 0;
		BUZZER_FLAG.OPSS = 0;
	}
	//--, 210821 ysm, 9S

	//++, 220428 ysm, 9V_DPF
	if(Tilt_Alarm_flag == 1)
	{
		COUNT_FLAG.Count_Tilt_Alarm++;
		if(COUNT_FLAG.Count_Tilt_Alarm >= 100)
		{
			COUNT_FLAG.Count_Tilt_Alarm = 200;
			BUZZER_FLAG.TiltAlarm = 1;
		}
		else
		{
			BUZZER_FLAG.TiltAlarm = 0;
		}
	}
	else
	{
		COUNT_FLAG.Count_Tilt_Alarm = 0;
		BUZZER_FLAG.TiltAlarm = 0;
		COUNT_FLAG.Flag_Tilt_ESC_Buzzer_Alarm = 0;
	}
	//--, 220428 ysm, 9V_DPF

	if((BUZZER_FLAG.OPSS == 1) || (BUZZER_FLAG.AutoParking == 1) || (BUZZER_FLAG.SpeedLimit == 1) || ((BUZZER_FLAG.TiltAlarm == 1)&&(COUNT_FLAG.Flag_Tilt_ESC_Buzzer_Alarm == 0)) ||
		((BUZZER_FLAG.BrakeOilLevel == 1) && (BrakeOilLevelLow_BuzzerStop == 0)) || (COUNT_FLAG.Flag_FSCU_Error >= 1)|| (COUNT_FLAG.Flag_FSCUCommError == 1) ||(COUNT_FLAG.Flag_FSCU_Invalid_Model == 1) ||
		((InfoModel2.ClutchProtectionAlarm == 1) && (BUZZER_FLAG.ClutchProtection == 1)) || (BUZZER_FLAG.DPFRegenAlarm == 1) || (BUZZER_FLAG.SeatBeltAlarm == 1) ||
		((EngineStartFlag == 1)&&(RX_FSCU_DATA_65525.Warning_Buzzer_Demand == 1))||((InfoDisplaySetting.LoadBuzzerSet == 1) && (BUZZER_FLAG.LoadSensor == 1)))  //++,--, 230127 ysm, FSCU_HAC
	{

		DOUT_DATA.WARNING_BUZZER_SIG = 1;
	}
	else
	{
		DOUT_DATA.WARNING_BUZZER_SIG = 0;
	}	
#if 0
	HAL_GPIO_WritePin(C_DOUT_IN5_GPIO_Port, C_DOUT_IN5_Pin, (RX_JIG_DATA_65292.DO_WarningBuzzerSig== 1)?GPIO_PIN_RESET:GPIO_PIN_SET);
	HAL_GPIO_WritePin(C_BUZZER_ON_GPIO_Port, C_BUZZER_ON_Pin, (RX_JIG_DATA_65292.Buzzer== 1)?GPIO_PIN_SET:GPIO_PIN_RESET);
#else
	//HAL_GPIO_WritePin(C_DOUT_IN5_GPIO_Port, C_DOUT_IN5_Pin, (DOUT_DATA.WARNING_BUZZER_SIG == 1)?GPIO_PIN_RESET:GPIO_PIN_SET);  // EX BUZZER -> RELAY SHFIT_2
	HAL_GPIO_WritePin(C_BUZZER_ON_GPIO_Port, C_BUZZER_ON_Pin, (DOUT_DATA.WARNING_BUZZER_SIG == 1)?GPIO_PIN_SET:GPIO_PIN_RESET);
#endif
}

