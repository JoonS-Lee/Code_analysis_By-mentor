#include "FLV_Cluster_APP.h"

float Old_mast_ang_buf[2];
unsigned short temp_mast_angle[2];
float Old_mast_angle;
float Old_mast_angle_Disp;
float Old_mast_angle_Disp_buf[2];
float Ref_Mast_Angle;

unsigned char Pitch_deg_Count;
unsigned short Pitch_buffer[2]; 

unsigned char Roll_deg_Count;
unsigned short Roll_buffer[2];

//++, 220428 ysm, 9V_DPF
unsigned char Tilt_Alarm_flag;
//--, 220428 ysm, 9V_DPF

//++, 230726 ysm, TILT_ALARM
unsigned char Tilt_Alarm_Count;
//--, 230726 ysm, TILT_ALARM
float Old_Pitch_angle, Old_Roll_Angle;
float Old_Pitch_Angle_HAC;
float Old_Pitch_angle_Disp;
float Old_Pitch_angle_buf[2],Old_Roll_Angle_buf[2];

float Old_Pitch_angle2_buf[2];
float Old_Pitch_angle2;

float temp_vehicle_angleX = 0;
float Average_Pitch_Angle,Average_Roll_Angle;
float Ref_Pitch_Angle, Ref_Roll_Angle;

float Ref_Pitch_Angle3, Ref_Roll_Angle3;

float Stb_Ref_Pitch_Angle, Stb_Ref_Roll_Angle;

unsigned short Angle_Filter_Count = 0;

//++, 220222 ysm, AUTO_TILT
unsigned char Auto_Tilt_Filter_Value;
float temp_filter_val;
//--, 220222 ysm, AUTO_TILT


extern st_HCESPN 		HCESPN;
extern st_COUNT_FLAG 	COUNT_FLAG;
extern unsigned char	Equipment_Calibration;
extern G_SENSOR_DATA_61460*						TX_G_SENSOR_DATA_61460;

//++, 220818 ysm, ACC_TEST2
extern ACC_SENSOR_DATA_65416*					TX_ACC_SENSOR_DATA_65416;
extern ACC_TUNE									ACCS;
//--, 220818 ysm, ACC_TEST2

//++, 210524 ysm, ACC_TEST
extern unsigned char MAST_EQUIPMENT;
extern unsigned char ANGLE_EQUIPMENT;
//--, 210524 ysm, ACC_TEST
extern float Calculate_Speed_Value;
extern unsigned short Angle_Filter_Count;


extern float fix_pitch_deg_tmp;
extern float fix_roll_deg_tmp;

extern EEPROM_MODEL_DATA1	InfoModel1;


void Initialize_Mast_Variable(void)
{
	float tempfloat;
	
	memset(&Old_mast_ang_buf,0,sizeof(Old_mast_ang_buf));
	memset(&temp_mast_angle,0,sizeof(temp_mast_angle));
	memset(&Old_Pitch_angle_buf,0,sizeof(Old_Pitch_angle_buf));
	memset(&Old_Roll_Angle_buf,0,sizeof(Old_Roll_Angle_buf));	
	memset(&Old_Pitch_angle2_buf,0,sizeof(Old_Pitch_angle2_buf));

	Old_mast_angle = 0;
	Average_Pitch_Angle = Average_Roll_Angle = Old_Pitch_angle = Old_Roll_Angle = Old_Pitch_angle2 = 0;
	Average_Roll_Angle = Ref_Pitch_Angle = Ref_Roll_Angle = 0;
	Ref_Mast_Angle = 0;

	Stb_Ref_Pitch_Angle =Stb_Ref_Roll_Angle=0;

	EepromRead(ADDRESS_PITCH_ANGLE, (unsigned char*)(&tempfloat), 4);
	Ref_Pitch_Angle = tempfloat;

	EepromRead(ADDRESS_ROLL_ANGLE, (unsigned char*)(&tempfloat), 4);
	Ref_Roll_Angle = tempfloat;

	EepromRead(ADDRESS_MAST_REF_ANGLE, (unsigned char*)(&tempfloat), 4);
	Ref_Mast_Angle = tempfloat;	

	if(Ref_Pitch_Angle==0xffff || Ref_Pitch_Angle==0x0000)	Ref_Pitch_Angle=0;
	if(Ref_Roll_Angle==0xffff || Ref_Roll_Angle==0x0000)		Ref_Roll_Angle=0;

	Tilt_Alarm_flag = 0; //++,--, 220428 ysm, 9V_DPF
	Tilt_Alarm_Count = 0; //++,--, 230726 ysm, TILT_ALARM

}

