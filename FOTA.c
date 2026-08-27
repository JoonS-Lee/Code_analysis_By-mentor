#include "main.h"
#include "FLV_Cluster_APP.h"
#include <string.h>

#define AppAddress     0x08020000
#define BootAddress    0x08000000

FOTA_FW_DATA* RX_FOTA_FW_DATA;
TP_CM* TP_CM_RTS;
FOTA_FW_INFO_PACKET* TX_FW_INFO_DATA;
FOTA_FW_INFO_FLASH* CPU_FW_INFO_DATA;
FOTA_FW_INFO_PACKET* RX_FW_INFO_DATA;

unsigned short FW_SectionInFile=0;
unsigned int sflash_addr;

extern unsigned short TotalPacketNum;
extern unsigned short TotPacketNum;
extern unsigned short TotMsgSize;
extern unsigned int pgn;

unsigned char tmpFWDatabuf[11 + 1024];
unsigned short CANUpdateCommand;
unsigned short CANUpdateFWID;		// ++, --, 200319 bwk

unsigned char CANUpdatePF = 0;
unsigned char CANUpdatePS = 0;
unsigned char CANUpdateTotalPacketNum = 0;
unsigned short CANUpdateTotalMsgSize = 0;


unsigned char CANUpdateData[255][7];
unsigned char CANUpdateDataIndex[255];
unsigned char CANUpdateAckData[8];
unsigned char CANUpdateCTSData[8];
unsigned char CANUpdateRTSData[8];
unsigned char CANUpdateRTSFlag = 0;
unsigned char CANUpdateCTSFlag = 0;
unsigned char CANUpdateACKFlag = 0;
unsigned char CANUpdateSendRTSFlag = 0;
unsigned short CANUpdateSectionIndex = 0; //++,--, 201218 ysm, CAN UPDATE
unsigned char CANUpdatePacketIndex = 0;
unsigned char CANUpdateCRCCount = 0;
unsigned short CanUpdateTimeoutTimer = 0;
unsigned short CanUpdateTimeoutCount = 0;

// ++, 150722 sys
unsigned char CanUpdateCompleteflag=0;
unsigned short CanUpdateCompleteTimer=0;
// --, 150722 sys
unsigned char Start_CAN_Update;
unsigned char tmp_fw_info[72 + 600];
unsigned char tmp_fw_info1[72 + 600];
unsigned char tmpFWInfobuf[77+600];			// 84->77  section crc packet add
unsigned short CalCRC_temp;
unsigned char temp_crc[4096];			// Check Total CRC
unsigned char Receive_Info[77+600];

unsigned char tmpRTSBuf[8];
unsigned char TotalPacketNum_CanUpdate;

unsigned char CANCompleteRTSFlag = 0;
unsigned char Start_TX_FW_INFO;

// ++, 150813 sys
unsigned char Flag_CAN_Update_Start;
// --, 150813 sys

//CAN UPDATE
unsigned char CAN_Update_flag;



#define FILE_NAME_LENGTH        (256)
#define FILE_SIZE_LENGTH        (16)

unsigned char file_name[FILE_NAME_LENGTH];
unsigned int FlashDestination = AppAddress; /* Flash user program offset */
unsigned int PageSize = PAGE_SIZE;
unsigned int EraseCounter = 0x0;
unsigned int NbrOfPage = 0;

unsigned int RamSource;

unsigned char temp_read_serial_flash[10];
unsigned char temp_dl_code;

FOTA_FW_INFO_CRC* READ_FW_INFO_DATA;

unsigned int sflash_address;
unsigned int index;
//unsigned char tab_1024[1024]={0};               //Serial Flash Buffer Data

unsigned char internal_tmp_fw_info[72 + 600];
unsigned char fw_info_code[72 + 600]={0,};

extern unsigned char Start_TX_FW_INFO;

//unsigned char temp_1024[1024]={0,};             //Check Flash CRC
unsigned char temp_1025[4096]={0,};             //Check CPU CRC
#if 0

#define AppAddress     0x0800A000
#define BootAddress    0x08000000

pFunction Jump_To_Application;
unsigned int JumpAddress;

void JumpAPP(void)
{
	/* Jump to user application */

	HAL_DeInit();
	JumpAddress = *(__IO uint32_t*) (AppAddress + 4);
	Jump_To_Application = (pFunction) JumpAddress;
	/* Initialize user application's Stack Pointer */

	__set_MSP(*(__IO uint32_t*) AppAddress);
	Jump_To_Application();
}

void JumpIAP(void)
{
	/* Jump to user application */
	HAL_DeInit();
	JumpAddress = *(__IO uint32_t*) (BootAddress+ 4);
	Jump_To_Application = (pFunction) JumpAddress;
	/* Initialize user application's Stack Pointer */

	__set_MSP(*(__IO uint32_t*) BootAddress);
	Jump_To_Application();
}
#endif

void Initialize_FOTA_CAN_Variable(void)
{
	memset(&tmpRTSBuf, 0xFF, sizeof(TP_CM));
	memset(&tmpFWInfobuf, 0xFF, sizeof(FOTA_FW_INFO_PACKET));
	memset(&tmpFWDatabuf, 0xFF, sizeof(FOTA_FW_DATA));
	memset(&Receive_Info, 0xFF, sizeof(FOTA_FW_INFO_PACKET));

	TP_CM_RTS = (TP_CM*)&tmpRTSBuf[0];
	TX_FW_INFO_DATA = (FOTA_FW_INFO_PACKET*)&tmpFWInfobuf[0];

	RX_FOTA_FW_DATA = (FOTA_FW_DATA*)&tmpFWDatabuf[0];
	RX_FW_INFO_DATA = (FOTA_FW_INFO_PACKET*)&Receive_Info[0];

	CANCompleteRTSFlag=Start_TX_FW_INFO = 0;
	Flag_CAN_Update_Start = 0;

	memset(&tmp_fw_info1[0], 0xFF, sizeof(FOTA_FW_INFO_CRC));
	READ_FW_INFO_DATA = (FOTA_FW_INFO_CRC*)&tmp_fw_info1[0];

	CAN_Update_flag = 0;
}


