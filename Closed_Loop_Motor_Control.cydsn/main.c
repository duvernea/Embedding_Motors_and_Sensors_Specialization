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

int main(void)
{
    CyGlobalIntEnable; /* Enable global interrupts. */

    PGA_1_Start();
    ADC_SAR_1_Start();
    PWM_1_Start();
    
    //P1_5_Write(0);
    

    for(;;)
    {
        /* Place your application code here. */
        //P1_5_Write(1);
        ADC_SAR_1_StartConvert();
        ADC_SAR_1_IsEndConversion(ADC_SAR_1_WAIT_FOR_RESULT);
        
        
        CyDelayUs(800);
        //P1_5_Write(0);
        CyDelayUs(800);
    }
}

/* [] END OF FILE */
