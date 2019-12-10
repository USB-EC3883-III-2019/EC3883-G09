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
#include "MBit1.h"
#include "Inhr1.h"
#include "Inhr2.h"
#include "Inhr3.h"
#include "Inhr4.h"
#include "MotorInt.h"
#include "PWM1.h"
#include "LIDAR.h"
#include "LIDARInt.h"
#include "MotorInt2.h"
#include "SendInt.h"
/* Include shared modules, which are used for whole project */
#include "PE_Types.h"
#include "PE_Error.h"
#include "PE_Const.h"
#include "IO_Map.h"

/* User includes (#include below this line is not maintained by Processor Expert) */

//Constants
#define MAX_POS 64
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
//Global variables
char error = 80;
bool motorFlag = TRUE, lidarFlag = TRUE, bufferFlag = TRUE, bufferPCFlag = TRUE, sendFlag = TRUE;


char masterCheck(char frame[]){
	if(frame[0] >> 6 == 3){
		return 0;
	}else{
		return 1;
	}
	
}

char determineZone(char frame[]){
	//This function determines the zone that the tower
	//is going to look for the other one to retransmit 
	//the message and adjusts the frame.
	
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
				}else{
					return 255;
				}
			}
		}
	}
	
	return zone;
}

void move2Zone(char zoneNumber){
	//This function moves the motor to the middle 
	//of the corresponding zone

	if(zoneNumber > 0){
		//If already in zone 1, don't move. Otherwise, move.
		
		char i = 0, stepsOffset = (zoneNumber - 1)*12 + 6;	//12 is the number of half steps per zone 
															//+6 to move to the centre of the zone
		MotorInt_Enable();
		for(i; i < stepsOffset; i++){
			
			while(motorFlag){}
			MBit1_PutVal(steps[i%8]);
			motorFlag = TRUE;
		}
		MotorInt_Disable();
		motorFlag = TRUE;
	}
}

int measureLidar(){
	//This function handles the LIDAR measurements.
	//It takes 8 samples in each position and
	//calculates the average.
	
	unsigned int measurement, accumulator = 0;
	char i = 0;
	
	LIDARInt_Enable();
	
	for(i; i < 8; i++){
		while(lidarFlag){}	//Wait for interrupt.
		LIDAR_Measure(TRUE);
		LIDAR_GetValue(&measurement);
		accumulator += measurement;
		lidarFlag = TRUE;	//Restoring the flag.
	}
	
	LIDARInt_Disable();
	
	return accumulator/8;	
}

void motorStep(char zoneNumber, char i){
	//This function handles the motor's motion
	
	char position = (zoneNumber - 1)*12 + i;
	
	while(motorFlag){}
	MBit1_PutVal(steps[position%8]);
	motorFlag = TRUE;
	
}

char findReceiver(char zoneNumber){
	//With this function, the tower scans the zone to 
	//determine the receiver's location
	//which is the closest object. 
	//Since the LIDAR varies as 1/x,
	//we have to look for the largest
	//measurement.
	
	char position = 0, i = 0;
	unsigned int largest = 0, measurement;
	
	MotorInt2_Enable();
	for(i; i < 12; i++){
		
		measurement = measureLidar();
		
		if(measurement > largest){
			largest = measurement;
			position = i;
		}
		
		motorStep(zoneNumber, i);
		
	}
	MotorInt2_Disable();
	return position;
} 

void move2Receiver(char receiver, char zoneNumber){
	//This function moves the tower
	//to establish sight line between
	//transmitter and receiver
	
	char currentPosition = zoneNumber*12 - 1, finalPosition = currentPosition - (11 - receiver);
	
	MotorInt_Enable();	//30ms
	
	for (currentPosition; (currentPosition - 1) >= finalPosition; currentPosition--){
		
		while(motorFlag){}
		MBit1_PutVal(steps[currentPosition%8]);
		motorFlag = TRUE;
		
	}
	
	MotorInt_Disable();	//30ms
}