void CAN_UPDATE_Operation(void)
{
	if(CANUpdateRTSFlag == 1)
	{
		SendMultiPacketCTS(7,SA_CANUPDATE,SA_FLV,&CANUpdateRTSData[0]);
		CANUpdateRTSFlag = 0;
	}
	switch(CANUpdateCommand)
	{
	case 0x0020:	// Request F/W #N Info
		SendFWInfo();	
		break;

		// ++, 150819 sys
	case 0x0045 :  // UPD Format
		for(int i = 0; i < 64; i++)
		{                 
			SPI_FLASH_SectorErase(i * 0x10000);           
			SendUPD_Format_Status(100*i/63);
			CAN2_TX_RingBuffer();
		}
		CANUpdateSectionIndex = 0;
		CANUpdatePacketIndex = 0;
		CANUpdateCRCCount = 0;
		CANUpdateCommand=0;

		SendUPD_Format_Complete();

		break;

	case 0x0046 :  // APP Down Cancel
		CANUpdateCommand=0;
		SendFWDLComplete(0x51);
		break;
		// --, 150819 sys

	case 0x0041:	// Enter DL Mode   
		CANUpdateCommand = 0;
		break;
	case 0x0042:	// Send New F/W #N Info
		SendMultiPacketAck(7,SA_CANUPDATE,SA_FLV,&CANUpdateRTSData[0]);
		SendAckNewFWInfo();
		CANUpdateCommand = 0;
		CANUpdateSendRTSFlag = 0;
		CANUpdateCTSFlag = 0;
		break;
	case 0x0040:	// App #N DL Start
		RequestAppPacket();
		//CANUpdateCRCCount = 0;
		break;
	case 0x0043:	// Send Packet # M
		SendMultiPacketAck(7,SA_CANUPDATE,SA_FLV,&CANUpdateRTSData[0]);
		ReceiveAppPacket();
		break;
	case 0x0051:	// Quit DL Mode
		CANUpdateCommand = 0;
		break;
	case 0x0060:	// FW Update Start
		CANUpdateCommand = 0;
		SCB->VTOR = FLASH_BASE;
		NVIC_SystemReset();
		break;
	default : 
		break;
	}

	// ++, 150722 sys
	if(CanUpdateCompleteflag == 1)
	{
		if(++CanUpdateCompleteTimer >= 3000)
		{
			CanUpdateCompleteTimer = 3000;
			SCB->VTOR = BootAddress;
			NVIC_SystemReset();
		}
	}
	// --, 150722 sys
}

void ReceiveAppPacket(void)
{
	if(CANUpdateSectionIndex < RX_FW_INFO_DATA->FW_INFO.COU_FWINFO.FW_SECTION_IN_A_FILE)
	{
		if(CANUpdatePacketIndex < RX_FW_INFO_DATA->FW_INFO.COU_FWINFO.FW_PACKET_IN_A_SECTION)
		{
			if(CheckPacketCRC() == 1)
			{
				CANUpdatePacketIndex++;
				CANUpdateCRCCount = 0;
			}
			else
				CANUpdateCRCCount++;
		}


		if(CANUpdatePacketIndex >= RX_FW_INFO_DATA->FW_INFO.COU_FWINFO.FW_PACKET_IN_A_SECTION)
		{
			CANUpdatePacketIndex = 0;
			CANUpdateSectionIndex++;
		}

		if(CANUpdateCRCCount >= 5)
		{
			CANUpdateCommand = 0;
			CANUpdateSectionIndex = 0;
			CANUpdatePacketIndex = 0;
			CANUpdateCRCCount = 0;
			// ++, 150908 sys
#if 0
			SendFWDLComplete(0x42);
#else
			SendFWDLComplete(0x52);
#endif
			// --, 150908 sys
		}
		else
		{
			CANUpdateCommand = 0x0040;
		}
	}


	if(CANUpdateSectionIndex >= RX_FW_INFO_DATA->FW_INFO.COU_FWINFO.FW_SECTION_IN_A_FILE - 1)
	{
		//CANUpdateSectionIndex--;
		//if(CheckPacketCRC() == 1)
		if(CANUpdatePacketIndex > 0)
		{
			//            SendMultiPacketAck(7,SA_CANUPDATE,SA_FLV,&CANUpdateRTSData[0]);

			CANUpdateSectionIndex = 0;
			CANUpdatePacketIndex = 0;
			CANUpdateCommand = 0;
		}

	}
}

