#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#include "xparameters.h"
#include "xil_cache.h"
#include "xspi.h"
#include "sleep.h"
#include "xuartlite.h"

#define SPI_DEVICE_ID   XPAR_AXI_QUAD_SPI_0_DEVICE_ID
#define UART_DEVICE_ID   XPAR_AXI_UARTLITE_0_DEVICE_ID
#define DATA_LEN 62
#define CHAR_LEN 1
#define SPACE_LEN 1
#define HEX_LEN 2
#define CMD_LEN (CHAR_LEN + SPACE_LEN + HEX_LEN + SPACE_LEN + HEX_LEN)

static XSpi Spi;
static XUartLite Uart;

const uint8_t data_empty[] = {
	    0x00, 0x00,
	    0x01, 0x00,
	    0x03, 0x00,
	    0x25, 0x00,
	    0x26, 0x00,
	    0x28, 0x00,
	    0x29, 0x00,
	    0x2B, 0x00,
	    0x2D, 0x00,
	    0x2E, 0x00,
	    0x3D, 0x00,
	    0x3F, 0x00,
	    0x40, 0x00,
	    0x41, 0x00,
	    0x42, 0x00,
	    0x44, 0x00,
	    0x45, 0x00,
	    0x4A, 0x00,
	    0x58, 0x00,
	    0xBF, 0x00,
	    0xC1, 0x00,
	    0xCF, 0x00,
	    0xD2, 0x00,
	    0xD5, 0x00,
	    0xD7, 0x00,
	    0xD8, 0x00,
	    0xDB, 0x00,
	    0xEA, 0x00,
	    0xEF, 0x00,
	    0xF1, 0x00,
	    0xF2, 0x00
};

uint8_t data_to_send[DATA_LEN];

//uint8_t data_AB_test_ramp[] = {
// 				  // all values are zeros apart from:
//    0x25, 0x04, // CH A 0b100 = Outputs digital ramp.
//    0x2B, 0x04, // CH B 0b100 = Outputs digital ramp.
//    0x42, 0x08, // 0b1000 = All digital functions (such as test patterns, gain, and offset correction) enabled
//    0xF1, 0x03, // 0b11 = LVDS swing control using the LVDS SWING register bits is enabled
//};

typedef enum {
	A,
	B
} channel;

typedef enum {
	NORMAL,
	ALL0s,
	ALL1s,
	TOGGLE,
	RAMP,
	CUSTOM
} test_pattern;

int spiInitialize();
int uartInitialize();

void resetDataToSend(uint8_t *data, unsigned int len);
int setDataCmdValue(uint8_t *data, unsigned int len, uint8_t cmd, uint8_t val);
uint8_t getDataCmdValue(uint8_t *data, unsigned int len, uint8_t cmd);
void listData(uint8_t *data, unsigned int len);
int transferData(uint8_t *data, unsigned int len);

int cmdEnLvdsSwing(uint8_t *data, unsigned int len, uint8_t en);
int cmdEnDigital(uint8_t *data, unsigned int len, uint8_t en);
int cmdTestPattern(uint8_t *data, unsigned int len, channel ch, test_pattern tp);

int transferABrampTestData(uint8_t chA_en, uint8_t chB_en);

int main() {
	spiInitialize();
	uartInitialize();
	unsigned int bytesNo;
	uint8_t buff[CMD_LEN];
	uint8_t cmd_buff[HEX_LEN], val_buff[HEX_LEN];
	uint8_t cmd, val;
	uint8_t rampA = 0;
	uint8_t rampB = 0;
	xil_printf("Microblaze CLI for SPI, type H for help\n\r");
	resetDataToSend(data_to_send, DATA_LEN);

	while(1) {
		bytesNo = XUartLite_Recv(&Uart, (u8*)buff, 1);
		if(bytesNo > 0) {
			bytesNo = 0;
			usleep(10000);
			switch(buff[0]) {
				case 'R':
					xil_printf("Reset cmd\n\r");
					resetDataToSend(data_to_send, DATA_LEN);
					break;
				case 'T':
					xil_printf("Transfer cmd\n\r");
					transferData(data_to_send, DATA_LEN);
					break;
				case 'L':
					xil_printf("List cmd\n\r");
					listData(data_to_send, DATA_LEN);
					break;
				case 'S':
					xil_printf("Set cmd\n\r");
					bytesNo = XUartLite_Recv(&Uart, (u8*)buff, CMD_LEN);
					cmd_buff[0] = buff[1];
					cmd_buff[1] = buff[2];
					val_buff[0] = buff[4];
					val_buff[1] = buff[5];
					cmd = strtoul(cmd_buff, NULL, 16);
					val = strtoul(val_buff, NULL, 16);
					setDataCmdValue(data_to_send, DATA_LEN, cmd, val);
					break;
				case 'A':
					xil_printf("Ramp for ch A en\n\r");
					rampA = 1;
					transferABrampTestData(rampA, rampB);
					break;
				case 'a':
					xil_printf("Ramp for ch A disable\n\r");
					rampA = 0;
					transferABrampTestData(rampA, rampB);
					break;
				case 'B':
					xil_printf("Ramp for ch B en\n\r");
					rampB = 1;
					transferABrampTestData(rampA, rampB);
					break;
				case 'b':
					xil_printf("Ramp for ch B disable\n\r");
					rampB = 0;
					transferABrampTestData(rampA, rampB);
					break;
				case 'H':
					xil_printf("Help:\n\r");
					xil_printf("R: Reset data buff\n\r");
					xil_printf("T: Transfer data buff\n\r");
					xil_printf("L: List data buff\n\r");
					xil_printf("S <cmd> <val>: Set cmd, for example S 2B 04\n\r");
					xil_printf("A: ramp pattern en for ch A\n\r");
					xil_printf("a: ramp pattern disable for ch A\n\r");
					xil_printf("B: ramp pattern en for ch B\n\r");
					xil_printf("b: ramp pattern diable for ch B\n\r");
					break;
			}
		}

	}

    return 0;
}

