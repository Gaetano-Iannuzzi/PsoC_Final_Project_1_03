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
 *@authors Simone Fiorani, Alessandra Gargiulo, Gaetano Iannuzzi
 *@date 2020
*/
// Include header
#include "InterruptRoutines.h"
#include "Menu_Functions.h"

// Include required header files
#include "project.h"

// Variables declaration
int32 value_digit;
int32 value_mv;
uint8 SendBytesFlag=0;
char F;
int16_t  value_temp[8];  //array of temperature data
float32   value_temp_float = 0;

int WTM_Full;  //Interrupt on Watemark of LIS3DH

CY_ISR(Custom_ISR_ADC)
{
    // Read Timer status register to bring interrupt line low
    if(p<8){
    Timer_ReadStatusRegister();}
    
    
    value_digit = ADC_DelSig_Read32();
        
    if (value_digit < 0)        value_digit = 0;
    if (value_digit > 65535)    value_digit = 65535;
    
    value_mv = ADC_DelSig_CountsTo_mVolts(value_digit);

    if      (TempUnit == CELSIUS)           
    {
    value_temp_float = (value_mv-500)/10;
    value_temp[p] = (int16)(value_temp_float * 100);
    }
    else if (TempUnit == FAHRENHEIT)
    {
        value_temp_float = (value_mv-500)/10;
        value_temp_float = (value_temp_float*9/5)+32;
        value_temp[p] = (int16)(value_temp_float * 100); 
    }
     p++;
    if(p ==8){
    PacketReadyFlag=1;
    }

    }



CY_ISR(Custom_Button_ISR)
{  
    if (Pin_Button_Read()==1)
    {
        Timer_Button_ReadStatusRegister();
        temp = 0;
    }
    
    else if (Pin_Button_Read() == 0)
    {
        Timer_Button_ReadStatusRegister();
    temp++;
    }
    if (temp >= 5)
    {
        Led_Update(999,999);
        UART_PutString("\r***** RESET EEPROM*****\r");
        FIFO_Reset();
        UART_PutString("*********    HELP: POSSIBLE SETTINGS    *********\r\r");
        UART_PutString("f. Full Scale\r\np. Sampling Freq\r\nt. Set Temperature Unit\r\nv. Print Data on BCP\r\nb. Start Registration\r\ns. Stop Registration\r\n\r");        
        UART_PutString("***********************************************\r\n");
        temp = 0;
    }
    
}

CY_ISR(Custom_isr_ACC)
{
    WTM_Full=1;
}
/* [] END OF FILE */