void SendMultiPacketRTS(unsigned char _Priority, unsigned char _DA, unsigned char _SA, unsigned char _PF, unsigned char _PS, unsigned short _SIZE)
{
	unsigned char _TotalPacketNum;
	unsigned char _Temp[8];
	struct st_CAN_Message1 Message;

	CANUpdateACKFlag = 0;

	if(_SIZE % 7 == 0){
		_TotalPacketNum = _SIZE / 7;
	}else{
		_TotalPacketNum = (_SIZE / 7) + 1;
	}

	Message.Priority = _Priority;
	Message.Data_Page=0;
	Message.PDU_Format = 0xEC;
	Message.PDU_Specific = _DA;
	Message.Source_Address = _SA;

	_Temp[0] = 0x10;
	_Temp[1] = _SIZE & 0x00FF;
	_Temp[2] = ((_SIZE & 0xFF00) >> 8);
	_Temp[3] = _TotalPacketNum;
	_Temp[4] = 0xFF;
	_Temp[5] = _PS;
	_Temp[6] = _PF;
	_Temp[7] = 0x00;

	memcpy((void *)&Message.Data[0],(void *)&_Temp, sizeof(_Temp));

	CAN2_OperateRingBuffer(Message);
}

void SendMultiPacketCTS(unsigned char _Priority, unsigned char _DA, unsigned char _SA, unsigned char* _Data)
{
	unsigned char _Temp[8];
	struct st_CAN_Message1 Message;

	Message.Priority = _Priority;
	Message.Data_Page=0;
	Message.PDU_Format = 0xEC;
	Message.PDU_Specific = _DA;
	Message.Source_Address = _SA;

	_Temp[0] = 	0x11;
	_Temp[1] = 	CANUpdateTotalPacketNum;
	_Temp[2] = 1;
	_Temp[3] = 0xFF;
	_Temp[4] = 0xFF;
	_Temp[5] = _Data[5];
	_Temp[6] = _Data[6];
	_Temp[7] = _Data[7];

	memcpy((void *)&Message.Data[0],(void *)&_Temp, sizeof(_Temp));

	CAN2_OperateRingBuffer(Message);
}


void SendMultiPacketAck(unsigned char _Priority, unsigned char _DA, unsigned char _SA, unsigned char* _Data)
{
	unsigned char _Temp[8];
	struct st_CAN_Message1 Message;

	Message.Priority = _Priority;
	Message.Data_Page=0;
	Message.PDU_Format = 0xEC;
	Message.PDU_Specific = _DA;
	Message.Source_Address = _SA;

	_Temp[0] = 	0x13;
	_Temp[1] = _Data[1];
	_Temp[2] = _Data[2];
	_Temp[3] = _Data[3];
	_Temp[4] = 0xff;
	_Temp[5] = _Data[5];
	_Temp[6] = _Data[6];
	_Temp[7] = _Data[7];

	memcpy((void *)&Message.Data[0],(void *)&_Temp, sizeof(_Temp));

	CAN2_OperateRingBuffer(Message);
}


void SendAckNewFWInfo(void)
{
	unsigned char _Temp[8];
	struct st_CAN_Message1 Message;

	Message.Priority = 6;
	Message.Data_Page=0;
	Message.PDU_Format = 0xEF;
	Message.PDU_Specific = SA_CANUPDATE;
	Message.Source_Address = SA_FLV;

	_Temp[0] = CANUPDATE_MESSAGETYPE;
	_Temp[1] = 0x52;
	_Temp[2] = 0x00;
	_Temp[3] = FW_SectionInFile;
	_Temp[4] = 0xFF;
	_Temp[5] = 0xFF;
	_Temp[6] = 0xFF;
	_Temp[7] = 0xFF;

	memcpy((void *)&Message.Data[0],(void *)&_Temp, sizeof(_Temp));

	CAN2_OperateRingBuffer(Message);
}


void RequestAppPacket_DT(unsigned short _SectionNumber, unsigned short _PacketNumber)
{
	unsigned char _Temp[8];
	struct st_CAN_Message1 Message;
	unsigned char _Temp2[8];
	struct st_CAN_Message1 Message2;

	Message.Priority = 7;
	Message.Data_Page=0;
	Message.PDU_Format = 0xEB;
	Message.PDU_Specific = SA_CANUPDATE;
	Message.Source_Address = SA_FLV;

	_Temp[0] = 1;
	_Temp[1] = CANUPDATE_MESSAGETYPE;
	_Temp[2] = 0x53;
	_Temp[3] = 0x00;
	_Temp[4] = 0;
	_Temp[5] = 0;
	_Temp[6] = _SectionNumber & 0x00FF;
	_Temp[7] = ((_SectionNumber & 0xFF00) >> 8);


	memcpy((void *)&Message.Data[0],(void *)&_Temp, sizeof(_Temp));

	CAN2_OperateRingBuffer(Message);

	Message2.Priority = 7;
	Message2.Data_Page=0;
	Message2.PDU_Format = 0xEB;
	Message2.PDU_Specific = SA_CANUPDATE;
	Message2.Source_Address = SA_FLV;

	_Temp2[0] = 2;
	_Temp2[1] = _PacketNumber & 0x00FF;
	_Temp2[2] = ((_PacketNumber & 0xFF00) >> 8);
	_Temp2[3] = 0xFF;
	_Temp2[4] = 0xFF;
	_Temp2[5] = 0xFF;
	_Temp2[6] = 0xFF;
	_Temp2[7] = 0xFF;

	memcpy((void *)&Message2.Data[0],(void *)&_Temp2, sizeof(_Temp2));

	CAN2_OperateRingBuffer(Message2);
}

