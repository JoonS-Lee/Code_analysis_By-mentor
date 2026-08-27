#include "FLV_Cluster_APP.h"
#include <math.h>
#define nop()		asm("nop");

#define ACC_SCL0        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_15, GPIO_PIN_RESET)
#define ACC_SCL1        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_15, GPIO_PIN_SET)

#define ACC_SDA0        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, GPIO_PIN_RESET)
#define ACC_SDA1        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, GPIO_PIN_SET)

#define ACC_SDA_READ    HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_14)

#define ACC_SDAIN       ACCEL_PortChange(0)
#define ACC_SDAOUT      ACCEL_PortChange(1) 

unsigned char XOUT_L;
unsigned short XOUT_H;
unsigned char YOUT_L;
unsigned short YOUT_H;
unsigned char ZOUT_L;
unsigned short ZOUT_H;
unsigned char DCST_RESP;
unsigned char WHO_AM_I;
unsigned char INT_SOURCE1;
unsigned char INT_SOURCE2;
unsigned char STATUS_REG;
unsigned char INT_REL;
unsigned char CTRL_REG1;
unsigned char CTRL_REG2;
unsigned char INT_CTRL_REG2;

unsigned char write_start = 0;
unsigned char add_test = 0;
unsigned char data_test = 0;


short XOUT;
short YOUT;
short ZOUT;

float Angle_pitch_rad, Angle_roll_rad;
float tmp_roll_rad, tmp_pitch_rad;
float fix_pitch_deg_tmp,fix_roll_deg_tmp;

CTRL_REG1_DATA REG1_DATA;

extern unsigned char ANGLE_EQUIPMENT;
extern st_COUNT_FLAG COUNT_FLAG;

extern float Old_Pitch_angle;
extern float Old_Pitch_Mast_angle;
extern float Old_Roll_Angle;

extern EEPROM_MODEL_DATA1	InfoModel1;
void ACCEL_Delay(void)
{
    nop () ;nop () ;nop () ;nop () ;nop () ;nop () ;nop () ; nop () ;nop () ;nop () ;nop () ;nop () ;nop () ;nop () ;
    nop () ;nop () ;nop () ;nop () ;nop () ;nop () ;nop () ; nop () ;nop () ;nop () ;nop () ;nop () ;nop () ;nop () ;   

}


void ACCEL_PortChange(unsigned char dir)
{	
    GPIO_InitTypeDef GPIO_InitStruct;
    if(dir)	// Output
    {	
        /*Configure GPIO pins : C_ACC_SDA_Pin */
        GPIO_InitStruct.Pin = GPIO_PIN_14;
        GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
        GPIO_InitStruct.Pull = GPIO_NOPULL;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
        HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
    }
    else	// Input
    {	
        /*Configure GPIO pins : C_ACC_SDA_Pin */
        GPIO_InitStruct.Pin = GPIO_PIN_14;
        GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
        GPIO_InitStruct.Pull = GPIO_NOPULL;
        HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
    }
}


void ACCEL_START(void)
{
    ACC_SDA1;
    ACC_SDAOUT ;
    
    /* start */
    ACC_SDA1;
    ACC_SCL1 ;
    ACCEL_Delay();
    ACC_SDA0;
    ACCEL_Delay();
    ACC_SCL0 ;
}


void ACCEL_END(void)
{
    /* stop */
    ACC_SDA0;
    ACCEL_Delay();
    ACC_SCL1 ;
    ACCEL_Delay();
    ACC_SDA1;
    
    Delay(25);
}


void ACCELByteWrite(unsigned char WData)
{
    unsigned char i, j;
    
    ACCEL_Delay();
    
    for(i = 0; i < 8; i++)
    {
        j = ( WData & 0x80 ) ? 1 : 0;
        (j) ? ACC_SDA1 : ACC_SDA0;
        ACCEL_Delay();
        ACC_SCL1;
        ACCEL_Delay();
        WData <<= 1;
        ACC_SCL0;
        ACCEL_Delay();
    }
    
    ACC_SDA0;
    ACCEL_Delay();
    ACC_SCL1;
    ACCEL_Delay();
    ACC_SCL0;
}


unsigned char ACCELByteRead(void)
{
    unsigned char i;
    
    //CPPCHECK ERROR
    unsigned char RData = 0;
    
    ACCEL_Delay();
    ACC_SCL0;
    ACCEL_Delay();
    ACC_SDA1;
    ACC_SDAIN;
    
    for(i = 0; i < 8; i++)
    {
        ACC_SCL1;
        ACCEL_Delay();
        RData = (RData << 1) | ACC_SDA_READ;
        ACC_SCL0;
        ACCEL_Delay();
    }
    ACC_SDA0;
    ACC_SDAOUT;
    return(RData);
}


