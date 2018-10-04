/**
 * @file   	%<%NAME%>%.%<%EXTENSION%>%
 * @author 	%<%USER%>%
 * @version
 * @date 	%<%DATE%>%, %<%TIME%>%
 * @brief
 */


/*#### |Begin| --> Секция - "Include" ########################################*/
#include "../inc/configuration_bits.h"
#include "../inc/main.h"
/*#### |End  | <-- Секция - "Include" ########################################*/


/*#### |Begin| --> Секция - "Глобальные переменные" ##########################*/
/*#### |End  | <-- Секция - "Глобальные переменные" ##########################*/


/*#### |Begin| --> Секция - "Локальные переменные" ###########################*/
float pitchAngle;

VTMR_tmr_s compFiltRuntime_s;
/*#### |End  | <-- Секция - "Локальные переменные" ###########################*/


/*#### |Begin| --> Секция - "Прототипы локальных функций" ####################*/
static void
SetNEDCoordinateSystem(
    float pData[]);
/*#### |End  | <-- Секция - "Прототипы локальных функций" ####################*/


/*#### |Begin| --> Секция - "Описание глобальных функций" ####################*/
int main(
    void)
{
	/*=== |Begin| --> Секция - "Конфигурирование периферии микроконтроллера" =*/
	/* Инициализация тактового генератора */
	PIC_Init_Oscillator_FRC_8MHz_FOSC_128MHz_FCY_64MIPS();

	/* Инициализация светодиодов платы */
	BLEDS_Init_AllLeds();

	/* Инициализация UART модуля для передачи отладочной информации */
	UDI_Init_All_UART3_RxTx_With_DMA_Tx(9600u);

	/* Инициализация аппаратного таймера для тактирования цикла while(1) */
	HPT_Init_TMRForProg_Tact(
	    __HARD_PROG_TACT_IN_US__);

	/* Инициализация аппаратных таймеров для подключения в ним виртуальных
	 * таймеров */
	MC32_Init_32bitsCntForVirtTimers();

	/* Инициализация всей периферии для работы с внутренним инерциальным датчиком */
	IISMPU_Init_AllPeriph();
	/*=== |End  | <-- Секция - "Конфигурирование периферии микроконтроллера" =*/

	VTMR_InitTimerStruct(
	    &compFiltRuntime_s,
	    (uint16_t*) &TMR7,
	    (uint16_t*) &TMR6);

	_GIE = 1;
	/* Loop */
	while (1)
	{
		if (HPT_status_s.newProgTactEn_flag != 0)
		{
			HPT_status_s.newProgTactEn_flag = 0;

			/* Опрос инерциального датчика */
			MPU60x0_SPI_GetAllNormData(
			    &IISMPU_SPIFnc_s,
			    &IISMPU_data_s,
			    &IISMPU_LSB_s);

			/* Приведение показаний инерциального датчика к NED системе кординат */
			SetNEDCoordinateSystem(&IISMPU_data_s.accelArr[1]);
			SetNEDCoordinateSystem(&IISMPU_data_s.gyrArr[1]);

			VTMR_StartTimer(&compFiltRuntime_s);
			pitchAngle =
			    PCF_GetPitchAngle(
			        IISMPU_data_s.accelArr[1],
			        IISMPU_data_s.accelArr[3],
			        IISMPU_data_s.gyrArr[2],
			        pitchAngle,
			        0.95f,
			        INTEGRATE_PERIOD_IN_SEC);
			VTMR_GetTimerValue(&compFiltRuntime_s);

			char testMessage[] = "Hello World";
			U3TXREG = 0xAA;
//			UDI_StartUart3_DMA3_Transmit(
//				(unsigned int) testMessage, 
//				strlen(testMessage));
			
			HPT_status_s.restProgTactTime = __HARD_PROG_TACT_IN_US__ - ReadTimer9();
		}
	}
	return (1);
}

void
SetNEDCoordinateSystem(
    float pData[])
{
	pData[0] = -pData[0];
}
/*#### |End  | <-- Секция - "Описание глобальных функций" ####################*/


/*#### |Begin| --> Секция - "Описание локальных функций" #####################*/
/*#### |End  | <-- Секция - "Описание локальных функций" #####################*/


/*############################################################################*/
/*############################ END OF FILE  ##################################*/
/*############################################################################*/