void SendUPD_Format_Status(unsigned char _Progress)
{
	unsigned char _Temp[8];
	struct st_CAN_Message1 Message;

	Message.Priority = 6;
	Message.Data_Page=0;
	Message.PDU_Format = 0xEF;
	Message.PDU_Specific = SA_CANUPDATE;
	Message.Source_Address = SA_FLV;

	_Temp[0] = CANUPDATE_MESSAGETYPE;
	_Temp[1] = 0x54;
	_Temp[2] = 0x00;
	_Temp[3] = 0xFF;
	_Temp[4] = 0x01;
	_Temp[5] = _Progress;
	_Temp[6] = 0xFF;
	_Temp[7] = 0xFF;

	memcpy((void *)&Message.Data[0],(void *)&_Temp, sizeof(_Temp));

	CAN2_OperateRingBuffer(Message);
}

void SendUPD_Format_Complete(void)
{
	unsigned char _Temp[8];
	struct st_CAN_Message1 Message;

	Message.Priority = 6;
	Message.Data_Page=0;
	Message.PDU_Format = 0xEF;
	Message.PDU_Specific = SA_CANUPDATE;
	Message.Source_Address = SA_FLV;

	_Temp[0] = CANUPDATE_MESSAGETYPE;
	_Temp[1] = 0x55;
	_Temp[2] = 0x00;
	_Temp[3] = 0xFF;
	_Temp[4] = 0xFF;
	_Temp[5] = 0xFF;
	_Temp[6] = 0xFF;
	_Temp[7] = 0xFF;


	memcpy((void *)&Message.Data[0],(void *)&_Temp, sizeof(_Temp));

	CAN2_OperateRingBuffer(Message);
}

void SendFWDLComplete(unsigned char _ResultCRC)
{
	unsigned char _Temp[8];
	struct st_CAN_Message1 Message;

	Message.Priority = 6;
	Message.Data_Page=0;
	Message.PDU_Format = 0xEF;
	Message.PDU_Specific = SA_CANUPDATE;
	Message.Source_Address = SA_FLV;

	_Temp[0] = CANUPDATE_MESSAGETYPE;
	_Temp[1] = 0x50;
	_Temp[2] = 0x00;
	_Temp[3] = 0xFF;
	_Temp[4] = _ResultCRC;
	_Temp[5] = 0xFF;
	_Temp[6] = 0xFF;
	_Temp[7] = 0xFF;

	memcpy((void *)&Message.Data[0],(void *)&_Temp, sizeof(_Temp));

	CAN2_OperateRingBuffer(Message);
}

void SendFWInfo(void)
{	
	if(CanUpdateTimeoutTimer % 500 == 0)
	{
		if(CanUpdateTimeoutCount < 5)
		{
			CanUpdateTimeoutCount++;
			Start_CAN_Update = 1;
			SendTP_CM_RTS_FW_Info();
		}
		else
		{
			CanUpdateTimeoutCount = 0;
			CANUpdateCommand = 0;
			CANUpdateCTSFlag = 0;
		}
	}

	if(CANUpdateCTSFlag == 1)
	{
		CANUpdateCTSFlag = 0;
		memset((unsigned char*)&TX_FW_INFO_DATA->FW_INFO.tempbuf,0x00,72);

		//FLASH_If_Init();

// ++, 200319 bwk
#if 0
		if(CheckCPUCRC())
		{
			unsigned int CPU_FwInfo_Address;

			unsigned char FW_Name1[18] = "FLV";
			unsigned char FW_Name2[18] = "FLV_IMG";
			unsigned char FW_Name3[18] = "FLV_LAN";
			
			if(memcmp(RX_FW_INFO_DATA->FW_INFO.COU_FWINFO.FW_NAME, FW_Name1, 18) == 0)
				CPU_FwInfo_Address = FIRMWAREINFO_ADDRESS;
			else if(memcmp(RX_FW_INFO_DATA->FW_INFO.COU_FWINFO.FW_NAME, FW_Name2, 18) == 0)
				CPU_FwInfo_Address = IMAGEINFO_ADDRESS;
			else if(memcmp(RX_FW_INFO_DATA->FW_INFO.COU_FWINFO.FW_NAME, FW_Name3, 18) == 0)
				CPU_FwInfo_Address = LANGUAGEINFO_ADDRESS;
			else
				return;

			FLASH_If_Read(&CPU_FwInfo_Address,&TX_FW_INFO_DATA->FW_INFO.tempbuf[0],72);
		}
		else
		{
			memset(&TX_FW_INFO_DATA->FW_INFO.tempbuf[0],0xFF,72);
			TX_FW_INFO_DATA->FW_INFO.tempbuf[0] = SA_FLV;
			TX_FW_INFO_DATA->FW_INFO.tempbuf[1] = 0x00;
		}
#else
		unsigned int CPU_FwInfo_Address;
		if(CANUpdateFWID == 1)
			CPU_FwInfo_Address = FIRMWAREINFO_ADDRESS;
		else if(CANUpdateFWID == 2)
			CPU_FwInfo_Address = IMAGEINFO_ADDRESS;
		else
			return;
		FLASH_If_Read(&CPU_FwInfo_Address,&TX_FW_INFO_DATA->FW_INFO.tempbuf[0],72);
		if((TX_FW_INFO_DATA->FW_INFO.tempbuf[0] == 0xff) && (TX_FW_INFO_DATA->FW_INFO.tempbuf[1] == 0xff))
		{
			TX_FW_INFO_DATA->FW_INFO.tempbuf[0] = SA_FLV;
			TX_FW_INFO_DATA->FW_INFO.tempbuf[1] = 0x00;
		}
#endif
// --, 200319 bwk
		

		int i;
		for(i = 0; i < 11; i++)
		{
			SendMultiPacketData_FWInfo(i+1);
		}
	}

	if(CANUpdateACKFlag == 1)
	{
		CANUpdateCommand = 0;
		CANUpdateACKFlag = 0;
		CanUpdateTimeoutCount = 0;
	}

	CanUpdateTimeoutTimer++;
}

