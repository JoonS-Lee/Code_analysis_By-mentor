#include "FLV_Cluster_APP.h"

unsigned char InitECU_Comm_error = 0;
unsigned char InitFLT_error = 0; //++,--, 220616 FLT_ERR

extern st_COUNT_FLAG 	COUNT_FLAG;
extern EEPROM_MODEL_DATA1 InfoModel1;


//++, 221226 ysm, FSCU
unsigned char Error_Popup_Step = 1;

unsigned char Error_POPUP_buf[6]; //++,--, 230524 ysm, 5->6 , FSCU_HAC
unsigned char Error_POPUP_Buf_Old[6]; //++,--, 230524 ysm, 5->6 , FSCU_HAC
//--, 221226 ysm, FSCU

void DisplayMain()
{
	//++, 221226 ysm, FSCU
	unsigned char error_count;
	unsigned char i;
	//--, 221226 ysm, FSCU
	CheckClutchProtection();	

	if((InfoModel1.ModelInfo >= MODEL_35D_9S)&&(InfoModel1.ModelInfo <= MODEL_50D_9S))
	{
		// 9S 모델은 ECU와 CAN 통신 하지 않음
		COUNT_FLAG.Flag_ECU_Comm_error = 0;
	}
	
	//++, 221226 ysm, FSCU

	error_count = 0;
	

	if(COUNT_FLAG.Flag_ECU_Comm_error == 1)  // ECU COMM
	{
		error_count++;
		Error_POPUP_buf[0] = error_count;
		
	}
	else
		Error_POPUP_buf[0] = 0;


	if(COUNT_FLAG.Flag_FLTCommError == 1)    // FLT COMM
	{
		error_count++;
		Error_POPUP_buf[1] = error_count;
		
	}
	else
		Error_POPUP_buf[1] = 0;

	
	if(COUNT_FLAG.Flag_FSCUCommError == 1) // FSCU COMM
	{
		error_count++;
		Error_POPUP_buf[2] = error_count;
		
	}
	else
		Error_POPUP_buf[2] = 0;


	if(COUNT_FLAG.Flag_FLT_Error >= 1)     // FLT ERROR
	{
		error_count++;
		Error_POPUP_buf[3] = error_count;
		
	}
	else
		Error_POPUP_buf[3] = 0;


	if(COUNT_FLAG.Flag_FSCU_Error >= 1)    // FSCU ERROR
	{
		error_count++;
		Error_POPUP_buf[4] = error_count;
		
	}
	else
		Error_POPUP_buf[4] = 0;

	//++, 230616 ysm, FSCU_HAC
	if(COUNT_FLAG.Flag_FSCU_Invalid_Model == 1)    // FSCU Invalid Model
	{
		error_count++;
		Error_POPUP_buf[5] = error_count;
		
	}
	else
		Error_POPUP_buf[5] = 0;
	//--, 230616 ysm, FSCU_HAC

	if((Error_POPUP_buf[0]!=0)||(Error_POPUP_buf[1]!=0)||(Error_POPUP_buf[2]!=0)||(Error_POPUP_buf[3]!=0)||(Error_POPUP_buf[4]!=0)||(Error_POPUP_buf[5]!=0))
	{

		if(InitECU_Comm_error == 0)
			InitECU_Comm_error = 1;

		for(i=0; i<6; i++)
		{
			if((Error_POPUP_Buf_Old[i] == 0)&&(Error_POPUP_buf[i] != 0))
			{
				if(InitECU_Comm_error == 2)
				{
					InitECU_Comm_error = 1;
				}
			}
			Error_POPUP_Buf_Old[i] = Error_POPUP_buf[i];
			
		}

		for(i=1; i<7; i++)
		{
			if(Error_POPUP_buf[i-1] != 0)
			{
				if(Error_Popup_Step == Error_POPUP_buf[i-1])
					break;

			}

		}

		if(InitECU_Comm_error == 1)
		{
			if(i == 1)
			{
				SetScreenIndex(SCREEN_STATE_MAIN_COMM_ERROR);
			}
			else if(i == 2)
			{
				SetScreenIndex(SCREEN_STATE_MAIN_FLT_COMM_ERROR);
			}
			else if(i == 3)
			{
				SetScreenIndex(SCREEN_STATE_MAIN_FSCU_COMM_ERROR);
			}
			else if(i == 4)
			{
				SetScreenIndex(SCREEN_STATE_MAIN_FLT_ERROR);
			}
			else if(i == 5)
			{
				SetScreenIndex(SCREEN_STATE_MAIN_FSCU_ERROR);
			}
			else if(i == 6)
			{
				SetScreenIndex(SCREEN_STATE_MAIN_FSCU_INVALID_MODEL); //++,--, 230616 ysm, FSCU_HAC
			}

			COUNT_FLAG.Count_Error_Popup++;
			if(COUNT_FLAG.Count_Error_Popup > 15)
			{
				COUNT_FLAG.Count_Error_Popup = 0;
				Error_Popup_Step++;

				if(Error_Popup_Step > error_count)
				{
					Error_Popup_Step = 1;
				}

			}
		}
		else
		{
			COUNT_FLAG.Count_Error_Popup = 0;
			Error_Popup_Step = 1;
		}
	}
	else
	{
		InitECU_Comm_error = 1;
		COUNT_FLAG.Count_Error_Popup = 0;
		Error_Popup_Step = 1;

	}
	//--, 221226 ysm, FSCU	

	if((OldScreenIndex != ScreenIndex) && ((OldScreenIndex & 0xF0000000) != SCREEN_STATE_MAIN_TOP))
	{
		DisplayBackground();
		DisplayMainGaugeBackground();
		DisplayMainCenterBackground();
		DisplayMainTopBackground();	
		DisplayMainBottomBackground();
		if(ScreenIndex!=SCREEN_STATE_ESL_PASSWORD)		OldScreenIndex = ScreenIndex;
	}
	else
	{
		DisplayMainGauge();
		DisplayMainCenter();
	}
}
