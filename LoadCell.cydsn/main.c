/* ========================================
 *
 * Copyright YOUR COMPANY, THE YEAR
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF your company.
 *
 * ========================================
*/
#include "project.h"
#include "stdio.h"

int loop_counter = 0;
char displayStr[6];

int main(void)
{
    CyGlobalIntEnable; /* Enable global interrupts. */
    
    PGA_1_Start();
    
    LCD_Start();
    LCD_IsReady();
    
    LCD_ClearDisplay();
    LCD_Position(0,0);
    LCD_PrintString("Load Cell Lab");
    CyDelay(2000);
    
    SW_Tx_UART_1_Start();
    SW_Tx_UART_1_PutString("hi");
    SW_Tx_UART_1_PutCRLF();
    SW_Tx_UART_1_PutString("hi2");
    SW_Tx_UART_1_PutCRLF();
    SW_Tx_UART_1_PutString("hi3");
   
    for(;;)
    {
        loop_counter = loop_counter + 1;
        
        if (loop_counter > 100) {
            LCD_Position(1,0);
            sprintf(displayStr, "%d  ", loop_counter);
            LCD_PrintString(displayStr);
            loop_counter = 0;
        }
    }
}

/* [] END OF FILE */
