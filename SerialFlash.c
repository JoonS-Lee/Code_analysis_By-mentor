/**
******************************************************************************
* @file    M25PXX.c 
* @author  YSM (ysm86@taeha.co.kr)
* @version V1.0.0
* @date    09/17/2010
* @brief   Program body
*
* Project Name       : ForkLift Monitor Application
* Project Enviroment : IAREmbedded Workbench for ARM 5.4x 
*                      STM32F10x Firmware Library v3.1.2
* Project Workspace  : Tunning_Device_APP
* MCU Type           : STM32F103ZE
*
* TAEHA MECHATRONICS Co., Ltd (http://www.taeha.co.kr)				
* ECS Division R&D Center. / Author by Young-Min Han (kutelf@taeha.co.kr)
* 
******************************************************************************
* @copy
*
* THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
* TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
* DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
* FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
* CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*
* <h2><center>&copy; COPYRIGHT 2010 STMicroelectronics</center></h2>
*/ 

#include "FLV_Cluster_APP.h"


const unsigned short Crc16Table[256] = {
	0x0000, 0xC0C1, 0xC181, 0x0140, 0xC301, 0x03C0, 0x0280, 0xC241,
	0xC601, 0x06C0, 0x0780, 0xC741, 0x0500, 0xC5C1, 0xC481, 0x0440,
	0xCC01, 0x0CC0, 0x0D80, 0xCD41, 0x0F00, 0xCFC1, 0xCE81, 0x0E40,
	0x0A00, 0xCAC1, 0xCB81, 0x0B40, 0xC901, 0x09C0, 0x0880, 0xC841,
	0xD801, 0x18C0, 0x1980, 0xD941, 0x1B00, 0xDBC1, 0xDA81, 0x1A40,
	0x1E00, 0xDEC1, 0xDF81, 0x1F40, 0xDD01, 0x1DC0, 0x1C80, 0xDC41,
	0x1400, 0xD4C1, 0xD581, 0x1540, 0xD701, 0x17C0, 0x1680, 0xD641,
	0xD201, 0x12C0, 0x1380, 0xD341, 0x1100, 0xD1C1, 0xD081, 0x1040,
	0xF001, 0x30C0, 0x3180, 0xF141, 0x3300, 0xF3C1, 0xF281, 0x3240,
	0x3600, 0xF6C1, 0xF781, 0x3740, 0xF501, 0x35C0, 0x3480, 0xF441,
	0x3C00, 0xFCC1, 0xFD81, 0x3D40, 0xFF01, 0x3FC0, 0x3E80, 0xFE41,
	0xFA01, 0x3AC0, 0x3B80, 0xFB41, 0x3900, 0xF9C1, 0xF881, 0x3840,
	0x2800, 0xE8C1, 0xE981, 0x2940, 0xEB01, 0x2BC0, 0x2A80, 0xEA41,
	0xEE01, 0x2EC0, 0x2F80, 0xEF41, 0x2D00, 0xEDC1, 0xEC81, 0x2C40,
	0xE401, 0x24C0, 0x2580, 0xE541, 0x2700, 0xE7C1, 0xE681, 0x2640,
	0x2200, 0xE2C1, 0xE381, 0x2340, 0xE101, 0x21C0, 0x2080, 0xE041,
	0xA001, 0x60C0, 0x6180, 0xA141, 0x6300, 0xA3C1, 0xA281, 0x6240,
	0x6600, 0xA6C1, 0xA781, 0x6740, 0xA501, 0x65C0, 0x6480, 0xA441,
	0x6C00, 0xACC1, 0xAD81, 0x6D40, 0xAF01, 0x6FC0, 0x6E80, 0xAE41,
	0xAA01, 0x6AC0, 0x6B80, 0xAB41, 0x6900, 0xA9C1, 0xA881, 0x6840,
	0x7800, 0xB8C1, 0xB981, 0x7940, 0xBB01, 0x7BC0, 0x7A80, 0xBA41,
	0xBE01, 0x7EC0, 0x7F80, 0xBF41, 0x7D00, 0xBDC1, 0xBC81, 0x7C40,
	0xB401, 0x74C0, 0x7580, 0xB541, 0x7700, 0xB7C1, 0xB681, 0x7640,
	0x7200, 0xB2C1, 0xB381, 0x7340, 0xB101, 0x71C0, 0x7080, 0xB041,
	0x5000, 0x90C1, 0x9181, 0x5140, 0x9301, 0x53C0, 0x5280, 0x9241,
	0x9601, 0x56C0, 0x5780, 0x9741, 0x5500, 0x95C1, 0x9481, 0x5440,
	0x9C01, 0x5CC0, 0x5D80, 0x9D41, 0x5F00, 0x9FC1, 0x9E81, 0x5E40,
	0x5A00, 0x9AC1, 0x9B81, 0x5B40, 0x9901, 0x59C0, 0x5880, 0x9841,
	0x8801, 0x48C0, 0x4980, 0x8941, 0x4B00, 0x8BC1, 0x8A81, 0x4A40,
	0x4E00, 0x8EC1, 0x8F81, 0x4F40, 0x8D01, 0x4DC0, 0x4C80, 0x8C41,
	0x4400, 0x84C1, 0x8581, 0x4540, 0x8701, 0x47C0, 0x4680, 0x8641,
	0x8201, 0x42C0, 0x4380, 0x8341, 0x4100, 0x81C1, 0x8081, 0x4040
};

