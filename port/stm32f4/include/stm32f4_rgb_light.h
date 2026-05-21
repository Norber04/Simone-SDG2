/**
 * @file stm32f4_rgb_light.h
 * @brief Header for stm32f4_rgb_light.c file.
 * @author Norberto de los Rios Gutierrez
 * @author Alejandro Suarez Suarez
 * @date 22/05/2026
 */
#ifndef STM32F4_RGB_LIGHT_SYSTEM_H_
#define STM32F4_RGB_LIGHT_SYSTEM_H_

/* Includes ------------------------------------------------------------------*/
/* Standard C includes */
#include <stdint.h>
/* HW dependent includes */
#include "stm32f4xx.h"

/* Defines and enums ----------------------------------------------------------*/
/* Defines */

typedef struct{
    GPIO_TypeDef * 	p_port_red;     /*!< GPIO where the RED LED is connected */
    uint8_t 	pin_red;            /*!< Pin/line where the RED LED is connected */
    GPIO_TypeDef * 	p_port_green;   /*!< GPIO where the GREEN LED is connected */
    uint8_t 	pin_green;          /*!< Pin/line where the GREEN LED is connected */
    GPIO_TypeDef * 	p_port_blue;    /*!< GPIO where the BLUE LED is connected */
    uint8_t 	pin_blue;           /*!< Pin/line where the BLUE LED is connected */
}stm32f4_rgb_light_hw_t;

#define 	STM32F4_RGB_LIGHT_R_GPIO    GPIOB
#define 	STM32F4_RGB_LIGHT_R_PIN     6
#define 	STM32F4_RGB_LIGHT_G_GPIO    GPIOB
#define 	STM32F4_RGB_LIGHT_G_PIN     8
#define 	STM32F4_RGB_LIGHT_B_GPIO    GPIOB
#define 	STM32F4_RGB_LIGHT_B_PIN     9

/**
 * @brief This is an extern variable that is defined in stm32f4_keyboard.c. It represents an array of hardware keyboards.
 * 
 */
extern stm32f4_rgb_light_hw_t 	rgb_lights_arr [];
#endif /* STM32F4_RGB_LIGHT_SYSTEM_H_ */