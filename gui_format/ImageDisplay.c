#include "FLV_Cluster_APP.h"

FL_STV_Image FL_Image;

GUI_FONT* gui_font17[NUMBEROFFONT];
GUI_FONT* gui_font23[NUMBEROFFONT];

unsigned int* StringSentence[NUMBEROFSENTENCE];

extern USHORT *StringList[];
extern USHORT *Signal_StringList[];

Img_Version_Info		Info_Version_Img;

extern GUI_FONT GUI_FontYDI13017AA4;
extern GUI_FONT GUI_FontYDI13023AA4_ALL;

USHORT	cntSetup;
UCHAR	SetupFlag;
UCHAR	OldSetupFlag;

unsigned int tempAddress;

void Initialize_Image_Variable()
{
	SPI_FLASH_BufferRead((unsigned char* )&FL_Image, IMG_START, sizeof(FL_STV_Image));
	SPI_FLASH_BufferRead((unsigned char* )&tempAddress, FL_Image.String_Image_Info, sizeof(tempAddress));

	if((tempAddress >= IMG_START) && (tempAddress < IMG_END))
	{
		SPI_FLASH_BufferRead((unsigned char* )&Info_Version_Img, tempAddress, sizeof(Img_Version_Info));
		if(Info_Version_Img.Version_High > 9)
			Info_Version_Img.Version_High = 0;
		if(Info_Version_Img.Version_Low > 9)
			Info_Version_Img.Version_Low = 0;
		if(Info_Version_Img.Version_Sub> 9)
			Info_Version_Img.Version_Sub = 0;
	}
	else
	{
		Info_Version_Img.Version_High = 0xff;
		Info_Version_Img.Version_Low = 0xff;
		Info_Version_Img.Version_Sub = 0xff;	
	}

	gui_font17[0] = &GUI_FontYDI13017AA4;	
	gui_font23[0] = &GUI_FontYDI13023AA4_ALL;	

	SetLanguage(InfoDisplaySetting.Language);
	
}

void SetLanguage(unsigned char nIndex)
{
	int i;
	
	if(nIndex == STATE_DISPLAY_LANGUAGE_KOREAN)
	{
		StringSentence[0] = (unsigned int*)String_korean;
	}
	else
	{
		StringSentence[0] = (unsigned int*)String_English;
	}
	
	for( i=0;i<NUMBEROFSENTENCE-1;i++)
	{
		StringSentence[i+1] = StringSentence[i];
		StringSentence[i+1]++;
	}

}

void String_Func(unsigned short *outstring, unsigned char *instring)
{
	unsigned short i = 0;  

	for(i = 0 ; i < strlen((char*)instring) ; i++)
	{
		outstring[i] = instring[i];
	}
	outstring[i+1] = 0;
}

unsigned short GetStringMultiLineHeight(unsigned short *s1, unsigned short *s2, unsigned short *s3, unsigned short width, unsigned short FontSize)
{
	FontSize = FONTSIZE_2;
	USHORT *StringTemp[3];
	USHORT y_temp[3];
	USHORT y_offset = 0, fontHeight;
	
	StringTemp[0] = &s1[0];
	StringTemp[1] = &s2[0];
	StringTemp[2] = &s3[0];

	for(UCHAR i=0;i<3;i++)
	{
		if(StringTemp[i] != StringEtc[SIGN_NOSTRING])
		{
			switch(FontSize)
			{
			/*	case FONTSIZE_1 : 
					y_temp[i] = GetStringMultiLine(StringTemp[i], gui_font30, width); 
					fontHeight = gui_font30[0]->YSize;
					break;*/
				case FONTSIZE_2 : 
					y_temp[i] = GetStringMultiLine(StringTemp[i], gui_font23, width); 
					fontHeight = gui_font23[0]->YSize;
					break;
			}
			y_offset += (y_temp[i] * fontHeight);
		}
	}
	return y_offset;

}

void GUI_DisplayStringAtWidth(unsigned short *s, uint16_t x,uint16_t y,  uint16_t width, uint32_t Color, unsigned short FontSize)
{
	FontSize = FONTSIZE_2;
	switch(FontSize)
	{
		//case FONTSIZE_1 : DisplayStringAtWidth(s, x, y, width, gui_font30,Color); break;
		case FONTSIZE_2 : DisplayStringAtWidth(s, x, y, width, gui_font23,Color); break;
	}
}

