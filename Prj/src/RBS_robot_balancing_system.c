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
static void
RBS_Init_PD_ForRetentionDesiredPitchAngle(
	regul_pid_s *pPID_s);

static void
RBS_Init_KI_ForFormationDesiredPitchAngle(
	regul_pid_s *pPID_s);

static void
RBS_GetControlForBalance(
	rbs_balancing_system_s *p_s,
	__PFPT__ pitchAngle,
	__PFPT__ pitchAngularSpeed);

static __PFPT__
RBS_GetDesiredAngle(
	rbs_speed_control_s *pSpeedControl_s);
/*#### |End  | <-- Секция - "Локальные переменные" ###########################*/


/*#### |Begin| --> Секция - "Прототипы локальных функций" ####################*/
/*#### |End  | <-- Секция - "Прототипы локальных функций" ####################*/


/*#### |Begin| --> Секция - "Описание глобальных функций" ####################*/
void
RBS_Init_BalancingSystem(
	rbs_balancing_system_s *p_s)
{
	/* Инициализация ПД регулятора для удержания заданного угла наклона */
	RBS_Init_PD_ForRetentionDesiredPitchAngle(
		&p_s->pdForBalance_s);

	/* Инициализация ПИ регулятора для формирования заданного угла наклона */
	RBS_Init_KI_ForFormationDesiredPitchAngle(
		&p_s->speedControl_s.piRegulator_s);

	p_s->speedControl_s.compFilt_s.filtCoeff = 0.99;

	p_s->desiredAngle = (__PFPT__) 0.0;
}

__PFPT__
RBS_GetControlForRobot(
	rbs_balancing_system_s *p_s,
	__PFPT__ pitchAngle,
	__PFPT__ pitchAngularSpeed)
{
	/* Если угол наклона больше некого значения по модулю, то необходимо
	 * управляющее воздействие установить в нуль */
	if (fabsf((float)pitchAngle) > (float) 1.59)
	{
		p_s->motorControl = (__PFPT__) 0.0;
		p_s->motorControl_a[RBS_LEFT_MOTOR] = (__PFPT__) 0.0;
		p_s->motorControl_a[RBS_RIGHT_MOTOR] = (__PFPT__) 0.0;
	}
	/* Иначе, штатный режим работы */
	else
	{
		/* Текущая скорость задается как управляющее воздействие
		 * на электродвигатели */
		p_s->speedControl_s.currSpeed =
			p_s->motorControl;

		/* Расчет заданного угла наклона */
		p_s->desiredAngle =
			RBS_GetDesiredAngle(
				&p_s->speedControl_s);

		/* Расчет управляющего воздействия для удержания заданного угла наклона */
		RBS_GetControlForBalance(
			p_s,
			pitchAngle,
			pitchAngularSpeed);

		/* Расчет управления для вращения по азимуту */
	}

	return (p_s->motorControl);
}
/*#### |End  | <-- Секция - "Описание глобальных функций" ####################*/


/*#### |Begin| --> Секция - "Описание локальных функций" #####################*/
__PFPT__
RBS_GetDesiredAngle(
	rbs_speed_control_s *pSpeedControl_s)
{
	/* Фильтрация текущей скорости */
	pSpeedControl_s->currSpeedFilt =
		FILT_ComplFilt_f32(
			&pSpeedControl_s->compFilt_s,
			pSpeedControl_s->currSpeed);

	/* Ошибка скорости*/
	__PFPT__ error =
		-pSpeedControl_s->target + pSpeedControl_s->currSpeedFilt;

//	pSpeedControl_s->piRegulator_s.proportional_s.kP = -(((error*error) * 0.001f) + 0.01);

//	if (fabsf(pSpeedControl_s->piRegulator_s.proportional_s.kP) > 0.2)
//	{
//		pSpeedControl_s->piRegulator_s.proportional_s.kP = -0.2f;
//	}
//	pSpeedControl_s->piRegulator_s.integral_s.kI = -(((error*error) * 10.0f) + 0.1);

	/* Формирование заданного угла наклона */
	__PFPT__ desiredAngle =
		REGUL_Get_PID(
			&pSpeedControl_s->piRegulator_s,
			error,
			NULL);



	return (desiredAngle);
}


