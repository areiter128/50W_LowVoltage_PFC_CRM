
/**
  ADC1 Generated Driver File

  @Company
    Microchip Technology Inc.

  @File Name
    adc1.c

  @Summary
    This is the generated header file for the ADC1 driver using PIC24 / dsPIC33 / PIC32MM MCUs

  @Description
    This header file provides APIs for driver for ADC1.
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
#include "adc1.h"

/**
  Section: Driver Interface
*/


void ADC1_Initialize (void)
{
    // ADSIDL disabled; PUMPEN disabled; ADON enabled; 
    ADCON1L = (0x8000 & 0x7FFF); //Disabling ADON bit
    // FORM Integer; SHRRES 12-bit resolution; 
    ADCON1H = 0x60;
    // SHRADCS 2; REFCIE disabled; SHREISEL Early interrupt is generated 1 TADCORE clock prior to data being ready; REFERCIE disabled; EIEN disabled; 
    ADCON2L = 0x0;
    // SHRSAMC 0; 
    ADCON2H = 0x0;
    // SWCTRG disabled; SHRSAMP disabled; SUSPEND disabled; SWLCTRG disabled; SUSPCIE disabled; CNVCHSEL AN0; REFSEL disabled; 
    ADCON3L = 0x0;
    // SHREN enabled; C3EN enabled; C2EN enabled; C1EN enabled; C0EN enabled; CLKDIV 1; CLKSEL FOSC; 
    ADCON3H = (0x408F & 0xFF00); //Disabling C0EN, C1EN, C2EN, C3EN and SHREN bits
    // SAMC3EN disabled; SYNCTRG3 disabled; SAMC0EN disabled; SYNCTRG2 disabled; SAMC1EN disabled; SAMC2EN disabled; SYNCTRG1 disabled; SYNCTRG0 disabled; 
    ADCON4L = 0x0;
    // C3CHS AN3; C0CHS AN0; C1CHS AN1; C2CHS AN2; 
    ADCON4H = 0x0;
    // SIGN0 disabled; SIGN4 disabled; SIGN3 disabled; SIGN2 disabled; SIGN1 disabled; SIGN7 disabled; SIGN6 disabled; DIFF0 disabled; SIGN5 disabled; DIFF1 disabled; DIFF2 disabled; DIFF3 disabled; DIFF4 disabled; DIFF5 disabled; DIFF6 disabled; DIFF7 disabled; 
    ADMOD0L = 0x0;
    // DIFF15 disabled; DIFF14 disabled; SIGN8 disabled; DIFF13 disabled; SIGN14 disabled; DIFF12 disabled; SIGN15 disabled; DIFF11 disabled; DIFF10 disabled; SIGN9 disabled; DIFF8 disabled; DIFF9 disabled; SIGN10 disabled; SIGN11 disabled; SIGN12 disabled; SIGN13 disabled; 
    ADMOD0H = 0x0;
    // DIFF19 disabled; DIFF18 disabled; SIGN20 disabled; DIFF17 disabled; SIGN21 disabled; DIFF16 disabled; SIGN16 disabled; SIGN17 disabled; DIFF21 disabled; SIGN18 disabled; DIFF20 disabled; SIGN19 disabled; 
    ADMOD1L = 0x0;
    // IE15 disabled; IE1 disabled; IE0 disabled; IE3 disabled; IE2 enabled; IE5 disabled; IE4 disabled; IE10 disabled; IE7 disabled; IE6 disabled; IE9 disabled; IE13 disabled; IE8 disabled; IE14 disabled; IE11 enabled; IE12 disabled; 
    ADIEL = 0x804;
    // IE17 disabled; IE18 disabled; IE16 disabled; IE19 disabled; IE20 disabled; IE21 disabled; 
    ADIEH = 0x0;
    // CMPEN10 disabled; CMPEN11 disabled; CMPEN6 disabled; CMPEN5 disabled; CMPEN4 disabled; CMPEN3 disabled; CMPEN2 disabled; CMPEN1 disabled; CMPEN0 disabled; CMPEN14 disabled; CMPEN9 disabled; CMPEN15 disabled; CMPEN8 disabled; CMPEN12 disabled; CMPEN7 disabled; CMPEN13 disabled; 
    ADCMP0ENL = 0x0;
    // CMPEN10 disabled; CMPEN11 disabled; CMPEN6 disabled; CMPEN5 disabled; CMPEN4 disabled; CMPEN3 disabled; CMPEN2 disabled; CMPEN1 disabled; CMPEN0 disabled; CMPEN14 disabled; CMPEN9 disabled; CMPEN15 disabled; CMPEN8 disabled; CMPEN12 disabled; CMPEN7 disabled; CMPEN13 disabled; 
    ADCMP1ENL = 0x0;
    // CMPEN21 disabled; CMPEN20 disabled; CMPEN18 disabled; CMPEN19 disabled; CMPEN16 disabled; CMPEN17 disabled; 
    ADCMP0ENH = 0x0;
    // CMPEN21 disabled; CMPEN20 disabled; CMPEN18 disabled; CMPEN19 disabled; CMPEN16 disabled; CMPEN17 disabled; 
    ADCMP1ENH = 0x0;
    // CMPLO 0; 
    ADCMP0LO = 0x0;
    // CMPLO 0; 
    ADCMP1LO = 0x0;
    // CMPHI 0; 
    ADCMP0HI = 0x0;
    // CMPHI 0; 
    ADCMP1HI = 0x0;
    // OVRSAM 4x; MODE Oversampling Mode; FLCHSEL AN0; IE disabled; FLEN disabled; 
    ADFL0CON = 0x400;
    // OVRSAM 4x; MODE Oversampling Mode; FLCHSEL AN0; IE disabled; FLEN disabled; 
    ADFL1CON = 0x400;
    // HIHI disabled; LOLO disabled; HILO disabled; BTWN disabled; LOHI disabled; CMPEN disabled; IE disabled; 
    ADCMP0CON = 0x0;
    // HIHI disabled; LOLO disabled; HILO disabled; BTWN disabled; LOHI disabled; CMPEN disabled; IE disabled; 
    ADCMP1CON = 0x0;
    // CHOFFSET 0; 
    ADBASE = 0x0;
    // LVLEN9 disabled; LVLEN8 disabled; LVLEN11 disabled; LVLEN7 disabled; LVLEN10 disabled; LVLEN6 disabled; LVLEN13 disabled; LVLEN5 disabled; LVLEN12 disabled; LVLEN4 disabled; LVLEN15 disabled; LVLEN3 disabled; LVLEN14 disabled; LVLEN2 disabled; LVLEN1 disabled; LVLEN0 disabled; 
    ADLVLTRGL = 0x0;
    // LVLEN20 disabled; LVLEN21 disabled; LVLEN17 disabled; LVLEN16 disabled; LVLEN19 disabled; LVLEN18 disabled; 
    ADLVLTRGH = 0x0;
    // SAMC 0; 
    ADCORE0L = 0x0;
    // SAMC 0; 
    ADCORE1L = 0x0;
    // SAMC 0; 
    ADCORE2L = 0x0;
    // SAMC 0; 
    ADCORE3L = 0x0;
    // RES 12-bit resolution; EISEL Early interrupt is generated 1 TADCORE clock prior to data being ready; ADCS 2; 
    ADCORE0H = 0x300;
    // RES 12-bit resolution; EISEL Early interrupt is generated 1 TADCORE clock prior to data being ready; ADCS 2; 
    ADCORE1H = 0x300;
    // RES 12-bit resolution; EISEL Early interrupt is generated 1 TADCORE clock prior to data being ready; ADCS 2; 
    ADCORE2H = 0x300;
    // RES 12-bit resolution; EISEL Early interrupt is generated 1 TADCORE clock prior to data being ready; ADCS 2; 
    ADCORE3H = 0x300;
    // EIEN9 disabled; EIEN7 disabled; EIEN8 disabled; EIEN5 disabled; EIEN6 disabled; EIEN3 disabled; EIEN4 disabled; EIEN1 disabled; EIEN2 disabled; EIEN13 disabled; EIEN0 disabled; EIEN12 disabled; EIEN11 disabled; EIEN10 disabled; EIEN15 disabled; EIEN14 disabled; 
    ADEIEL = 0x0;
    // EIEN17 disabled; EIEN16 disabled; EIEN19 disabled; EIEN18 disabled; EIEN20 disabled; EIEN21 disabled; 
    ADEIEH = 0x0;
    // C0CIE disabled; C1CIE disabled; C2CIE disabled; C3CIE disabled; SHRCIE disabled; WARMTIME 2048 Source Clock Periods; 
    ADCON5H = (0x1100 & 0xF0FF); //Disabling WARMTIME bit
    
    // Clearing ADCAN11 interrupt flag.
    IFS9bits.ADCAN11IF = 0;
    // Enabling ADCAN11 interrupt.
    IEC9bits.ADCAN11IE = 1;
    // Clearing ADCAN2 interrupt flag.
    IFS7bits.ADCAN2IF = 0;
    // Enabling ADCAN2 interrupt.
    IEC7bits.ADCAN2IE = 1;

    // Setting WARMTIME bit
    ADCON5Hbits.WARMTIME = 0xF;
    // Enabling ADC Module
    ADCON1Lbits.ADON = 0x1;
    // Enabling Power for the Shared Core
    ADC1_SharedCorePowerEnable();
    // Calibrating the Shared Core
    ADC1_SharedCoreCalibration();
    // Enabling Power for Core3
    ADC1_Core3PowerEnable();
    // Calibrating Core3
    ADC1_Core3Calibration();
    // Enabling Power for Core2
    ADC1_Core2PowerEnable();
    // Calibrating Core2
    ADC1_Core2Calibration();
    // Enabling Power for Core1
    ADC1_Core1PowerEnable();
    // Calibrating Core1
    ADC1_Core1Calibration();
    // Enabling Power for Core0
    ADC1_Core0PowerEnable();
    // Calibrating Core0
    ADC1_Core0Calibration();

    //TRGSRC0 PWM1 Primary Trigger; TRGSRC1 PWM2 Primary Trigger; 
    ADTRIG0L = 0x605;
    //TRGSRC3 PWM2 Primary Trigger; TRGSRC2 PWM2 Primary Trigger; 
    ADTRIG0H = 0x606;
    //TRGSRC4 None; TRGSRC5 None; 
    ADTRIG1L = 0x0;
    //TRGSRC6 None; TRGSRC7 None; 
    ADTRIG1H = 0x0;
    //TRGSRC8 None; TRGSRC9 None; 
    ADTRIG2L = 0x0;
    //TRGSRC11 TMR1; TRGSRC10 None; 
    ADTRIG2H = 0xC00;
    //TRGSRC13 None; TRGSRC12 None; 
    ADTRIG3L = 0x0;
    //TRGSRC15 None; TRGSRC14 None; 
    ADTRIG3H = 0x0;
    //TRGSRC17 None; TRGSRC16 None; 
    ADTRIG4L = 0x0;
    //TRGSRC19 None; TRGSRC18 None; 
    ADTRIG4H = 0x0;
    //TRGSRC20 None; TRGSRC21 None; 
    ADTRIG5L = 0x0;
}
void ADC1_Core0PowerEnable ( ) 
{
    ADCON5Lbits.C0PWR = 1; 
    while(ADCON5Lbits.C0RDY == 0);
    ADCON3Hbits.C0EN = 1;     
}