void Calculate_Mast_Angle(void)
{
	
	float Temp_Mast_Angle;

	Old_mast_ang_buf[1] = (float)temp_mast_angle[0];

	if(Angle_Filter_Count < 200)
	{
		Old_mast_angle = (0.1*Old_mast_ang_buf[1] + 0.1*Old_mast_ang_buf[0] + 0.8*Old_mast_angle);
	}
	else
	{
		if((COUNT_FLAG.Flag_AutoLeveling == 1)&&(gHardware_Version == RevC_01))
			Old_mast_angle = (0.01*Old_mast_ang_buf[1] + 0.01*Old_mast_ang_buf[0] + 0.98*Old_mast_angle);
		else
			Old_mast_angle = (0.05*Old_mast_ang_buf[1] + 0.05*Old_mast_ang_buf[0] + 0.9*Old_mast_angle);

	}
	Old_mast_ang_buf[0] = Old_mast_ang_buf[1];

	Temp_Mast_Angle = (float)(((Old_mast_angle * 0.002)-64))-Ref_Mast_Angle;

	TX_ACC_SENSOR_DATA_65416->Mast = (unsigned short)((Temp_Mast_Angle/10+64)*500);
	
	Temp_Mast_Angle = Temp_Mast_Angle - (temp_vehicle_angleX/10);

	HCESPN.Mast_Angle = Old_mast_angle_Disp_buf[0] = Temp_Mast_Angle*10;
	
	Old_mast_angle_Disp = (0.1*Old_mast_angle_Disp_buf[1] + 0.1*Old_mast_angle_Disp_buf[0] + 0.8*Old_mast_angle_Disp);	
	Old_mast_angle_Disp_buf[1] = Old_mast_angle_Disp_buf[0];

	if(gHardware_Version == RevC_01)
	{
		HCESPN.Mast_Angle_Disp = Old_mast_angle_Disp;		
	}
	else
	{
		HCESPN.Mast_Angle_Disp = HCESPN.Mast_Angle;
	}
	
	TX_G_SENSOR_DATA_61460->Mast= (unsigned short)((Temp_Mast_Angle+64)*500);
	TX_ACC_SENSOR_DATA_65416->Vehicle_Mast = (unsigned short)((Temp_Mast_Angle+64)*500);
	
	
	//++, 220428 ysm, 9V_DPF
	if((InfoModel1.TonInfo > 35)||(InfoModel1.ModelInfo == MODEL_35D_9HDI)||(InfoModel1.ModelInfo == MODEL_35D_9S)||(InfoModel1.ModelInfo == MODEL_35L_9))
	{
		if(HCESPN.Mast_Angle <= -60)
		{
			Tilt_Alarm_Count = 0;
			Tilt_Alarm_flag = 1;			

		}
		else
		{
			if(Tilt_Alarm_flag == 1)
			{
				Tilt_Alarm_Count++;

				if(Tilt_Alarm_Count > 20)
					Tilt_Alarm_flag = 0;
			}
			else
			{
				Tilt_Alarm_flag = 0;
				Tilt_Alarm_Count = 0;
			}

		}
	}
	else
	{
		Tilt_Alarm_flag = 0;
		Tilt_Alarm_Count = 0;
		
	}
	//--, 220428 ysm, 9V_DPF

}


