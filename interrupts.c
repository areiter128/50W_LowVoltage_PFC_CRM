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

#include "interrupts.h"
#include "dsp.h"
#include "faults.h"
#include "mcc_generated_files/pwm.h"

/*******************************************************************************
Global variables
*******************************************************************************/
uint16_t pfcCurrent = 0, currRefScaler = 0, pfcCurrCompOut = 0;
uint16_t denCurrRefCalc = 0;
uint16_t fltTimer = 0,CCMFlag = 0;
uint16_t transitionLHEnable, transitionHLEnable;
uint16_t overCurrentCycle, freqCount;
uint16_t vacRMS,pfcBulkVoltageFiltered,valueSyncPDC,currentCH1Filtered,currentCH2Filtered;
uint16_t tempInputFiltered;
uint16_t peakCurrent, rmsCurrent,computedPower;
uint16_t pfcUnderVoltageLimit = PFCVOUTMINADC;
uint16_t vacRMSSquared = 0, xoverFactor = 0;
uint16_t pfcVoltCompGain = 32767;
uint16_t temp,phaseThreshold,phaseState;
uint16_t pfcVoltCompOutputMax = VOLTAGE_COMP_2P2Z_MAX_CLAMP;
uint16_t peakDetect = 0;
uint16_t currentDeratingFlag;
uint16_t icValue = 0;
uint16_t faultState = 0;
uint16_t zeroCrossDetect;
uint16_t slavePDC, masterPDC, syncPDC;


int16_t sftStrtScaler = 0;
int16_t  pfcCurrentRef = 0;
int16_t  vacDiff,vacRef;
int16_t  pfcCurrentLimit;

extern uint16_t vacRMSSquared ;
extern uint16_t faultElapsedCount;
extern int16_t pfcCurrentPostShiftFactors[2];
extern volatile uint16_t transitionLHEndFlag,transitionHLEndFlag;
extern volatile FLT_CODE faultCode;   //Default Input Under Voltage
extern uint16_t master,slave;
extern STATE_CODE state;

volatile unsigned long outVoltage,timePeriod,icDiff;
volatile uint8_t displayValue;
volatile int16_t displayCount;
volatile uint16_t vacInputFiltered;
volatile int16_t pfcVoltCompOutput = 0;
volatile int16_t pfcVoltageRef = 0;
volatile uint16_t sftStartTimer,timerCount;

PFC_FAULTS pfcFaultFlags; //= {0,1,0,0,0,1,0,0,0,0,0,0}; //Input UV and Freq. Faults

/* Input Current */
FLT_STATE fltIOCSource = { .fltThreshold=CURRENTLIMITMAX, .fltHysLimit=CURRENTLIMITMAX, .fltThresholdCnt=100, .fltHysCnt=0, .fltCode=IOC};

/* Input Voltage */
FLT_STATE fltIUVSource = { .fltThreshold=INPUTUNDERVOLTAGEADC, .fltHysLimit=(INPUTUNDERVOLTAGEADC + INPUTVOLTAGEHYSTADC), .fltThresholdCnt=10, .fltHysCnt=5, .fltCode=IUV};
FLT_STATE fltIOVSource = { .fltThreshold=INPUTOVERVOLTAGEADC, .fltHysLimit=(INPUTOVERVOLTAGEADC - INPUTVOLTAGEHYSTADC), .fltThresholdCnt=10, .fltHysCnt=5, .fltCode=IOV};

/* Output Voltage */
FLT_STATE fltOUVSource = { .fltThreshold=PFCVOUTMINADC, .fltHysLimit=(PFCVOUTMAXADC + PFCVOUTMINHYSTADC), .fltThresholdCnt=10, .fltHysCnt=5, .fltCode=OUV};
FLT_STATE fltOOVSource = { .fltThreshold=PFCVOUTMAXADC, .fltHysLimit=(PFCVOUTMAXADC - PFCVOUTMAXHYSTADC), .fltThresholdCnt=10, .fltHysCnt=5, .fltCode=OOV};

