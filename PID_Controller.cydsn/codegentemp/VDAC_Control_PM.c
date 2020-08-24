/*******************************************************************************
* File Name: VDAC_Control_PM.c  
* Version 1.90
*
* Description:
*  This file provides the power management source code to API for the
*  VDAC8.  
*
* Note:
*  None
*
********************************************************************************
* Copyright 2008-2012, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#include "VDAC_Control.h"

static VDAC_Control_backupStruct VDAC_Control_backup;


/*******************************************************************************
* Function Name: VDAC_Control_SaveConfig
********************************************************************************
* Summary:
*  Save the current user configuration
*
* Parameters:  
*  void  
*
* Return: 
*  void
*
*******************************************************************************/
void VDAC_Control_SaveConfig(void) 
{
    if (!((VDAC_Control_CR1 & VDAC_Control_SRC_MASK) == VDAC_Control_SRC_UDB))
    {
        VDAC_Control_backup.data_value = VDAC_Control_Data;
    }
}


/*******************************************************************************
* Function Name: VDAC_Control_RestoreConfig
********************************************************************************
*
* Summary:
*  Restores the current user configuration.
*
* Parameters:  
*  void
*
* Return: 
*  void
*
*******************************************************************************/
void VDAC_Control_RestoreConfig(void) 
{
    if (!((VDAC_Control_CR1 & VDAC_Control_SRC_MASK) == VDAC_Control_SRC_UDB))
    {
        if((VDAC_Control_Strobe & VDAC_Control_STRB_MASK) == VDAC_Control_STRB_EN)
        {
            VDAC_Control_Strobe &= (uint8)(~VDAC_Control_STRB_MASK);
            VDAC_Control_Data = VDAC_Control_backup.data_value;
            VDAC_Control_Strobe |= VDAC_Control_STRB_EN;
        }
        else
        {
            VDAC_Control_Data = VDAC_Control_backup.data_value;
        }
    }
}


/*******************************************************************************
* Function Name: VDAC_Control_Sleep
********************************************************************************
* Summary:
*  Stop and Save the user configuration
*
* Parameters:  
*  void:  
*
* Return: 
*  void
*
* Global variables:
*  VDAC_Control_backup.enableState:  Is modified depending on the enable 
*  state  of the block before entering sleep mode.
*
*******************************************************************************/
void VDAC_Control_Sleep(void) 
{
    /* Save VDAC8's enable state */    
    if(VDAC_Control_ACT_PWR_EN == (VDAC_Control_PWRMGR & VDAC_Control_ACT_PWR_EN))
    {
        /* VDAC8 is enabled */
        VDAC_Control_backup.enableState = 1u;
    }
    else
    {
        /* VDAC8 is disabled */
        VDAC_Control_backup.enableState = 0u;
    }
    
    VDAC_Control_Stop();
    VDAC_Control_SaveConfig();
}


/*******************************************************************************
* Function Name: VDAC_Control_Wakeup
********************************************************************************
*
* Summary:
*  Restores and enables the user configuration
*  
* Parameters:  
*  void
*
* Return: 
*  void
*
* Global variables:
*  VDAC_Control_backup.enableState:  Is used to restore the enable state of 
*  block on wakeup from sleep mode.
*
*******************************************************************************/
void VDAC_Control_Wakeup(void) 
{
    VDAC_Control_RestoreConfig();
    
    if(VDAC_Control_backup.enableState == 1u)
    {
        /* Enable VDAC8's operation */
        VDAC_Control_Enable();

        /* Restore the data register */
        VDAC_Control_SetValue(VDAC_Control_Data);
    } /* Do nothing if VDAC8 was disabled before */    
}


/* [] END OF FILE */
