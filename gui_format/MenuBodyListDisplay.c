#include "FLV_Cluster_APP.h"

UCHAR CurListCount;
USHORT *StringList[NUMBEROFLIST];
USHORT* StringSmallList[NUMBEROFSMALLLIST];

UCHAR CurList;

extern EEPROM_MODEL_DATA1	InfoModel1;
extern EEPROM_MODEL_DATA2	InfoModel2;
extern EEPROM_MODEL_DATA3	InfoModel3;

void SetListString(UCHAR nListCnt, USHORT *s1, USHORT *s2, USHORT *s3)
{
	CurListCount = nListCnt;
	
	StringList[0] = &s1[0];
	StringList[1] = &s2[0];
	StringList[2] = &s3[0];

	DisplayMenuBackgroundBG();
}

void SetListString_1(USHORT *s1)

{
	CurListCount = 1;
	StringList[0] = &s1[0];

	DisplayMenuBackgroundBG();
}

void SetListString_2(USHORT *s1, USHORT *s2)

{
	CurListCount = 2;
	StringList[0] = &s1[0];
	StringList[1] = &s2[0];

	DisplayMenuBackgroundBG();
}

void SetListString_3(USHORT *s1, USHORT *s2,USHORT *s3)

{
	CurListCount = 3;
	StringList[0] = &s1[0];
	StringList[1] = &s2[0];
	StringList[2] = &s3[0];

	DisplayMenuBackgroundBG();
}


void SetListString1(USHORT s1)
{
	CurListCount = 1;
	StringList[0] = (USHORT*)*StringSentence[s1];
	StringList[1] = StringList[2] = StringEtc[SIGN_NOSTRING];

	DisplayMenuBackgroundBG();
}

void SetListString2(USHORT s1, USHORT s2)
{
	CurListCount = 2;
	StringList[0] = (USHORT*)*StringSentence[s1];
	StringList[1] = (USHORT*)*StringSentence[s2];
	StringList[2] = StringEtc[SIGN_NOSTRING];

	DisplayMenuBackgroundBG();
}


void SetListString3(USHORT s1, USHORT s2, USHORT s3)
{
	CurListCount = 3;
	StringList[0] = (USHORT*)*StringSentence[s1];
	StringList[1] = (USHORT*)*StringSentence[s2];
	StringList[2] = (USHORT*)*StringSentence[s3];

	DisplayMenuBackgroundBG();
}


void SetMarqueeTextView(UCHAR nList)
{
	USHORT y_Pos[] = {48, 90, 132};

	m_MarInfo.cntMarText = 0;
	m_MarInfo.SetInfo = 0;

	m_MarText[0].PictureX = 6;
	m_MarText[0].PictureY = y_Pos[nList];
	m_MarText[0].PictureWidth = 200;
	m_MarText[0].PictureHeight = 38;
	switch(ScreenIndex & 0xF0000000)
	{
		case SCREEN_STATE_MENU_EQUIPMENT_TOP:
			m_MarText[0].uwStartAddress = COLOR_MENU_ORANGE;
			break;			
		case SCREEN_STATE_MENU_MAINTENANCE_TOP:
			m_MarText[0].uwStartAddress = COLOR_MENU_GREEN;
			break;			
		case SCREEN_STATE_MENU_DISPLAYSETTING_TOP:
			m_MarText[0].uwStartAddress = COLOR_MENU_BLUE;
			break;			
	}			

	m_MarText[0].StringX= 9;
	m_MarText[0].StringY= y_Pos[nList]+9;
	m_MarText[0].StringWidth = (ScreenIndex == SCREEN_STATE_MENU_MAINTENANCE_MANAGEMENT_LIST)? (WIDTH_LIST_LAMP):WIDTH_LIST;
	m_MarText[0].FontSize = 23;
	m_MarText[0].StringColor = COLOR_WHITE;
	m_MarText[0].StringIndex = nList; //StringList[nList];
	GetStringLength(StringList[nList], gui_font23, m_MarText[0].StringWidth, &m_MarText[0].WidthLength, &m_MarText[0].MaxLength);
	
	m_MarInfo.cntMarText  = 1;	
	m_MarInfo.SetInfo = 1;
}

