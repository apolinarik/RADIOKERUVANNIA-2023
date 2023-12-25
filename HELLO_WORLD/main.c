#include "stm32f10x.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"


void _NOP();
void delay_us( int Val);
void delay_ms( int Val);


int main(void)
{

  GPIO_InitTypeDef  GPIO_InitStructure;

  /* Initialize LED which connected to PC13 */
  // Enable PORTC Clock
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
  /* Configure the GPIO_LED pin */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
  GPIO_Init(GPIOC, &GPIO_InitStructure);

  GPIO_SetBits(GPIOC, GPIO_Pin_13); // Set C13 to High level ("1")
  GPIO_ResetBits(GPIOC, GPIO_Pin_13); // Set C13 to Low level ("0")

 

  while (1) {

	
    	
    	GPIO_SetBits(GPIOC, GPIO_Pin_13); // Invert C13

    	

    	delay_ms(2);
    	GPIO_ResetBits(GPIOC, GPIO_Pin_13);
			delay_ms(8);
    	/* delay */
    	//for(i=0;i<0x100000;i++);
   

  }
	
}

void _NOP(){
	__ASM("nop");
}

void delay_us( int Val)
{
	Val=Val*10;
	for( ; Val != 0; Val--)
	{
		__NOP();
	}
}

void delay_ms( int Val)
{
	Val=Val*10000;
	for( ; Val != 0; Val--)
	{
		_NOP();
	}
}
