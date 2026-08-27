#include "FLV_Cluster_APP.h"

UCHAR CurNumber;
UCHAR OldCurNumber;
UCHAR BufNumber[10];
UCHAR BufNumber_change[10];
UCHAR MaxNumber;
UCHAR strLength;
UCHAR strMaxLength;
UCHAR InputType;
UCHAR bInputDot;
UCHAR DotCursur;
UCHAR UnitWidth;

UCHAR Password_Check_Result;
PASSWORD_DATA	InfoPassword;

UCHAR ArrNumberDot_num[]={1,2,3,4,5,'.',6,7,8,9,0};
UCHAR ArrNumber_num[]={1,2,3,4,5,6,7,8,9,0};

extern st_AD_DATA 		AD_DATA;
extern unsigned char		Equipment_Calibration;

extern EEPROM_MODEL_DATA2	InfoModel2;

void DrawNumberButton(uint32_t Xpos, uint32_t Ypos, UCHAR Select, UCHAR Number)
{
	USHORT covbuf[2]={0x00,};
	if((InputType == NUMBER_INFO_ENTER) || ((InputType == NUMBER_INFO_DOT) && (bInputDot == 1)))
		covbuf[0] = 0x30 + ArrNumber_num[Number];
	else
	{
		if(ArrNumberDot_num[Number] == '.')
			covbuf[0] = ArrNumberDot_num[Number];
		else
			covbuf[0] = 0x30 + ArrNumberDot_num[Number];
	}

	if(Select == CURSER_ON)
		LCD_DrawEdge_FillColor(Xpos, Ypos, 31, 39, 2, COLOR_WHITE, COLOR_MENU_GRAY);
	else
		LCD_Draw_Color(Xpos, Ypos, 31, 39, COLOR_MENU_GRAY);

	GUI_DisplayCenterStringAt(&covbuf[0], Xpos, Ypos+10,  31, COLOR_WHITE, FONTSIZE);		
	
}


void ReDrawNumberButton(uint32_t Xpos, uint32_t Ypos, UCHAR Select, UCHAR Number)
{
	LCD_DrawEdge_Color(Xpos, Ypos, 31, 39, 1, (Select == CURSER_ON)?COLOR_WHITE:COLOR_MENU_GRAY);
}


void ReDrawNumber()
{
	USHORT x_POS[]={6,40,74,108,142,176};
	USHORT y_POS[]={90,132};

	USHORT temp_x, temp_y;

	if(OldCurNumber < MaxNumber)
	{
		if(((InputType == NUMBER_INFO_ENTER)) && (OldCurNumber == (MaxNumber-1)))
		{
			LCD_DrawEdge_Color(x_POS[5], y_POS[0], 30, 80, 2, COLOR_MENU_GRAY);
		}
		else
		{
			if(OldCurNumber<(MaxNumber/2))
			{
				temp_x = x_POS[OldCurNumber];					temp_y = y_POS[0];
			}
			else
			{
				temp_x = x_POS[OldCurNumber-(MaxNumber/2)];	temp_y = y_POS[1];
			}
			if(OldCurNumber == (MaxNumber-1))
			{
				LCD_DrawEdge_Color(temp_x, temp_y, 31, 39, 2, COLOR_MENU_GRAY);
			}
			else
			{
				LCD_DrawEdge_Color(temp_x, temp_y, 31, 39, 2, COLOR_MENU_GRAY);
			}
		}
	}

	if(CurNumber != MaxNumber)
	{
		if(((InputType == NUMBER_INFO_ENTER)) && (CurNumber == (MaxNumber-1)))
		{
			LCD_DrawEdge_Color(x_POS[5], y_POS[0], 30, 80, 2, COLOR_WHITE);
		}
		else
		{
			if(CurNumber<(MaxNumber/2))
			{
				temp_x = x_POS[CurNumber];		temp_y = y_POS[0];
			}
			else
			{
				temp_x = x_POS[CurNumber-(MaxNumber/2)];		temp_y = y_POS[1];
			}
			if(CurNumber == (MaxNumber-1))
			{
				LCD_DrawEdge_Color(temp_x, temp_y, 31, 39, 2, COLOR_WHITE);
			}
			else
			{
				DrawNumberButton(temp_x, temp_y , CURSER_ON, CurNumber);
				LCD_DrawEdge_Color(temp_x, temp_y, 31, 39, 2, COLOR_WHITE);
			}
		}
	}
}

