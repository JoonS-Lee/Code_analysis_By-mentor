#include "FLV_Cluster_APP.h"

extern UCHAR CurList;

void DisplayUnitSet2(UINT32 nDisplayList)
{
	if(OldScreenIndex != ScreenIndex)
	{
		switch(ScreenIndex)
		{
			case SCREEN_STATE_MENU_DISPLAYSETTING_UNIT_SPEED_CHANGE:
				SetSubListString(2, CurserIndex, StringEtc[SIGN_KMH], StringEtc[SIGN_MPH], StringEtc[SIGN_NOSTRING], StringEtc[SIGN_NOSTRING], StringEtc[SIGN_NOSTRING], StringEtc[SIGN_NOSTRING]);
				break;
			case SCREEN_STATE_MENU_DISPLAYSETTING_UNIT_WEIGHT_CHANGE:
				SetSubListString(2, CurserIndex, StringEtc[SIGN_TON], StringEtc[SIGN_LB], StringEtc[SIGN_NOSTRING], StringEtc[SIGN_NOSTRING], StringEtc[SIGN_NOSTRING], StringEtc[SIGN_NOSTRING]);
				break;
			case SCREEN_STATE_MENU_DISPLAYSETTING_UNIT_TEMPERATURE_CHANGE:
				SetSubListString(2, CurserIndex, StringEtc[SIGN_TEMP_C], StringEtc[SIGN_TEMP_F], StringEtc[SIGN_NOSTRING], StringEtc[SIGN_NOSTRING], StringEtc[SIGN_NOSTRING], StringEtc[SIGN_NOSTRING]);
				break;
		}		
		OldCurserIndex = CurserIndex;
	}

	else if(OldCurserIndex != CurserIndex)
	{
		DisplaySubDoubleList(CurserIndex);
	}
}

void DisplayUnitSet3(UINT32 nDisplayList)
{
	if(OldScreenIndex != ScreenIndex)
	{
		SetSubListString(3, CurserIndex, StringEtc[SIGN_BAR], StringEtc[SIGN_MPA], StringEtc[SIGN_KGCM2], StringEtc[SIGN_NOSTRING], StringEtc[SIGN_NOSTRING], StringEtc[SIGN_NOSTRING]);
		OldCurserIndex = CurserIndex;
	}
	else if(OldCurserIndex != CurserIndex)
	{
		DisplaySubDoubleList(CurserIndex);
	}
}

void DisplayUnitSet()
{
	switch(ScreenIndex)
	{
		case SCREEN_STATE_MENU_DISPLAYSETTING_UNIT_SPEED_TOP:
		case SCREEN_STATE_MENU_DISPLAYSETTING_UNIT_WEIGHT_TOP:
		case SCREEN_STATE_MENU_DISPLAYSETTING_UNIT_TEMPERATURE_TOP:
			if((OldScreenIndex != SCREEN_STATE_MENU_DISPLAYSETTING_UNIT_SPEED_TOP)
					&& (OldScreenIndex != SCREEN_STATE_MENU_DISPLAYSETTING_UNIT_WEIGHT_TOP)
					&& (OldScreenIndex != SCREEN_STATE_MENU_DISPLAYSETTING_UNIT_TEMPERATURE_TOP))
			{
				SetListString3(87, 88, 89);
				DisplayListAll((ScreenIndex&0x00F00000)>>20);
				DisplayListPage(1,2);
				DrawMenuFull();
			}
			else if(OldScreenIndex != ScreenIndex)
			{
				DisplayList((ScreenIndex&0x00F00000)>>20);
			}
			break;
		case SCREEN_STATE_MENU_DISPLAYSETTING_UNIT_PRESSURE_TOP:
			if(OldScreenIndex != ScreenIndex)
			{
				SetListString1(90);
				DisplayListAll(1);
				DisplayListPage(2,2);
				DrawMenuFull();
			}
			break;
		case SCREEN_STATE_MENU_DISPLAYSETTING_UNIT_SPEED_CHANGE:
		case SCREEN_STATE_MENU_DISPLAYSETTING_UNIT_WEIGHT_CHANGE:
		case SCREEN_STATE_MENU_DISPLAYSETTING_UNIT_TEMPERATURE_CHANGE:
			if((OldScreenIndex != ScreenIndex) || (OldCurserIndex != CurserIndex))
			{
				DisplayUnitSet2(ScreenIndex);
			}
			break;
		case SCREEN_STATE_MENU_DISPLAYSETTING_UNIT_PRESSURE_CHANGE:
			if((OldScreenIndex != ScreenIndex) || (OldCurserIndex != CurserIndex))
			{
				DisplayUnitSet3(ScreenIndex);
			}
			break;
	}
}