void DisplayListAll(UCHAR nList)
{
	USHORT y_Pos[] = {48, 90, 132};
	unsigned char SetMarquee;

	if((nList != 0) && (nList < (NUMBEROFLIST+1)))
		CurList = nList;
	else
		CurList = 0;

	for(UCHAR i=0;i<CurListCount;i++)
	{
		if(nList-1==i)
		{
			switch(ScreenIndex & 0xF0000000)
			{
				case SCREEN_STATE_MENU_EQUIPMENT_TOP:
					LCD_Draw_Color(6, y_Pos[nList-1], 200, 38, COLOR_MENU_ORANGE);
					break;			
				case SCREEN_STATE_MENU_MAINTENANCE_TOP:
					LCD_Draw_Color(6, y_Pos[nList-1], 200, 38, COLOR_MENU_GREEN);
					break;			
				case SCREEN_STATE_MENU_DISPLAYSETTING_TOP:
					LCD_Draw_Color(6, y_Pos[nList-1], 200, 38, COLOR_MENU_BLUE);
					break;			
			}			
		}
		else
		{
			LCD_Draw_Color(6, y_Pos[i], 200, 38, COLOR_MENU_GRAY);
		}
		if(((InfoModel1.DCSR_Status == CURSER_OFF)
			&& (ScreenIndex == SCREEN_STATE_MENU_EQUIPMENT_DCSR_MODE_TOP)
			&& (i == 1))
			||((InfoModel2.SpeedLimitStatus == CURSER_OFF)
			&& (ScreenIndex == SCREEN_STATE_MENU_EQUIPMENT_MAXSPEEDLIMIT_FUNC_TOP)
			&& (i == 1))
			||((InfoModel3.AutoShift_Status == CURSER_OFF)
			&& (ScreenIndex == SCREEN_STATE_MENU_EQUIPMENT_AUTOSHIFT_MODE_TOP)
			&& (i != 0))
			||((InfoModel2.ZeroStart== CURSER_OFF)
			&& (ScreenIndex == SCREEN_STATE_MENU_EQUIPMENT_ZEROSTART_MODE_TOP)
			&& (i != 0)))			
			SetMarquee = GUI_DisplayLeftStringAt(StringList[i], 9, y_Pos[i]+9,  WIDTH_LIST, COLOR_MENU_LIGHTGRAY, 23);
		else
			SetMarquee = GUI_DisplayLeftStringAt(StringList[i], 9, y_Pos[i]+9,  WIDTH_LIST, COLOR_WHITE, 23);
		if((nList-1 == i) && SetMarquee)
			SetMarqueeTextView(nList-1);

	}
}

void DisplayList(UCHAR nList)
{
	USHORT y_Pos[] = {48, 90, 132};
	unsigned char SetMarquee;

	// 현재커서 
	if((CurList != 0)&& (CurList < 4))
	{
		LCD_Draw_Color(6, y_Pos[CurList-1], 200, 38, COLOR_MENU_GRAY);
		GUI_DisplayLeftStringAt(StringList[CurList-1], 9, y_Pos[CurList-1]+9,  WIDTH_LIST, COLOR_WHITE, 23);
	}
	
	if((nList != 0) && (nList < 4))
	{
		
		switch(ScreenIndex & 0xF0000000)
		{
			case SCREEN_STATE_MENU_EQUIPMENT_TOP:
				LCD_Draw_Color(6, y_Pos[nList-1], 200, 38, COLOR_MENU_ORANGE);
				break;			
			case SCREEN_STATE_MENU_MAINTENANCE_TOP:
				LCD_Draw_Color(6, y_Pos[nList-1], 200, 38, COLOR_MENU_GREEN);
				break;			
			case SCREEN_STATE_MENU_DISPLAYSETTING_TOP:
				LCD_Draw_Color(6, y_Pos[nList-1], 200, 38, COLOR_MENU_BLUE);
				break;			
		}			
		
		SetMarquee = GUI_DisplayLeftStringAt(StringList[nList-1], 9, y_Pos[nList-1]+9, WIDTH_LIST,  COLOR_WHITE, 23);   // select
		if(SetMarquee)
			SetMarqueeTextView(nList-1);
		CurList = nList;
	}
	else
		CurList = 0;

	DrawMenuFull();
	
	if(ScreenIndex !=SCREEN_STATE_ESL_PASSWORD)		OldScreenIndex = ScreenIndex;
}


