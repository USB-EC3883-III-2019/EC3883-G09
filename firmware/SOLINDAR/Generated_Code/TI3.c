/* ###################################################################
**     THIS COMPONENT MODULE IS GENERATED BY THE TOOL. DO NOT MODIFY IT.
**     Filename    : TI3.c
**     Project     : SOLINDAR
**     Processor   : MC9S08QE128CLK
**     Component   : TimerInt
**     Version     : Component 02.161, Driver 01.23, CPU db: 3.00.067
**     Compiler    : CodeWarrior HCS08 C Compiler
**     Date/Time   : 2019-11-02, 18:30, # CodeGen: 32
**     Abstract    :
**         This component "TimerInt" implements a periodic interrupt.
**         When the component and its events are enabled, the "OnInterrupt"
**         event is called periodically with the period that you specify.
**         TimerInt supports also changing the period in runtime.
**         The source of periodic interrupt can be timer compare or reload
**         register or timer-overflow interrupt (of free running counter).
**     Settings    :
**         Timer name                  : TPM1 (16-bit)
**         Compare name                : TPM11
**         Counter shared              : Yes
**
**         High speed mode
**             Prescaler               : divide-by-32
**             Clock                   : 786432 Hz
**           Initial period/frequency
**             Xtal ticks              : 1966
**             microseconds            : 60000
**             milliseconds            : 60
**             seconds (real)          : 0.060000101725
**             Hz                      : 17
**
**         Runtime setting             : none
**
**         Initialization:
**              Timer                  : Enabled
**              Events                 : Enabled
**
**         Timer registers
**              Counter                : TPM1CNT   [$0041]
**              Mode                   : TPM1SC    [$0040]
**              Run                    : TPM1SC    [$0040]
**              Prescaler              : TPM1SC    [$0040]
**
**         Compare registers
**              Compare                : TPM1C1V   [$0049]
**
**         Flip-flop registers
**              Mode                   : TPM1C1SC  [$0048]
**     Contents    :
**         Enable  - byte TI3_Enable(void);
**         Disable - byte TI3_Disable(void);
**
**     Copyright : 1997 - 2014 Freescale Semiconductor, Inc. 
**     All Rights Reserved.
**     
**     Redistribution and use in source and binary forms, with or without modification,
**     are permitted provided that the following conditions are met:
**     
**     o Redistributions of source code must retain the above copyright notice, this list
**       of conditions and the following disclaimer.
**     
**     o Redistributions in binary form must reproduce the above copyright notice, this
**       list of conditions and the following disclaimer in the documentation and/or
**       other materials provided with the distribution.
**     
**     o Neither the name of Freescale Semiconductor, Inc. nor the names of its
**       contributors may be used to endorse or promote products derived from this
**       software without specific prior written permission.
**     
**     THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
**     ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
**     WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
**     DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
**     ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
**     (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
**     LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
**     ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
**     (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
**     SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
**     
**     http: www.freescale.com
**     mail: support@freescale.com
** ###################################################################*/
/*!
** @file TI3.c
** @version 01.23
** @brief
**         This component "TimerInt" implements a periodic interrupt.
**         When the component and its events are enabled, the "OnInterrupt"
**         event is called periodically with the period that you specify.
**         TimerInt supports also changing the period in runtime.
**         The source of periodic interrupt can be timer compare or reload
**         register or timer-overflow interrupt (of free running counter).
*/         
/*!
**  @addtogroup TI3_module TI3 module documentation
**  @{
*/         

/* MODULE TI3. */

#include "Events.h"
#include "PE_Error.h"
#include "TI3.h"
#pragma MESSAGE DISABLE C2705          /* WARNING C2705: Possible loss of data */
#pragma MESSAGE DISABLE C5919          /* WARNING C5919: Conversion of floating to unsigned integral */
#pragma MESSAGE DISABLE C5703          /* WARNING C5703: Parameter X declared in function F but not referenced */
#pragma MESSAGE DISABLE C4002          /* Disable warning C4002 "Result not used" */

static bool EnUser;                    /* Enable device by user */
static word CmpVal;                    /* Value added to compare register in ISR */
/*** Internal macros and method prototypes ***/

/*
** ===================================================================
**     Method      :  SetCV (component TimerInt)
**
**     Description :
**         The method computes and sets compare eventually modulo value 
**         for time measuring.
**         This method is internal. It is used by Processor Expert only.
** ===================================================================
*/
#define TI3_SetCV(_Val) ( \
  ((TPM1C1V = (word)(TPM1CNT + (_Val)),((CmpVal = (_Val))))))

