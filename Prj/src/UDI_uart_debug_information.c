/**
 * @file   	%<%NAME%>%.%<%EXTENSION%>%
 * @author 	%<%USER%>%
 * @version
 * @date 	%<%DATE%>%, %<%TIME%>%
 * @brief
 */


/*#### |Begin| --> Секция - "Include" ########################################*/
#include "../inc/UDI_uart_debug_information.h"
/*#### |End  | <-- Секция - "Include" ########################################*/


/*#### |Begin| --> Секция - "Глобальные переменные" ##########################*/
/*#### |End  | <-- Секция - "Глобальные переменные" ##########################*/


/*#### |Begin| --> Секция - "Локальные переменные" ###########################*/
/*#### |End  | <-- Секция - "Локальные переменные" ###########################*/


/*#### |Begin| --> Секция - "Прототипы локальных функций" ####################*/
static void
UDI_Init_UART3_RxTx(
	unsigned int baudrate);

static void
UDI_Init_IO_Ports(
	void);

static void
UDI_Init_DMA3_For_Uart3Tx(
	void);
/*#### |End  | <-- Секция - "Прототипы локальных функций" ####################*/


/*#### |Begin| --> Секция - "Описание глобальных функций" ####################*/
void
UDI_Init_All_UART3_RxTx_With_DMA_Tx(
	unsigned int baudrate)
{
	UDI_Init_IO_Ports();
	UDI_Init_DMA3_For_Uart3Tx();
	UDI_Init_UART3_RxTx(baudrate);
}

void
UDI_Init_UART3_RxTx(
	unsigned int baudrate)
{
	PIC_Init_USART_3_1StopBit_8BitData_RxIntEnChar_TxIntEnChar (
		FCY,
		baudrate);
}

void
UDI_Init_IO_Ports(void)
{
//--------------------------------------------------------------------------
	// ##################### //
	// <UART 3 ports config> //
	// ##################### //
	//  "UART 3 <RX>" - "AN8/PMA6/RPI40/RB8" - "0101000"
	//  |Input Name     |   |Function Name|     |Register|      |Configuration Bits|
	//  |UART3 Receive  |   |U3RX         |     |RPINR27 |      |U3RXR 6:0         |
	//	TRISBbits.TRISB8 = 1;
	//	RPINR27bits.U3RXR = 0b0101000;

	// "UART 3 <TX>" - "AN28/PWM3L/PMD4/RP84/RE4"
	//  |Function|      |RPnR 5:0 |     |Output Name                 |
	//  |U3TX    |      |011011   |     |RPn tied to UART3 Transmit  |
	TRISEbits.TRISE4 = 0;
	RPOR5bits.RP84R = 0b011011;
	//--------------------------------------------------------------------------
}

void
UDI_Init_DMA3_For_Uart3Tx(void)
{
	CloseDMA3();

	// UART3TX – UART3 Transmitter 01010011 — 0x0254 (U3TXREG)
	unsigned int config_ =
		DMA3_MODULE_ON
		& DMA3_SIZE_BYTE
		& DMA3_TO_PERIPHERAL
		& DMA3_INTERRUPT_BLOCK
		& DMA3_NORMAL
		& DMA3_REGISTER_POST_INCREMENT
		& DMA3_ONE_SHOT;

	unsigned int irq_ = DMA3_AUTOMATIC;

	unsigned int sta_address = (0x0000);

	unsigned int stb_address = 0x0000;

	unsigned int pad_address = (unsigned int) &U3TXREG;

	unsigned int count = 1;

	OpenDMA3(config_, irq_, sta_address, stb_address, pad_address, count);

	ConfigIntDMA3(DMA3_INT_PRI_7 & DMA3_INT_ENABLE);

	//  UART3TX – UART3 Transmitter
	DMA3REQbits.IRQSEL = 0b01010011;
}

void
UDI_StartUart3_DMA3_Transmit(
	unsigned int *pMemSrc,
	unsigned int cnt)
{
	if (DMA3CONbits.CHEN == 0)
	{
		UDI_StartForceUart3_DMA3_Transmit(
			pMemSrc,
			cnt);
	}
}


void
UDI_StartForceUart3_DMA3_Transmit(
	unsigned int *pMemSrc,
	unsigned int cnt)
{
	unsigned int trash = U3TXREG;
	trash = U3TXREG;
	trash = U3TXREG;
	trash = U3TXREG;

	/* Сброс флага Overrun модуля UART */
	U3STAbits.OERR = 0;

	/* Присваивание адреса в памяти */
	DMA3STAL = pMemSrc;
	DMA3STAHbits.STA = 0x0000;

	// Выставка количества байт, которое необходимо передать;
	DMA3CNTbits.CNT = cnt - 1;

	// Старт канала DMA;
	DMA3CONbits.CHEN = 1;
	DMA3REQbits.FORCE = 1;
}

/* Написать обработчик прерывания */
/*#### |End  | <-- Секция - "Описание глобальных функций" ####################*/


/*#### |Begin| --> Секция - "Описание локальных функций" #####################*/
/*#### |End  | <-- Секция - "Описание локальных функций" #####################*/


/*############################################################################*/
/*############################ END OF FILE  ##################################*/
/*############################################################################*/