void InitializeArrNumber()
{
	if(InputType == NUMBER_INFO_ENTER) 
	{
		MaxNumber = 11;
	}
	else if(InputType == NUMBER_INFO_DOT)
	{
		if((bInputDot == 1) 
				|| ((strLength == 3) && (ScreenIndex == SCREEN_STATE_MENU_EQUIPMENT_WEIGHTSENSOR_CROSSSECTION_INPUT))
			|| ((strLength == 2) && (ScreenIndex == SCREEN_STATE_MENU_EQUIPMENT_WEIGHTSENSOR_LOADADJUST_LOAD_INPUT)))
		{
			if(((ScreenIndex == SCREEN_STATE_MENU_EQUIPMENT_WEIGHTSENSOR_LOADADJUST_LOAD_INPUT)
				&& (bInputDot == 1) && (DotCursur == 2))
				||((ScreenIndex == SCREEN_STATE_MENU_EQUIPMENT_WEIGHTSENSOR_CROSSSECTION_INPUT)
				&& (bInputDot == 1) && (DotCursur == 3)))
			{
				strMaxLength = strMaxLength - 1;
			}
			else if((ScreenIndex == SCREEN_STATE_MENU_EQUIPMENT_WEIGHTSENSOR_CROSSSECTION_INPUT)
				&& (bInputDot == 1) && (DotCursur == 2))
			{
				strMaxLength = strMaxLength - 2;
			}
			
			if(CurNumber == 5)
			{
				OldCurNumber = 5;
			
				CurNumber = 0;
				ReDrawNumber();
			}
			else if(CurNumber > 5)
			{
				CurNumber = CurNumber-1;
			}

			MaxNumber = 11;

		}
		else
		{
			MaxNumber = 12;
		}

	}
}

void DisplayNumber()
{
	USHORT x_POS[]={6,40,74,108,142,176};
	USHORT y_POS[]={90,132};
	
	DrawNumberButton(x_POS[0], y_POS[0], CURSER_ON, 0);
	
	for(UCHAR i=1;i<MaxNumber;i++)
	{
		if(i==MaxNumber-1)
		{
			LCD_Draw_Color(x_POS[5], y_POS[0], 30, 80,COLOR_MENU_GRAY);
			PCXtoBMP_16bit(x_POS[5]+5, y_POS[0]+30, 22, 20, FL_Image.password_enter_nor);
		}
		else
		{
			if(i<5)
				DrawNumberButton(x_POS[i], y_POS[0], CURSER_OFF, i);
			else
				DrawNumberButton(x_POS[i-5], y_POS[1], CURSER_OFF, i);
		}
	}
}

void DisplayNumberDot()
{
	USHORT x_POS[]={6,40,74,108,142,176};
	USHORT y_POS[]={90,132};
	
	DrawNumberButton(x_POS[0], y_POS[0], CURSER_ON, 0);
	
	for(UCHAR i=1;i<MaxNumber;i++)
	{
		if(i==MaxNumber-1)
		{
			LCD_Draw_Color(x_POS[5], y_POS[1], 31, 39, COLOR_MENU_GRAY);
			PCXtoBMP_16bit(x_POS[5]+4, y_POS[1]+10, 22, 20, FL_Image.password_enter_nor);
		}
		else
		{
			if(i<6)
				DrawNumberButton(x_POS[i], y_POS[0], 0, i);
			else
				DrawNumberButton(x_POS[i-6], y_POS[1], 0, i);
		}
	}
}