void ACCELWrite(unsigned short Add,  unsigned char *Data, unsigned short size)
{
    unsigned char CompareData[256];
    unsigned char Error_cnt = 5;
    
    while(Error_cnt)
    {
        ACCEL_START();
        
        ACCELByteWrite(0x1C);
        ACCELByteWrite((unsigned char)(Add >> 8));
        ACCELByteWrite((unsigned char)Add);
        
        for(int i = 0; i < size; i++)
            ACCELByteWrite(Data[i]);
        
        ACCEL_END();
        
        for(unsigned int i = 0; i < 1000; i++);
        
        ACCELRead(Add, CompareData, size);
        
        if(memcmp(Data, CompareData, size) == 0)
            break;
        
        Error_cnt--;
    }
    Delay(600000);
}


void ACCEL_Write_Byte(unsigned short Add,  unsigned int Data, unsigned short size)
{
    unsigned char CompareData;
    unsigned char Error_cnt = 5;
    unsigned char Position;
    
    while(Error_cnt)
    {
        ACCEL_START();
        
        ACCELByteWrite(0x1C);
        ACCELByteWrite((unsigned char)Add);
        
        for(unsigned short i = 0; i < size; i++)
        {
            Position = i * 8;
            ACCELByteWrite((Data>>Position)&0xFF);
        }
        
        ACCEL_END();
        
        for(unsigned int i = 0; i < 1000; i++);
        ACCELRead(Add, &CompareData, 1);
        
        if(CompareData == Data)
            break;
        
        Error_cnt--;
    }
    Delay(600000);
}


void ACCELRead(unsigned short Add, unsigned char *Data, unsigned short size)
{
    ACCEL_START();
    
    ACCELByteWrite(0x1C);
    
    ACCELByteWrite((unsigned char)Add);
    
    /* start */
    ACC_SDA1;
    ACCEL_Delay();
    ACC_SCL1;
    ACCEL_Delay();
    
    ACC_SDA0;
    ACCEL_Delay();
    ACC_SCL0;
    /* start end */
    
    ACCELByteWrite(0x1D);
    
    for(int i = 0; i < (size - 1); i++)
    {
        Data[i] = ACCELByteRead();
        /* ack */
        ACC_SDA0;
        ACCEL_Delay();
        ACC_SCL1;
        ACCEL_Delay();
        ACC_SCL0;
        ACCEL_Delay();
    }
    
    Data[size - 1] = ACCELByteRead();
    /* nak */
    ACC_SDA1;
    ACCEL_Delay();
    ACC_SCL1;
    ACCEL_Delay();
    ACC_SCL0;
    ACCEL_Delay();
    
    /* stop */
    ACCEL_END();
    
    ACCEL_Delay();
}

float PredictAccG_roll(short a_z, short a_y, short a_x)
{
	float tmp;
	float ka_x,ka_y;
	//float ka_xz;

	ka_x = (float)a_x;
	ka_y = (float)a_y;
	//ka_z = (float)a_z;
	
	//ka_xz = sqrt(pow(a_x,2)+pow(a_z,2));

	#if 1
	if( ka_y < 0 )
	{
		//tmp_roll_rad = atan2(ka_y, -ka_x) + (3.14 * 2.0);
		tmp_roll_rad = atan2(ka_y, -ka_x) + (3.14 * 2.0);

	}
	else
	{	
		tmp_roll_rad = atan2(ka_y, -ka_x);

	}
	#else
	tmp_roll_rad = atan2(ka_x, ka_y);

	//tmp_roll_rad = atan2(ka_xz, ka_y);

	
	#endif

	tmp = tmp_roll_rad;
	return tmp;

}

float PredictAccG_pitch(short a_z, short a_y, short a_x)
{
	float tmp;
	float ka_x,ka_z;
	
	ka_x = (float)a_x;
	ka_z = (float)a_z;	  

	#if 1
	if(ka_z < 0 ) 
	{
		tmp_pitch_rad = (3.14 * 2.0) + atan2(ka_z, -ka_x);

	}
	else
	{
		tmp_pitch_rad = atan2(ka_z, -ka_x);

	}
	#else
	tmp_pitch_rad = atan2(ka_z, ka_x);

	#endif

	tmp = tmp_pitch_rad;
	return tmp;

}

