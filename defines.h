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
#include "user.h"
#include "voltage_dcdt.h"


#define ON 1
#define OFF 0

#define ENABLED 1
#define DISABLED 0

#define ACTIVE 1
#define INACTIVE 0

#define IO_DRV LATBbits.LATB3     // Debug I/O TP2
#define ZCDL LATCbits.LATC1     // RP49 to IC3
#define FAULT_LED_PORT  LATCbits.LATC14
#define RELAY_DRV LATCbits.LATC0

// Debug LED Use Case
#define NONE            0
#define ZEROCROSS       1
#define FAULTMODE       2
#define VOLTAGELOOP     3
#define AN1ISR          4  
#define AN2ISR          5  
#define FOURTH_AN2ISR   6  
#define IC1ISR          7  
#define IC3ISR          8  
#define CMP1ISR         9  
#define CMP2ISR         10  
#define SSMODE          11
#define PWMDEBUG        12
#define LEDTMR2         13
#define TIMERBLINK      14
#define TEMP            15

#define DEBUG_IO  NONE     // Set what LED purpose is

// DACOUT use case
#define VLOOPOUT            1
#define VACFILTERED         2
#define VOUTFILTERED        3
#define VOLTERROR           4
#define VOLTREF             5
#define DUTY                6
#define I1FILTERED          7
#define I2FILTERED          8
#define TEMPFILTERED        9

#define DACOUTPIN NONE          // Disable for using CMP2 for current limit
//#define DACOUTPIN DUTY            // Set what DACOUT signal should be
                                    // DISABLED will remove DACOUT drive

#define FCY 70000000UL              // Define instruction rate for delay routine
#define UART1_BAUD 57600            // UART Baud Rate

#define F_ACLK  (unsigned long)(7372800UL * 128U)             // 943,718,400 ( = 8 x Auxiliary Clock Frequency)
#define T_ACLK	((float)(1/(float)(F_ACLK/PWMCLKDIVBY))) 

/* Define Master and Slave PWMs */
/* Changes here should match settings in pwm.c file */
#define MASTERPWM 2
#define SLAVEPWM 1
#define SYNCPWMA 3
#define SYNCPWMB 5

// PWM Period Calculation
#define PFCPERIOD  ((unsigned int)(((float)((1/(float)(CONVSWITCHFREQ))/T_ACLK)-9)))

#define ADCRESOLUTION   1241U      // 4095/3.3V
#define DACRESOLUTION   1241U

//  Feedback Gains  // 
#define VACFBGAIN       0.0681114     // 2.2k/(3*10k+2.2K+100), Vbase = 48.5V
#define VBULKFBGAIN     0.068004      // 2.2k/(3*10k+2.2k+51+100)
#define CURRENTFBGAIN   0.6           // 1/100 * 60

#define PFCVOLTAGEREFADC    (unsigned int)(VBULKFBGAIN*ADCRESOLUTION*PFCVOLTAGEREF)

#define VPK2VRMS        46341       // 2^16 * 1/sqrt(2)
#define VRMS2VPK        23170       // 2^14 * sqrt(2) - For DCINPUTVOLTAGE == ENABLED
#define BULKLIMTVACPK   6554        // 2^16 * 0.1 - 10% of Vpk

#define VOLTDERATED 18
#define VOLTDERATINGLIMIT   (unsigned int)(VACFBGAIN*ADCRESOLUTION*VOLTDERATED)
#define VOLTDERATEDHYST 21
#define VOLTDERATINGHYSTLIMIT   (unsigned int)(VACFBGAIN*ADCRESOLUTION*VOLTDERATEDHYST)
#define DRTCONST    22118 // (40-32)/(18-12)>> 14

/* Input Voltage Parameters */
#define VDIODE 1   //Peak Value
#define INPUTUNDERVOLTAGEADC (unsigned int)((INPUTUNDERVOLTAGE)*VACFBGAIN*ADCRESOLUTION)
#define INPUTOVERVOLTAGEADC  (unsigned int)((INPUTOVERVOLTAGE)*VACFBGAIN*ADCRESOLUTION)
#define INPUTVOLTAGEHYSTADC  (unsigned int)(INPUTVOLTAGEHYST*VACFBGAIN*ADCRESOLUTION)
#define DIODEOFFSETADC (unsigned int) (VDIODE*VACFBGAIN*ADCRESOLUTION)

