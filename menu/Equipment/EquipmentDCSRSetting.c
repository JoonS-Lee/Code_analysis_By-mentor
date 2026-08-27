#include "FLV_Cluster_APP.h"

unsigned char tempSpeed;
unsigned char OldtempSpeed;

//FingerTip DeadZone Value
//bit per 0.01V (0.00V~5.00V)

// REAL VALUE
USHORT FLT_Lever_Voltage[3];
// CLUSTER VALUE
USHORT Temp_FLT_Lever_Voltage[3];
// CONTROL VALUE - Real Value
USHORT Control_FLT_Lever_Voltage, Old_Control_FLT_Lever_Voltage;

//FingerTip DeadZone Value
//bit per 0.01V (0.00V~0.50V)
USHORT FLT_DeadZone_Voltage[2];
USHORT Temp_FLT_DeadZone_Voltage[2];
USHORT Control_FLT_DeadZone_Voltage,Old_Control_FLT_DeadZone_Voltage;

//FingerTip Valve Value
//bit per 0.01V (0.00V~0.50V)
// REAL VALUE
USHORT FLT_Valve_Value[4];
// CLUSTER VALUE
USHORT Temp_FLT_Valve_Value[4];
USHORT Control_FLT_Valve_Value, Old_Control_FLT_Valve_Value;

USHORT	FLT_loading[]={'L','o','a','d','i','n','g',' ', '.','.','.', 0x00 ,0x00};
UCHAR FLT_Step_Old = 0;



extern UCHAR SubCurserIndex;
extern UCHAR OldSubCurserIndex;

void DisplaySettingSpeed()
{
	UCHAR buf[10];
	USHORT covbuf[10];

	USHORT autoshfit_tempSpeed;
	float zerostart_tempSpeed;
	float sr_tempSpeed;

	if(OldScreenIndex != ScreenIndex)
	{
		DisplayMenuBackgroundBG();
		OldCurserIndex = OldtempSpeed = 0xff;
		CurserIndex = CURSER_NONE;
	}

	if(OldtempSpeed != tempSpeed)
	{
		OldtempSpeed = tempSpeed;

		autoshfit_tempSpeed = 10*OldtempSpeed;
		zerostart_tempSpeed = 0.4*(float)OldtempSpeed;
		sr_tempSpeed = 0.1*(float)OldtempSpeed;
		
		memset((&buf), 0x0, sizeof( buf));
		memset((&covbuf), 0x0, sizeof( covbuf));

		if((ScreenIndex == SCREEN_STATE_MENU_EQUIPMENT_AUTOSHIFT_OVERLAP_UP_SET)||(ScreenIndex == SCREEN_STATE_MENU_EQUIPMENT_AUTOSHIFT_OVERLAP_DOWN_SET))
		{
			sprintf ( (char*)buf, "%d ms",autoshfit_tempSpeed) ;
		}
		else if(ScreenIndex == SCREEN_STATE_MENU_EQUIPMENT_ZEROSTART_ACCEL_SET)
		{

			sprintf ( (char*)buf, "%.1f %c",zerostart_tempSpeed, 0x25) ;
		}
		else if((ScreenIndex == SCREEN_STATE_MENU_EQUIPMENT_AUTOSHIFT_SR_UP_SET)||(ScreenIndex == SCREEN_STATE_MENU_EQUIPMENT_AUTOSHIFT_SR_DOWN_SET))
		{
			sprintf ( (char*)buf, "%.1f",sr_tempSpeed) ;
		}
		else
		{
			sprintf ( (char*)buf, "%d km/h",OldtempSpeed) ; 
		}
		String_Func(covbuf, buf);

		LCD_Draw_Color(50, 97, 120, 22, COLOR_BLACK);
		GUI_DisplayCenterStringAt(&covbuf[0], 0, 97, LCD_WIDTH, COLOR_WHITE, FONTSIZE);	
	}

	if(OldCurserIndex != CurserIndex)
	{
		LCD_Draw_Color(6, 90, 31, 39, (CurserIndex == CURSER_UP_LEFT)?COLOR_MENU_ORANGE:COLOR_MENU_GRAY);
		PCXtoBMP_16bit(16, 101, 10, 15, (CurserIndex == CURSER_UP_LEFT)?FL_Image.menu_arrow_setting_select_2:FL_Image.menu_arrow_nor_2);

		LCD_Draw_Color(183, 90, 31, 39, (CurserIndex == CURSER_DOWN_RIGHT)?COLOR_MENU_ORANGE:COLOR_MENU_GRAY);
		PCXtoBMP_16bit(193, 101, 10, 15, (CurserIndex == CURSER_DOWN_RIGHT)?FL_Image.menu_arrow_setting_select:FL_Image.menu_arrow_nor);
	}

	if(OldScreenIndex != ScreenIndex)
	{
		OldCurserIndex = CurserIndex;
		DrawMenuFull();
	}
	else if(OldCurserIndex != CurserIndex)
	{
		OldCurserIndex = CurserIndex;
	}	
}

