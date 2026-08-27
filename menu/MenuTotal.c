#include "FLV_Cluster_APP.h"

extern unsigned char			Equipment_Calibration;
extern EEPROM_DISPLAY_DATA OldInfoDisplaySetting;

extern st_AD_DATA 		AD_DATA;

void DisplayMenu()
{
	if(OldScreenIndex != ScreenIndex)
	{
		DisplayListTitleMenu();
	}


	if(ScreenIndex & 0x0FFFFFFF)
	{
		switch(ScreenIndex & 0xF0000000)
		{
		case SCREEN_STATE_MENU_EQUIPMENT_TOP:
			DisplayMenuEquipment();
			break;
		case SCREEN_STATE_MENU_MAINTENANCE_TOP:
			DisplayMenuMaintenance();
			break;
		case SCREEN_STATE_MENU_DISPLAYSETTING_TOP:
			DisplayMenuDisplaySetting();
			break;
		}
	}
	else
	{
		if((OldScreenIndex != SCREEN_STATE_MENU_EQUIPMENT_TOP)
			&& (OldScreenIndex != SCREEN_STATE_MENU_MAINTENANCE_TOP)
			&& (OldScreenIndex != SCREEN_STATE_MENU_DISPLAYSETTING_TOP))
		{
			SetListString3(15, 16, 17);
			DisplayListAll(((ScreenIndex&0xF0000000)>>28)-2);
			DrawMenuFull();
		}
		else if(OldScreenIndex != ScreenIndex)
		{
			DisplayList(((ScreenIndex&0xF0000000)>>28)-2);
		}
	}
}


void DrawMenuFull()
{
	if(ScreenIndex!=SCREEN_STATE_ESL_PASSWORD)		OldScreenIndex = ScreenIndex;

}

