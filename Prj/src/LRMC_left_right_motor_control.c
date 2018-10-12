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

}

void
LRMC_Init_UART2_TransmitReceive(
	unsigned long fcy,
	unsigned long baudrate)
{

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