short Calculate_Accelator2G_Output(unsigned short data)
{
	short return_value;
	
	if(data > 0xFFF)
		data = 0xFFF;
	
	if(data<=2047)
		return_value = (short)data;
	else
	{	
		return_value = (short)data - 4096;	
	}	

	return return_value;
}

short Calculate_Accelator_Output(unsigned short data)
{
	short return_value;
	
	if(data > 0x3FFF)
		data = 0x3FFF;
	
	if(data<=8191)
		return_value = (short)data;
	else
	{	
		//return_value = -1*((short)((~data)&0x3FFF)+1);
		return_value = (short)data - 16384;
	}

	return return_value;
}

// 0xDC  , 16G
// 0xC0  , 2G
void ACC_Test(void)
{
	unsigned short acc_data_x,acc_data_y,acc_data_z;


	if(gHardware_Version == RevC_01)
	{

	    if(write_start == 0)
	    {
	        write_start = 1;
 

	        //ACCEL_Write_Byte(0x1B, 0xDC, 1);   // 14bit, 16G
              
            ACCEL_Write_Byte(0x1B, 0xD8, 1); // 14bit, 8G
			//ACCEL_Write_Byte(0x1B, 0xC0, 1); // 12bit, 2G
	    }

	    ACCELRead(0x06, (unsigned char*)&XOUT_L, 1);
	    ACCELRead(0x07, (unsigned char*)&XOUT_H, 1);
	    ACCELRead(0x08, (unsigned char*)&YOUT_L, 1);
	    ACCELRead(0x09, (unsigned char*)&YOUT_H, 1);
	    ACCELRead(0x0A, (unsigned char*)&ZOUT_L, 1);
	    ACCELRead(0x0B, (unsigned char*)&ZOUT_H, 1);

		#if 1
		// 14-bit
		acc_data_x = ((XOUT_H<<6)|(XOUT_L>>2));
		XOUT = Calculate_Accelator_Output(acc_data_x);
		
		acc_data_y = ((YOUT_H<<6)|(YOUT_L>>2));
		YOUT = Calculate_Accelator_Output(acc_data_y);

	 	acc_data_z = ((ZOUT_H<<6)|(ZOUT_L>>2));
	  	ZOUT = Calculate_Accelator_Output(acc_data_z);
		
		#else
		// 12-bit
		acc_data_x = ((XOUT_H<<4)|(XOUT_L>>4));
		XOUT = Calculate_Accelator2G_Output(acc_data_x);
		
		acc_data_y = ((YOUT_H<<4)|(YOUT_L>>4));
		YOUT = Calculate_Accelator2G_Output(acc_data_y);

	 	acc_data_z = ((ZOUT_H<<4)|(ZOUT_L>>4));
	  	ZOUT = Calculate_Accelator2G_Output(acc_data_z);
		#endif
		Angle_pitch_rad = PredictAccG_pitch(ZOUT, YOUT, XOUT);
		Angle_roll_rad = PredictAccG_roll(ZOUT, YOUT, XOUT);

		if(Angle_pitch_rad>=0)
			fix_pitch_deg_tmp = Angle_pitch_rad*(180/3.14159)*10;  // Pitch
		else
			fix_pitch_deg_tmp = Angle_pitch_rad*(180/3.14159)*(-10);  // Pitch

		if(Angle_roll_rad>=0)
			fix_roll_deg_tmp = Angle_roll_rad*(180/3.14159)*10;
		else	
			fix_roll_deg_tmp = Angle_roll_rad*(180/3.14159)*(-10);

		#if 0
		if((InfoModel1.ModelInfo >= MODEL_35D_9S)&&(InfoModel1.ModelInfo <= MODEL_50D_9S))
		{
			ANGLE_EQUIPMENT = 0;
		}
		else if((InfoModel1.ModelInfo >= MODEL_25D_9VS)&&(InfoModel1.ModelInfo <= MODEL_35DN_9VS))
		{
			ANGLE_EQUIPMENT = 1;
		}
		else if(InfoModel1.ModelInfo <= MODEL_35DN_9VB)
		{
			ANGLE_EQUIPMENT = 1;
		}
		else
			ANGLE_EQUIPMENT = 1;
		#else
		ANGLE_EQUIPMENT = 1;
		#endif
		
		COUNT_FLAG.Count_ANGLE_CAN_Err = 0;
	}
}


