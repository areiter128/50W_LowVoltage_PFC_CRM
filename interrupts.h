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

#include <p33Exxxx.h>
#include "defines.h"
#include "stdint.h"
#include "smps_control.h"
#include <stdbool.h>

typedef union{

    struct {
        uint16_t inputCurrent     : 1;
        uint16_t inputUnderVoltage : 1;
        uint16_t inputOverVoltage  : 1;
        uint16_t outputUnderVoltage: 1;
        uint16_t outputOverVoltage : 1;
        uint16_t inputUnderFrequency: 1;
        uint16_t inputOverFrequency : 1;
        uint16_t inputOverPower    : 1;
        uint16_t boardOverTemp     : 1;
        uint16_t auxPowerSupply    : 1;
        uint16_t dc2dcComm         : 1;
        uint16_t uartComm          : 1;
        uint16_t : 4;
    };
    
    uint16_t systemFaults;

} PFC_FAULTS;

typedef enum
{
    INIT = 1,
    IDLE = 2,
    SOFTSTART = 3,
    NORMAL = 4,
    FAULT = 5
} STATE_CODE;

/* Function Call Prototypes*/

//void ADCAN2_CallBack(void);
//void TMR2_CallBack(void);
//void TMR3_CallBack(void);
//void IC1_CallBack(void);
//void IC3_CallBack(void);
  