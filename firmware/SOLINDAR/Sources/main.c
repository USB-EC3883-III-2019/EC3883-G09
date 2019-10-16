/* ###################################################################
**     Filename    : main.c
**     Project     : SOLINDAR
**     Processor   : MC9S08QE128CLK
**     Version     : Driver 01.12
**     Compiler    : CodeWarrior HCS08 C Compiler
**     Date/Time   : 2019-10-02, 12:46, # CodeGen: 0
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
#include "MBit1.h"
#include "Inhr1.h"
#include "Inhr2.h"
#include "Inhr3.h"
#include "Inhr4.h"
#include "Trigger.h"
#include "TI1.h"
#include "TI2.h"
#include "Echo.h"
/* Include shared modules, which are used for whole project */
#include "PE_Types.h"
#include "PE_Error.h"
#include "PE_Const.h"
#include "IO_Map.h"

/* User includes (#include below this line is not maintained by Processor Expert) */
#define STEPS_MODE 0    //0 for whole steps, 1 for half steps
#define MAX_POS 64      

//Constants
//Array for half steps
const char steps[] = { 10,     //1010
                       11,     //1000
                        9,     //1001
                        1,     //0001
                        5,     //0101
                        4,     //0100
                        6,     //0110
                        2,     //0010
};
char aux_flag = 0;



void main(void)
{
  /* Write your local variable definition here */

  char pos = 0, frame[4];
  /*
   * pos       : position. Motor's position.
   * frame     : frame for serial communication.
   */
  bool dir_flg = TRUE, trgg_flg = TRUE;
  /*
   * dir_flg   : direction flag. TRUE for clockwise, FALSE for counterclockwise.
   * trgg_flg  : trigger flag. 
   */
  unsigned int echo_time, son_dis, lid_vol;
  /*
   * echo_time : echo time. Duration of echo signal in HIGH state.
   * son_dis   : sonar distance. Distance measured by the SONAR.
   * lid_vol   : lidar voltage. Voltage output of LIDAR.
   */
   
		  
  /*** Processor Expert internal initialization. DON'T REMOVE THIS CODE!!! ***/
  PE_low_level_init();
  /*** End of Processor Expert internal initialization.                    ***/

  /* Write your code here */
  /* For example: for(;;) { } */
 
  for (;;){
	  
	  

/*###################################################################
						Lidar Control
###################################################################*/
	  
	  
	  
/*###################################################################
				    	End of Lidar Control
###################################################################*/
/*###################################################################
							Sonar Control
###################################################################*/

    //So far, it only takes one sample.

	  //Trigger
	  Trigger_SetVal();	//Set output to HIGH. Start trigger.
	  TI1_Enable();		//Enable interrupt service.
	  while(trgg_flg){}	//Wait for interrupt.
	  
	  //Ultrasonic burst
	  
	  //Echo
	  
	  //Calculate Distance
	  son_dis = echo_time/58;
	  
/*###################################################################
		     	       End of Sonar Control
###################################################################*/
/*###################################################################
                          Motor Control
###################################################################*/
/*
	  
	  if(aux_flag){ //for stand-alone simulation purpose
    //Applying signal
    MBit1_PutVal(steps[pos%8]);
    
    //Signal for the next step
    if(STEPS_MODE){
    	//half steps
      if(dir_flg){
        pos++;	
      }else{
        pos--;
      }
    }else{
        //whole steps
      if(dir_flg){
        pos += 2;	
      }else{
        pos -= 2;
      }
    }
    
    //Switching rotation's direction if counter is out of range
    if(pos>=MAX_POS || pos<=0){
    	dir_flg = !dir_flg;
    }
	aux_flag = 0;	//for stand-alone simulation purpose
	  }
    
/*###################################################################
				    	End of Motor Control
###################################################################*/
/*###################################################################
				     	 	Frame Construction
###################################################################*/
/*
    //First byte
    if(STEPS_MODE){
      //Half steps
      frame[0] = pos;
    }else{
      //Whole steps
      frame[0] = pos/2;
    }

    //Second byte
    frame[1] = 0b10000000 | (son_dis >> 2);

    //Third byte 
    frame[2] = 0b10000000 | (son_dis << 5) | (lid_vol >> 7);

    //Forth byte
    frame[3] = 0b10000000 | lid_vol;

/*###################################################################
						End of Frame Construction
###################################################################*/
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