unsigned char Stabilizing_Pitch_Angle(float Data_Pitch)
{
	if((Data_Pitch>=17)||(Data_Pitch<=-17))
	{
		if(Stb_Ref_Pitch_Angle >= Data_Pitch)
		{
			if(Stb_Ref_Pitch_Angle-Data_Pitch<=30)
			{
				COUNT_FLAG.Count_Pitch_Angle++;
			}
			else
			{
				if((Data_Pitch>=20)||(Data_Pitch<=-20))
					Stb_Ref_Pitch_Angle=Data_Pitch;
				COUNT_FLAG.Count_Pitch_Angle=0;
				
			}
		}
		else
		{
			if(Data_Pitch - Stb_Ref_Pitch_Angle <= 30)
			{
				COUNT_FLAG.Count_Pitch_Angle++;
			}
			else
			{
				if((Data_Pitch>=20)||(Data_Pitch<=-20))
					Stb_Ref_Pitch_Angle=Data_Pitch;
				COUNT_FLAG.Count_Pitch_Angle=0;
			}
		}
	}
	else
		return 1;  // red off
	
	if(COUNT_FLAG.Count_Pitch_Angle>=200)
	{
		COUNT_FLAG.Count_Pitch_Angle=200;
		return 0; // red on
	}
	else
	{
		return 1; // red off
	}
}

void Cal_Vehicle_X_Angle_AccelSensor()
{
	unsigned char accel_pos;

	float hacF_value;
	float dispxF_value;

	if(ACCS.TUNE_Enable == 1)
	{
		hacF_value = (float)ACCS.HACX_Filter_Value * 0.001;
		dispxF_value = (float)ACCS.DISPX_Filter_Value * 0.001;
	}
	else
	{
		hacF_value 	 = 20 * 0.001;
		dispxF_value = 10 * 0.001;

	}

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
	


	Old_Pitch_angle_buf[1] = fix_pitch_deg_tmp;

	if(Angle_Filter_Count < 200)
	{
		Old_Pitch_angle_Disp = Old_Pitch_angle = (0.1*Old_Pitch_angle_buf[1] + 0.1*Old_Pitch_angle_buf[0] + 0.8*Old_Pitch_angle);
		

	}
	else
	{
		Old_Pitch_angle = (0.1*Old_Pitch_angle_buf[1] + 0.1*Old_Pitch_angle_buf[0] + (1-2*0.1)*Old_Pitch_angle);
		
		Old_Pitch_angle_Disp = (dispxF_value*Old_Pitch_angle_buf[1] + dispxF_value*Old_Pitch_angle_buf[0] + (1-2*dispxF_value)*Old_Pitch_angle_Disp);		
	}


	if((HCESPN.Gear_542==GEAR_FORWARD) && (Flag_DIN[INDEX_BRAKE_SW_SIG] == 1) && (accel_pos == 0)
		&& (Calculate_Speed_Value == 0))
	{
		COUNT_FLAG.Flag_HAC_Check = 1;

		#if 0
		if(((InfoModel1.ModelInfo >= MODEL_25LC_9)&&(InfoModel1.ModelInfo <= MODEL_33LC_9))
			||((InfoModel1.ModelInfo >= MODEL_25D_9HDI) && (InfoModel1.ModelInfo <= MODEL_35DN_9HDI)))
			Old_Pitch_Angle_HAC = (0.1*Old_Pitch_angle_buf[1] + 0.1*Old_Pitch_angle_buf[0] + (1-2*0.1)*Old_Pitch_Angle_HAC);
		else
			Old_Pitch_Angle_HAC = (0.01*Old_Pitch_angle_buf[1] + 0.01*Old_Pitch_angle_buf[0] + (1-2*0.01)*Old_Pitch_Angle_HAC);
		#else
		Old_Pitch_Angle_HAC = (hacF_value*Old_Pitch_angle_buf[1] + hacF_value*Old_Pitch_angle_buf[0] + (1-2*hacF_value)*Old_Pitch_Angle_HAC);
		#endif
	}
	else
	{
		COUNT_FLAG.Flag_HAC_Check = 0;

		#if 0
		Old_Pitch_Angle_HAC = Old_Pitch_angle_buf[1];
		#else
		Old_Pitch_Angle_HAC = (hacF_value*Old_Pitch_angle_buf[1] + hacF_value*Old_Pitch_angle_buf[0] + (1-2*hacF_value)*Old_Pitch_Angle_HAC);
		#endif
	}
	
	Old_Pitch_angle_buf[0] = Old_Pitch_angle_buf[1];


	//MAST ANGLE CALCULATION ////////////////////////////////////////////////////////////////////////////////////
	Old_Pitch_angle2_buf[1] = Old_Pitch_angle_buf[1];

	if(Angle_Filter_Count < 200)
	{
		Old_Pitch_angle2 = (0.1*Old_Pitch_angle2_buf[1] + 0.1*Old_Pitch_angle2_buf[0] + 0.8*Old_Pitch_angle2);
	}
	else
	{
		//++, 220317 ysm, AUTO_TILT
	#if 0
		Old_Pitch_angle2 = (temp_filter_val*Old_Pitch_angle2_buf[1] + temp_filter_val*Old_Pitch_angle2_buf[0] + (1-2*temp_filter_val)*Old_Pitch_angle2);
	#else
		if(COUNT_FLAG.Flag_AutoLeveling == 1)
			Old_Pitch_angle2 = (0.01*Old_Pitch_angle2_buf[1] + 0.01*Old_Pitch_angle2_buf[0] + 0.98*Old_Pitch_angle2);
		else
			Old_Pitch_angle2 = (0.05*Old_Pitch_angle2_buf[1] + 0.05*Old_Pitch_angle2_buf[0] + 0.9 *Old_Pitch_angle2);
	#endif
		//--, 220317 ysm,  AUTO_TILT
	}
	Old_Pitch_angle2_buf[0] = Old_Pitch_angle2_buf[1];	
	//////////////////////////////////////////////////////////////////////////////////////////////////////////

}