/* Input Frequency */
FLT_STATE fltIUFSource = { .fltThreshold=LOFREQCOUNT, .fltHysLimit=(LOFREQCOUNT - FREQHYSTCOUNTL), .fltThresholdCnt=10, .fltHysCnt=5, .fltCode=IUF};
FLT_STATE fltIOFSource = { .fltThreshold=HIFREQCOUNT, .fltHysLimit=(HIFREQCOUNT + FREQHYSTCOUNTH), .fltThresholdCnt=10, .fltHysCnt=5, .fltCode=IOF};

/* Input Power */
FLT_STATE fltIOPSource = { .fltThreshold=INPUTMAXIMUMPOWER, .fltHysLimit=(INPUTMAXIMUMPOWER-INPUTPOWERHYST), .fltThresholdCnt=10, .fltHysCnt=5, .fltCode=IOP};

/* Temperature */
FLT_STATE fltIOTSource = { .fltThreshold=INPUTMAXIMUMPOWER, .fltHysLimit=(INPUTMAXIMUMPOWER-BOARDTEMPHYST), .fltThresholdCnt=10, .fltHysCnt=5, .fltCode=IOT};


/*******************************************************************************
ISR: 		 ADCAN2Interrupt
Description: Interrupt for Input/Output Voltage filtering and calculations
*******************************************************************************/
//void __attribute__((__interrupt__,no_auto_psv)) _ADCAN2Interrupt()
void ADCAN2_CallBack(void)
{
    static uint16_t isrCnt = 0;
    static uint16_t vacFilteredMax = 0, prevVacFiltered = 0;
    static uint16_t vacInputSample[4] = {0,0,0,0};
    static uint16_t pfcBulkVoltage = 0;
    static uint16_t vacPeak = 0;
    static uint8_t upCounter = 0, downCounter = 0;
    
    uint8_t vacArrayIndex = 0;
    int16_t vacInputSum = 0;
        
    #if(DEBUG_IO ==  AN2ISR)
    IO_DRV = ON;
    #endif
    
    /*Store ADC values*/
    currentCH1Filtered = ADCBUF0;
    currentCH2Filtered = ADCBUF2;
        
    #if(INPUTCURRENTFAULT)
        if(state == NORMAL)
        {
            pfcFaultFlags.inputCurrent = FAULT_CheckMax(&fltIOCSource, currentCH1Filtered, &faultState) | 
            FAULT_CheckMax(&fltIOCSource, currentCH2Filtered, &faultState); //CH1 or CH2 current fault
        }    
    #endif

    
    // Filter for Vin
    vacInputSample[0] = ADCBUF1+ DIODEOFFSETADC;
    vacInputSum = ADCBUF1+ DIODEOFFSETADC;
    
    for(vacArrayIndex = 3;vacArrayIndex >= 1;vacArrayIndex--)   
    {
        vacInputSum += vacInputSample[vacArrayIndex];
        vacInputSample[vacArrayIndex] = vacInputSample[vacArrayIndex-1];
    }

    vacInputFiltered = vacInputSum >> 2;

    pfcBulkVoltage += ADCBUF3;

#if (DACOUTPIN == VACFILTERED)
    CMP2DAC = vacInputFiltered;
    #endif
      
    #if (DACOUTPIN == I1FILTERED)
    CMP2DAC = currentCH1Filtered;
    #endif

    #if (DACOUTPIN == I2FILTERED)
    CMP2DAC = currentCH2Filtered;
    #endif

    ++isrCnt;
    if((isrCnt&3) == 0)     // Executed every fourth ISR        //Check with PWM interrupt every cycle at full load and peak
    {
        #if(DEBUG_IO == FOURTH_AN2ISR)
            IO_DRV = !IO_DRV;
        #endif
        
        // Average bulk voltage and clear accumulated value
        pfcBulkVoltageFiltered = (pfcBulkVoltage >> 2);
        pfcBulkVoltage = 0;
       
                
        #if(DACOUTPIN == VOUTFILTERED)
            CMP2DAC = pfcBulkVoltageFiltered;
        #endif
                    
        #if(OUTPUTVOLTAGEFAULT)
            /* Check for Output Over Voltage Or Under Voltage Fault */
            if(state == SOFTSTART)  //Required for fault recovery
                fltOUVSource.faultActive = 0;
            
            if(state == NORMAL)
            {
                pfcFaultFlags.outputUnderVoltage = FAULT_CheckMin(&fltOUVSource, pfcBulkVoltageFiltered, &faultState); //Under
            }

            pfcFaultFlags.outputOverVoltage = FAULT_CheckMax(&fltOOVSource, pfcBulkVoltageFiltered, &faultState); //Over
        #endif
               
        
        // Find the peak input voltage to calculate the RMS voltage
        if((vacInputFiltered > vacFilteredMax) && (peakDetect == ACTIVE))
        {
            vacFilteredMax = vacInputFiltered;
        }
        
        if(vacInputFiltered > prevVacFiltered)
        {
            upCounter++;
            downCounter = 0;

            if((upCounter >= 5) && (peakDetect == INACTIVE))
            {
                peakDetect = ACTIVE;     // Start looking for peak
                              
                /* Frequency Measurement */
                {
                    IO_DRV = !IO_DRV;     
                    ZCDL = !ZCDL;   // Output Zero cross signal on RP49 for IC3 trigger
                    
                #if(PHASE_MGMT)
                    zeroCrossDetect = 1;    //For enabling phase management
                #endif  
                    
                #if(DEBUG_IO == ZEROCROSS)
                    IO_DRV = !IO_DRV;
                #endif
                }
               
            }
        }
        else
        {
            upCounter = 0;
            downCounter++;

            //if((downCounter >= 5) && (peakDetect == ACTIVE))
            if((downCounter >= 2) && (peakDetect == ACTIVE))
            {
                peakDetect = INACTIVE;  
                //ZCDL = !ZCDL;   // Output Zero cross signal on RP49 for IC3 trigger
                               
                //vacPeak = vacFilteredMax;
                vacPeak = prevVacFiltered;
                vacRMS = (__builtin_muluu(vacPeak,VPK2VRMS) >> 16); // VacRMS 12bits
                
                // For V^2 Calculations
                vacRMSSquared = (__builtin_muluu(vacRMS,vacRMS)) >> 8;  //24-8 =16
                            
                #if(INPUTVOLTAGEFAULT)
             
                    /* Check for Input Over Voltage Or Under Voltage Fault */
                    pfcFaultFlags.inputUnderVoltage = FAULT_CheckMin(&fltIUVSource, vacRMS, &faultState); //Under
                    pfcFaultFlags.inputOverVoltage = FAULT_CheckMax(&fltIOVSource, vacRMS, &faultState); //Over
                                         
                #endif  
                
                /*Switch Current to detect Overload Condition at Vpeak*/
                peakCurrent = (currentCH1Filtered + currentCH2Filtered); //Ipeak = Ip1+Ip2
                rmsCurrent = (__builtin_muluu(peakCurrent,VPK2VRMS) >> 16); // 12bits
    
                #if(INPUTPOWERFAULT)
                    computedPower = __builtin_muluu(vacPeak,peakCurrent)>>9;  //(24-16)+1 bit P=Vp*Ip/2
                                        
                    /* Check for Input Over Power */
                    pfcFaultFlags.inputOverPower = FAULT_CheckMax(&fltIOPSource, computedPower, &faultState); 

                    #if(POWERDERATING_CURRENT == ENABLED)    
                    /* Limit Input Current to set value (3.2A) */
                    if((rmsCurrent >= CURRENTRMSMAXADC) && !(currentDeratingFlag))
                    {
                        pfcVoltCompOutputMax = pfcVoltCompOutput;
                        currentDeratingFlag = 1;
                    }
                    else if(rmsCurrent <= (CURRENTRMSMAXADC - 450)) //600mA hysteresis
                    {
                        currentDeratingFlag = 0;
                        pfcVoltCompOutputMax = VOLTAGE_COMP_2P2Z_MAX_CLAMP;
                    }

                    #endif
                      
                #endif  
                    
                #if(POWERDERATING_REFVOLTAGE==ENABLED)
                    /*Derate Power below 18V by changing the Reference Voltage */
                    if(vacRMS < VOLTDERATINGLIMIT)
                    {
                        vacDiff = (VOLTDERATINGLIMIT - vacRMS);
                        vacRef = (__builtin_muluu(DRTCONST,vacDiff))>>14;    
                        pfcVoltageRef = PFCVOLTAGEREFADC - vacRef;
                    }
                    else if(vacRMS > VOLTDERATINGHYSTLIMIT)
                    {
                        pfcVoltageRef = PFCVOLTAGEREFADC;
                    }
                    
                #endif
                    
                vacFilteredMax = 0;
                
                transitionHLEnable = 0; //Flag Reset at Peak
            }
        }

        prevVacFiltered = vacInputFiltered;   
    }
    
    #if(DEBUG_IO ==  AN2ISR)
    IO_DRV = OFF;
    #endif

//    _ADCAN2IF = 0;          // Clear Interrupt Flag
}

