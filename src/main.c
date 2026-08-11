#include <stdint.h>

typedef struct {
  volatile uint32_t GPIO_MODER;
  volatile uint32_t GPIO_OTYPER;
  volatile uint32_t GPIO_OSPEEDR;
  volatile uint32_t GPIO_PUPDR;
  volatile uint32_t GPIOx_IDR; 
  volatile uint32_t GPIOx_ODR;
  volatile uint32_t GPIOx_BSRR;
} GPIO_Struct;


//Casting to a pointer of type GPIO_Struct
#define GPIOA ((GPIO_Struct *)0x40020000)

#define RCC 0x40023800
#define RCCAHB1 (*(volatile uint32_t *)(RCC+0x00000030))

void delay_timer(uint32_t count) {
  for (volatile uint32_t i = 0; i < count; i++) {
    //stop compiling c for a second, because I'm writing assembly code.
    //It's volatile, so because it's useless, compiler optimization can remove it, but volatile says to keep it
    //nop simply means no operation code
    __asm__ volatile ("nop");
  }
}


int main (void) {
  RCCAHB1 |= (1 << 0);

  GPIOA->GPIO_MODER &= ~(3 << 10);
  GPIOA->GPIO_MODER |= (1 << 10);

  GPIOA->GPIO_OTYPER &= ~(1 << 5);

  GPIOA->GPIO_OSPEEDR &= ~(3 << 10);

  GPIOA->GPIO_PUPDR &= ~(3 << 10);

  while (1) {
    GPIOA->GPIOx_BSRR |= (1 << 5);

    delay_timer(500000);

    GPIOA->GPIOx_BSRR |= (1 << 21);

    delay_timer(500000);
  }
}