
/**
  TMR4 Generated Driver API Source File 

  @Company
    Microchip Technology Inc.

  @File Name
    tmr4.c

  @Summary
    This is the generated source file for the TMR4 driver using PIC24 / dsPIC33 / PIC32MM MCUs

  @Description
    This source file provides APIs for driver for TMR4. 
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

/**
  Section: Included Files
*/

#include <xc.h>
#include "tmr4.h"

/**
  Section: Data Type Definitions
*/

/** TMR Driver Hardware Instance Object

  @Summary
    Defines the object required for the maintainence of the hardware instance.

  @Description
    This defines the object required for the maintainence of the hardware
    instance. This object exists once per hardware instance of the peripheral.

  Remarks:
    None.
*/

typedef struct _TMR_OBJ_STRUCT
{
    /* Timer Elapsed */
    bool                                                    timerElapsed;
    /*Software Counter value*/
    uint8_t                                                 count;

} TMR_OBJ;

static TMR_OBJ tmr4_obj;

/**
  Section: Driver Interface
*/


void TMR4_Initialize (void)
{
    //TMR5 0; 
    TMR5 = 0x0;
    //PR5 1040; 
    PR5 = 0x410;
    //TMR4 0; 
    TMR4 = 0x0;
    //Period = 1 s; Frequency = 68172500 Hz; PR4 15060; 
    PR4 = 0x3AD4;
    //TCKPS 1:1; T32 32 Bit; TON enabled; TSIDL disabled; TCS FOSC/2; TGATE disabled; 
    T4CON = 0x8008;

    
    IFS1bits.T5IF = false;
    IEC1bits.T5IE = true;
	
    tmr4_obj.timerElapsed = false;

}


void __attribute__ ( ( interrupt, no_auto_psv ) ) _T5Interrupt (  )
{
    /* Check if the Timer Interrupt/Status is set */

    //***User Area Begin

    // ticker function call;
    // ticker is 1 -> Callback function gets called everytime this ISR executes
    TMR4_CallBack();

    //***User Area End

    tmr4_obj.count++;
    tmr4_obj.timerElapsed = true;
    IFS1bits.T5IF = false;
}



void TMR4_Period32BitSet( uint32_t value )
{
    /* Update the counter values */
    PR4 = (value & 0x0000FFFF);
    PR5 = ((value & 0xFFFF0000)>>16);
}

uint32_t TMR4_Period32BitGet( void )
{
    uint32_t periodVal = 0xFFFFFFFF;

    /* get the timer period value and return it */
    periodVal = (((uint32_t)PR5 <<16) | PR4);

    return( periodVal );

}

void TMR4_Counter32BitSet( uint32_t value )
{
    /* Update the counter values */
   TMR5HLD = ((value & 0xFFFF0000)>>16);
   TMR4 = (value & 0x0000FFFF);

}

uint32_t TMR4_Counter32BitGet( void )
{
    uint32_t countVal = 0xFFFFFFFF;
    uint16_t countValUpper;
    uint16_t countValLower;

    countValLower = TMR4;
    countValUpper = TMR5HLD;

    /* get the current counter value and return it */
    countVal = (((uint32_t)countValUpper<<16)| countValLower );

    return( countVal );

}


void __attribute__ ((weak)) TMR4_CallBack(void)
{
    // Add your custom callback code here
}

void TMR4_Start( void )
{
    /* Reset the status information */
    tmr4_obj.timerElapsed = false;

    /*Enable the interrupt*/
    IEC1bits.T5IE = true;

    /* Start the Timer */
    T4CONbits.TON = 1;
}

void TMR4_Stop( void )
{
    /* Stop the Timer */
    T4CONbits.TON = false;

    /*Disable the interrupt*/
    IEC1bits.T5IE = false;
}

bool TMR4_GetElapsedThenClear(void)
{
    bool status;
    
    status = tmr4_obj.timerElapsed;

    if(status == true)
    {
        tmr4_obj.timerElapsed = false;
    }
    return status;
}

int TMR4_SoftwareCounterGet(void)
{
    return tmr4_obj.count;
}

void TMR4_SoftwareCounterClear(void)
{
    tmr4_obj.count = 0; 
}

/**
 End of File
*/
