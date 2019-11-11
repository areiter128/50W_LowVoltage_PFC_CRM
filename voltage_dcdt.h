/**
  Software License Agreement

  Copyright © 2017 Microchip Technology Inc.  All rights reserved.
  Microchip licenses to you the right to use, modify, copy and distribute
  Software only when embedded on a Microchip microcontroller or digital
  signal controller, which is integrated into your product or third party
  product (pursuant to the sublicense terms in the accompanying license
  agreement).

  You should refer to the license agreement accompanying this Software
  for additional information regarding your rights and obligations.

  SOFTWARE AND DOCUMENTATION ARE PROVIDED ?AS IS? WITHOUT WARRANTY OF ANY
  KIND, EITHER EXPRESS OR IMPLIED, INCLUDING WITHOUT LIMITATION, ANY WARRANTY
  OF MERCHANTABILITY, TITLE, NON-INFRINGEMENT AND FITNESS FOR A PARTICULAR
  PURPOSE. IN NO EVENT SHALL MICROCHIP OR ITS LICENSORS BE LIABLE OR
  OBLIGATED UNDER CONTRACT, NEGLIGENCE, STRICT LIABILITY, CONTRIBUTION,
  BREACH OF WARRANTY, OR OTHER LEGAL EQUITABLE THEORY ANY DIRECT OR INDIRECT
  DAMAGES OR EXPENSES INCLUDING BUT NOT LIMITED TO ANY INCIDENTAL, SPECIAL,
  INDIRECT, PUNITIVE OR CONSEQUENTIAL DAMAGES, LOST PROFITS OR LOST DATA,
  COST OF PROCUREMENT OF SUBSTITUTE GOODS, TECHNOLOGY, SERVICES, OR ANY
  CLAIMS BY THIRD PARTIES (INCLUDING BUT NOT LIMITED TO ANY DEFENSE THEREOF),
  OR OTHER SIMILAR COSTS.
 **/
#ifndef _VOLTAGE_DCDT_DEFINES_H
#define _VOLTAGE_DCDT_DEFINES_H

/* Compensator Coefficient Defines */

/****************************************************************************/
/* Integrator followed by Pole-Zero pair */
/* fp0 = 0.1, fp1 = 20, fz1 = 4, Kdc = 1 */

//K1(z)
#define VOLTAGE_COMP_2P2Z_COEFF_B10      6400 
#define VOLTAGE_COMP_2P2Z_COEFF_B11      6400 
#define VOLTAGE_COMP_2P2Z_COEFF_A11      32089

// K2(z))
#define VOLTAGE_COMP_2P2Z_COEFF_B20      32496
#define VOLTAGE_COMP_2P2Z_COEFF_B21      -32360
#define VOLTAGE_COMP_2P2Z_COEFF_A21      32767

//K3(z) Bypass for 2P2Z
#define VOLTAGE_COMP_2P2Z_COEFF_B30      32767
#define VOLTAGE_COMP_2P2Z_COEFF_B31      0
#define VOLTAGE_COMP_2P2Z_COEFF_A31      0

#define VOLTAGE_COMP_2P2Z_POSTSCALER     32767
#define VOLTAGE_COMP_2P2Z_POSTSHIFT      3 
#define VOLTAGE_COMP_2P2Z_PRESHIFT       0

// Compensator Clamp Limits
#define VOLTAGE_COMP_2P2Z_MIN_CLAMP    0
#define VOLTAGE_COMP_2P2Z_MAX_CLAMP    2500

#endif
