/*******************************************************************************
* File Name:   main.c
*
* Description: This is the source code for the PWM brightness control code example
*              for ModusToolbox.
*
********************************************************************************
* The code is modified based on the example code provided by Infineon
* Example Code Name: PWM square wave
* Modifier: Yen-Chen Chang @ makestraight
*******************************************************************************/

/*******************************************************************************
* Header Files
*******************************************************************************/
#include "cybsp.h"
#include "cyhal.h"
#include "cy_retarget_io.h"
#include <inttypes.h>


/*******************************************************************************
* Macros
*******************************************************************************/
/* PWM Frequency = 100Hz */
#define PWM_FREQUENCY (100u)
/* PWM Duty-cycle = 50% */
#define PWM_DUTY_CYCLE (50.0f)


/*******************************************************************************
* Global Variables
*******************************************************************************/


/*******************************************************************************
* Function Prototypes
*******************************************************************************/


/*******************************************************************************
* Function Definitions
*******************************************************************************/

/*******************************************************************************
* Function Name: handle_error
********************************************************************************
* Summary:
*  User defined error handling function.
*
* Parameters:
*  status - status for evaluation.
*
* Return:
*  void
*
*******************************************************************************/
void handle_error(cy_rslt_t status)
{
    if (CY_RSLT_SUCCESS != status)
    {
        /* Halt the CPU while debugging */
        CY_ASSERT(0);
    }
}


/*******************************************************************************
* Function Name: check_status
********************************************************************************
* Summary:
*  Prints the message and waits forever when an error occurs.
*
* Parameters:
*  message - message to print if status is non-zero.
*  status - status for evaluation.
*
* Return:
*  void
*
*******************************************************************************/
void check_status(char *message, cy_rslt_t status)
{
    if (CY_RSLT_SUCCESS != status)
    {
        printf("\r\n=====================================================\r\n");
        printf("\nFAIL: %s\r\n", message);
        printf("Error Code: 0x%08" PRIX32 "\n", status);
        printf("\r\n=====================================================\r\n");

        while(true);
    }
}


/*******************************************************************************
* Function Name: main
********************************************************************************
* Summary:
* This is the main function for the CPU. It configures the PWM and puts the CPU
* in Sleep mode to save power.
*
* Parameters:
*  void
*
* Return:
*  int
*
*******************************************************************************/
int main(void)
{
    /* PWM object */
    cyhal_pwm_t pwm_led_control;
    /* API return code */
    cy_rslt_t result;

#if defined(CY_DEVICE_SECURE)
    cyhal_wdt_t wdt_obj;
    /* Clear watchdog timer so that it doesn't trigger a reset */
    result = cyhal_wdt_init(&wdt_obj, cyhal_wdt_get_max_timeout_ms());
    CY_ASSERT(CY_RSLT_SUCCESS == result);
    cyhal_wdt_free(&wdt_obj);
#endif

    /* Initialize the device and board peripherals */
    result = cybsp_init();
    handle_error(result);

    /* Enable global interrupts */
    __enable_irq();

    /* Initialize the retarget-io to use the debug UART port */
    result = cy_retarget_io_init(CYBSP_DEBUG_UART_TX, CYBSP_DEBUG_UART_RX,
                                 CY_RETARGET_IO_BAUDRATE);
    handle_error(result);

    /* \x1b[2J\x1b[;H - ANSI ESC sequence for clear screen */
    printf("\x1b[2J\x1b[;H");
    printf("****************** "
           "HAL: PWM square wave "
           "****************** \r\n\n");

    /* In this example, PWM output is routed to the user LED on the kit.
       See HAL API Reference document for API details. */

    /* Initialize the PWM */
    result = cyhal_pwm_init(&pwm_led_control, CYBSP_USER_LED, NULL);
    check_status("API cyhal_pwm_init failed with error code", result);

    /* Set the PWM output frequency and duty cycle */
    result = cyhal_pwm_set_duty_cycle(&pwm_led_control, 0,
                                      PWM_FREQUENCY);
    check_status("API cyhal_pwm_set_duty_cycle failed with error code", result);

    /* Start the PWM */
    result = cyhal_pwm_start(&pwm_led_control);
    check_status("API cyhal_pwm_start failed with error code", result);


    for (;;)
    {
    	// increase duty cycle to let LED become dimmer
    	for (int i = 0; i < 100; i++){
    	    /* Set the PWM output frequency and duty cycle */
    	    result = cyhal_pwm_set_duty_cycle(&pwm_led_control, i,
    	                                      PWM_FREQUENCY);

    	    /* Start the PWM */
    	    result = cyhal_pwm_start(&pwm_led_control);
            cyhal_system_delay_ms(10);

    	}
    	// decrease duty cycle to let LED become brighter
    	for (int i = 100; i > 0; i--){
    	    /* Set the PWM output frequency and duty cycle */
    	    result = cyhal_pwm_set_duty_cycle(&pwm_led_control, i,
    	                                      PWM_FREQUENCY);

    	    /* Start the PWM */
    	    result = cyhal_pwm_start(&pwm_led_control);
            cyhal_system_delay_ms(10);

    	}
    }
}


/* [] END OF FILE */
