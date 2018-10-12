/**
 * @file   	%<%NAME%>%.%<%EXTENSION%>%
 * @author 	%<%USER%>%
 * @version
 * @date 	%<%DATE%>%, %<%TIME%>%
 * @brief
 */


/*#### |Begin| --> Секция - "Include" ########################################*/
#include "../inc/LRMC_left_right_motor_control.h"
/*#### |End  | <-- Секция - "Include" ########################################*/


/*#### |Begin| --> Секция - "Глобальные переменные" ##########################*/
/*#### |End  | <-- Секция - "Глобальные переменные" ##########################*/


/*#### |Begin| --> Секция - "Локальные переменные" ###########################*/
/*#### |End  | <-- Секция - "Локальные переменные" ###########################*/


/*#### |Begin| --> Секция - "Прототипы локальных функций" ####################*/
static void
LRMC_Init_IOPorts(
	void);

static void
LRMC_Init_UART2_TransmitReceive(
	unsigned long fcy,
	unsigned long baudrate);

static void
LRMC_Init_DMA2_ForTramsmit(
	void);
/*#### |End  | <-- Секция - "Прототипы локальных функций" ####################*/


/*#### |Begin| --> Секция - "Описание глобальных функций" ####################*/
void
LRMC_Init_UART_DMA_IOPins(
	unsigned long fcy,
	unsigned long baudrate)
{
	/* Инициализация портов ввода/вывода */
	LRMC_Init_IOPorts();

	/* Инициализация модуля USART */
	LRMC_Init_UART2_TransmitReceive(
		fcy,
		baudrate);

	/* Инициализация модуля DMA */
	LRMC_Init_DMA2_ForTramsmit();
}

void
LRMC_Init_IOPorts(
	void)
{
	//  "UART 2 <RX>" - "RC13/PGED2/SOSCI/C3IN3-/RPI61" - "0111101"
	//  |Input Name     |   |Function Name|     |Register|      |Configuration Bits|
	//  |UART2 Receive  |   |U2RX         |     |RPINR19 |      |U2RXR 6:0         |
	ANSELCbits.ANSC13 = 0;
	TRISCbits.TRISC13 = 1;
	RPINR19bits.U2RXR = 0b0111101;

	//  "UART 2 <TX>" - "INT0/DMH/RP64/RD0"
	//  |Function|      |RPnR 5:0 |     |Output Name                 |
	//  |U2TX    |      |000011   |     |RPn tied to UART2 Transmit  |
	TRISDbits.TRISD0 = 0;
	RPOR0bits.RP64R = 0b000011;
}

void
LRMC_Init_UART2_TransmitReceive(
	unsigned long fcy,
	unsigned long baudrate)
{
	unsigned int config1 =
		UART_EN
		& UART_IDLE_CON
		& UART_IrDA_DISABLE
		& UART_MODE_SIMPLEX
		& UART_UEN_00
		& UART_DIS_WAKE
		& UART_DIS_LOOPBACK
		& UART_DIS_ABAUD
		& UART_UXRX_IDLE_ONE
		& UART_BRGH_SIXTEEN
		& UART_NO_PAR_8BIT
		& UART_1STOPBIT,

		config2 =
			UART_INT_TX
			& UART_IrDA_POL_INV_ZERO
			& UART_SYNC_BREAK_DISABLED
			& UART_TX_ENABLE
			& UART_TX_BUF_NOT_FUL
			& UART_INT_RX_CHAR
			& UART_ADR_DETECT_DIS
			& UART_RX_OVERRUN_CLEAR,

			U_BRG =
				((fcy / baudrate) / 16) - 1;

	OpenUART2(
		config1,
		config2,
		U_BRG);

	ConfigIntUART2(
		UART_RX_INT_EN
		& UART_RX_INT_PR7
		& UART_TX_INT_EN
		& UART_TX_INT_PR7);
}

void
LRMC_Init_DMA2_ForTramsmit(
	void)
{

}

void __attribute__ ((__interrupt__, auto_psv))
_U2TXInterrupt (void)
{
	/* Сброс флага прерывания */
	IFS1bits.U2TXIF = 0;
}


void __attribute__ ((__interrupt__, auto_psv))
_U2RXInterrupt (void)
{
	/* Сброс флага прерывания */
	IFS1bits.U2RXIF = 0;
}
/*#### |End  | <-- Секция - "Описание глобальных функций" ####################*/


/*#### |Begin| --> Секция - "Описание локальных функций" #####################*/
/*#### |End  | <-- Секция - "Описание локальных функций" #####################*/


/*############################################################################*/
/*############################ END OF FILE  ##################################*/
/*############################################################################*/
