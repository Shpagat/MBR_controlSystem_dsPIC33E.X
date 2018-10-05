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
float acc_a[IISMPU_VECT_SIZE];
float gyr_a[IISMPU_VECT_SIZE];
float mpuTemperature;
/*#### |End  | <-- Секция - "Глобальные переменные" ##########################*/


/*#### |Begin| --> Секция - "Локальные переменные" ###########################*/
float pitchAngle;

VTMR_tmr_s compFiltRuntime_s;
/*#### |End  | <-- Секция - "Локальные переменные" ###########################*/


/*#### |Begin| --> Секция - "Прототипы локальных функций" ####################*/
static void
InitAllPeriphAndModules(
	void);
/*#### |End  | <-- Секция - "Прототипы локальных функций" ####################*/


/*#### |Begin| --> Секция - "Описание глобальных функций" ####################*/
int main(
	void)
{
	/* Инициализация всей периверии и программных модулей находится в
	 * функции InitAllPeriphAndModules() */
	InitAllPeriphAndModules();

	/* Loop */
	while (1)
	{
		if (HPT_status_s.newProgTactEn_flag != 0)
		{
			/* Сброс флага */
			HPT_status_s.newProgTactEn_flag = 0;

			/* Опрос инерциального датчика и копирование его показаний в 
			 * массивы */
			IISMPU_GetAccGyrTemperature(
				&acc_a[0],
				&gyr_a[0],
				&mpuTemperature);

			/* Определение угла наклона балансирующего робота по тангажу */
			VTMR_StartTimer(
				&compFiltRuntime_s);
			pitchAngle =
				PCF_GetPitchAngle(
					IISMPU_data_s.accelArr[MPU60x0_X],
					IISMPU_data_s.accelArr[MPU60x0_Z],
					IISMPU_data_s.gyrArr[MPU60x0_Y],
					pitchAngle,
					0.95f,
					INTEGRATE_PERIOD_IN_SEC);
			VTMR_GetTimerValue(
				&compFiltRuntime_s);

			/* ################ Отладочная информация ####################### */
			/* Формирование отладочного пакета данных */
			UDI_GetAndSendDebugPackForSerialPlot(
				&UDI_serialPlotDataPackage_s);
			/* ############################################################## */

			/* Нахождение оставшегося времени программного такта */
			HPT_status_s.restProgTactTime =
				__HARD_PROG_TACT_IN_US__ - TMR9;
			/* Здесь не должно быть НИЧЕГО!!! */
		}
	}
	return (1);
}

void
InitAllPeriphAndModules(
	void)
{
	/* Запрет глобальных прерываний */
	_GIE = 0;

	/* Отключение вложенных прерываний */
	_NSTDIS = 1;

	/*=== |Begin| --> Секция - "Конфигурирование периферии микроконтроллера" =*/
	/* Инициализация тактового генератора */
#if defined (__USE_FRC_FOR_FCY__)
	PIC_Init_Oscillator_FRC_8MHz_FOSC_128MHz_FCY_64MIPS();
#elif defined (__USE_HS_16_MHz_FOR_FCY__)
	PIC_Init_Oscillator_HS_16MHz_FOSC_128MHz_FCY_64MIPS();
#else
#error "Please, set source for system clock"
#endif

	/* Инициализация светодиодов платы */
	BLEDS_Init_AllLeds();

	/* Инициализация UART модуля для передачи отладочной информации */
	UDI_Init_All_UART3_RxTx_With_DMA_Tx(
		(unsigned int long) FCY,
		(unsigned int long) 115200UL);

	/* Инициализация аппаратного таймера для тактирования цикла while(1) */
	HPT_Init_TMRForProg_Tact(
		__HARD_PROG_TACT_IN_US__);

	/* Инициализация аппаратных таймеров для подключения в ним виртуальных
	 * таймеров */
	MC32_Init_32bitsCntForVirtTimers();

	/* Инициализация всей периферии для работы с внутренним инерциальным датчиком */
	IISMPU_Init_AllPeriphForInternalMPU6000();
	/*=== |End  | <-- Секция - "Конфигурирование периферии микроконтроллера" =*/

	VTMR_InitTimerStruct(
		&compFiltRuntime_s,
		(uint16_t*) &TMR7,
		(uint16_t*) &TMR6);

	/* Разрешение глобальных прерываний */
	_GIE = 1;
}
/*#### |End  | <-- Секция - "Описание глобальных функций" ####################*/


/*#### |Begin| --> Секция - "Описание локальных функций" #####################*/
/*#### |End  | <-- Секция - "Описание локальных функций" #####################*/


/*############################################################################*/
/*############################ END OF FILE  ##################################*/
/*############################################################################*/