void DisplayInputEditBox()
{
	UCHAR buf[15];
	USHORT covbuf[15];
	USHORT OldWeightTon;
	
	memset((&buf), 0x0, sizeof( buf));
	memset((&covbuf), 0x0, sizeof( covbuf));

	UnitWidth = 0;
	strLength = 0;
	memset(&BufNumber[0], 0xFF, sizeof(BufNumber));

	LCD_DrawEdge_FillColor(6, 48, 200, 38, 3, COLOR_WHITE, COLOR_BLACK);

	if(InputType == NUMBER_INFO_DOT)
	{
		switch(ScreenIndex)
		{
			case SCREEN_STATE_MENU_EQUIPMENT_WEIGHTSENSOR_CROSSSECTION_INPUT:
				sprintf ( (char*)buf, "%.2f",AD_DATA.C_A);
				break;
			case SCREEN_STATE_MENU_EQUIPMENT_WEIGHTSENSOR_LOADADJUST_LOAD_INPUT:
				OldWeightTon = AD_DATA.C_Max_Ton;
				if(OldWeightTon == 0 || OldWeightTon == 0xffff)
				{
					AD_DATA.C_Max_Ton= 0;
					sprintf ( (char*)buf, "0.0");
				}
				else
				{
					sprintf ( (char*)buf, "%d.%d",OldWeightTon/10, OldWeightTon%10) ; 
				}
				break;
		}

		String_Func(covbuf, buf);
		if(ScreenIndex == SCREEN_STATE_MENU_EQUIPMENT_WEIGHTSENSOR_CROSSSECTION_INPUT)
		{		
			GUI_DisplayRightStringAt(StringEtc[SIGN_CM2], 12, 55, 186, COLOR_WHITE, FONTSIZE);	
			UnitWidth = GetStringLen(StringEtc[SIGN_CM2], gui_font23);
		}
		else
		{
			if(InfoDisplaySetting.WeightUnit == UNIT_WEIGHT_LB)
			{
				GUI_DisplayRightStringAt(StringEtc[SIGN_LB], 12, 55, 186, COLOR_WHITE, FONTSIZE);	
				UnitWidth = GetStringLen(StringEtc[SIGN_LB], gui_font23);
			}
			else
			{
				GUI_DisplayRightStringAt(StringEtc[SIGN_TON], 12, 55, 186, COLOR_WHITE, FONTSIZE);	
				UnitWidth = GetStringLen(StringEtc[SIGN_TON], gui_font23);
			}
		}
		DisplayStringAt(&covbuf[0], 12, 55, gui_font23, COLOR_WHITE);	
	}
}

void DisplayNumberInput()
{
	UCHAR i;
	USHORT covbuf[30];
	UCHAR displaystart=0;

	switch(ScreenIndex)
	{
		case SCREEN_STATE_ESL_PASSWORD:
		case SCREEN_STATE_MENU_EQUIPMENT_PW:
		case SCREEN_STATE_MENU_MAINTENANCE_PW:
		case SCREEN_STATE_MENU_MAINTENANCE_USERPWCHANGE_CURRENT:	
		case SCREEN_STATE_MENU_DISPLAYSETTING_ESLPWCHANGE_CURRENT:
			displaystart = 1;
			break;
		case SCREEN_STATE_MENU_EQUIPMENT_WEIGHTSENSOR_CROSSSECTION_INPUT:
		case SCREEN_STATE_MENU_EQUIPMENT_WEIGHTSENSOR_LOADADJUST_LOAD_INPUT:
		case SCREEN_STATE_MENU_MAINTENANCE_USERPWCHANGE_NEW:	
		case SCREEN_STATE_MENU_MAINTENANCE_USERPWCHANGE_RETYPE:	
		case SCREEN_STATE_MENU_DISPLAYSETTING_ESLPWCHANGE_NEW:
		case SCREEN_STATE_MENU_DISPLAYSETTING_ESLPWCHANGE_RETYPE:
			displaystart = 0;
			break;
	}	
	LCD_Draw_Color(12, 55, 186-UnitWidth , FONTSIZE, COLOR_BLACK);
	if(displaystart == 1)
	{
		for(i = 0 ; i < strLength ; i++)   
	        {                           
			PCXtoBMP_16bit(12+(i*17), 55, 17, 23, FL_Image.password_star);
	        }                          
	}
	else
	{
		memset((USHORT *)(&covbuf), 0x0, sizeof( covbuf));

		for(i = 0 ; i < strLength ; i++)   
	        {       
	        	if(BufNumber[i] == 0x2E)
				covbuf[i] = BufNumber[i];
			else
				covbuf[i] = 0x30 + BufNumber[i];                   
	        }                          
	        covbuf[i+1] = 0;       

		DisplayStringAt(&covbuf[0], 12,55, gui_font23,  COLOR_WHITE);	
	}

}

