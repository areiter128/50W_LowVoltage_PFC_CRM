
/**
  PWM Generated Driver API Source File 

  @Company
    Microchip Technology Inc.

  @File Name
    pwm.c

  @Summary
    This is the generated source file for the PWM driver using PIC24 / dsPIC33 / PIC32MM MCUs

  @Description
    This source file provides APIs for driver for PWM. 
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
#include "pwm.h"

/**
  Section: Driver Interface
*/


void PWM_Initialize (void)
{
    // PCLKDIV 4; 
    PTCON2 = 0x2;
    // PTPER 0; 
    PTPER = 0x0;
    // SEVTCMP 0; 
    SEVTCMP = 0x0;
    // MDC 0; 
    MDC = 0x0;
    // CHOPCLK 0; CHPCLKEN disabled; 
    CHOP = 0x0;
    // PWMKEY 0; 
    PWMKEY = 0x0;
    // TRGIEN disabled; MDCS Primary; XPRES enabled; ITB Primary; IUE disabled; CLIEN disabled; MTBS disabled; FLTIEN disabled; CAM Edge Aligned; 
    PWMCON1 = 0x202;
    // TRGIEN enabled; MDCS Primary; XPRES enabled; ITB Primary; IUE disabled; CLIEN disabled; MTBS disabled; FLTIEN disabled; CAM Edge Aligned; 
    PWMCON2 = 0x602;
    // TRGIEN disabled; MDCS Primary; XPRES enabled; ITB Primary; IUE enabled; CLIEN disabled; MTBS disabled; FLTIEN disabled; CAM Edge Aligned; 
    PWMCON3 = 0x203;
    // TRGIEN disabled; MDCS Primary; XPRES enabled; ITB Primary; IUE enabled; CLIEN disabled; MTBS disabled; FLTIEN disabled; CAM Edge Aligned; 
    PWMCON4 = 0x203;
    // TRGIEN disabled; MDCS Primary; XPRES enabled; ITB Primary; IUE disabled; CLIEN disabled; MTBS disabled; FLTIEN disabled; CAM Edge Aligned; 
    PWMCON5 = 0x202;
    // TRGIEN disabled; MDCS Primary; XPRES disabled; ITB Master; IUE disabled; CLIEN disabled; MTBS disabled; FLTIEN disabled; CAM Edge Aligned; 
    PWMCON6 = 0x0;
    // TRGIEN disabled; MDCS Primary; XPRES disabled; ITB Master; IUE disabled; CLIEN disabled; MTBS disabled; FLTIEN disabled; CAM Edge Aligned; 
    PWMCON7 = 0x0;
    // TRGIEN disabled; MDCS Primary; XPRES disabled; ITB Master; IUE disabled; CLIEN disabled; MTBS disabled; FLTIEN disabled; CAM Edge Aligned; 
    PWMCON8 = 0x0;
    //FLTDAT PWM1L Low, PWM1H Low; SWAP disabled; OVRENH enabled; PENL disabled; PMOD Redundant Output Mode; OVRENL enabled; OSYNC disabled; POLL disabled; PENH enabled; CLDAT PWM1L Low, PWM1H Low; OVRDAT PWM1L Low, PWM1H Low; POLH disabled; 
    __builtin_write_PWMSFR(&IOCON1, 0x8700, &PWMKEY);
    //FLTDAT PWM2L Low, PWM2H Low; SWAP disabled; OVRENH enabled; PENL disabled; PMOD Redundant Output Mode; OVRENL enabled; OSYNC disabled; POLL disabled; PENH enabled; CLDAT PWM2L Low, PWM2H Low; OVRDAT PWM2L Low, PWM2H Low; POLH disabled; 
    __builtin_write_PWMSFR(&IOCON2, 0x8700, &PWMKEY);
    //FLTDAT PWM3L Low, PWM3H Low; SWAP disabled; OVRENH disabled; PENL disabled; PMOD Complementary Output Mode; OVRENL disabled; OSYNC disabled; POLL disabled; PENH enabled; CLDAT PWM3L Low, PWM3H Low; OVRDAT PWM3L Low, PWM3H Low; POLH disabled; 
    __builtin_write_PWMSFR(&IOCON3, 0x8000, &PWMKEY);
    //FLTDAT PWM4L Low, PWM4H Low; SWAP disabled; OVRENH enabled; PENL disabled; PMOD Redundant Output Mode; OVRENL disabled; OSYNC disabled; POLL disabled; PENH enabled; CLDAT PWM4L Low, PWM4H Low; OVRDAT PWM4L Low, PWM4H Low; POLH disabled; 
    __builtin_write_PWMSFR(&IOCON4, 0x8600, &PWMKEY);
    //FLTDAT PWM5L Low, PWM5H Low; SWAP disabled; OVRENH disabled; PENL enabled; PMOD Redundant Output Mode; OVRENL disabled; OSYNC disabled; POLL disabled; PENH enabled; CLDAT PWM5L Low, PWM5H Low; OVRDAT PWM5L Low, PWM5H Low; POLH disabled; 
    __builtin_write_PWMSFR(&IOCON5, 0xC400, &PWMKEY);
    //FLTDAT PWM6L Low, PWM6H Low; SWAP disabled; OVRENH disabled; PENL disabled; PMOD Complementary Output Mode; OVRENL disabled; OSYNC disabled; POLL disabled; PENH disabled; CLDAT PWM6L Low, PWM6H Low; OVRDAT PWM6L Low, PWM6H Low; POLH disabled; 
    __builtin_write_PWMSFR(&IOCON6, 0x0, &PWMKEY);
    //FLTDAT PWM7L Low, PWM7H Low; SWAP disabled; OVRENH disabled; PENL disabled; PMOD Complementary Output Mode; OVRENL disabled; OSYNC disabled; POLL disabled; PENH disabled; CLDAT PWM7L Low, PWM7H Low; OVRDAT PWM7L Low, PWM7H Low; POLH disabled; 
    __builtin_write_PWMSFR(&IOCON7, 0x0, &PWMKEY);
    //FLTDAT PWM8L Low, PWM8H Low; SWAP disabled; OVRENH disabled; PENL disabled; PMOD Complementary Output Mode; OVRENL disabled; OSYNC disabled; POLL disabled; PENH disabled; CLDAT PWM8L Low, PWM8H Low; OVRDAT PWM8L Low, PWM8H Low; POLH disabled; 
    __builtin_write_PWMSFR(&IOCON8, 0x0, &PWMKEY);
    //FLTPOL disabled; CLPOL enabled; CLSRC FLT6; CLMOD disabled; FLTMOD Fault input is disabled; IFLTMOD disabled; FLTSRC FLT1; 
    __builtin_write_PWMSFR(&FCLCON1, 0x1A0B, &PWMKEY);
    //FLTPOL disabled; CLPOL disabled; CLSRC CMP4; CLMOD disabled; FLTMOD Fault input is disabled; IFLTMOD disabled; FLTSRC FLT1; 
    __builtin_write_PWMSFR(&FCLCON2, 0x400B, &PWMKEY);
    //FLTPOL disabled; CLPOL disabled; CLSRC CMP4; CLMOD disabled; FLTMOD Fault input is disabled; IFLTMOD disabled; FLTSRC FLT8; 
    __builtin_write_PWMSFR(&FCLCON3, 0x4043, &PWMKEY);
    //FLTPOL disabled; CLPOL disabled; CLSRC CMP4; CLMOD disabled; FLTMOD Fault input is disabled; IFLTMOD disabled; FLTSRC FLT1; 
    __builtin_write_PWMSFR(&FCLCON4, 0x400B, &PWMKEY);
    //FLTPOL disabled; CLPOL disabled; CLSRC CMP3; CLMOD disabled; FLTMOD Fault input is disabled; IFLTMOD disabled; FLTSRC FLT1; 
    __builtin_write_PWMSFR(&FCLCON5, 0x3C0B, &PWMKEY);
    //FLTPOL disabled; CLPOL disabled; CLSRC FLT1; CLMOD disabled; FLTMOD PWM6H, PWM6L pins to FLTDAT values- Latched; IFLTMOD disabled; FLTSRC FLT1; 
    __builtin_write_PWMSFR(&FCLCON6, 0x408, &PWMKEY);
    //FLTPOL disabled; CLPOL disabled; CLSRC FLT1; CLMOD disabled; FLTMOD PWM7H, PWM7L pins to FLTDAT values- Latched; IFLTMOD disabled; FLTSRC FLT1; 
    __builtin_write_PWMSFR(&FCLCON7, 0x408, &PWMKEY);
    //FLTPOL disabled; CLPOL disabled; CLSRC FLT1; CLMOD disabled; FLTMOD PWM8H, PWM8L pins to FLTDAT values- Latched; IFLTMOD disabled; FLTSRC FLT1; 
    __builtin_write_PWMSFR(&FCLCON8, 0x408, &PWMKEY);
    // PDC1 100; 
    PDC1 = 0x64;
    // PDC2 100; 
    PDC2 = 0x64;
    // PDC3 0; 
    PDC3 = 0x0;
    // PDC4 0; 
    PDC4 = 0x0;
    // PDC5 10; 
    PDC5 = 0xA;
    // PDC6 0; 
    PDC6 = 0x0;
    // PDC7 0; 
    PDC7 = 0x0;
    // PDC8 0; 
    PDC8 = 0x0;
    // PHASE1 7856; 
    PHASE1 = 0x1EB0;
    // PHASE2 7856; 
    PHASE2 = 0x1EB0;
    // PHASE3 7856; 
    PHASE3 = 0x1EB0;
    // PHASE4 7856; 
    PHASE4 = 0x1EB0;
    // PHASE5 7856; 
    PHASE5 = 0x1EB0;
    // PHASE6 0; 
    PHASE6 = 0x0;
    // PHASE7 0; 
    PHASE7 = 0x0;
    // PHASE8 0; 
    PHASE8 = 0x0;
    // DTR1 0; 
    DTR1 = 0x0;
    // DTR2 0; 
    DTR2 = 0x0;
    // DTR3 0; 
    DTR3 = 0x0;
    // DTR4 0; 
    DTR4 = 0x0;
    // DTR5 0; 
    DTR5 = 0x0;
    // DTR6 0; 
    DTR6 = 0x0;
    // DTR7 0; 
    DTR7 = 0x0;
    // DTR8 0; 
    DTR8 = 0x0;
    // ALTDTR1 0; 
    ALTDTR1 = 0x0;
    // ALTDTR2 0; 
    ALTDTR2 = 0x0;
    // ALTDTR3 0; 
    ALTDTR3 = 0x0;
    // ALTDTR4 0; 
    ALTDTR4 = 0x0;
    // ALTDTR5 0; 
    ALTDTR5 = 0x0;
    // ALTDTR6 0; 
    ALTDTR6 = 0x0;
    // ALTDTR7 0; 
    ALTDTR7 = 0x0;
    // ALTDTR8 0; 
    ALTDTR8 = 0x0;
    // SDC1 0; 
    SDC1 = 0x0;
    // SDC2 0; 
    SDC2 = 0x0;
    // SDC3 0; 
    SDC3 = 0x0;
    // SDC4 0; 
    SDC4 = 0x0;
    // SDC5 0; 
    SDC5 = 0x0;
    // SDC6 0; 
    SDC6 = 0x0;
    // SDC7 0; 
    SDC7 = 0x0;
    // SDC8 0; 
    SDC8 = 0x0;
    // SPHASE1 0; 
    SPHASE1 = 0x0;
    // SPHASE2 0; 
    SPHASE2 = 0x0;
    // SPHASE3 0; 
    SPHASE3 = 0x0;
    // SPHASE4 0; 
    SPHASE4 = 0x0;
    // SPHASE5 0; 
    SPHASE5 = 0x0;
    // SPHASE6 0; 
    SPHASE6 = 0x0;
    // SPHASE7 0; 
    SPHASE7 = 0x0;
    // SPHASE8 0; 
    SPHASE8 = 0x0;
    // TRGCMP 45; 
    TRIG1 = 45;
    // TRGCMP 45; 
    TRIG2 = 45;
    // TRGCMP 0; 
    TRIG3 = 0x0;
    // TRGCMP 0; 
    TRIG4 = 0x0;
    // TRGCMP 0; 
    TRIG5 = 0x0;
    // TRGCMP 0; 
    TRIG6 = 0x0;
    // TRGCMP 0; 
    TRIG7 = 0x0;
    // TRGCMP 0; 
    TRIG8 = 0x0;
    // TRGDIV 4; TRGSTRT 1; DTM disabled; 
    TRGCON1 = 0x3001;
    // TRGDIV 4; TRGSTRT 1; DTM disabled; 
    TRGCON2 = 0x3001;
    // TRGDIV 1; TRGSTRT 0; DTM disabled; 
    TRGCON3 = 0x0;
    // TRGDIV 1; TRGSTRT 0; DTM disabled; 
    TRGCON4 = 0x0;
    // TRGDIV 1; TRGSTRT 0; DTM disabled; 
    TRGCON5 = 0x0;
    // TRGDIV 1; TRGSTRT 0; DTM disabled; 
    TRGCON6 = 0x0;
    // TRGDIV 1; TRGSTRT 0; DTM disabled; 
    TRGCON7 = 0x0;
    // TRGDIV 1; TRGSTRT 0; DTM disabled; 
    TRGCON8 = 0x0;
    // STRGCMP 0; 
    STRIG1 = 0x0;
    // STRGCMP 0; 
    STRIG2 = 0x0;
    // STRGCMP 0; 
    STRIG3 = 0x0;
    // STRGCMP 0; 
    STRIG4 = 0x0;
    // STRGCMP 0; 
    STRIG5 = 0x0;
    // STRGCMP 0; 
    STRIG6 = 0x0;
    // STRGCMP 0; 
    STRIG7 = 0x0;
    // STRGCMP 0; 
    STRIG8 = 0x0;
    // PWMCAP 0; 
    PWMCAP1 = 0x0;
    // PWMCAP 0; 
    PWMCAP2 = 0x0;
    // PWMCAP 0; 
    PWMCAP3 = 0x0;
    // PWMCAP 0; 
    PWMCAP4 = 0x0;
    // PWMCAP 0; 
    PWMCAP5 = 0x0;
    // PWMCAP 0; 
    PWMCAP6 = 0x0;
    // PWMCAP 0; 
    PWMCAP7 = 0x0;
    // PWMCAP 0; 
    PWMCAP8 = 0x0;
    // BPLL disabled; BPHH disabled; BPLH disabled; BCH disabled; FLTLEBEN disabled; PLR disabled; CLLEBEN disabled; BCL disabled; PLF disabled; PHR disabled; BPHL disabled; PHF disabled; 
    LEBCON1 = 0x0;
    // BPLL disabled; BPHH disabled; BPLH disabled; BCH disabled; FLTLEBEN disabled; PLR disabled; CLLEBEN disabled; BCL disabled; PLF disabled; PHR disabled; BPHL disabled; PHF disabled; 
    LEBCON2 = 0x0;
    // BPLL disabled; BPHH disabled; BPLH disabled; BCH disabled; FLTLEBEN disabled; PLR disabled; CLLEBEN disabled; BCL disabled; PLF disabled; PHR disabled; BPHL disabled; PHF disabled; 
    LEBCON3 = 0x0;
    // BPLL disabled; BPHH disabled; BPLH disabled; BCH disabled; FLTLEBEN disabled; PLR disabled; CLLEBEN disabled; BCL disabled; PLF disabled; PHR disabled; BPHL disabled; PHF disabled; 
    LEBCON4 = 0x0;
    // BPLL disabled; BPHH disabled; BPLH disabled; BCH disabled; FLTLEBEN disabled; PLR disabled; CLLEBEN disabled; BCL disabled; PLF disabled; PHR disabled; BPHL disabled; PHF disabled; 
    LEBCON5 = 0x0;
    // BPLL disabled; BPHH disabled; BPLH disabled; BCH disabled; FLTLEBEN disabled; PLR disabled; CLLEBEN disabled; BCL disabled; PLF disabled; PHR disabled; BPHL disabled; PHF disabled; 
    LEBCON6 = 0x0;
    // BPLL disabled; BPHH disabled; BPLH disabled; BCH disabled; FLTLEBEN disabled; PLR disabled; CLLEBEN disabled; BCL disabled; PLF disabled; PHR disabled; BPHL disabled; PHF disabled; 
    LEBCON7 = 0x0;
    // BPLL disabled; BPHH disabled; BPLH disabled; BCH disabled; FLTLEBEN disabled; PLR disabled; CLLEBEN disabled; BCL disabled; PLF disabled; PHR disabled; BPHL disabled; PHF disabled; 
    LEBCON8 = 0x0;
    // LEB 0; 
    LEBDLY1 = 0x0;
    // LEB 0; 
    LEBDLY2 = 0x0;
    // LEB 0; 
    LEBDLY3 = 0x0;
    // LEB 0; 
    LEBDLY4 = 0x0;
    // LEB 0; 
    LEBDLY5 = 0x0;
    // LEB 0; 
    LEBDLY6 = 0x0;
    // LEB 0; 
    LEBDLY7 = 0x0;
    // LEB 0; 
    LEBDLY8 = 0x0;
    // CHOPLEN disabled; HRDDIS disabled; CHOPHEN disabled; BLANKSEL No state blanking; CHOPSEL No state blanking; HRPDIS disabled; 
    AUXCON1 = 0x0;
    // CHOPLEN disabled; HRDDIS disabled; CHOPHEN disabled; BLANKSEL No state blanking; CHOPSEL No state blanking; HRPDIS disabled; 
    AUXCON2 = 0x0;
    // CHOPLEN disabled; HRDDIS disabled; CHOPHEN disabled; BLANKSEL No state blanking; CHOPSEL No state blanking; HRPDIS disabled; 
    AUXCON3 = 0x0;
    // CHOPLEN disabled; HRDDIS disabled; CHOPHEN disabled; BLANKSEL No state blanking; CHOPSEL No state blanking; HRPDIS disabled; 
    AUXCON4 = 0x0;
    // CHOPLEN disabled; HRDDIS disabled; CHOPHEN disabled; BLANKSEL No state blanking; CHOPSEL No state blanking; HRPDIS disabled; 
    AUXCON5 = 0x0;
    // CHOPLEN disabled; HRDDIS disabled; CHOPHEN disabled; BLANKSEL No state blanking; CHOPSEL No state blanking; HRPDIS disabled; 
    AUXCON6 = 0x0;
    // CHOPLEN disabled; HRDDIS disabled; CHOPHEN disabled; BLANKSEL No state blanking; CHOPSEL No state blanking; HRPDIS disabled; 
    AUXCON7 = 0x0;
    // CHOPLEN disabled; HRDDIS disabled; CHOPHEN disabled; BLANKSEL No state blanking; CHOPSEL No state blanking; HRPDIS disabled; 
    AUXCON8 = 0x0;
    

    // SYNCOEN disabled; SEIEN disabled; SESTAT disabled; SEVTPS 1; SYNCSRC SYNCI1; SYNCEN disabled; PTSIDL disabled; PTEN disabled; EIPU disabled; SYNCPOL disabled; 
    PTCON = 0x0;
}


    
/**
 End of File
*/
