//********************************************************************
//*                    EEE2046F C template                           *
//*==================================================================*
//* WRITTEN BY: Jesse Arendse   	                 		         *
//* DATE CREATED: 07/04/2023                                         *
//* MODIFIED: Athenkosi Matyumza                                     *
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
#include <stdio.h>
//====================================================================
// GLOBAL CONSTANTS
//====================================================================
#define TRUE 1
#define FALSE 0

//====================================================================
// TYPE MODIFIER
//====================================================================
typedef uint8_t flag_t;

//====================================================================
// GLOBAL VARIABLES
//====================================================================
flag_t startFlag = FALSE, lapFlag = FALSE, stopFlag = FALSE, resetFlag = TRUE;
uint8_t minutes = 0, seconds = 0, hundredths = 0;

//====================================================================
// FUNCTION DECLARATIONS
//====================================================================
void welcome(void);                                                                             
void initGPIO(void);                                                                          
void startWatch(void);                                                                          
void lapTimer(void);                                                                            
void stopTimer(void);                                                                             
void resetTimer(void);                                                                            
void checkPB(void);                                                                               
void display(void);                                                                                                            
void initTIM14(void);
void TIM14_IRQHandler(void);                                                                     
void convert2BCDASCII(const uint8_t min, const uint8_t sec, const uint8_t hund, char *resultPtr); // WRITE DEFINITION
//====================================================================
// MAIN FUNCTION
//====================================================================
int main(void)
{
    // welcome();
    initGPIO();
    initTIM14();

    GPIOA->ODR = 0b0000000000000000;
    while (1)
    {
        // checkPB();
        // display();
    }

} // End of main

//====================================================================
// FUNCTION DEFINITIONS
//====================================================================
void welcome()
{
    init_LCD();
    lcd_command(CLEAR);
    lcd_putstring("Stop Watch");
    lcd_command(CURSOR_HOME); // ensure the cursor is in line one
    lcd_command(LINE_TWO);    // Moves the cursor to line two
    lcd_putstring("Press SW0...");
}

void initGPIO(void)
{
    RCC->AHBENR |= RCC_AHBENR_GPIOAEN | RCC_AHBENR_GPIOBEN; // Enable Port A (LED) & B (Switches)
    GPIOB->MODER |= 0x00505555;                             // Set mode of LEDs to Output
    GPIOB->ODR = 0;                                         // Set LEDs off

    // GPIOA->PUPDR |= 0x55; // Setting to pull up because one end is connected to ground
    GPIOA->MODER |= 0b00000000000000000000000100000000; // Set mode of PA4 to Output
}

void initTIM14(void)
{
    // Initialising the clock registers to the calculated values
    RCC->APB1ENR |= RCC_APB1ENR_TIM14EN;

    // Approximate the fastest delay closest to a hundredth of a second
    TIM14->ARR = 2799; 
    TIM14->PSC = 3;     

    TIM14->CR1 &= ~TIM_CR1_CEN;
    TIM14->DIER |= TIM_DIER_UIE; // Enables interrupt
}

void TIM14_IRQHandler(void)
{
    // Update and display the time
    char* result;
    convert2BCDASCII(minutes, seconds, hundredths, result);

    // Increment seconds if 99 hundredths of a second have passed
    if (hundredths >= 99)
    {
        seconds++;
    }

    // Increment minutes and reset seconds if 60 seconds have passed
    if (seconds >= 59)
    {
        minutes++;
        seconds = 0;
    }

    // Reset minutes and seconds if 60 minutes have passed
    if (minutes >= 59)
    {
        minutes = 0;
        seconds = 0;
    }

    // Increment hundredths of a second and wrap around at 100
    hundredths = (hundredths + 1) % 100;

    // Reset the interrupt flag to allow the ISR to be called again
    TIM14->SR &= ~TIM_SR_UIF;
}

void lapTimer(void)
{
    GPIOB->ODR = 2; // Set D1 on
    TIM14->CR1 &= ~TIM_CR1_CEN;
}

void stopTimer(void)
{
    GPIOB->ODR = 4; // Set D2 on
    TIM14->CR1 &= ~TIM_CR1_CEN; // Disables clock
    minutes = 0; // resets back to zero
    seconds = 0; // resets back to zero
    hundredths = 0; // resets back to zero
}

void startWatch(void)
{
    // Indicates on the diode that button start is clicked
    GPIOB->ODR = 1;
}

void resetTimer(void)
{
    GPIOB->ODR = 8; // Set D3 on
    minutes = 0;    // resets back to zero
    seconds = 0;    // resets back to zero
    hundredths = 0; // resets back to zero
}

void checkPB(void)
{
    if (GPIO_IDR_0 & ~(GPIOA->IDR))
    {
        lcd_command(CLEAR);
        TIM14->CR1 |= TIM_CR1_CEN;  // Enable Timer once the START button is pressed
        NVIC_EnableIRQ(TIM14_IRQn); // Enables the interrupt to run
        startFlag = TRUE;
        lapFlag = FALSE;
        stopFlag = FALSE;
        resetFlag = FALSE;
    }
    else if (GPIO_IDR_1 & ~(GPIOA->IDR))
    {
        startFlag = TRUE;
        lapFlag = TRUE;
        stopFlag = FALSE;
        resetFlag = FALSE;
    }
    else if (GPIO_IDR_2 & ~(GPIOA->IDR))
    {
        startFlag = TRUE;
        lapFlag = FALSE;
        stopFlag = TRUE;
        resetFlag = FALSE;
    }
    else if (GPIO_IDR_3 & ~(GPIOA->IDR))
    {
        TIM14->CR1 &= ~TIM_CR1_CEN; // Disables clock
        welcome(); // Resets display back to the starting state when the program first runs
        startFlag = FALSE;
        lapFlag = FALSE;
        stopFlag = FALSE;
        resetFlag = TRUE;
    }
}

void display(void)
{
    if (startFlag && !lapFlag && !stopFlag && !resetFlag)
    {
        startWatch();
    }

    if (lapFlag && startFlag)
    {
        lapTimer();
    }

    if (stopFlag && startFlag)
    {
        stopTimer();
    }

    if (resetFlag)
    {
        resetTimer();
    }
}

void convert2BCDASCII(const uint8_t min, const uint8_t sec, const uint8_t hund, char *resultPtr)
{
    // Move the cursor to the home position on the first line
    lcd_command(CURSOR_HOME); // ensure the cursor is in line one

    // Create an array to hold the formatted time string
    char lineTwo[20];

    // Format the time string and store it in the lineTwo array
    sprintf(lineTwo, "%02d:%02d.%02d", min, sec, hund);
    lcd_putstring("Time");

    // Move the cursor to the beginning of the second line
    lcd_command(LINE_TWO);
    lcd_putstring(lineTwo);
}

//********************************************************************
// END OF PROGRAM
//********************************************************************