void sync(char auxFrame[], char frame[]){
	//This part checks which byte is the head of the frame, 
	//for sync purposes
	char i = 0, offset = 0;
	for(i; i < 5; i++){
		if((auxFrame[i] >> 4) == 8){
			offset = i;
		} 
	}
	
	i = 0;
	for(i; i<4; i++){
	frame[i] = auxFrame[offset + i];
	}
}


void main(void)
{
  /* Write your local variable definition here */
	char frame2Send[4], frameReceived[4], auxFrame[8], i, zone, receiverPosition, mode, done;
	word ptr;

    
  /*** Processor Expert internal initialization. DON'T REMOVE THIS CODE!!! ***/
  PE_low_level_init();
  /*** End of Processor Expert internal initialization.                    ***/

  /* Write your code here */
  
  SendInt_Disable();
  do{
	  //Wait for PC to tell if it is master or slave. If master, receive data.
	  while(bufferPCFlag){}
	  bufferPCFlag = TRUE;
	  error = PC_RecvBlock(frame2Send,sizeof(frame2Send),&ptr);
  }while(error != ERR_OK);
  
  mode = masterCheck(frame2Send);	//mode = 1 if master and 0 if slave
  
  if(mode){
		//Case Master
		
		//Move tower
		zone = determineZone(frame2Send);	//Determine the zone of the other tower's receiver.
		move2Zone(zone);				//Move to the middle of the specified zone.
//		receiverPosition = findReceiver(zone);	//Determine the receiver's position.
//		move2Receiver(receiverPosition, zone);	//Move the tower to the receiver's position.
		
		for(;;){
		  
		  SendInt_Enable();
		  i = 0;
		  
		  for(i; i < 4; i++){
			  //Send 4 times, 50-millisecond intervals
			  while(sendFlag){}	//Wait for interrupt to occur.
			  sendFlag = TRUE;
			  IR_SendBlock(frame2Send, sizeof(frame2Send), &ptr);	//Send message via IR.
		  }
		  sendFlag = TRUE;
//		  SendInt_Disable();
		  
		  
		  i = 0;
		  IR_ClearRxBuf();
		  for(i;i < 4; i++){
			  while(bufferFlag && sendFlag){}	//Wait for the buffer to be full or timeout
			  bufferFlag = TRUE;
		  	  sendFlag = TRUE;
			  
			  IR_RecvBlock(&auxFrame, sizeof(auxFrame), &ptr);	//Check if a message was received
			  
			  sync(auxFrame, frameReceived);	//Sync received data 
			  done = determineZone(frameReceived);
//			  done = 255;
//			  frameReceived[0] = 0b10001111;
//			  frameReceived[1] = 0b00000000;
//			  frameReceived[2] = 0b00000000;
//			  frameReceived[3] = 0b00000000;
			  
			  if(done == 255){
				  PC_SendBlock(frameReceived, sizeof(frameReceived), &ptr);	//Send new data to PC
			  }
		  }
		}
  
	}else{
	  //Case Salve
	  
		while(bufferFlag){}	//Wait for the buffer to be full
		bufferFlag = TRUE;
		
		IR_RecvBlock(&auxFrame, sizeof(auxFrame), &ptr);	//Check if a message was received
		
		sync(auxFrame, frameReceived);	//Sync received data 
				  
		PC_SendBlock(frameReceived, sizeof(frameReceived), &ptr);	//When message is received, send it to PC
		
		//Move tower
		zone = determineZone(frameReceived);	//Determine the zone of the other tower's receiver.
		move2Zone(zone);				//Move to the specified zone.
//		receiverPosition = findReceiver(zone);	//Determine the receiver's position.
//		move2Receiver(receiverPosition, zone);	//Move the tower to the receiver's position.
		
		for(;;){
			IR_SendBlock(frameReceived, sizeof(frameReceived), &ptr);	//Send message via IR
			Cpu_Delay100US(300);
		}
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
