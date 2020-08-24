/*******************************************************************************
* File Name: VDAC_TP1_PM.c  
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

#include "VDAC_TP1.h"

static VDAC_TP1_backupStruct VDAC_TP1_backup;


/*******************************************************************************
* Function Name: VDAC_TP1_SaveConfig
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
void VDAC_TP1_SaveConfig(void) 
{
    if (!((VDAC_TP1_CR1 & VDAC_TP1_SRC_MASK) == VDAC_TP1_SRC_UDB))
    {
        VDAC_TP1_backup.data_value = VDAC_TP1_Data;
    }
}


/*******************************************************************************
* Function Name: VDAC_TP1_RestoreConfig
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
void VDAC_TP1_RestoreConfig(void) 
{
    if (!((VDAC_TP1_CR1 & VDAC_TP1_SRC_MASK) == VDAC_TP1_SRC_UDB))
    {
        if((VDAC_TP1_Strobe & VDAC_TP1_STRB_MASK) == VDAC_TP1_STRB_EN)
        {
            VDAC_TP1_Strobe &= (uint8)(~VDAC_TP1_STRB_MASK);
            VDAC_TP1_Data = VDAC_TP1_backup.data_value;
            VDAC_TP1_Strobe |= VDAC_TP1_STRB_EN;
        }
        else
        {
            VDAC_TP1_Data = VDAC_TP1_backup.data_value;
        }
    }
}


/*******************************************************************************
* Function Name: VDAC_TP1_Sleep
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
*  VDAC_TP1_backup.enableState:  Is modified depending on the enable 
*  state  of the block before entering sleep mode.
*
*******************************************************************************/
void VDAC_TP1_Sleep(void) 
{
    /* Save VDAC8's enable state */    
    if(VDAC_TP1_ACT_PWR_EN == (VDAC_TP1_PWRMGR & VDAC_TP1_ACT_PWR_EN))
    {
        /* VDAC8 is enabled */
        VDAC_TP1_backup.enableState = 1u;
    }
    else
    {
        /* VDAC8 is disabled */
        VDAC_TP1_backup.enableState = 0u;
    }
    
    VDAC_TP1_Stop();
    VDAC_TP1_SaveConfig();
}


/*******************************************************************************
* Function Name: VDAC_TP1_Wakeup
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
*  VDAC_TP1_backup.enableState:  Is used to restore the enable state of 
*  block on wakeup from sleep mode.
*
*******************************************************************************/
void VDAC_TP1_Wakeup(void) 
{
    VDAC_TP1_RestoreConfig();
    
    if(VDAC_TP1_backup.enableState == 1u)
    {
        /* Enable VDAC8's operation */
        VDAC_TP1_Enable();

        /* Restore the data register */
        VDAC_TP1_SetValue(VDAC_TP1_Data);
    } /* Do nothing if VDAC8 was disabled before */    
}


/* [] END OF FILE */