/*******************************************************************************
End of ISR
*******************************************************************************/

/*******************************************************************************
Function Description:  PWM Time Period Calculation using Input Capture
*******************************************************************************/  
uint16_t icCapture2,icValue1;
uint16_t transitionStartFlag=0;
static uint16_t icCapture1;
static uint16_t transitionCount,varCount;

//void __attribute__ ((__interrupt__, no_auto_psv)) _IC1Interrupt(void)
void IC1_CallBack(void)
{
    #if(DEBUG_IO ==  IC1ISR)
    IO_DRV = ON;
    #endif
    icCapture2 = icCapture1;
    icCapture1 = IC1BUF; // Read and save off first capture entry
    icValue1 = (icCapture1-icCapture2);
    icDiff = __builtin_muluu(icValue1,FREQCONST);
    valueSyncPDC = (icDiff>>15)-PROPDELAY; //Multiply by relation between Fpwm and Fperipheral ((120M*(8/4))/70M) (>> 14 + 1) //30 count prop. delay
           
    #if(DIST_MGMT)
            if((transitionStartFlag == 0) && (transitionCount == 0))
            {
                syncPDC = valueSyncPDC >> 1;
                if(varCount++ >= 3)
                {
                    transitionCount = 1;
                    varCount = 0;
                }
            }
            else if((transitionStartFlag == 0) && (transitionCount == 1))
            {
                syncPDC = valueSyncPDC;
            }  
            else
            {
                syncPDC = valueSyncPDC >> 1;
                transitionCount = 0;
            }
            
            PWM_DutyCycleSet(SYNCPWMA,syncPDC);
    
        #else
            PWM_DutyCycleSet(SYNCPWMA,valueSyncPDC);   //50% dutycyle
        #endif
    
    
    #if(DEBUG_IO ==  IC1ISR)
    IO_DRV = OFF;
    #endif
    
//    IFS0bits.IC1IF = 0; // Reset respective interrupt flag
}