void DisplayRetypeNumber()
{
	DisplayInputEditBox();
	OldCurNumber = CurNumber;
	CurNumber = 0;
	ReDrawNumber();
}

//////////////////////////////////////////////////////////////////////////////
USHORT GetNumberInteger()
{
	UCHAR NumLength ;
	
	if(bInputDot == 0)
		NumLength = strLength;
	else
		NumLength = DotCursur-1;
	
	switch(NumLength)
	{
		case 1:
			return  BufNumber[0];
		case 2:
			return  (BufNumber[0])*10 + (BufNumber[1]);
		case 3:
			return (BufNumber[0])*100 + (BufNumber[1])*10 + (BufNumber[2]);
		default:
			return 0;
	}
}

USHORT GetNumberPoint()
{
	UCHAR NumLength;
	
	if(bInputDot == 0)
		return 0;
	else
	{
		NumLength = strLength - DotCursur;
		if(NumLength == 1)
			return BufNumber[strLength-1];
		else if(NumLength == 2)
			return (BufNumber[strLength-2])*10 + (BufNumber[strLength-1]);
	}
	return 0;
}

void GetPassword()
{
	InfoPassword.Data1 = BufNumber[0];
	InfoPassword.Data2 = BufNumber[1];
	InfoPassword.Data3 = BufNumber[2];
	InfoPassword.Data4 = BufNumber[3];
	InfoPassword.Data5 = BufNumber[4];
	InfoPassword.Data6 = BufNumber[5];
	InfoPassword.Data7 = BufNumber[6];
	InfoPassword.Data8 = BufNumber[7];
	InfoPassword.Data9 = BufNumber[8];
	InfoPassword.Data10 = BufNumber[9];	
}
//////////////////////////////////////////////////////////////////////////////
void LeftKeyEventNumberInput()
{
	if(strLength < strMaxLength)
	{
		OldCurNumber = CurNumber;
		if(CurNumber > 0)
			CurNumber--;
		else
			CurNumber = MaxNumber-1;
		
		ReDrawNumber();
	}
}

void RightKeyEventNumberInput()
{
	if(strLength < strMaxLength)
	{
		OldCurNumber = CurNumber;

		if(CurNumber < MaxNumber-1)
			CurNumber++;
		else
			CurNumber = 0;
		
		ReDrawNumber();
	}
}

