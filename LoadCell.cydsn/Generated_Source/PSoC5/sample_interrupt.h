/*******************************************************************************
* File Name: sample_interrupt.h
* Version 1.70
*
*  Description:
*   Provides the function definitions for the Interrupt Controller.
*
*
********************************************************************************
* Copyright 2008-2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/
#if !defined(CY_ISR_sample_interrupt_H)
#define CY_ISR_sample_interrupt_H


#include <cytypes.h>
#include <cyfitter.h>

/* Interrupt Controller API. */
void sample_interrupt_Start(void);
void sample_interrupt_StartEx(cyisraddress address);
void sample_interrupt_Stop(void);

CY_ISR_PROTO(sample_interrupt_Interrupt);

void sample_interrupt_SetVector(cyisraddress address);
cyisraddress sample_interrupt_GetVector(void);

void sample_interrupt_SetPriority(uint8 priority);
uint8 sample_interrupt_GetPriority(void);

void sample_interrupt_Enable(void);
uint8 sample_interrupt_GetState(void);
void sample_interrupt_Disable(void);

void sample_interrupt_SetPending(void);
void sample_interrupt_ClearPending(void);


/* Interrupt Controller Constants */

/* Address of the INTC.VECT[x] register that contains the Address of the sample_interrupt ISR. */
#define sample_interrupt_INTC_VECTOR            ((reg32 *) sample_interrupt__INTC_VECT)

/* Address of the sample_interrupt ISR priority. */
#define sample_interrupt_INTC_PRIOR             ((reg8 *) sample_interrupt__INTC_PRIOR_REG)

/* Priority of the sample_interrupt interrupt. */
#define sample_interrupt_INTC_PRIOR_NUMBER      sample_interrupt__INTC_PRIOR_NUM

/* Address of the INTC.SET_EN[x] byte to bit enable sample_interrupt interrupt. */
#define sample_interrupt_INTC_SET_EN            ((reg32 *) sample_interrupt__INTC_SET_EN_REG)

/* Address of the INTC.CLR_EN[x] register to bit clear the sample_interrupt interrupt. */
#define sample_interrupt_INTC_CLR_EN            ((reg32 *) sample_interrupt__INTC_CLR_EN_REG)

/* Address of the INTC.SET_PD[x] register to set the sample_interrupt interrupt state to pending. */
#define sample_interrupt_INTC_SET_PD            ((reg32 *) sample_interrupt__INTC_SET_PD_REG)

/* Address of the INTC.CLR_PD[x] register to clear the sample_interrupt interrupt. */
#define sample_interrupt_INTC_CLR_PD            ((reg32 *) sample_interrupt__INTC_CLR_PD_REG)


#endif /* CY_ISR_sample_interrupt_H */


/* [] END OF FILE */