void DisplayDSCR_MaxSpeedLImit()
{
	switch(ScreenIndex)
	{
		case SCREEN_STATE_MENU_EQUIPMENT_DCSR_MODE_TOP:						
		case SCREEN_STATE_MENU_EQUIPMENT_DCSR_SPEED_TOP:						
		case SCREEN_STATE_MENU_EQUIPMENT_MAXSPEEDLIMIT_FUNC_TOP:	
		case SCREEN_STATE_MENU_EQUIPMENT_MAXSPEEDLIMIT_SPEED_TOP:
		case SCREEN_STATE_MENU_EQUIPMENT_ZEROSTART_MODE_TOP:
		case SCREEN_STATE_MENU_EQUIPMENT_ZEROSTART_ACCEL_TOP:		
			if((OldScreenIndex != SCREEN_STATE_MENU_EQUIPMENT_DCSR_MODE_TOP)
				&& (OldScreenIndex != SCREEN_STATE_MENU_EQUIPMENT_DCSR_SPEED_TOP)
				&& (OldScreenIndex != SCREEN_STATE_MENU_EQUIPMENT_MAXSPEEDLIMIT_FUNC_TOP)
				&& (OldScreenIndex != SCREEN_STATE_MENU_EQUIPMENT_MAXSPEEDLIMIT_SPEED_TOP)
				&& (OldScreenIndex != SCREEN_STATE_MENU_EQUIPMENT_ZEROSTART_MODE_TOP)
				&& (OldScreenIndex != SCREEN_STATE_MENU_EQUIPMENT_ZEROSTART_ACCEL_TOP))
			{
				if((ScreenIndex == SCREEN_STATE_MENU_EQUIPMENT_DCSR_MODE_TOP)
					|| (ScreenIndex == SCREEN_STATE_MENU_EQUIPMENT_DCSR_SPEED_TOP))
					SetListString2(62, 63);
				else if((ScreenIndex == SCREEN_STATE_MENU_EQUIPMENT_ZEROSTART_MODE_TOP)
					|| (ScreenIndex == SCREEN_STATE_MENU_EQUIPMENT_ZEROSTART_ACCEL_TOP))
					SetListString2(62, 230); //++,--, 230202 ysm, FSCU 200->230
				else
					SetListString2(62, 66);
				DisplayListAll((ScreenIndex&0x00F00000)>>20);
				DrawMenuFull();
			}
			else if(OldScreenIndex != ScreenIndex)
			{
				DisplayList((ScreenIndex&0x00F00000)>>20);
			}
			break;

		
		case SCREEN_STATE_MENU_EQUIPMENT_AUTOSHIFT_MODE_TOP: 
		case SCREEN_STATE_MENU_EQUIPMENT_AUTOSHIFT_SPEED_TOP:		
			if((OldScreenIndex != SCREEN_STATE_MENU_EQUIPMENT_AUTOSHIFT_MODE_TOP)
				&& (OldScreenIndex != SCREEN_STATE_MENU_EQUIPMENT_AUTOSHIFT_SPEED_TOP))
			{			
				SetListString2(62,63);
				DisplayListAll((ScreenIndex&0x00F00000)>>20);	
				DrawMenuFull();

			}			
			else if(OldScreenIndex != ScreenIndex)
			{
				DisplayList((ScreenIndex&0x00F00000)>>20);
			}
			break;	
		case SCREEN_STATE_MENU_EQUIPMENT_DCSR_MODE_SET:		
		case SCREEN_STATE_MENU_EQUIPMENT_MAXSPEEDLIMIT_FUNC_SET:	
		case SCREEN_STATE_MENU_EQUIPMENT_HAC_SET:					
		case SCREEN_STATE_MENU_EQUIPMENT_CLUTCHPROTECTION_SET:
		case SCREEN_STATE_MENU_EQUIPMENT_ZEROSTART_MODE_SET:
		case SCREEN_STATE_MENU_EQUIPMENT_SEATBELTINTERLOCK_SET:	
		case SCREEN_STATE_MENU_EQUIPMENT_AUTOSHIFT_MODE_SET:
			if(OldScreenIndex != ScreenIndex)
			{
				SetSubListString(2, CurserIndex, StringEtc[SIGN_OFF], StringEtc[SIGN_ON], StringEtc[SIGN_NOSTRING], StringEtc[SIGN_NOSTRING], StringEtc[SIGN_NOSTRING], StringEtc[SIGN_NOSTRING]);
				OldCurserIndex = CurserIndex;
			}
			else if(OldCurserIndex != CurserIndex)
			{
				DisplaySubDoubleList(CurserIndex);
			}
			break;
		case SCREEN_STATE_MENU_EQUIPMENT_HAC_COMPLETE:							
		case SCREEN_STATE_MENU_EQUIPMENT_HAC_NOT_COMPLETE:				
		case SCREEN_STATE_MENU_EQUIPMENT_DCSR_MODE_COMPLETE:				
		case SCREEN_STATE_MENU_EQUIPMENT_DCSR_MODE_NOT_COMPLETE:		
		case SCREEN_STATE_MENU_EQUIPMENT_DCSR_SPEED_BLOCKING_COMPLETE:			
		case SCREEN_STATE_MENU_EQUIPMENT_DCSR_SPEED_RESTORE_COMPLETE:			
		case SCREEN_STATE_MENU_EQUIPMENT_DCSR_SPEED_BLOCKING_NOT_COMPLETE:			
		case SCREEN_STATE_MENU_EQUIPMENT_DCSR_SPEED_RESTORE_NOT_COMPLETE:			
		case SCREEN_STATE_MENU_EQUIPMENT_MAXSPEEDLIMIT_FUNC_COMPLETE:
		case SCREEN_STATE_MENU_EQUIPMENT_MAXSPEEDLIMIT_SPEED_SET_COMPLETE:
		case SCREEN_STATE_MENU_EQUIPMENT_MAXSPEEDLIMIT_FUNC_NOT_COMPLETE:
		case SCREEN_STATE_MENU_EQUIPMENT_MAXSPEEDLIMIT_SPEED_SET_NOT_COMPLETE:
		case SCREEN_STATE_MENU_EQUIPMENT_AUTOSHIFT_MODE_COMPLETE:
		case SCREEN_STATE_MENU_EQUIPMENT_AUTOSHIFT_MODE_NOT_COMPLETE:
		case SCREEN_STATE_MENU_EQUIPMENT_AUTOSHIFT_SPEED_UP_COMPLETE:
		case SCREEN_STATE_MENU_EQUIPMENT_AUTOSHIFT_SPEED_UP_NOT_COMPLETE:
		case SCREEN_STATE_MENU_EQUIPMENT_AUTOSHIFT_SPEED_DOWN_COMPLETE:
		case SCREEN_STATE_MENU_EQUIPMENT_AUTOSHIFT_SPEED_DOWN_NOT_COMPLETE:
			
		case SCREEN_STATE_MENU_EQUIPMENT_ZEROSTART_MODE_COMPLETE:
		case SCREEN_STATE_MENU_EQUIPMENT_ZEROSTART_MODE_NOT_COMPLETE:
		case SCREEN_STATE_MENU_EQUIPMENT_ZEROSTART_ACCEL_COMPLETE:
		case SCREEN_STATE_MENU_EQUIPMENT_ZEROSTART_ACCEL_NOT_COMPLETE:	
		case SCREEN_STATE_MENU_EQUIPMENT_CLUTCHPROTECTION_SET_COMPLETE:
		case SCREEN_STATE_MENU_EQUIPMENT_CLUTCHPROTECTION_SET_NOT_COMPLETE:	
		case SCREEN_STATE_MENU_EQUIPMENT_SEATBELTINTERLOCK_SET_COMPLETE:
		case SCREEN_STATE_MENU_EQUIPMENT_SEATBELTINTERLOCK_SET_NOT_COMPLETE:	
			DisplayPopupStringComplete();
			break;

			
		case SCREEN_STATE_MENU_EQUIPMENT_DCSR_SPEED_BLOCKING_TOP:	
		case SCREEN_STATE_MENU_EQUIPMENT_DCSR_SPEED_RESTORE_TOP:
			if((OldScreenIndex != SCREEN_STATE_MENU_EQUIPMENT_DCSR_SPEED_BLOCKING_TOP)
				&& (OldScreenIndex != SCREEN_STATE_MENU_EQUIPMENT_DCSR_SPEED_RESTORE_TOP))
			{
				SetListString2(64, 65);
				DisplayListAll((ScreenIndex&0x000F0000)>>16);
				DrawMenuFull();
			}
			else if(OldScreenIndex != ScreenIndex)
			{
				DisplayList((ScreenIndex&0x000F0000)>>16);
			}
			break;
		case SCREEN_STATE_MENU_EQUIPMENT_AUTOSHIFT_SPEED_UP_TOP:
		case SCREEN_STATE_MENU_EQUIPMENT_AUTOSHIFT_SPEED_DOWN_TOP:
			if((OldScreenIndex != SCREEN_STATE_MENU_EQUIPMENT_AUTOSHIFT_SPEED_UP_TOP)
			&& (OldScreenIndex != SCREEN_STATE_MENU_EQUIPMENT_AUTOSHIFT_SPEED_DOWN_TOP))
			{
				SetListString2(191, 192);
				DisplayListAll((ScreenIndex&0x000F0000)>>16);
				DrawMenuFull();		

			}
			else if(OldScreenIndex != ScreenIndex)
			{
				DisplayList((ScreenIndex&0x000F0000)>>16);
			}
			break;

	
		case SCREEN_STATE_MENU_EQUIPMENT_DCSR_SPEED_BLOCKING_SET:	
		case SCREEN_STATE_MENU_EQUIPMENT_DCSR_SPEED_RESTORE_SET:	
		case SCREEN_STATE_MENU_EQUIPMENT_MAXSPEEDLIMIT_SPEED_SET:
		case SCREEN_STATE_MENU_EQUIPMENT_AUTOSHIFT_SPEED_UP_SET:
		case SCREEN_STATE_MENU_EQUIPMENT_AUTOSHIFT_SPEED_DOWN_SET:	
		case SCREEN_STATE_MENU_EQUIPMENT_ZEROSTART_ACCEL_SET:	
			DisplaySettingSpeed();
			break;

		case SCREEN_STATE_MENU_EQUIPMENT_MACHINEINFO_ECM_TOP:
		case SCREEN_STATE_MENU_EQUIPMENT_MACHINEINFO_CLUSTER_TOP:
		case SCREEN_STATE_MENU_EQUIPMENT_MACHINEINFO_RMCU_TOP:
		case SCREEN_STATE_MENU_EQUIPMENT_MACHINEINFO_FLT_TOP:
		case SCREEN_STATE_MENU_EQUIPMENT_MACHINEINFO_FSCU_TOP:	//++,--, 220929 ysm, FSCU
			if((OldScreenIndex != SCREEN_STATE_MENU_EQUIPMENT_MACHINEINFO_ECM_TOP)
			&& (OldScreenIndex != SCREEN_STATE_MENU_EQUIPMENT_MACHINEINFO_CLUSTER_TOP)
			&& (OldScreenIndex != SCREEN_STATE_MENU_EQUIPMENT_MACHINEINFO_RMCU_TOP)
			&& (OldScreenIndex != SCREEN_STATE_MENU_EQUIPMENT_MACHINEINFO_FLT_TOP)
			&& (OldScreenIndex != SCREEN_STATE_MENU_EQUIPMENT_MACHINEINFO_FSCU_TOP))
			{
				//++, --, 220929 ysm, FSCU
				//++, 220211 ysm, FINGERTIP
				if(COUNT_FLAG.Flag_FSCU_Enable == 1)
				{
					if((COUNT_FLAG.Flag_ECM_Check == 1)&&(COUNT_FLAG.Flag_RMCU_Check == 1)&&(COUNT_FLAG.Flag_FLT_Check == 1)) // 111
					{
						SetListString_3(StringMachine[0], StringMachine[1], StringMachine[2]);
						DisplayListPage(1,2);
						if(ScreenIndex == SCREEN_STATE_MENU_EQUIPMENT_MACHINEINFO_ECM_TOP)
							DisplayListAll(1);
						else if(ScreenIndex == SCREEN_STATE_MENU_EQUIPMENT_MACHINEINFO_CLUSTER_TOP)
							DisplayListAll(2);
						else
							DisplayListAll(3);
						DrawMenuFull();
					}
					else if((COUNT_FLAG.Flag_ECM_Check == 1)&&(COUNT_FLAG.Flag_RMCU_Check == 1)&&(COUNT_FLAG.Flag_FLT_Check == 0)) // 110
					{
						SetListString_3(StringMachine[0], StringMachine[1], StringMachine[2] );
						DisplayListPage(1,2);
						if(ScreenIndex == SCREEN_STATE_MENU_EQUIPMENT_MACHINEINFO_ECM_TOP)
							DisplayListAll(1);
						else if(ScreenIndex == SCREEN_STATE_MENU_EQUIPMENT_MACHINEINFO_CLUSTER_TOP)
							DisplayListAll(2);
						else
							DisplayListAll(3);
						DrawMenuFull();

					}
					else if((COUNT_FLAG.Flag_ECM_Check == 1)&&(COUNT_FLAG.Flag_RMCU_Check == 0)&&(COUNT_FLAG.Flag_FLT_Check == 1)) // 101
					{

						SetListString_3(StringMachine[0], StringMachine[1], StringMachine[3] );
						DisplayListPage(1,2);
						if(ScreenIndex == SCREEN_STATE_MENU_EQUIPMENT_MACHINEINFO_ECM_TOP)
							DisplayListAll(1);
						else if(ScreenIndex == SCREEN_STATE_MENU_EQUIPMENT_MACHINEINFO_CLUSTER_TOP)
							DisplayListAll(2);
						else
							DisplayListAll(3);
						DrawMenuFull();

					}
					else if((COUNT_FLAG.Flag_ECM_Check == 1)&&(COUNT_FLAG.Flag_RMCU_Check == 0)&&(COUNT_FLAG.Flag_FLT_Check == 0)) // 100
					{
						SetListString_3(StringMachine[0], StringMachine[1], StringMachine[4] );
						if(ScreenIndex == SCREEN_STATE_MENU_EQUIPMENT_MACHINEINFO_ECM_TOP)
							DisplayListAll(1);
						else if(ScreenIndex == SCREEN_STATE_MENU_EQUIPMENT_MACHINEINFO_CLUSTER_TOP)
							DisplayListAll(2);
						else
							DisplayListAll(3);
						DrawMenuFull();

					}
					else if((COUNT_FLAG.Flag_ECM_Check == 0)&&(COUNT_FLAG.Flag_RMCU_Check == 1)&&(COUNT_FLAG.Flag_FLT_Check == 1)) // 011
					{
						SetListString_3(StringMachine[1], StringMachine[2], StringMachine[3] );
						DisplayListPage(1,2);
						if(ScreenIndex == SCREEN_STATE_MENU_EQUIPMENT_MACHINEINFO_CLUSTER_TOP)
							DisplayListAll(1);
						else if(ScreenIndex == SCREEN_STATE_MENU_EQUIPMENT_MACHINEINFO_RMCU_TOP)
							DisplayListAll(2);
						else
							DisplayListAll(3);
						DrawMenuFull();


					}
					else if((COUNT_FLAG.Flag_ECM_Check == 0)&&(COUNT_FLAG.Flag_RMCU_Check == 1)&&(COUNT_FLAG.Flag_FLT_Check == 0)) // 010
					{
						SetListString_3(StringMachine[1], StringMachine[2], StringMachine[4] );
						if(ScreenIndex == SCREEN_STATE_MENU_EQUIPMENT_MACHINEINFO_CLUSTER_TOP)
							DisplayListAll(1);
						else if(ScreenIndex == SCREEN_STATE_MENU_EQUIPMENT_MACHINEINFO_RMCU_TOP)
							DisplayListAll(2);
						else
							DisplayListAll(3);
						DrawMenuFull();



					}
					else if((COUNT_FLAG.Flag_ECM_Check == 0)&&(COUNT_FLAG.Flag_RMCU_Check == 0)&&(COUNT_FLAG.Flag_FLT_Check == 1)) // 001
					{

						SetListString_3(StringMachine[1], StringMachine[3], StringMachine[4] );
						if(ScreenIndex == SCREEN_STATE_MENU_EQUIPMENT_MACHINEINFO_CLUSTER_TOP)
							DisplayListAll(1);
						else if(ScreenIndex == SCREEN_STATE_MENU_EQUIPMENT_MACHINEINFO_FLT_TOP)
							DisplayListAll(2);
						else
							DisplayListAll(3);
						DrawMenuFull();


					}	
					else // 000
					{
						SetListString_2(StringMachine[1], StringMachine[4]);
						if(ScreenIndex == SCREEN_STATE_MENU_EQUIPMENT_MACHINEINFO_CLUSTER_TOP)
							DisplayListAll(1);
						else
							DisplayListAll(2);
						DrawMenuFull();

					}				

				}
				else
				{				
					if((COUNT_FLAG.Flag_ECM_Check == 1)&&(COUNT_FLAG.Flag_RMCU_Check == 1)&&(COUNT_FLAG.Flag_FLT_Check == 1)) // 111
					{

						if((OldScreenIndex&0xFFF00000) == SCREEN_STATE_MENU_EQUIPMENT_MACHINEINFO_FLT_TOP)
						{
							SetListString_1(StringMachine[3]);
							DisplayListPage(2,2);
							DisplayListAll(1);
							DrawMenuFull();

						}
						else
						{
							SetListString_3(StringMachine[0], StringMachine[1], StringMachine[2] );
							DisplayListPage(1,2);
							if(ScreenIndex == SCREEN_STATE_MENU_EQUIPMENT_MACHINEINFO_ECM_TOP)
								DisplayListAll(1);
							else if(ScreenIndex == SCREEN_STATE_MENU_EQUIPMENT_MACHINEINFO_CLUSTER_TOP)
								DisplayListAll(2);
							else
								DisplayListAll(3);
							DrawMenuFull();
						}	

					}
					else if((COUNT_FLAG.Flag_ECM_Check == 1)&&(COUNT_FLAG.Flag_RMCU_Check == 1)&&(COUNT_FLAG.Flag_FLT_Check == 0)) // 110
					{
						SetListString_3(StringMachine[0], StringMachine[1], StringMachine[2] );
						if(ScreenIndex == SCREEN_STATE_MENU_EQUIPMENT_MACHINEINFO_ECM_TOP)
							DisplayListAll(1);
						else if(ScreenIndex == SCREEN_STATE_MENU_EQUIPMENT_MACHINEINFO_CLUSTER_TOP)
							DisplayListAll(2);
						else
							DisplayListAll(3);
						DrawMenuFull();

					}
					else if((COUNT_FLAG.Flag_ECM_Check == 1)&&(COUNT_FLAG.Flag_RMCU_Check == 0)&&(COUNT_FLAG.Flag_FLT_Check == 1)) // 101
					{

						SetListString_3(StringMachine[0], StringMachine[1], StringMachine[3] );
						if(ScreenIndex == SCREEN_STATE_MENU_EQUIPMENT_MACHINEINFO_ECM_TOP)
							DisplayListAll(1);
						else if(ScreenIndex == SCREEN_STATE_MENU_EQUIPMENT_MACHINEINFO_CLUSTER_TOP)
							DisplayListAll(2);
						else
							DisplayListAll(3);
						DrawMenuFull();

					}
					else if((COUNT_FLAG.Flag_ECM_Check == 1)&&(COUNT_FLAG.Flag_RMCU_Check == 0)&&(COUNT_FLAG.Flag_FLT_Check == 0)) // 100
					{
						SetListString_2(StringMachine[0], StringMachine[1]);
						if(ScreenIndex == SCREEN_STATE_MENU_EQUIPMENT_MACHINEINFO_ECM_TOP)
							DisplayListAll(1);
						else
							DisplayListAll(2);
						DrawMenuFull();


					}
					else if((COUNT_FLAG.Flag_ECM_Check == 0)&&(COUNT_FLAG.Flag_RMCU_Check == 1)&&(COUNT_FLAG.Flag_FLT_Check == 1)) // 011
					{
						SetListString_3(StringMachine[1], StringMachine[2], StringMachine[3] );
						if(ScreenIndex == SCREEN_STATE_MENU_EQUIPMENT_MACHINEINFO_CLUSTER_TOP)
							DisplayListAll(1);
						else if(ScreenIndex == SCREEN_STATE_MENU_EQUIPMENT_MACHINEINFO_RMCU_TOP)
							DisplayListAll(2);
						else
							DisplayListAll(3);
						DrawMenuFull();


					}
					else if((COUNT_FLAG.Flag_ECM_Check == 0)&&(COUNT_FLAG.Flag_RMCU_Check == 1)&&(COUNT_FLAG.Flag_FLT_Check == 0)) // 010
					{
						SetListString_2(StringMachine[1], StringMachine[2]);
						if(ScreenIndex == SCREEN_STATE_MENU_EQUIPMENT_MACHINEINFO_CLUSTER_TOP)
							DisplayListAll(1);
						else
							DisplayListAll(2);
						DrawMenuFull();


					}
					else if((COUNT_FLAG.Flag_ECM_Check == 0)&&(COUNT_FLAG.Flag_RMCU_Check == 0)&&(COUNT_FLAG.Flag_FLT_Check == 1)) // 001
					{

						SetListString_2(StringMachine[1], StringMachine[3]);
						if(ScreenIndex == SCREEN_STATE_MENU_EQUIPMENT_MACHINEINFO_CLUSTER_TOP)
							DisplayListAll(1);
						else
							DisplayListAll(2);
						DrawMenuFull();

					}	
					else // 000
					{
						SetListString_1(StringMachine[1]);
						DisplayListAll(1);
						DrawMenuFull();

					}				
				}
				//--, 220211 ysm, FINGERTIP			

			}
			else if(OldScreenIndex != ScreenIndex)
			{
				//++, --, 220929 ysm, FSCU
				//++, 220211 ysm, FINGERTIP
				if(COUNT_FLAG.Flag_FSCU_Enable == 1)
				{
					if((COUNT_FLAG.Flag_ECM_Check == 1)&&(COUNT_FLAG.Flag_RMCU_Check == 1)&&(COUNT_FLAG.Flag_FLT_Check == 1)) // 111
					{

						if((ScreenIndex != SCREEN_STATE_MENU_EQUIPMENT_MACHINEINFO_FLT_TOP)&&(ScreenIndex != SCREEN_STATE_MENU_EQUIPMENT_MACHINEINFO_FSCU_TOP))
						{				
							if((OldScreenIndex == SCREEN_STATE_MENU_EQUIPMENT_MACHINEINFO_FLT_TOP)||(OldScreenIndex == SCREEN_STATE_MENU_EQUIPMENT_MACHINEINFO_FSCU_TOP))
							{
								SetListString_3(StringMachine[0], StringMachine[1], StringMachine[2] );
								DisplayListPage(1,2);
								if(ScreenIndex == SCREEN_STATE_MENU_EQUIPMENT_MACHINEINFO_ECM_TOP)
									DisplayListAll(1);
								else if(ScreenIndex == SCREEN_STATE_MENU_EQUIPMENT_MACHINEINFO_CLUSTER_TOP)
									DisplayListAll(2);
								else
									DisplayListAll(3);
								DrawMenuFull();
							}
							else
							{						
								if(ScreenIndex == SCREEN_STATE_MENU_EQUIPMENT_MACHINEINFO_ECM_TOP)
									DisplayList(1);
								else if(ScreenIndex == SCREEN_STATE_MENU_EQUIPMENT_MACHINEINFO_CLUSTER_TOP)
									DisplayList(2);
								else
									DisplayList(3);
							}
						}
						else
						{


							if((OldScreenIndex != SCREEN_STATE_MENU_EQUIPMENT_MACHINEINFO_FLT_TOP)&&(OldScreenIndex != SCREEN_STATE_MENU_EQUIPMENT_MACHINEINFO_FSCU_TOP))
							{						
								SetListString_2(StringMachine[3], StringMachine[4]);
								DisplayListPage(2,2);
								if(ScreenIndex == SCREEN_STATE_MENU_EQUIPMENT_MACHINEINFO_FLT_TOP)
									DisplayListAll(1);
								else
									DisplayListAll(2);
								DrawMenuFull();
							}
							else
							{
								if(ScreenIndex == SCREEN_STATE_MENU_EQUIPMENT_MACHINEINFO_FLT_TOP)
									DisplayList(1);
								else
									DisplayList(2);

							}
						}

					}
					else if((COUNT_FLAG.Flag_ECM_Check == 1)&&(COUNT_FLAG.Flag_RMCU_Check == 1)&&(COUNT_FLAG.Flag_FLT_Check == 0)) // 110
					{

						if(ScreenIndex != SCREEN_STATE_MENU_EQUIPMENT_MACHINEINFO_FSCU_TOP)
						{
							if(OldScreenIndex == SCREEN_STATE_MENU_EQUIPMENT_MACHINEINFO_FSCU_TOP)
							{
								SetListString_3(StringMachine[0], StringMachine[1], StringMachine[2] );
								DisplayListPage(1,2);
								if(ScreenIndex == SCREEN_STATE_MENU_EQUIPMENT_MACHINEINFO_ECM_TOP)
									DisplayListAll(1);
								else if(ScreenIndex == SCREEN_STATE_MENU_EQUIPMENT_MACHINEINFO_CLUSTER_TOP)
									DisplayListAll(2);
								else
									DisplayListAll(3);
								DrawMenuFull();

							}
							else
							{
								if(ScreenIndex == SCREEN_STATE_MENU_EQUIPMENT_MACHINEINFO_ECM_TOP)
									DisplayList(1);
								else if(ScreenIndex == SCREEN_STATE_MENU_EQUIPMENT_MACHINEINFO_CLUSTER_TOP)
									DisplayList(2);
								else
									DisplayList(3);
							}
						}
						else
						{							
							SetListString_1(StringMachine[4]);
							DisplayListPage(2,2);
							DisplayListAll(1);
							DrawMenuFull();
						}

					}
					else if((COUNT_FLAG.Flag_ECM_Check == 1)&&(COUNT_FLAG.Flag_RMCU_Check == 0)&&(COUNT_FLAG.Flag_FLT_Check == 1)) // 101
					{
						if(ScreenIndex != SCREEN_STATE_MENU_EQUIPMENT_MACHINEINFO_FSCU_TOP)
						{
							if(OldScreenIndex == SCREEN_STATE_MENU_EQUIPMENT_MACHINEINFO_FSCU_TOP)
							{
								SetListString_3(StringMachine[0], StringMachine[1], StringMachine[3] );
								DisplayListPage(1,2);
								if(ScreenIndex == SCREEN_STATE_MENU_EQUIPMENT_MACHINEINFO_ECM_TOP)
									DisplayListAll(1);
								else if(ScreenIndex == SCREEN_STATE_MENU_EQUIPMENT_MACHINEINFO_CLUSTER_TOP)
									DisplayListAll(2);
								else
									DisplayListAll(3);
								DrawMenuFull();

							}
							else
							{
								if(ScreenIndex == SCREEN_STATE_MENU_EQUIPMENT_MACHINEINFO_ECM_TOP)
									DisplayList(1);
								else if(ScreenIndex == SCREEN_STATE_MENU_EQUIPMENT_MACHINEINFO_CLUSTER_TOP)
									DisplayList(2);
								else
									DisplayList(3);
							}
						}
						else
						{							
							SetListString_1(StringMachine[4]);
							DisplayListPage(2,2);
							DisplayListAll(1);
							DrawMenuFull();
						}

					}
					else if((COUNT_FLAG.Flag_ECM_Check == 1)&&(COUNT_FLAG.Flag_RMCU_Check == 0)&&(COUNT_FLAG.Flag_FLT_Check == 0)) // 100
					{
						if(ScreenIndex == SCREEN_STATE_MENU_EQUIPMENT_MACHINEINFO_ECM_TOP)
							DisplayList(1);
						else if(ScreenIndex == SCREEN_STATE_MENU_EQUIPMENT_MACHINEINFO_CLUSTER_TOP)
							DisplayList(2);
						else
							DisplayList(3);

					}
					else if((COUNT_FLAG.Flag_ECM_Check == 0)&&(COUNT_FLAG.Flag_RMCU_Check == 1)&&(COUNT_FLAG.Flag_FLT_Check == 1)) // 011
					{

						if(ScreenIndex != SCREEN_STATE_MENU_EQUIPMENT_MACHINEINFO_FSCU_TOP)
						{
							if(OldScreenIndex == SCREEN_STATE_MENU_EQUIPMENT_MACHINEINFO_FSCU_TOP)
							{
								SetListString_3(StringMachine[1], StringMachine[2], StringMachine[3] );
								DisplayListPage(1,2);
								if(ScreenIndex == SCREEN_STATE_MENU_EQUIPMENT_MACHINEINFO_CLUSTER_TOP)
									DisplayListAll(1);
								else if(ScreenIndex == SCREEN_STATE_MENU_EQUIPMENT_MACHINEINFO_RMCU_TOP)
									DisplayListAll(2);
								else
									DisplayListAll(3);
								DrawMenuFull();

							}
							else
							{
								if(ScreenIndex == SCREEN_STATE_MENU_EQUIPMENT_MACHINEINFO_CLUSTER_TOP)
									DisplayList(1);
								else if(ScreenIndex == SCREEN_STATE_MENU_EQUIPMENT_MACHINEINFO_RMCU_TOP)
									DisplayList(2);
								else
									DisplayList(3);
							}
						}
						else
						{							
							SetListString_1(StringMachine[4]);
							DisplayListPage(2,2);
							DisplayListAll(1);
							DrawMenuFull();
						}

					}
					else if((COUNT_FLAG.Flag_ECM_Check == 0)&&(COUNT_FLAG.Flag_RMCU_Check == 1)&&(COUNT_FLAG.Flag_FLT_Check == 0)) // 010
					{

						if(ScreenIndex == SCREEN_STATE_MENU_EQUIPMENT_MACHINEINFO_CLUSTER_TOP)
							DisplayList(1);
						else if(ScreenIndex == SCREEN_STATE_MENU_EQUIPMENT_MACHINEINFO_RMCU_TOP)
							DisplayList(2);
						else
							DisplayList(3);


					}
					else if((COUNT_FLAG.Flag_ECM_Check == 0)&&(COUNT_FLAG.Flag_RMCU_Check == 0)&&(COUNT_FLAG.Flag_FLT_Check == 1)) // 001
					{

						if(ScreenIndex == SCREEN_STATE_MENU_EQUIPMENT_MACHINEINFO_CLUSTER_TOP)
							DisplayList(1);
						else if(ScreenIndex == SCREEN_STATE_MENU_EQUIPMENT_MACHINEINFO_FLT_TOP)
							DisplayList(2);
						else
							DisplayList(3);

					}	
					else // 000
					{
						if(ScreenIndex == SCREEN_STATE_MENU_EQUIPMENT_MACHINEINFO_CLUSTER_TOP)
							DisplayList(1);
						else
							DisplayList(2);	
					}				
					//--, 220211 ysm, FINGERTIP		
				}
				else
				{
					if((COUNT_FLAG.Flag_ECM_Check == 1)&&(COUNT_FLAG.Flag_RMCU_Check == 1)&&(COUNT_FLAG.Flag_FLT_Check == 1)) // 111
					{
					
						if(ScreenIndex != SCREEN_STATE_MENU_EQUIPMENT_MACHINEINFO_FLT_TOP)
						{				
							if(OldScreenIndex == SCREEN_STATE_MENU_EQUIPMENT_MACHINEINFO_FLT_TOP)
							{
								SetListString_3(StringMachine[0], StringMachine[1], StringMachine[2] );
								DisplayListPage(1,2);
								if(ScreenIndex == SCREEN_STATE_MENU_EQUIPMENT_MACHINEINFO_ECM_TOP)
									DisplayListAll(1);
								else if(ScreenIndex == SCREEN_STATE_MENU_EQUIPMENT_MACHINEINFO_CLUSTER_TOP)
									DisplayListAll(2);
								else
									DisplayListAll(3);
								DrawMenuFull();
							}
							else
							{						
								if(ScreenIndex == SCREEN_STATE_MENU_EQUIPMENT_MACHINEINFO_ECM_TOP)
									DisplayList(1);
								else if(ScreenIndex == SCREEN_STATE_MENU_EQUIPMENT_MACHINEINFO_CLUSTER_TOP)
									DisplayList(2);
								else
									DisplayList(3);
							}
						}
						else
						{
							SetListString_1(StringMachine[3]);
							DisplayListPage(2,2);
							DisplayListAll(1);
							DrawMenuFull();
						}
					
					}
					else if((COUNT_FLAG.Flag_ECM_Check == 1)&&(COUNT_FLAG.Flag_RMCU_Check == 1)&&(COUNT_FLAG.Flag_FLT_Check == 0)) // 110
					{
						if(ScreenIndex == SCREEN_STATE_MENU_EQUIPMENT_MACHINEINFO_ECM_TOP)
							DisplayList(1);
						else if(ScreenIndex == SCREEN_STATE_MENU_EQUIPMENT_MACHINEINFO_CLUSTER_TOP)
							DisplayList(2);
						else
							DisplayList(3);
					
					}
					else if((COUNT_FLAG.Flag_ECM_Check == 1)&&(COUNT_FLAG.Flag_RMCU_Check == 0)&&(COUNT_FLAG.Flag_FLT_Check == 1)) // 101
					{
						if(ScreenIndex == SCREEN_STATE_MENU_EQUIPMENT_MACHINEINFO_ECM_TOP)
							DisplayList(1);
						else if(ScreenIndex == SCREEN_STATE_MENU_EQUIPMENT_MACHINEINFO_CLUSTER_TOP)
							DisplayList(2);
						else
							DisplayList(3);
					
					
					}
					else if((COUNT_FLAG.Flag_ECM_Check == 1)&&(COUNT_FLAG.Flag_RMCU_Check == 0)&&(COUNT_FLAG.Flag_FLT_Check == 0)) // 100
					{
						if(ScreenIndex == SCREEN_STATE_MENU_EQUIPMENT_MACHINEINFO_ECM_TOP)
							DisplayList(1);
						else
							DisplayList(2);
					
					
					}
					else if((COUNT_FLAG.Flag_ECM_Check == 0)&&(COUNT_FLAG.Flag_RMCU_Check == 1)&&(COUNT_FLAG.Flag_FLT_Check == 1)) // 011
					{
					
						if(ScreenIndex == SCREEN_STATE_MENU_EQUIPMENT_MACHINEINFO_CLUSTER_TOP)
							DisplayList(1);
						else if(ScreenIndex == SCREEN_STATE_MENU_EQUIPMENT_MACHINEINFO_RMCU_TOP)
							DisplayList(2);
						else
							DisplayList(3);
					
					
					}
					else if((COUNT_FLAG.Flag_ECM_Check == 0)&&(COUNT_FLAG.Flag_RMCU_Check == 1)&&(COUNT_FLAG.Flag_FLT_Check == 0)) // 010
					{
					
						if(ScreenIndex == SCREEN_STATE_MENU_EQUIPMENT_MACHINEINFO_CLUSTER_TOP)
							DisplayList(1);
						else
							DisplayList(2);
					
					
					}
					else if((COUNT_FLAG.Flag_ECM_Check == 0)&&(COUNT_FLAG.Flag_RMCU_Check == 0)&&(COUNT_FLAG.Flag_FLT_Check == 1)) // 001
					{
					
						if(ScreenIndex == SCREEN_STATE_MENU_EQUIPMENT_MACHINEINFO_CLUSTER_TOP)
							DisplayList(1);
						else
							DisplayList(2);
					
					
					}	
					else // 000
					{
						DisplayList(1); 					
					}				
					//--, 220211 ysm, FINGERTIP 	

				}
				break;

		}
		
	}
}

