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
DI_data_for_serial_plot_s UDI_serialPlotDataPackage_s __attribute__((space(xmemory)));
/*#### |End  | <-- Секция - "Глобальные переменные" ##########################*/


/*#### |Begin| --> Секция - "Локальные переменные" ###########################*/
/*#### |End  | <-- Секция - "Локальные переменные" ###########################*/


/*#### |Begin| --> Секция - "Прототипы локальных функций" ####################*/
static void
UDI_Init_UART3_RxTx(
	unsigned int long fcy,
	unsigned int long baudrate);

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
	unsigned int long fcy,
	unsigned int long baudrate)
{
	UDI_Init_IO_Ports();

	UDI_Init_UART3_RxTx(
		fcy,
		baudrate);

	UDI_Init_DMA3_For_Uart3Tx();

}

static void
UDI_Init_UART3_RxTx(
	unsigned int long fcy,
	unsigned int long baudrate)
{
	PIC_Init_USART_3_1StopBit_8BitData_RxIntEnChar_TxIntEnChar(
		fcy,
		baudrate);

//	U3MODEbits.STSEL = 0;
//	U3MODEbits.USIDL = 0;
//	U3MODEbits.UEN = 0b00;
//	U3MODEbits.PDSEL = 0b00;
//
//	U3STAbits.URXISEL = 0b00;
//
//	if (U3MODEbits.BRGH == 0)
//	{
//		U3BRG = (unsigned int)((fcy / (16ul * baudrate)) - 1ul);
//	}
//	else
//	{
//		U3BRG = (unsigned int)((fcy / (4ul * baudrate)) - 1ul);
//	}
//
//	IEC5bits.U3EIE = 1;
//
//	U3MODEbits.UARTEN = 1;
//	U3STAbits.UTXEN = 1;
}

static void
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
	ANSELEbits.ANSE4 = 0;
	TRISEbits.TRISE4 = 0;
	RPOR5bits.RP84R = 0b011011;
	//--------------------------------------------------------------------------
}

static void
UDI_Init_DMA3_For_Uart3Tx(
	void)
{
//	CloseDMA3();
//
//	// UART3TX – UART3 Transmitter 01010011 — 0x0254 (U3TXREG)
//	unsigned int config_ =
//		DMA3_MODULE_ON
//		& DMA3_SIZE_BYTE
//		& DMA3_TO_PERIPHERAL
//		& DMA3_INTERRUPT_BLOCK
//		& DMA3_NORMAL
//		& DMA3_REGISTER_POST_INCREMENT
//		& DMA3_ONE_SHOT;
//
//	unsigned int irq_ = DMA3_AUTOMATIC;
//
//	unsigned int sta_address = (0x0000);
//
//	unsigned int stb_address = (0x0000);
//
//	unsigned int pad_address = (volatile unsigned int) &U3TXREG;
//
//	unsigned int count = 1;
//
//	OpenDMA3(config_, irq_, sta_address, stb_address, pad_address, count);
////
//	ConfigIntDMA3(DMA3_INT_PRI_7 & DMA3_INT_ENABLE);
////
////	//  UART3TX – UART3 Transmitter
//	DMA3REQbits.IRQSEL = 0b01010011;
//	DMA3CONbits.CHEN = 0;

	DMA3CONbits.AMODE = 0; //	Configure DMA for Register Indirect mode
//								with post-increment
	DMA3CONbits.SIZE = 1;
	DMA3CONbits.MODE = 0; // Configure DMA for Continuous mode
	DMA3CONbits.DIR = 1; // RAM-to-Peripheral data transfers
	DMA3CNT = 7; // 8 DMA Requests
	DMA3REQ = 0x0053; // Select UART3 Transmitter
	DMA3PAD = (volatile unsigned int) &U3TXREG;
	DMA3STAL = 0x0000;
	DMA3STAH = 0x0000;
	DMA3STBL = 0x0000;
	DMA3STBH = 0x0000;
	//  UART3TX – UART3 Transmitter
	DMA3REQbits.IRQSEL = 0b01010011;
	IFS0bits.DMA0IF = 0; // Clear DMA Interrupt Flag
	IEC0bits.DMA0IE = 1; // Enable DMA Interrupt
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
//	unsigned int trash = U3TXREG;
//	trash = U3TXREG;
//	trash = U3TXREG;
//	trash = U3TXREG;

	/* Сброс флага Overrun модуля UART */
//	U3STAbits.OERR = 0;

	/* Присваивание адреса в памяти */
	DMA3STAL = (unsigned int)pMemSrc;
	DMA3STAH = (unsigned int)pMemSrc;

	// Выставка количества байт, которое необходимо передать;
	DMA3CNTbits.CNT = cnt - 1;

	// Старт канала DMA;
	DMA3CONbits.CHEN = 1;
	DMA3REQbits.FORCE = 1;
}

void __attribute__ ((__interrupt__, auto_psv))
_U3TXInterrupt (void)
{
	// Сброс флага прерывания;
	IFS5bits.U3TXIF = 0;
}


void __attribute__ ((__interrupt__, auto_psv))
_U3RXInterrupt (void)
{
	// Сброс флага прерывания;
	IFS5bits.U3RXIF = 0;
}

void __attribute__ ((__interrupt__, no_auto_psv))
_DMA3Interrupt (void)
{
	// Сброс флага прерывания;
	IFS2bits.DMA3IF = 0;

	/* Отключение канала DMA */
	DMA3CONbits.CHEN = 0;
}
/* Написать обработчик прерывания */
/*#### |End  | <-- Секция - "Описание глобальных функций" ####################*/


/*#### |Begin| --> Секция - "Описание локальных функций" #####################*/
/*#### |End  | <-- Секция - "Описание локальных функций" #####################*/


/*############################################################################*/
/*############################ END OF FILE  ##################################*/
/*############################################################################*/
