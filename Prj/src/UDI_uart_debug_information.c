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
DI_data_for_serial_plot_s UDI_serialPlotDataPackage_s __attribute__((eds, space(dma)));
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
UDI_GetAndSendDebugPackForSerialPlot(
	DI_data_for_serial_plot_s *p_s)
{
	/* Запуск канала DMA для передачи отладочного пакета данных */
	UDI_StartUart3_DMA3_Transmit
	(
		(unsigned int*) p_s,
		(unsigned int) DI_CopyDataForSerialPlot_f32(
			/* Указатель на структуру данных отладочного пакета данных */
			p_s,

			/* Показания акселерометра */
			acc_a[IISMPU_ROLL],
			acc_a[IISMPU_PITCH],
			acc_a[IISMPU_YAW],

			/* Показания гироскопа */
			gyr_a[IISMPU_ROLL],
			gyr_a[IISMPU_PITCH],
			gyr_a[IISMPU_YAW],

			/* Расчёт углов наклона */
            pitchAngle,
            pitchByAcc,
            deltaPitch,
            pitchByGyr,
            err,

			/* Терминальный символ, должен быть крайним параметром для
			 * функции DI_CopyDataForSerialPlot_f32() */
			(float) DI_TERMINAL_SYMBOL)
	);
}

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

	DisableIntU3TX;
	DisableIntU3RX;
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
	DMA3CONbits.AMODE = 0; //	Configure DMA for Register Indirect mode
//								with post-increment
	DMA3CONbits.SIZE = 1;
	DMA3CONbits.MODE = 0; // Configure DMA for Continuous mode
	DMA3CONbits.DIR = 1; // RAM-to-Peripheral data transfers
	DMA3CNT = 0;
	DMA3REQ = 0x0053; // Select UART3 Transmitter
	DMA3PAD = (volatile unsigned int) &U3TXREG;
	DMA3STAL = 0x0000;
	DMA3STAH = 0x0000;
	DMA3STBL = 0x0000;
	DMA3STBH = 0x0000;

	/*  UART3TX – UART3 Transmitter */
	DMA3REQbits.IRQSEL = 0b01010011;

	IFS2bits.DMA3IF = 0; // Clear DMA Interrupt Flag
	IEC2bits.DMA3IE = 1; // Enable DMA Interrupt
}

void
UDI_StartUart3_DMA3_Transmit(
	unsigned int *pMemSrc,
	unsigned int cnt)
{
	if (DMA3CONbits.CHEN == 0)
	{
		/* Выключение прерывания */
		IEC2bits.DMA3IE = 0;

		/* Непрерывный режим работы DMA */
		DMA3CONbits.MODE = 0;

		unsigned int trash = U3TXREG;
		trash = U3TXREG;
		trash = U3TXREG;
		trash = U3TXREG;

		/* Сброс флага Overrun модуля UART */
		U3STAbits.OERR = 0;

		/* Присваивание адреса в памяти */
		DMA3STAL = (unsigned int)pMemSrc;
//	DMA3STAH = (unsigned int)pMemSrc;

		// Выставка количества байт, которое необходимо передать;
		DMA3CNTbits.CNT = cnt - 1;

		// Старт канала DMA;
		DMA3CONbits.CHEN = 1;
		DMA3REQbits.FORCE = 1;
	}
}

void
UDI_StartForceUart3_DMA3_Transmit(
	unsigned int *pMemSrc,
	unsigned int cnt)
{
	/* Отключение канала DMA */
	DMA3CONbits.CHEN = 0;

	/* Включение прерывания */
	IEC2bits.DMA3IE = 1;

	unsigned int trash = U3TXREG;
	trash = U3TXREG;
	trash = U3TXREG;
	trash = U3TXREG;

	/* Сброс флага Overrun модуля UART */
	U3STAbits.OERR = 0;

	/* Присваивание адреса в памяти */
	DMA3STAL = (unsigned int)pMemSrc;
//	DMA3STAH = (unsigned int)pMemSrc;

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

void __attribute__ ((__interrupt__, auto_psv))
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
