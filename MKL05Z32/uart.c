 

#include "uart.h"

static uint32_t osr = 15;
static uint16_t sbr;
static uint8_t temp;

void uart_init(uint32_t baud_rate)
{
  UART_ERROR_CODE_t error = UART_SUCCES;
  /* UART0 appended to the clock (UART0 Clock Gate Control) */
  SIM->SCGC4 |= SIM_SCGC4_UART0_MASK;
  /* Port B appended to the clock (Port B Clock Gate Control) */
  SIM->SCGC5 |= SIM_SCGC5_PORTB_MASK;
  /* block the transmitter and receiver */
  UART0->C2 &= ~(UART0_C2_TE_MASK | UART0_C2_RE_MASK);

  SIM->SOPT2 |= SIM_SOPT2_UART0SRC(MCGFLLCLK);
  /* Alternative 1(GPIO) RX(PTB1) */
  PORTB->PCR[1] &= ~PORT_PCR_MUX_MASK;
  PORTB->PCR[1] = PORT_PCR_ISF_MASK|PORT_PCR_MUX(0x2);
  /* Alternative 1(GPIO) TX(PTB2) */
  PORTB->PCR[2] &= ~PORT_PCR_MUX_MASK;
  PORTB->PCR[2] = PORT_PCR_ISF_MASK|PORT_PCR_MUX(0x2);

  /* Baud rate calculation */
  sbr = (uint16_t)((SystemCoreClock)/(baud_rate * (osr+1)));

  /* Save off the current value of the uartx_BDH except for the SBR field */
  temp = UART0->BDH & ~(UART0_BDH_SBR(0x1F));

  /* Set Baud rate */
  UART0->BDH = temp | UART0_BDH_SBR(((sbr & 0x1F00)>> 8));
  UART0->BDL = (uint8_t)(sbr & UART0_BDL_SBR_MASK);
  UART0->BDH &= ~UART0_BDH_SBNS_MASK;
  UART0->C4 |= UART0_C4_OSR(osr);

  
  UART0->C1 &= ~UART0_C1_M_MASK;
  UART0->C1 &= ~UART0_C1_PE_MASK;

  /* enable the transmitter and receiver */
  UART0->C2 |= (UART0_C2_TE_MASK | UART0_C2_RE_MASK);

  /* interrupts settings */
  NVIC_EnableIRQ(UART0_IRQn);
  NVIC_SetPriority(UART0_IRQn, 0);
  NVIC_ClearPendingIRQ(UART0_IRQn);
}

void uart_sendCh(uint8_t data){
  while(!(UART0->S1 & UART0_S1_TDRE_MASK) && !(UART0->S1&UART0_S1_TC_MASK));
  UART0->D = data;
}

void uart_sendStr(char* str){
  while(*str)
  {
    uart_sendCh(*str);
    ++str;    
  }
} 

void uart_getChar(uint8_t *data){
  while (!(UART0->S1 & UART0_S1_RDRF_MASK));
  *data = UART0->D;
}