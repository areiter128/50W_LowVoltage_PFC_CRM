;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; © 2015 Microchip Technology Inc.
;;
;; MICROCHIP SOFTWARE NOTICE AND DISCLAIMER:  You may use this software, and any 
;;derivatives, exclusively with Microchip?s products. This software and any 
;;accompanying information is for suggestion only.  It does not modify Microchip?s 
;;standard warranty for its products.  You agree that you are solely responsible 
;;for testing the software and determining its suitability.  Microchip has no 
;;obligation to modify, test, certify, or support the software.
;;
;; THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS".  NO WARRANTIES, WHETHER EXPRESS,
;;IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED WARRANTIES OF
;;NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A PARTICULAR PURPOSE, OR ITS 
;;INTERACTION WITH MICROCHIP PRODUCTS, COMBINATION WITH ANY OTHER PRODUCTS, OR USE 
;;IN ANY APPLICATION.
 
;;IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, INCIDENTAL 
;;OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND WHATSOEVER RELATED TO THE 
;;SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS BEEN ADVISED OF THE POSSIBILITY OR 
;;THE DAMAGES ARE FORESEEABLE.  TO THE FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S 
;;TOTAL LIABILITY ON ALL CLAIMS IN ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED 
;;THE AMOUNT OF FEES, IF ANY, THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.

;;MICROCHIP PROVIDES THIS SOFTWARE CONDITIONALLY UPON YOUR ACCEPTANCE OF THESE TERMS. 

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;


.include "p33Exxxx.inc"
#include "defines.h"

.data   ; Tell assembler to add subsequent data to the data section

.text   ; Begin program instructions

.global _InitAltRegContext2Setup

  _InitAltRegContext2Setup:

  CTXTSWP #0x2   ; Swap to Alternate W-Reg #2

  ; Initialize Alternate Working Registers context #2

  mov #_pfcVoltageRef,			w0   ; Address of the Reference register (Control Ref.)
  mov #ADCBUF3,				    w1   ; Address of the Input register (Input)
  mov #_pfcVoltCompOutput,		w2   ; Address of the Output target register (Output)

  ; w3, w4, w5 used for ACCAx registers and for MAC/MPY instructions
  ; Initialize registers to '0'
  mov #_resultacc,			         w3
  mov #VOLTAGE_COMP_2P2Z_POSTSCALER, w6
  mov #VOLTAGE_COMP_2P2Z_POSTSHIFT,	 w7
  mov #_pfcOptions,			         w8    ; Points to user options structure
  mov #_pfcVoltageABCoefficients,	 w9
  mov #_pfcVoltageErrorControlHistory,  w10
  mov #VOLTAGE_COMP_2P2Z_MIN_CLAMP,	 w11
  mov #VOLTAGE_COMP_2P2Z_MAX_CLAMP,	 w12
  mov #ACCAL,				         w13
  
  CTXTSWP #0x0  ; Swap back to main register set

  return        ; Exit Alt-WREG2 set-up function

.end