void SPI_FLASH_SectorErase(unsigned long SectorAddr)
{
	// Send write enable instruction 
	SPI_FLASH_WriteEnable();
	// Sector Erase  
	// Select the FLASH: Chip Select low 
	SPI_FLASH_CS_LOW();
	// Send Sector Erase instruction 
	SPI_Transfer(SE);
	// Send SectorAddr high nibble address byte 
	SPI_Transfer((SectorAddr & 0xFF0000) >> 16);
	// Send SectorAddr medium nibble address byte 
	SPI_Transfer((SectorAddr & 0xFF00) >> 8);
	// Send SectorAddr low nibble address byte 
	SPI_Transfer(SectorAddr & 0xFF);
	// Deselect the FLASH: Chip Select high 
	SPI_FLASH_CS_HIGH();

	// Wait the end of Flash writing 
	SPI_FLASH_WaitForWriteEnd();
}

void SPI_FLASH_BulkErase(void)
{
	// Send write enable instruction 
	SPI_FLASH_WriteEnable();

	// Bulk Erase  
	// Select the FLASH: Chip Select low 
	SPI_FLASH_CS_LOW();
	// Send Bulk Erase instruction  
	SPI_Transfer(BE);
	// Deselect the FLASH: Chip Select high 
	SPI_FLASH_CS_HIGH();

	// Wait the end of Flash writing 
	SPI_FLASH_WaitForWriteEnd();
}

void SPI_FLASH_PageWrite(unsigned char* pBuffer, unsigned long WriteAddr, unsigned short NumByteToWrite)
{
	// Enable the write access to the FLASH 
	SPI_FLASH_WriteEnable();

	// Select the FLASH: Chip Select low 
	SPI_FLASH_CS_LOW();
	// Send "Write to Memory " instruction 
	SPI_Transfer(WRITE);
	// Send WriteAddr high nibble address byte to write to 
	SPI_Transfer((WriteAddr & 0xFF0000) >> 16);
	// Send WriteAddr medium nibble address byte to write to 
	SPI_Transfer((WriteAddr & 0xFF00) >> 8);  
	// Send WriteAddr low nibble address byte to write to 
	SPI_Transfer(WriteAddr & 0xFF);

	// while there is data to be written on the FLASH 
	while(NumByteToWrite--) 
	{
		// Send the current byte 
		SPI_Transfer(*pBuffer);
		// Point on the next byte to be written 
		pBuffer++; 
	}

	// Deselect the FLASH: Chip Select high 
	SPI_FLASH_CS_HIGH();

	// Wait the end of Flash writing 
	SPI_FLASH_WaitForWriteEnd();
}

