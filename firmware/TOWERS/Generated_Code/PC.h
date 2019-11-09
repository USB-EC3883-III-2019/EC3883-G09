/* ###################################################################
**     THIS COMPONENT MODULE IS GENERATED BY THE TOOL. DO NOT MODIFY IT.
**     Filename    : PC.h
**     Project     : TOWERS
**     Processor   : MC9S08QE128CLK
**     Component   : AsynchroSerial
**     Version     : Component 02.611, Driver 01.33, CPU db: 3.00.067
**     Compiler    : CodeWarrior HCS08 C Compiler
**     Date/Time   : 2019-11-08, 13:54, # CodeGen: 4
**     Abstract    :
**         This component "AsynchroSerial" implements an asynchronous serial
**         communication. The component supports different settings of
**         parity, word width, stop-bit and communication speed,
**         user can select interrupt or polling handler.
**         Communication speed can be changed also in runtime.
**         The component requires one on-chip asynchronous serial channel.
**     Settings    :
**         Serial channel              : SCI1
**
**         Protocol
**             Init baud rate          : 9600baud
**             Width                   : 8 bits
**             Stop bits               : 1
**             Parity                  : none
**             Breaks                  : Disabled
**             Input buffer size       : 4
**             Output buffer size      : 4
**
**         Registers
**             Input buffer            : SCI1D     [$0027]
**             Output buffer           : SCI1D     [$0027]
**             Control register        : SCI1C1    [$0022]
**             Mode register           : SCI1C2    [$0023]
**             Baud setting reg.       : SCI1BD    [$0020]
**             Special register        : SCI1S1    [$0024]
**
**         Input interrupt
**             Vector name             : Vsci1rx
**             Priority                : 
**
**         Output interrupt
**             Vector name             : Vsci1tx
**             Priority                : 
**
**         Used pins:
**         ----------------------------------------------------------
**           Function | On package           |    Name
**         ----------------------------------------------------------
**            Input   |     42               |  PTB0_KBI1P4_RxD1_ADP4
**            Output  |     41               |  PTB1_KBI1P5_TxD1_ADP5
**         ----------------------------------------------------------
**
**
**
**     Contents    :
**         RecvChar        - byte PC_RecvChar(PC_TComData *Chr);
**         SendChar        - byte PC_SendChar(PC_TComData Chr);
**         RecvBlock       - byte PC_RecvBlock(PC_TComData *Ptr, word Size, word *Rcv);
**         SendBlock       - byte PC_SendBlock(PC_TComData *Ptr, word Size, word *Snd);
**         ClearRxBuf      - byte PC_ClearRxBuf(void);
**         ClearTxBuf      - byte PC_ClearTxBuf(void);
**         GetCharsInRxBuf - word PC_GetCharsInRxBuf(void);
**         GetCharsInTxBuf - word PC_GetCharsInTxBuf(void);
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
** @file PC.h
** @version 01.33
** @brief
**         This component "AsynchroSerial" implements an asynchronous serial
**         communication. The component supports different settings of
**         parity, word width, stop-bit and communication speed,
**         user can select interrupt or polling handler.
**         Communication speed can be changed also in runtime.
**         The component requires one on-chip asynchronous serial channel.
*/         
/*!
**  @addtogroup PC_module PC module documentation
**  @{
*/         

#ifndef __PC
#define __PC

/* MODULE PC. */

#include "Cpu.h"




#ifndef __BWUserType_PC_TError
#define __BWUserType_PC_TError
  /*lint -save  -esym(960,18.4) Disable MISRA rule (18.4) checking. */
  typedef union {
    byte err;
    struct {
      /*lint -save  -e46 Disable MISRA rule (6.4) checking. */
      bool OverRun  : 1;               /* Overrun error flag */
      bool Framing  : 1;               /* Framing error flag */
      bool Parity   : 1;               /* Parity error flag */
      bool RxBufOvf : 1;               /* Rx buffer full error flag */
      bool Noise    : 1;               /* Noise error flag */
      bool Break    : 1;               /* Break detect */
      bool LINSync  : 1;               /* LIN synchronization error */
      bool BitError  : 1;              /* Bit error flag - mismatch to the expected value happened. */
      /*lint -restore Enable MISRA rule (6.4) checking. */
    } errName;
  } PC_TError;                         /* Error flags. For languages which don't support bit access is byte access only to error flags possible. */
  /*lint -restore  +esym(960,18.4) Enable MISRA rule (18.4) checking. */
