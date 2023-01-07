#include <stdint.h>
#include "ADC.h"
#include "tm4c123gh6pm.h"
#include "DebugTools.h"

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

#define NVIC_EN0_INT14 0x4000 

static unsigned char ConversionInProgress;
static uint16_t *SampleBuf;
static uint32_t SampleBufIndex;
static uint32_t NumberOfSamples;

static void StartTimer(uint32_t freq);
static int InitChannel(enum ADC_Channel channelNum);

static int InitADCTimerTriggeredSeq0(enum ADC_Channel channelNum) {
    SYSCTL_RCGCADC_R |= 0x0001;     // 0) activate ADC0 
    if (!InitChannel(channelNum))
        return 0;
    while((SYSCTL_PRADC_R&0x0001) != 0x0001)
        ;
    ADC0_PC_R &= ~0xF;              // 7) clear max sample rate field
    ADC0_PC_R |= 0x1;               //    configure for 125K samples/sec
    ADC0_ACTSS_R &= ~0x0001;        // 9) disable sample sequencer 0
    ADC0_EMUX_R = (ADC0_EMUX_R & ~0xF) | 0x5;         // 10) seq0 is timer triggered
    ADC0_SSMUX0_R &= ~0x000F;       // 11) clear SS0 field
    ADC0_SSMUX0_R += channelNum;    //    set channel
    ADC0_SSCTL0_R = 0x0006;         // 12) no TS0 D0, yes IE0 END0
    ADC0_IM_R |= 0x0001;            // 13) enable SS0 interrupts
    ADC0_ACTSS_R |= 1;              // 14) enable SS0
    NVIC_EN0_R |= NVIC_EN0_INT14;
    return 1;
}

int ADC_Open(uint32_t channelNum) {
    SYSCTL_RCGCADC_R |= 0x0001;     // 0) activate ADC0 
    if (!InitChannel(channelNum))
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

uint16_t ADC_In(void) {
  uint32_t result;
  ADC0_PSSI_R = 0x0008;            // 1) initiate SS3
  while((ADC0_RIS_R&0x08)==0)      // 2) wait for conversion done
      ;   
  // if you have an A0-A3 revision number, you need to add an 8 usec wait here
  result = ADC0_SSFIFO3_R & 0xFFF;   // 3) read result
  ADC0_ISC_R = 0x0008;             // 4) acknowledge completion
  return result;
}

int ADC_Collect(uint32_t channelNum, uint32_t fs,
        uint16_t buffer[], uint32_t numberOfSamples) {
    if (InitADCTimerTriggeredSeq0(channelNum) || fs == 0)
        return 0;
    SampleBuf = buffer;
    SampleBufIndex = 0;
    NumberOfSamples = numberOfSamples;
    ConversionInProgress = 1;
    StartTimer(fs); 
    return 1;
}

int ADC_Status(void) {
    return ConversionInProgress;
}

// *************** ADC0Sequence0_Handler ********************
// ISR that runs each time timer trigger occurs 
// during ADC_Collect.
void ADC0Sequence0_Handler(void) {
    ADC0_ISC_R = 1;
    if (SampleBufIndex >= NumberOfSamples) {
        ADC0_IM_R &= ~1;
        TIMER2_CTL_R &= ~1;
        NVIC_EN0_R &= ~NVIC_EN0_INT14;
        ConversionInProgress = 0;
    } else {
        SampleBuf[SampleBufIndex++] = ADC0_SSFIFO0_R & 0xFFF;
    }
}

static int InitChannel(enum ADC_Channel channelNum) {
    volatile uint32_t *portDirReg;
    volatile uint32_t *portAfselReg;
    volatile uint32_t *portDenReg;
    volatile uint32_t *portAmselReg;
    uint32_t channelPort;
    uint32_t channelPin;

    switch (channelNum) {
        case ADC_CHANNEL0:
        case ADC_CHANNEL1:
        case ADC_CHANNEL2:
        case ADC_CHANNEL3:
        case ADC_CHANNEL8:
        case ADC_CHANNEL9:
            portDirReg = &GPIO_PORTE_DIR_R;
            portAfselReg = &GPIO_PORTE_AFSEL_R;
            portDenReg = &GPIO_PORTE_DEN_R;
            portAmselReg = &GPIO_PORTE_AMSEL_R;
            break;
        case ADC_CHANNEL4:
        case ADC_CHANNEL5:
        case ADC_CHANNEL6:
        case ADC_CHANNEL7:
            portDirReg = &GPIO_PORTD_DIR_R;
            portAfselReg = &GPIO_PORTD_AFSEL_R;
            portDenReg = &GPIO_PORTD_DEN_R;
            portAmselReg = &GPIO_PORTD_AMSEL_R;
            break;
        case ADC_CHANNEL10:
        case ADC_CHANNEL11:
            portDirReg = &GPIO_PORTB_DIR_R;
            portAfselReg = &GPIO_PORTB_AFSEL_R;
            portDenReg = &GPIO_PORTB_DEN_R;
            portAmselReg = &GPIO_PORTB_AMSEL_R;
            break;
        default:
            return 0;
    } 
    
    switch (channelNum) {
        case ADC_CHANNEL0:
            channelPort = ADC_CHANNEL0_PORT_M;
            channelPin = ADC_CHANNEL0_PIN_M;
            break;
        case ADC_CHANNEL1:
            channelPort = ADC_CHANNEL1_PORT_M;
            channelPin = ADC_CHANNEL1_PIN_M;
            break;
        case ADC_CHANNEL2:
            channelPort = ADC_CHANNEL2_PORT_M;
            channelPin = ADC_CHANNEL2_PIN_M;
            break;
        case ADC_CHANNEL3:
            channelPort = ADC_CHANNEL3_PORT_M;
            channelPin = ADC_CHANNEL3_PIN_M;
            break;
        case ADC_CHANNEL4:
            channelPort = ADC_CHANNEL4_PORT_M;
            channelPin = ADC_CHANNEL4_PIN_M;
            break;
        case ADC_CHANNEL5:
            channelPort = ADC_CHANNEL5_PORT_M;
            channelPin = ADC_CHANNEL5_PIN_M;
            break;
        case ADC_CHANNEL6:
            channelPort = ADC_CHANNEL6_PORT_M;
            channelPin = ADC_CHANNEL6_PIN_M;
            break;
        case ADC_CHANNEL7:
            channelPort = ADC_CHANNEL7_PORT_M;
            channelPin = ADC_CHANNEL7_PIN_M;
            break;
        case ADC_CHANNEL8:
            channelPort = ADC_CHANNEL8_PORT_M;
            channelPin = ADC_CHANNEL8_PIN_M;
            break;
        case ADC_CHANNEL9:
            channelPort = ADC_CHANNEL9_PORT_M;
            channelPin = ADC_CHANNEL9_PIN_M;
            break;
        case ADC_CHANNEL10:
            channelPort = ADC_CHANNEL10_PORT_M;
            channelPin = ADC_CHANNEL10_PIN_M;
            break;
        case ADC_CHANNEL11:
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

static void StartTimer(uint32_t freq) {
    SYSCTL_RCGCTIMER_R |= SYSCTL_RCGCTIMER_R2;
    TIMER2_CTL_R &= ~1;  // disable timer A
    TIMER2_CTL_R |= 0x20;  // enable ADC trigger
    TIMER2_CFG_R = (TIMER2_CFG_R & ~7);  // use 32-bit timer
    TIMER2_TAMR_R = 2;  // enable periodic timer mode
    TIMER2_TAILR_R = 80000000 / freq;
    TIMER2_CTL_R |= 3;
}