void SPI_FLASH_BufferWrite(unsigned char* pBuffer, unsigned long WriteAddr, unsigned short  NumByteToWrite)
{
	unsigned char NumOfPage = 0, NumOfSingle = 0, Addr = 0, count = 0, temp = 0;

	Addr = WriteAddr % SPI_FLASH_PageSize;
	count = SPI_FLASH_PageSize - Addr;
	NumOfPage =  NumByteToWrite / SPI_FLASH_PageSize;
	NumOfSingle = NumByteToWrite % SPI_FLASH_PageSize;

	if(Addr == 0) // WriteAddr is SPI_FLASH_PageSize aligned  
	{
		if(NumOfPage == 0) // NumByteToWrite < SPI_FLASH_PageSize 
		{
			SPI_FLASH_PageWrite(pBuffer, WriteAddr, NumByteToWrite);
		}
		else // NumByteToWrite > SPI_FLASH_PageSize  
		{
			while(NumOfPage--)
			{
				SPI_FLASH_PageWrite(pBuffer, WriteAddr, SPI_FLASH_PageSize);
				WriteAddr +=  SPI_FLASH_PageSize;
				pBuffer += SPI_FLASH_PageSize;  
			}    

			SPI_FLASH_PageWrite(pBuffer, WriteAddr, NumOfSingle);
		}
	}

	else // WriteAddr is not SPI_FLASH_PageSize aligned  
	{
		if(NumOfPage== 0) // NumByteToWrite < SPI_FLASH_PageSize 
		{
			if(NumOfSingle > count) // (NumByteToWrite + WriteAddr) > SPI_FLASH_PageSize 
			{
				temp = NumOfSingle - count;

				SPI_FLASH_PageWrite(pBuffer, WriteAddr, count);
				WriteAddr +=  count;
				pBuffer += count; 

				SPI_FLASH_PageWrite(pBuffer, WriteAddr, temp);
			}
			else
			{
				SPI_FLASH_PageWrite(pBuffer, WriteAddr, NumByteToWrite);
			}

		}

		else // NumByteToWrite > SPI_FLASH_PageSize 
		{
			NumByteToWrite -= count;
			NumOfPage =  NumByteToWrite / SPI_FLASH_PageSize;
			NumOfSingle = NumByteToWrite % SPI_FLASH_PageSize;

			SPI_FLASH_PageWrite(pBuffer, WriteAddr, count);
			WriteAddr +=  count;
			pBuffer += count;  

			while(NumOfPage--)
			{
				SPI_FLASH_PageWrite(pBuffer, WriteAddr, SPI_FLASH_PageSize);
				WriteAddr +=  SPI_FLASH_PageSize;
				pBuffer += SPI_FLASH_PageSize;
			}

			if(NumOfSingle != 0)
			{
				SPI_FLASH_PageWrite(pBuffer, WriteAddr, NumOfSingle);
			}
		}
	}
}

void SPI_FLASH_BufferRead(unsigned char* pBuffer, unsigned long ReadAddr, unsigned short  NumByteToRead)
{
	// Select the FLASH: Chip Select low 
	SPI_FLASH_CS_LOW();

	// Send "Read from Memory " instruction 
	SPI_Transfer(READ);

	// Send ReadAddr high nibble address byte to read from 
	SPI_Transfer((ReadAddr & 0xFF0000) >> 16);
	// Send ReadAddr medium nibble address byte to read from 
	SPI_Transfer((ReadAddr& 0xFF00) >> 8);
	// Send ReadAddr low nibble address byte to read from 
	SPI_Transfer(ReadAddr & 0xFF);

	while(NumByteToRead--) // while there is data to be read 
	{
		// Read a byte from the FLASH 
		*pBuffer = SPI_Transfer(Dummy_Byte);
		// Point to the next location where the byte read will be saved 
		pBuffer++;
	}

	// Deselect the FLASH: Chip Select high 
	SPI_FLASH_CS_HIGH();
}