unsigned char GUI_DisplayLeftStringAt(unsigned short *s, uint16_t x, uint16_t y, uint16_t width, uint32_t Color, unsigned short OrgSize)
{
	USHORT DotWidth = 0;

	switch(OrgSize)
	{
		case FONTSIZE_1 :
		case FONTSIZE_2 : 
			if(GetStringLen(s, gui_font23) > width)
			{
				DotWidth = GetStringLen(StringEtc[SIGN_DOT], gui_font23);
				USHORT x_Pos = DisplayStringAtWidth(s, x, y, width-DotWidth, gui_font23,Color); 
				DisplayStringAtWidth(StringEtc[SIGN_DOT], x_Pos, y, DotWidth, gui_font23,Color);
			}
			else 
				DisplayStringAtWidth(s, x, y, width-DotWidth, gui_font23,Color); 
			break;
		case FONTSIZE_3: 
			if(GetStringLen(s, gui_font23) > width)
			{
				if(GetStringLen(s, gui_font17) > width)
				{
					DotWidth = GetStringLen(StringEtc[SIGN_DOT], gui_font23);
					USHORT x_Pos = DisplayStringAtWidth(s, x, y, width-DotWidth, gui_font23,Color); 
					DisplayStringAtWidth(StringEtc[SIGN_DOT], x_Pos, y, DotWidth, gui_font23,Color);
				}
				else
				{
					DisplayStringAtWidth(s, x, y+(FONTSIZE_2-FONTSIZE_3), width-DotWidth, gui_font17,Color); 
				}
			}
			else 
				DisplayStringAtWidth(s, x, y, width-DotWidth, gui_font23,Color); 
			break;
	}

	if(DotWidth > 0)
		return 1;
	else
		return 0;
}

void GUI_DisplayRightStringAt(unsigned short *s, uint16_t x, uint16_t y,uint16_t width, uint32_t Color, unsigned short OrgSize)
{
	OrgSize = FONTSIZE_2;
	USHORT FontSize  = FONTSIZE_1;
	/*if((GetStringLen(s, gui_font30) <= width) && (OrgSize >= FONTSIZE_1))
		FontSize = FONTSIZE_1;
	else*/ if((GetStringLen(s, gui_font23) <= width) && (OrgSize >= FONTSIZE_2))
		FontSize = FONTSIZE_2;
	else
		FontSize = 0;
	
	switch(FontSize)
	{
		case 0: 	DisplayMultiLineRightStringAt(s, x, y, width, gui_font23,Color); break;
		case FONTSIZE_2 : DisplayStringRightAt(s, x, y+(OrgSize-FontSize), width, gui_font23,Color); break;
	//	case FONTSIZE_1 : DisplayStringRightAt(s, x, y+(OrgSize-FontSize), width, gui_font30,Color); break;
	}
}

unsigned short GUI_DisplayCenterStringAt(unsigned short *s, uint16_t x, uint16_t y,uint16_t width, uint32_t Color, unsigned short OrgSize)
{
	OrgSize = FONTSIZE_2;
	USHORT FontSize  = FONTSIZE_1;
	/*if((GetStringLen(s, gui_font30) <= width) && (OrgSize >= FONTSIZE_1))
		FontSize = FONTSIZE_1;
	else*/ if((GetStringLen(s, gui_font23) <= width) && (OrgSize >= FONTSIZE_2))
		FontSize = FONTSIZE_2;
	else
		FontSize = 0;
	
	switch(FontSize)
	{
		case 0:	DisplayMultiLineCenterStringAt(s, x, y, width, 0, gui_font23,Color); 		break;
		case FONTSIZE_2 : DisplayStringCenterAt(s, x, y+(OrgSize-FontSize), width, gui_font23,Color); break;
	//	case FONTSIZE_1 : DisplayStringCenterAt(s, x, y+(OrgSize-FontSize), width, gui_font30,Color); break;
	}

	return FontSize;

}

USHORT GUI_DisplayMultiStringAt(unsigned short *s, uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t Color, unsigned short OrgSize)
{
	OrgSize = FONTSIZE_2;
	switch(OrgSize)
	{
		case FONTSIZE_2 : return DisplayMultiLineStringAt(s, x, y, width, height, gui_font23,Color);
	//	case FONTSIZE_1 : return DisplayMultiLineStringAt(s, x, y, width, height, gui_font30,Color);
	}

	return 0;
}

