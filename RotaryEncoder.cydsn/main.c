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
int count = 0;
int count_last = 0;

int count_manual = 0;
_Bool rotary1 = 1;
char displayStr[6] = "";
char displayStr2[6] = "";

_Bool sample_n;
_Bool sample_n_minus_1;
_Bool sample_n_minus_2;

_Bool sample_m;
_Bool sample_m_minus_1;
_Bool sample_m_minus_2;


_Bool sample_out;


//CY_ISR ( sample_interrupt_handler ) {
//    sample_n_minus_2 = sample_n_minus_1;
//    sample_n_minus_1 = sample_n;
//    sample_n = P3_5_Read();
//    
//    sample_m_minus_2 = sample_m_minus_1;
//    sample_m_minus_1 = sample_m;
//    sample_m = P3_7_Read();
//    
//    if (sample_n == 0 && sample_n_minus_1 == 0 && sample_n_minus_2 == 0) {
//        sample_out = 0;
//        if (P3_7_Read() == 1) {
//            // we record a turn
//            count_manual = count_manual - 1;
//            LCD_Position(1,12);
//            sprintf(displayStr2, "%d   ", count_manual);
//            LCD_PrintString(displayStr2);
//            // we ignore all the zeros after this?
//        }
//    }
//    if (sample_m == 0 && sample_m_minus_1 == 0 && sample_m_minus_2 == 0) {
//        sample_out = 0;
//        if (P3_5_Read() == 1) {
//            // we record a turn
//            count_manual = count_manual + 1;
//            LCD_Position(1,12);
//            sprintf(displayStr2, "%d   ", count_manual);
//            LCD_PrintString(displayStr2);
//            // we ignore all the zeros after this?
//        }
//    }
//    Timer_1_ReadStatusRegister();
//}

int main(void)
{
    CyGlobalIntEnable; /* Enable global interrupts. */
    

    LCD_Start();
    LCD_IsReady();
    LCD_ClearDisplay();
    LCD_Position(0,0);
    LCD_PrintString("rotary encoder");
    CyDelay(2000);
    
    QuadDec_1_Start();
    Timer_1_Start();
    SW_Tx_UART_1_Start();
    SW_Tx_UART_1_PutString("test");
    SW_Tx_UART_1_PutCRLF();
    SW_Tx_UART_1_PutString("hello world");
    SW_Tx_UART_1_PutCRLF();
    SW_Tx_UART_1_PutString("monkey");
    //sample_interrupt_StartEx ( sample_interrupt_handler );
    
    LCD_ClearDisplay();
    LCD_Position(0,0);
    LCD_PrintString("QuadDec");
    
    LCD_Position(0,10);
    LCD_PrintString("MyCode");
    
    
    for(;;)
    {
        loop_counter = loop_counter + 1;
        count = QuadDec_1_GetCounter();
        
        if (loop_counter > 100) {
            LCD_Position(1,0);
            sprintf(displayStr, "%d  ", count);
            LCD_PrintString(displayStr);
            loop_counter = 0;
           


        }
    }
}

/* [] END OF FILE */
