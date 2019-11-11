
////////////////////////////////////////////////////////////////////////////////
// © 2015 Microchip Technology Inc.
//
// MICROCHIP SOFTWARE NOTICE AND DISCLAIMER:  You may use this software, and any 
//derivatives, exclusively with Microchip?s products. This software and any 
//accompanying information is for suggestion only.  It does not modify Microchip?s 
//standard warranty for its products.  You agree that you are solely responsible 
//for testing the software and determining its suitability.  Microchip has no 
//obligation to modify, test, certify, or support the software.
//
// THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS".  NO WARRANTIES, WHETHER EXPRESS,
//IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED WARRANTIES OF
//NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A PARTICULAR PURPOSE, OR ITS 
//INTERACTION WITH MICROCHIP PRODUCTS, COMBINATION WITH ANY OTHER PRODUCTS, OR USE 
//IN ANY APPLICATION.
 
//IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, INCIDENTAL 
//OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND WHATSOEVER RELATED TO THE 
//SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS BEEN ADVISED OF THE POSSIBILITY OR 
//THE DAMAGES ARE FORESEEABLE.  TO THE FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S 
//TOTAL LIABILITY ON ALL CLAIMS IN ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED 
//THE AMOUNT OF FEES, IF ANY, THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.

//MICROCHIP PROVIDES THIS SOFTWARE CONDITIONALLY UPON YOUR ACCEPTANCE OF THESE TERMS. 

////////////////////////////////////////////////////////////////////////////////

#include "compensator.h"

int16_t resultacc[9]={0,0,0,0,0,0,0,0,0}; // FOR 3 STAGE CASCADE
int16_t pfcVoltageErrorControlHistory[4] __attribute__ ((space (ymemory), far)); //FOR 3 STAGE CASCADE
int16_t pfcVoltageABCoefficients[9]__attribute__ ((section (".xbss")));  // FOR 3 STAGE CASCADE

SMPS_Controller_Options_T pfcOptions;   // Create variable for pfc converter options


void initPFCComp(void)
{

    MACRO_CLR_VOLTAGEHISTORY();
    
    pfcVoltageABCoefficients[0] = VOLTAGE_COMP_2P2Z_COEFF_B10;
    pfcVoltageABCoefficients[1] = VOLTAGE_COMP_2P2Z_COEFF_B11;
    pfcVoltageABCoefficients[2] = VOLTAGE_COMP_2P2Z_COEFF_A11;

    pfcVoltageABCoefficients[3] = VOLTAGE_COMP_2P2Z_COEFF_B20;
    pfcVoltageABCoefficients[4] = VOLTAGE_COMP_2P2Z_COEFF_B21;
    pfcVoltageABCoefficients[5] = VOLTAGE_COMP_2P2Z_COEFF_A21;
    
    pfcVoltageABCoefficients[6] = VOLTAGE_COMP_2P2Z_COEFF_B30;
    pfcVoltageABCoefficients[7] = VOLTAGE_COMP_2P2Z_COEFF_B31;
    pfcVoltageABCoefficients[8] = VOLTAGE_COMP_2P2Z_COEFF_A31;
//    
    // Library options:        // For 'triggerSelectFlag', only one bit may be set:
    pfcOptions.triggerSelectFlag = 0;   // 00 = Trigger functions disabled (0)
                                        // 10 = Trigger On-Time enabled    (1)
                                        // 10 = Trigger Off-Time enabled   (2)
    pfcOptions.delayValue = 0;
    pfcOptions.trigger    = &TRIG2;  // Pointer to TRIG1 register
    pfcOptions.period     = &PHASE2;  // Pointer to period register

    InitAltRegContext2Setup();       // Setup  working registers to be used with compensator

}