short GUI_DisplayMultiLineCenterStringAt(unsigned short *s, uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t Color, unsigned short OrgSize)
{
	OrgSize = FONTSIZE_2;
	USHORT y_Height, fontHeight;
	USHORT fontsize[]={FONTSIZE_1,FONTSIZE_2,};
	UCHAR i;
	
	for(i=0;i<2;i++)
	{
		if(fontsize[i] <= OrgSize)
		{
			switch(fontsize[i])
			{
		/*		case FONTSIZE_1 : 
					y_Height = GetStringMultiLine(s, gui_font30, width); 
					fontHeight = gui_font30[0]->YSize;
					break;*/
				case FONTSIZE_2 : 
					y_Height = GetStringMultiLine(s, gui_font23, width); 
					fontHeight = gui_font23[0]->YSize;
					break;
			}
			if((y_Height*fontHeight) < height)
				break;
		}
	}

	switch(fontsize[i])
	{
		case FONTSIZE_2 : return DisplayMultiLineCenterStringAt(s, x, y, width, height, gui_font23,Color);
	//	case FONTSIZE_1 : return DisplayMultiLineCenterStringAt(s, x, y, width, height, gui_font30,Color);
		default : return DisplayMultiLineCenterStringAt(s, x, y, width, 0, gui_font23,Color);
	}
}

void GUI_DisplayRightStringAt_Value(unsigned short data, uint16_t x, uint16_t y,uint16_t width, uint32_t Color)
{
	UCHAR buf[10];
	USHORT covbuf[10];
	
	memset(buf,0xff,sizeof(buf));
	memset(covbuf,0xff,sizeof(covbuf));
	sprintf ( (char*)buf, "%d",data) ; 
	String_Func(covbuf, buf);

	LCD_Draw_Color(x,y, width,FONTSIZE, (Color == COLOR_WHITE)?COLOR_MENU_GREEN:COLOR_MENU_GRAY);
	GUI_DisplayRightStringAt(&covbuf[0], x, y,  width, Color, FONTSIZE);
}

void GUI_DisplayRightStringAt_Value2(unsigned short data, uint16_t x, uint16_t y,uint16_t width, uint32_t Color)
{
	UCHAR buf[10];
	USHORT covbuf[10];
	
	memset(buf,0xff,sizeof(buf));
	memset(covbuf,0xff,sizeof(covbuf));
	sprintf ( (char*)buf, "%d",data) ; 
	String_Func(covbuf, buf);

	GUI_DisplayRightStringAt(&covbuf[0], x, y,  width, Color, FONTSIZE);
}


void DisplayBackground()
{
	LCD_Draw_Color(0, 0, LCD_WIDTH, LCD_HEIGHT, COLOR_BLACK);
}

void DisplayMenuBackgroundBG()
{
	//LCD_Draw_Color(6, 48, 210, 122, COLOR_BLACK);
	LCD_Draw_Color(0, 48, LCD_WIDTH, LCD_HEIGHT-48, COLOR_BLACK);
}

