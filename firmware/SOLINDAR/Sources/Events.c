/* ###################################################################
**     Filename    : Events.c
**     Project     : SOLINDAR
**     Processor   : MC9S08QE128CLK
**     Component   : Events
**     Version     : Driver 01.02
**     Compiler    : CodeWarrior HCS08 C Compiler
**     Date/Time   : 2019-10-02, 12:46, # CodeGen: 0
**     Abstract    :
**         This is user's event module.
**         Put your event handler code here.
**     Settings    :
**     Contents    :
**         No public methods
**
** ###################################################################*/
/*!
** @file Events.c
** @version 01.02
** @brief
**         This is user's event module.
**         Put your event handler code here.
*/         
/*!
**  @addtogroup Events_module Events module documentation
**  @{
*/         
/* MODULE Events */


#include "Cpu.h"
#include "Events.h"

/* User includes (#include below this line is not maintained by Processor Expert) */
extern bool echo_flg, trgg_flg, cap_flg_ris, motor_flg;
extern unsigned int son_dis, echo_time, count;
/*	Variables explanation:
**  echo_flg   	: 	auxiliary flag. Wait for interrupt to occur.
**	trgg_flg	:	trigger flag. TRUE until the interrupt occurs, FALSE after the interrupt.
**	cap_flg_ris	:	capture flag rising. TRUE if rising edge, FLASE if falling edge.
**	motor_flg	:	motor flag. TRUE if motor is to move. FALSE otherwise.
**	son_dis		:	sonar distance. Sonar distance measured in centimetres.
**	echo_time	:	echo time. Measures the duration of the echo signal. 
*/

/*
** ===================================================================
**     Event       :  TI1_OnInterrupt (module Events)
**
**     Component   :  TI1 [TimerInt]
**     Description :
**         When a timer interrupt occurs this event is called (only
**         when the component is enabled - <Enable> and the events are
**         enabled - <EnableEvent>). This event is enabled only if a
**         <interrupt service/event> is enabled.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/
void TI1_OnInterrupt(void)
{
  /* Write your code here ... */
	
	TI1_Disable();			//Disable interrupt service.
	Trigger_ClrVal();		//Set output LOW. End of trigger.
	trgg_flg = !trgg_flg;	//Brake while loop.
}

/*
** ===================================================================
**     Event       :  TI2_OnInterrupt (module Events)
**
**     Component   :  TI2 [TimerInt]
**     Description :
**         When a timer interrupt occurs this event is called (only
**         when the component is enabled - <Enable> and the events are
**         enabled - <EnableEvent>). This event is enabled only if a
**         <interrupt service/event> is enabled.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/
void TI2_OnInterrupt(void)
{
  /* Write your code here ... */
	motor_flg = FALSE;
}

/*
** ===================================================================
**     Event       :  Echo_OnCapture (module Events)
**
**     Component   :  Echo [Capture]
**     Description :
**         This event is called on capturing of Timer/Counter actual
**         value (only when the component is enabled - <Enable> and the
**         events are enabled - <EnableEvent>.This event is available
**         only if a <interrupt service/event> is enabled.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/
void Echo_OnCapture(void)
{
  /* Write your code here ... */
	Echo_GetCaptureValue(&echo_time);	//Measure the time the signal echo was HIGH.
	count++;
	echo_flg = !echo_flg;	//Brake while loop.
	Echo_Reset();			//Reset timer.
}

/*
** ===================================================================
**     Event       :  Lidar_OnEnd (module Events)
**
**     Component   :  Lidar [ADC]
**     Description :
**         This event is called after the measurement (which consists
**         of <1 or more conversions>) is/are finished.
**         The event is available only when the <Interrupt
**         service/event> property is enabled.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/
void Lidar_OnEnd(void)
{
  /* Write your code here ... */
}


/* END Events */

/*!
** @}
*/
/*
** ###################################################################
**
**     This file was created by Processor Expert 10.3 [05.09]
**     for the Freescale HCS08 series of microcontrollers.
**
** ###################################################################
*/
