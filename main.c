/**
 * @file main.c
 * @brief Main file.
 * @author Sistemas Digitales II
 * @date 2026-01-01
 */

/* Includes ------------------------------------------------------------------*/
/* Standard C libraries */
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
/* HW libraries */
#include "port_system.h"
#include "port_button.h"
#include "port_keyboard.h"
#include "port_rgb_light.h"
/* Project includes */
#include "fsm.h"
#include "fsm_button.h"
#include "fsm_keyboard.h"
#include "fsm_rgb_light.h"
#include "fsm_simone.h"
/* Defines ------------------------------------------------------------------*/
#define 	SIMONE_ON_OFF_PRESS_TIME_MS 1000

/**
 * @brief  The application entry point.
 * @retval int
 */
int main(void)
{
    /* Init board */
    port_system_init();

    fsm_button_t *p_fsm_button = fsm_button_new(PORT_USER_BUTTON_DEBOUNCE_TIME_MS,PORT_USER_BUTTON_ID);
    fsm_keyboard_t *p_fsm_keyboard = fsm_keyboard_new(PORT_KEYBOARD_MAIN_DEBOUNCE_TIME_MS,PORT_KEYBOARD_MAIN_ID);
    fsm_rgb_light_t *p_fsm_rgb_light = fsm_rgb_light_new(PORT_RGB_LIGHT_ID);

    fsm_simone_t *p_fsm_simone = fsm_simone_new(p_fsm_button,SIMONE_ON_OFF_PRESS_TIME_MS,p_fsm_keyboard,p_fsm_rgb_light,LEVEL_EASY);


    /* Infinite loop */
    while (1)
    {
        fsm_button_fire(p_fsm_button);
        fsm_keyboard_fire(p_fsm_keyboard);
        fsm_rgb_light_fire(p_fsm_rgb_light);
        fsm_simone_fire(p_fsm_simone);


    } // End of while(1)

    fsm_button_destroy(p_fsm_button);
    fsm_keyboard_destroy(p_fsm_keyboard);
    fsm_rgb_light_destroy(p_fsm_rgb_light);
    fsm_simone_destroy(p_fsm_simone);
    return 0;
}
