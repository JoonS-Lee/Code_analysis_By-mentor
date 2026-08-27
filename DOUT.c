#include "FLV_Cluster_APP.h"

st_DOUT_DATA				DOUT_DATA;

extern CAN_JIG_DATA_65292	RX_JIG_DATA_65292;
extern st_BUZZER				BUZZER_FLAG;
extern unsigned char			Flag_ESL;		// ++, --, 200326 bwk
extern unsigned CAN_DATA_0xABAB_Flag;                // ++, --, 210904 ctw Auto JIG Final Test
extern unsigned char Tab_Number;                         // ++, --, 210904 ctw Auto JIG Final Test

//++, 211124 ysm
extern unsigned char MSS_ESL_Flag;
//--, 211124 ysm
extern unsigned char OPSS_Travel_Cut_Relay;
extern unsigned char OPSS_Attach_Cut_Relay;
extern unsigned char APC_Parking_Relay;
extern unsigned char DCSR_Travel_Cut_Relay;
extern unsigned char ZEROSTART_Travel_Cut_Relay;
extern unsigned char AUTOLEVELING_Attach_Cut_Relay;
extern unsigned char HAC_Parking_Relay;

//++, 221226 ysm, FSCU
unsigned char FLT_ERROR_Travel_Cut_Relay;
unsigned char FSCU_ERROR_Travel_Cut_Relay;

unsigned char FNR_ERROR_Travel_Cut_Relay;


extern CAN_FSCU_STATE1_65525		RX_FSCU_DATA_65525;
extern CAN_FSCU_DEMAND_65524*		TX_FSCU_DEMAND_DATA_65524;
//--, 221226 ysm, FSCU

//++, 230511 ysm, FSCU_HAC
extern unsigned char FSCU_HAC_OFF_Flag;
//--, 230511 ysm, FSCU_HAC

/*
#define C_DOUT_ENABLE1_Pin GPIO_PIN_13
#define C_DOUT_ENABLE1_GPIO_Port GPIOC
#define C_DOUT_RESET1_Pin GPIO_PIN_1
#define C_DOUT_RESET1_GPIO_Port GPIOC
#define C_DOUT_IN1_Pin GPIO_PIN_2
#define C_DOUT_IN1_GPIO_Port GPIOC
#define C_DOUT_IN2_Pin GPIO_PIN_3
#define C_DOUT_IN2_GPIO_Port GPIOC
#define C_DOUT_IN3_Pin GPIO_PIN_4
#define C_DOUT_IN3_GPIO_Port GPIOC
#define C_DOUT_IN4_Pin GPIO_PIN_5
#define C_DOUT_IN4_GPIO_Port GPIOC
#define C_DOUT_IN5_Pin GPIO_PIN_13
#define C_DOUT_IN5_GPIO_Port GPIOD
#define C_DOUT_IN6_Pin GPIO_PIN_14
#define C_DOUT_IN6_GPIO_Port GPIOD
#define C_DOUT_IN7_Pin GPIO_PIN_15
#define C_DOUT_IN7_GPIO_Port GPIOD
#define C_DOUT_RESET2_Pin GPIO_PIN_1
#define C_DOUT_RESET2_GPIO_Port GPIOB
#define C_DOUT_ENABLE2_Pin GPIO_PIN_7
#define C_DOUT_ENABLE2_GPIO_Port GPIOE
#define C_DOUT_SPI_MISO_Pin GPIO_PIN_12
#define C_DOUT_SPI_MISO_GPIO_Port GPIOE
*/
#define DOUT_CS1_LOW()     		HAL_GPIO_WritePin(C_DOUT_CS1_GPIO_Port, C_DOUT_CS1_Pin, GPIO_PIN_RESET)
#define DOUT_CS1_HIGH()    		HAL_GPIO_WritePin(C_DOUT_CS1_GPIO_Port, C_DOUT_CS1_Pin, GPIO_PIN_SET)