void SendTP_CM_RTS_FW_Info(void)
{
	struct st_CAN_Message1 Message;

	Message.Priority = 7;
	Message.Data_Page = 0;
	Message.PDU_Format = 0xEC;
	Message.PDU_Specific = SA_CANUPDATE;
	Message.Source_Address = SA_FLV;

	TP_CM_RTS->Control = 16;	// Control Byte
	TP_CM_RTS->TotMsgSize = 77;	// 
	TP_CM_RTS->TotPacketNum = 11;
	TP_CM_RTS->Reserved = 0xFF;
	TP_CM_RTS->pgn_low = 0xEF00;
	TP_CM_RTS->pgn_high = 0;

	memcpy((void *)&Message.Data[0],(void *)&tmpRTSBuf, sizeof(tmpRTSBuf));

	CAN2_OperateRingBuffer(Message);

	TX_FW_INFO_DATA->Message_Type = 0xFE;
	TX_FW_INFO_DATA->Command = 0x0030;		// Send F/WInformation
	TotalPacketNum_CanUpdate = 0;
}

void SendTP_CM_RTS_FW_Info_Update_Complete(void)
{
	struct st_CAN_Message1 Message;

	Message.Priority = 7;
	Message.Data_Page=0;
	Message.PDU_Format = 0xEC;
	Message.PDU_Specific = SA_CANUPDATE;
	Message.Source_Address = SA_FLV;

	TP_CM_RTS->Control = 16;	// Control Byte
	TP_CM_RTS->TotMsgSize = 77;	// 
	TP_CM_RTS->TotPacketNum = 11;
	TP_CM_RTS->Reserved = 0xFF;
	TP_CM_RTS->pgn_low = 0xEF00;
	TP_CM_RTS->pgn_high = 0;

	memcpy((void *)&Message.Data[0],(void *)&tmpRTSBuf, sizeof(tmpRTSBuf));

	CAN2_OperateRingBuffer(Message);

	TX_FW_INFO_DATA->Message_Type = 0xFE;
	TX_FW_INFO_DATA->Command = 0x0070;		// Send App Update Complete
	TX_FW_INFO_DATA->RESERVED = 0x41FF; 
	TotalPacketNum_CanUpdate = 0;
}
// --, 150820 sys

void SendMultiPacketData_FWInfo(unsigned char packet_no)
{
	struct st_CAN_Message1 Message;

	Message.Priority = 7;
	Message.Data_Page = 0;
	Message.PDU_Format = 0xEB;
	Message.PDU_Specific = SA_CANUPDATE;
	Message.Source_Address = SA_FLV;

	Message.Data[0] = packet_no;

	memcpy((void *)&Message.Data[1],(void *)&tmpFWInfobuf[(packet_no-1)*7], 7);

	CAN2_OperateRingBuffer(Message);
}

void RequestAppPacket(void)
{
	if(CanUpdateTimeoutTimer % 500 == 0)
	{
		if(CanUpdateTimeoutCount < 5)
		{
			CanUpdateTimeoutCount++;	
			CANUpdateCRCCount++;
			SendMultiPacketRTS(7, SA_CANUPDATE, SA_FLV, 0xEF, 0x00, 9);
		}
		else if(CanUpdateTimeoutCount >= 5 || CANUpdateCRCCount >= 5)
		{
			CanUpdateTimeoutCount = 0;
			CANUpdateCommand = 0;
			CANUpdateCTSFlag = 0;
			CANUpdateCRCCount = 0;
			// ++, 150908 sys
#if 0
			SendFWDLComplete(0x42);
#else
			SendFWDLComplete(0x52);
#endif
			// --, 150908 sys
		}
	}

	if(CANUpdateCTSFlag == 1)
	{
		CANUpdateCTSFlag = 0;
		RequestAppPacket_DT(CANUpdateSectionIndex, CANUpdatePacketIndex);
	}

	if(CANUpdateACKFlag == 1)
	{
		CANUpdateACKFlag = 0;
		CanUpdateTimeoutCount = 0;
	}

	CanUpdateTimeoutTimer++;

}

int CheckCPUCRC(void)
{
	unsigned char i;
	unsigned short CalCRC;

	unsigned int flashdestination = AppAddress;
	unsigned int fwinfo_flashdestnation;

	unsigned char FW_Name1[18] = "FLV";
	unsigned char FW_Name2[18] = "FLV_IMG";
	
	if(memcmp(RX_FW_INFO_DATA->FW_INFO.COU_FWINFO.FW_NAME, FW_Name1, 18) == 0)
		fwinfo_flashdestnation = FIRMWAREINFO_ADDRESS;
	else if(memcmp(RX_FW_INFO_DATA->FW_INFO.COU_FWINFO.FW_NAME, FW_Name2, 18) == 0)
		fwinfo_flashdestnation = IMAGEINFO_ADDRESS;
	else
		return 0;

	CPU_FW_INFO_DATA = (FOTA_FW_INFO_FLASH*)&tmp_fw_info[0];
	memset(&tmp_fw_info[0], 0xff, sizeof(FOTA_FW_INFO));

	FLASH_If_Read(&fwinfo_flashdestnation,&tmp_fw_info[0],672);

	for(i = 0; i < CPU_FW_INFO_DATA->FW_INFO.COU_FWINFO.FW_SECTION_IN_A_FILE - 1; i++)
	{
		FLASH_If_Read(&flashdestination, &temp_1025[0], 4096);

		CalCRC = MakeCrc16(temp_1025, 4096);

		if(CalCRC != CPU_FW_INFO_DATA->SecTion_CRC[i])
		{
			return 0;
		}

	}
	return 1;
}