void DisplayFLTValveSet()
{

	UCHAR buf[10];
	USHORT covbuf[10];

	USHORT	min_open[]={'M','i','n',' ','O','p','e','n',0x00 ,0x00};
	USHORT	max_open[]={'M','a','x',' ','O','p','e','n',0x00 ,0x00};
	USHORT	open_delay[]={'O','p','e','n',' ','D','e','l','a','y',0x00 ,0x00};
	USHORT	close_delay[]={'C','l','o','s','e',' ','D','e','l','a','y',0x00 ,0x00};

	

	USHORT page_num1[] = {'1','/','4',0x00,0x00};
	USHORT page_num2[] = {'2','/','4',0x00,0x00};
	USHORT page_num3[] = {'3','/','4',0x00,0x00};
	USHORT page_num4[] = {'4','/','4',0x00,0x00};

	USHORT current_value;


		if(OldScreenIndex != ScreenIndex)
		{
			DisplayMenuBackgroundBG();
			OldSubCurserIndex = 0xff;
			CurserIndex = CURSER_NONE;

			Old_Control_FLT_DeadZone_Voltage = 0xff;

			DrawMenuFull();
			
		}

		
		if(COUNT_FLAG.Flag_FLT_Step == 2)
		{

			if(OldSubCurserIndex != SubCurserIndex)
			{
				OldSubCurserIndex = SubCurserIndex;

				DisplayMenuBackgroundBG();
				
				memset((&buf), 0x0, sizeof( buf));
				memset((&covbuf), 0x0, sizeof( covbuf));

				if(SubCurserIndex == 1)
				{					
					GUI_DisplayMultiStringAt(&page_num2[0], 160, 50, 200, 0, COLOR_MAIN_ORANGE, 23);
					GUI_DisplayMultiStringAt(&max_open[0], 20, 80, 200, 0, COLOR_YELLOW, 23);		

					current_value = Old_Control_FLT_Valve_Value = Control_FLT_Valve_Value = Temp_FLT_Valve_Value[1];
					sprintf ( (char*)buf, "%d mA",current_value);
				
				}
				else if(SubCurserIndex == 2)
				{

					GUI_DisplayMultiStringAt(&page_num3[0], 160, 50, 200, 0, COLOR_MAIN_ORANGE, 23);
					GUI_DisplayMultiStringAt(&open_delay[0], 20, 80, 200, 0, COLOR_YELLOW, 23);		

					current_value = Old_Control_FLT_Valve_Value = Control_FLT_Valve_Value= Temp_FLT_Valve_Value[2];
					sprintf ( (char*)buf, "%d ms",current_value);
				}
				else if(SubCurserIndex == 3)
				{
					GUI_DisplayMultiStringAt(&page_num4[0], 160, 50, 200, 0, COLOR_MAIN_ORANGE, 23);
					GUI_DisplayMultiStringAt(&close_delay[0], 20, 80, 200, 0, COLOR_YELLOW, 23);

					current_value = Old_Control_FLT_Valve_Value = Control_FLT_Valve_Value = Temp_FLT_Valve_Value[3];
					sprintf ( (char*)buf, "%d ms",current_value);

				}
				else
				{

					GUI_DisplayMultiStringAt(&page_num1[0], 160, 50, 200, 0, COLOR_MAIN_ORANGE, 23);
					GUI_DisplayMultiStringAt(&min_open[0], 20, 80, 200, 0, COLOR_YELLOW, 23);	

					current_value = Old_Control_FLT_Valve_Value = Control_FLT_Valve_Value = Temp_FLT_Valve_Value[0];
					sprintf ( (char*)buf, "%d mA",current_value);
				
				}
				
				String_Func(covbuf, buf);
				GUI_DisplayMultiStringAt(&covbuf[0], 85, 130, 200, 0, COLOR_WHITE, 23);

				LCD_Draw_Color(10, 125, 30, 30, (CurserIndex == CURSER_UP_LEFT)?COLOR_MENU_ORANGE:COLOR_MENU_GRAY);
				PCXtoBMP_16bit(20, 133, 10, 15, (CurserIndex == CURSER_UP_LEFT)?FL_Image.menu_arrow_setting_select_2:FL_Image.menu_arrow_nor_2);

				LCD_Draw_Color(180, 125, 30, 30, (CurserIndex == CURSER_DOWN_RIGHT)?COLOR_MENU_ORANGE:COLOR_MENU_GRAY);
				PCXtoBMP_16bit(187, 133, 10, 15, (CurserIndex == CURSER_DOWN_RIGHT)?FL_Image.menu_arrow_setting_select:FL_Image.menu_arrow_nor);
			
			}	


			if(Old_Control_FLT_Valve_Value != Control_FLT_Valve_Value)
			{
				current_value = Old_Control_FLT_Valve_Value = Control_FLT_Valve_Value;

				memset((&buf), 0x0, sizeof( buf));
				memset((&covbuf), 0x0, sizeof( covbuf));

				if((SubCurserIndex == 0)||(SubCurserIndex == 1))
					sprintf ( (char*)buf, "%d mA",current_value);	
				else
					sprintf ( (char*)buf, "%d ms",current_value);	

				LCD_Draw_Color(40, 125, 140, 30, COLOR_BLACK );
				
				String_Func(covbuf, buf);
				GUI_DisplayMultiStringAt(&covbuf[0], 85, 130, 200, 0, COLOR_WHITE, 23);

			}

			if(OldCurserIndex != CurserIndex)
			{
				LCD_Draw_Color(10, 125, 30, 30, (CurserIndex == CURSER_UP_LEFT)?COLOR_MENU_ORANGE:COLOR_MENU_GRAY);
				PCXtoBMP_16bit(20, 133, 10, 15, (CurserIndex == CURSER_UP_LEFT)?FL_Image.menu_arrow_setting_select_2:FL_Image.menu_arrow_nor_2);

				LCD_Draw_Color(180, 125, 30, 30, (CurserIndex == CURSER_DOWN_RIGHT)?COLOR_MENU_ORANGE:COLOR_MENU_GRAY);
				PCXtoBMP_16bit(187, 133, 10, 15, (CurserIndex == CURSER_DOWN_RIGHT)?FL_Image.menu_arrow_setting_select:FL_Image.menu_arrow_nor);

				OldCurserIndex = CurserIndex;
			}
		}
		else
		{
			GUI_DisplayMultiStringAt(&FLT_loading[0], 60, 90, 200, 0, COLOR_WHITE, 23);
		}


}

