#include "FLV_Cluster_APP.h"

extern UCHAR gMaintenanceItem;

UCHAR OldMaintenanceAlarm[MAINTENANCE_TOTAL];
USHORT Interval,OldInterval,temp_interval;
USHORT OldElapse;
unsigned char Replacement_Count;

extern st_Maintenance Maintenance_Data[];
extern unsigned char Engine_Type;
extern EEPROM_MODEL_DATA1 InfoModel1;
void DrawAlarm(unsigned char lamp_on)
{
	if(lamp_on == 0)
		return;

	UCHAR flag = 0x00000001;
	UCHAR totallist;
	UCHAR index;

	if( ((InfoModel1.ModelInfo >= MODEL_35D_9S)&&(InfoModel1.ModelInfo <= MODEL_50D_9S))
		||
((InfoModel1.ModelInfo >= MODEL_25D_9HDI)&&(InfoModel1.ModelInfo <= MODEL_50DN_9HDI)))
	{
		totallist = 3;
		index = CurserIndex;
	}
	else if((InfoModel1.ModelInfo >= MODEL_35L_9)&&(InfoModel1.ModelInfo <= MODEL_50L_9))
	{
		if(CurserIndex > 11)
		{
			totallist = 2;
		}
		else
		{	
			totallist = 3;
		}

		if(CurserIndex >= MAINTENANCE_FUEL_FILTER)
			index = CurserIndex+1;
		else
			index = CurserIndex;
		
	}
	else if((InfoModel1.ModelInfo >= MODEL_25LC_9)&&(InfoModel1.ModelInfo <= MODEL_33LC_9))
	{
		if(CurserIndex > 8)
		{
			totallist = 2;
		}
		else
		{	
			totallist = 3;
		}

		if(CurserIndex >= MAINTENANCE_FUEL_FILTER)
			index = CurserIndex+1;
		else
			index = CurserIndex;

	}
	else if((InfoModel1.ModelInfo >= MODEL_25L_9A)&&(InfoModel1.ModelInfo <= MODEL_35LN_9A))
	{
		if(CurserIndex > 8)
		{
			totallist = 2;
		}
		else
		{	
			totallist = 3;
		}
		
		if(CurserIndex > MAINTENANCE_DPF_ASH_CLEANING)
			index = MAINTENANCE_BRAKE_COOLING_OIL_FILTER;
		else if(CurserIndex >= MAINTENANCE_FAN_BELT)
			index = CurserIndex+2;
		else	
			index = CurserIndex;

	}
	else // 9V
	{
		if(CurserIndex > 11)
		{
			totallist = 1;
		}
		else
		{	
			totallist = 3;
		}

		if(CurserIndex > MAINTENANCE_HYD_OIL)
			index = MAINTENANCE_DPF_ASH_CLEANING;
		else
			index = CurserIndex;

	}	

	USHORT y_Pos[] = {58, 100, 142};

	for(int i=0;i<totallist;i++)
	{
		if((flag & lamp_on) == flag)
		{
			if(OldMaintenanceAlarm[i+(index/NUMBEROFLIST)*NUMBEROFLIST] == 0)
				PCXtoBMP_16bit(180, y_Pos[i], 19, 19, (CurserIndex ==( i+(CurserIndex/NUMBEROFLIST)*NUMBEROFLIST) )?FL_Image.signal_grey_green_bg:FL_Image.signal_grey);
			else
				PCXtoBMP_16bit(180, y_Pos[i], 19, 19, (CurserIndex == (i+(CurserIndex/NUMBEROFLIST)*NUMBEROFLIST) )?FL_Image.signal_red_green_bg:FL_Image.signal_red);

		}
		flag = flag << 1;
	}
}