/* Output Voltage Parameters */
#define PFCVOUTMAX          (PFCVOLTAGEREF + PFCVOUTLIMIT)
#define PFCVOUTMAXADC       (unsigned int)(VBULKFBGAIN*ADCRESOLUTION*PFCVOUTMAX)
#define PFCVOUTMIN          (PFCVOLTAGEREF - PFCVOUTLIMIT)
#define PFCVOUTMINADC       (unsigned int)(VBULKFBGAIN*ADCRESOLUTION*PFCVOUTMIN)
#define PFCVOUTMAXHYSTADC   (unsigned int)(VBULKFBGAIN*ADCRESOLUTION*PFCVOUTHYST)
#define PFCVOUTMINHYSTADC   (unsigned int)(VBULKFBGAIN*ADCRESOLUTION*PFCVOUTHYST)

/* Input Frequency Parameters */
#define LOFREQCOUNT (unsigned int)(((float) (FCY/(float)(LOFREQ*64)))-500)
#define HIFREQCOUNT (unsigned int) (((float) FCY/(float)(HIFREQ*64))-300)
#define FREQHYSTCOUNTH 500  //60Hz
#define FREQHYSTCOUNTL 900  //47Hz

/* Input current changes with input voltage at fixed maximum output load */
/* Input Current not allowed to increase beyond CURRENTRMSMAX */
#define CURRENTRMSMAXADC ((unsigned int) (CURRENTFBGAIN*ADCRESOLUTION*CURRENTRMSMAX))
#define CURRENTPEAKMAX CURRENTRMSMAX*1.4142 //Input Current Peak and individual inductor current peaks are same
#define CURRENTLIMITMAX ((unsigned int) (CURRENTFBGAIN*ADCRESOLUTION*CURRENTPEAKMAX) *1.2/2) //10% Margin, divided between stages


#define INPUTMAXIMUMPOWER 14500  //(61W@0.85)/(Vrms*(I1+I2)/2))>>9; (Pin*0.068*1241*0.6*1241*2)>>9
#define INPUTPOWERHYST 1000

#define BOARDMAXIMUMTEMP (unsigned int) ((((float) (TEMPERATUREMAX) * 0.01) + (float) (0.5))*1241)    //1365  //60C - (0.5+10m*60)*1241
#define BOARDTEMPHYST 124   //50C


// PFC Soft Start Parameters
#define SOFTSTARTCOUNT  (unsigned int)(SOFTSTARTTIME/.1667)       // softstart time (ms) / T3ISR (ms) found in init.h
#define SOFTSTARTSCALER (unsigned int) ((32767/SOFTSTARTCOUNT)<<10)

#define GATE_DRVTEST                DISABLED         // AC input power should not be applied for this test

#define CURR_COMPENSATOR            ENABLED          // Removes current loop and PDC update, useful for testing other function w/o PFC running
 
#define CURR_RESET_MODE ENABLED

#define PROPDELAY 30    //Propogation Delay

#define FREQCONST   55221   // ((7372800*128/4)/(70000000))<<14

/* Constant Crossover Feature */
#define MAXINPUTVOLTAGEADCSQUARED 5191452UL     //(27*0.068*1241)^2
//#define MAXINPUTVOLTAGEADCSQUARED 2848533UL     //(20*0.068*1241)^2

/* Reduction of Crossover Distortion*/
#define VTHRESH 10U
#define VTHRESHADC (unsigned int)(VBULKFBGAIN*ADCRESOLUTION*VTHRESH)
#define VTHRESHINC 100U

/* Phase Management for Better Efficiency at Lighter Loads */
#define PHASE_MGMT_THRESHOLD 300U    //500 for 0.55A at 26V
#define PHASE_MGMT_HYST 30U

/* Fault Enable */
#define INPUTCURRENTFAULT ENABLED
#define OUTPUTVOLTAGEFAULT ENABLED
#define INPUTVOLTAGEFAULT ENABLED
#define INPUTPOWERFAULT ENABLED
#define FREQUENCYFAULT ENABLED
#define TEMPFAULT ENABLED

#define POWERDERATING_CURRENT ENABLED
#define POWERDERATING_REFVOLTAGE DISABLED

#define UART_DEBUG ENABLED