void DisplayFLTDeadZoneSet()
{

	UCHAR buf[10];
	USHORT covbuf[10];

	USHORT	upper_zone[]={'U','p','p','e','r',0x00 ,0x00};
	USHORT	lower_zone[]={'L','o','w','e','r',0x00 ,0x00};
	USHORT	voltage[]={'V','o','l','t','a','g','e',0x00 ,0x00};

	USHORT upper_voltage;
	USHORT lower_voltage;
	USHORT current_voltage;


		if(OldScreenIndex != ScreenIndex)
		{
			DisplayMenuBackgroundBG();
			OldSubCurserIndex = 0xff;
			CurserIndex = CURSER_NONE;

			Old_Control_FLT_DeadZone_Voltage = 0xff;
			FLT_Step_Old = 0xff;	
			DrawMenuFull();		
			
		}


		if(COUNT_FLAG.Flag_FLT_Step == 2)
		{
			if(FLT_Step_Old != COUNT_FLAG.Flag_FLT_Step)
			{
				FLT_Step_Old = COUNT_FLAG.Flag_FLT_Step;				

				DisplayMenuBackgroundBG();

				GUI_DisplayMultiStringAt(&upper_zone[0], 20, 50, 200, 0, COLOR_YELLOW, 23);
				GUI_DisplayMultiStringAt(&lower_zone[0], 135, 50, 200, 0, COLOR_YELLOW, 23);

				GUI_DisplayMultiStringAt(&voltage[0], 70, 115, 200, 0, COLOR_YELLOW, 23);				

			}		

			if(OldSubCurserIndex != SubCurserIndex)
			{
				OldSubCurserIndex = SubCurserIndex;

				if(SubCurserIndex == 1)
				{				
					LCD_Draw_Color(15, 75, 80, 30, COLOR_MENU_LIGHTGRAY );
					LCD_Draw_Color(130, 75, 80, 30, COLOR_MENU_ORANGE );	
				
				}
				else
				{				
					LCD_Draw_Color(15, 75, 80, 30, COLOR_MENU_ORANGE );
					LCD_Draw_Color(130, 75, 80, 30, COLOR_MENU_LIGHTGRAY );
				
				}

				upper_voltage = Temp_FLT_DeadZone_Voltage[0];
				lower_voltage = Temp_FLT_DeadZone_Voltage[1];

				memset((&buf), 0x0, sizeof( buf));
				memset((&covbuf), 0x0, sizeof( covbuf));

				sprintf ( (char*)buf, "0.%d%d V",upper_voltage/10, upper_voltage%10);		
				String_Func(covbuf, buf);
				GUI_DisplayMultiStringAt(&covbuf[0], 20, 80, 200, 0, COLOR_WHITE, 23);
				
				memset((&buf), 0x0, sizeof( buf));
				memset((&covbuf), 0x0, sizeof( covbuf));
				
				sprintf ( (char*)buf, "0.%d%d V",lower_voltage/10, lower_voltage%10);
				String_Func(covbuf, buf);
				GUI_DisplayMultiStringAt(&covbuf[0], 135, 80, 200, 0, COLOR_WHITE, 23);

				
			}	


			if(Old_Control_FLT_DeadZone_Voltage != Control_FLT_DeadZone_Voltage)
			{
				if(Control_FLT_DeadZone_Voltage < 20)
					Control_FLT_DeadZone_Voltage = 20;
				else if(Control_FLT_DeadZone_Voltage > 50)
					Control_FLT_DeadZone_Voltage = 50;
			
				current_voltage = Old_Control_FLT_DeadZone_Voltage = Control_FLT_DeadZone_Voltage;

				memset((&buf), 0x0, sizeof( buf));
				memset((&covbuf), 0x0, sizeof( covbuf));

				sprintf ( (char*)buf, "0.%d%d V",current_voltage/10, current_voltage%10);	
				String_Func(covbuf, buf);

				LCD_Draw_Color(60, 140, 100, 30, COLOR_MENU_GRAY );
				GUI_DisplayMultiStringAt(&covbuf[0], 78, 145, 200, 0, COLOR_WHITE, 23);

			}

			if(OldCurserIndex != CurserIndex)
			{
				LCD_Draw_Color(30, 140, 30, 30, (CurserIndex == CURSER_UP_LEFT)?COLOR_MENU_ORANGE:COLOR_MENU_GRAY);
				PCXtoBMP_16bit(39, 147, 10, 15, (CurserIndex == CURSER_UP_LEFT)?FL_Image.menu_arrow_setting_select_2:FL_Image.menu_arrow_nor_2);

				LCD_Draw_Color(160, 140, 30, 30, (CurserIndex == CURSER_DOWN_RIGHT)?COLOR_MENU_ORANGE:COLOR_MENU_GRAY);
				PCXtoBMP_16bit(169, 147, 10, 15, (CurserIndex == CURSER_DOWN_RIGHT)?FL_Image.menu_arrow_setting_select:FL_Image.menu_arrow_nor);

				OldCurserIndex = CurserIndex;
			}
	
		}
		else
		{
			GUI_DisplayMultiStringAt(&FLT_loading[0], 60, 90, 200, 0, COLOR_WHITE, 23);
		}

}


