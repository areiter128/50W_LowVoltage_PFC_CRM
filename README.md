# Microchip Technology 50W Low Voltage PFC Development Kit (DV330101)

Critical Conduction Mode PFC Firmware Example using dsPIC33EP128GS506 Digital Power PIM (DM330029)

### Official Website: 
Please visit Microchip Technology's official website for more information on this development kit, such as user guides and design documentation as well as ordering information:

          https://www.microchip.com/DevelopmentTools/ProductDetails/PartNO/DV330101

### Description:
Microchip Technology's Low Voltage Power Factor Correction (LVPFC) Development Kit offers safe voltage levels at moderate power while designing algorithms on a boost power factor correction topology. These algorithms can be applied on real systems under development with minimal changes. The LVPFC Development Board utilizes the dsPIC33EP128GS806 device, supporting full digital and advanced power control algorithm schemes.

LVPFC development kit utilizes an additional isolation transformer (ASC70) with turn ratio of 10:1 and a 50W Active load (ASC50). These additional tools are available here from our third party tools provider ASCALAB:

          http://www.ascalab.com/shop/

The Active Load module is also available directly from microchip Direct under the part number TH1026. 

          https://www.microchipdirect.com/product/search/all/ascalab

### Critical Conduction Mode Firmware

This code example demonstrates how to run an interleaved bost PFC stage in Critical Conduction Mode, also known as Boundary Mode. In this switching scheme the PFC MOSFETs are turned on until the refrene current provided by the feedback loop hits the limit. The inductors are discharging until the inductor current reaches/crosses zero. At this point the next PWM pulse is produced. 

This code example utilizes the on-chip comparators of dsPIC33EP128GS506 to trigger on peak values and zero crossings of the current feedback waveform. The digital software control loop regulates the 40V output voltage by setting the reference of the peak current. 
In addition, the control loop performs power factor correction by aligning the input current under the input voltage using various techniques to optimize power factor and total harmonic distortion (THD)

### Please note:

Critical Consudtion Mode/Boundary Mode is one of three control schemes supported by this design. 
Code examples for Fixed Frequency Continuous Conduction Mode and Fixed Frequency Discontinuous Conduction Mode are availabe in their respective repositories.