void ADC1_Core0Calibration ( ) 
{   
    ADCAL0Lbits.CAL0EN = 1;   
    ADCAL0Lbits.CAL0DIFF = 0;
    ADCAL0Lbits.CAL0RUN = 1;   
    while(ADCAL0Lbits.CAL0RDY == 0);
    ADCAL0Lbits.CAL0EN = 0;
}
void ADC1_Core1PowerEnable ( ) 
{
    ADCON5Lbits.C1PWR = 1; 
    while(ADCON5Lbits.C1RDY == 0);
    ADCON3Hbits.C1EN = 1;     
}

void ADC1_Core1Calibration ( ) 
{   
    ADCAL0Lbits.CAL1EN = 1;   
    ADCAL0Lbits.CAL1DIFF = 0;
    ADCAL0Lbits.CAL1RUN = 1;   
    while(ADCAL0Lbits.CAL1RDY == 0);
    ADCAL0Lbits.CAL1EN = 0;
}
void ADC1_Core2PowerEnable ( ) 
{
    ADCON5Lbits.C2PWR = 1; 
    while(ADCON5Lbits.C2RDY == 0);
    ADCON3Hbits.C2EN = 1;     
}

void ADC1_Core2Calibration ( ) 
{   
    ADCAL0Hbits.CAL2EN = 1;   
    ADCAL0Hbits.CAL2DIFF = 0;
    ADCAL0Hbits.CAL2RUN = 1;   
    while(ADCAL0Hbits.CAL2RDY == 0);
    ADCAL0Hbits.CAL2EN = 0;
}
void ADC1_Core3PowerEnable ( ) 
{
    ADCON5Lbits.C3PWR = 1; 
    while(ADCON5Lbits.C3RDY == 0);
    ADCON3Hbits.C3EN = 1;     
}