void DisplayFLTLeverPositionSet()
{

	UCHAR buf[10];
	USHORT covbuf[10];

	USHORT	min[]={'M','i','n',0x00 ,0x00};
	USHORT	mid[]={'M','i','d',0x00 ,0x00};
	USHORT	max[]={'M','a','x',0x00 ,0x00};
	USHORT	voltage[]={'V','o','l','t','a','g','e',0x00 ,0x00};

	USHORT min_voltage,mid_voltage,max_voltage;
	USHORT current_voltage;

		if(OldScreenIndex != ScreenIndex)
		{
			DisplayMenuBackgroundBG();
			OldCurserIndex = 0xff;		
			Old_Control_FLT_Lever_Voltage = 0xffff;
			FLT_Step_Old = 0xff;
			
			DrawMenuFull();
		}

		if(COUNT_FLAG.Flag_FLT_Step == 2)
		{

			if(FLT_Step_Old != COUNT_FLAG.Flag_FLT_Step)
			{
				FLT_Step_Old = COUNT_FLAG.Flag_FLT_Step;

				DisplayMenuBackgroundBG();

				GUI_DisplayMultiStringAt(&min[0], 20, 50, 200, 0, COLOR_YELLOW, 23);
				GUI_DisplayMultiStringAt(&mid[0], 90, 50, 200, 0, COLOR_YELLOW, 23);
				GUI_DisplayMultiStringAt(&max[0], 160, 50, 200, 0, COLOR_YELLOW, 23);
				GUI_DisplayMultiStringAt(&voltage[0], 70, 115, 200, 0, COLOR_YELLOW, 23);

			}				

			if(OldCurserIndex != CurserIndex)
			{
				OldCurserIndex = CurserIndex;

				if(CurserIndex == 1)
				{					
					LCD_Draw_Color(1, 75, 70, 30, COLOR_MENU_LIGHTGRAY );
					LCD_Draw_Color(75, 75, 70, 30, COLOR_MENU_ORANGE );
					LCD_Draw_Color(149, 75, 70, 30, COLOR_MENU_LIGHTGRAY );
				
				}
				else if(CurserIndex == 2)
				{
					LCD_Draw_Color(1, 75, 70, 30, COLOR_MENU_LIGHTGRAY );
					LCD_Draw_Color(75, 75, 70, 30, COLOR_MENU_LIGHTGRAY );
					LCD_Draw_Color(149, 75, 70, 30, COLOR_MENU_ORANGE );
				}
				else
				{
					LCD_Draw_Color(1, 75, 70, 30, COLOR_MENU_ORANGE );
					LCD_Draw_Color(75, 75, 70, 30, COLOR_MENU_LIGHTGRAY );
					LCD_Draw_Color(149, 75, 70, 30, COLOR_MENU_LIGHTGRAY );
				
				}
				
				min_voltage = Temp_FLT_Lever_Voltage[0];
				mid_voltage = Temp_FLT_Lever_Voltage[1];
				max_voltage = Temp_FLT_Lever_Voltage[2];
				
				memset((&buf), 0x0, sizeof( buf));
				memset((&covbuf), 0x0, sizeof( covbuf));
				
				sprintf ( (char*)buf, "%d.%d%d V",min_voltage/100,(min_voltage/10)%10, min_voltage%10);		
				String_Func(covbuf, buf);
				GUI_DisplayMultiStringAt(&covbuf[0], 3, 80, 200, 0, COLOR_WHITE, 23);
				
				memset((&buf), 0x0, sizeof( buf));
				memset((&covbuf), 0x0, sizeof( covbuf));
				
				sprintf ( (char*)buf, "%d.%d%d V",mid_voltage/100,(mid_voltage/10)%10, mid_voltage%10);	
				String_Func(covbuf, buf);
				GUI_DisplayMultiStringAt(&covbuf[0], 77, 80, 200, 0, COLOR_WHITE, 23);
				
				
				memset((&buf), 0x0, sizeof( buf));
				memset((&covbuf), 0x0, sizeof( covbuf));
				
				sprintf ( (char*)buf, "%d.%d%d V",max_voltage/100,(max_voltage/10)%10, max_voltage%10);	
				String_Func(covbuf, buf);
				GUI_DisplayMultiStringAt(&covbuf[0], 151, 80, 200, 0, COLOR_WHITE, 23);		

				
			}	


			if(Old_Control_FLT_Lever_Voltage != Control_FLT_Lever_Voltage)
			{
				current_voltage = Old_Control_FLT_Lever_Voltage = Control_FLT_Lever_Voltage;

				memset((&buf), 0x0, sizeof( buf));
				memset((&covbuf), 0x0, sizeof( covbuf));

				sprintf ( (char*)buf, "%d.%d%d V",current_voltage/100,(current_voltage/10)%10, current_voltage%10);
				String_Func(covbuf, buf);
				LCD_Draw_Color(60, 140, 100, 30, COLOR_MENU_GRAY );
				GUI_DisplayMultiStringAt(&covbuf[0], 78, 145, 200, 0, COLOR_WHITE, 23);	

			}
		}
		else
		{
			GUI_DisplayMultiStringAt(&FLT_loading[0], 60, 90, 200, 0, COLOR_WHITE, 23);
		}

}