void Cal_Vehicle_X_Angle_CAN()
{

	Old_Pitch_angle_buf[1] = (float)Pitch_buffer[0];

	if(Angle_Filter_Count < 200)
	{
		Old_Pitch_angle = (0.1*Old_Pitch_angle_buf[1] + 0.1*Old_Pitch_angle_buf[0] + 0.8*Old_Pitch_angle);
	}
	else
	{
		Old_Pitch_angle = (0.001*Old_Pitch_angle_buf[1] + 0.001*Old_Pitch_angle_buf[0] + 0.998*Old_Pitch_angle);
	}
	Old_Pitch_angle_buf[0] = Old_Pitch_angle_buf[1];

	//MAST ANGLE CALCULATION ////////////////////////////////////////////////////////////////////////////////////
	Old_Pitch_angle2_buf[1] = (float)Pitch_buffer[0];

	if(Angle_Filter_Count < 200)
	{
		Old_Pitch_angle2 = (0.1*Old_Pitch_angle2_buf[1] + 0.1*Old_Pitch_angle2_buf[0] + 0.8*Old_Pitch_angle2);
	}
	else
	{
		Old_Pitch_angle2 = (0.05*Old_Pitch_angle2_buf[1] + 0.05*Old_Pitch_angle2_buf[0] + 0.9*Old_Pitch_angle2);
	}
	Old_Pitch_angle2_buf[0] = Old_Pitch_angle2_buf[1];	
	//////////////////////////////////////////////////////////////////////////////////////////////////////////

}


