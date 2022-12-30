/*
* Copyright (c) 2018, Shawn D'silva <shawn@shawndsilva.com>
* All rights reserved.
*
*  This file is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This file is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this library.  If not, see <http://www.gnu.org/licenses/>.
*
* File:			startup.c
* Author:		Shawn D'silva <https://www.shawndsilva.com>.
* Version:		1.0.0.
* Description:	startup file for the TM4C Launchpad board,defines the vector table,
  and most importantly the Reset_Handler enabling the TM4C to execute the main program when the <RESET>
  button is pressed on board
*/

#include "startup.h"

// +-----------------------------------------------------------------------------------+
// +										            Vector Table                                       +
// +-----------------------------------------------------------------------------------+

__attribute__((section(".vector_table"))) //marks this vector table as a part of the section "".vector_table"
//in the linker script
const vector_table_t vectors[] = {
    {.stack_top = &_stack_ptr}, // 0    Pointer to top of Stack
    Reset_Handler,              // 1    Reset handler is called when the <RESET> button is pressed
    NMI_Handler,                // 2    Non-Maskable Interrupt handler
    HardFault_Handler,          // 3    Hard Fault Handler
    MemManageFault_Handler,     // 4    Memory management fault Handler
    BusFault_Handler,           // 5    Bus Fault Handler
    UsageFault_Handler,         // 6    Usage Fault Handler
    0,                          // 7    Reserved
    0,                          // 8    Reserved
    0,                          // 9    Reserved
    0,                          // 10   Reserved
    SVC_Handler,                // 11   SuperVisor Call Handler
    DebugMonitor_Handler,       // 12   Debug Monitor Handler
    0,                          // 13   Reserved
    PendSV_Handler,             // 14   Pendeable interrupt driven request
    SysTick_Handler,            // 15   SysTick Timer handler
    GPIOPortA_Handler,              // 16   GPIO Port A Interrupt Service Routine
    GPIOPortB_Handler,              // 17   GPIO Port B Interrupt Service Routine
    GPIOPortC_Handler,              // 18   GPIO Port C Interrupt Service Routine
    GPIOPortD_Handler,              // 19   GPIO Port D Interrupt Service Routine
    GPIOPortE_Handler,              // 20   GPIO Port C Interrupt Service Routine
    UART0_Handler,                  // 21   UART 0
    UART1_Handler,                  // 22   UART 1
    SPI0_Handler,                   // 23   SPI 0
    I2C0_Handler,
    PWM0Fault_Handler,
    PWM0Generator0_Handler,
    PWM0Generator1_Handler,
    PWM0Generator2_Handler,
    QEI0_Handler,
    ADC0Sequence0_Handler,
    ADC0Sequence1_Handler,
    ADC0Sequence2_Handler,
    ADC0Sequence3_Handler,
    WatchDogTimer_Handler,
    Timer0A_Handler,
    Timer0B_Handler,
    Timer1A_Handler,
    Timer1B_Handler,
    Timer2A_Handler,
    Timer2B_Handler,
    AnalogComparator0_Handler,
    AnalogComparator1_Handler,
    0,
    SystemCtrl_Handler,
    FlashCtrl_Handler,
    GPIOPortF_Handler,
    0,
    0,
    UART2_Handler,
    SPI1_Handler,
    Timer3A_Handler,
    Timer3B_Handler,
    I2C1_Handler,
    QEI1_Handler,
    CAN0_Handler,
    CAN1_Handler,
    0,
    0,
    Hibernation_Handler,
    USB0_Handler,
    PWM0Generator3_Handler,
    UDMASoftware_Handler,
    UDMAError_Handler,
    ADC1Sequence0_Handler,
    ADC1Sequence1_Handler,
    ADC1Sequence2_Handler,
    ADC1Sequence3_Handler,
    0,
    0,
    0,
    0,
    0,
    SPI2_Handler,
    SPI3_Handler,
    UART3_Handler,
    UART4_Handler,
    UART5_Handler,
    UART6_Handler,
    UART7_Handler,
    0,
    0,
    0,
    0,
    I2C2_Handler,
    I2C3_Handler,
    Timer4A_Handler,
    Timer4B_Handler,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0, // 95 Reserved
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    Timer5A_Handler,
    Timer5B_Handler,
    WideTimer0A_Handler,
    WideTimer0B_Handler,
    WideTimer1A_Handler,
    WideTimer1B_Handler,
    WideTimer2A_Handler,
    WideTimer2B_Handler,
    WideTimer3A_Handler,
    WideTimer3B_Handler,
    WideTimer4A_Handler,
    WideTimer4B_Handler,
    WideTimer5A_Handler,
    WideTimer5B_Handler,
    SystemException_Handler,
    0, //123 Reserved
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    PWM1Generator0_Handler,
    PWM1Generator1_Handler,
    PWM1Generator2_Handler,
    PWM1Generator3_Handler,
    PWM1Fault_Handler,
};

// +-----------------------------------------------------------------------------------+
// +                Implementations of Interrupt Service Routines                      +
// +-----------------------------------------------------------------------------------+

void Reset_Handler(void) {
    int *src, *dest;

    /* copying of the .data values into RAM */
    src = &_etext;
    for (dest = &_data; dest < &_edata;) {
        *dest++ = *src++;
    }

    /* initializing .bss values to zero*/
    for (dest = &__bss_start__; dest < &__bss_end__;) {
        *dest++ = 0;
    }

    main();
}

void Default_Handler(void) {
    while (1)
        ;   //does literally nothing except infinitely loop
}

void _exit() {
    while (1)
        ;
}

/*****************************************END OF FILE*********************************************/
