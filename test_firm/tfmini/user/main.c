#include "stm32f4xx.h"
#include "stm32f4xx_it.h"
#include "string.h"
/*
https://stm32f4-discovery.net/2017/07/stm32-tutorial-efficiently-receive-uart-data-using-dma/
GPS STM32 for quadcopter.
*/
void tfmini_init(void);
void tfmini_parsing(void);

void delay_us(uint32_t period){
	
  	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
  	TIM2->PSC = 83;		// clk = SystemCoreClock /2/(PSC+1) = 1MHz
  	TIM2->ARR = period-1;
  	TIM2->CNT = 0;
  	TIM2->EGR = 1;		// update registers;
  	TIM2->SR  = 0;		// clear overflow flag
  	TIM2->CR1 = 1;		// enable Timer6
  	while (!TIM2->SR);
  	TIM6->CR2 = 0;		// stop Timer6
  	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, DISABLE);
}
//delay 0.1ms
void delay_01ms(uint16_t period){
	
  	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6, ENABLE);
  	TIM6->PSC = 8399;		// clk = SystemCoreClock /2 /(PSC+1) = 10KHz
  	TIM6->ARR = period-1;
  	TIM6->CNT = 0;
  	TIM6->EGR = 1;		// update registers;
  	TIM6->SR  = 0;		// clear overflow flag
  	TIM6->CR1 = 1;		// enable Timer6
  	while (!TIM6->SR);
  	TIM6->CR1 = 0;		// stop Timer6
  	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6, DISABLE);
}

/* Receive buffer for DMA */
// 725
#define DMA_RX_BUFFER_SIZE          50
uint8_t DMA_RX_Buffer[DMA_RX_BUFFER_SIZE];
DMA_InitTypeDef DMA_InitStruct;
NVIC_InitTypeDef NVIC_InitStruct;
GPIO_InitTypeDef  GPIO_InitStructure;
GPIO_InitTypeDef GPIO_InitStruct;
USART_InitTypeDef USART_InitStruct;
NVIC_InitTypeDef NVIC_InitStructure;
uint16_t tfmini_distance, tfmini_strength;
int main(void)
{
		RCC->AHB1ENR |= RCC_AHB1Periph_GPIOD;
	
		/* Configure PD12, PD13 in output pushpull mode */
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15 ;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
		GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
		GPIO_Init(GPIOD, &GPIO_InitStructure);
	  tfmini_init();
	
  while (1)
  {

  }
}
//2 ways
void UARTmain_Init()
{
 
}
void USART3_IRQHandler(void)
{
	  if (USART3->SR & USART_FLAG_IDLE) {         /* We want IDLE flag only */
        DMA_Cmd(DMA1_Stream1, DISABLE);
        /* This part is important */
        /* Clear IDLE flag by reading status register first */
        /* And follow by reading data register */
        volatile uint32_t tmp;                  /* Must be volatile to prevent optimizations */
        tmp = USART3->SR;                       /* Read status register */
        tmp = USART3->DR;                       /* Read data register */
      //  DMA1_Stream1->CR &= ~DMA_SxCR_EN;       /* Disabling DMA will force transfer complete interrupt if enabled */
		//	  idle_cnt++;
					//	DMA_Cmd(DMA1_Stream1, DISABLE);
      
		//	DMA1->HIFCR = DMA_FLAG_DMEIF1 | DMA_FLAG_FEIF1 | DMA_FLAG_HTIF1 | DMA_FLAG_TCIF1 | DMA_FLAG_TEIF1;
	//		DMA_Init(DMA1_Stream1, &DMA_InitStruct);
			
				DMA1_Stream1->M0AR = (uint32_t)DMA_RX_Buffer;   /* Set memory address for DMA again */
        DMA1_Stream1->NDTR = DMA_RX_BUFFER_SIZE;    /* Set number of bytes to receive */
			  DMA_Cmd(DMA1_Stream1, ENABLE);
	//		  DMA1_Stream1->M0AR = (uint32_t)DMA_RX_Buffer;   /* Set memory address for DMA again */
	     tfmini_parsing();
			  
    }	
}

void tfmini_parsing(void) {
	int m;
	if (DMA_RX_Buffer[0]==0x59) {
		tfmini_distance = (DMA_RX_Buffer[3]>>8)+DMA_RX_Buffer[2];
		tfmini_strength = (DMA_RX_Buffer[5]>>8)+DMA_RX_Buffer[4];
	}
	}


void tfmini_init(void) {
	uint8_t kk;
	  RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1, ENABLE);
	
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_Init(GPIOB, &GPIO_InitStruct);
	
	  /*PD5 to PL2303 RX*/
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource10, GPIO_AF_USART3);//UART Tx pins 
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource11, GPIO_AF_USART3);//UART Rx pins
	
    //USART2 configuration
    USART_InitStruct.USART_BaudRate = 115200;
    USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStruct.USART_Mode = USART_Mode_Tx|USART_Mode_Rx;
    USART_InitStruct.USART_Parity = USART_Parity_No;
    USART_InitStruct.USART_StopBits = USART_StopBits_1;
    USART_InitStruct.USART_WordLength = USART_WordLength_8b;
    USART_Init(USART3, &USART_InitStruct);
		
		NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
		NVIC_Init(&NVIC_InitStructure);
//		USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);
		USART_ITConfig(USART3, USART_IT_IDLE, ENABLE);
		USART_Cmd(USART3, ENABLE);
	  USART_DMACmd(USART3, USART_DMAReq_Rx, ENABLE);
	
	  /* Configure DMA for USART RX, DMA1, Stream1, Channel4 */
    DMA_StructInit(&DMA_InitStruct);
    DMA_InitStruct.DMA_Channel = DMA_Channel_4;
    DMA_InitStruct.DMA_Memory0BaseAddr = (uint32_t)DMA_RX_Buffer;
    DMA_InitStruct.DMA_BufferSize = DMA_RX_BUFFER_SIZE;
    DMA_InitStruct.DMA_PeripheralBaseAddr = (uint32_t)&USART3->DR;
    DMA_InitStruct.DMA_DIR = DMA_DIR_PeripheralToMemory;
    DMA_InitStruct.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
    DMA_InitStruct.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
		DMA_InitStruct.DMA_Mode = DMA_Mode_Circular;
		DMA_InitStruct.DMA_Priority = DMA_Priority_High;
    DMA_InitStruct.DMA_MemoryInc = DMA_MemoryInc_Enable;
    DMA_InitStruct.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    DMA_Init(DMA1_Stream1, &DMA_InitStruct);
		DMA_Cmd(DMA1_Stream1, ENABLE);
// send setting
uint8_t command[] = {0x42, 0x57,0x02,0x00,0x00,0x00,0x01,0x06};
			for (kk=0;kk<8;kk++) {
			while(USART_GetFlagStatus(USART3, USART_FLAG_TXE) == RESET) {
				USART_SendData(USART3,command[kk]);
			}
			}
}
