/*
 * File:   compensator.c
 * Author: C17431
 *
 * Created on February 23, 2018, 11:23 AM
 */

#include "xc.h"
#include "compensator.h"

volatile int16_t pfcVoltageABCoefficients[6] __attribute__((section (".xbss")));
volatile int16_t pfcVoltageErrorControlHistory[3] __attribute__ ((space (ymemory), far));

SMPS_Controller_Options_T trigOptions;  // Variable for trigger options

/*******************************************************************************
Function: 	initPFCComp
Description:	Initialize compensators for PFC
*******************************************************************************/
void initPFCComp(void)
{
    MACRO_CLR_VOLTAGEHISTORY();
    
    pfcVoltageABCoefficients[0] = VOLTAGE_COMP_2P2Z_COEFF_B11;
    pfcVoltageABCoefficients[1] = VOLTAGE_COMP_2P2Z_COEFF_B10;
    pfcVoltageABCoefficients[2] = VOLTAGE_COMP_2P2Z_COEFF_A11;
    pfcVoltageABCoefficients[3] = VOLTAGE_COMP_2P2Z_COEFF_B21;
    pfcVoltageABCoefficients[4] = VOLTAGE_COMP_2P2Z_COEFF_B20;
    pfcVoltageABCoefficients[5] = VOLTAGE_COMP_2P2Z_COEFF_A21;
    
    // Library options:        // For 'triggerSelectFlag', only one bit may be set:

    trigOptions.triggerSelectFlag = 0;  // 00 = Trigger functions disabled (0)
                                        // 10 = Trigger On-Time enabled    (1)
                                        // 10 = Trigger Off-Time enabled   (2)
    trigOptions.delayValue = 0;
    trigOptions.trigger    = &TRIG2;  // Pointer to TRIG1 register
    trigOptions.period     = &PHASE2;  // Pointer to period register
    
    AltWREG2Setup();        // Setup working registers to be used with compensator
   
}
/*******************************************************************************
End of Function
*******************************************************************************/

/*******************************************************************************
Function: 	AltWREG2Setup
Description: Setup working registers to be used with compensator
*******************************************************************************/
void AltWREG2Setup(void)
{
    /* Swap to Alternate W-Reg #2 */
    asm("CTXTSWP #0x2");

    /* W0, W1, W2 used for reference, voltage and dutycycle */
    asm("mov #_pfcVoltageRef, w0");
    asm("mov #_pfcBulkVoltageFiltered, w1");
    asm("mov #_pfcVoltCompOutput, w2");

    /* W3, W4, W5 used for ACCAx, MAC/MPY instructions */
    
    /* W6, W7 for postscaler */
    //asm("mov #VOLTAGE_COMP_1_2P2Z_POSTSCALER, w6");
    asm("mov #32767, w6");
    //asm("mov #VOLTAGE_COMP_2_2P2Z_POSTSCALER, w7");
    asm("mov #32767, w7");
    
    /* W9, W10 for coefficients and error history */
    asm("mov #_pfcVoltageABCoefficients, w9");
    asm("mov #_pfcVoltageErrorControlHistory, w10");
    
    /* W11, W12 for output clamp */
    //asm("mov #VOLTAGE_COMP_2P2Z_MIN_CLAMP, w11");
      asm("mov #0x0000, w11");
//    asm("mov #VOLTAGE_COMP_2P2Z_MAX_CLAMP, w12");
      asm("mov #2000, w12");   

    /* W13, W14 for postshift */
    //asm("mov #VOLTAGE_COMP_1_2P2Z_POSTSHIFT, w13");
    asm("mov #0, w13");
    //asm("mov #VOLTAGE_COMP_2_2P2Z_POSTSHIFT, w14");
    asm("mov #0, w14");
    
    /* Swap back to W-Reg #0 */
    asm("CTXTSWP #0x0");
    
}
/*******************************************************************************
End of Function
*******************************************************************************/

