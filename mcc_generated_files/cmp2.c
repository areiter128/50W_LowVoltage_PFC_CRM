
/**
  CMP2 Generated Driver File 

  @Company
    Microchip Technology Inc.

  @File Name
    cmp2.c

  @Summary
    This is the generated driver implementation file for the CMP2 driver using PIC24 / dsPIC33 / PIC32MM MCUs

  @Description
    This header file provides implementations for driver APIs for CMP2. 
    Generation Information : 
        Product Revision  :  PIC24 / dsPIC33 / PIC32MM MCUs - pic24-dspic-pic32mm : v1.35
        Device            :  dsPIC33EP64GS806
    The generated drivers are tested against the following:
        Compiler          :  XC16 1.31
        MPLAB             :  MPLAB X 3.60
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
/**
  Section: Included Files
*/
#include <xc.h>
#include "cmp2.h"
/**
  Section: Driver Interface
*/

void CMP2_Initialize(void)
{           

    // Disable the CMP module before the initialization
    CMP2_Enable(false);



    // DACOE disabled; HYSPOL Rising Edge; ALTINP Comparator; FLTREN disabled; FCLKSEL System Clock; CMPSTAT disabled; EXTREF AVDD; CMPPOL Not Inverted; CMPSIDL disabled; CMPON enabled; HYSSEL 5 mV hysteresis; INSEL CMP2A; RANGE AVDD is the maximum DAC output voltage; 
    CMP2CON = 0x8801;

    // CMREF 3300; 
    CMP2DAC = 0xCE4;
    

    CMP2_Enable(true);

}

void CMP2_Enable(bool enable)
{
    (CMP2CONbits.CMPON) = enable;
}

void CMP2_SetInputSource(CMP2_INPUT inpSrc)
{
    (CMP2CONbits.INSEL) = inpSrc;
}

bool CMP2_DACOutputStatusGet(void)
{
    return (CMP2CONbits.DACOE);
}

bool CMP2_StatusGet(void)
{
    return (CMP2CONbits.CMPSTAT);
}

void CMP2_Tasks(void)
{
    // clear the CMP2 interrupt flag
    IFS6bits.AC2IF = 0;
}


/**
  End of File
*/
