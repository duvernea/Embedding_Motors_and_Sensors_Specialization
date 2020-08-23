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
#include "stdio.h"
#include "project.h"

int main(void)
{
    CyGlobalIntEnable; /* Enable global interrupts. */
    
    uint16 voltage_sample = 0;
    uint16 test = 0;
    float Ka = 0.3466;   // Volts / RPM
    float rpm = 0;
    float a = 5.2;
    uint16 converted_sample = 0;
    int32 voltCount = 0;
    
    char displayStr[15] = {'\0'};
    
    /* Variable to hold the result in millivolts converted from ADC counts */
    int32 mVolts = 0;
   
    // Initialization / Startup
    ADC_SAR_Start();
    // Turn motor off
    P12_6_Write(0);
    
    LCD_Start();
    LCD_Position(0,0);
    LCD_PrintString("Motor Test");
    
    CyDelay(1000);

    for(;;)
    {
        // Turn Motor On
        P12_6_Write(1);
        voltCount = ADC_SAR_GetResult16();
                
        // LCD Display update (same every loop)
        LCD_ClearDisplay();
        LCD_IsReady();
        LCD_Position(0,0);
        LCD_PrintString("speed:");
        LCD_Position(1,0);
        LCD_PrintString("voltage:");
        LCD_Position(0, 13);
        LCD_PrintString("RPM");
        LCD_Position(1, 14);
        LCD_PrintString("mV");

        // LCD Display speed and voltage (change every loop)
        LCD_Position(1, 9);
        mVolts = ADC_SAR_CountsTo_mVolts(voltCount);
        LCD_PrintNumber(mVolts);
        rpm = (float )(5000-mVolts) / Ka;
        LCD_Position(0,7);
        LCD_PrintNumber(rpm);
        
        // Let it run for 1 second
        CyDelay(1000); 
        // Turn Motor Off
        P12_6_Write(0);

        // Short 1us delay before sampling
        CyDelayUs(25);
        ADC_SAR_StartConvert();
        ADC_SAR_IsEndConversion(ADC_SAR_WAIT_FOR_RESULT);
        // Measure Drain voltage of FET
        
        //LCD_ClearDisplay();
        //LCD_IsReady();
        //LCD_Position(1,0);
        //LCD_PrintString("voltage:");

        
    }
}

/* [] END OF FILE */
