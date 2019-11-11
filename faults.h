/* Microchip Technology Inc. and its subsidiaries.  You may use this software 
 * and any derivatives exclusively with Microchip products. 
 * 
 * THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS".  NO WARRANTIES, WHETHER 
 * EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED 
 * WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A 
 * PARTICULAR PURPOSE, OR ITS INTERACTION WITH MICROCHIP PRODUCTS, COMBINATION 
 * WITH ANY OTHER PRODUCTS, OR USE IN ANY APPLICATION. 
 *
 * IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, 
 * INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND 
 * WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS 
 * BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE.  TO THE 
 * FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS 
 * IN ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF 
 * ANY, THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
 *
 * MICROCHIP PROVIDES THIS SOFTWARE CONDITIONALLY UPON YOUR ACCEPTANCE OF THESE 
 * TERMS. 
 */

// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef FAULTS_H
#define	FAULTS_H

#include <stdbool.h>
#include <stdint.h>
#include <p33Exxxx.h>
#include "defines.h"

#define FAULT_BLNKLED_TASK_RATE     300        // In ms, ON/OFF rate of FLT LED
#define FAULT_BLINKLED_DLYCNT       4          // Delay of x*BLNKLEDFLT_TASK_RATE ms between blink sets
#define FAULT_BLINKLED_OFFCNT       6          // Delay of x*BLNKLEDFLT_TASK_RATE ms after last blink set before repeating blink cycle

typedef enum
{
    NON = 0,    // None
    IOC = 1,    // Input Over Current
    IUV = 2,    // Input Under Voltage
    IOV = 3,    // Input Over Voltage
    OUV = 4,    // Output Under Voltage
    OOV = 5,    // Output Over Voltage
    IUF = 6,    // Input Under Line Frequency
    IOF = 7,    // Input Over Line Frequency
    IOP = 8,    // Input Over Power
    IOT = 9,    // Input Over Temp
    APS = 10,   // Auxillary Pwr Supply
    DCD = 11,   // DC2DC Comm Fault
    URT = 12    // UART Fault
                     
} FLT_CODE;

typedef struct
{
   int16_t   fltThreshold;        // Fault threshold limit: Range of -32768 to 32767
   int16_t   fltHysLimit;         // Fault hysteresis limit. If hysteresis is not needed, fltThreshold = fltHysLimit
   uint16_t  fltThresholdCnt;     // Number of consecutive fault events before fault becomes active.  
   uint16_t  fltHysCnt;           // Number of consecutive events when flt input outside hysteresis limit in order to remove fault, set to 0 to disable Hys 
   uint16_t  fltCounter;          // Internal counter for activating/removing fault 
   FLT_CODE  fltCode;             // Code that can be used to display fault (1st fault occurred) to global variable
   uint8_t   faultActive;         // Set/Cleared inside flt check loop
        
}FLT_STATE;   

typedef enum
{
    FAULT_LED_ON = 0,
    FAULT_LED_OFF,
    DLY_BTWN_BLNKS,     // Blink x times, delay, Blink y times, delay, etc.
    DLY_RESTART         // Delay before repeating x,y,z blinks
            
}LED_FLT_STATE;


typedef void (*callback)(uint16_t *fltState);

/*********************************************************************
* Function:  
*
* Overview: 
* Input:  None
* Output: None
********************************************************************/

bool FAULT_CheckMax(FLT_STATE *faultInput, int16_t faultSource, uint16_t *fltState);//, callback functCallBackPtr);

/*********************************************************************
* Function: 
*
* Overview: 
* Input:  None
* Output: None
********************************************************************/
bool FAULT_CheckMin(FLT_STATE *faultInput, int16_t faultSource, uint16_t *fltState);//, callback functCallBackPtr);

/*********************************************************************
* Function: void FAULT_BlinkLED(void)
*
* Overview: Blinks fault LED or I/O according to faultstate
* Input:  None
* Output: None
********************************************************************/
void FAULT_BlinkLED (void);

/*********************************************************************
* Function: 
*
* Overview: 
* Input:  
* Output: None
*
********************************************************************/
void FAULT_SetLEDStates(uint16_t fltState);


/*********************************************************************
* Function: 
*
* Overview: 
* Input:  
* Output: None
*
********************************************************************/
void FAULT_ResetLEDStates(void);

#endif	/* XC_HEADER_TEMPLATE_H */