void ADC1_Core3Calibration ( ) 
{   
    ADCAL0Hbits.CAL3EN = 1;   
    ADCAL0Hbits.CAL3DIFF = 0;
    ADCAL0Hbits.CAL3RUN = 1;   
    while(ADCAL0Hbits.CAL3RDY == 0);
    ADCAL0Hbits.CAL3EN = 0;
}
void ADC1_SharedCorePowerEnable ( ) 
{
    ADCON5Lbits.SHRPWR = 1;   
    while(ADCON5Lbits.SHRRDY == 0);
    ADCON3Hbits.SHREN = 1;   
}
void ADC1_SharedCoreCalibration ( ) 
{
    ADCAL1Hbits.CSHREN = 1;   
    ADCAL1Hbits.CSHRDIFF = 0;
    ADCAL1Hbits.CSHRRUN = 1;   
    while(ADCAL1Hbits.CSHRRDY == 0);
    ADCAL1Hbits.CSHREN = 0;   
}

void ADC1_Tasks ( void )
{
    // clear the ADC1 interrupt flag
    IFS0bits.ADCIF = 0;
}


void __attribute__ ((weak)) ADCAN11_CallBack(void)
{
    // Add your custom callback code here
}

void __attribute__ ( ( __interrupt__ , auto_psv ) ) _ADCAN11Interrupt ( void )
{
    ADCAN11_CallBack();
    //clear the ADCAN11 interrupt flag
    IFS9bits.ADCAN11IF = 0;
}

void __attribute__ ((weak)) ADCAN2_CallBack(void)
{
    // Add your custom callback code here
}

void __attribute__ ( ( __interrupt__ , auto_psv ) ) _ADCAN2Interrupt ( void )
{
    ADCAN2_CallBack();
    //clear the ADCAN2 interrupt flag
    IFS7bits.ADCAN2IF = 0;
}


/**
  End of File
*/