UCHAR GetAlarmCheck()
{
	UCHAR totallist;
	UCHAR index;


	if( ((InfoModel1.ModelInfo >= MODEL_35D_9S)&&(InfoModel1.ModelInfo <= MODEL_50D_9S))
		||
((InfoModel1.ModelInfo >= MODEL_25D_9HDI)&&(InfoModel1.ModelInfo <= MODEL_50DN_9HDI)))
	{
		totallist = 3;
		index = CurserIndex;
	}
	else if((InfoModel1.ModelInfo >= MODEL_35L_9)&&(InfoModel1.ModelInfo <= MODEL_50L_9))
	{
		if(CurserIndex > 11)
		{
			totallist = 2;
		}
		else
		{
			totallist = 3;
		}

		if(CurserIndex >= MAINTENANCE_FUEL_FILTER)
			index = CurserIndex+1;
		else
			index = CurserIndex;
	}
	else if((InfoModel1.ModelInfo >= MODEL_25LC_9)&&(InfoModel1.ModelInfo <= MODEL_33LC_9))
	{
		if(CurserIndex > 8)
		{
			totallist = 2;
		}
		else
		{	
			totallist = 3;
		}

		if(CurserIndex >= MAINTENANCE_FUEL_FILTER)
			index = CurserIndex+1;
		else
			index = CurserIndex;

	}
	else if((InfoModel1.ModelInfo >= MODEL_25L_9A)&&(InfoModel1.ModelInfo <= MODEL_35LN_9A))
	{
		if(CurserIndex > 8)
		{
			totallist = 2;
		}
		else
		{	
			totallist = 3;
		}
		
		if(CurserIndex > MAINTENANCE_DPF_ASH_CLEANING)
			index = MAINTENANCE_BRAKE_COOLING_OIL_FILTER;
		else if(CurserIndex >= MAINTENANCE_FAN_BELT)
			index = CurserIndex+2;
		else	
			index = CurserIndex;

	}
	else // 9V
	{
		if(CurserIndex > 11)
		{
			totallist = 1;		
		}
		else
		{	
			totallist = 3;		
		}

		if(CurserIndex > MAINTENANCE_HYD_OIL)
			index = MAINTENANCE_DPF_ASH_CLEANING;
		else
			index = CurserIndex;

	}

	UCHAR temp = 0x00000001;
	UCHAR ChangeAlarm = 0;	

	for(int i=0;i<totallist;i++)
	{
		if(OldMaintenanceAlarm[i+(index/NUMBEROFLIST)*NUMBEROFLIST] != Maintenance_Data[i+(index/NUMBEROFLIST)*NUMBEROFLIST].Maintenance_Alarm)
		{
			OldMaintenanceAlarm[i+(index/NUMBEROFLIST)*NUMBEROFLIST] = Maintenance_Data[i+(index/NUMBEROFLIST)*NUMBEROFLIST].Maintenance_Alarm;
			
			ChangeAlarm |= temp;
		}
		temp = temp << 1;
	}

	
	return ChangeAlarm;
}

