
/**
  CMP4 Generated Driver File 

  @Company
    Microchip Technology Inc.

  @File Name
    cmp4.c

  @Summary
    This is the generated driver implementation file for the CMP4 driver using PIC24 / dsPIC33 / PIC32MM MCUs

  @Description
    This header file provides implementations for driver APIs for CMP4. 
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
#include "cmp4.h"
/**
  Section: Driver Interface
*/

void CMP4_Initialize(void)
{           

    // Disable the CMP module before the initialization
    CMP4_Enable(false);



    // DACOE disabled; HYSPOL Rising Edge; ALTINP Comparator; FLTREN disabled; FCLKSEL System Clock; CMPSTAT disabled; EXTREF AVDD; CMPPOL Inverted; CMPSIDL disabled; CMPON enabled; HYSSEL No hysteresis; INSEL CMP4D; RANGE AVDD is the maximum DAC output voltage; 
    CMP4CON = 0x80C3;

    // CMREF 10; 
    CMP4DAC = 0xA;
    

    CMP4_Enable(true);

}

void CMP4_Enable(bool enable)
{
    (CMP4CONbits.CMPON) = enable;
}

void CMP4_SetInputSource(CMP4_INPUT inpSrc)
{
    (CMP4CONbits.INSEL) = inpSrc;
}

bool CMP4_DACOutputStatusGet(void)
{
    return (CMP4CONbits.DACOE);
}

bool CMP4_StatusGet(void)
{
    return (CMP4CONbits.CMPSTAT);
}

void CMP4_Tasks(void)
{
    // clear the CMP4 interrupt flag
    IFS6bits.AC4IF = 0;
}


/**
  End of File
*/