void DisplaySubListAll(UCHAR nList)
{
	USHORT y_Pos[] = {48, 90, 132};
	UCHAR FontSize = 23;
	unsigned char SetMarquee;

	if(nList  < CurListCount)
		CurList = nList;
	else
		CurList = CurListCount;

	if((ScreenIndex == SCREEN_STATE_MENU_MAINTENANCE_MANAGEMENT_LIST) && m_MarInfo.cntMarText > 0)
	{
		memset(&m_MarText[0], 0x00, 10*sizeof(MarqueeTextView));
		memset(&m_MarInfo, 0x00, sizeof(MarqueeTextInfo));
	}

	for(UCHAR i=0;i<CurListCount;i++)
	{
		USHORT y_Offset = (FontSize==FONTSIZE_2)? (FONTSIZE_1-FONTSIZE_2-2):0;

		if(nList-1==i)
		{
			switch(ScreenIndex & 0xF0000000)
			{
				case SCREEN_STATE_MENU_EQUIPMENT_TOP:
					LCD_Draw_Color(6, y_Pos[nList-1], 200, 38, COLOR_MENU_ORANGE);
					break;			
				case SCREEN_STATE_MENU_MAINTENANCE_TOP:
					LCD_Draw_Color(6, y_Pos[nList-1], 200, 38, COLOR_MENU_GREEN);
					break;			
				case SCREEN_STATE_MENU_DISPLAYSETTING_TOP:
					LCD_Draw_Color(6, y_Pos[nList-1], 200, 38, COLOR_MENU_BLUE);
					break;			
			}			
		}
		else
		{
			LCD_Draw_Color(6, y_Pos[i], 200, 38, COLOR_MENU_GRAY);
		}
		if(ScreenIndex == SCREEN_STATE_MENU_MAINTENANCE_MANAGEMENT_LIST)
			SetMarquee = GUI_DisplayLeftStringAt(StringList[i], 9, y_Pos[i]+4+y_Offset,  WIDTH_LIST_LAMP, COLOR_WHITE, FontSize);
		else if(ScreenIndex == SCREEN_STATE_MENU_MAINTENANCE_MANAGEMENT_LIST_HISTORY)
			SetMarquee = GUI_DisplayLeftStringAt(StringList[i], 9, y_Pos[i]+4+y_Offset,  WIDTH_LIST_MANAGEMENT, COLOR_WHITE, FontSize);
		else
			SetMarquee = GUI_DisplayLeftStringAt(StringList[i], 9, y_Pos[i]+4+y_Offset, WIDTH_LIST, COLOR_WHITE, FontSize);
		if((ScreenIndex == SCREEN_STATE_MENU_MAINTENANCE_MANAGEMENT_LIST) && ((nList-1 == i) && SetMarquee))
			SetMarqueeTextView(nList-1);

	}
}

void DisplaySubList(UCHAR nList)
{
	USHORT y_Pos[] = {48, 90, 132};
	UCHAR FontSize = 23;
	unsigned char SetMarquee;

	// 현재커서 
	if((CurList != 0)&& (CurList < 4))
	{
		LCD_Draw_Color(6, y_Pos[CurList-1], 200, 38, COLOR_MENU_GRAY);
		if(ScreenIndex == SCREEN_STATE_MENU_MAINTENANCE_MANAGEMENT_LIST)
			GUI_DisplayLeftStringAt(StringList[CurList-1], 9, y_Pos[CurList-1]+9, WIDTH_LIST_LAMP, COLOR_WHITE, FontSize);
		else if(ScreenIndex == SCREEN_STATE_MENU_MAINTENANCE_MANAGEMENT_LIST_HISTORY)
			GUI_DisplayLeftStringAt(StringList[CurList-1], 9, y_Pos[CurList-1]+9, WIDTH_LIST_MANAGEMENT, COLOR_WHITE, FontSize);
		else
			GUI_DisplayLeftStringAt(StringList[CurList-1], 9, y_Pos[CurList-1]+9, WIDTH_LIST, COLOR_WHITE, FontSize);

		if((ScreenIndex == SCREEN_STATE_MENU_MAINTENANCE_MANAGEMENT_LIST) && m_MarInfo.cntMarText > 0)
		{
			memset(&m_MarText[0], 0x00, 10*sizeof(MarqueeTextView));
			memset(&m_MarInfo, 0x00, sizeof(MarqueeTextInfo));
		}
	}
	
	if((nList != 0) && (nList < 4))
	{
		
		switch(ScreenIndex & 0xF0000000)
		{
			case SCREEN_STATE_MENU_EQUIPMENT_TOP:
				LCD_Draw_Color(6, y_Pos[nList-1], 200, 38, COLOR_MENU_ORANGE);
				break;			
			case SCREEN_STATE_MENU_MAINTENANCE_TOP:
				LCD_Draw_Color(6, y_Pos[nList-1], 200, 38, COLOR_MENU_GREEN);
				break;			
			case SCREEN_STATE_MENU_DISPLAYSETTING_TOP:
				LCD_Draw_Color(6, y_Pos[nList-1], 200, 38, COLOR_MENU_BLUE);
				break;			
		}			

		if(ScreenIndex == SCREEN_STATE_MENU_MAINTENANCE_MANAGEMENT_LIST)
			SetMarquee = GUI_DisplayLeftStringAt(StringList[nList-1], 9, y_Pos[nList-1]+9, WIDTH_LIST_LAMP,  COLOR_WHITE, FontSize);   // select
		else if(ScreenIndex == SCREEN_STATE_MENU_MAINTENANCE_MANAGEMENT_LIST_HISTORY)
			SetMarquee = GUI_DisplayLeftStringAt(StringList[nList-1], 9, y_Pos[nList-1]+9, WIDTH_LIST_MANAGEMENT,  COLOR_WHITE, FontSize);   // select
		else
			SetMarquee = GUI_DisplayLeftStringAt(StringList[nList-1], 9, y_Pos[nList-1]+9, WIDTH_LIST,  COLOR_WHITE, FontSize);   // select
		if((ScreenIndex == SCREEN_STATE_MENU_MAINTENANCE_MANAGEMENT_LIST) && SetMarquee)
			SetMarqueeTextView(nList-1);
		CurList = nList;
	}
	else
		CurList = 0;
	
	OldCurserIndex = CurserIndex;
	DrawMenuFull();
}