void DisplayListOffOn()
{
	if(OldScreenIndex != ScreenIndex)
	{
		OldCurserIndex = CurserIndex;

		SetSubListString(2, CurserIndex, StringEtc[SIGN_OFF], StringEtc[SIGN_ON], StringEtc[SIGN_NOSTRING], StringEtc[SIGN_NOSTRING], StringEtc[SIGN_NOSTRING], StringEtc[SIGN_NOSTRING]);
	}	
	else
	{
		DisplaySubDoubleList(CurserIndex);		
	}
}
///////////////////////////////////////////////////////////////////////////
void CheckKey()
{
	if(KeyUIcnt100ms++ > 1)
	{
		CurserIndex = 0xff;
		KeyUIcnt100ms = 3;
	}
}
///////////////////////////////////////////////////////////////////////////
void CheckCalibrationFlag()
{
	if(Checkcnt100ms++ > 30)
	{
		switch(ScreenIndex)
		{
			case SCREEN_STATE_MENU_DISPLAYSETTING_LCD_BRIGHTNESS_CHANGE:
			case SCREEN_STATE_MENU_DISPLAYSETTING_LED_BRIGHTNESS_CHANGE:
			case SCREEN_STATE_MENU_DISPLAYSETTING_LANGUAGE_CHANGE:		
				SetScreenIndex(ScreenIndex & 0xFF000000);
				break;
			case SCREEN_STATE_MENU_EQUIPMENT_WEIGHTSENSOR_DISPLAYSET_UNIT:		
			case SCREEN_STATE_MENU_EQUIPMENT_WEIGHTSENSOR_BUZZER_SET:									
			case SCREEN_STATE_MENU_DISPLAYSETTING_UNIT_SPEED_CHANGE:
			case SCREEN_STATE_MENU_DISPLAYSETTING_UNIT_WEIGHT_CHANGE:
			case SCREEN_STATE_MENU_DISPLAYSETTING_UNIT_TEMPERATURE_CHANGE:
			case SCREEN_STATE_MENU_DISPLAYSETTING_UNIT_PRESSURE_CHANGE:
				SetScreenIndex(ScreenIndex & 0xFFF00000);
				break;
			default:
				SetScreenIndex(ScreenIndex | 0x00000002);
				Equipment_Calibration = 0;
				break;
		}
	}
	else
	{
		if(CalibrationFlag == DISPLAY_INFO_CHANGE)
		{
			UINT32 tempIndex = ScreenIndex;
			switch(ScreenIndex)
			{
				case SCREEN_STATE_MENU_EQUIPMENT_WEIGHTSENSOR_DISPLAYSET_UNIT:		
					if(InfoDisplaySetting.WeightDisplayUnit == OldInfoDisplaySetting.WeightDisplayUnit)
						SetScreenIndex(ScreenIndex & 0xFFF00000);
					break;
				case SCREEN_STATE_MENU_EQUIPMENT_WEIGHTSENSOR_BUZZER_SET:									
					if(InfoDisplaySetting.LoadBuzzerSet == OldInfoDisplaySetting.LoadBuzzerSet)
						SetScreenIndex(ScreenIndex & 0xFFF00000);
					break;
			
				case SCREEN_STATE_MENU_DISPLAYSETTING_LCD_BRIGHTNESS_CHANGE:
					if(InfoDisplaySetting.LCDBrightnessLevel == OldInfoDisplaySetting.LCDBrightnessLevel)
						SetScreenIndex(ScreenIndex & 0xFF000000);
					break;
				case SCREEN_STATE_MENU_DISPLAYSETTING_LED_BRIGHTNESS_CHANGE:
					if(InfoDisplaySetting.LEDBrightnessLevel == OldInfoDisplaySetting.LEDBrightnessLevel)
						SetScreenIndex(ScreenIndex & 0xFF000000);
					break;
				case SCREEN_STATE_MENU_DISPLAYSETTING_UNIT_SPEED_CHANGE:
					if(InfoDisplaySetting.SpeedUnit == OldInfoDisplaySetting.SpeedUnit)
						SetScreenIndex(ScreenIndex & 0xFFF00000);
					break;
				case SCREEN_STATE_MENU_DISPLAYSETTING_UNIT_WEIGHT_CHANGE:
					if(InfoDisplaySetting.WeightUnit == OldInfoDisplaySetting.WeightUnit)
						SetScreenIndex(ScreenIndex & 0xFFF00000);
					break;
				case SCREEN_STATE_MENU_DISPLAYSETTING_UNIT_TEMPERATURE_CHANGE:
					if(InfoDisplaySetting.TemperatureUnit == OldInfoDisplaySetting.TemperatureUnit)
						SetScreenIndex(ScreenIndex & 0xFFF00000);
					break;
				case SCREEN_STATE_MENU_DISPLAYSETTING_UNIT_PRESSURE_CHANGE:
					if(InfoDisplaySetting.PressureUnit == OldInfoDisplaySetting.PressureUnit)
						SetScreenIndex(ScreenIndex & 0xFFF00000);
					break;
				case SCREEN_STATE_MENU_DISPLAYSETTING_LANGUAGE_CHANGE:					
					if(InfoDisplaySetting.Language == OldInfoDisplaySetting.Language)
					{
						SetScreenIndex(ScreenIndex & 0xFF000000);
						SetLanguage(InfoDisplaySetting.Language);
					}
					break;
			}
			
			if(tempIndex == ScreenIndex)
			{
				if(Checkcnt100ms > 3)
					Checkcnt100ms = 30;
				else
					SaveDisplaySetting();
			}
			else
			{
				Checkcnt100ms = 0;
				CalibrationFlag = 0;	
			}
		}
		else if(CalibrationFlag == CROSSSECTRION_INPUT)
		{
			unsigned short tempshort = AD_DATA.C_A_IntegerValue;
			unsigned char tempchar = AD_DATA.C_A_PointValue;
			float tempfloat = tempchar;
			EepromWrite(ADDRESS2_USER_MAST_INTEGER, (unsigned char*)(&tempshort), 2);   

			EepromWrite(ADDRESS2_USER_MAST_POINT, (unsigned char*)(&tempchar), 1);   
			AD_DATA.C_A = tempshort + tempfloat/100;

			SetScreenIndex(SCREEN_STATE_MENU_EQUIPMENT_WEIGHTSENSOR_CROSSSECTION_INPUT_COMPLETE);
			Checkcnt100ms = 0;
			CalibrationFlag = 0;
		}
		else if(Equipment_Calibration == 0)
		{
			SetScreenIndex(ScreenIndex | 0x00000001);
			Checkcnt100ms = 0;
			CalibrationFlag = 0;
		}
        }
}
#if 0
		else if(CalibrationFlag == MAXSPEEDLIMIT_CHANGE)
		{
			if(RX_MCU_DATA_65452.MaxSpeedLimitSet == (TX_MCU_DATA_65452->MaxSpeedLimitSet<<2))
			{
				TX_MCU_DATA_65452->MaxSpeedLimitSet = 0xF;
				SetScreenIndex(SCREEN_STATE_MENU_EQUIPMENT_MAXSPEEDLIMIT_FUNC_COMPLETE);
				Chekcnt100ms = 0;
				CalibrationFlag = 0;
			}
		}
		else if(CalibrationFlag == MAXSPEEDLIMIT_VALUE_CHANGE)
		{
			if(RX_MCU_DATA_65452.MaxSpeedLimitSet == (TX_MCU_DATA_65452->MaxSpeedLimitSet<<2))
			{
				TX_MCU_DATA_65452->MaxSpeedLimitSet = 0xF;
				TX_MCU_DATA_65452->MaxSpeedLimitValue = 0xff;
				SetScreenIndex(SCREEN_STATE_MENU_EQUIPMENT_MAXSPEEDLIMIT_SPEED_SET_COMPLETE);
				Chekcnt100ms = 0;
				CalibrationFlag = 0;
			}
		}
#endif
