/*******************************************************************************
Microchip's products.  Microchip and its licensors retain all ownership and
intellectual property rights in the accompanying software and in all
derivatives here to.

This software and any accompanying information is for suggestion only. It
does not modify Microchip's standard warranty for its products. You agree
that you are solely responsible for testing the software and determining its
suitability. Microchip has no obligation to modify, test, certify, or
support the software.

THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS".  NO WARRANTIES, WHETHER
EXPRESS, IMPLIED OR STATUTORY, INCLUDING, BUT NOT LIMITED TO, IMPLIED
WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A PARTICULAR
PURPOSE APPLY TO THIS SOFTWARE, ITS INTERACTION WITH MICROCHIP?S PRODUCTS,
COMBINATION WITH ANY OTHER PRODUCTS, OR USE IN ANY APPLICATION.

IN NO EVENT, WILL MICROCHIP BE LIABLE, WHETHER IN CONTRACT, WARRANTY, TORT
(INCLUDING NEGLIGENCE OR BREACH OF STATUTORY DUTY), STRICT LIABILITY,
INDEMNITY, CONTRIBUTION, OR OTHERWISE, FOR ANY INDIRECT, SPECIAL, PUNITIVE,
EXEMPLARY, INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, FOR COST OR EXPENSE OF
ANY KIND WHATSOEVER RELATED TO THE SOFTWARE, HOWSOEVER CAUSED, EVEN IF
MICROCHIP HAS BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE.
TO THE FULLEST EXTENT ALLOWABLE BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL
CLAIMS IN ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF
FEES, IF ANY, THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.

MICROCHIP PROVIDES THIS SOFTWARE CONDITIONALLY UPON YOUR ACCEPTANCE OF
THESE TERMS.
*******************************************************************************/

#include "bulkVoltControl.h"
#include "defines.h"

/*******************************************************************************
Global variables
*******************************************************************************/
uint16_t vacRMSSquared = 0, xoverFactor = 0;
uint16_t sftStartTimer = 0;
uint16_t pfcVoltCompGain = 32767;
uint16_t temp,phaseThreshold,phaseState;
uint16_t pfcVoltCompOutputMax = VOLTAGE_COMP_2P2Z_MAX_CLAMP;

extern uint16_t transitionStartFlag;
extern uint16_t pfcBulkVoltageFiltered;
extern volatile int16_t  pfcVoltageRef;
extern PFC_FLAGS pfcStateFlags;

extern volatile uint16_t UARTOnTime, UARTVoltageReference;

volatile uint16_t tempPDC1,tempPDC2,zeroCrossDetect;
volatile uint16_t vacInputFiltered;
volatile uint16_t transitionLHEndFlag,transitionHLEndFlag;

volatile int16_t pfcVoltCompOutput = 0;