void DisplayMarqueeTextView()
{
	if(m_MarInfo.SetInfo == 1)
	{
		m_MarInfo.MinX = LCD_WIDTH;
		m_MarInfo.MinY = LCD_HEIGHT;
		m_MarInfo.MaxX = 0;
		m_MarInfo.MaxY = 0;
		m_MarInfo.MaxFlag = 0;
		for(int i=0;i<m_MarInfo.cntMarText;i++)
		{

			if((m_MarText[i].PictureX+m_MarText[i].PictureWidth)> m_MarInfo.MaxX)
				m_MarInfo.MaxX = m_MarText[i].PictureX+m_MarText[i].PictureWidth;
			if(m_MarText[i].PictureX < m_MarInfo.MinX)
				m_MarInfo.MinX = m_MarText[i].PictureX;

			if((m_MarText[i].PictureY+m_MarText[i].PictureHeight)> m_MarInfo.MaxY)
				m_MarInfo.MaxY = m_MarText[i].PictureY+m_MarText[i].PictureHeight;
			if(m_MarText[i].PictureY < m_MarInfo.MinY)
				m_MarInfo.MinY = m_MarText[i].PictureY;

			if(m_MarInfo.MaxFlag < ( (m_MarText[i].MaxLength - m_MarText[i].WidthLength)+3))
				m_MarInfo.MaxFlag = ( (m_MarText[i].MaxLength - m_MarText[i].WidthLength)+3);
		}

		cntSetup = SetupFlag = 0;
		OldSetupFlag = 0xFF;				
		
		m_MarInfo.SetInfo = 2;
	}

	if((m_MarInfo.cntMarText > 0) && (m_MarInfo.SetInfo == 2))
	{
		cntSetup++;

		SetupFlag = cntSetup/3;
		

		if(OldSetupFlag != SetupFlag)
		{
			OldSetupFlag = SetupFlag;

			for(int i=0;i<m_MarInfo.cntMarText;i++)
			{
				unsigned short covbuf[100];
				USHORT flag = OldSetupFlag%m_MarInfo.MaxFlag;
				
				memset((&covbuf[0]), 0x0, sizeof( covbuf));
				if((m_MarText[i].uwStartAddress == COLOR_MENU_ORANGE) || (m_MarText[i].uwStartAddress == COLOR_MENU_GREEN)
					|| (m_MarText[i].uwStartAddress == COLOR_MENU_BLUE))
				{
					if(flag < 3)
						memcpy(&covbuf[0], &(StringList[m_MarText[i].StringIndex])[0], m_MarText[i].MaxLength*sizeof(unsigned short));
					else if(flag >= (m_MarText[i].MaxLength+3))
						memcpy(&covbuf[0], &(StringList[m_MarText[i].StringIndex])[m_MarText[i].MaxLength-1], sizeof(unsigned short));
					else
						memcpy(&covbuf[0], &(StringList[m_MarText[i].StringIndex])[flag-3], (m_MarText[i].MaxLength-(flag-3))*sizeof(unsigned short));
				
				}
				else if((ScreenIndex == SCREEN_STATE_MENU_MAINTENANCE_SIGNALSTATUS_ANALOG_LIST)
						|| (ScreenIndex == SCREEN_STATE_MENU_MAINTENANCE_SIGNALSTATUS_DIGITALINPUT_LIST)
						|| (ScreenIndex == SCREEN_STATE_MENU_MAINTENANCE_SIGNALSTATUS_DIGITALOUTPUT_LIST))
				{
					if(flag < 3)
						memcpy(&covbuf[0], &(Signal_StringList[m_MarText[i].StringIndex])[0], m_MarText[i].MaxLength*sizeof(unsigned short));
					else if(flag >= (m_MarText[i].MaxLength+3))
						memcpy(&covbuf[0], &(Signal_StringList[m_MarText[i].StringIndex])[m_MarText[i].MaxLength-1], sizeof(unsigned short));
					else
						memcpy(&covbuf[0], &(Signal_StringList[m_MarText[i].StringIndex])[flag-3], (m_MarText[i].MaxLength-(flag-3))*sizeof(unsigned short));
				}
				else
				{
					if(flag < 3)
						memcpy(&covbuf[0], &((USHORT*)*StringSentence[m_MarText[i].StringIndex])[0], m_MarText[i].MaxLength*sizeof(unsigned short));
					else if(flag >= (m_MarText[i].MaxLength+3))
						memcpy(&covbuf[0], &((USHORT*)*StringSentence[m_MarText[i].StringIndex])[m_MarText[i].MaxLength-1], sizeof(unsigned short));
					else
						memcpy(&covbuf[0], &((USHORT*)*StringSentence[m_MarText[i].StringIndex])[flag-3], (m_MarText[i].MaxLength-(flag-3))*sizeof(unsigned short));
				}
				if((flag < 1) || (flag > 3))
				{
					if(ScreenIndex == SCREEN_STATE_MENU_MAINTENANCE_MANAGEMENT_LIST)
						LCD_Draw_Color(m_MarText[i].PictureX, m_MarText[i].PictureY, m_MarText[i].StringWidth+6, m_MarText[i].PictureHeight, m_MarText[i].uwStartAddress);
					else
						LCD_Draw_Color(m_MarText[i].PictureX, m_MarText[i].PictureY, m_MarText[i].PictureWidth, m_MarText[i].PictureHeight, m_MarText[i].uwStartAddress);
					GUI_DisplayStringAtWidth(covbuf, m_MarText[i].StringX, m_MarText[i].StringY, m_MarText[i].StringWidth, m_MarText[i].StringColor, m_MarText[i].FontSize);
				}
			}
		}
	}
	else
	{
		cntSetup = SetupFlag = 0;
		OldSetupFlag = 0xFF;				
	}
	
}
