/**
  Generated Main Source File

  Company:
    Microchip Technology Inc.

  File Name:
    main.c

  Summary:
    This is the main file generated using PIC24 / dsPIC33 / PIC32MM MCUs

  Description:
    This header file provides implementations for driver APIs for all modules selected in the GUI.
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
#include "main.h"
#include <stdio.h>
#include <libpic30.h>

uint16_t faultLEDCode[15] = {0x000, 0x111, 0x112, 0x113, 0x114,0x121, 0x122, 0x123, 0x124, 0x131, 0x132, 0x133, 0x134};
uint16_t faultElapsedCount = 0;
uint16_t test, timer_count;
uint16_t displayVoltage, displayFreq, displayCurrent, displayTemp;
uint16_t master = MASTERPWM, slave = SLAVEPWM;

static uint16_t sftStrtInitVoltage = 0, sftStrtVoltageDelta = 0;

long freqConstant = 1093750, freqSwConstant = 236016;
long genConstant;

extern uint16_t fltTimer;
extern uint16_t faultState;
extern uint16_t peakDetect;
extern uint16_t vacRMS,icValue,pfcBulkVoltageFiltered,tempInputFiltered,currentCH1Filtered,currentCH2Filtered;
extern int16_t sftStrtScaler;


extern volatile uint16_t sftStartTimer,timerCount;
extern volatile int16_t pfcVoltCompOutput;
extern volatile int16_t displayCount;
extern volatile int16_t  pfcVoltageRef;
extern volatile uint8_t displayValue;

extern uint16_t rmsCurrent;

extern PFC_FAULTS pfcFaultFlags;

volatile FLT_CODE faultCode = NON;   //Default 

struct uartTxData uartTxBytes;
STATE_CODE state = INIT, nextState = IDLE;

#define LEDTGLCNTMAX 500000L
volatile uint32_t ledtglcnt = 0;

#define FLTMSGCNTMAX 500000L
volatile uint32_t fltmsgcnt = 0;
volatile bool flt_send_msg = false;

/*
                         Main application
 */
