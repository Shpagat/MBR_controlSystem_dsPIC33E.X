/**
 * @file   	%<%NAME%>%.%<%EXTENSION%>%
 * @author 	%<%USER%>%
 * @version
 * @date 	%<%DATE%>%, %<%TIME%>%
 * @brief
 */


/*#### |Begin| --> Секция - "Include" ########################################*/
#include "../inc/RBS_robot_balancing_system.h"
/*#### |End  | <-- Секция - "Include" ########################################*/


/*#### |Begin| --> Секция - "Глобальные переменные" ##########################*/
rbs_balancing_system_s RBS_balancingSystem_s;
/*#### |End  | <-- Секция - "Глобальные переменные" ##########################*/


/*#### |Begin| --> Секция - "Локальные переменные" ###########################*/
/*#### |End  | <-- Секция - "Локальные переменные" ###########################*/


/*#### |Begin| --> Секция - "Прототипы локальных функций" ####################*/
/*#### |End  | <-- Секция - "Прототипы локальных функций" ####################*/


/*#### |Begin| --> Секция - "Описание глобальных функций" ####################*/
void
RBS_Init_BalancingSystem(
	rbs_balancing_system_s *p_s)
{
	regul_pid_init_struct_s pidInit_s;
	REGUL_PID_StructInit(&pidInit_s);
	pidInit_s.dT 						= (__REGUL_FPT__) INTEGRATE_PERIOD_IN_SEC;
	pidInit_s.integralValSaturation 	= (__REGUL_FPT__) 0.5;
	pidInit_s.kP 						= (__REGUL_FPT__) 15.0;
	pidInit_s.kI 						= (__REGUL_FPT__) 0.001;
	pidInit_s.kD 						= (__REGUL_FPT__) - 0.025;
	pidInit_s.returnValSaturation 		= (__REGUL_FPT__) 0.5;
	REGUL_Init_PID(
		&p_s->pid_s,
		&pidInit_s);
}

__PFPT__
RBS_GetMotorControlForBalancingRobot(
	rbs_balancing_system_s *p_s,
	__PFPT__ pitchAngle,
	__PFPT__ pitchAngularSpeed)
{
	/* Получить управляющее воздействие для балансирования */
	p_s->motorControl_a[RBS_RIGHT_MOTOR] =
		REGUL_Get_PID(
			&p_s->pid_s,
			pitchAngle,
			pitchAngularSpeed);

	/* Если угол наклона больше некого значения по модулю, то необходимо 
	 * управляющее воздействие установить в нуль */
	if (fabsf((float)pitchAngle) > (float) 1.0)
	{
		p_s->motorControl_a[RBS_RIGHT_MOTOR] = (__PFPT__) 0.0;
	}
	
	/* Управляющее воздействие для правого и лового моторов одно и тоже */
	p_s->motorControl_a[RBS_LEFT_MOTOR] = 
		p_s->motorControl_a[RBS_RIGHT_MOTOR];
	
	return (p_s->motorControl_a[RBS_RIGHT_MOTOR]);
}
/*#### |End  | <-- Секция - "Описание глобальных функций" ####################*/


/*#### |Begin| --> Секция - "Описание локальных функций" #####################*/
/*#### |End  | <-- Секция - "Описание локальных функций" #####################*/


/*############################################################################*/
/*############################ END OF FILE  ##################################*/
/*############################################################################*/
