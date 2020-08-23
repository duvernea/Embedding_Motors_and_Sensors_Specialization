#include "project.h"
#include "stdio.h"
#include "math.h"


int loop_counter = 0;
char displayStr[6] = "";
char displayStr2[6] = "";

uint8 dac_val = 25; // manually tuned, for now to this val
int16 ADC_result = 0;
int16 ADC_mV = 0;

double mass = 0;
double full_scale_mV = 175;
double capacity = 500;

int main(void)
{
    CyGlobalIntEnable; /* Enable global interrupts. */
    
    // Start up the modules
    PGA_1_Start();
    LCD_Char_1_Start();
    SW_Tx_UART_1_Start();
    VDAC8_1_Start();
    Opamp_1_Start();
    ADC_DelSig_1_Start();
    
    // Set up LCD Char Display
    LCD_Char_1_IsReady();
    LCD_Char_1_ClearDisplay();
    LCD_Char_1_Position(0,0);
    LCD_Char_1_PrintString("test");
    
    // Test the UART
    SW_Tx_UART_1_PutString("is there anybody out there");
    SW_Tx_UART_1_PutCRLF();
    CyDelay(1000);

    // Set fixed value for DAC value = input to ADC terminal
    // Note in practice this should be zero'd out on use
    VDAC8_1_SetValue(dac_val);
    
    for(;;)
    {
        CyDelay(1);
        loop_counter += 1;
        // every 100000 loops
        if (loop_counter > 300) {
            // Convert value of Delta Sigma ADC
            ADC_DelSig_1_StartConvert();
            ADC_DelSig_1_IsEndConversion(ADC_DelSig_1_WAIT_FOR_RESULT);
            ADC_result = ADC_DelSig_1_GetResult16();
            ADC_mV = ADC_DelSig_1_CountsTo_mVolts(ADC_result);
            
            // Calculate mass
            mass = ((double) ADC_mV) / full_scale_mV * capacity;
            
            // Update displays
            SW_Tx_UART_1_PutString("ADC result in mV: ");
            sprintf(displayStr, "%d", ADC_mV);
            SW_Tx_UART_1_PutString(displayStr);
            //if (mass > 500 || mass < 500) {
                //SW_Tx_UART_1_PutString("OVERLOAD");
            //} else {
            
            // Note: below is required because sprintf doesn't seem to work for type float
            char *tmpSign = (mass < 0) ? "" : "-";
            float tmpVal = (mass < 0) ? -mass : mass;

            int tmpInt1 = tmpVal;                  // Get the integer (678).
            float tmpFrac = tmpVal - tmpInt1;      // Get fraction (0.0123).
            int tmpInt2 = trunc(tmpFrac * 10);  // Turn into integer (123).
            sprintf (displayStr2, " mass = %s%d.%d g", tmpSign, tmpInt1, tmpInt2);
  
            LCD_Char_1_ClearDisplay();
            LCD_Char_1_Position(0,0);
            LCD_Char_1_PrintString(displayStr2);
            
            SW_Tx_UART_1_PutString(displayStr2);
            //}
            SW_Tx_UART_1_PutCRLF();
            loop_counter = 0;
        }  
    }
}

/* [] END OF FILE */