void ESCKeyEventNumberInput()
{
	if(strLength == 0)
	{
		switch(ScreenIndex)
		{
			case SCREEN_STATE_MENU_EQUIPMENT_PW:
				SetScreenIndex(SCREEN_STATE_MENU_EQUIPMENT_TOP);
				break;
			case SCREEN_STATE_MENU_EQUIPMENT_WEIGHTSENSOR_CROSSSECTION_INPUT:
				SetScreenIndex(SCREEN_STATE_MENU_EQUIPMENT_WEIGHTSENSOR_CROSSSECTION_TOP);
				break;
			case SCREEN_STATE_MENU_EQUIPMENT_WEIGHTSENSOR_LOADADJUST_LOAD_INPUT:
				SetScreenIndex(SCREEN_STATE_MENU_EQUIPMENT_WEIGHTSENSOR_LOADADJUST_LOAD_TOP);
				break;
			case SCREEN_STATE_MENU_MAINTENANCE_PW:
				SetScreenIndex(SCREEN_STATE_MENU_MAINTENANCE_TOP);
				break;
			case SCREEN_STATE_MENU_MAINTENANCE_USERPWCHANGE_CURRENT:	
				SetScreenIndex(SCREEN_STATE_MENU_MAINTENANCE_USERPWCHANGE_TOP);
				break;
			case SCREEN_STATE_MENU_MAINTENANCE_USERPWCHANGE_NEW:	
				SetScreenIndex(SCREEN_STATE_MENU_MAINTENANCE_USERPWCHANGE_CURRENT);
				break;
			case SCREEN_STATE_MENU_MAINTENANCE_USERPWCHANGE_RETYPE:	
				SetScreenIndex(SCREEN_STATE_MENU_MAINTENANCE_USERPWCHANGE_NEW);
				break;

			case SCREEN_STATE_MENU_DISPLAYSETTING_ESLPWCHANGE_CURRENT:
				SetScreenIndex(SCREEN_STATE_MENU_DISPLAYSETTING_ESLPWCHANGE_TOP);
				break;
			case SCREEN_STATE_MENU_DISPLAYSETTING_ESLPWCHANGE_NEW:
				SetScreenIndex(SCREEN_STATE_MENU_DISPLAYSETTING_ESLPWCHANGE_CURRENT);
				break;
			case SCREEN_STATE_MENU_DISPLAYSETTING_ESLPWCHANGE_RETYPE:
				SetScreenIndex(SCREEN_STATE_MENU_DISPLAYSETTING_ESLPWCHANGE_NEW);
				break;
		}	
	}
	else
	{
		switch(ScreenIndex)
		{
			case SCREEN_STATE_ESL_PASSWORD:
			case SCREEN_STATE_MENU_EQUIPMENT_WEIGHTSENSOR_CROSSSECTION_INPUT:
			case SCREEN_STATE_MENU_EQUIPMENT_WEIGHTSENSOR_LOADADJUST_LOAD_INPUT:
			case SCREEN_STATE_MENU_MAINTENANCE_PW:
			case SCREEN_STATE_MENU_EQUIPMENT_PW:
			case SCREEN_STATE_MENU_MAINTENANCE_USERPWCHANGE_CURRENT:	
			case SCREEN_STATE_MENU_MAINTENANCE_USERPWCHANGE_NEW:	
			case SCREEN_STATE_MENU_MAINTENANCE_USERPWCHANGE_RETYPE:	
			case SCREEN_STATE_MENU_DISPLAYSETTING_ESLPWCHANGE_CURRENT:
			case SCREEN_STATE_MENU_DISPLAYSETTING_ESLPWCHANGE_NEW:
			case SCREEN_STATE_MENU_DISPLAYSETTING_ESLPWCHANGE_RETYPE:
				DisplayRetypeNumber();
				break;
		}	

		DrawMenuFull();
	}
}