void SetSubListString(UCHAR nListCnt, UCHAR SelectCurser, USHORT *s1, USHORT *s2, USHORT *s3, USHORT *s4, USHORT *s5, USHORT *s6)
{
	CurListCount = nListCnt;
	StringSmallList[0] = &s1[0];
	StringSmallList[1] = &s2[0];
	StringSmallList[2] = &s3[0];
	StringSmallList[3] = &s4[0];
	StringSmallList[4] = &s5[0];
	StringSmallList[5] = &s6[0];
	DisplaySubDoubleListAll(SelectCurser);
}

void SetSubListString2(USHORT s1, USHORT s2, UCHAR SelectCurser)
{
	CurListCount = 2;
	StringSmallList[0] = (USHORT*)*StringSentence[s1];
	StringSmallList[1] = (USHORT*)*StringSentence[s2];
	StringSmallList[2] = StringSmallList[3] = StringSmallList[4] = StringSmallList[5] = StringEtc[SIGN_NOSTRING];
	DisplaySubDoubleListAll(SelectCurser);
}

void SetSubListString3(USHORT s1, USHORT s2, USHORT s3, UCHAR SelectCurser)
{
	CurListCount = 3;
	StringSmallList[0] = (USHORT*)*StringSentence[s1];
	StringSmallList[1] = (USHORT*)*StringSentence[s2];
	StringSmallList[2] = (USHORT*)*StringSentence[s3]; 
	StringSmallList[3] = StringSmallList[4] = StringSmallList[5] = StringEtc[SIGN_NOSTRING];
	DisplaySubDoubleListAll(SelectCurser);
}

void SetSubListString4(USHORT s1, USHORT s2, USHORT s3, USHORT s4, UCHAR SelectCurser)
{
	CurListCount = 4;
	StringSmallList[0] = (USHORT*)*StringSentence[s1];
	StringSmallList[1] = (USHORT*)*StringSentence[s2];
	StringSmallList[2] = (USHORT*)*StringSentence[s3]; 
	StringSmallList[3] = (USHORT*)*StringSentence[s4]; 
	StringSmallList[4] = StringSmallList[5] = StringEtc[SIGN_NOSTRING];
	DisplaySubDoubleListAll(SelectCurser);
}

void SetSubListString5(USHORT s1, USHORT s2, USHORT s3, USHORT s4, USHORT s5, UCHAR SelectCurser)
{
	CurListCount = 5;
	StringSmallList[0] = (USHORT*)*StringSentence[s1];
	StringSmallList[1] = (USHORT*)*StringSentence[s2];
	StringSmallList[2] = (USHORT*)*StringSentence[s3]; 
	StringSmallList[3] = (USHORT*)*StringSentence[s4]; 
	StringSmallList[4] = (USHORT*)*StringSentence[s5]; 
	StringSmallList[5] = StringEtc[SIGN_NOSTRING];
	DisplaySubDoubleListAll(SelectCurser);
}