int main(void)
{
     
    _TRISC14 = 0;
    _LATC14 = 0;
    
    while(1)
    {   
    /* State Machine applicable for most Power Topologies 
     * Contains 5 States: Init, Idle, SoftStart, Normal, Fault
     * User can add any number of intermediate states 
     */    
        
        switch(state) 
        {
            
           /* Initial State: 
            * Purpose: Initializes all peripherals 
            * Next State: Idle
            */
            case INIT:
                /* Initialize the device */
                SYSTEM_Initialize();
                TMR3_Stop();
                INTERRUPT_GlobalEnable();
                PWM_ModuleEnable();
                pfcFaultFlags.systemFaults = IUV | IUF;  //Start in IUV and Fault condition
                state = IDLE;   //Next state
            break;
 
           /* Idle State: 
            * Purpose: Monitors power supply inputs, goes to next state if 
            *          all inputs are within limits. 
            * Next State: SoftStart
            * PWM: Override enabled, PDC set to value higher than trigger
            */
            case IDLE:
                
                /* Wait for input faults to clear */                
                if(pfcFaultFlags.systemFaults != INACTIVE)
                {
                    state = IDLE;   //Next state
                   
                }    
                else
                {
                    __delay_ms(40);             // AC Input settling time (2 AC cycles)
                    RELAY_DRV = ENABLED;
                    faultState = 0;
                    faultElapsedCount = 0;
                    
                    /*Calculate softstart reference voltage increment*/
                    sftStrtInitVoltage = pfcBulkVoltageFiltered;
                    sftStrtVoltageDelta = PFCVOLTAGEREFADC - pfcBulkVoltageFiltered;
                    
                    /*Set PWM dutycycle to 0 at start*/
                    PWM_DutyCycleSet(1,0);
                    PWM_DutyCycleSet(2,0);

                    /* Disable PWM override */
                    PWM_OverrideHighDisable(1);
                    PWM_OverrideHighDisable(2);
                   
                    /*Initialize compensator*/
                    initPFCComp();
                    
                    /*Wait till AC input zero cross*/
                    while(peakDetect != ACTIVE) //Wait for ZeroCross (Needs a clean input voltage signal at No-load)
                    
                    /* Reset Fault LED */
                    FAULT_ResetLEDStates();
                    FAULT_SetLEDStates(faultLEDCode[0]);
                    
                    /*Start Timer3 for compensator interrupt*/
                    TMR3_Start();
                    
                    sftStartTimer = 0;
                    pfcVoltageRef = 0;
                    
                    state = SOFTSTART;   //Next state
                    
                }

                // Timeout to be added in case of faults                   
            break;

           /* SoftStart State: 
            * Purpose: Output voltage reference is increased from zero to final 
            *          value in incremental steps based on softstart time
            * Next State: Normal
            * PWM: Override disabled
            */
            case SOFTSTART:
                
                /* sftStartTimer is incremented in Timer Interrupt */
                if(sftStartTimer < SOFTSTARTCOUNT)
                {
                    /*Calculate reference voltage*/
                    sftStrtScaler = (__builtin_mulss(SOFTSTARTSCALER ,sftStartTimer) >> 10);
                    pfcVoltageRef = sftStrtInitVoltage + (__builtin_mulss(sftStrtVoltageDelta, sftStrtScaler) >> 15);
                    
                    /*Check for faults*/
                    if(pfcFaultFlags.systemFaults != INACTIVE)
                    {
                        state = FAULT;   //Next state
                    }
                }
                
                /*Softstart complete, set final reference voltage*/
                else
                {
                    pfcVoltageRef = PFCVOLTAGEREFADC;
                    sftStartTimer = 0;  //Reset
                    state = NORMAL;  //Next State
                    printf("\033[2J");      //Clear screen
                }

                // Timeout to be added in case of faults 

            break;

           /* Normal State: 
            * Purpose: Power transfer mode, UART communication 
            * Next State: Fault
            * PWM: Enabled
            */
            case NORMAL:

                #if(UART_DEBUG == ENABLED)    
                /* Display Values at Vin peak every 80ms on RealTerm */ 
                   
                if((peakDetect == INACTIVE) && (timerCount & 4))
                {
                    timerCount = 0;
                                        
                    switch(displayValue)
                    {   
                        case 0:
                                //printf("\033[2J");      //Clear screen
                                printf("\033[0;0f");    //return cursor to 0,0
                                printf("\033[?25l");    //disable cursor
                                //printf("Start Value = 0x%x \n",0xAA);
                                printf("*** LVPFC UART Interface ***\n");
                                printf("\n");
                                displayValue = 1; //Test Current values mA
                            break;

                        case 1:
                                displayVoltage = (__builtin_muluu(vacRMS,390))>>15;           //390 = 2^15/(0.068*1241)
                                printf("Measured input voltage = %dV \n",displayVoltage);
                                displayValue = 2;
                            break;

                        case 2:
                                genConstant = 1093750;
                                displayFreq = __builtin_divud(genConstant,icValue);
                                printf("Measured input frequency = %dHz \n",displayFreq);
                                displayValue = 3;
                            break;

                        case 3:
                                displayVoltage = (__builtin_muluu(pfcBulkVoltageFiltered,390))>>15;           //390 = 2^15/(0.068*1241)
                                printf("Measured output voltage = %dV \n",displayVoltage);
                                displayValue = 4;
                            break;

                        case 4:
                                printf("Measured RMS Current = %dmA \n", rmsCurrent);
                                displayValue = 5;
                            break;
                                
                        case 5:
                                displayCurrent = (__builtin_muluu(currentCH1Filtered,44000))>>15;             //32767*1000/(0.6*1241)
                                printf("Measured CH1 current = %dmA \n",displayCurrent);
                                displayValue = 6;
                            break;

                        case 6:
                                displayCurrent = (__builtin_muluu(currentCH2Filtered,44000))>>15;             //32767*1000/(0.6*1241)
                                printf("Measured CH2 current = %dmA \n",displayCurrent);
                                displayValue = 7;
                            break;

                        case 7:
                                displayTemp = (__builtin_muluu((tempInputFiltered-620),2640))>>15;    //32767* (T-0.5)/(1241*10m))                            
                                printf("Measured board temperature = %dC \n",displayTemp);
                                displayValue = 8;
                            break;

                        default:
                            {
                               //printf("End Value = 0x%x \n",0x0d);
                               displayValue = 0;
                            }

                            break;
                    }
                }
            #endif   

                /*Check for faults*/
                if(pfcFaultFlags.systemFaults != INACTIVE)
                {
                    state = FAULT;   //Next state
                    
                }

            break;

           /* Fault State: 
            * Purpose:  
            * Next State: Idle or shutdown depending on fault type
            * PWM: Disabled
            */
            case FAULT:
                
                if(pfcFaultFlags.systemFaults != INACTIVE)
                {
                    if(!faultElapsedCount) //Latch first fault
                        faultCode = faultState;
                    
                    /*PWM Override until fault clears */
                    PWM_OverrideHighEnable(1);
                    PWM_OverrideHighEnable(2);
                    TMR3_Stop();
                    FAULT_SetLEDStates(faultLEDCode[faultCode]);
                    RELAY_DRV = DISABLED;
                    flt_send_msg = false;
                    
                    if(++fltmsgcnt > FLTMSGCNTMAX)
                    {
                        fltmsgcnt = 0;
                        flt_send_msg = true;
                    }
                    
                    if(flt_send_msg){
                        printf("\n");
                        printf("---------------------------------\n");
                    }
                    /*Check for specific fault and action */
                    if(faultCode == IOC) //Current Fault,Needs Power Cycling
                    {
                        if(flt_send_msg){
                            printf("Input Over Current Fault\n");
                            printf("Turn OFF power and turn ON again\n");
                        }
                    }
                    else if(faultCode == IUV) //Input Under Voltage
                    {
                        if(flt_send_msg){
                            printf("Input Under Voltage Fault\n");
                        }
                        if(faultElapsedCount > 10)   //10s
                        {
                            pfcFaultFlags.systemFaults = INACTIVE;  // Clear Faults
                            nextState = IDLE; //Soft Restart
                        }
                    } 
                    else if(faultCode == IOV) //Input Over Voltage
                    {
                        if(flt_send_msg){
                            printf("Input Over Voltage Fault\n");
                        }
                        if(faultElapsedCount > 10)   //10s
                        {
                            pfcFaultFlags.systemFaults = INACTIVE;  //Clear Faults
                            nextState = IDLE; //Soft Restart
                        }
                    } 
                    else if(faultCode == OUV) //Output Under Voltage
                    {
                        if(flt_send_msg){
                            printf("Output Under Voltage Fault\n");
                        }
                        
                        if(faultElapsedCount > 100)   //100s
                        {
                            INTERRUPT_GlobalDisable();  //Disable fault checking and display
                            pfcFaultFlags.systemFaults = INACTIVE;  //Clear Faults
                            nextState = INIT; //Hard Restart
                        }
                    } 
                    else if(faultCode == OOV) //Output Over Voltage
                    {
                        if(flt_send_msg){
                            printf("Output Over Voltage Fault\n");
                        }
                        
                        if(faultElapsedCount > 100)   //100s
                        {
                            INTERRUPT_GlobalDisable();  //Disable fault checking and display
                            pfcFaultFlags.systemFaults = INACTIVE;  //Clear Faults
                            nextState = INIT; //Hard Restart
                        }    
                    } 
                    else if(faultCode == IUF) //Input Under Frequency
                    {
                        if(flt_send_msg){
                            printf("Input Under Frequency Fault\n");
                        }
                        
                        if(faultElapsedCount > 10)   //10s
                        {
                            pfcFaultFlags.systemFaults = INACTIVE;  //Clear Faults
                            nextState = IDLE; //Soft Restart
                        }
                    }
                    else if(faultCode == IOF) //Input Over Frequency
                    {
                        if(flt_send_msg){
                            printf("Input Over Frequency Fault\n");
                        }
                        
                        if(faultElapsedCount > 100)   //100s
                        {
                            pfcFaultFlags.systemFaults = INACTIVE;  //Clear Faults
                            nextState = IDLE; //Soft Restart
                        }
                    }
                    else if(faultCode == IOP) //Input Over Power, Needs Power Cycling
                    {
                        if(flt_send_msg){
                            printf("Input Over Power Fault\n");
                        }
                    } 
                    else if(faultCode == IOT)  //Over Temperature
                    {
                        if(flt_send_msg){
                            printf("Board Over Temperature Fault\n");
                        }
                        
                        if(faultElapsedCount > 1000)   //1000s
                        {
                            pfcFaultFlags.systemFaults = INACTIVE;  //Clear Faults
                            nextState = IDLE; //Soft Restart
                        }
                    } 
                    else if(faultCode == APS)  //Aux Power Supply, Needs Power Cycling
                    {
                        if(flt_send_msg){
                            printf("Aux Power Supply Fault\n");   
                            printf("Turn OFF power and turn ON again\n");
                        }
                    } 
                    else if(faultCode == URT) //UART
                    {
                        if(flt_send_msg){
                            printf("UART Comm. Fault\n");
                        }
                        nextState = INIT; //Hard Restart
                    }    
                }
                else  //Fault Recovery
                {
                    state = nextState; 
                    pfcFaultFlags.systemFaults = IUV | IUF;  //Start in IUV and Fault condition
                }
            break;

            default:
            /* Should never execute under normal operating conditions */

                state = INIT;  //Default

            break;

        }//end of switch  
        
        if(++ledtglcnt > LEDTGLCNTMAX)
        { 
            ledtglcnt = 0;
            _LATC14 ^= 1; 
        }
        
        
    } //end of while

        return 0;
} //end of main