void DisplayMaintanenceList(UCHAR Index)
{

	if( ((InfoModel1.ModelInfo >= MODEL_35D_9S)&&(InfoModel1.ModelInfo <= MODEL_50D_9S))
		||
((InfoModel1.ModelInfo >= MODEL_25D_9HDI)&&(InfoModel1.ModelInfo <= MODEL_50DN_9HDI)))
	{
		SetListString3(INDEX_STR_MANAGEMENT+(Index/NUMBEROFLIST)*NUMBEROFLIST, 1+INDEX_STR_MANAGEMENT+(Index/NUMBEROFLIST)*NUMBEROFLIST, 2+INDEX_STR_MANAGEMENT+(Index/NUMBEROFLIST)*NUMBEROFLIST);
	}
	else if((InfoModel1.ModelInfo >= MODEL_35L_9)&&(InfoModel1.ModelInfo <= MODEL_50L_9))
	{
		if(Index <= 2)
		{
			SetListString3(INDEX_STR_MANAGEMENT_ENGINE_OIL_FILTER, INDEX_STR_MANAGEMENT_TM_OIL_FILTER, INDEX_STR_MANAGEMENT_DIFF_GEAR_OIL);
		}
		else if((Index >= 3)&&(Index <= 5))
		{	
			SetListString3(INDEX_STR_MANAGEMENT_HYD_AIR_BREATHER, INDEX_STR_MANAGEMENT_HYD_OIL_RETURN_FILTER, INDEX_STR_MANAGEMENT_FAN_BELT); // FUEL FILTER 없음
			
		}
		else if((Index >= 6)&&(Index <= 8))
		{	
			SetListString3(INDEX_STR_MANAGEMENT_BRAKE_OIL, INDEX_STR_MANAGEMENT_AIR_CLEANER_ELEMENT, INDEX_STR_MANAGEMENT_HYD_SUCTION_STRAINER);
			
		}
		else if((Index >= 9)&&(Index <= 11))
		{	
			SetListString3(INDEX_STR_MANAGEMENT_RADIATOR_COOLANT, INDEX_STR_MANAGEMENT_HYD_OIL, INDEX_STR_MANAGEMENT_LPG_VAPORIZE_COOLANT_HOSE);
			
		}
		else
		{	
			SetListString2(INDEX_STR_MANAGEMENT_SPARK_PLUG, INDEX_STR_MANAGEMENT_LOCK_OFF_VALVE_FILTER);
			
		}
	}
	else if((InfoModel1.ModelInfo >= MODEL_25LC_9)&&(InfoModel1.ModelInfo <= MODEL_33LC_9))
	{
		if(Index <= 2)
		{
			SetListString3(INDEX_STR_MANAGEMENT_ENGINE_OIL_FILTER, INDEX_STR_MANAGEMENT_TM_OIL_FILTER, INDEX_STR_MANAGEMENT_DIFF_GEAR_OIL);
		}
		else if((Index >= 3)&&(Index <= 5))
		{	
			SetListString3(INDEX_STR_MANAGEMENT_HYD_AIR_BREATHER, INDEX_STR_MANAGEMENT_HYD_OIL_RETURN_FILTER, INDEX_STR_MANAGEMENT_FAN_BELT); // FUEL FILTER 없음
			
		}
		else if((Index >= 6)&&(Index <= 8))
		{	
			SetListString3(INDEX_STR_MANAGEMENT_BRAKE_OIL, INDEX_STR_MANAGEMENT_AIR_CLEANER_ELEMENT, INDEX_STR_MANAGEMENT_HYD_SUCTION_STRAINER);
			
		}
		else
		{	
			SetListString2(INDEX_STR_MANAGEMENT_RADIATOR_COOLANT, INDEX_STR_MANAGEMENT_HYD_OIL);
			
		}
	}
	else if((InfoModel1.ModelInfo >= MODEL_25L_9A)&&(InfoModel1.ModelInfo <= MODEL_35LN_9A))
	{
		if(Index <= 2)
		{
			SetListString3(INDEX_STR_MANAGEMENT_ENGINE_OIL_FILTER , INDEX_STR_MANAGEMENT_TM_OIL_FILTER, INDEX_STR_MANAGEMENT_DIFF_GEAR_OIL);
		}
		else if((Index >= 3)&&(Index <= 5))
		{	
			SetListString3(INDEX_STR_MANAGEMENT_HYD_AIR_BREATHER , INDEX_STR_MANAGEMENT_HYD_OIL_RETURN_FILTER, INDEX_STR_MANAGEMENT_FUEL_FILTER);
			
		}
		else if((Index >= 6)&&(Index <= 8))
		{	
			SetListString3(INDEX_STR_MANAGEMENT_BRAKE_COOLING_OIL_FILTER, INDEX_STR_MANAGEMENT_AIR_CLEANER_ELEMENT, INDEX_STR_MANAGEMENT_HYD_SUCTION_STRAINER); // FAN BELT 없음 -> Brake Cooling Oil & Filter
			
		}
		else
		{	
			SetListString2(INDEX_STR_MANAGEMENT_RADIATOR_COOLANT, INDEX_STR_MANAGEMENT_HYD_OIL);
			
		}
	}
	else // 9V
	{
		if(Index  < MAINTENANCE_LPG_VAPORIZER)
			SetListString3(INDEX_STR_MANAGEMENT+(Index /NUMBEROFLIST)*NUMBEROFLIST, 1+INDEX_STR_MANAGEMENT+(Index /NUMBEROFLIST)*NUMBEROFLIST, 2+INDEX_STR_MANAGEMENT+(Index /NUMBEROFLIST)*NUMBEROFLIST);
		else
		{
			SetListString1(INDEX_STR_MANAGEMENT_DPF_ASH_CLEANING);
		}
	}

}