void
RBS_GetControlForBalance(
	rbs_balancing_system_s *p_s,
	__PFPT__ pitchAngle,
	__PFPT__ pitchAngularSpeed)
{
	/* Ошибка угла наклона как разница между заданным углом и текущим */
	/* FIXME поменять слагаемые местами в соответствии с текстом выше */
	__PFPT__ error =
		pitchAngle - p_s->desiredAngle;

	/* Получить управляющее воздействие для балансирования */
	p_s->motorControl =
		REGUL_Get_PID(
			&p_s->pdForBalance_s,
			error,
			NULL);

	/* Копирование найденного значения управляющего воздействия в переменные
	 * для правого и лового моторов */
	p_s->motorControl_a[RBS_LEFT_MOTOR] =
		p_s->motorControl;
	p_s->motorControl_a[RBS_RIGHT_MOTOR] =
		p_s->motorControl;
}


/**
 * @brief	Функция выполняет инициализацию структуры ПД регулятора который
 *        	приводит текущий угол наклона балансирующего робота к заданному
 * @param[in]	*pPID_s:	Указатель на структуру ПИД регулятора
 * @return 	None
 */
void
RBS_Init_PD_ForRetentionDesiredPitchAngle(
	regul_pid_s *pPID_s)
{
	regul_pid_init_struct_s pidInit_s;
	REGUL_PID_StructInit(
		&pidInit_s);

	/* Период интегрирования/дифференцирования */
	pidInit_s.dT =
		(__REGUL_FPT__) INTEGRATE_PERIOD_IN_SEC;

	/* Коэффициент пропорциональной составляющей регулятора */
	pidInit_s.kP =
		(__REGUL_FPT__) 30.0;

	/* Коэффициент интегральной составляющей регулятора */
	pidInit_s.kI =
		(__REGUL_FPT__) 0.0;

	/* Коэффициент дифференциальной составляющей регулятора */
	pidInit_s.kD =
		(__REGUL_FPT__) 0.15;

	/* Значение насыщения интегральной составляющей */
	pidInit_s.integralValSaturation =
		(__REGUL_FPT__) 0.99;

	/* Значение насыщения выходной величины регулятора */
	pidInit_s.returnValSaturation =
		(__REGUL_FPT__) 0.99;

	/* Инициализация структуры регулятора */
	REGUL_Init_PID(
		pPID_s,
		&pidInit_s);
}

/**
 * @brief	Функция выполняет инициализацию структуры ПИ регулятора который
 *        	задает необходимый угол наклона балансирующего робота
 * @param[in]	*pPID_s:	Указатель на структуру ПИД регулятора
 * @return 	None
 */
void
RBS_Init_KI_ForFormationDesiredPitchAngle(
	regul_pid_s *pPID_s)
{
	regul_pid_init_struct_s pidInit_s;
	REGUL_PID_StructInit(
		&pidInit_s);

	/* Период интегрирования/дифференцирования */
	pidInit_s.dT =
		(__REGUL_FPT__) INTEGRATE_PERIOD_IN_SEC;

	/* Коэффициент пропорциональной составляющей регулятора */
	pidInit_s.kP =
		(__REGUL_FPT__) - 0.03;

	/* Коэффициент интегральной составляющей регулятора */
	pidInit_s.kI =
		(__REGUL_FPT__) - 0.9;

	/* Значение насыщения интегральной составляющей */
	pidInit_s.integralValSaturation =
		(__REGUL_FPT__) 0.95;

	/* Значение насыщения выходной величины регулятора */
	pidInit_s.returnValSaturation =
		(__REGUL_FPT__) 0.95;

	/* Инициализация структуры регулятора */
	REGUL_Init_PID(
		pPID_s,
		&pidInit_s);
}
/*#### |End  | <-- Секция - "Описание локальных функций" #####################*/


/*############################################################################*/
/*############################ END OF FILE  ##################################*/
/*############################################################################*/
