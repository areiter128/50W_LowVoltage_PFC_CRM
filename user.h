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

#define PWMRESOLUTION_REG 3U    // 1=1.06ns, 2=2.12ns, 3=4.24ns
                                // 4=8.48ns, Used in PTCON2 register
#define PWMCLKDIVBY 4U          // Value of CLKDIV divider taken from above

#define CONVSWITCHFREQ 30000UL  // Min PWM Switching frequency in Hz

#define PFCVOLTAGEREF  40U      // PFC output voltage in Volts (start up
                                // and when variable DC_Link is disabled)

#define SOFTSTARTTIME   1000    // value in ms

#define INPUTUNDERVOLTAGE 10    // Under Voltage threshold in Volts

#define INPUTOVERVOLTAGE 27     // Over Voltage threshold in Volts 

#define INPUTVOLTAGEHYST 2      // Hysteresis in Volts

#define PFCVOUTLIMIT  8         // Output Over/Under Voltage Limit over the reference value (40+/-8)
#define PFCVOUTHYST   2         // Hysteresis in Volts

#define LOFREQ 45               // Lower Operating Frequency limit in Hz
#define HIFREQ 62               // Upper Operating Frequency limit in Hz

#define CURRENTRMSMAX 3.1       // RMS current value (A) 

#define TEMPERATUREMAX 70       //Board Temperature near Heat Sink

#define CONSTXOVERFREQ DISABLED  // Constant Crossover Frequency Feature

#define DIST_MGMT DISABLED      // Reduction of Crossover Distortion

#define PHASE_MGMT DISABLED     // Phase Management for Better Efficiency at Lighter Loads