int CheckPacketCRC(void)
{
	unsigned short CalCRC;

	if((CANUpdateSectionIndex==RX_FOTA_FW_DATA->Section_Num)&&(CANUpdatePacketIndex==RX_FOTA_FW_DATA->Packet_Num))
	{
		CalCRC = MakeCrc16(RX_FOTA_FW_DATA->FW_DATA, RX_FOTA_FW_DATA->Packet_length);

		if(CalCRC == RX_FOTA_FW_DATA->FW_DATA_CRC)
		{
			if(CANUpdateSectionIndex >= RX_FW_INFO_DATA->FW_INFO.COU_FWINFO.FW_SECTION_IN_A_FILE-1)
			{
				WriteFlash();
				if(CheckTotalCRC())
				{
					// ++, 150722 sys
					CanUpdateCompleteflag=1;
					// --, 150722 sys
					WriteFWInfo();
					SendFWDLComplete(0x41);
					return 1;
				}
				else
				{
					// ++, 150722 sys
					CanUpdateCompleteflag=0;
					// --, 150722 sys
					CANUpdateCommand = 0;
					CANUpdateSectionIndex = 0;
					CANUpdatePacketIndex = 0;
					CANUpdateCRCCount = 0;
					SendFWDLComplete(0x42);
				}

			}
			else
			{
				WriteFlash();
				return 1;
			}

		}

	}	

	return 0;
}

int CheckTotalCRC(void)
{
	unsigned short i;

	for(i = 0; i < RX_FW_INFO_DATA->FW_INFO.COU_FWINFO.FW_SECTION_IN_A_FILE - 1; i++)
	{
		SPI_FLASH_BufferRead(&temp_crc[0], UPD_FW_BIN + (i<<12), 4096);  
		CalCRC_temp = MakeCrc16(temp_crc, 4096);

		if(CalCRC_temp != RX_FW_INFO_DATA->SecTion_CRC[i])
		{
			return 0;
		}
	}
	return 1;
}

unsigned int FLASH_If_Read(volatile unsigned int* FlashAddress, unsigned char* Data ,unsigned int DataLength)
{
	unsigned int i = 0;

	for (i = 0;i <DataLength; i++)
	{
		Data[i] = (*((volatile unsigned char *) (*FlashAddress))) ;
		*FlashAddress += 1;
	}

	return 1;
}

void EraseSerialFlash(void)
{
	FW_SectionInFile = RX_FW_INFO_DATA->FW_INFO.COU_FWINFO.FW_SECTION_IN_A_FILE;

	//for(i=0;i<(FW_SectionInFile/64)+1;i++)
	//	SPI_FLASH_SectorErase(UPD_FW_BIN+(0x10000*i)); 	
}

int WriteFlash(void)
{
	unsigned int sflash_addr = UPD_FW_BIN + ((CANUpdateSectionIndex * 4 + CANUpdatePacketIndex) * RX_FW_INFO_DATA->FW_INFO.COU_FWINFO.FW_PACKET_UNIT_SIZE);	
	SPI_FLASH_BufferWrite(RX_FOTA_FW_DATA->FW_DATA, sflash_addr, RX_FW_INFO_DATA->FW_INFO.COU_FWINFO.FW_PACKET_UNIT_SIZE);

	return 1;
}

int WriteFWInfo(void)
{
	//    WriteDL_Code(2);
	memcpy((void *)&fw_info_code[0], (void *)&RX_FW_INFO_DATA->FW_INFO.tempbuf[0], 672);
	SPI_FLASH_BufferWrite(fw_info_code, UPD_FW_INFO, 679); // Download Enable Command & file size 


	SPI_FLASH_BufferRead(&tmp_fw_info1[0], UPD_FW_INFO, sizeof(FOTA_FW_INFO_CRC));

	WriteDL_Code(2);

	return 0;
}

