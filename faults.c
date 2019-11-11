/*
 * File:   faults.c
 * Author: C17431
 *
 * Created on February 20, 2018, 8:52 AM
 */


#include "xc.h"
#include "faults.h"
#include "defines.h"

#define DRV_FLT_LED     FAULT_LED_PORT   

static LED_FLT_STATE fltLEDState = FAULT_LED_ON; 
static uint8_t fltLEDToggleCnt = 0, fltDelayBlinks = 0;
static uint8_t fltLEDBlinkCnt[3] = {0,0,0};
static uint8_t *fltLEDBlinkCntPtr = &fltLEDBlinkCnt[0];

//extern volatile FLT_CODE faultCode; 

/*******************************************************************************
 * Function:       FAULT_CheckMax
 * Inputs:         Pointer to faultStructure, input fault source, pointer to 
 *                 variable that captures fault code, and call back function pointer 
 * Output:         Returns 1 if fault active else 0  
 * Description:	   Checks if input fault source is greater than set max threshold,
 *                 sets fltState which captures event and will remove fault once
 *                 input source goes below Hysteresis limit (if set). Function does 
 *                 not clear fltState. 
*******************************************************************************/
bool FAULT_CheckMax(FLT_STATE *faultInput, int16_t faultSource, uint16_t *fltState)
{    
    //If fault present return 1, else return 0
    if((faultSource >= faultInput->fltThreshold) && (faultInput->faultActive == false))
    {
        faultInput->fltCounter++;
        if(faultInput->fltCounter > faultInput->fltThresholdCnt)
        {   
            if(*fltState == false) 
                *fltState = faultInput->fltCode;
            
            faultInput->faultActive = 1;
            faultInput->fltCounter = 0;

            return true;
        }
        else {
            return false;
        }
    } 
    else if((faultSource < (faultInput->fltHysLimit)) && (faultInput->faultActive == true))
    {
        faultInput->fltCounter++;
        if(faultInput->fltCounter > faultInput->fltHysCnt)
        {  
            faultInput->fltCounter = 0;
            faultInput->faultActive = 0;
            return false;
        }
        else {
            return (faultInput->faultActive);    //true
        }
    }
    else {
        faultInput->fltCounter = 0;
       return (faultInput->faultActive);
    }
}


/*******************************************************************************
 * Function:       FAULT_CheckMin
 * Inputs:         Pointer to faultStructure, input fault source, pointer to 
 *                 variable that captures fault code, and call back function pointer  
 * Output:         Returns 1 if fault active else 0  
 * Description:    Checks if input fault source is less than set threshold,
 *                 sets fltState which captures event and will remove fault once
 *                 input source goes above Hysteresis limit (if set). Function does 
 *                 not clear fltState.  
*******************************************************************************/
bool FAULT_CheckMin(FLT_STATE *faultInput, int16_t faultSource, uint16_t *fltState)
{
    //If fault present return 1, else return 0
    if((faultSource <= faultInput->fltThreshold) && (faultInput->faultActive == false))
    {
        faultInput->fltCounter++;
        if( faultInput->fltCounter > faultInput->fltThresholdCnt)
        {
            if(*fltState == false)
				*fltState = faultInput->fltCode;
                        
            faultInput->faultActive = 1;
            faultInput->fltCounter = 0;
            
            return true;
        }
        else {
            return false;
        }
    } 
    else if((faultSource > (faultInput->fltHysLimit)) && (faultInput->faultActive == true))
    {
        faultInput->fltCounter++;
        if( faultInput->fltCounter > faultInput->fltHysCnt)
        {  
            faultInput->fltCounter = 0;
            faultInput->faultActive = 0;
            return false;
        }
        else {
            return (faultInput->faultActive);   //true
        }
    }    
    else {
        faultInput->fltCounter = 0; 
        return (faultInput->faultActive);
    }
}


/*******************************************************************************
 * Function: 	FAULT_SetLEDStates
 * Description:	This function should be called prior to scheduling the FAULT_BlinkLED
 *              task to determine the blink counts based on fault code 
*******************************************************************************/
void FAULT_SetLEDStates (uint16_t fltState)
{
    fltLEDBlinkCnt[0] = (fltState >> 8);
    fltLEDBlinkCnt[1] = fltLEDBlinkCnt[0] + ((fltState & 0x00F0) >> 4); 
    fltLEDBlinkCnt[2] = fltLEDBlinkCnt[1] + (fltState & 0x000F); 
}

/*******************************************************************************
 * Function: 	FAULT_ResetLEDStates
 * Description:	This function should be used after fault was removed and FAULT_BlinkLED
 *              task is no longer scheduled. This functions clears all states related 
 *              to blinking the fault LED to prepare for next use. 
*******************************************************************************/
void FAULT_ResetLEDStates(void)
{ 
    fltLEDBlinkCntPtr = &fltLEDBlinkCnt[0]; 
    fltLEDState = FAULT_LED_ON;
    fltLEDToggleCnt = fltDelayBlinks = 0;
}


/*******************************************************************************
 * Function: 	FAULT_BlinkLED
 * Description:	This function should be scheduled when system enters fault mode, 
 *              LED will blink based on fault code passed to FAULT_SetLEDStates
*******************************************************************************/
void FAULT_BlinkLED(void)
{     
    if(fltLEDBlinkCnt[0] == 0)
        return;         // If 0, SetFaultLEDStates wasn't called which means  
                        // missing information to blink LED properly
     
    switch(fltLEDState)
    {
        case FAULT_LED_ON:
            DRV_FLT_LED = true;
            fltLEDState = FAULT_LED_OFF;
        break;

        case FAULT_LED_OFF:
            DRV_FLT_LED = false;

            if(++fltLEDToggleCnt < *fltLEDBlinkCntPtr)
            {
                fltLEDState = FAULT_LED_ON;
            }
            else 
            {
                fltLEDState = DLY_BTWN_BLNKS;
                fltLEDBlinkCntPtr++;
            }

        break;

        case DLY_BTWN_BLNKS: // Delay between blink sets

            if(++fltDelayBlinks >= (FAULT_BLINKLED_DLYCNT-1))
            {
                fltLEDState = (fltLEDToggleCnt >= *fltLEDBlinkCntPtr) ? DLY_RESTART : FAULT_LED_ON;
                fltDelayBlinks = 0;
            }    

        break;

        case DLY_RESTART: // Longer delay after last blink set and before restarting
            if(++fltDelayBlinks >= (FAULT_BLINKLED_OFFCNT - 1))
            {
                // Reset States
                fltLEDBlinkCntPtr = &fltLEDBlinkCnt[0]; 
                fltLEDState = FAULT_LED_ON;
                fltLEDToggleCnt = fltDelayBlinks = 0;
            }
        break;

        default:

        break;

    }    
}