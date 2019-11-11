/**
  CMP4 Generated Driver API Header File

  @Company
    Microchip Technology Inc.

  @File Name
    cmp4.h

  @Summary
    This is the generated header file for the CMP4 driver using PIC24 / dsPIC33 / PIC32MM MCUs

  @Description
    This header file provides APIs for driver for CMP4.
    Generation Information :
        Product Revision  :  PIC24 / dsPIC33 / PIC32MM MCUs - pic24-dspic-pic32mm : v1.35
        Device            :  dsPIC33EP64GS806
    The generated drivers are tested against the following:
        Compiler          :  XC16 1.31
        MPLAB 	          :  MPLAB X 3.60
*/

/*
    (c) 2016 Microchip Technology Inc. and its subsidiaries. You may use this
    software and any derivatives exclusively with Microchip products.

    THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER
    EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED
    WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A
    PARTICULAR PURPOSE, OR ITS INTERACTION WITH MICROCHIP PRODUCTS, COMBINATION
    WITH ANY OTHER PRODUCTS, OR USE IN ANY APPLICATION.

    IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE,
    INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND
    WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS
    BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO THE
    FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN
    ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF ANY,
    THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.

    MICROCHIP PROVIDES THIS SOFTWARE CONDITIONALLY UPON YOUR ACCEPTANCE OF THESE
    TERMS.
*/

#ifndef _CMP4_H
#define _CMP4_H

/**
  Section: Included Files
*/
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

/**
    Section: Enumerations
*/
/**
  @Summary
    The input sources to the comparator (Non inverting inputs)

  @Description
    The source values that are provided to the Non-inverting inputs of the 
    Comparator.  These could be CMPxA = 0, CMPxB = 1, CMPxC = 2, CMPxD = 3, 
    PGA1 = 0 and PGA2 = 1.  

*/
typedef enum
{
    CMP4_INPUT_CMPA = 0,
    CMP4_INPUT_PGA1 = 0, 
    CMP4_INPUT_CMPB = 1,
    CMP4_INPUT_PGA2 = 1,
    CMP4_INPUT_CMPC = 2,
    CMP4_INPUT_CMPD = 3	
}CMP4_INPUT;

#ifdef __cplusplus  // Provide C++ Compatibility

    extern "C" {

#endif


/**
  @Summary
    This function initializes Comparator instance : 4

  @Description
    This routine initializes the Comparator driver instance for : 4.
    It also initializes any internal data structures.

  @Param
    None.

  @Returns 
    None
 
  @Comment
    
*/
void CMP4_Initialize(void);

/**
  @Summary
    This function returns the output of the DAC

  @Description
    This routine returns the output of the DAC. It returns a bool which 
    is the output of the DAC.

  @Param
    None.

  @Returns 
    None
 
*/
bool CMP4_DACOutputStatusGet(void);

        
/**
  @Summary
    This function returns the current output state of the Comparator

  @Description
    This routine returns true if the comparator output status is high. It 
    returns false if the comparator output status is low.

  @Param
    None.

  @Returns 
    bool.
 
*/
bool CMP4_StatusGet(void);

/**
  @Summary
    This function enables or disables the Comparator module

  @Description
    This routine enables the Comparator if "enable" is true and disables it
    if "enable" is false.

  @Param
    bool.

  @Returns 
    none.
 
*/
void CMP4_Enable(bool enable);
    
/**
  @Summary
    This function sets the Non Inverting input to the Comparator

  @Description
    This routine accepts a Input source number (CMPxA to CMPxD or PGAx)
    and sets the same in the CMPCON register.

  @Param
    Input Source channel.

  @Returns 
    none.
 
*/
void CMP4_SetInputSource(CMP4_INPUT inpSrc);

/**
  @Summary
    This function defines the a Task which can be used by the application in
    the polling mode.

  @Description
    The Task function can be called in the main application using the High Speed
    Comparator, when interrupts are not used.  This would thus introduce the 
    polling mode feature of the Analog Comparator.

  @Param
    None.

  @Returns 
    none.
 
*/
void CMP4_Tasks(void);

#endif //_CMP4_H

/**
  End of File
*/