/*******************************************************************************
End of Function
*******************************************************************************/

/*******************************************************************************
Function Description:  Line Frequency Period Calculation using Input Capture
*******************************************************************************/  
static uint16_t icFirstPass = 0;
static uint16_t icCapture3, icCapture4, icValuePrev;
uint16_t icCapture3Prev, icCapture4Prev;
uint16_t icCapture3Diff,icCapture4Diff;

//void __attribute__ ((__interrupt__, no_auto_psv)) _IC3Interrupt(void)
void IC3_CallBack(void)
{
    #if(DEBUG_IO ==  IC3ISR)
        IO_DRV = ON;
    #endif
    icCapture3Prev = icCapture3;
    icCapture4Prev = icCapture4;
    icCapture3 = IC3BUF; // Read and save off first capture entry
    icCapture4 = IC4BUF; // Read and save off first capture entry
    icCapture3Diff = icCapture3 - icCapture3Prev;
    icCapture4Diff = icCapture4 - icCapture4Prev;
    
    icValue = ((icCapture4Diff << 8) + (icCapture3Diff >> 8) + icValuePrev)>>1;
    icValuePrev = icValue;
    
    if(icFirstPass)
    {
        #if(FREQUENCYFAULT)
        {
            /* Check for Output Over Voltage Or Under Voltage Fault */
            pfcFaultFlags.inputUnderFrequency = FAULT_CheckMax(&fltIUFSource, icValue, &faultState); //Under
            pfcFaultFlags.inputOverFrequency = FAULT_CheckMin(&fltIOFSource, icValue, &faultState); //Over
                
        }
        #endif
    }
    
    icFirstPass = 1;    //First reading of icValue is erroneous
    
    #if(DEBUG_IO ==  IC3ISR)
        IO_DRV = OFF;
    #endif
    
//    IFS2bits.IC3IF = 0; // Reset respective interrupt flag
}

