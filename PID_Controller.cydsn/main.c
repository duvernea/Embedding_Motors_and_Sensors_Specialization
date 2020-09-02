/* ========================================
 * Embedding Sensors and Motors Specialization
 * PID Controller
 * Brian DuVerneay
 * ========================================
*/

#include "project.h"
#include "stdio.h"

#define SAMPLE_LIMIT 250

char TransmitBuffer[16];
char testString[16] = "";

uint32 loop_count = 0;
uint8 dac_value = 0;
uint8 Vmeas_raw = 0;

uint8 Vmeas_buffer[SAMPLE_LIMIT];
uint32 Vmeas_buffer_index = 0;

int32 Vmeas, Vmeas_delta, Vmeas_last, Verror;
int32 Vref = 120;   // The goal output. In volts = 120/255*2.048 = 0.9637
int32 Verror_sum, Verror_sum_limit = 10000;
int32 Vout, Vout_limited;

int32 Kp = 80, Kd = -120, Ki = 10;

uint32 Kp_term_overflow_count, Kp_term_underflow_count;
uint32 Kd_term_overflow_count, Kd_term_underflow_count;

uint32 Kd_display_gain = 8;
int32 Kd_term, Kp_term, Ki_term, FF_term = 100;
int32 Kd_term_limited, Kp_term_limited, Ki_term_limited;

int32 Kp_scale_factor = 128, Kd_scale_factor = 128, Ki_scale_factor = 1024;

// Overflow and underflow counts
uint32 Kp_term_overflow_count, Kp_term_underflow_count;
uint32 Kd_term_overflow_count, Kd_term_underflow_count;
uint32 Ki_term_overflow_count, Ki_term_underflow_count;
uint32 Vout_term_underflow_count, Vout_term_overflow_count;

// Debugging vars
int32 feedforward_only = 0;

int main(void)
{
    CyGlobalIntEnable; /* Enable global interrupts. */
    
    // Power up / start up blocks
    Opamp_0_Start();
    Opamp_1_Start();
    Opamp_2_Start();
    Opamp_3_Start();
    LCD_Char_1_Start();
    SW_Tx_UART_1_Start();
    VDAC_Control_Start();
    VDAC_TP1_Start();
    VDAC_TP2_Start();
    ADC_SAR_1_Start();
    
    // Output basic display infos
    SW_Tx_UART_1_PutCRLF();
    SW_Tx_UART_1_PutString("PID Control Lab");
    SW_Tx_UART_1_PutCRLF();
 
    LCD_Char_1_IsReady();
    LCD_Char_1_ClearDisplay();
    LCD_Char_1_Position(0,0);
    LCD_Char_1_PrintString("PID Control Lab");
    CyDelay(1000);
    
    // Test clock / instruction speed
    P1_6_Write(1);
    P1_6_Write(0);
    P1_6_Write(1);
    P1_6_Write(0);
    VDAC_Control_SetValue(0);
    
    for(;;)
    {
        loop_count += 1;
        
        Vmeas_last = Vmeas;
        
        // ADC Conversion
        P1_6_Write(0);
        ADC_SAR_1_StartConvert();
        ADC_SAR_1_IsEndConversion(ADC_SAR_1_WAIT_FOR_RESULT);
        P1_6_Write(1);
        
        Vmeas_raw = (int32) ADC_SAR_1_GetResult8();
        
        // Test point - output measured voltage 
        VDAC_TP1_SetValue(127 + Vmeas_raw);
        // Save measurement to buffer
        Vmeas_buffer[loop_count] = Vmeas_raw;
        // Copy 8 bit to 32 bit int
        Vmeas = (int32) Vmeas_raw;
        
        
        if (loop_count == 1) {
            Vmeas_delta = 0;
        } else {
            Vmeas_delta = Vmeas - Vmeas_last;
        }
        
        VDAC_TP2_SetValue( 127 + Kd_display_gain*Vmeas_delta);
        
        // Kd - derivative gain (fixed)
        Kd_term = Kd * Vmeas_delta / Kd_scale_factor;   // using "scale_factor" to keep integer math
        
        Kd_term_limited = Kd_term;
        
        if (Kd_term_limited > 127) {
            Kd_term_limited = 127;
            Kd_term_overflow_count += 1;
        }
        if (Kd_term_limited < -127) {
            Kd_term_limited = -127;
            Kd_term_underflow_count += 1;
        }
        
        Verror = Vref - Vmeas;      // Vref is what we want it to be. Vmeas is what we have
        
        Kp_term = Kp * Verror / Kp_scale_factor;
        
        Kp_term_limited = Kp_term;
        
        if (Kp_term_limited > 127) {
            Kp_term_limited = 127;
            Kp_term_overflow_count += 1;
        }
        if (Kp_term_limited < -127) {
            Kp_term_limited = -127;
            Kp_term_underflow_count += 1;
        }
        
        // INTEGRAL TERM
        Verror_sum += Verror;
        if (Verror_sum > Verror_sum_limit) {
            Verror_sum = Verror_sum_limit;
            Ki_term_overflow_count += 1;
        }
        if (Verror_sum < -Verror_sum_limit) {
            Verror_sum = -Verror_sum_limit;
            Ki_term_underflow_count += 1;
        }
        
        Ki_term = Ki * Verror_sum / Ki_scale_factor;
        
        if (feedforward_only == 1) {
            Vout = FF_term;                     // FF only
        } else {
            //Vout = FF_term + Kp_term + Ki_term + Kd_term; // FF + PID control
            Vout = Kp_term + Ki_term + Kd_term; // FF + PID control
            
        }
            
        Vout_limited = Vout;
        if (Vout_limited > 255) {
            Vout_limited = 255;
            Vout_term_overflow_count += 1;
        }
        if (Vout_limited < 0) {
            Vout_limited = 0;
            Vout_term_underflow_count += 1;
        }

        VDAC_Control_SetValue(Vout_limited);
        
        // Step response test
        if (loop_count < SAMPLE_LIMIT) {
            //P12_3_Write(1);
        }
        if (loop_count >= SAMPLE_LIMIT) {
            loop_count = 0;
            //P12_3_Write(0);
            
            VDAC_Control_SetValue(0);
            CyDelay(100);
            SW_Tx_UART_1_PutString("\r\n Index  Vcap  \n\r");
            SW_Tx_UART_1_PutString(" _____   ____  \n\n\r");
            
            Vmeas_buffer_index = 0;
   
            while (Vmeas_buffer_index < SAMPLE_LIMIT) {
                sprintf(TransmitBuffer, " %03d     %d", (uint16) Vmeas_buffer_index, Vmeas_buffer[Vmeas_buffer_index]);
                Vmeas_buffer_index += 1;
                SW_Tx_UART_1_PutString(TransmitBuffer);
                SW_Tx_UART_1_PutCRLF();
            }
//            LCD_Char_1_ClearDisplay();
//            LCD_Char_1_Position(0,0);
//            sprintf(testString, "Vmeas_raw = %d", Vmeas_raw);
//            LCD_Char_1_PrintString(testString);
           
        }
    }
}

/* [] END OF FILE */