void DisplayManagementList()
{
	unsigned char m_list;
	unsigned char m_list_page;
	unsigned char i;
	
	m_list = 0;
	m_list_page = 0;
	
	for (i = 0; i < MAINTENANCE_TOTAL; i++)
	{
		if(Maintenance_Data[i].Maintenance_Interval != 0xff)
		{	
			m_list++;
		}
	}
	
	if(m_list >= 4)
		m_list_page = (m_list/2)-1;
	else
	{
		m_list_page = 1;
		m_list = 4;
	}
	

	if((OldScreenIndex != SCREEN_STATE_MENU_MAINTENANCE_MANAGEMENT_LIST)
		|| (((OldCurserIndex % NUMBEROFLIST== (NUMBEROFLIST-1)) && (CurserIndex % NUMBEROFLIST == 0)) 
		|| ((OldCurserIndex % NUMBEROFLIST == 0) && (CurserIndex % NUMBEROFLIST == (NUMBEROFLIST-1)))
		|| ((OldCurserIndex == 0) && (CurserIndex == (m_list-1))) 
		|| ((OldCurserIndex == (m_list-1)) && (CurserIndex == 0))))
	{
		OldScreenIndex = ScreenIndex;
		OldCurserIndex = CurserIndex;

		memset(&OldMaintenanceAlarm[0], 0xff, sizeof(OldMaintenanceAlarm));

		#if 0
		if(CurserIndex < MAINTENANCE_LPG_VAPORIZER)
			SetListString3(INDEX_STR_MANAGEMENT+(CurserIndex/NUMBEROFLIST)*NUMBEROFLIST, 1+INDEX_STR_MANAGEMENT+(CurserIndex/NUMBEROFLIST)*NUMBEROFLIST, 2+INDEX_STR_MANAGEMENT+(CurserIndex/NUMBEROFLIST)*NUMBEROFLIST);
		else
		{
			SetListString3(INDEX_STR_MANAGEMENT_LPG , 1+INDEX_STR_MANAGEMENT_LPG, 2+INDEX_STR_MANAGEMENT_LPG);
		}
		#else
		DisplayMaintanenceList(CurserIndex);
		#endif
		
		DisplaySubListAll(CurserIndex%NUMBEROFLIST+1);
		//DisplayListPage((CurserIndex/NUMBEROFLIST)+1,(MAINTENANCE_TOTAL/NUMBEROFLIST)+1);
		DisplayListPage((CurserIndex/NUMBEROFLIST)+1,m_list_page);
		DrawMenuFull();
	}
	else if((OldCurserIndex != CurserIndex))
	{
		OldMaintenanceAlarm[CurserIndex] = 0xff;
		OldMaintenanceAlarm[OldCurserIndex] = 0xff;
		OldCurserIndex = CurserIndex;
		DisplaySubList(CurserIndex%NUMBEROFLIST+1);
	}

	DrawAlarm(GetAlarmCheck());
}