void Pitch_deg_Processing() // ÁÂ¿ì 
{	

	if(gHardware_Version == RevC_01)
	{
		Cal_Vehicle_X_Angle_AccelSensor();

		HCESPN.Vehicle_Angle_X  = Ref_Pitch_Angle - Old_Pitch_Angle_HAC;
		HCESPN.Vehicle_Angle_X_Disp = Ref_Pitch_Angle - Old_Pitch_angle_Disp;
		temp_vehicle_angleX = Ref_Pitch_Angle-Old_Pitch_angle2;
		
	}
	else
	{
		Cal_Vehicle_X_Angle_CAN();
		
		HCESPN.Vehicle_Angle_X = (float)((Ref_Pitch_Angle - (((Old_Pitch_angle * 0.002)-64)))*10);
		HCESPN.Vehicle_Angle_X_Disp = HCESPN.Vehicle_Angle_X;
		temp_vehicle_angleX = (float)((Ref_Pitch_Angle - (((Old_Pitch_angle2 * 0.002)-64)))*10);
		
	}	
	
	TX_G_SENSOR_DATA_61460->Vehicle_Angle_X = (unsigned short)(((HCESPN.Vehicle_Angle_X/10)+64)*500);	

	//++, 220818 ysm, ACC_TEST2
	#if 0
	TX_ACC_SENSOR_DATA_65416->Vehicle_Angle_X = TX_G_SENSOR_DATA_61460->Vehicle_Angle_X;
	TX_ACC_SENSOR_DATA_65416->Vehicle_Angle_Y =  (unsigned short)(((HCESPN.Vehicle_Angle_X_Disp/10)+64)*500);	
	TX_ACC_SENSOR_DATA_65416->Mast = TX_G_SENSOR_DATA_61460->Mast;
	#else
	//TX_ACC_SENSOR_DATA_65416->Vehicle_Angle_X = (unsigned short)((((Ref_Pitch_Angle - Old_Pitch_angle)/10)+64)*500);	
	TX_ACC_SENSOR_DATA_65416->Vehicle_Angle_X = (unsigned short)((((Ref_Pitch_Angle - Old_Pitch_angle2)/10)+64)*500);	
	TX_ACC_SENSOR_DATA_65416->Vehicle_Angle_Y =  (unsigned short)(((HCESPN.Vehicle_Angle_X/10)+64)*500);		
	#endif
	
	
	//--, 220818 ysm, ACC_TEST2
	
	HCESPN.Pitch_Angle_Stabilize_Status = Stabilizing_Pitch_Angle(HCESPN.Vehicle_Angle_X);
	
	
}

unsigned char  Stabilizing_Roll_Angle(float Data_Roll)
{
	if((Data_Roll>=32)||(Data_Roll<=-32))
	{
		if(Stb_Ref_Roll_Angle >= Data_Roll)
		{
			if(Stb_Ref_Roll_Angle-Data_Roll<=30)
			{
				COUNT_FLAG.Count_Roll_Angle++;
			}
			else
			{
				if((Data_Roll>=35)||(Data_Roll<=-35))
					Stb_Ref_Roll_Angle=Data_Roll;
				COUNT_FLAG.Count_Roll_Angle=0;
			}
		}
		else
		{
			if(Data_Roll - Stb_Ref_Roll_Angle <= 30)
			{
				COUNT_FLAG.Count_Roll_Angle++;
			}
			else
			{
				if((Data_Roll>=35)||(Data_Roll<=-35))
					Stb_Ref_Roll_Angle=Data_Roll;
				COUNT_FLAG.Count_Roll_Angle=0;
			}
		}
	}
	else
		return 1;

	if(COUNT_FLAG.Count_Roll_Angle>=200)
	{
		COUNT_FLAG.Count_Roll_Angle=200;
		return 0;
	}
	else
	{
		return 1;
	}
}




void Cal_Vehicle_Y_Angle_AccelSensor()
{
	Old_Roll_Angle_buf[1] = fix_roll_deg_tmp;

	if(Angle_Filter_Count < 200)
	{
		Old_Roll_Angle = (0.1*Old_Roll_Angle_buf[1] + 0.1*Old_Roll_Angle_buf[0] + 0.8*Old_Roll_Angle);
	}
	else
	{
		Old_Roll_Angle = (0.005*Old_Roll_Angle_buf[1] + 0.005*Old_Roll_Angle_buf[0] + 0.99*Old_Roll_Angle);

	}
	Old_Roll_Angle_buf[0] = Old_Roll_Angle_buf[1];

}

void Cal_Vehicle_Y_Angle_CAN()
{

	Old_Roll_Angle_buf[1] = (float)Roll_buffer[0];

	if(Angle_Filter_Count < 200)
	{
		Old_Roll_Angle = (0.1*Old_Roll_Angle_buf[1] + 0.1*Old_Roll_Angle_buf[0] + 0.8*Old_Roll_Angle);
	}
	else
	{
		Old_Roll_Angle = (0.001*Old_Roll_Angle_buf[1] + 0.001*Old_Roll_Angle_buf[0] + 0.998*Old_Roll_Angle);
	}
	Old_Roll_Angle_buf[0] = Old_Roll_Angle_buf[1];


}