/*******************************************************************************
 * Function: 	swapMasterSlavePWMChannels
 * Description:	Swaps Master and Slave PWM Channels
 *              Channels have to be configured first using MCC, and this
 *              function can be used to swap the Master and Slave channels
 *              Ensure setting #pragma config IOL1WAY = OFF
*******************************************************************************/
void swapMasterSlavePWMChannels(void)
{
    if( master == MASTERPWM)
    {
        //FLTPOL disabled; CLPOL disabled; CLSRC CMP3; CLMOD disabled; FLTMOD Fault input is disabled; IFLTMOD disabled; FLTSRC FLT1; 
        __builtin_write_PWMSFR(&FCLCON1, 0x3C0B, &PWMKEY);
        //FLTPOL disabled; CLPOL enabled; CLSRC FLT6; CLMOD disabled; FLTMOD Fault input is disabled; IFLTMOD disabled; FLTSRC FLT1; 
        __builtin_write_PWMSFR(&FCLCON2, 0x1A0B, &PWMKEY);
        //FLTPOL disabled; CLPOL disabled; CLSRC CMP3; CLMOD disabled; FLTMOD Fault input is disabled; IFLTMOD disabled; FLTSRC FLT8; 
        __builtin_write_PWMSFR(&FCLCON3, 0x3C43, &PWMKEY);
        //FLTPOL disabled; CLPOL disabled; CLSRC CMP4; CLMOD disabled; FLTMOD Fault input is disabled; IFLTMOD disabled; FLTSRC FLT1; 
        __builtin_write_PWMSFR(&FCLCON5, 0x400B, &PWMKEY);
        
        // Setup AN Trigger Source
        ADTRIG0Lbits.TRGSRC0 = 6;       // AN0 triggered by PWM2  
        ADTRIG0Lbits.TRGSRC1 = 5;       // AN1 triggered by PWM1  
        ADTRIG0Hbits.TRGSRC2 = 5;       // AN2 triggered by PWM1  
        ADTRIG0Hbits.TRGSRC3 = 5;       // AN3 triggered by PWM1  
        
        __builtin_write_OSCCONL(OSCCON & 0xbf); // unlock PPS
        RPINR7bits.IC1R = 0x0014;   //RA4->IC1:IC1;
        __builtin_write_OSCCONL(OSCCON | 0x40); // lock   PPS
        
        master = SLAVEPWM;
        slave = MASTERPWM;
    
    }
    else if(master == SLAVEPWM)
    {
        //FLTPOL disabled; CLPOL enabled; CLSRC FLT6; CLMOD disabled; FLTMOD Fault input is disabled; IFLTMOD disabled; FLTSRC FLT1; 
        __builtin_write_PWMSFR(&FCLCON1, 0x1A0B, &PWMKEY);
        //FLTPOL disabled; CLPOL disabled; CLSRC CMP4; CLMOD disabled; FLTMOD Fault input is disabled; IFLTMOD disabled; FLTSRC FLT1; 
        __builtin_write_PWMSFR(&FCLCON2, 0x400B, &PWMKEY);
        //FLTPOL disabled; CLPOL disabled; CLSRC CMP4; CLMOD disabled; FLTMOD Fault input is disabled; IFLTMOD disabled; FLTSRC FLT8; 
        __builtin_write_PWMSFR(&FCLCON3, 0x4043, &PWMKEY);
        //FLTPOL disabled; CLPOL disabled; CLSRC CMP3; CLMOD disabled; FLTMOD Fault input is disabled; IFLTMOD disabled; FLTSRC FLT1; 
        __builtin_write_PWMSFR(&FCLCON5, 0x3C0B, &PWMKEY);
    
        // Setup AN Trigger Source
        ADTRIG0Lbits.TRGSRC0 = 5;       // AN0 triggered by PWM1  
        ADTRIG0Lbits.TRGSRC1 = 6;       // AN1 triggered by PWM2  
        ADTRIG0Hbits.TRGSRC2 = 6;       // AN2 triggered by PWM2  
        ADTRIG0Hbits.TRGSRC3 = 6;       // AN3 triggered by PWM2  
    
        __builtin_write_OSCCONL(OSCCON & 0xbf); // unlock PPS
        RPINR7bits.IC1R = 0x002D;   //RB13->IC1:IC1;
        __builtin_write_OSCCONL(OSCCON | 0x40); // lock   PPS

        master = MASTERPWM;
        slave = SLAVEPWM;
    }
}

// End of File