void SetSubListString6(USHORT s1, USHORT s2, USHORT s3, USHORT s4, USHORT s5, USHORT s6, UCHAR SelectCurser)
{
	CurListCount = 6;
	StringSmallList[0] = (USHORT*)*StringSentence[s1];
	StringSmallList[1] = (USHORT*)*StringSentence[s2];
	StringSmallList[2] = (USHORT*)*StringSentence[s3]; 
	StringSmallList[3] = (USHORT*)*StringSentence[s4]; 
	StringSmallList[4] = (USHORT*)*StringSentence[s5]; 
	StringSmallList[5] = (USHORT*)*StringSentence[s6];
	DisplaySubDoubleListAll(SelectCurser);
}

void DisplaySubDoubleListAll(UCHAR nList)
{
	USHORT y_Pos_3[] = {48, 90, 132};
	USHORT y_Pos_2[] = {68, 110};
	USHORT y_Pos[3];

	if(CurListCount <= 2)
		y_Pos[0] = y_Pos_3[1];
	else if(CurListCount <= 4)
		memcpy(&y_Pos[0], &y_Pos_2[0], sizeof(y_Pos_2));
	else
		memcpy(&y_Pos[0], &y_Pos_3[0], sizeof(y_Pos_3));


	DisplayMenuBackgroundBG();
    
	if(nList  < CurListCount)
		CurList = nList;
	else
		CurList = CurListCount;
    
	for(UCHAR i=0;i<CurListCount;i++)
	{
		if(i == nList)
		{
			UINT32	color;
			switch(ScreenIndex & 0xF0000000)
			{
				case SCREEN_STATE_MENU_EQUIPMENT_TOP:
					color = COLOR_MENU_ORANGE;
					break;			
				case SCREEN_STATE_MENU_MAINTENANCE_TOP:
					color = COLOR_MENU_GREEN;
					break;			
				case SCREEN_STATE_MENU_DISPLAYSETTING_TOP:
					color = COLOR_MENU_BLUE;
					break;			
			}		

			LCD_Draw_Color((i%2==0)?6:108, y_Pos[i/2], 98, 38, color);
		}
		else
			LCD_Draw_Color((i%2==0)?6:108, y_Pos[i/2], 98, 38, COLOR_MENU_GRAY);
        
		GUI_DisplayCenterStringAt(StringSmallList[i], (i%2==0)?6:108,  (y_Pos[i/2]+8),  98, COLOR_WHITE, FONTSIZE);
	}
	DrawMenuFull();
}

void DisplaySubDoubleList(UCHAR nList)
{
	USHORT y_Pos_3[] = {48, 90, 132};
	USHORT y_Pos_2[] = {68, 110};
	USHORT y_Pos[3];

	if(CurListCount <= 2)
		y_Pos[0] = y_Pos_3[1];
	else if(CurListCount <= 4)
		memcpy(&y_Pos[0], &y_Pos_2[0], sizeof(y_Pos_2));
	else
		memcpy(&y_Pos[0], &y_Pos_3[0], sizeof(y_Pos_3));
      
	OldCurserIndex = CurserIndex;
    
	if(CurList  < CurListCount)
	{
		LCD_Draw_Color((CurList%2==0)?6:108, y_Pos[CurList/2], 98, 38, COLOR_MENU_GRAY);
		GUI_DisplayCenterStringAt(StringSmallList[CurList], (CurList%2==0)?6:108, (y_Pos[CurList/2]+8),  98, COLOR_WHITE, FONTSIZE);
	}
	
	if(nList < CurListCount)
	{
		UINT32	color;
		switch(ScreenIndex & 0xF0000000)
		{
			case SCREEN_STATE_MENU_EQUIPMENT_TOP:
				color = COLOR_MENU_ORANGE;
				break;			
			case SCREEN_STATE_MENU_MAINTENANCE_TOP:
				color = COLOR_MENU_GREEN;
				break;			
			case SCREEN_STATE_MENU_DISPLAYSETTING_TOP:
				color = COLOR_MENU_BLUE;
				break;			
		}		

		LCD_Draw_Color((nList%2==0)?6:108, y_Pos[nList/2], 98, 38, color);
	
		GUI_DisplayCenterStringAt(StringSmallList[nList], (nList%2==0)?6:108, (y_Pos[nList/2]+8),  98, COLOR_WHITE, FONTSIZE);
		
		CurList = nList;
	}
	else
		CurList = CurListCount;
    
	DrawMenuFull();
	
}