void DisplayManagementDetail()
{
	UINT32 tempHourmeter;
	USHORT Elapse;
	UCHAR Count;
	USHORT y_Pos[] = {48, 90, 132};

	tempHourmeter = HCESPN.Hourmeter_Hour_701;

	if(Maintenance_Data[gMaintenanceItem].Maintenance_Hourmeter == 0xffff)
		Elapse = 0;
	else
	{
		if(tempHourmeter < Maintenance_Data[gMaintenanceItem].Maintenance_Hourmeter)
			Elapse=0;
		else
			Elapse = (tempHourmeter - Maintenance_Data[gMaintenanceItem].Maintenance_Hourmeter);
	}
	
	if(OldScreenIndex != SCREEN_STATE_MENU_MAINTENANCE_MANAGEMENT_LIST_HISTORY)
	{
		OldScreenIndex = ScreenIndex;
		OldElapse = 0xffff;
		SetListString3(153, 154, 155);
		DisplaySubListAll(3);
		DisplayListPage(1,2);
		DrawMenuFull();

			
		if(Maintenance_Data[gMaintenanceItem].Maintenance_Interval== 0xff)
			Interval = 0;
		else
			Interval = Maintenance_Data[gMaintenanceItem].Maintenance_Interval*50;

		if(Maintenance_Data[gMaintenanceItem].Maintenance_Replacement_Count == 0xff)
			Count = 0;
		else 
			Count = Maintenance_Data[gMaintenanceItem].Maintenance_Replacement_Count;

		GUI_DisplayRightStringAt_Value(Interval, 142, y_Pos[0]+9,  60, COLOR_MENU_GREEN);
		GUI_DisplayRightStringAt_Value(Count, 142, y_Pos[2]+9,  60, COLOR_WHITE);

	}

	if(OldElapse != Elapse)	
	{
		OldElapse = Elapse;
		GUI_DisplayRightStringAt_Value(Elapse, 142, y_Pos[1]+9,  60, COLOR_MENU_GREEN);
	}

}

void DisplayManagementHistory()
{
	USHORT y_Pos[] = {48, 90, 132};
	UCHAR buf[10]={0,};
	USHORT covbuf[10]={0,};	

	if(OldScreenIndex != ScreenIndex)
	{
		OldCurserIndex = 0xff;
		CurserIndex = 0;
		
		if(Maintenance_Data[gMaintenanceItem].Maintenance_Replacement_Count == 0xff)
			Replacement_Count = 0;
		else 
			Replacement_Count = Maintenance_Data[gMaintenanceItem].Maintenance_Replacement_Count;

		if(Replacement_Count>=9)	Replacement_Count=9;
	}
	if(OldCurserIndex != CurserIndex)
	{
		unsigned char Count; 
		OldCurserIndex = CurserIndex;
		if(CurserIndex == 0)
		{
			if(Replacement_Count >= 6)
				Count = 6;
			else 
				Count = Replacement_Count;
		}
		else
		{
			Count = Replacement_Count - 6;
		}
		
		DisplayMenuBackgroundBG();

		for(UCHAR i=0;i<Count;i++)
		{
			LCD_Draw_Color((i%2==0)?6:108, y_Pos[i/2], 98, 38, COLOR_MENU_GRAY);

			if(CurserIndex == 0)
				sprintf ( (char*)buf, "%d",Maintenance_Data[gMaintenanceItem].Maintenance_History[i]) ; 
			else
				sprintf ( (char*)buf, "%d",Maintenance_Data[gMaintenanceItem].Maintenance_History[i+6]) ; 

			String_Func(covbuf, buf);

			GUI_DisplayRightStringAt(&covbuf[0],  (i%2==0)?6:108,  (y_Pos[i/2]+8),  93, COLOR_WHITE, FONTSIZE);
		}
	
		DrawMenuFull();	
	}

	
}


void DrawMaintenanceCycleChange()
{
	UCHAR buf[10];
	USHORT covbuf[10];

	if(OldScreenIndex != ScreenIndex)
	{
		DisplayMenuBackgroundBG();
		OldCurserIndex = 0xff;
		CurserIndex = CURSER_NONE;	

		OldInterval=0xffff;
		temp_interval = Interval;
	}

	if(OldInterval != temp_interval)
	{
		OldInterval = temp_interval;
		memset((&buf), 0x0, sizeof( buf));
		memset((&covbuf), 0x0, sizeof( covbuf));


		sprintf ( (char*)buf, "%d",OldInterval) ; 

		String_Func(covbuf, buf);

		LCD_Draw_Color(74, 97, 65, 22, COLOR_BLACK);
		GUI_DisplayCenterStringAt(&covbuf[0], 0, 97, LCD_WIDTH, COLOR_WHITE, FONTSIZE);	
	}


	if(OldCurserIndex != CurserIndex)
	{
		LCD_Draw_Color(6, 90, 31, 39, (CurserIndex == CURSER_UP_LEFT)?COLOR_MENU_GREEN:COLOR_MENU_GRAY);
		PCXtoBMP_16bit(16, 101, 10, 15, (CurserIndex == CURSER_UP_LEFT)?FL_Image.menu_arrow_maintenance_select_2:FL_Image.menu_arrow_nor_2);

		LCD_Draw_Color(183, 90, 31, 39, (CurserIndex == CURSER_DOWN_RIGHT)?COLOR_MENU_GREEN:COLOR_MENU_GRAY);
		PCXtoBMP_16bit(193, 101, 10, 15, (CurserIndex == CURSER_DOWN_RIGHT)?FL_Image.menu_arrow_maintenance_select:FL_Image.menu_arrow_nor);
	}

	if(OldScreenIndex != ScreenIndex)
	{
		OldCurserIndex = CurserIndex;
		OldInterval = Interval;
		DrawMenuFull();
	}
	else if(OldCurserIndex != CurserIndex)
	{
		OldCurserIndex = CurserIndex;
	}
}