#define DOUT_CS2_LOW()     		HAL_GPIO_WritePin(C_DOUT_CS2_GPIO_Port, C_DOUT_CS2_Pin, GPIO_PIN_RESET)
#define DOUT_CS2_HIGH()    		HAL_GPIO_WritePin(C_DOUT_CS2_GPIO_Port, C_DOUT_CS2_Pin, GPIO_PIN_SET)

#define DOUT_SCK_LOW() 			HAL_GPIO_WritePin(C_DOUT_SPI_SCK_GPIO_Port, C_DOUT_SPI_SCK_Pin, GPIO_PIN_RESET)
#define DOUT_SCK_HIGH() 		HAL_GPIO_WritePin(C_DOUT_SPI_SCK_GPIO_Port, C_DOUT_SPI_SCK_Pin, GPIO_PIN_SET)

#define DOUT_MOSI_LOW()   		HAL_GPIO_WritePin(C_DOUT_SPI_MOSI_GPIO_Port, C_DOUT_SPI_MOSI_Pin, GPIO_PIN_RESET)
#define DOUT_MOSI_HIGH()   		HAL_GPIO_WritePin(C_DOUT_SPI_MOSI_GPIO_Port, C_DOUT_SPI_MOSI_Pin, GPIO_PIN_SET)


unsigned char SPI_DOUT_Transfer(unsigned char data, unsigned char size)
{
    unsigned char i = 0;
    unsigned char so_data = 0;
    
    for(i = 0; i < size; i++)
    {
        DOUT_SCK_LOW();
        DOUT_SCK_HIGH();
        so_data = (so_data<<1) | HAL_GPIO_ReadPin(C_DOUT_SPI_MISO_GPIO_Port, C_DOUT_SPI_MISO_Pin);	
        ((data & (1 << (size- i-1)))==(1 << (size- i-1))) ? DOUT_MOSI_HIGH():DOUT_MOSI_LOW();
        DOUT_SCK_LOW();	
    }
    
    return so_data;
}


void DOUT_Delay(void)
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



//R_W가 0이면 읽기 1이면 쓰기
unsigned char SPI_DOUT_Write(unsigned char port, unsigned char R_W, unsigned char address, unsigned char data)
{
	unsigned char parity = 0;
	unsigned char bit_sum = 0;
        unsigned char so_data = 0;

	for(unsigned char i = 0; i < 4; i++)
	    if((address>>i)&0x01 == 0x01)
	        bit_sum++;
        
	for(unsigned char i = 0; i < 8; i++)
          if((data>>i)&0x01 == 0x01)
            bit_sum++;
        
	parity = (bit_sum + R_W + 1)%2;
        
        for(unsigned char i = R_W; i < 2; i++)
        {
          if(i == 1)
            DOUT_Delay();
          //CS LOW로 해주기
          if(port == 1)
          {          	
            DOUT_CS1_LOW();
			DOUT_CS2_HIGH();
            
          }
          else
          {
          	
            DOUT_CS2_LOW();
			DOUT_CS1_HIGH();
           
          }

          SPI_DOUT_Transfer(R_W, 1);            //15번째 비트 Write(1)
          SPI_DOUT_Transfer(parity, 1);       //14번째 비트 Parity
          SPI_DOUT_Transfer(0, 2);            //13, 12번째 비트 0
          SPI_DOUT_Transfer(address, 4);       
          so_data = SPI_DOUT_Transfer(data, 8);   

          //CS HIGH로 해주기  
          if(port == 1)
            DOUT_CS1_HIGH();
          else
            DOUT_CS2_HIGH();
        }
        return so_data;
}

unsigned char testsetset[20];

