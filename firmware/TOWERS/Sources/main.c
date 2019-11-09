/* ###################################################################
**     Filename    : main.c
**     Project     : TOWERS
**     Processor   : MC9S08QE128CLK
**     Version     : Driver 01.12
**     Compiler    : CodeWarrior HCS08 C Compiler
**     Date/Time   : 2019-11-08, 11:22, # CodeGen: 0
**     Abstract    :
**         Main module.
**         This module contains user's application code.
**     Settings    :
**     Contents    :
**         No public methods
**
** ###################################################################*/
/*!
** @file main.c
** @version 01.12
** @brief
**         Main module.
**         This module contains user's application code.
*/         
/*!
**  @addtogroup main_module main module documentation
**  @{
*/         
/* MODULE main */


/* Including needed modules to compile this module/procedure */
#include "Cpu.h"
#include "Events.h"
#include "PC.h"
#include "IR.h"
#include "Bit1.h"
/* Include shared modules, which are used for whole project */
#include "PE_Types.h"
#include "PE_Error.h"
#include "PE_Const.h"
#include "IO_Map.h"

/* User includes (#include below this line is not maintained by Processor Expert) */
#define MAX_POS 64   

//Constants
//Array for half steps
const char steps[] = { 10,     //1010
                        8,     //1000
                        9,     //1001
                        1,     //0001
                        5,     //0101
                        4,     //0100
                        6,     //0110
                        2,     //0010
};
char z1, z2, z3, z4;

char determineZone(char frame[]){
	
	char zone;
	if(frame[2] > 7){
		//TOWER 1
		zone = frame[2] >> 3;
		frame[2] = frame[2] & 0b00000111;
	}else{
		if(frame[2] < 8 && frame[2] > 0){
			//TOWER 2		
			zone = frame[2];
			frame[2] = 0b00000000;
		}else{
			if(frame[3] > 7){
					//TOWER 3
					zone = frame[3] >> 3;
					frame[3] = frame[3] & 0b00000111;

			}else{
				if(frame[3] < 8 && frame[3] > 0){
					//TOWER 4		
					zone = frame[3];
					frame[3] = 0b00000000;
				}
			}
		}
	}
	
	return zone;
}

void main(void)
{
  /* Write your local variable definition here */
	char frame[4], zone;
	word ptr;

    
  /*** Processor Expert internal initialization. DON'T REMOVE THIS CODE!!! ***/
  PE_low_level_init();
  /*** End of Processor Expert internal initialization.                    ***/

  /* Write your code here */
  for(;;) {
	  
	  //if (master)
	  zone = determineZone(frame);	//Determine the zone to move to
	  
  }

  /*** Don't write any code pass this line, or it will be deleted during code generation. ***/
  /*** RTOS startup code. Macro PEX_RTOS_START is defined by the RTOS component. DON'T MODIFY THIS CODE!!! ***/
  #ifdef PEX_RTOS_START
    PEX_RTOS_START();                  /* Startup of the selected RTOS. Macro is defined by the RTOS component. */
  #endif
  /*** End of RTOS startup code.  ***/
  /*** Processor Expert end of main routine. DON'T MODIFY THIS CODE!!! ***/
  for(;;){}
  /*** Processor Expert end of main routine. DON'T WRITE CODE BELOW!!! ***/
} /*** End of main routine. DO NOT MODIFY THIS TEXT!!! ***/

/* END main */
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
