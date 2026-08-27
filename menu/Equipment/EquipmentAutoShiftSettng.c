//#include "FLV_Cluster_APP.h"
//
//unsigned char tempSpeed;
//unsigned char OldtempSpeed;
//
//void DisplaySettingSpeed()
//{
//	UCHAR buf[10];
//	USHORT covbuf[10];
//
//	if(OldScreenIndex != ScreenIndex)
//	{
//		DisplayMenuBackgroundBG();
//		OldCurserIndex = OldtempSpeed = 0xff;
//		CurserIndex = CURSER_NONE;
//	}
//
//	if(OldtempSpeed != tempSpeed)
//	{
//		OldtempSpeed = tempSpeed;
//		
//		memset((&buf), 0x0, sizeof( buf));
//		memset((&covbuf), 0x0, sizeof( covbuf));
//
//		sprintf ( (char*)buf, "%d km/h",OldtempSpeed) ; 
//		String_Func(covbuf, buf);
//
//		LCD_Draw_Color(50, 97, 120, 22, COLOR_BLACK);
//		GUI_DisplayCenterStringAt(&covbuf[0], 0, 97, LCD_WIDTH, COLOR_WHITE, FONTSIZE);	
//	}
//
//	if(OldCurserIndex != CurserIndex)
//	{
//		LCD_Draw_Color(6, 90, 31, 39, (CurserIndex == CURSER_UP_LEFT)?COLOR_MENU_ORANGE:COLOR_MENU_GRAY);
//		PCXtoBMP_16bit(16, 101, 10, 15, (CurserIndex == CURSER_UP_LEFT)?FL_Image.menu_arrow_setting_select_2:FL_Image.menu_arrow_nor_2);
//
//		LCD_Draw_Color(183, 90, 31, 39, (CurserIndex == CURSER_DOWN_RIGHT)?COLOR_MENU_ORANGE:COLOR_MENU_GRAY);
//		PCXtoBMP_16bit(193, 101, 10, 15, (CurserIndex == CURSER_DOWN_RIGHT)?FL_Image.menu_arrow_setting_select:FL_Image.menu_arrow_nor);
//	}
//
//	if(OldScreenIndex != ScreenIndex)
//	{
//		OldCurserIndex = CurserIndex;
//		DrawMenuFull();
//	}
//	else if(OldCurserIndex != CurserIndex)
//	{
//		OldCurserIndex = CurserIndex;
//	}	
//}
//
//void DisplayAutoShiftSetting_MaxSpeedLImit()
//{
//	switch(ScreenIndex)
//	{
//		case SCREEN_STATE_MENU_EQUIPMENT_DCSR_MODE_TOP:						
//		case SCREEN_STATE_MENU_EQUIPMENT_DCSR_SPEED_TOP:						
//		case SCREEN_STATE_MENU_EQUIPMENT_MAXSPEEDLIMIT_FUNC_TOP:	
//		case SCREEN_STATE_MENU_EQUIPMENT_MAXSPEEDLIMIT_SPEED_TOP:					
//			if((OldScreenIndex != SCREEN_STATE_MENU_EQUIPMENT_DCSR_MODE_TOP)
//				&& (OldScreenIndex != SCREEN_STATE_MENU_EQUIPMENT_DCSR_SPEED_TOP)
//				&& (OldScreenIndex != SCREEN_STATE_MENU_EQUIPMENT_MAXSPEEDLIMIT_FUNC_TOP)
//				&& (OldScreenIndex != SCREEN_STATE_MENU_EQUIPMENT_MAXSPEEDLIMIT_SPEED_TOP))
//			{
//				if((ScreenIndex == SCREEN_STATE_MENU_EQUIPMENT_DCSR_MODE_TOP)
//					&& (ScreenIndex == SCREEN_STATE_MENU_EQUIPMENT_DCSR_SPEED_TOP))
//					SetListString2(62, 63);
//				else
//					SetListString2(62, 66);
//				DisplayListAll((ScreenIndex&0x00F00000)>>20);
//				DrawMenuFull();
//			}
//			else if(OldScreenIndex != ScreenIndex)
//			{
//				DisplayList((ScreenIndex&0x00F00000)>>20);
//			}
//			break;
//			
//		case SCREEN_STATE_MENU_EQUIPMENT_DCSR_MODE_SET:		
//		case SCREEN_STATE_MENU_EQUIPMENT_MAXSPEEDLIMIT_FUNC_SET:	
//		case SCREEN_STATE_MENU_EQUIPMENT_HAC_SET:					
//		case SCREEN_STATE_MENU_EQUIPMENT_CLUTCHPROTECTION_SET:
//		case SCREEN_STATE_MENU_EQUIPMENT_ZEROSTART_SET:
//			if(OldScreenIndex != ScreenIndex)
//			{
//				SetSubListString(2, CurserIndex, StringEtc[SIGN_OFF], StringEtc[SIGN_ON], StringEtc[SIGN_NOSTRING], StringEtc[SIGN_NOSTRING], StringEtc[SIGN_NOSTRING], StringEtc[SIGN_NOSTRING]);
//				OldCurserIndex = CurserIndex;
//			}
//			else if(OldCurserIndex != CurserIndex)
//			{
//				DisplaySubDoubleList(CurserIndex);
//			}
//			break;
//		case SCREEN_STATE_MENU_EQUIPMENT_HAC_COMPLETE:							
//		case SCREEN_STATE_MENU_EQUIPMENT_HAC_NOT_COMPLETE:				
//		case SCREEN_STATE_MENU_EQUIPMENT_DCSR_MODE_COMPLETE:				
//		case SCREEN_STATE_MENU_EQUIPMENT_DCSR_MODE_NOT_COMPLETE:		
//		case SCREEN_STATE_MENU_EQUIPMENT_DCSR_SPEED_BLOCKING_COMPLETE:			
//		case SCREEN_STATE_MENU_EQUIPMENT_DCSR_SPEED_RESTORE_COMPLETE:			
//		case SCREEN_STATE_MENU_EQUIPMENT_DCSR_SPEED_BLOCKING_NOT_COMPLETE:			
//		case SCREEN_STATE_MENU_EQUIPMENT_DCSR_SPEED_RESTORE_NOT_COMPLETE:			
//		case SCREEN_STATE_MENU_EQUIPMENT_MAXSPEEDLIMIT_FUNC_COMPLETE:
//		case SCREEN_STATE_MENU_EQUIPMENT_MAXSPEEDLIMIT_SPEED_SET_COMPLETE:
//		case SCREEN_STATE_MENU_EQUIPMENT_MAXSPEEDLIMIT_FUNC_NOT_COMPLETE:
//		case SCREEN_STATE_MENU_EQUIPMENT_MAXSPEEDLIMIT_SPEED_SET_NOT_COMPLETE:
//			DisplayPopupStringComplete();
//			break;
//
//			
//		case SCREEN_STATE_MENU_EQUIPMENT_DCSR_SPEED_BLOCKING_TOP:	
//		case SCREEN_STATE_MENU_EQUIPMENT_DCSR_SPEED_RESTORE_TOP:			
//			if((OldScreenIndex != SCREEN_STATE_MENU_EQUIPMENT_DCSR_SPEED_BLOCKING_TOP)
//				&& (OldScreenIndex != SCREEN_STATE_MENU_EQUIPMENT_DCSR_SPEED_RESTORE_TOP))
//			{
//				SetListString2(64, 65);
//				DisplayListAll((ScreenIndex&0x000F0000)>>16);
//				DrawMenuFull();
//			}
//			else if(OldScreenIndex != ScreenIndex)
//			{
//				DisplayList((ScreenIndex&0x000F0000)>>16);
//			}
//			break;
//			
//		case SCREEN_STATE_MENU_EQUIPMENT_DCSR_SPEED_BLOCKING_SET:	
//		case SCREEN_STATE_MENU_EQUIPMENT_DCSR_SPEED_RESTORE_SET:	
//		case SCREEN_STATE_MENU_EQUIPMENT_MAXSPEEDLIMIT_SPEED_SET:			
//			DisplaySettingSpeed();
//			break;
//	}
//}
//
