//********************************************************************
//*                    EEE2046F C template                           *
//*==================================================================*
//* WRITTEN BY: Jesse Arendse   	                 		         *
//* DATE CREATED: 07/04/2023                                         *
//* MODIFIED:   Athenkosi Matyumza                                   *
//*==================================================================*
//* PROGRAMMED IN: Visual Studio Code                                *
//* TARGET:        STM32F0                                           *
//*==================================================================*
//* DESCRIPTION: Practical 4                                         *
//*                                                                  *
//********************************************************************
// INCLUDE FILES
//====================================================================
#include "stm32f0xx.h"
#include <lcd_stm32f0.c>
//====================================================================
// GLOBAL CONSTANTS
//====================================================================
#define DELAY1 700
#define DELAY2 500
#define TRUE 1
#define FALSE 0

//====================================================================
// TYPE MODIFIER
//====================================================================
typedef uint8_t flag_t;

//====================================================================
// GLOBAL VARIABLES
//====================================================================
int bitpattern1 = 0;
int bitpattern2 = 255;
int bitpattern3 = 170;
flag_t startFlag = FALSE, firstFlag = FALSE, secondFlag = FALSE, thirdFlag = FALSE;

//====================================================================
// FUNCTION DECLARATIONS
//====================================================================
void greetings(void);
void initGPIO(void);
void Delay(void);
void firstDisplay(void);
void secondDisplay(void);
void thirdDisplay(void);
void checkPB(void);
void display(void);

//====================================================================
// MAIN FUNCTION
//====================================================================
int main(void)
{
    greetings();
    initGPIO();

    while (1)
    {
        checkPB();
        display();
    }

} // End of main

//====================================================================
// FUNCTION DEFINITIONS
//====================================================================
void greetings()
{
    init_LCD();
    lcd_command(CLEAR);
    lcd_putstring("Prac 4_Demo! :)");
}

void initGPIO(void)
{
    RCC->AHBENR |= RCC_AHBENR_GPIOAEN | RCC_AHBENR_GPIOBEN; // Enable Port A (LED) & B (Switches)
    GPIOB->MODER |= 0x00505555;                             // Set mode of LEDs to Output
    GPIOB->ODR = 0;                                         // Set LEDs off

    // Default mode for port A is Input
    GPIOA->PUPDR |= 0x55; // Setting to pull up because one end is connected to ground
}

void Delay(void)
{
    for (volatile int i = 0; i <= DELAY1; ++i)
        for (volatile int j = 0; j <= DELAY2; ++j)
        {
        }
}

void firstDisplay(void)
{
    GPIOB->ODR = bitpattern1; // Set B0 on
    Delay();
    GPIOB->ODR = bitpattern2; // Set B0 on
    Delay();
    GPIOB->ODR = bitpattern3; // Set B0 on
    Delay();
}

void secondDisplay(void)
{
    for (int i = 0; i < 8; ++i)
    {
        GPIOB->ODR = pow(2, i); // Set LEDs on
        Delay();
    }
}

void thirdDisplay(void)
{
    for (int i = 0; i < 256; ++i)
    {
        GPIOB->ODR = i; // Set LEDs on
        Delay();
    }
}

void checkPB(void)
{
    if (GPIO_IDR_0 & ~(GPIOA->IDR))
    {
        startFlag = TRUE;
        firstFlag = FALSE;
        secondFlag = FALSE;
        thirdFlag = FALSE;
    }
    else if (GPIO_IDR_1 & ~(GPIOA->IDR))
    {
        startFlag = TRUE;
        firstFlag = TRUE;
        secondFlag = FALSE;
        thirdFlag = FALSE;
    }
    else if (GPIO_IDR_2 & ~(GPIOA->IDR))
    {
        startFlag = TRUE;
        firstFlag = FALSE;
        secondFlag = TRUE;
        thirdFlag = FALSE;
    }
    else if (GPIO_IDR_3 & ~(GPIOA->IDR))
    {
        startFlag = TRUE;
        firstFlag = FALSE;
        secondFlag = FALSE;
        thirdFlag = TRUE;
    }
}

void display(void)
{
    if (startFlag && !firstFlag && !secondFlag && !thirdFlag)
    {
        GPIOB->ODR = 1;
    }

    if (firstFlag && startFlag)
    {
        firstDisplay();
    }

    if (secondFlag && startFlag)
    {
        secondDisplay();
    }

    if (thirdFlag && startFlag)
    {
        thirdDisplay();
    }
}

//********************************************************************
// END OF PROGRAM
//********************************************************************
