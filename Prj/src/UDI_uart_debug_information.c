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
UDI_Init_IO_Ports(void);

static void
UDI_Init_DMA_For_Tx(void);
/*#### |End  | <-- Секция - "Прототипы локальных функций" ####################*/


/*#### |Begin| --> Секция - "Описание глобальных функций" ####################*/
void
UDI_Init_All_UART3_RxTx_With_DMA_Tx(
unsigned int baudrate)
{
	UDI_Init_IO_Ports();
	UDI_Init_DMA_For_Tx();
	UDI_Init_UART3_RxTx(baudrate);
}

void
UDI_Init_UART3_RxTx(
unsigned int baudrate)
{
    U3MODEBITS.UARTEN = 1;
    U3STABITS.URXISEL = 0x;
    U3STABITS.UTXEN = 1; 
    U3BRG = (64000000u / (16 * baudrate)) - 1u;
    
}

void
UDI_Init_IO_Ports(void)
{
    /* UART Tx port AN28/PWM3L/PMD4/RP84/RE4 */
    /* 101 0100 I/O RP84 */
    /* U3TX 011011 RPn tied to UART3 Transmit */
    ANSELEbits.ANSE4 = 0;
    TRISEbits.TRISE4 = 0;
    RPOR5bits.RP84R = 0b1010100;
    
    /* UART Rx Port RB3/AN3/C2IN1+/VPIO/RPI35 */
    /* UART3 Receive U3RX RPINR27 U3RXR<6:0> */
    ANSELBbits.ANSB3 = 0;
    TRISBbits.TRISB3 = 1;
    RPINR27BITS.U3RXR = 0b010 011 ;
}

void
UDI_Init_DMA_For_Tx(void)
{
	DMA3CONBITS.CHEN = 1;
    DMA3CONBITS.SIZE = 1;
    DMA3CONBITS.DIR = 1;
    DMA3CONBITS.AMODE = 0b00;
    DMA3CONBITS.MODE = 0b00;
    
    DMA3REQBITS.IRQSEL = 01010011;
    
}

void
UDI_StartForceUartDMATransmit(
	unsigned int *pMemSrc, 
	unsigned int cnt)
{
	
}

/* Написать обработчик прерывания */
/*#### |End  | <-- Секция - "Описание глобальных функций" ####################*/


/*#### |Begin| --> Секция - "Описание локальных функций" #####################*/
/*#### |End  | <-- Секция - "Описание локальных функций" #####################*/


/*############################################################################*/
/*############################ END OF FILE  ##################################*/
/*############################################################################*/
