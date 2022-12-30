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
* Description:	startup header for the TM4C Launchpad board,defines the vector table
    handlers and ISRS,also declares external variables
*/

// +-----------------------------------------------------------------------------------+
// +			        Type Definitions and Macros                                    +
// +-----------------------------------------------------------------------------------+

/*
 * Defines a macro DEFAULT that aliases the function prototype
 * to Default_Handler if the function is not defined
*/

#ifndef _STARTUP_H_
#define _STARTUP_H_


#define DEFAULT __attribute__((weak, alias("Default_Handler")))

/* Defines a type for the ISR's in the vector table */
typedef void (*element_t)(void);

/* Defines a type for the vector table */
typedef union {
    element_t isr;   //all ISRs use this type
    void *stack_top; //pointer to top of the stack
} vector_table_t;


// +-----------------------------------------------------------------------------------+
// +			        Prototypes of Basic Exception Handlers                         +
// +-----------------------------------------------------------------------------------+

//Default Handler,does nothing
void Default_Handler(void);

//System Exception Handlers

void Reset_Handler(void);
DEFAULT void NMI_Handler(void);
DEFAULT void SVC_Handler(void);
DEFAULT void DebugMonitor_Handler(void);
DEFAULT void PendSV_Handler(void);
DEFAULT void SysTick_Handler(void);

//Fault Handlers

DEFAULT void HardFault_Handler(void);
DEFAULT void MemManageFault_Handler(void);
DEFAULT void BusFault_Handler(void);
DEFAULT void UsageFault_Handler(void);

// +-----------------------------------------------------------------------------------+
// +                Prototypes of Interrupt Service Routines                           +
// +-----------------------------------------------------------------------------------+
DEFAULT void GPIOPortA_Handler(void);
DEFAULT void GPIOPortB_Handler(void);
DEFAULT void GPIOPortC_Handler(void);
DEFAULT void GPIOPortD_Handler(void);
DEFAULT void GPIOPortE_Handler(void);
DEFAULT void UART0_Handler(void);
DEFAULT void UART1_Handler(void);
DEFAULT void SPI0_Handler(void);
DEFAULT void I2C0_Handler(void);
DEFAULT void PWM0Fault_Handler(void);
DEFAULT void PWM0Generator0_Handler(void);
DEFAULT void PWM0Generator1_Handler(void);
DEFAULT void PWM0Generator2_Handler(void);
DEFAULT void QEI0_Handler(void);
DEFAULT void ADC0Sequence0_Handler(void);
DEFAULT void ADC0Sequence1_Handler(void);
DEFAULT void ADC0Sequence2_Handler(void);
DEFAULT void ADC0Sequence3_Handler(void);
DEFAULT void WatchDogTimer_Handler(void);
DEFAULT void Timer0A_Handler(void);
DEFAULT void Timer0B_Handler(void);
DEFAULT void Timer1A_Handler(void);
DEFAULT void Timer1B_Handler(void);
DEFAULT void Timer2A_Handler(void);
DEFAULT void Timer2B_Handler(void);
DEFAULT void AnalogComparator0_Handler(void);
DEFAULT void AnalogComparator1_Handler(void);
DEFAULT void SystemCtrl_Handler(void);
DEFAULT void FlashCtrl_Handler(void);
DEFAULT void GPIOPortF_Handler(void);
DEFAULT void UART2_Handler(void);
DEFAULT void SPI1_Handler(void);
DEFAULT void Timer3A_Handler(void);
DEFAULT void Timer3B_Handler(void);
DEFAULT void I2C1_Handler(void);
DEFAULT void QEI1_Handler(void);
DEFAULT void CAN0_Handler(void);
DEFAULT void CAN1_Handler(void);
DEFAULT void Hibernation_Handler(void);
DEFAULT void USB0_Handler(void);
DEFAULT void PWM0Generator3_Handler(void);
DEFAULT void UDMASoftware_Handler(void);
DEFAULT void UDMAError_Handler(void);
DEFAULT void ADC1Sequence0_Handler(void);
DEFAULT void ADC1Sequence1_Handler(void);
DEFAULT void ADC1Sequence2_Handler(void);
DEFAULT void ADC1Sequence3_Handler(void);
DEFAULT void SPI2_Handler(void);
DEFAULT void SPI3_Handler(void);
DEFAULT void UART3_Handler(void);
DEFAULT void UART4_Handler(void);
DEFAULT void UART5_Handler(void);
DEFAULT void UART6_Handler(void);
DEFAULT void UART7_Handler(void);
DEFAULT void I2C2_Handler(void);
DEFAULT void I2C3_Handler(void);
DEFAULT void Timer4A_Handler(void);
DEFAULT void Timer4B_Handler(void);
DEFAULT void Timer5A_Handler(void);
DEFAULT void Timer5B_Handler(void);
DEFAULT void WideTimer0A_Handler(void);
DEFAULT void WideTimer0B_Handler(void);
DEFAULT void WideTimer1A_Handler(void);
DEFAULT void WideTimer1B_Handler(void);
DEFAULT void WideTimer2A_Handler(void);
DEFAULT void WideTimer2B_Handler(void);
DEFAULT void WideTimer3A_Handler(void);
DEFAULT void WideTimer3B_Handler(void);
DEFAULT void WideTimer4A_Handler(void);
DEFAULT void WideTimer4B_Handler(void);
DEFAULT void WideTimer5A_Handler(void);
DEFAULT void WideTimer5B_Handler(void);
DEFAULT void SystemException_Handler(void);
DEFAULT void PWM1Generator0_Handler(void);
DEFAULT void PWM1Generator1_Handler(void);
DEFAULT void PWM1Generator2_Handler(void);
DEFAULT void PWM1Generator3_Handler(void);
DEFAULT void PWM1Fault_Handler(void);

// +-----------------------------------------------------------------------------------+
// +					External Variables declaration					               +
// +-----------------------------------------------------------------------------------+

//main() of your program
extern int main(void);

//stack pointer
extern int _stack_ptr;
//.text/code,stored in Flash
extern int _etext;
//.data,copied into RAM on boot
extern int _data;
extern int _edata;
//.bss,unitialized variables
extern int __bss_start__;
extern int __bss_end__;

/***************************************** END OF FILE *******************************************/

#endif /* _STARTUP_H_ */