/*******************************************************************************
End of Function
*******************************************************************************/


/*******************************************************************************
ISR: 		TMR3Interrupt
Description:	Interrupt for voltage compensator, bulk voltage reduction/boost,
                and converter softstart
Working Reg:    Uses Alt W-Reg Bank #2 for compensator (manual switch)
*******************************************************************************/
//void __attribute__((__interrupt__,no_auto_psv)) _T3Interrupt()
void TMR3_CallBack(void)
{
 
    #if(DEBUG_IO == VOLTAGELOOP)
    IO_DRV = ON;
    #endif

    sftStartTimer++;    //Increment Softstart timer
   
    // Call 2P2Z Compensator
    __asm__("CTXTSWP #0x2");
    SMPS_Controller2P2ZUpdate_HW_Accel();  
    __asm__("CTXTSWP #0x0");

    #if(CONSTXOVERFREQ)
        xoverFactor = __builtin_divud(MAXINPUTVOLTAGEADCSQUARED,vacRMSSquared);   //32-16 = 16   
        pfcVoltCompOutput = (__builtin_muluu(pfcVoltCompOutput,xoverFactor)) >> 8;      //Multiply by crossover factor
    #endif
           
    /*Limit ON time if Current Derating is Enabled*/
    if(pfcVoltCompOutput >= pfcVoltCompOutputMax)
        pfcVoltCompOutput = pfcVoltCompOutputMax;
    
    /* Distortion Reduction Code */    
    #if(DIST_MGMT)
        if(vacInputFiltered < VTHRESHADC)
        {
            slavePDC = 0;
            masterPDC = pfcVoltCompOutput << 1;  //PDC*2
         
            IOCON1bits.OVRENH = 1;    //Disable PWM1  
            transitionStartFlag = 1;
        }

        else
        {
            transitionStartFlag = 0;
            slavePDC = pfcVoltCompOutput;
            masterPDC = pfcVoltCompOutput;
            IOCON1bits.OVRENH = 0;    //Enable PWM1  
        }
    #endif
    
    /* Phase management Code */
     #if(PHASE_MGMT)
        
        if(zeroCrossDetect && (state == NORMAL))
        {
            zeroCrossDetect = 0;
            
            phaseThreshold = PHASE_MGMT_THRESHOLD;
                    
            #if(CONSTXOVERFREQ)
                phaseThreshold = (__builtin_muluu(PHASE_MGMT_THRESHOLD,xoverFactor)) >> 8;      //Multiply by crossover factor
            #endif
                          
               
            if(rmsCurrent <= (phaseThreshold-PHASE_MGMT_HYST))
            {
                slavePDC = 0;
                masterPDC = pfcVoltCompOutput << 1;
                phaseState = 1;
            }
            else if(rmsCurrent >= (phaseThreshold+PHASE_MGMT_HYST))
            {
                slavePDC = pfcVoltCompOutput;
                masterPDC = pfcVoltCompOutput;
                phaseState = 2;
            }
            else if((rmsCurrent >= (phaseThreshold-PHASE_MGMT_HYST)) && (rmsCurrent <= (phaseThreshold+PHASE_MGMT_HYST)))
            {
                if(phaseState == 1)
                {
                    slavePDC = 0;    
                    masterPDC = pfcVoltCompOutput << 1;
                }
                else if(phaseState == 2)
                {
                    slavePDC = pfcVoltCompOutput;
                    masterPDC = pfcVoltCompOutput;
                }
            }
            
        }
        else if(state == SOFTSTART)  //Allow both units during softstart
        {
            slavePDC = pfcVoltCompOutput;
            masterPDC = pfcVoltCompOutput;
        }
        
    #endif
    
    #if((DIST_MGMT) && (PHASE_MGMT))
    
        if(phaseState == 1)
        {
            slavePDC = 0;
            masterPDC = pfcVoltCompOutput << 1;       
        }
       
    #endif    
        
    #if((!DIST_MGMT) && (!PHASE_MGMT))
    
        slavePDC = pfcVoltCompOutput;
        masterPDC = pfcVoltCompOutput;       
       
    #endif
      
    /* Assign DutyCycle */    
    PWM_DutyCycleSet(master, masterPDC);
    PWM_DutyCycleSet(slave, slavePDC);
            
    /* Calculate Trigger Point */
    PWM_TriggerCompareValueSet(master, ((masterPDC>>1)+PROPDELAY));
    PWM_TriggerCompareValueSet(slave, ((slavePDC>>1)+PROPDELAY));
    
//    PWM_TriggerCompareValueSet(master, ((masterPDC>>1)));
//    PWM_TriggerCompareValueSet(slave, ((slavePDC>>1)));
          
    #if (DACOUTPIN == VLOOPOUT)
    CMP2DAC = pfcVoltCompOutput;
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


/*******************************************************************************
ISR: 		TMR4Interrupt
Description:	Interrupt for LED blinking routine 
*******************************************************************************/
//void __attribute__((__interrupt__,no_auto_psv)) _T5Interrupt()
void TMR4_CallBack(void)
{
 
    #if(DEBUG_IO == TIMERBLINK)
    IO_DRV = !IO_DRV;
    #endif

    if(state == FAULT)
        faultElapsedCount++;
    
    FAULT_BlinkLED();

//    #if(DEBUG_IO == TIMERBLINK)
//    IO_DRV = OFF;
//    #endif
    
//    _T5IF = 0;       // Clear Interrupt Flag
}
/*******************************************************************************
End of ISR
*******************************************************************************/

/*******************************************************************************
ISR: 		 ADCAN11Interrupt
Description: Interrupt for Temperature Monitoring
*******************************************************************************/
//void __attribute__((__interrupt__,no_auto_psv)) _ADCAN11Interrupt()
void ADCAN11_CallBack()
{
    static uint16_t tempInputSample[4] = {0,0,0,0};
        
    uint8_t tempArrayIndex = 0;
    uint16_t tempInputSum = 0;
                  
    #if(DEBUG_IO ==  TEMP)
    IO_DRV = !IO_DRV;
    #endif
    
    timerCount++;
    
    tempInputSum = ADCBUF11;
    tempInputSample[0] = ADCBUF11;
               
    for(tempArrayIndex = 3;tempArrayIndex >= 1;tempArrayIndex--)   
    {
        tempInputSum += tempInputSample[tempArrayIndex];
        tempInputSample[tempArrayIndex] = tempInputSample[tempArrayIndex-1];
                
    }
    
    tempInputFiltered = tempInputSum >> 2;
    
    #if(TEMPFAULT)
        /* Check for Board Over Temp */
        pfcFaultFlags.boardOverTemp = FAULT_CheckMax(&fltIOTSource, tempInputFiltered, &faultState); 
    #endif
            
    #if (DACOUTPIN == TEMPFILTERED)
    CMP2DAC = tempInputFiltered;
    #endif
    
//    _ADCAN11IF = 0;          // Clear Interrupt Flag
}

/*******************************************************************************
End of ISR
*******************************************************************************/