int spiInitialize() {
	int Status;
	XSpi_Config *ConfigPtr;
	ConfigPtr = XSpi_LookupConfig(SPI_DEVICE_ID);

	if (ConfigPtr == NULL) {
		return XST_DEVICE_NOT_FOUND;
	}

	Status = XSpi_CfgInitialize(&Spi, ConfigPtr,
				    ConfigPtr->BaseAddress);
	if (Status != XST_SUCCESS) {
		return XST_FAILURE;
	}

	Status = XSpi_SetOptions(&Spi, XSP_MASTER_OPTION |
				 XSP_MANUAL_SSELECT_OPTION);
	if (Status != XST_SUCCESS) {
		return XST_FAILURE;
	}

	Status = XSpi_SetSlaveSelect(&Spi, 0x01);
	if (Status != XST_SUCCESS) {
		return XST_FAILURE;
	}

	XSpi_Start(&Spi);
	XSpi_IntrGlobalDisable(&Spi); // if not called, only 1st transfer is correct

	return XST_SUCCESS;
}

int uartInitialize() {
	return XUartLite_Initialize(&Uart, UART_DEVICE_ID);
}

void resetDataToSend(uint8_t *data, unsigned int len) {
	for(int i = 0; i < len; i++) {
		data[i] = data_empty[i];
	}
}

int setDataCmdValue(uint8_t *data, unsigned int len, uint8_t cmd, uint8_t val) {
	for(int i = 0; i < len-1; i=i+2) {
		if(data[i] == cmd) {
			data[i + 1] = val;
			xil_printf("cmd 0x%02x val 0x%02x\n\r", cmd, val);
			return XST_SUCCESS;
		}
	}
	xil_printf("No cmd 0x%02x\n\r", cmd);
	return XST_FAILURE;
}

uint8_t getDataCmdValue(uint8_t *data, unsigned int len, uint8_t cmd) {
	uint8_t val = 0;
	for(int i = 0; i < len-1; i=i+2) {
		if(data[i] == cmd) {
			val = data[i + 1];
			return val;
		}
	}
	return val;
}

void listData(uint8_t *data, unsigned int len) {
	xil_printf("List registers\n\r");
	for(int i = 0; i < len-1; i=i+2) {
			xil_printf("cmd 0x%02x val 0x%02x\n\r", data[i], data[i+1]);
	}
}

int transferData(uint8_t *data, unsigned int len) {
	int Status;
	Status = XSpi_Transfer(&Spi, data, NULL, len);
	if(Status != XST_SUCCESS) {
		xil_printf("SPI transfer failed\n\r");
		return XST_FAILURE;
	} else {
		listData(data, len);
		return XST_SUCCESS;
	}
}

int cmdEnLvdsSwing(uint8_t *data, unsigned int len, uint8_t en) {
	return setDataCmdValue(data, len, 0xF1, en ? 0x03 : 0x00);
}

int cmdEnDigital(uint8_t *data, unsigned int len, uint8_t en) {
	uint8_t val;
	val = getDataCmdValue(data, len, 0x42);
	if(en) {
		val |= 0x08;
	} else {
		val &= ~(0x08);
	}

	return setDataCmdValue(data, len, 0x42, val);
}

int cmdTestPattern(uint8_t *data, unsigned int len, channel ch, test_pattern tp) {
	uint8_t cmd, val;

	if(ch == (channel)A) {
		cmd = 0x25;
	} else {
		cmd = 0x2B;
	}

	val = getDataCmdValue(data, len, cmd);
	val &= 0b11111000;
	switch(tp) {
		case (test_pattern)NORMAL:
				val |= 0b000;
				break;
		case (test_pattern)ALL0s:
				val |= 0b001;
				break;
		case (test_pattern)ALL1s:
				val |= 0b010;
				break;
		case (test_pattern)TOGGLE:
				val |= 0b011;
				break;
		case (test_pattern)RAMP:
				val |= 0b100;
				break;
		case (test_pattern)CUSTOM:
				val |= 0b101;
				break;
	}

	return setDataCmdValue(data, len, cmd, val);
}

int transferABrampTestData(uint8_t chA_en, uint8_t chB_en) {
	resetDataToSend(data_to_send, DATA_LEN);
	cmdEnLvdsSwing(data_to_send, DATA_LEN, 1);
	cmdEnDigital(data_to_send, DATA_LEN, 1);
	if(chA_en) {
		cmdTestPattern(data_to_send, DATA_LEN, (channel)A, (test_pattern)RAMP);
	} else {
		cmdTestPattern(data_to_send, DATA_LEN, (channel)A, (test_pattern)NORMAL);
	}
	if(chB_en) {
		cmdTestPattern(data_to_send, DATA_LEN, (channel)B, (test_pattern)RAMP);
	} else {
		cmdTestPattern(data_to_send, DATA_LEN, (channel)B, (test_pattern)NORMAL);
	}

	return transferData(data_to_send, DATA_LEN);
}