//unsigned long Temp = 0, Temp0 = 0, Temp1 = 0, Temp2 = 0;

unsigned long SPI_FLASH_ReadID(void)
{
	unsigned long Temp = 0, Temp0 = 0, Temp1 = 0, Temp2 = 0;

	// Select the FLASH: Chip Select low 
	SPI_FLASH_CS_LOW();
	// Send "RDID " instruction 
	SPI_Transfer(0x9F);
	// Read a byte from the FLASH 
	Temp0 = SPI_Transfer(Dummy_Byte);

	// Read a byte from the FLASH 
	Temp1 = SPI_Transfer(Dummy_Byte);

	// Read a byte from the FLASH 
	Temp2 = SPI_Transfer(Dummy_Byte);

	// Deselect the FLASH: Chip Select high 
	SPI_FLASH_CS_HIGH();
	Temp = (Temp0 << 16) | (Temp1 << 8) | Temp2;

	return Temp;
}

void SPI_FLASH_StartReadSequence(unsigned long ReadAddr)
{
	// Select the FLASH: Chip Select low 
	SPI_FLASH_CS_LOW();

	// Send "Read from Memory " instruction 
	SPI_Transfer(READ);

	// Send the 24-bit address of the address to read from -----------------------  
	// Send ReadAddr high nibble address byte 
	SPI_Transfer((ReadAddr & 0xFF0000) >> 16);
	// Send ReadAddr medium nibble address byte 
	SPI_Transfer((ReadAddr& 0xFF00) >> 8);
	// Send ReadAddr low nibble address byte 
	SPI_Transfer(ReadAddr & 0xFF);
}

unsigned char SPI_FLASH_ReadByte(void)
{
	return (SPI_Transfer(Dummy_Byte));
}

unsigned char  SPI_Transfer(unsigned char data)
{      
	unsigned char i;
	unsigned char result;

	result = 0;

	for (i = 0; i < 8; i++)
	{
		SPI_FLASH_SCK_LOW();
		(data & (1 << (7- i))) ? SPI_FLASH_MOSI_HIGH():SPI_FLASH_MOSI_LOW();
		SPI_FLASH_SCK_HIGH();

		result <<= 1;
		result |= HAL_GPIO_ReadPin(C_SPI_MISO_GPIO_Port,C_SPI_MISO_Pin);
	}

	return result;

}

void SPI_FLASH_WriteEnable(void)
{
	// Select the FLASH: Chip Select low 
	SPI_FLASH_CS_LOW();

	// Send "Write Enable" instruction 
	SPI_Transfer(WREN);

	// Deselect the FLASH: Chip Select high 
	SPI_FLASH_CS_HIGH();
}

void SPI_FLASH_WaitForWriteEnd(void)
{
	unsigned char FLASH_Status = 0;

	// Select the FLASH: Chip Select low 
	SPI_FLASH_CS_LOW();

	// Send "Read Status Register" instruction 
	SPI_Transfer(RDSR);

	// Loop as long as the memory is busy with a write cycle 

	do
	{
		// Send a dummy byte to generate the clock needed by the FLASH and put the value of the status register in FLASH_Status variable 
		FLASH_Status = SPI_Transfer(Dummy_Byte);

	} while((FLASH_Status & WIP_Flag) == SET); // Write in progress 

	// Deselect the FLASH: Chip Select high 

	SPI_FLASH_CS_HIGH();
}


unsigned short MakeCrc16(unsigned char *data, unsigned short nLen)
{
	unsigned short i = 0;
	unsigned short crc16 = 0;

	while (nLen--)
	{
		crc16 = Crc16Table[((crc16 ^ data[i]) & 0xFF)] ^ (crc16 >> 8);
		i++;
	}

	return(crc16);
}