void Roll_deg_Processing() //Àü ÈÄ¹æ 
{
		
	if(gHardware_Version == RevC_01)
	{
		Cal_Vehicle_Y_Angle_AccelSensor();
		HCESPN.Vehicle_Angle_Y  = Ref_Roll_Angle-Old_Roll_Angle;
	}
	else
	{
		Cal_Vehicle_Y_Angle_CAN();
		HCESPN.Vehicle_Angle_Y = (float)(((((Old_Roll_Angle * 0.002)-64))-Ref_Roll_Angle)*10);
	}		

	TX_G_SENSOR_DATA_61460->Vehicle_Angle_Y = (unsigned short)(((HCESPN.Vehicle_Angle_Y/10)+64)*500);

	//++, 220818 ysm, ACC_TEST2
	//TX_ACC_SENSOR_DATA_65416->Vehicle_Angle_Y = TX_G_SENSOR_DATA_61460->Vehicle_Angle_Y;
	//--, 220818 ysm, ACC_TEST2
	
	HCESPN.Roll_Angle_Stabilize_Status = Stabilizing_Roll_Angle(HCESPN.Vehicle_Angle_Y);


}


void Check_Vehicle_Angle()
{
	float tempfloat;

	//++, 210524 ysm, ACC_TEST
	#if 0
	Calculate_Mast_Angle();
	Pitch_deg_Processing();
	Roll_deg_Processing();
	#else

	if(++Angle_Filter_Count >= 100)
		Angle_Filter_Count = 200;


	Pitch_deg_Processing();
	Roll_deg_Processing();

	if(MAST_EQUIPMENT == 1)
	{
		if(++COUNT_FLAG.Count_MAST_CAN_Err >= 200)
		{
			MAST_EQUIPMENT = 0;
			COUNT_FLAG.Count_MAST_CAN_Err = 250;
		}
	}

	if((MAST_EQUIPMENT == 1)&&(ANGLE_EQUIPMENT == 1))
	{
		Calculate_Mast_Angle();
	}
	#endif
	//--, 210524 ysm, ACC_TEST

	if ( Equipment_Calibration == CALIBRATION_ANGLE_INITIAL ) // Vehicle angle Init
	{

		if(gHardware_Version == RevC_01)
		{
			tempfloat =  Old_Pitch_angle;
			EepromWrite(ADDRESS_PITCH_ANGLE, (unsigned char*)(&tempfloat), 4);
			Ref_Pitch_Angle = tempfloat;
			
			tempfloat =  Old_Roll_Angle;
			EepromWrite(ADDRESS_ROLL_ANGLE, (unsigned char*)(&tempfloat), 4);
			Ref_Roll_Angle = tempfloat;
			
			tempfloat = (float)((Old_mast_angle*0.002)-64);
			EepromWrite(ADDRESS_MAST_REF_ANGLE, (unsigned char*)(&tempfloat), 4);
			Ref_Mast_Angle = tempfloat;
		}
		else
		{
			tempfloat = (float)((Old_Pitch_angle*0.002)-64);
			EepromWrite(ADDRESS_PITCH_ANGLE, (unsigned char*)(&tempfloat), 4);
			Ref_Pitch_Angle = tempfloat;
			
			tempfloat = (float)((Old_Roll_Angle*0.002)-64);
			EepromWrite(ADDRESS_ROLL_ANGLE, (unsigned char*)(&tempfloat), 4);
			Ref_Roll_Angle = tempfloat;
			
			tempfloat = (float)((Old_mast_angle*0.002)-64);
			EepromWrite(ADDRESS_MAST_REF_ANGLE, (unsigned char*)(&tempfloat), 4);
			Ref_Mast_Angle = tempfloat;
		}
			
		Equipment_Calibration = CALIBRATION_NO;
	}
}