#endif

#ifndef __BWUserType_PC_TComData
#define __BWUserType_PC_TComData
  typedef byte PC_TComData ;           /* User type for communication. Size of this type depends on the communication data width. */
#endif

#define PC_INP_BUF_SIZE  0x04U         /* Input buffer size */
#define PC_OUT_BUF_SIZE  0x04U         /* Output buffer size */

extern byte PC_OutLen;                 /* Length of the output buffer content */
extern byte PC_InpLen;                 /* Length of the input buffer content */

byte PC_RecvChar(PC_TComData *Chr);
/*
** ===================================================================
**     Method      :  PC_RecvChar (component AsynchroSerial)
**     Description :
**         If any data is received, this method returns one character,
**         otherwise it returns an error code (it does not wait for
**         data). This method is enabled only if the receiver property
**         is enabled.
**         [Note:] Because the preferred method to handle error and
**         break exception in the interrupt mode is to use events
**         <OnError> and <OnBreak> the return value ERR_RXEMPTY has
**         higher priority than other error codes. As a consequence the
**         information about an exception in interrupt mode is returned
**         only if there is a valid character ready to be read.
**     Parameters  :
**         NAME            - DESCRIPTION
**       * Chr             - Pointer to a received character
**     Returns     :
**         ---             - Error code, possible codes:
**                           ERR_OK - OK
**                           ERR_SPEED - This device does not work in
**                           the active speed mode
**                           ERR_RXEMPTY - No data in receiver
**                           ERR_BREAK - Break character is detected
**                           (only when the <Interrupt service> property
**                           is disabled and the <Break signal> property
**                           is enabled)
**                           ERR_COMMON - common error occurred (the
**                           <GetError> method can be used for error
**                           specification)
** ===================================================================
*/

byte PC_SendChar(PC_TComData Chr);
/*
** ===================================================================
**     Method      :  PC_SendChar (component AsynchroSerial)
**     Description :
**         Sends one character to the channel. If the component is
**         temporarily disabled (Disable method) SendChar method only
**         stores data into an output buffer. In case of a zero output
**         buffer size, only one character can be stored. Enabling the
**         component (Enable method) starts the transmission of the
**         stored data. This method is available only if the
**         transmitter property is enabled.
**     Parameters  :
**         NAME            - DESCRIPTION
**         Chr             - Character to send
**     Returns     :
**         ---             - Error code, possible codes:
**                           ERR_OK - OK
**                           ERR_SPEED - This device does not work in
**                           the active speed mode
**                           ERR_TXFULL - Transmitter is full
** ===================================================================
*/

byte PC_RecvBlock(PC_TComData *Ptr,word Size,word *Rcv);
/*
** ===================================================================
**     Method      :  PC_RecvBlock (component AsynchroSerial)
**     Description :
**         If any data is received, this method returns the block of
**         the data and its length (and incidental error), otherwise it
**         returns an error code (it does not wait for data).
**         This method is available only if non-zero length of the
**         input buffer is defined and the receiver property is enabled.
**         If less than requested number of characters is received only
**         the available data is copied from the receive buffer to the
**         user specified destination. The value ERR_EXEMPTY is
**         returned and the value of variable pointed by the Rcv
**         parameter is set to the number of received characters.
**     Parameters  :
**         NAME            - DESCRIPTION
**       * Ptr             - Pointer to the block of received data
**         Size            - Size of the block
**       * Rcv             - Pointer to real number of the received data
**     Returns     :
**         ---             - Error code, possible codes:
**                           ERR_OK - OK
**                           ERR_SPEED - This device does not work in
**                           the active speed mode
**                           ERR_RXEMPTY - The receive buffer didn't
**                           contain the requested number of data. Only
**                           available data has been returned.
**                           ERR_COMMON - common error occurred (the
**                           GetError method can be used for error
**                           specification)
** ===================================================================
*/