/*
** ===================================================================
**     Method      :  HWEnDi (component TimerInt)
**
**     Description :
**         Enables or disables the peripheral(s) associated with the 
**         component. The method is called automatically as a part of the 
**         Enable and Disable methods and several internal methods.
**         This method is internal. It is used by Processor Expert only.
** ===================================================================
*/
static void HWEnDi(void);


/*** End of internal method prototypes ***/

/*
** ===================================================================
**     Method      :  HWEnDi (component TimerInt)
**
**     Description :
**         Enables or disables the peripheral(s) associated with the 
**         component. The method is called automatically as a part of the 
**         Enable and Disable methods and several internal methods.
**         This method is internal. It is used by Processor Expert only.
** ===================================================================
*/
static void HWEnDi(void)
{
  word TmpCmpVal;                      /* Temporary variable for compare value */

  if (EnUser) {
    TmpCmpVal = (word)(TPM1CNT + CmpVal); /* Count current value for the compare register */
    TPM1C1V = TmpCmpVal;               /* Set compare register */
    while (TPM1C1V != TmpCmpVal) {}    /* Wait for register update (because of Latching mechanism) */
    /* TPM1C1SC: CH1F=0 */
    clrReg8Bits(TPM1C1SC, 0x80U);      /* Reset request flag */ 
    /* TPM1C1SC: CH1IE=1 */
    setReg8Bits(TPM1C1SC, 0x40U);      /* Enable compare interrupt */ 
  } else {
    /* TPM1C1SC: CH1IE=0 */
    clrReg8Bits(TPM1C1SC, 0x40U);      /* Disable Compare interrupt */ 
  }
}

/*
** ===================================================================
**     Method      :  TI3_Enable (component TimerInt)
**     Description :
**         This method enables the component - it starts the timer.
**         Events may be generated (<DisableEvent>/<EnableEvent>).
**     Parameters  : None
**     Returns     :
**         ---             - Error code, possible codes:
**                           ERR_OK - OK
**                           ERR_SPEED - This device does not work in
**                           the active speed mode
** ===================================================================
*/
byte TI3_Enable(void)
{
  if (!EnUser) {                       /* Is the device disabled by user? */
    EnUser = TRUE;                     /* If yes then set the flag "device enabled" */
    HWEnDi();                          /* Enable the device */
  }
  return ERR_OK;                       /* OK */
}

/*
** ===================================================================
**     Method      :  TI3_Disable (component TimerInt)
**     Description :
**         This method disables the component - it stops the timer. No
**         events will be generated.
**     Parameters  : None
**     Returns     :
**         ---             - Error code, possible codes:
**                           ERR_OK - OK
**                           ERR_SPEED - This device does not work in
**                           the active speed mode
** ===================================================================
*/
byte TI3_Disable(void)
{
  if (EnUser) {                        /* Is the device enabled by user? */
    EnUser = FALSE;                    /* If yes then set the flag "device disabled" */
    HWEnDi();                          /* Disable the device */
  }
  return ERR_OK;                       /* OK */
}

/*
** ===================================================================
**     Method      :  TI3_Init (component TimerInt)
**
**     Description :
**         Initializes the associated peripheral(s) and the component 
**         internal variables. The method is called automatically as a 
**         part of the application initialization code.
**         This method is internal. It is used by Processor Expert only.
** ===================================================================
*/
void TI3_Init(void)
{
  /* TPM1C1SC: CH1F=0,CH1IE=0,MS1B=0,MS1A=1,ELS1B=0,ELS1A=0,??=0,??=0 */
  setReg8(TPM1C1SC, 0x10U);            /* Set output compare mode and disable compare interrupt */ 
  EnUser = TRUE;                       /* Enable device */
  TI3_SetCV(0xB852U);                  /* Initialize appropriate value to the compare/modulo/reload register */
  HWEnDi();
}


/*
** ===================================================================
**     Method      :  TI3_Interrupt (component TimerInt)
**
**     Description :
**         The method services the interrupt of the selected peripheral(s)
**         and eventually invokes the component event(s).
**         This method is internal. It is used by Processor Expert only.
** ===================================================================
*/
ISR(TI3_Interrupt)
{
  /* TPM1C1SC: CH1F=0 */
  clrReg8Bits(TPM1C1SC, 0x80U);        /* Reset compare interrupt request flag */ 
  TPM1C1V += CmpVal;                   /* Set new value to the compare register */
  TI3_OnInterrupt();                   /* Invoke user event */
}



/* END TI3. */

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