void EnterKeyEventNumberInput()
{
	if(CurNumber < MaxNumber-1)
	{
		if(strLength == 0)
		{
			if(InputType == NUMBER_INFO_DOT)
			{
				if(ArrNumberDot_num[CurNumber] == '.')
				{
					BufNumber[0] = 0;
					strLength = 1;
					BufNumber[strLength++] = ArrNumberDot_num[CurNumber];
					bInputDot = 1;
					DotCursur = strLength;
					InitializeArrNumber();
				}
				else
				{
					bInputDot = 0;
					InitializeArrNumber();
					BufNumber[strLength++] = ArrNumberDot_num[CurNumber];
				}
			}
			else
			{
				BufNumber[strLength++] = ArrNumber_num[CurNumber];
			}
			DisplayNumberInput();
		}
		else if(strLength < strMaxLength)
		{
			if(bInputDot == 0)
			{
				BufNumber[strLength++] = ArrNumberDot_num[CurNumber];
				if(ArrNumberDot_num[CurNumber] == '.')
				{
					bInputDot = 1;
					DotCursur = strLength;
					InitializeArrNumber();
				}
				else if(((strLength == 3) && (ScreenIndex == SCREEN_STATE_MENU_EQUIPMENT_WEIGHTSENSOR_CROSSSECTION_INPUT))
					|| ((strLength == 2) && (ScreenIndex == SCREEN_STATE_MENU_EQUIPMENT_WEIGHTSENSOR_LOADADJUST_LOAD_INPUT)))
				{
					BufNumber[strLength++] = '.';
					bInputDot = 1;
					DotCursur = strLength;
					InitializeArrNumber();
				}
				
			}
			else
			{
				BufNumber[strLength++] = ArrNumber_num[CurNumber];
			}
			DisplayNumberInput();

			if(strLength == strMaxLength)
			{
				OldCurNumber = CurNumber;
				CurNumber = MaxNumber-1;
				ReDrawNumber();
			}
		}
		else
		{
			OldCurNumber = CurNumber;
			CurNumber = MaxNumber-1;
			ReDrawNumber();
		}
	}
	// Enter ??? 
	else
	{
		if(ScreenIndex == SCREEN_STATE_MENU_EQUIPMENT_WEIGHTSENSOR_CROSSSECTION_INPUT)
		{
			if(strLength != 0)
			{
				AD_DATA.C_A_IntegerValue = GetNumberInteger();
				AD_DATA.C_A_PointValue= GetNumberPoint();
			}
			CalibrationFlag = CROSSSECTRION_INPUT;
			Checkcnt100ms = 0;
		}
		else if(ScreenIndex == SCREEN_STATE_MENU_EQUIPMENT_WEIGHTSENSOR_LOADADJUST_LOAD_INPUT)
		{
			if(strLength != 0)
			{
				AD_DATA.C_Max_Ton =  GetNumberInteger()*10 + GetNumberPoint();
			}
			Equipment_Calibration = 0x02;
			SetScreenIndex(SCREEN_STATE_MENU_EQUIPMENT_WEIGHTSENSOR_LOADADJUST_LOAD_INFO);
		}
		else
		{
			if(strLength <5)
			{
				Password_Check_Result = PASSWORD_ENTER_REQUEST;
				SetScreenIndex(ScreenIndex|0xf);
			}
			else
			{
				Password_Check_Result = PASSWORD_NONE;
				GetPassword();
				switch(ScreenIndex)
				{
					case SCREEN_STATE_ESL_PASSWORD:
					case SCREEN_STATE_MENU_DISPLAYSETTING_ESLPWCHANGE_CURRENT:
						Password_Check_Result = ESL_Master_password_check();
						break;
					case SCREEN_STATE_MENU_MAINTENANCE_PW:
					case SCREEN_STATE_MENU_EQUIPMENT_PW:
					case SCREEN_STATE_MENU_MAINTENANCE_USERPWCHANGE_CURRENT:
						Password_Check_Result = User_Master_password_check();
						break;
					case SCREEN_STATE_MENU_MAINTENANCE_USERPWCHANGE_NEW:	
					case SCREEN_STATE_MENU_DISPLAYSETTING_ESLPWCHANGE_NEW:
						for(UCHAR i=0;i<sizeof(BufNumber_change);i++)
						{
							BufNumber_change[i] = BufNumber[i];
						}
						SetScreenIndex(ScreenIndex|0x00001000);
						break;
					case SCREEN_STATE_MENU_MAINTENANCE_USERPWCHANGE_RETYPE:	
					case SCREEN_STATE_MENU_DISPLAYSETTING_ESLPWCHANGE_RETYPE:
						for(UCHAR i=0;i<sizeof(BufNumber_change);i++)
						{
							if(BufNumber_change[i] != BufNumber[i])
							{
								Password_Check_Result = PASSWORD_NOT_CONFIRM;
								SetScreenIndex(ScreenIndex|0xf);
								return;
							}
						}

						if(ScreenIndex == SCREEN_STATE_MENU_MAINTENANCE_USERPWCHANGE_RETYPE)
							Password_Check_Result = User_password_Change();
						else
							Password_Check_Result = ESL_password_Change();
					
						break;

				}

				if(Password_Check_Result == ESL_PASSWORD_CHECK_OK)
				{
					switch(ScreenIndex)
					{
						case SCREEN_STATE_ESL_PASSWORD:
							SetScreenIndex(SCREEN_STATE_MAIN_TOP);
							break;
						case SCREEN_STATE_MENU_DISPLAYSETTING_ESLPWCHANGE_CURRENT:	
							SetScreenIndex(SCREEN_STATE_MENU_DISPLAYSETTING_ESLPWCHANGE_NEW);
							break;
					}
				}
				else if(Password_Check_Result == USER_PASSWORD_CHECK_OK)
				{
					switch(ScreenIndex)
					{
						case SCREEN_STATE_MENU_EQUIPMENT_PW:		
							// ++, 200914 bwk
							if((Flag_DIN[INDEX_SW_BUCKLE] == 1)||(InfoModel2.SeatBeltInterlock == 1)) //++,--, 201019 ysm
								COUNT_FLAG.ShowMenuSbIl = 1;
							else
								COUNT_FLAG.ShowMenuSbIl = 0;
							// --, 200914 bwk
							SetScreenIndex(SCREEN_STATE_MENU_EQUIPMENT_MODELSELECT_TOP);
							break;
						case SCREEN_STATE_MENU_MAINTENANCE_PW:	
							SetScreenIndex(SCREEN_STATE_MENU_MAINTENANCE_FAULTHISTORY_TOP);
							break;
						case SCREEN_STATE_MENU_MAINTENANCE_USERPWCHANGE_CURRENT:	
							SetScreenIndex(SCREEN_STATE_MENU_MAINTENANCE_USERPWCHANGE_NEW);
							break;
					}
				}		
				else if(Password_Check_Result == MASTER_PASSWORD_CHECK_OK)
				{
					switch(ScreenIndex)
					{
						case SCREEN_STATE_ESL_PASSWORD:
							SetScreenIndex(SCREEN_STATE_MAIN_TOP);
							break;
						case SCREEN_STATE_MENU_EQUIPMENT_PW:		
							// ++, 200914 bwk
							if((Flag_DIN[INDEX_SW_BUCKLE] == 1)||(InfoModel2.SeatBeltInterlock == 1)) //++,--, 201019 ysm
								COUNT_FLAG.ShowMenuSbIl = 1; // 1->0
							else
								COUNT_FLAG.ShowMenuSbIl = 0;
							// --, 200914 bwk
							SetScreenIndex(SCREEN_STATE_MENU_EQUIPMENT_MODELSELECT_TOP);
							break;
						case SCREEN_STATE_MENU_MAINTENANCE_PW:	
							SetScreenIndex(SCREEN_STATE_MENU_MAINTENANCE_FAULTHISTORY_TOP);
							break;
						case SCREEN_STATE_MENU_MAINTENANCE_USERPWCHANGE_CURRENT:	
							SetScreenIndex(SCREEN_STATE_MENU_MAINTENANCE_USERPWCHANGE_NEW);
							break;
						case SCREEN_STATE_MENU_DISPLAYSETTING_ESLPWCHANGE_CURRENT:	
							SetScreenIndex(SCREEN_STATE_MENU_DISPLAYSETTING_ESLPWCHANGE_NEW);
							break;
					}
				}
				else if(Password_Check_Result == ESL_PASSWORD_CHANGE_OK)
				{
					if(ScreenIndex == SCREEN_STATE_MENU_DISPLAYSETTING_ESLPWCHANGE_RETYPE)
						SetScreenIndex(SCREEN_STATE_MENU_DISPLAYSETTING_ESLPWCHANGE_COMPLETE);
				}
				else if(Password_Check_Result == ESL_PASSWORD_CHANGE_NOT_OK)
				{
					if(ScreenIndex == SCREEN_STATE_MENU_DISPLAYSETTING_ESLPWCHANGE_RETYPE)
						SetScreenIndex(SCREEN_STATE_MENU_DISPLAYSETTING_ESLPWCHANGE_NOT_COMPLETE);
				}
				else if(Password_Check_Result == USER_PASSWORD_CHANGE_OK)
				{
					if(ScreenIndex == SCREEN_STATE_MENU_MAINTENANCE_USERPWCHANGE_RETYPE)
						SetScreenIndex(SCREEN_STATE_MENU_MAINTENANCE_USERPWCHANGE_COMPLETE);
				}
				else if(Password_Check_Result == USER_PASSWORD_CHANGE_NOT_OK)
				{
					if(ScreenIndex == SCREEN_STATE_MENU_MAINTENANCE_USERPWCHANGE_RETYPE)
						SetScreenIndex(SCREEN_STATE_MENU_MAINTENANCE_USERPWCHANGE_NOT_COMPLETE);
				}
				else if((Password_Check_Result == USER_PASSWORD_CHECK_NOT_OK)
					|| (Password_Check_Result == ESL_PASSWORD_CHECK_NOT_OK))
				{
					SetScreenIndex(ScreenIndex|0xf);
				}
			}
		}

	}
		
}
//////////////////////////////////////////////////////////////////////////////
void SetMaxStrLength()
{
	switch(ScreenIndex)
	{
		case SCREEN_STATE_ESL_PASSWORD:
		case SCREEN_STATE_MENU_EQUIPMENT_PW:
		case SCREEN_STATE_MENU_MAINTENANCE_PW:
		case SCREEN_STATE_MENU_MAINTENANCE_USERPWCHANGE_CURRENT:	
		case SCREEN_STATE_MENU_MAINTENANCE_USERPWCHANGE_NEW:	
		case SCREEN_STATE_MENU_MAINTENANCE_USERPWCHANGE_RETYPE:	
		case SCREEN_STATE_MENU_DISPLAYSETTING_ESLPWCHANGE_CURRENT:
		case SCREEN_STATE_MENU_DISPLAYSETTING_ESLPWCHANGE_NEW:
		case SCREEN_STATE_MENU_DISPLAYSETTING_ESLPWCHANGE_RETYPE:
			strMaxLength = 10;
			break;
		case SCREEN_STATE_MENU_EQUIPMENT_WEIGHTSENSOR_CROSSSECTION_INPUT:
			strMaxLength = 6;
			break;
		case SCREEN_STATE_MENU_EQUIPMENT_WEIGHTSENSOR_LOADADJUST_LOAD_INPUT:
			strMaxLength = 4;
			break;
	}
}


