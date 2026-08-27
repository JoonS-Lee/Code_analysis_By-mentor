#include "FLV_Cluster_APP.h"

void DisplayWeighSensorInitSetting()
{
	if(OldScreenIndex != ScreenIndex)
	{
		DisplayPopupString1((USHORT*)*StringSentence[89]);
	}
}

void DisplayWeighSensorDisplaySetting()
{
	if(OldScreenIndex != ScreenIndex)
	{
		OldCurserIndex = CurserIndex;

		SetListString2(47, 48);
		DisplaySubListAll(CurserIndex+1);
		DrawMenuFull();
	}
	else if(OldCurserIndex != CurserIndex)
	{
		DisplaySubList(CurserIndex+1);
	}
}

void DisplayWeighSensor()
{
	switch(ScreenIndex)
	{
		case SCREEN_STATE_MENU_EQUIPMENT_WEIGHTSENSOR_CROSSSECTION_TOP:		
		case SCREEN_STATE_MENU_EQUIPMENT_WEIGHTSENSOR_LOADADJUST_TOP:				
		case SCREEN_STATE_MENU_EQUIPMENT_WEIGHTSENSOR_DISPLAYSET_TOP:		
			if((OldScreenIndex != SCREEN_STATE_MENU_EQUIPMENT_WEIGHTSENSOR_CROSSSECTION_TOP)
				&& (OldScreenIndex != SCREEN_STATE_MENU_EQUIPMENT_WEIGHTSENSOR_LOADADJUST_TOP)
				&& (OldScreenIndex != SCREEN_STATE_MENU_EQUIPMENT_WEIGHTSENSOR_DISPLAYSET_TOP))
			{
				SetListString3(33, 34, 35);
				DisplayListPage(1,2);
				DisplayListAll((ScreenIndex&0x00F00000)>>20);
				DrawMenuFull();
			}
			else if(OldScreenIndex != ScreenIndex)
			{
				DisplayList((ScreenIndex&0x00F00000)>>20);
			}
			break;
		case SCREEN_STATE_MENU_EQUIPMENT_WEIGHTSENSOR_BUZZER_TOP:
			if(OldScreenIndex != SCREEN_STATE_MENU_EQUIPMENT_WEIGHTSENSOR_BUZZER_TOP)
			{
				SetListString1(36);
				DisplayListAll(((ScreenIndex&0x00F00000)>>20)-3);
				DisplayListPage(2,2);
				DrawMenuFull();
			}
			else if(OldScreenIndex != ScreenIndex)
			{
				DisplayList(((ScreenIndex&0x00F00000)>>20)-3);
			}
			break;
		case SCREEN_STATE_MENU_EQUIPMENT_WEIGHTSENSOR_CROSSSECTION_INPUT:	
		case SCREEN_STATE_MENU_EQUIPMENT_WEIGHTSENSOR_LOADADJUST_LOAD_INPUT:		
			if(OldScreenIndex != ScreenIndex)
			{
				InitializeDisplayPassword(NUMBER_INFO_DOT);
				DrawMenuFull();
			}
			break;
		case SCREEN_STATE_MENU_EQUIPMENT_WEIGHTSENSOR_LOADADJUST_UNLOAD_TOP:		
		case SCREEN_STATE_MENU_EQUIPMENT_WEIGHTSENSOR_LOADADJUST_LOAD_TOP:		
		case SCREEN_STATE_MENU_EQUIPMENT_WEIGHTSENSOR_LOADADJUST_INITIAL_TOP:		
			if((OldScreenIndex != SCREEN_STATE_MENU_EQUIPMENT_WEIGHTSENSOR_LOADADJUST_UNLOAD_TOP)
				&& (OldScreenIndex != SCREEN_STATE_MENU_EQUIPMENT_WEIGHTSENSOR_LOADADJUST_LOAD_TOP)
				&& (OldScreenIndex != SCREEN_STATE_MENU_EQUIPMENT_WEIGHTSENSOR_LOADADJUST_INITIAL_TOP))
			{
				SetListString3(37, 38, 39);
				DisplayListAll((ScreenIndex&0x000F0000)>>16);
				DrawMenuFull();
			}
			else if(OldScreenIndex != ScreenIndex)
			{
				DisplayList((ScreenIndex&0x000F0000)>>16);
			}
			break;
		case SCREEN_STATE_MENU_EQUIPMENT_WEIGHTSENSOR_LOADADJUST_UNLOAD_INFO:	
			if((OldScreenIndex != ScreenIndex) || (OldCurserIndex != CurserIndex))
				DisplayWeighSensorUnloadSetting();
			break;
		case SCREEN_STATE_MENU_EQUIPMENT_WEIGHTSENSOR_LOADADJUST_UNLOAD_COMPLETE:	
		case SCREEN_STATE_MENU_EQUIPMENT_WEIGHTSENSOR_LOADADJUST_LOAD_COMPLETE:			
		case SCREEN_STATE_MENU_EQUIPMENT_WEIGHTSENSOR_LOADADJUST_INITIAL_COMPLETE:	
		case SCREEN_STATE_MENU_EQUIPMENT_WEIGHTSENSOR_CROSSSECTION_INPUT_COMPLETE:
		case SCREEN_STATE_MENU_EQUIPMENT_WEIGHTSENSOR_LOADADJUST_UNLOAD_NOT_COMPLETE:	
		case SCREEN_STATE_MENU_EQUIPMENT_WEIGHTSENSOR_LOADADJUST_LOAD_NOT_COMPLETE:
		case SCREEN_STATE_MENU_EQUIPMENT_WEIGHTSENSOR_LOADADJUST_INITIAL_NOT_COMPLETE:
		case SCREEN_STATE_MENU_EQUIPMENT_WEIGHTSENSOR_CROSSSECTION_INPUT_NOT_COMPLETE:
			DisplayPopupStringComplete();
			break;
		case SCREEN_STATE_MENU_EQUIPMENT_WEIGHTSENSOR_LOADADJUST_LOAD_INFO:		
			if(OldScreenIndex != ScreenIndex)
				DisplayWeighSensorLoadSetting();
			break;
		case SCREEN_STATE_MENU_EQUIPMENT_WEIGHTSENSOR_LOADADJUST_INITIAL_SET:		
			if(OldScreenIndex != ScreenIndex)
				DisplayPopupString1((USHORT*)*StringSentence[46]);
			break;
		case SCREEN_STATE_MENU_EQUIPMENT_WEIGHTSENSOR_DISPLAYSET_UNIT:				
			if((OldScreenIndex != ScreenIndex) || (OldCurserIndex != CurserIndex))
				DisplayWeighSensorDisplaySetting();
			break;
		case SCREEN_STATE_MENU_EQUIPMENT_WEIGHTSENSOR_BUZZER_SET :
			if((OldScreenIndex != ScreenIndex) || (OldCurserIndex != CurserIndex))
				DisplayListOffOn();
			break;
	}
}