unsigned char Check_maintenance(void)
{	
	unsigned char i;
	unsigned short Temp;
	unsigned char check_aralm = 0;
	
	for (i = 0; i < MAINTENANCE_TOTAL; i++)
	{
		if(Maintenance_Data[i].Maintenance_Interval != 0xff)
		{
			if ((Maintenance_Data[i].Maintenance_Hourmeter + (unsigned short)(Maintenance_Data[i].Maintenance_Interval) * 50) > (HCESPN.Hourmeter_Hour_701))
			{
				Temp = Maintenance_Data[i].Maintenance_Hourmeter + (unsigned short)((Maintenance_Data[i].Maintenance_Interval) * 50) - (HCESPN.Hourmeter_Hour_701);          
			}
			else
			{
				Temp = 0;
			}

			if (Temp > 30)
			{
				Maintenance_Data[i].Maintenance_Alarm = 0;
			}
			else if (Temp == 0)
			{
				Maintenance_Data[i].Maintenance_Alarm = 2;
				check_aralm=1;
			}
			else
			{
				Maintenance_Data[i].Maintenance_Alarm = 1;  
				check_aralm=1;
			}
		}
	} 
	
	return check_aralm;
}

void DisplayManagement()
{
	switch(ScreenIndex)
	{
		case SCREEN_STATE_MENU_MAINTENANCE_MANAGEMENT_LIST:
			DisplayManagementList();
			break;
		case SCREEN_STATE_MENU_MAINTENANCE_MANAGEMENT_LIST_HISTORY:
			DisplayManagementDetail();
			break;
		case SCREEN_STATE_MENU_MAINTENANCE_MANAGEMENT_HISTORY_DETAIL:
			DisplayManagementHistory();
			break;
		case SCREEN_STATE_MENU_MAINTENANCE_MANAGEMENT_LIST_REPLACE:
		case SCREEN_STATE_MENU_MAINTENANCE_MANAGEMENT_LIST_CYCLE:
			if((OldScreenIndex != SCREEN_STATE_MENU_MAINTENANCE_MANAGEMENT_LIST_REPLACE)
				&& (OldScreenIndex != SCREEN_STATE_MENU_MAINTENANCE_MANAGEMENT_LIST_CYCLE))
			{
				SetListString2(156, 157);
				DisplayListAll(((ScreenIndex&0x000F0000)>>16)-1);
				DisplayListPage(2,2);
				DrawMenuFull();
			}
			else if(OldScreenIndex != ScreenIndex)
			{
				DisplayList(((ScreenIndex&0x000F0000)>>16)-1);
			}
			break;
	
		case SCREEN_STATE_MENU_MAINTENANCE_MANAGEMENT_REPLACE_POPUP :
			if(OldScreenIndex != ScreenIndex)
				DisplayPopupString1((USHORT*)*StringSentence[158]);
			break;
		case SCREEN_STATE_MENU_MAINTENANCE_MANAGEMENT_CHANGE_CYCLE :
			DrawMaintenanceCycleChange();
			break;
		case SCREEN_STATE_MENU_MAINTENANCE_MANAGEMENT_CYCLE_POPUP:
			if(OldScreenIndex != ScreenIndex)
				DisplayPopupString1((USHORT*)*StringSentence[159]);
			break;
	}
}