void DisplayFLTPopUp()
{

	if(OldScreenIndex != ScreenIndex)
	{
		DisplayMenuPopup();

		GUI_DisplayMultiStringAt((USHORT*)*StringSentence[49], 11, 48, 200, 0, COLOR_WHITE, 23);		
		DrawMenuFull();		
		
	}

}

void DisplayFLTSet()
{
	switch(ScreenIndex)
	{
		case SCREEN_STATE_MENU_EQUIPMENT_FINGERTIP_LEVER_POSITION_TOP:					
		case SCREEN_STATE_MENU_EQUIPMENT_FINGERTIP_LEVER_DEADZONE_TOP:
		case SCREEN_STATE_MENU_EQUIPMENT_FINGERTIP_VALVE_SETTING_TOP:			
			if((OldScreenIndex != SCREEN_STATE_MENU_EQUIPMENT_FINGERTIP_LEVER_POSITION_TOP)
				&& (OldScreenIndex != SCREEN_STATE_MENU_EQUIPMENT_FINGERTIP_LEVER_DEADZONE_TOP)
				&& (OldScreenIndex != SCREEN_STATE_MENU_EQUIPMENT_FINGERTIP_VALVE_SETTING_TOP))
			{
				SetListString3(216, 217, 218);
			
				if(ScreenIndex == SCREEN_STATE_MENU_EQUIPMENT_FINGERTIP_LEVER_POSITION_TOP)
					DisplayListAll(1);
				else if(ScreenIndex == SCREEN_STATE_MENU_EQUIPMENT_FINGERTIP_LEVER_DEADZONE_TOP)
					DisplayListAll(2);
				else
					DisplayListAll(3);
				
				DrawMenuFull();
			}
			else if(OldScreenIndex != ScreenIndex)
			{
				if(ScreenIndex == SCREEN_STATE_MENU_EQUIPMENT_FINGERTIP_LEVER_POSITION_TOP)
					DisplayList(1);
				else if(ScreenIndex == SCREEN_STATE_MENU_EQUIPMENT_FINGERTIP_LEVER_DEADZONE_TOP)
					DisplayList(2);
				else
					DisplayList(3);
			}
			break;

		case SCREEN_STATE_MENU_EQUIPMENT_FINGERTIP_LEVER_POSITION_LIFT_TOP:					
		case SCREEN_STATE_MENU_EQUIPMENT_FINGERTIP_LEVER_POSITION_TILT_TOP:
		case SCREEN_STATE_MENU_EQUIPMENT_FINGERTIP_LEVER_POSITION_AUX1_TOP:			
			if((OldScreenIndex != SCREEN_STATE_MENU_EQUIPMENT_FINGERTIP_LEVER_POSITION_LIFT_TOP)
				&& (OldScreenIndex != SCREEN_STATE_MENU_EQUIPMENT_FINGERTIP_LEVER_POSITION_TILT_TOP)
				&& (OldScreenIndex != SCREEN_STATE_MENU_EQUIPMENT_FINGERTIP_LEVER_POSITION_AUX1_TOP))
			{
				SetListString3(219, 220, 221);
			
				if(ScreenIndex == SCREEN_STATE_MENU_EQUIPMENT_FINGERTIP_LEVER_POSITION_LIFT_TOP)
					DisplayListAll(1);
				else if(ScreenIndex == SCREEN_STATE_MENU_EQUIPMENT_FINGERTIP_LEVER_POSITION_TILT_TOP)
					DisplayListAll(2);
				else
					DisplayListAll(3);

				DisplayListPage(1,2);				
				DrawMenuFull();
			}
			else if(OldScreenIndex != ScreenIndex)
			{
				if(ScreenIndex == SCREEN_STATE_MENU_EQUIPMENT_FINGERTIP_LEVER_POSITION_LIFT_TOP)
					DisplayList(1);
				else if(ScreenIndex == SCREEN_STATE_MENU_EQUIPMENT_FINGERTIP_LEVER_POSITION_TILT_TOP)
					DisplayList(2);
				else
					DisplayList(3);
			}
			break;
		case SCREEN_STATE_MENU_EQUIPMENT_FINGERTIP_LEVER_POSITION_AUX2_TOP: 				
			if(OldScreenIndex != SCREEN_STATE_MENU_EQUIPMENT_FINGERTIP_LEVER_POSITION_AUX2_TOP)
			{
				SetListString1(222);
				
				DisplayListAll(1);		

				DisplayListPage(2,2);
				DrawMenuFull();
			}
			else if(OldScreenIndex != ScreenIndex)
			{
				DisplayList(1);
			}
			break;

		case SCREEN_STATE_MENU_EQUIPMENT_FINGERTIP_LEVER_DEADZONE_LIFT_TOP:					
		case SCREEN_STATE_MENU_EQUIPMENT_FINGERTIP_LEVER_DEADZONE_TILT_TOP:
		case SCREEN_STATE_MENU_EQUIPMENT_FINGERTIP_LEVER_DEADZONE_AUX1_TOP:			
			if((OldScreenIndex != SCREEN_STATE_MENU_EQUIPMENT_FINGERTIP_LEVER_DEADZONE_LIFT_TOP)
				&& (OldScreenIndex != SCREEN_STATE_MENU_EQUIPMENT_FINGERTIP_LEVER_DEADZONE_TILT_TOP)
				&& (OldScreenIndex != SCREEN_STATE_MENU_EQUIPMENT_FINGERTIP_LEVER_DEADZONE_AUX1_TOP))
			{
				SetListString3(219, 220, 221);
			
				if(ScreenIndex == SCREEN_STATE_MENU_EQUIPMENT_FINGERTIP_LEVER_DEADZONE_LIFT_TOP)
					DisplayListAll(1);
				else if(ScreenIndex == SCREEN_STATE_MENU_EQUIPMENT_FINGERTIP_LEVER_DEADZONE_TILT_TOP)
					DisplayListAll(2);
				else
					DisplayListAll(3);

				DisplayListPage(1,2);				
				DrawMenuFull();
			}
			else if(OldScreenIndex != ScreenIndex)
			{
				if(ScreenIndex == SCREEN_STATE_MENU_EQUIPMENT_FINGERTIP_LEVER_DEADZONE_LIFT_TOP)
					DisplayList(1);
				else if(ScreenIndex == SCREEN_STATE_MENU_EQUIPMENT_FINGERTIP_LEVER_DEADZONE_TILT_TOP)
					DisplayList(2);
				else
					DisplayList(3);
			}
			break;
		case SCREEN_STATE_MENU_EQUIPMENT_FINGERTIP_LEVER_DEADZONE_AUX2_TOP: 				
			if(OldScreenIndex != SCREEN_STATE_MENU_EQUIPMENT_FINGERTIP_LEVER_DEADZONE_AUX2_TOP)
			{
				SetListString1(222);
				
				DisplayListAll(1);		

				DisplayListPage(2,2);
				DrawMenuFull();
			}
			else if(OldScreenIndex != ScreenIndex)
			{
				DisplayList(1);
			}
			break;

		case SCREEN_STATE_MENU_EQUIPMENT_FINGERTIP_VALVE_SETTING_LIFT_TOP:					
		case SCREEN_STATE_MENU_EQUIPMENT_FINGERTIP_VALVE_SETTING_TILT_TOP:
		case SCREEN_STATE_MENU_EQUIPMENT_FINGERTIP_VALVE_SETTING_AUX1_TOP:			
			if((OldScreenIndex != SCREEN_STATE_MENU_EQUIPMENT_FINGERTIP_VALVE_SETTING_LIFT_TOP)
				&& (OldScreenIndex != SCREEN_STATE_MENU_EQUIPMENT_FINGERTIP_VALVE_SETTING_TILT_TOP)
				&& (OldScreenIndex != SCREEN_STATE_MENU_EQUIPMENT_FINGERTIP_VALVE_SETTING_AUX1_TOP))
			{
				SetListString3(219, 220, 221);
			
				if(ScreenIndex == SCREEN_STATE_MENU_EQUIPMENT_FINGERTIP_VALVE_SETTING_LIFT_TOP)
					DisplayListAll(1);
				else if(ScreenIndex == SCREEN_STATE_MENU_EQUIPMENT_FINGERTIP_VALVE_SETTING_TILT_TOP)
					DisplayListAll(2);
				else
					DisplayListAll(3);

				DisplayListPage(1,2);				
				DrawMenuFull();
			}
			else if(OldScreenIndex != ScreenIndex)
			{
				if(ScreenIndex == SCREEN_STATE_MENU_EQUIPMENT_FINGERTIP_VALVE_SETTING_LIFT_TOP)
					DisplayList(1);
				else if(ScreenIndex == SCREEN_STATE_MENU_EQUIPMENT_FINGERTIP_VALVE_SETTING_TILT_TOP)
					DisplayList(2);
				else
					DisplayList(3);
			}
			break;
		case SCREEN_STATE_MENU_EQUIPMENT_FINGERTIP_VALVE_SETTING_AUX2_TOP: 				
			if(OldScreenIndex != SCREEN_STATE_MENU_EQUIPMENT_FINGERTIP_VALVE_SETTING_AUX2_TOP)
			{
				SetListString1(222);
				
				DisplayListAll(1);		

				DisplayListPage(2,2);
				DrawMenuFull();
			}
			else if(OldScreenIndex != ScreenIndex)
			{
				DisplayList(1);
			}
			break;

			

		case SCREEN_STATE_MENU_EQUIPMENT_FINGERTIP_VALVE_SETTING_LIFT_UP_TOP:					
		case SCREEN_STATE_MENU_EQUIPMENT_FINGERTIP_VALVE_SETTING_LIFT_DOWN_TOP:		
			if((OldScreenIndex != SCREEN_STATE_MENU_EQUIPMENT_FINGERTIP_VALVE_SETTING_LIFT_UP_TOP)
				&& (OldScreenIndex != SCREEN_STATE_MENU_EQUIPMENT_FINGERTIP_VALVE_SETTING_LIFT_DOWN_TOP))
			{
				SetListString2(223, 224);
			
				if(ScreenIndex == SCREEN_STATE_MENU_EQUIPMENT_FINGERTIP_VALVE_SETTING_LIFT_UP_TOP)
					DisplayListAll(1);
				else
					DisplayListAll(2);
		
				DrawMenuFull();
			}
			else if(OldScreenIndex != ScreenIndex)
			{
				if(ScreenIndex == SCREEN_STATE_MENU_EQUIPMENT_FINGERTIP_VALVE_SETTING_LIFT_UP_TOP)
					DisplayList(1);
				else
					DisplayList(2);
			}
			break;
		case SCREEN_STATE_MENU_EQUIPMENT_FINGERTIP_VALVE_SETTING_TILT_A_TOP:					
		case SCREEN_STATE_MENU_EQUIPMENT_FINGERTIP_VALVE_SETTING_TILT_B_TOP: 	
			if((OldScreenIndex != SCREEN_STATE_MENU_EQUIPMENT_FINGERTIP_VALVE_SETTING_TILT_A_TOP)
				&& (OldScreenIndex != SCREEN_STATE_MENU_EQUIPMENT_FINGERTIP_VALVE_SETTING_TILT_B_TOP))
			{
				SetListString2(225, 226);
			
				if(ScreenIndex == SCREEN_STATE_MENU_EQUIPMENT_FINGERTIP_VALVE_SETTING_TILT_A_TOP)
					DisplayListAll(1);
				else
					DisplayListAll(2);
		
				DrawMenuFull();
			}
			else if(OldScreenIndex != ScreenIndex)
			{
				if(ScreenIndex == SCREEN_STATE_MENU_EQUIPMENT_FINGERTIP_VALVE_SETTING_TILT_A_TOP)
					DisplayList(1);
				else
					DisplayList(2);
			}
			break;
		case SCREEN_STATE_MENU_EQUIPMENT_FINGERTIP_VALVE_SETTING_AUX1_A_TOP:					
		case SCREEN_STATE_MENU_EQUIPMENT_FINGERTIP_VALVE_SETTING_AUX1_B_TOP:	
			if((OldScreenIndex != SCREEN_STATE_MENU_EQUIPMENT_FINGERTIP_VALVE_SETTING_AUX1_A_TOP)
				&& (OldScreenIndex != SCREEN_STATE_MENU_EQUIPMENT_FINGERTIP_VALVE_SETTING_AUX1_B_TOP))
			{
				SetListString2(225, 226);
			
				if(ScreenIndex == SCREEN_STATE_MENU_EQUIPMENT_FINGERTIP_VALVE_SETTING_AUX1_A_TOP)
					DisplayListAll(1);
				else
					DisplayListAll(2);
		
				DrawMenuFull();
			}
			else if(OldScreenIndex != ScreenIndex)
			{
				if(ScreenIndex == SCREEN_STATE_MENU_EQUIPMENT_FINGERTIP_VALVE_SETTING_AUX1_A_TOP)
					DisplayList(1);
				else
					DisplayList(2);
			}
			break;
		case SCREEN_STATE_MENU_EQUIPMENT_FINGERTIP_VALVE_SETTING_AUX2_A_TOP:					
		case SCREEN_STATE_MENU_EQUIPMENT_FINGERTIP_VALVE_SETTING_AUX2_B_TOP:	
			if((OldScreenIndex != SCREEN_STATE_MENU_EQUIPMENT_FINGERTIP_VALVE_SETTING_AUX2_A_TOP)
				&& (OldScreenIndex != SCREEN_STATE_MENU_EQUIPMENT_FINGERTIP_VALVE_SETTING_AUX2_B_TOP))
			{
				SetListString2(225, 226);
			
				if(ScreenIndex == SCREEN_STATE_MENU_EQUIPMENT_FINGERTIP_VALVE_SETTING_AUX2_A_TOP)
					DisplayListAll(1);
				else
					DisplayListAll(2);
		
				DrawMenuFull();
			}
			else if(OldScreenIndex != ScreenIndex)
			{
				if(ScreenIndex == SCREEN_STATE_MENU_EQUIPMENT_FINGERTIP_VALVE_SETTING_AUX2_A_TOP)
					DisplayList(1);
				else
					DisplayList(2);
			}
			break;

		case SCREEN_STATE_MENU_EQUIPMENT_FINGERTIP_VALVE_SETTING_LIFT_UP_SET:
		case SCREEN_STATE_MENU_EQUIPMENT_FINGERTIP_VALVE_SETTING_LIFT_DOWN_SET:
		case SCREEN_STATE_MENU_EQUIPMENT_FINGERTIP_VALVE_SETTING_TILT_A_SET:
		case SCREEN_STATE_MENU_EQUIPMENT_FINGERTIP_VALVE_SETTING_TILT_B_SET:
		case SCREEN_STATE_MENU_EQUIPMENT_FINGERTIP_VALVE_SETTING_AUX1_A_SET:
		case SCREEN_STATE_MENU_EQUIPMENT_FINGERTIP_VALVE_SETTING_AUX1_B_SET:
		case SCREEN_STATE_MENU_EQUIPMENT_FINGERTIP_VALVE_SETTING_AUX2_A_SET:
		case SCREEN_STATE_MENU_EQUIPMENT_FINGERTIP_VALVE_SETTING_AUX2_B_SET:
			DisplayFLTValveSet();
			break;
			
		case SCREEN_STATE_MENU_EQUIPMENT_FINGERTIP_LEVER_DEADZONE_LIFT_SET:
		case SCREEN_STATE_MENU_EQUIPMENT_FINGERTIP_LEVER_DEADZONE_TILT_SET:
		case SCREEN_STATE_MENU_EQUIPMENT_FINGERTIP_LEVER_DEADZONE_AUX1_SET:
		case SCREEN_STATE_MENU_EQUIPMENT_FINGERTIP_LEVER_DEADZONE_AUX2_SET:
			DisplayFLTDeadZoneSet();
			break;	

		case SCREEN_STATE_MENU_EQUIPMENT_FINGERTIP_LEVER_POSITION_LIFT_SET:
		case SCREEN_STATE_MENU_EQUIPMENT_FINGERTIP_LEVER_POSITION_TILT_SET:
		case SCREEN_STATE_MENU_EQUIPMENT_FINGERTIP_LEVER_POSITION_AUX1_SET:
		case SCREEN_STATE_MENU_EQUIPMENT_FINGERTIP_LEVER_POSITION_AUX2_SET:
			DisplayFLTLeverPositionSet();
			break;

		case SCREEN_STATE_MENU_EQUIPMENT_FINGERTIP_LEVER_DEADZONE_LIFT_POPUP:	
		case SCREEN_STATE_MENU_EQUIPMENT_FINGERTIP_LEVER_DEADZONE_TILT_POPUP:
		case SCREEN_STATE_MENU_EQUIPMENT_FINGERTIP_LEVER_DEADZONE_AUX1_POPUP:	
		case SCREEN_STATE_MENU_EQUIPMENT_FINGERTIP_LEVER_DEADZONE_AUX2_POPUP:

		case SCREEN_STATE_MENU_EQUIPMENT_FINGERTIP_LEVER_POSITION_LIFT_POPUP:
		case SCREEN_STATE_MENU_EQUIPMENT_FINGERTIP_LEVER_POSITION_TILT_POPUP:
		case SCREEN_STATE_MENU_EQUIPMENT_FINGERTIP_LEVER_POSITION_AUX1_POPUP:
		case SCREEN_STATE_MENU_EQUIPMENT_FINGERTIP_LEVER_POSITION_AUX2_POPUP:	

		case SCREEN_STATE_MENU_EQUIPMENT_FINGERTIP_VALVE_SETTING_LIFT_UP_POPUP:
		case SCREEN_STATE_MENU_EQUIPMENT_FINGERTIP_VALVE_SETTING_LIFT_DOWN_POPUP:
		case SCREEN_STATE_MENU_EQUIPMENT_FINGERTIP_VALVE_SETTING_TILT_A_POPUP:
		case SCREEN_STATE_MENU_EQUIPMENT_FINGERTIP_VALVE_SETTING_TILT_B_POPUP:	
		case SCREEN_STATE_MENU_EQUIPMENT_FINGERTIP_VALVE_SETTING_AUX1_A_POPUP:
		case SCREEN_STATE_MENU_EQUIPMENT_FINGERTIP_VALVE_SETTING_AUX1_B_POPUP:
		case SCREEN_STATE_MENU_EQUIPMENT_FINGERTIP_VALVE_SETTING_AUX2_A_POPUP:
		case SCREEN_STATE_MENU_EQUIPMENT_FINGERTIP_VALVE_SETTING_AUX2_B_POPUP:				
			DisplayFLTPopUp();
			break;

		case SCREEN_STATE_MENU_EQUIPMENT_FINGERTIP_LEVER_DEADZONE_LIFT_COMPLETE: 
		case SCREEN_STATE_MENU_EQUIPMENT_FINGERTIP_LEVER_DEADZONE_TILT_COMPLETE:
		case SCREEN_STATE_MENU_EQUIPMENT_FINGERTIP_LEVER_DEADZONE_AUX1_COMPLETE: 
		case SCREEN_STATE_MENU_EQUIPMENT_FINGERTIP_LEVER_DEADZONE_AUX2_COMPLETE:

		case SCREEN_STATE_MENU_EQUIPMENT_FINGERTIP_LEVER_POSITION_LIFT_COMPLETE: 
		case SCREEN_STATE_MENU_EQUIPMENT_FINGERTIP_LEVER_POSITION_TILT_COMPLETE:
		case SCREEN_STATE_MENU_EQUIPMENT_FINGERTIP_LEVER_POSITION_AUX1_COMPLETE: 
		case SCREEN_STATE_MENU_EQUIPMENT_FINGERTIP_LEVER_POSITION_AUX2_COMPLETE:

		case SCREEN_STATE_MENU_EQUIPMENT_FINGERTIP_VALVE_SETTING_LIFT_UP_COMPLETE:
		case SCREEN_STATE_MENU_EQUIPMENT_FINGERTIP_VALVE_SETTING_LIFT_DOWN_COMPLETE:
		case SCREEN_STATE_MENU_EQUIPMENT_FINGERTIP_VALVE_SETTING_TILT_A_COMPLETE:
		case SCREEN_STATE_MENU_EQUIPMENT_FINGERTIP_VALVE_SETTING_TILT_B_COMPLETE:	
		case SCREEN_STATE_MENU_EQUIPMENT_FINGERTIP_VALVE_SETTING_AUX1_A_COMPLETE:
		case SCREEN_STATE_MENU_EQUIPMENT_FINGERTIP_VALVE_SETTING_AUX1_B_COMPLETE:
		case SCREEN_STATE_MENU_EQUIPMENT_FINGERTIP_VALVE_SETTING_AUX2_A_COMPLETE:
		case SCREEN_STATE_MENU_EQUIPMENT_FINGERTIP_VALVE_SETTING_AUX2_B_COMPLETE:	
			DisplayPopupStringComplete();
			break;
	}
}

