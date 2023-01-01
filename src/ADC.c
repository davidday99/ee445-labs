#include <stdint.h>
#include "ADC.h"
#include "tm4c123gh6pm.h"

#define PORTE_MASK              0x00000010
#define PORTD_MASK              0x00000008
#define PORTB_MASK              0x00000002

#define ADC_CHANNEL0_PORT_M     PORTE_MASK 
#define ADC_CHANNEL0_PIN_M      0x00000008 
#define ADC_CHANNEL1_PORT_M     PORTE_MASK 
#define ADC_CHANNEL1_PIN_M      0x00000004
#define ADC_CHANNEL2_PORT_M     PORTE_MASK 
#define ADC_CHANNEL2_PIN_M      0x00000002 
#define ADC_CHANNEL3_PORT_M     PORTE_MASK 
#define ADC_CHANNEL3_PIN_M      0x00000001 
#define ADC_CHANNEL4_PORT_M     PORTD_MASK 
#define ADC_CHANNEL4_PIN_M      0x00000008 
#define ADC_CHANNEL5_PORT_M     PORTD_MASK 
#define ADC_CHANNEL5_PIN_M      0x00000004
#define ADC_CHANNEL6_PORT_M     PORTD_MASK 
#define ADC_CHANNEL6_PIN_M      0x00000002 
#define ADC_CHANNEL7_PORT_M     PORTD_MASK 
#define ADC_CHANNEL7_PIN_M      0x00000001 
#define ADC_CHANNEL8_PORT_M     PORTE_MASK 
#define ADC_CHANNEL8_PIN_M      0x00000020 
#define ADC_CHANNEL9_PORT_M     PORTE_MASK 
#define ADC_CHANNEL9_PIN_M      0x00000010 
#define ADC_CHANNEL10_PORT_M    PORTB_MASK 
#define ADC_CHANNEL10_PIN_M     0x00000010 
#define ADC_CHANNEL11_PORT_M    PORTB_MASK 
#define ADC_CHANNEL11_PIN_M     0x00000020 

static int Init_Channel(enum ADC_Channel channelNum);

int ADC_Open(uint32_t channelNum) {
    SYSCTL_RCGCADC_R |= 0x0001;     // 0) activate ADC0 
    if (!Init_Channel(channelNum))
        return 0;
    while((SYSCTL_PRADC_R&0x0001) != 0x0001)
        ;
    ADC0_PC_R &= ~0xF;              // 7) clear max sample rate field
    ADC0_PC_R |= 0x1;               //    configure for 125K samples/sec
    ADC0_SSPRI_R = 0x0123;          // 8) Sequencer 3 is highest priority
    ADC0_ACTSS_R &= ~0x0008;        // 9) disable sample sequencer 3
    ADC0_EMUX_R &= ~0xF000;         // 10) seq3 is software trigger
    ADC0_SSMUX3_R &= ~0x000F;       // 11) clear SS3 field
    ADC0_SSMUX3_R += channelNum;             //    set channel
    ADC0_SSCTL3_R = 0x0006;         // 12) no TS0 D0, yes IE0 END0
    ADC0_IM_R &= ~0x0008;           // 13) disable SS3 interrupts
    ADC0_ACTSS_R |= 0x0008;         // 14) enable sample sequencer 3
    return 1;
}

uint16_t ADC_In(void ) {
  uint32_t result;
  ADC0_PSSI_R = 0x0008;            // 1) initiate SS3
  while((ADC0_RIS_R&0x08)==0)      // 2) wait for conversion done
      ;   
    // if you have an A0-A3 revision number, you need to add an 8 usec wait here
  result = ADC0_SSFIFO3_R & 0xFFF;   // 3) read result
  ADC0_ISC_R = 0x0008;             // 4) acknowledge completion
  return result;
}

static int Init_Channel(enum ADC_Channel channelNum) {
    volatile uint32_t *portDirReg;
    volatile uint32_t *portAfselReg;
    volatile uint32_t *portDenReg;
    volatile uint32_t *portAmselReg;
    uint32_t channelPort;
    uint32_t channelPin;

    switch (channelNum) {
        case CHANNEL0:
        case CHANNEL1:
        case CHANNEL2:
        case CHANNEL3:
        case CHANNEL8:
        case CHANNEL9:
            portDirReg = &GPIO_PORTE_DIR_R;
            portAfselReg = &GPIO_PORTE_AFSEL_R;
            portDenReg = &GPIO_PORTE_DEN_R;
            portAmselReg = &GPIO_PORTE_AMSEL_R;
            break;
        case CHANNEL4:
        case CHANNEL5:
        case CHANNEL6:
        case CHANNEL7:
            portDirReg = &GPIO_PORTD_DIR_R;
            portAfselReg = &GPIO_PORTD_AFSEL_R;
            portDenReg = &GPIO_PORTD_DEN_R;
            portAmselReg = &GPIO_PORTD_AMSEL_R;
            break;
        case CHANNEL10:
        case CHANNEL11:
            portDirReg = &GPIO_PORTB_DIR_R;
            portAfselReg = &GPIO_PORTB_AFSEL_R;
            portDenReg = &GPIO_PORTB_DEN_R;
            portAmselReg = &GPIO_PORTB_AMSEL_R;
            break;
        default:
            return 0;
    } 
    
    switch (channelNum) {
        case CHANNEL0:
            channelPort = ADC_CHANNEL0_PORT_M;
            channelPin = ADC_CHANNEL0_PIN_M;
            break;
        case CHANNEL1:
            channelPort = ADC_CHANNEL1_PORT_M;
            channelPin = ADC_CHANNEL1_PIN_M;
            break;
        case CHANNEL2:
            channelPort = ADC_CHANNEL2_PORT_M;
            channelPin = ADC_CHANNEL2_PIN_M;
            break;
        case CHANNEL3:
            channelPort = ADC_CHANNEL3_PORT_M;
            channelPin = ADC_CHANNEL3_PIN_M;
            break;
        case CHANNEL4:
            channelPort = ADC_CHANNEL4_PORT_M;
            channelPin = ADC_CHANNEL4_PIN_M;
            break;
        case CHANNEL5:
            channelPort = ADC_CHANNEL5_PORT_M;
            channelPin = ADC_CHANNEL5_PIN_M;
            break;
        case CHANNEL6:
            channelPort = ADC_CHANNEL6_PORT_M;
            channelPin = ADC_CHANNEL6_PIN_M;
            break;
        case CHANNEL7:
            channelPort = ADC_CHANNEL7_PORT_M;
            channelPin = ADC_CHANNEL7_PIN_M;
            break;
        case CHANNEL8:
            channelPort = ADC_CHANNEL8_PORT_M;
            channelPin = ADC_CHANNEL8_PIN_M;
            break;
        case CHANNEL9:
            channelPort = ADC_CHANNEL9_PORT_M;
            channelPin = ADC_CHANNEL9_PIN_M;
            break;
        case CHANNEL10:
            channelPort = ADC_CHANNEL10_PORT_M;
            channelPin = ADC_CHANNEL10_PIN_M;
            break;
        case CHANNEL11:
            channelPort = ADC_CHANNEL11_PORT_M;
            channelPin = ADC_CHANNEL11_PIN_M;
            break;
        default:
            return 0;
    } 

    SYSCTL_RCGCGPIO_R |= channelPort;      
    while((SYSCTL_PRGPIO_R & channelPort) != channelPort)
        ;
    *portDirReg |= channelPin; 
    *portAfselReg |= channelPin; 
    *portDenReg |= channelPin; 
    *portAmselReg |= channelPin; 
    return 1; 
}

