/**
 * @file stm32f4_keyboard.h
 * @brief Header for stm32f4_keyboard.c file.
 * @author Norberto de los Rios Gutierrez
 * @author Alejandro Suarez Suarez
 * @date 22/05/2026
 */
#ifndef STM32F4_KEYBOARD_H_
#define STM32F4_KEYBOARD_H_

/* Includes ------------------------------------------------------------------*/
/* Standard C includes */
#include <stdint.h>
#include <stdbool.h>
/* HW dependent includes */
#include "stm32f4xx.h"
#include "keyboards.h"
/* Defines and enums ----------------------------------------------------------*/
/* Defines */
/**
 * @brief 	Structure to define the HW dependencies of a keyboard 
 * 
 */
typedef struct
{
    const keyboard_t *p_keyboard;       /*!< Pointer to the keyboard structure */
    GPIO_TypeDef **p_row_ports;         /*!< Pointer to an array of GPIO ports where the keyboard rows are connected */
    uint8_t *p_row_pins;                /*!< Pointer to an array of pins/lines where the keyboard rows are connected */
    GPIO_TypeDef **p_col_ports;         /*!< Pointer to an array of GPIO ports where the keyboard columns are connected */
    uint8_t *p_col_pins;                /*!< Pointer to an array of pins/lines where the keyboard columns are connected */
    bool flag_key_pressed;              /*!< Flag to indicate that a key has been pressed */
    bool flag_row_timeout;              /*!< Flag to indicate that the column scanning timeout has occurred */
    uint8_t col_idx_interrupt;          /*!< Index of the column that provoked an interrupt */
    uint8_t current_excited_row;        /*!< Current row being excited */
}stm32f4_keyboard_hw_t;

#define STM32F4_KEYBOARD_MAIN_ROW_0_GPIO    GPIOA
#define STM32F4_KEYBOARD_MAIN_ROW_0_PIN     0
#define STM32F4_KEYBOARD_MAIN_ROW_1_GPIO    GPIOA
#define STM32F4_KEYBOARD_MAIN_ROW_1_PIN     1
#define STM32F4_KEYBOARD_MAIN_ROW_2_GPIO    GPIOA
#define STM32F4_KEYBOARD_MAIN_ROW_2_PIN     4
#define STM32F4_KEYBOARD_MAIN_ROW_3_GPIO    GPIOB
#define STM32F4_KEYBOARD_MAIN_ROW_3_PIN     0
#define STM32F4_KEYBOARD_MAIN_COL_0_GPIO    GPIOA
#define STM32F4_KEYBOARD_MAIN_COL_0_PIN     8
#define STM32F4_KEYBOARD_MAIN_COL_1_GPIO    GPIOB
#define STM32F4_KEYBOARD_MAIN_COL_1_PIN     10
#define STM32F4_KEYBOARD_MAIN_COL_2_GPIO    GPIOB
#define STM32F4_KEYBOARD_MAIN_COL_2_PIN     4
#define STM32F4_KEYBOARD_MAIN_COL_3_GPIO    GPIOB
#define STM32F4_KEYBOARD_MAIN_COL_3_PIN     5

/**
 * @brief This is an extern variable that is declared in stm32f4_keyboard.h. It represents an array of hardware keyboards.
 * 
 */
extern stm32f4_keyboard_hw_t 	keyboards_arr [];

/* Function prototypes and explanation -------------------------------------------------*/


#endif /* STM32F4_KEYBOARD_H_ */