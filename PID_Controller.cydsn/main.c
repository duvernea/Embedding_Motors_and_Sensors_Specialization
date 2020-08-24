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

uint8 dac_value = 0;

int main(void)
{
    CyGlobalIntEnable; /* Enable global interrupts. */
    
    VDAC_Control_Start();
    Opamp_1_Start();
    LCD_Char_1_Start();
    

    /* Place your initialization/startup code here (e.g. MyInst_Start()) */

    for(;;)
    {
        dac_value += 1;
        VDAC_Control_SetValue(dac_value);
        /* Place your application code here. */
    }
}

/* [] END OF FILE */