void InitializeDisplayPassword(UCHAR nInputType)
{
	CurNumber = 0;
	OldCurNumber = 0;
	InputType = nInputType;
	DotCursur = 0;
	if(nInputType == NUMBER_INFO_DOT)
		bInputDot = 0;
	else if(nInputType == NUMBER_INFO_ENTER)
		bInputDot = 1;
	
	InitializeArrNumber();
	SetMaxStrLength();
	DisplayMenuBackgroundBG();
	DisplayInputEditBox();

	if(nInputType == NUMBER_INFO_DOT)
	{
		DisplayNumberDot();
	}
	else if(nInputType == NUMBER_INFO_ENTER)
	{
		DisplayNumber();
	}
}

void DisplayPasswordPopup()
{
	if(OldScreenIndex != ScreenIndex)
	{
		OldScreenIndex = ScreenIndex;
		switch(Password_Check_Result)
		{
			case PASSWORD_ENTER_REQUEST:
				DisplayPopupString1((USHORT*)*StringSentence[4]);
				break;
			case ESL_PASSWORD_CHECK_NOT_OK:
			case USER_PASSWORD_CHECK_NOT_OK:				
				DisplayPopupString1((USHORT*)*StringSentence[2]);
				break;
			case USER_PASSWORD_CHANGE_OK:
			case ESL_PASSWORD_CHANGE_OK:
				DisplayPopupString1((USHORT*)*StringSentence[INDEX_STR_COMPLETE]);
				break;
			case USER_PASSWORD_CHANGE_NOT_OK:
			case ESL_PASSWORD_CHANGE_NOT_OK:
				DisplayPopupString1((USHORT*)*StringSentence[INDEX_STR_NOT_COMPLETE]);
				break;
			case PASSWORD_NOT_CONFIRM:
				DisplayPopupString1((USHORT*)*StringSentence[3]);
				break;
		}
	}

}