void Init_Dout()
{
	//HAL_GPIO_WritePin(GPIOA, C_SERIAL_FLASH_CS_Pin, GPIO_PIN_SET); //SERIAL CS        
	
	HAL_GPIO_WritePin(C_DOUT_RESET1_GPIO_Port, C_DOUT_RESET1_Pin, GPIO_PIN_RESET); // DOUT RST       
	HAL_GPIO_WritePin(C_DOUT_RESET2_GPIO_Port, C_DOUT_RESET2_Pin, GPIO_PIN_RESET); // DOUT RST       
	HAL_GPIO_WritePin(C_DOUT_RESET1_GPIO_Port, C_DOUT_RESET1_Pin, GPIO_PIN_SET); // DOUT RST       
	HAL_GPIO_WritePin(C_DOUT_RESET2_GPIO_Port, C_DOUT_RESET2_Pin, GPIO_PIN_SET); // DOUT RST       

	DOUT_CS1_HIGH(); //SERIAL CS        
	DOUT_CS2_HIGH(); //SERIAL CS        
//	HAL_Delay(10);

	HAL_GPIO_WritePin(C_DOUT_ENABLE2_GPIO_Port, C_DOUT_ENABLE2_Pin, GPIO_PIN_SET);//DOUT ENABLE
//	HAL_GPIO_WritePin(C_DOUT_ENABLE1_GPIO_Port, C_DOUT_ENABLE1_Pin, GPIO_PIN_SET);//DOUT ENABLE	
	HAL_GPIO_WritePin(C_DOUT_ENABLE1_GPIO_Port, GPIO_PIN_12, GPIO_PIN_SET);//DOUT ENABLE //++,--, 211013 ysm


	HAL_GPIO_WritePin(C_DOUT_IN1_GPIO_Port, C_DOUT_IN1_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(C_DOUT_IN2_GPIO_Port, C_DOUT_IN2_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(C_DOUT_IN3_GPIO_Port, C_DOUT_IN3_Pin, GPIO_PIN_SET);
	// ++, 200323 bwk
	#if 0
	HAL_GPIO_WritePin(C_DOUT_IN4_GPIO_Port, C_DOUT_IN4_Pin, GPIO_PIN_SET);		
	#else
	if(DOUT_DATA.ANTI_RESTART_OUTPUT == 1)
		HAL_GPIO_WritePin(C_DOUT_IN4_GPIO_Port, C_DOUT_IN4_Pin, GPIO_PIN_SET);
	else
		HAL_GPIO_WritePin(C_DOUT_IN4_GPIO_Port, C_DOUT_IN4_Pin, GPIO_PIN_RESET);
	#endif
	// --, 200323 bkw
	HAL_GPIO_WritePin(C_DOUT_IN5_GPIO_Port, C_DOUT_IN5_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(C_DOUT_IN6_GPIO_Port, C_DOUT_IN6_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(C_DOUT_IN7_GPIO_Port, C_DOUT_IN7_Pin, GPIO_PIN_SET);

//	DOUT_CS1_LOW();  // DOUT CS
//	DOUT_CS2_LOW();  // DOUT CS
//	HAL_Delay(10);
	DOUT_SCK_LOW(); 

//	SPI_DOUT_Write(1,1,0x00,0xAA); 
	SPI_DOUT_Write(1,1,0x01,0x0F); 
	SPI_DOUT_Write(1,1,0x04,0xC0); 
	SPI_DOUT_Write(1,1,0x05,0xC0); 
	SPI_DOUT_Write(1,1,0x07,0x80);	          
 
//	SPI_DOUT_Write(2,1,0x00,0xAA); 
	SPI_DOUT_Write(2,1,0x01,0x0F); //0x0F 
	SPI_DOUT_Write(2,1,0x04,0xC0); 
	SPI_DOUT_Write(2,1,0x05,0xC0); 
	SPI_DOUT_Write(2,1,0x07,0x80); 
        
        
    #if 0    
    testsetset[0] = SPI_DOUT_Write(1,0,0,0x00); 
	testsetset[1] = SPI_DOUT_Write(1,0,1,0x00); 
	testsetset[2] = SPI_DOUT_Write(1,0,2,0x00); 
	testsetset[3] = SPI_DOUT_Write(1,0,3,0x00); 
	testsetset[4] = SPI_DOUT_Write(1,0,4,0x00); 
	testsetset[5] = SPI_DOUT_Write(1,0,5,0x00); 
	testsetset[6] = SPI_DOUT_Write(1,0,6,0x00); 
	testsetset[7] = SPI_DOUT_Write(1,0,7,0x00); 
	testsetset[8] = SPI_DOUT_Write(1,0,8,0x00); 
    testsetset[10] = SPI_DOUT_Write(2,0,0,0x00); 
	testsetset[11] = SPI_DOUT_Write(2,0,1,0x00); 
	testsetset[12] = SPI_DOUT_Write(2,0,2,0x00); 
	testsetset[13] = SPI_DOUT_Write(2,0,3,0x00); 
	testsetset[14] = SPI_DOUT_Write(2,0,4,0x00); 
	testsetset[15] = SPI_DOUT_Write(2,0,5,0x00); 
	testsetset[16] = SPI_DOUT_Write(2,0,6,0x00); 
	testsetset[17] = SPI_DOUT_Write(2,0,7,0x00); 
	testsetset[18] = SPI_DOUT_Write(2,0,8,0x00); 
        
	#endif
        
        
	DOUT_SCK_LOW(); 
	//HAL_Delay(10);
}
void Run_Dout2()
{
	 unsigned char temp_test_flag;

	 temp_test_flag = CAN_DATA_0xABAB_Flag;

	  if((temp_test_flag == 0x01) && (Tab_Number == 0x03))
	  {
		  HAL_GPIO_WritePin(C_DOUT_IN7_GPIO_Port, C_DOUT_IN7_Pin, (RX_JIG_DATA_65292.DO_7== 1)?GPIO_PIN_RESET:GPIO_PIN_SET);
		  HAL_GPIO_WritePin(C_DOUT_IN5_GPIO_Port, C_DOUT_IN5_Pin, (RX_JIG_DATA_65292.DO_WarningBuzzerSig== 1)?GPIO_PIN_RESET:GPIO_PIN_SET);


	  }
	  else
	  {
		  HAL_GPIO_WritePin(C_DOUT_IN5_GPIO_Port, C_DOUT_IN5_Pin, (DOUT_DATA.RELAY_SHIFT2_DOUT5 == 1)?GPIO_PIN_RESET:GPIO_PIN_SET);
		  HAL_GPIO_WritePin(C_DOUT_IN7_GPIO_Port, C_DOUT_IN7_Pin, (DOUT_DATA.RELAY_SHIFT_DOUT7 == 1)?GPIO_PIN_RESET:GPIO_PIN_SET);
	  }
	


}

void Run_Dout()
{
    unsigned char temp_CAN_DATA_0xABAB_Flag;


	//++, 230125 ysm, FSCU
	if((COUNT_FLAG.Flag_FSCUCommError == 1)||(COUNT_FLAG.Flag_FSCU_Invalid_Model == 1))
	{
		FSCU_ERROR_Travel_Cut_Relay = 1;
	}
	else if(COUNT_FLAG.Flag_FSCU_Error >= 1)
	{

		if(HCESPN.H3333[0] != 0)
		{
			FSCU_ERROR_Travel_Cut_Relay = 1;
		}
		else if(HCESPN.H3333[1] != 0)
		{
			if((HCESPN.H3333[1] == 0x10) || (HCESPN.H3333[1] == 0x20))
			{
				FSCU_ERROR_Travel_Cut_Relay = 0;
			}
			else
			{
				FSCU_ERROR_Travel_Cut_Relay = 1;
			}
		
			
		}

	}
	else
	{
		FSCU_ERROR_Travel_Cut_Relay = 0;
	}
	//--, 230125 ysm, FSCU
	
	if(COUNT_FLAG.Flag_FSCU_Enable == 1)
	{

		if(((RX_FSCU_DATA_65525.Travel_Cut_Relay_Demand == 1)&&(FSCU_HAC_OFF_Flag == 0))
			||(DCSR_Travel_Cut_Relay == 1)||(ZEROSTART_Travel_Cut_Relay == 1)
			||(FSCU_ERROR_Travel_Cut_Relay == 1)||(FLT_ERROR_Travel_Cut_Relay == 1) || (MSS_ESL_Flag == 1))
		{
			DOUT_DATA.TRAVEL_CUT_OUTPUT = 1;
		}
		else
		{

		
			DOUT_DATA.TRAVEL_CUT_OUTPUT = 0;
		}

		
	}
	else
	{
		if((OPSS_Travel_Cut_Relay == 1)||(DCSR_Travel_Cut_Relay == 1)||(ZEROSTART_Travel_Cut_Relay == 1)
			||(FLT_ERROR_Travel_Cut_Relay == 1)||(MSS_ESL_Flag == 1))
		{
			DOUT_DATA.TRAVEL_CUT_OUTPUT = 1;
		}
		else
		{
			DOUT_DATA.TRAVEL_CUT_OUTPUT = 0;
		}
	}
	
	if(COUNT_FLAG.Flag_FSCU_Enable == 1)
	{
		DOUT_DATA.ATTACH_CUT_OUTPUT = 0;
		
		if((AUTOLEVELING_Attach_Cut_Relay == 1)||(MSS_ESL_Flag == 1))
		{
			TX_FSCU_DEMAND_DATA_65524->Attach_Cut_SV_Demand = 1;
		}
		else
		{
			TX_FSCU_DEMAND_DATA_65524->Attach_Cut_SV_Demand = 0;
		}
	}
	else
	{
		if((OPSS_Attach_Cut_Relay == 1)||(AUTOLEVELING_Attach_Cut_Relay == 1)||(MSS_ESL_Flag == 1))
		{
			DOUT_DATA.ATTACH_CUT_OUTPUT= 1;
		}
		else
		{
			DOUT_DATA.ATTACH_CUT_OUTPUT = 0;
		}

	}


	if(COUNT_FLAG.Flag_FSCU_Enable == 1)
	{
		DOUT_DATA.AUTO_PARKING_OUTPUT = 0;
		
		if((APC_Parking_Relay == 1)||(HAC_Parking_Relay == 1))
		{
			TX_FSCU_DEMAND_DATA_65524->Parking_SV_Demand = 1;
		}
		else
		{
			TX_FSCU_DEMAND_DATA_65524->Parking_SV_Demand = 0;
		}
	}
	else
	{
		if((APC_Parking_Relay == 1)||(HAC_Parking_Relay == 1))
		{
			DOUT_DATA.AUTO_PARKING_OUTPUT= 1;
		}
		else
		{
			DOUT_DATA.AUTO_PARKING_OUTPUT = 0;
		}

	}
	//--, 221226 ysm, FSCU


    
    temp_CAN_DATA_0xABAB_Flag=CAN_DATA_0xABAB_Flag;
    if((temp_CAN_DATA_0xABAB_Flag == 0x01) && (Tab_Number == 0x03))
    {
            HAL_GPIO_WritePin(C_DOUT_IN1_GPIO_Port, C_DOUT_IN1_Pin, (RX_JIG_DATA_65292.DO_AutoParking== 1)?GPIO_PIN_RESET:GPIO_PIN_SET);
            HAL_GPIO_WritePin(C_DOUT_IN2_GPIO_Port, C_DOUT_IN2_Pin, (RX_JIG_DATA_65292.DO_TravelCut== 1)?GPIO_PIN_RESET:GPIO_PIN_SET);
            HAL_GPIO_WritePin(C_DOUT_IN3_GPIO_Port, C_DOUT_IN3_Pin, (RX_JIG_DATA_65292.DO_AttachCut== 1)?GPIO_PIN_RESET:GPIO_PIN_SET);
            HAL_GPIO_WritePin(C_DOUT_IN4_GPIO_Port, C_DOUT_IN4_Pin, (RX_JIG_DATA_65292.DO_AntiRestart== 1)?GPIO_PIN_RESET:GPIO_PIN_SET);
            HAL_GPIO_WritePin(C_DOUT_IN6_GPIO_Port, C_DOUT_IN6_Pin, (RX_JIG_DATA_65292.DO_HydAutoRegenration == 1)?GPIO_PIN_RESET:GPIO_PIN_SET);
           
    }
    else
    {
            HAL_GPIO_WritePin(C_DOUT_IN1_GPIO_Port, C_DOUT_IN1_Pin, (DOUT_DATA.AUTO_PARKING_OUTPUT == 1)?GPIO_PIN_RESET:GPIO_PIN_SET);
            HAL_GPIO_WritePin(C_DOUT_IN2_GPIO_Port, C_DOUT_IN2_Pin, (DOUT_DATA.TRAVEL_CUT_OUTPUT == 1)?GPIO_PIN_RESET:GPIO_PIN_SET);
            HAL_GPIO_WritePin(C_DOUT_IN3_GPIO_Port, C_DOUT_IN3_Pin, (DOUT_DATA.ATTACH_CUT_OUTPUT == 1)?GPIO_PIN_RESET:GPIO_PIN_SET);
            HAL_GPIO_WritePin(C_DOUT_IN4_GPIO_Port, C_DOUT_IN4_Pin, (DOUT_DATA.ANTI_RESTART_OUTPUT == 1)?GPIO_PIN_RESET:GPIO_PIN_SET);
           // HAL_GPIO_WritePin(C_DOUT_IN5_GPIO_Port, C_DOUT_IN5_Pin, (DOUT_DATA.RELAY_SHIFT2_DOUT5 == 1)?GPIO_PIN_RESET:GPIO_PIN_SET);
            HAL_GPIO_WritePin(C_DOUT_IN6_GPIO_Port, C_DOUT_IN6_Pin, (DOUT_DATA.HYD_AUTO_REGENERATION == 1)?GPIO_PIN_RESET:GPIO_PIN_SET);
           // HAL_GPIO_WritePin(C_DOUT_IN7_GPIO_Port, C_DOUT_IN7_Pin, (DOUT_DATA.RELAY_SHIFT_DOUT7 == 1)?GPIO_PIN_RESET:GPIO_PIN_SET);
	}        

}

void Initialize_DOUT(void)
{

	//DOUT_DATA.ANTI_RESTART_OUTPUT=0;
	Init_Dout();
	
	for(int i=0;i<10;i++)
		Check_ADC_Value();
	
	memset(&DOUT_DATA,0x00,sizeof(DOUT_DATA));
	
	DOUT_DATA.RELAY_SHIFT_DOUT7 = 1;

	//Init_Dout();
	Run_Dout();

}

unsigned char diag1;
unsigned char diag2;
void Diag_Dout(void)
{
#if 0
	/*
	diag1
	Bit  7   6   5   4    3  2   1  0
	     Z   Z   F   OT   DIA6   DIA5
	diag2
	Bit  7  6   5  4   3  2   1  0
	     DIA4   DIA3   DIA2   DIA1

	F, OT = 1 : No Fault
	F, OT = 0 : Fault, Over-temperature

	DIAx
	1  1 : Power stage o.k.
	1  0 : Overload, short circuit to battery (SCB) or over-temperature (OT)
	0  1 : Open load (OL)
	0  0 : Short circuit to ground (SCG)
	*/
	HAL_GPIO_WritePin(GPIOA, C_SERIAL_FLASH_CS_Pin, GPIO_PIN_SET); //SERIAL CS     
	HAL_GPIO_WritePin(GPIOD, C_DOUT_CS_Pin, GPIO_PIN_RESET); //DOUT CS

	diag1 = SPI_Transfer(0x80); //1000 0000
	diag2 = SPI_Transfer(0xFF);

	HAL_GPIO_WritePin(GPIOD, C_DOUT_CS_Pin, GPIO_PIN_SET); //DOUT CS
	HAL_GPIO_WritePin(GPIOA, C_SERIAL_FLASH_CS_Pin, GPIO_PIN_RESET); //SERIAL CS     
#endif
}