/*******************************************************************************
ISR: 		TMR3Interrupt
Description:	Interrupt for voltage compensator, bulk voltage reduction/boost,
                and converter softstart
Working Reg:    Uses Alt W-Reg Bank #2 for compensator (manual switch)
*******************************************************************************/
//void __attribute__((__interrupt__,no_auto_psv)) _T3Interrupt()
void TMR3_CallBack(void)
{
    static uint16_t sftStrtInitVoltage = 0, sftStrtVoltageDelta = 0;
    int16_t sftStrtScaler;
        
    #if(DEBUG_IO == VOLTAGELOOP)
    IO_DRV = ON;
    #endif
        
    // Softstart code and bulk voltage adaption placed here before compensator
    if(pfcStateFlags.firstPassT3ISR == ACTIVE) {
        sftStrtInitVoltage = pfcBulkVoltageFiltered;
        sftStrtVoltageDelta = PFCVOLTAGEREFADC - pfcBulkVoltageFiltered;
        pfcStateFlags.firstPassT3ISR = INACTIVE;

        pfcStateFlags.softStart = ACTIVE;
      }

    if(pfcStateFlags.softStart == ACTIVE) {  
       
        
    #if(DEBUG_IO == SSMODE)
    IO_DRV = ON;
    #endif
       if((sftStartTimer&3) == 0){
            sftStrtScaler = (__builtin_mulss(SOFTSTARTSCALER ,sftStartTimer) >> 10);
            pfcVoltageRef = sftStrtInitVoltage + (__builtin_mulss(sftStrtVoltageDelta, sftStrtScaler) >> 15);
       }

       if(sftStartTimer >= SOFTSTARTCOUNT) {         
           pfcVoltageRef = PFCVOLTAGEREFADC;
           pfcStateFlags.softStart = INACTIVE;
           pfcStateFlags.steadyState = ACTIVE;
           
           #if(DEBUG_IO == SSMODE)
             IO_DRV = OFF;
           #endif
           
       }
    
        ++sftStartTimer;
    }
    else if (pfcStateFlags.steadyState == ACTIVE){   
             
    }

    // Call 2P2Z Compensator
    __asm__("CTXTSWP #0x2");
    SMPS_Controller2P2ZUpdate_HW_Accel();  
    __asm__("CTXTSWP #0x0");

   #if(CONSTXOVERFREQ)
        xoverFactor = __builtin_divud(MAXINPUTVOLTAGEADCSQUARED,vacRMSSquared);   //32-16 = 16   
    #else
        xoverFactor = 256;    //2^8 = 1
    #endif
    
    pfcVoltCompOutput = (__builtin_muluu(pfcVoltCompOutput,xoverFactor)) >> 8;      //Multiply by crossover factor
    
    /*Limit ON time if Current Derating is Enabled*/
    if(pfcVoltCompOutput >= pfcVoltCompOutputMax)
        pfcVoltCompOutput = pfcVoltCompOutputMax;
    
    #if(ZC_DIST_IMPR_STEP && (!PHASE_MGMT))
        if(vacInputFiltered < VTHRESHADC)
        {
            PDC1 = 0;
            PDC2 = pfcVoltCompOutput << 1;  //PDC*2
         
            IOCON1bits.OVRENH = 1;    //Disable PWM1  
            //IOCON5bits.OVRENH = 1;      // Sync pulse disabled
            transitionStartFlag = 1;
        }

        else
        {
            transitionStartFlag = 0;
            PDC1 = pfcVoltCompOutput;
            PDC2 = pfcVoltCompOutput;
            IOCON1bits.OVRENH = 0;    //Enable PWM1  
    //            if(countCycles++ >= 10)
    //            IOCON5bits.OVRENH = 0;      // Override disabled, PWM controls
        }
    #endif
    
    
     #if(PHASE_MGMT && (!ZC_DIST_IMPR_STEP))
        
        if(zeroCrossDetect && (pfcStateFlags.steadyState == ACTIVE))
        {
            zeroCrossDetect = 0;
            
            #if(!CONSTXOVERFREQ)
                xoverFactor = __builtin_divud(MAXINPUTVOLTAGEADCSQUARED,vacRMSSquared);   //32-16 = 16   
            #endif
            phaseThreshold = (__builtin_muluu(PHASE_MGMT_THRESHOLD,xoverFactor)) >> 8;      //Multiply by crossover factor
            //phaseThreshold = PHASE_MGMT_THRESHOLD;
            
            if(pfcVoltCompOutput <= (phaseThreshold-PHASE_MGMT_HYST))
            {
                PDC1 = 0;
                PDC2 = pfcVoltCompOutput << 1;
                phaseState = 1;
            }
            else if(pfcVoltCompOutput >= (phaseThreshold+PHASE_MGMT_HYST))
            {
                PDC1 = pfcVoltCompOutput;
                PDC2 = pfcVoltCompOutput;
                phaseState = 2;
            }
            else if((pfcVoltCompOutput >= (phaseThreshold-PHASE_MGMT_HYST)) && (pfcVoltCompOutput <= (phaseThreshold+PHASE_MGMT_HYST)))
            {
                if(phaseState == 1)
                {
                    PDC1 = 0;
                    PDC2 = pfcVoltCompOutput << 1;
                }
                else if(phaseState == 2)
                {
                    PDC1 = pfcVoltCompOutput;
                    PDC2 = pfcVoltCompOutput;
                }
            }
                        
            phaseState = pfcVoltCompOutput;
        }
        else if(pfcStateFlags.softStart == ACTIVE)  //Allow both units during softstart
        {
                PDC1 = pfcVoltCompOutput;
                PDC2 = pfcVoltCompOutput;
        }
        
    #endif
    
    #if(GUI_ON_TIME && (!ZC_DIST_IMPR_STEP) && (!PHASE_MGMT))
        if(vacInputFiltered < VTHRESHADC)
        {
            PDC1 = UARTOnTime;
            PDC2 = UARTOnTime;         
        }
        else
        {
            PDC1 = pfcVoltCompOutput;
            PDC2 = pfcVoltCompOutput;       
        }

    #endif
    
    #if((!PHASE_MGMT) && (!ZC_DIST_IMPR_STEP) && (!GUI_ON_TIME))    
        PDC1 = pfcVoltCompOutput;
        PDC2 = pfcVoltCompOutput;       
    #endif
    
    /* Calculate Trigger Point */
    TRIG1 = (PDC1>>1)+30;
    TRIG2 = (PDC2>>1)+30;
          
    #if (DACOUTPIN == VLOOPOUT)
    CMP2DAC = pfcVoltCompOutput>>1;
    #endif
    
    #if (DACOUTPIN == VOLTREF)
    CMP2DAC = pfcVoltageRef;
    #endif
    
    #if(DACOUTPIN == VOLTERROR)
    if(pfcBulkVoltageFiltered > pfcVoltageRef){
        CMP2DAC = (pfcBulkVoltageFiltered - pfcVoltageRef);
    }
    else{
        CMP2DAC = (pfcVoltageRef - pfcBulkVoltageFiltered);
    }    
    #endif
    
    
    #if(DEBUG_IO == VOLTAGELOOP)
    IO_DRV = OFF;
    #endif
    
//    _T3IF = 0;       // Clear Interrupt Flag
}
/*******************************************************************************
End of ISR
*******************************************************************************/



