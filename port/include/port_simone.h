/**
 * @file port_simone.h
 * @brief Header for the portable functions os the Simone board game.
 * @author Norberto de los Rios Gutierrez
 * @author Alejandro Suarez Suarez
 * @date 22/05/2026
 */
#ifndef PORT_SIMONE_H_
#define PORT_SIMONE_H_
/* Includes ------------------------------------------------------------------*/
/* Standard C includes */
#include <stdint.h>
#include <stdbool.h>

/* Function prototypes and explanation -------------------------------------------------*/

/**
 * @brief This function initializes the HW associated to the Simone game, such as the timer that controls 
 * the duration of the different events of the game. It is called from the fsm_simone_init() function to initialize 
 * the HW dependencies of the Simone FSM.
 * 
 */
void port_simone_init(void);

/**
 * @brief This function is called to configure the timer that controls the duration of the different events of the game. 
 * The duration of the timeout is received as a parameter in milliseconds. When the timer expires, an interrupt is generated 
 * and the flag of the simone_hw struct is set to indicate that the time has expired.
 * 
 * @param duration_ms duration set for the timer
 */
void port_simone_set_timer_timeout(uint32_t duration_ms);

/**
 * @brief This function is called to stop the timer of the Simone game when the game is over or when the player has won. 
 * 
 */
void port_simone_stop_timer(void);

/**
 * @brief Get the timer timeout status of the Simone game. 
 * 
 * @return true when the timeout time has passed 
 * @return false 
 */
bool port_simone_get_timeout_status(void);

/**
 * @brief Set the status of the timer timeout status of the Simone game. 
 * 
 * @param status status for the timeout status
 */
void port_simone_set_timeout_status(bool status);


#endif /* PORT_SIMONE_H_ */