void WriteDL_Code(unsigned char Data)
{
	unsigned char download_enable_code1[10]="CAN Update";
	unsigned char download_enable_code2[10]="IMG Update";
	unsigned char FW_Name1[18] = "FLV";
	unsigned char FW_Name2[18] = "FLV_IMG";
	unsigned char DL_Code=0;
	unsigned char retry_DL_Code=0,Wirite_DL_CODE_Complete=0;

	DL_Code = Data;

	//    SPI_FLASH_SectorErase(UPD_DL_CODE);

	if(memcmp(READ_FW_INFO_DATA->FW_NAME, FW_Name1, 18) == 0)
		SPI_FLASH_PageWrite(download_enable_code1, UPD_DL_CODE, 10); // Download Enable Command & file size
	else if(memcmp(READ_FW_INFO_DATA->FW_NAME, FW_Name2, 18) == 0)
		SPI_FLASH_PageWrite(download_enable_code2, UPD_DL_CODE, 10); // Download Enable Command & file size
	else
		return ;

	SPI_FLASH_PageWrite(&DL_Code,UPD_DL_CODE+100,1); // Download Enable Command & file size 

	SPI_FLASH_BufferRead(temp_read_serial_flash,UPD_DL_CODE,10);

	SPI_FLASH_BufferRead(&temp_dl_code,UPD_DL_CODE+100,1);

	if(memcmp(READ_FW_INFO_DATA->FW_NAME, FW_Name1, 18) == 0)
		Wirite_DL_CODE_Complete = memcmp(download_enable_code1, temp_read_serial_flash,10);
	else if(memcmp(READ_FW_INFO_DATA->FW_NAME, FW_Name2, 18) == 0)
		Wirite_DL_CODE_Complete = memcmp(download_enable_code2, temp_read_serial_flash,10);
	else
		return ;

	while(Wirite_DL_CODE_Complete) //0 -> same 1->different
	{
		if(retry_DL_Code++<5)
		{
			SPI_FLASH_SectorErase(UPD_DL_CODE);

			if(memcmp(READ_FW_INFO_DATA->FW_NAME, FW_Name1, 18) == 0)
				SPI_FLASH_PageWrite(download_enable_code1, UPD_DL_CODE, 10); // Download Enable Command & file size
			else if(memcmp(READ_FW_INFO_DATA->FW_NAME, FW_Name2, 18) == 0)
				SPI_FLASH_PageWrite(download_enable_code2, UPD_DL_CODE, 10); // Download Enable Command & file size
			else
				return ;

			SPI_FLASH_BufferRead(temp_read_serial_flash,UPD_DL_CODE,10);

			if(memcmp(READ_FW_INFO_DATA->FW_NAME, FW_Name1, 18) == 0)
				Wirite_DL_CODE_Complete = memcmp(download_enable_code1,temp_read_serial_flash,10);
			else if(memcmp(READ_FW_INFO_DATA->FW_NAME, FW_Name2, 18) == 0)
				Wirite_DL_CODE_Complete = memcmp(download_enable_code2,temp_read_serial_flash,10);
			else
				return ;

			if(Wirite_DL_CODE_Complete==0)	
			{
				SPI_FLASH_PageWrite(&DL_Code,UPD_DL_CODE+100,1); // Download Enable Command & file size
				break;
			}
		}
		else
		{
			Wirite_DL_CODE_Complete=0;
			DL_Code=3;
			SPI_FLASH_SectorErase(UPD_DL_CODE);
			SPI_FLASH_PageWrite(&DL_Code,UPD_DL_CODE+100,1); // Download Enable Command & file size 
		}
	}
}

void CAN_Parsing_CANUpdate(CanRxMsg* RxMessage_temp)
{
	unsigned char PF;
	unsigned char PS;
	unsigned char MessageType;
	unsigned char Command;
	PF = (RxMessage_temp->ExtId  & 0x00ff0000) >> 16;
	PS = (RxMessage_temp->ExtId  & 0x0000ff00) >> 8;
	if(PS == SA_FLV)
	{
		switch(PF)
		{
		case 0xEC:		//Multi Packet TP.CM_BAM
			if(RxMessage_temp->Data[0] == 0x20) // ControlByte
			{
				CANUpdatePF = RxMessage_temp->Data[6];
				CANUpdatePS = RxMessage_temp->Data[5];
				CANUpdateTotalPacketNum = RxMessage_temp->Data[3];
				CANUpdateTotalMsgSize = (RxMessage_temp->Data[2] << 8) | RxMessage_temp->Data[1];
				memset((unsigned char*)CANUpdateDataIndex,0xFF,sizeof(CANUpdateDataIndex));
			}
			else if(RxMessage_temp->Data[0] == 0x10) // ControlByte	RTS
			{
				CANUpdatePF = RxMessage_temp->Data[6];
				CANUpdatePS = RxMessage_temp->Data[5];
				CANUpdateTotalPacketNum = RxMessage_temp->Data[3];
				CANUpdateTotalMsgSize = (RxMessage_temp->Data[2] << 8) | RxMessage_temp->Data[1];

				memset((unsigned char*)CANUpdateDataIndex,0xFF,sizeof(CANUpdateDataIndex));
				memcpy((unsigned char*)&CANUpdateRTSData[0],(unsigned char*)&RxMessage_temp->Data[0],sizeof(CANUpdateRTSData));
				CANUpdateAckData[0] = 0x13;
				// ++, 150820 sys
#if 0
				CANUpdateRTSFlag = 1;		
#else
				if(Flag_CAN_Update_Start)
					CANUpdateRTSFlag = 1;		
#endif
				// --, 150820 sys			
			}
			else if(RxMessage_temp->Data[0] == 0x11) // ControlByte CTS
			{
				CANUpdateCTSFlag = 1;
			}
			else if(RxMessage_temp->Data[0] == 0x13) // ControlByte ACK
			{
				CANUpdateACKFlag = 1;
				CANUpdateCTSFlag = 0;
			}
			break;
		case 0xEB:		// Multi Packet TP.DT
			if(((CANUpdatePF & 0xFF) << 8) + CANUpdatePS != 0)
				CAN_Parsing_CANUpdate_Multi(((CANUpdatePF & 0xFF) << 8) + CANUpdatePS , CANUpdateTotalPacketNum, CANUpdateTotalMsgSize, &RxMessage_temp->Data[0]);
			break;
		case 0xEF:		// Single Packet
			MessageType = RxMessage_temp->Data[0];
			if(MessageType == CANUPDATE_MESSAGETYPE)
			{

				Command = (RxMessage_temp->Data[2] << 8) | RxMessage_temp->Data[1];
				CANUpdateFWID = 0;		// ++, --, 200319 bwk
				switch(Command)
				{
				case 0x0020:	// Requeset F/W #N Info
					CANUpdateCommand = Command;
					CanUpdateTimeoutTimer = 0;
					CanUpdateTimeoutCount = 0;
					CANUpdateCRCCount = 0;
					// ++, 150813 sys
					Flag_CAN_Update_Start=1;
					// --, 150813 sys
					CANUpdateFWID =  (RxMessage_temp->Data[4] << 8) | RxMessage_temp->Data[3];		// ++, --, 200319 bwk
					break;
				case 0x0041:	// Enter DL Mode
					CANUpdateCommand = Command;
					CanUpdateTimeoutTimer = 0;
					CanUpdateTimeoutCount = 0;
					CANUpdateCRCCount = 0;
					// ++, 150813 sys
					Flag_CAN_Update_Start=1;
					// --, 150813 sys
					break;
				case 0x0042:	// Send New F/W #N Info		
					// Multi Packet
					break;
				case 0x0040:	// App #N DL Start
					CANUpdateCommand = Command;
					CanUpdateTimeoutTimer = 0;
					CanUpdateTimeoutCount = 0;
					CANUpdateCRCCount = 0;
					break;
				case 0x0043:	// Send Packet #M
					// Multi Packet
					break;
				case 0x0051:	// Quit DL Mode
					CANUpdateCommand = Command;
					CanUpdateTimeoutTimer = 0;
					CanUpdateTimeoutCount = 0;
					CANUpdateCRCCount = 0;
					break;
				case 0x0060:	// FW Update Start
					CANUpdateCommand = Command;
					CanUpdateTimeoutTimer = 0;
					CanUpdateTimeoutCount = 0;
					CANUpdateCRCCount = 0;
					break;
					// ++, 150819 sys
				case 0x0045:	// UPD Format
					CANUpdateCommand = Command;
					break;
				case 0x0046:	// APP DL CANCEL
					CANUpdateCommand = Command;
					CanUpdateTimeoutTimer = 0;
					CanUpdateTimeoutCount = 0;
					CANUpdateCRCCount = 0;
					Flag_CAN_Update_Start=0;
					break;
					// --, 150819 sys	
				}
			}
			break;
		}
	}
}

