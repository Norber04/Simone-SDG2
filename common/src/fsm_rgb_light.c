/**
 * @file fsm_rgb_light.c
 * @brief RGB light system FSM main file.
 * @author Norberto de los Ríos Gutiérrez
 * @author Alejandro Suarez Suarez
 * @date 22/05/2026
 */

/* Includes ------------------------------------------------------------------*/
/* Standard C includes */
#include <stdlib.h>
#include <stdio.h>
#include "math.h"
/* HW dependent includes */
#include "port_rgb_light.h"
#include "port_system.h"
/* Project includes */
#include "fsm.h"
#include "fsm_rgb_light.h"
#include "rgb_colors.h"
/* Typedefs --------------------------------------------------------------------*/

/* Private functions -----------------------------------------------------------*/

/**
 * @brief This function takes a regular color and applies a reduction based on the given intensity.
 * 
 * @param p_color Pointer to the color to be corrected. 
 * @param intensity_perc Linear input intensity, in the range [0, 100]. 
 */
void _correct_rgb_light_levels	(rgb_color_t * p_color, uint8_t intensity_perc )
{
    p_color->r = (uint8_t)((((float)p_color->r * intensity_perc) / 100.0f) + 0.5f);
    p_color->g = (uint8_t)((((float)p_color->g * intensity_perc) / 100.0f) + 0.5f);
    p_color->b = (uint8_t)((((float)p_color->b * intensity_perc) / 100.0f) + 0.5f);
}		
/* State machine input or transition functions */

/**
 * @brief Check if the RGB light is set to be active (ON), independently if it is idle or not. 
 * 
 * @param p_this Pointer to an fsm_t struct than contains an fsm_rgb_light_t. 
 * @return true If the RGB light system has been indicated to be active independently if it is idle or not. 
 * @return false If the RGB light system has been indicated to be inactive. 
 */
static bool check_active(fsm_t * p_this)
{
    fsm_rgb_light_t *p_rgb_light =(fsm_rgb_light_t *)p_this;
    return p_rgb_light->status;
}	

/**
 * @brief Check if the RGB light is set to be inactive (OFF). 
 * 
 * @param p_this Pointer to an fsm_t struct than contains an fsm_rgb_light_t. 
 * @return true If the RGB light system has been indicated to be inactive. 
 * @return false If the RGB light system has been indicated to be active. 
 */
static bool check_off(fsm_t * p_this)
{
    fsm_rgb_light_t *p_rgb_light =(fsm_rgb_light_t *)p_this;
    return !p_rgb_light->status;
}

/**
 * @brief Check if a new color has to be set. 
 * 
 * @param p_this Pointer to an fsm_t struct than contains an fsm_rgb_light_t. 
 * @return true If a new color has to be set 
 * @return false If a new color does not have to be set 
 */
static bool check_set_new_color	(fsm_t * p_this)
{
    fsm_rgb_light_t *p_rgb_light =(fsm_rgb_light_t *)p_this;
    return p_rgb_light->new_color;
}

/* State machine output or action functions */

/**
 * @brief Set the color of the RGB LED according to the intensity measured by the ultrasound sensor. 
 * 
 * @param p_this 	Pointer to an fsm_t struct than contains an fsm_rgb_light_t. 
 */
static void do_set_color(fsm_t * p_this)
{
    fsm_rgb_light_t *p_rgb_light =(fsm_rgb_light_t *)p_this;
    _correct_rgb_light_levels(&p_rgb_light->color,p_rgb_light->intensity_perc);
    port_rgb_light_set_rgb(p_rgb_light->rgb_light_id,p_rgb_light->color);
    p_rgb_light->new_color = false;
    p_rgb_light->idle = true;
}

/**
 * @brief Turn the RGB light system OFF. 
 * 
 * @param p_this 	Pointer to an fsm_t struct than contains an fsm_rgb_light_t. 
 */
static void do_set_off(	fsm_t * p_this)	
{
    fsm_rgb_light_t *p_rgb_light =(fsm_rgb_light_t *)p_this;
    port_rgb_light_set_rgb(p_rgb_light->rgb_light_id,color_off);
    p_rgb_light->idle = false;
}

/**
 * @brief Turn the RGB light system ON for the first time. 
 * 
 * @param p_this Pointer to an fsm_t struct than contains an fsm_rgb_light_t. 
 */
static void do_set_on(fsm_t * p_this)
{
    fsm_rgb_light_t *p_rgb_light =(fsm_rgb_light_t *)p_this;
    port_rgb_light_set_rgb(p_rgb_light->rgb_light_id,color_off);
}

/**
 * @brief Array representing the transitions table of the FSM RGB light. 
 * 
 */
fsm_trans_t fsm_trans_rgb_light[] = {
    {IDLE_RGB,      check_active,           SET_COLOR,      do_set_on},
    {SET_COLOR,     check_set_new_color,    SET_COLOR,      do_set_color},
    {SET_COLOR,     check_off,              IDLE_RGB,       do_set_off},
    {-1,            NULL,                   -1,             NULL}
};
/* Other auxiliary functions */
/**
 * @brief This function initializes the default values of the FSM struct and calls to the port to initialize the associated HW given the ID.
 * 
 * @param p_fsm_rgb_light   Pointer to the RGB light FSM. 
 * @param rgb_light_id      Unique RGB light identifier number. 
 */
static void fsm_rgb_light_init(	fsm_rgb_light_t * p_fsm_rgb_light, uint8_t rgb_light_id)
{
    fsm_init(&p_fsm_rgb_light->f ,fsm_trans_rgb_light);

    p_fsm_rgb_light->rgb_light_id = rgb_light_id;
    p_fsm_rgb_light->intensity_perc = MAX_LEVEL_INTENSITY;
    p_fsm_rgb_light->color = color_off;
    p_fsm_rgb_light->new_color = false;
    p_fsm_rgb_light->idle = false;
    p_fsm_rgb_light->status = false;

    port_rgb_light_init(p_fsm_rgb_light->rgb_light_id);
}	

/* Public functions -----------------------------------------------------------*/
fsm_rgb_light_t *fsm_rgb_light_new(uint8_t rgb_light_id)
{
    fsm_rgb_light_t *p_fsm_rgb_light = malloc(sizeof(fsm_rgb_light_t)); /* Do malloc to reserve memory of all other FSM elements, although it is interpreted as fsm_t (the first element of the structure) */
    fsm_rgb_light_init(p_fsm_rgb_light, rgb_light_id); /* Initialize the FSM */
    return p_fsm_rgb_light;
}

void fsm_rgb_light_destroy (fsm_rgb_light_t *p_fsm)
{
    if(p_fsm != NULL){
        free(p_fsm);
    }
}

void fsm_rgb_light_set_color_intensity (fsm_rgb_light_t *p_fsm, rgb_color_t color, uint8_t intensity_perc)
{
    p_fsm->color = color;
    p_fsm->intensity_perc = intensity_perc;
    p_fsm->new_color = true;
}

void fsm_rgb_light_fire (fsm_rgb_light_t *p_fsm)
{
    fsm_fire(&p_fsm->f);
}

bool fsm_rgb_light_get_status (fsm_rgb_light_t *p_fsm)
{
    return p_fsm->status;
}
 
void fsm_rgb_light_set_status (fsm_rgb_light_t *p_fsm, bool pause)
{
    p_fsm->status = pause;
}

bool fsm_rgb_light_check_activity(fsm_rgb_light_t * p_fsm)
{
   return p_fsm->status && !(p_fsm->idle);
}