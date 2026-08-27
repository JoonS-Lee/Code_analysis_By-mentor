#include "FLV_Cluster_APP.h"

unsigned char PowerIGState;
unsigned short PowerIGCNT;
extern unsigned char System_Init_flag;
unsigned char Die_cnt;
unsigned char Die_cnt2;

unsigned char Power_Reset;

extern UCHAR SystemReset;
//extern IWDG_HandleTypeDef IwdgHandle;

extern unsigned char Flag_Engine_Start;

//	Software_Reset
#define BootLoaderAddress       0x0000000
#define ApplicationAddress      0x08020000

pFunction Jump_To_Application_Cluster_App;
uint32_t JumpAddress_Cluster_App;

pFunction Jump_To_Application_Cluster_Iap;
uint32_t JumpAddress_Cluster_Iap;

//++, 210821 ysm, 9S
unsigned short Parking_Buzzer_Count;
unsigned short Parking_Buzzer_Count2;

unsigned char Parking_Buzzer_flag;
extern EEPROM_MODEL_DATA1 InfoModel1;
extern st_BUZZER BUZZER_FLAG;
//--, 210821 ysm, 9S


extern ADC_HandleTypeDef hadc1;

extern CAN_HandleTypeDef hcan1;
extern CAN_HandleTypeDef hcan2;

extern TIM_HandleTypeDef htim1;
extern TIM_HandleTypeDef htim3;
extern TIM_HandleTypeDef htim6;

extern EEPROM_MODEL_DATA2	InfoModel2;
extern unsigned char			Save_ESL_Interval;

extern unsigned char Daily_Report_Send_flag_PowerOff;


void Software_Reset_Application(void)
{ 	
	/* Jump to user application */
	JumpAddress_Cluster_App = *(__IO uint32_t*) (ApplicationAddress + 4);
	Jump_To_Application_Cluster_App = (pFunction) JumpAddress_Cluster_App;
	/* Initialize user application's Stack Pointer */
	__set_MSP(*(__IO uint32_t*) ApplicationAddress);
	Jump_To_Application_Cluster_App();
}	

void Software_Reset_BootLoader(void)
{ 	
	/* Jump to user application */
	JumpAddress_Cluster_Iap = *(__IO uint32_t*) (BootLoaderAddress + 4);
	Jump_To_Application_Cluster_Iap = (pFunction) JumpAddress_Cluster_Iap;
	/* Initialize user application's Stack Pointer */
	__set_MSP(*(__IO uint32_t*) BootLoaderAddress);
	Jump_To_Application_Cluster_Iap();
}	


extern CAN_HandleTypeDef CanHandle;

void Initialize_Power_Variable(void)
{
	PowerIGState=PowerIGCNT=0;
	System_Init_flag=Die_cnt=Die_cnt2=0;
	Power_Reset=0;

	SystemReset = 0;
	//++, 210821 ysm, 9S
	Parking_Buzzer_Count = Parking_Buzzer_Count2 = 0;
	Parking_Buzzer_flag = 0;
	//--, 210821 ysm, 9S
}

