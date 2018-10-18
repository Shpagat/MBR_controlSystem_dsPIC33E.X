/**
 * @file   	%<%NAME%>%.%<%EXTENSION%>%
 * @author 	%<%USER%>%
 * @version
 * @date 	%<%DATE%>%, %<%TIME%>%
 * @brief
 */


#ifndef RBS_ROBOT_BALANCING_SYSTEM_H_
#define RBS_ROBOT_BALANCING_SYSTEM_H_


/*#### |Begin| --> Секция - "Include" ########################################*/
/*==== |Begin| --> Секция - "C libraries" ====================================*/
#include <math.h>
/*==== |End  | <-- Секция - "C libraries" ====================================*/

/*==== |Begin| --> Секция - "MK peripheral libraries" ========================*/
/*==== |End  | <-- Секция - "MK peripheral libraries" ========================*/

/*==== |Begin| --> Секция - "Extern libraries" ===============================*/
#include "../inc/RPA_robot_pitch_angle.h"
#include "../../Lib_A_REGUL_regulators/Lib_A_REGUL_regulators.h"
#include "main.h"
/*==== |End  | <-- Секция - "Extern libraries" ===============================*/
/*#### |End  | <-- Секция - "Include" ########################################*/


/*#### |Begin| --> Секция - "Определение констант" ###########################*/
/*#### |End  | <-- Секция - "Определение констант" ###########################*/


/*#### |Begin| --> Секция - "Определение типов" ##############################*/
typedef struct
{
	regul_pid_s pid_s;
	__PFPT__ motorControl;
} rbs_balancing_system_s;
/*#### |End  | <-- Секция - "Определение типов" ##############################*/


/*#### |Begin| --> Секция - "Определение глобальных переменных" ##############*/
extern rbs_balancing_system_s RBS_balancingSystem_s;
/*#### |End  | <-- Секция - "Определение глобальных переменных" ##############*/


/*#### |Begin| --> Секция - "Прототипы глобальных функций" ###################*/
extern void
RBS_Init_BalancingSystem(
	rbs_balancing_system_s *p_s);

extern __PFPT__
RBS_GetMotorControlForBalancingRobot(
	rbs_balancing_system_s *p_s,
	__PFPT__ pitchAngle,
	__PFPT__ pitchAngularSpeed);
/*#### |End  | <-- Секция - "Прототипы глобальных функций" ###################*/


/*#### |Begin| --> Секция - "Определение макросов" ###########################*/
/*#### |End  | <-- Секция - "Определение макросов" ###########################*/

#endif	/* RBS_ROBOT_BALANCING_SYSTEM_H_ */

/*############################################################################*/
/*################################ END OF FILE ###############################*/
/*############################################################################*/