void CAN_Parsing_CANUpdate_Multi(unsigned short _PGN ,unsigned char _TotalPacketNum, unsigned short _TotalMsgSize, unsigned char* _Data)
{
	int Index;
	unsigned short Command;
	Index = _Data[0];
	switch(_PGN)
	{
	case 0xEF00:		// 61423 CID Write
		if(Index > 0)
		{
			CANUpdateDataIndex[Index - 1] = Index;
			memcpy((unsigned char*)&CANUpdateData[Index - 1][0],(unsigned char*)&_Data[1],7);
			//if(_TotalPacketNum == Index)
			{
				if(CheckMultiPacketIndex(_TotalPacketNum,&CANUpdateDataIndex[0]) == 1)
				{
					Command = (CANUpdateData[0][2] << 8) | CANUpdateData[0][1];

					switch(Command)
					{
					case 0x0042:		// Send New F/W # Info
						memcpy((unsigned char*)&Receive_Info[0],(unsigned char*)&CANUpdateData[0][0],_TotalMsgSize);
						CANUpdateCommand = Command;
						CanUpdateTimeoutTimer = 0;
						CanUpdateTimeoutCount = 0;
						CANUpdateCRCCount = 0;
						break;
					case 0x0043:		// Send Packet #M  
						memcpy((unsigned char*)&tmpFWDatabuf[0],(unsigned char*)&CANUpdateData[0][0],_TotalMsgSize);
						CANUpdateCommand = Command;
						CanUpdateTimeoutTimer = 0;
						CanUpdateTimeoutCount = 0;
						break;
					}
					CANUpdatePF = CANUpdatePS = CANUpdateTotalPacketNum = CANUpdateTotalMsgSize = 0;
				}
			}
		}
		break;
	}
}

int CheckMultiPacketIndex(unsigned char _TotalPacketNum, unsigned char* _IndexData)
{
	for(int i = 0; i < _TotalPacketNum; i++)
	{
		if(_IndexData[i] != i + 1)
			return 0;
	}

	return 1;
}

/**
* @brief  This function writes a data buffer in flash (data are 32-bit aligned).
* @note   After writing data buffer, the flash content is checked.
* @param  FlashAddress: start address for writing data buffer
* @param  Data: pointer on data buffer
* @param  DataLength: length of data buffer (unit is 32-bit word)   
* @retval 0: Data successfully written to Flash memory
*         1: Error occurred while writing data in Flash memory
*         2: Written Data in flash memory is different from expected one
*/

unsigned int FLASH_If_Write(volatile unsigned int* FlashAddress, unsigned int* Data ,unsigned int DataLength)
{
	unsigned int i = 0;

	for (i = 0; (i < DataLength) && (*FlashAddress <= (USER_FLASH_END_ADDRESS-4)); i++)
	{
		/* Device voltage range supposed to be [2.7V to 3.6V], the operation will
		be done by word */ 	

		if (HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, *FlashAddress, *(unsigned int*)(Data+i))== HAL_OK)
		{
			/* Check the written value */
			if (*(unsigned int*)*FlashAddress != *(unsigned int*)(Data+i))
			{
				/* Flash content doesn't match SRAM content */
				return(2);
			}
			/* Increment FLASH destination address */
			*FlashAddress += 4;
		}
		else
		{
			/* Error occurred while writing data in Flash memory */
			return (1);
		}
	}

	return (0);
}

///////////////////////////////////////////////////////////////////////