void SystemPowerOff(void)
{
#if 0
	//POWER OFF 일 때, 시리얼과 캔 통신 부분을 죽여놔야지, 정상적으로 RESET 동작을 수행
	//CAN_ITConfig(CAN_IT_FMP0, DISABLE);
	// watchdog off
	//	IWDG_SetReload(150*10);
	//	IWDG_ReloadCounter();

	C_ILLM_DISABLE();
	C_LAMP_CLR_ENABLE();
	C_KEY_LED_DISABLE();

#endif
	unsigned char tempchar[2];

	if(ScreenIndex != SCREEN_STATE_MSS_TOP)
	{
		SetScreenIndex(SCREEN_STATE_POWEROFF);
		DisplayUpdate();
	}	
	//++, 220929 ysm, FSCU
	Save_FSCU_Status();		
	//--, 220929 ysm, FSCU
	
	Save_hourmeter();
	Save_odometer();
	Save_maintenance();
	Diagnosis_End();

	SatelliteCommEnd();
	SatelliteCommEnd3();


	#if 0 
	if(InfoModel2.ESL_Set_Status==ACTIVE) // always ESL set 
	{
		tempchar[0] = Checked_ESL;
		if(Save_ESL_Interval==1)
		{
			tempchar[0]=1;
			Save_ESL_Interval=0;
		}
		EepromWrite(ADDRESS_CHECKED_ESL, (unsigned char*)(&tempchar[0]), 1);
		Set_ESL_Time(0xff); // always ESL Not 
	}
	else if(InfoModel2.ESL_Set_Status==DELAYTIME) // 지정 시간 후 동작 
	#else
	if(InfoModel2.ESL_Set_Status==DELAYTIME) // 지정 시간 후 동작 
	#endif
	{
		#if 0
		if((Save_ESL_Interval==1) ||(Flag_Engine_Start==1))
		{
			EepromRead(ADDRESS_ESL_INTERVAL, (unsigned char*)(&tempchar[0]), 1);	
			Set_ESL_Time(tempchar[0]); 
			Save_ESL_Interval=0;
		}
		#else
		EepromRead(ADDRESS_ESL_INTERVAL, (unsigned char*)(&tempchar[0]), 1);	
		Set_ESL_Time(tempchar[0]); 
		Save_ESL_Interval=0;
		#endif
	}
	//++, 210821 ysm, 9S	
	HAL_GPIO_WritePin(GPIOE, GPIO_PIN_0, GPIO_PIN_RESET);   //C_LAMP_EN
	
	if((InfoModel1.ModelInfo >= MODEL_35D_9S)&&(InfoModel1.ModelInfo <= MODEL_50D_9S))		
	{
		while(1)
		{

			if(Parking_Buzzer_Count%100 == 0)
			{
				IWDG->KR = 0xAAAA;
			}
			
			if(Parking_Buzzer_Count >= 3000)
			{		
				Parking_Buzzer_Count = 3000;
				break;
			}

			if(Flag_DIN[INDEX_PARKING_PRESSURE_SW] == 1)
			{
				Parking_Buzzer_flag = 2;
				Parking_Buzzer_Count2 = 0;
				
			}
			else
			{
				
				Parking_Buzzer_flag = 1;
			}

			if (System_GetPowerIG() == POWER_IGMON_ON || SystemReset == 1) 	
			{	
				break;
			}	
			__NOP();

		}

	}

	HAL_GPIO_WritePin(C_BUZZER_ON_GPIO_Port, C_BUZZER_ON_Pin, GPIO_PIN_RESET);
	//--, 210821 ysm, 9S	

	HAL_CAN_DeInit(&hcan1);
	HAL_CAN_DeInit(&hcan2);
	HAL_ADC_DeInit(&hadc1);
	HAL_TIM_IC_DeInit(&htim1);
	HAL_TIM_Base_DeInit(&htim3);
	HAL_TIM_PWM_DeInit(&htim3);
	HAL_TIM_Base_DeInit(&htim6);

	if (System_GetPowerIG() == POWER_IGMON_ON || SystemReset == 1) 	
	{	
		Software_Reset_Application();
	}	
	
	System_PowerIG(POWERIG_OFF);
	System_PowerIG(POWERIG_OFF);
	System_PowerIG(POWERIG_OFF);

	TimeDelay_msec(300);
	while (1)
	{
		__NOP();
		
		if (System_GetPowerIG() == POWER_IGMON_ON) 	
		{				
			Software_Reset_Application();
		}	
	}
}	


void PowerSystem(void)
{
	if (System_GetPowerIG() == 1)		SystemPowerOff();
	else 						System_PowerIG(POWERIG_ON);
}

unsigned char System_GetPowerIG(void)
{
	return HAL_GPIO_ReadPin(POWER_MON_PORT, C_POWER_MON_Pin);
}


void System_PowerIG(unsigned char BitData)
{
	if(BitData==0)
	{
		HAL_GPIO_WritePin(POWER_ON_PORT, C_POWER_ON_Pin, GPIO_PIN_RESET);
	}
	else
	{
		HAL_GPIO_WritePin(POWER_ON_PORT, C_POWER_ON_Pin, GPIO_PIN_SET);
	}
}

void System_CheckPowerIG_SystemReset(void)
{
	if (System_GetPowerIG() == POWER_IGMON_ON) 	
	{	
		SystemReset = 1;
	}
}

//	PowerOff 수정. 
void TimeDelay_msec(uint32_t nCount)
{
	uint32_t i, j;

#if 0
	for (i = 0; i < nCount; i++)
		for (j = 0; j < 5000; j++);
#else
	for (i = 0; i < nCount; i++)
		for (j = 0; j < 17000; j++);
#endif
}

void System_CheckPowerIG()
{
	//	100msec KeyIG On 상태 체크 
	if (System_GetPowerIG() == POWER_IGMON_ON)		
	{ 
		System_PowerIG(POWERIG_ON);

		SystemReset = 0;
	}
	else
	{
		Die_cnt = 0;
		Die_cnt2 = 0;

		while (1)
		{
			COUNT_FLAG.Comm_Error_FSCU = 0; //++,--, 230118 ysm, FSCU_BUG_FIX
			//	100msec KeyIG On 상태 체크 
			if (System_GetPowerIG() == POWER_IGMON_ON)		
			{ 
				//	POWER OFF 일 때, 시리얼과 캔 통신 부분을 죽여놔야지, 정상적으로 RESET 동작을 수행
				//CAN_ITConfig(CAN_IT_FMP0, DISABLE);    //  __HAL_CAN_DISABLE_IT(CanHandle, CAN_IT_FMP0);

				System_PowerIG(POWERIG_ON);

				SystemReset = 0;
				return;
			}
						
			HAL_Delay(100);

			Die_cnt++;
			if (Die_cnt >= 20)	//	100msec 마다 10번 체크. 
			{
				Die_cnt = 30;
				SatelliteCommRun_Poweroff();

				if(Daily_Report_Send_flag_PowerOff == 1)
				{
					if(++Die_cnt2 > 30)
					{
						Die_cnt2 = 50;
						break;
					}

				}
				else
				{
					break;
				}
			}
				
		}

		SystemPowerOff();

	}
}
