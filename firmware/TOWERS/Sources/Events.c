/* ###################################################################
**     Filename    : Events.c
**     Project     : TOWERS
**     Processor   : MC9S08QE128CLK
**     Component   : Events
**     Version     : Driver 01.02
**     Compiler    : CodeWarrior HCS08 C Compiler
**     Date/Time   : 2019-11-08, 11:22, # CodeGen: 0
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
extern bool motorFlag, lidarFlag;
/*
** ===================================================================
**     Event       :  IR_OnError (module Events)
**
**     Component   :  IR [AsynchroSerial]
**     Description :
**         This event is called when a channel error (not the error
**         returned by a given method) occurs. The errors can be read
**         using <GetError> method.
**         The event is available only when the <Interrupt
**         service/event> property is enabled.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/
void  IR_OnError(void)
{
  /* Write your code here ... */
}

/*
** ===================================================================
**     Event       :  IR_OnRxChar (module Events)
**
**     Component   :  IR [AsynchroSerial]
**     Description :
**         This event is called after a correct character is received.
**         The event is available only when the <Interrupt
**         service/event> property is enabled and either the <Receiver>
**         property is enabled or the <SCI output mode> property (if
**         supported) is set to Single-wire mode.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/
void  IR_OnRxChar(void)
{
  /* Write your code here ... */
}

/*
** ===================================================================
**     Event       :  IR_OnTxChar (module Events)
**
**     Component   :  IR [AsynchroSerial]
**     Description :
**         This event is called after a character is transmitted.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/
void  IR_OnTxChar(void)
{
  /* Write your code here ... */
}

/*
** ===================================================================
**     Event       :  PC_OnError (module Events)
**
**     Component   :  PC [AsynchroSerial]
**     Description :
**         This event is called when a channel error (not the error
**         returned by a given method) occurs. The errors can be read
**         using <GetError> method.
**         The event is available only when the <Interrupt
**         service/event> property is enabled.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/
void  PC_OnError(void)
{
  /* Write your code here ... */
}

/*
** ===================================================================
**     Event       :  PC_OnRxChar (module Events)
**
**     Component   :  PC [AsynchroSerial]
**     Description :
**         This event is called after a correct character is received.
**         The event is available only when the <Interrupt
**         service/event> property is enabled and either the <Receiver>
**         property is enabled or the <SCI output mode> property (if
**         supported) is set to Single-wire mode.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/
void  PC_OnRxChar(void)
{
  /* Write your code here ... */
}

/*
** ===================================================================
**     Event       :  PC_OnTxChar (module Events)
**
**     Component   :  PC [AsynchroSerial]
**     Description :
**         This event is called after a character is transmitted.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/
void  PC_OnTxChar(void)
{
  /* Write your code here ... */
}

/*
** ===================================================================
**     Event       :  PC_OnFullRxBuf (module Events)
**
**     Component   :  PC [AsynchroSerial]
**     Description :
**         This event is called when the input buffer is full;
**         i.e. after reception of the last character 
**         that was successfully placed into input buffer.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/
void  PC_OnFullRxBuf(void)
{
  /* Write your code here ... */
}

/*
** ===================================================================
**     Event       :  PC_OnFreeTxBuf (module Events)
**
**     Component   :  PC [AsynchroSerial]
**     Description :
**         This event is called after the last character in output
**         buffer is transmitted.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/
void  PC_OnFreeTxBuf(void)
{
  /* Write your code here ... */
}

/*
** ===================================================================
**     Event       :  IR_OnFullRxBuf (module Events)
**
**     Component   :  IR [AsynchroSerial]
**     Description :
**         This event is called when the input buffer is full;
**         i.e. after reception of the last character 
**         that was successfully placed into input buffer.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/
void  IR_OnFullRxBuf(void)
{
  /* Write your code here ... */
}

/*
** ===================================================================
**     Event       :  IR_OnFreeTxBuf (module Events)
**
**     Component   :  IR [AsynchroSerial]
**     Description :
**         This event is called after the last character in output
**         buffer is transmitted.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/
void  IR_OnFreeTxBuf(void)
{
  /* Write your code here ... */
}

/*
** ===================================================================
**     Event       :  MotorInt_OnInterrupt (module Events)
**
**     Component   :  MotorInt [TimerInt]
**     Description :
**         When a timer interrupt occurs this event is called (only
**         when the component is enabled - <Enable> and the events are
**         enabled - <EnableEvent>). This event is enabled only if a
**         <interrupt service/event> is enabled.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/
void MotorInt_OnInterrupt(void)
{
  /* Write your code here ... */
	motorFlag = FALSE;
}

/*
** ===================================================================
**     Event       :  LIDARInt_OnInterrupt (module Events)
**
**     Component   :  LIDARInt [TimerInt]
**     Description :
**         When a timer interrupt occurs this event is called (only
**         when the component is enabled - <Enable> and the events are
**         enabled - <EnableEvent>). This event is enabled only if a
**         <interrupt service/event> is enabled.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/
void LIDARInt_OnInterrupt(void)
{
  /* Write your code here ... */
	lidarFlag = FALSE;
}

/*
** ===================================================================
**     Event       :  LIDAR_OnEnd (module Events)
**
**     Component   :  LIDAR [ADC]
**     Description :
**         This event is called after the measurement (which consists
**         of <1 or more conversions>) is/are finished.
**         The event is available only when the <Interrupt
**         service/event> property is enabled.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/
void LIDAR_OnEnd(void)
{
  /* Write your code here ... */
}


/*
** ===================================================================
**     Event       :  MotorInt2_OnInterrupt (module Events)
**
**     Component   :  MotorInt2 [TimerInt]
**     Description :
**         When a timer interrupt occurs this event is called (only
**         when the component is enabled - <Enable> and the events are
**         enabled - <EnableEvent>). This event is enabled only if a
**         <interrupt service/event> is enabled.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/
void MotorInt2_OnInterrupt(void)
{
  /* Write your code here ... */
	motorFlag = FALSE;

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