byte PC_SendBlock(const PC_TComData * Ptr,word Size,word *Snd);
/*
** ===================================================================
**     Method      :  PC_SendBlock (component AsynchroSerial)
**     Description :
**         Sends a block of characters to the channel.
**         This method is available only if non-zero length of the
**         output buffer is defined and the transmitter property is
**         enabled.
**     Parameters  :
**         NAME            - DESCRIPTION
**       * Ptr             - Pointer to the block of data to send
**         Size            - Size of the block
**       * Snd             - Pointer to number of data that are sent
**                           (moved to buffer)
**     Returns     :
**         ---             - Error code, possible codes:
**                           ERR_OK - OK
**                           ERR_SPEED - This device does not work in
**                           the active speed mode
**                           ERR_TXFULL - It was not possible to send
**                           requested number of bytes
** ===================================================================
*/

byte PC_ClearRxBuf(void);
/*
** ===================================================================
**     Method      :  PC_ClearRxBuf (component AsynchroSerial)
**     Description :
**         Clears the receive buffer.
**         This method is available only if non-zero length of the
**         input buffer is defined and the receiver property is enabled.
**     Parameters  : None
**     Returns     :
**         ---             - Error code, possible codes:
**                           ERR_OK - OK
**                           ERR_SPEED - This device does not work in
**                           the active speed mode
** ===================================================================
*/

byte PC_ClearTxBuf(void);
/*
** ===================================================================
**     Method      :  PC_ClearTxBuf (component AsynchroSerial)
**     Description :
**         Clears the transmit buffer.
**         This method is available only if non-zero length of the
**         output buffer is defined and the receiver property is
**         enabled.
**     Parameters  : None
**     Returns     :
**         ---             - Error code, possible codes:
**                           ERR_OK - OK
**                           ERR_SPEED - This device does not work in
**                           the active speed mode
** ===================================================================
*/

#define PC_GetCharsInRxBuf() \
((word) PC_InpLen)                     /* Return number of chars in receive buffer */
/*
** ===================================================================
**     Method      :  PC_GetCharsInRxBuf (component AsynchroSerial)
**     Description :
**         Returns the number of characters in the input buffer. This
**         method is available only if the receiver property is enabled.
**     Parameters  : None
**     Returns     :
**         ---             - The number of characters in the input
**                           buffer.
** ===================================================================
*/

#define PC_GetCharsInTxBuf() \
((word) PC_OutLen)                     /* Return number of chars in the transmitter buffer */
/*
** ===================================================================
**     Method      :  PC_GetCharsInTxBuf (component AsynchroSerial)
**     Description :
**         Returns the number of characters in the output buffer. This
**         method is available only if the transmitter property is
**         enabled.
**     Parameters  : None
**     Returns     :
**         ---             - The number of characters in the output
**                           buffer.
** ===================================================================
*/

__interrupt void PC_InterruptRx(void);
/*
** ===================================================================
**     Method      :  PC_InterruptRx (component AsynchroSerial)
**
**     Description :
**         The method services the receive interrupt of the selected 
**         peripheral(s) and eventually invokes the component's event(s).
**         This method is internal. It is used by Processor Expert only.
** ===================================================================
*/

__interrupt void PC_InterruptTx(void);
/*
** ===================================================================
**     Method      :  PC_InterruptTx (component AsynchroSerial)
**
**     Description :
**         The method services the transmit interrupt of the selected 
**         peripheral(s) and eventually invokes the component's event(s).
**         This method is internal. It is used by Processor Expert only.
** ===================================================================
*/

__interrupt void PC_InterruptError(void);
/*
** ===================================================================
**     Method      :  PC_InterruptError (component AsynchroSerial)
**
**     Description :
**         The method services the error interrupt of the selected 
**         peripheral(s) and eventually invokes the component's event(s).
**         This method is internal. It is used by Processor Expert only.
** ===================================================================
*/


void PC_Init(void);
/*
** ===================================================================
**     Method      :  PC_Init (component AsynchroSerial)
**
**     Description :
**         Initializes the associated peripheral(s) and the component's 
**         internal variables. The method is called automatically as a 
**         part of the application initialization code.
**         This method is internal. It is used by Processor Expert only.
** ===================================================================
*/


/* END PC. */

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

#endif /* ifndef __PC */
