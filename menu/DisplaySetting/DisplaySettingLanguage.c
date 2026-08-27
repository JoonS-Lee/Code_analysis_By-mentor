#include "FLV_Cluster_APP.h"

#if 0
#define STATE_DISPLAY_LANGUAGE_KOREAN 			        0    // 한국어
#define STATE_DISPLAY_LANGUAGE_ENGLISH   		        1    // 영어
#endif

extern int* StringLanguage[];

void DisplayLanguageList()
{
	if(OldScreenIndex != SCREEN_STATE_MENU_DISPLAYSETTING_LANGUAGE_CHANGE)
	{
		OldCurserIndex = CurserIndex;

		SetSubListString(NUMBEROFLANGUAGE, CurserIndex,String_Language[STATE_DISPLAY_LANGUAGE_KOREAN], 
					String_Language[STATE_DISPLAY_LANGUAGE_ENGLISH], StringEtc[SIGN_NOSTRING], 
					StringEtc[SIGN_NOSTRING], StringEtc[SIGN_NOSTRING], StringEtc[SIGN_NOSTRING]);
	}	
	else
	{
		DisplaySubDoubleList(CurserIndex);		
	}
}


