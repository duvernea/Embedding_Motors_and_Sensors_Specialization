/*******************************************************************************
* File Name: VDAC_TP2_PM.c  
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

#include "VDAC_TP2.h"

static VDAC_TP2_backupStruct VDAC_TP2_backup;


/*******************************************************************************
* Function Name: VDAC_TP2_SaveConfig
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
void VDAC_TP2_SaveConfig(void) 
{
    if (!((VDAC_TP2_CR1 & VDAC_TP2_SRC_MASK) == VDAC_TP2_SRC_UDB))
    {
        VDAC_TP2_backup.data_value = VDAC_TP2_Data;
    }
}


/*******************************************************************************
* Function Name: VDAC_TP2_RestoreConfig
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
void VDAC_TP2_RestoreConfig(void) 
{
    if (!((VDAC_TP2_CR1 & VDAC_TP2_SRC_MASK) == VDAC_TP2_SRC_UDB))
    {
        if((VDAC_TP2_Strobe & VDAC_TP2_STRB_MASK) == VDAC_TP2_STRB_EN)
        {
            VDAC_TP2_Strobe &= (uint8)(~VDAC_TP2_STRB_MASK);
            VDAC_TP2_Data = VDAC_TP2_backup.data_value;
            VDAC_TP2_Strobe |= VDAC_TP2_STRB_EN;
        }
        else
        {
            VDAC_TP2_Data = VDAC_TP2_backup.data_value;
        }
    }
}


/*******************************************************************************
* Function Name: VDAC_TP2_Sleep
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
*  VDAC_TP2_backup.enableState:  Is modified depending on the enable 
*  state  of the block before entering sleep mode.
*
*******************************************************************************/
void VDAC_TP2_Sleep(void) 
{
    /* Save VDAC8's enable state */    
    if(VDAC_TP2_ACT_PWR_EN == (VDAC_TP2_PWRMGR & VDAC_TP2_ACT_PWR_EN))
    {
        /* VDAC8 is enabled */
        VDAC_TP2_backup.enableState = 1u;
    }
    else
    {
        /* VDAC8 is disabled */
        VDAC_TP2_backup.enableState = 0u;
    }
    
    VDAC_TP2_Stop();
    VDAC_TP2_SaveConfig();
}


/*******************************************************************************
* Function Name: VDAC_TP2_Wakeup
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
*  VDAC_TP2_backup.enableState:  Is used to restore the enable state of 
*  block on wakeup from sleep mode.
*
*******************************************************************************/
void VDAC_TP2_Wakeup(void) 
{
    VDAC_TP2_RestoreConfig();
    
    if(VDAC_TP2_backup.enableState == 1u)
    {
        /* Enable VDAC8's operation */
        VDAC_TP2_Enable();

        /* Restore the data register */
        VDAC_TP2_SetValue(VDAC_TP2_Data);
    } /* Do nothing if VDAC8 was disabled before */    
}


/* [] END OF FILE */
