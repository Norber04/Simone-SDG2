/**
 * @file fsm_simone.c
 * @brief Simone FSM main file.
 * @author alumno1
 * @author alumno2
 * @date fecha
 */

/* Includes ------------------------------------------------------------------*/
/* Standard C includes */
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

/* HW dependent includes */
#include "port_system.h"
#include "port_button.h"
#include "port_keyboard.h"
#include "port_rgb_light.h"
#include "port_simone.h"
/* Project includes */
#include "fsm.h"
#include "fsm_simone.h"
#include "rgb_colors.h"

const rgb_color_t *p_colors_library[] = {&color_red, &color_green, &color_blue, &color_yellow, &color_turquoise, &color_white};

static char _get_key_from_color(rgb_color_t color)
{
    if (color.r == color_red.r && color.g == color_red.g && color.b == color_red.b) {
        return KEY_RED;
    } else if (color.r == color_green.r && color.g == color_green.g && color.b == color_green.b) {
        return KEY_GREEN;
    } else if (color.r == color_blue.r && color.g == color_blue.g && color.b == color_blue.b) {
        return KEY_BLUE;
    } else if (color.r == color_yellow.r && color.g == color_yellow.g && color.b == color_yellow.b) {
        return KEY_YELLOW;
    } else if (color.r == color_turquoise.r && color.g == color_turquoise.g && color.b == color_turquoise.b) {
        return KEY_TURQUOISE;
    } else if (color.r == color_white.r && color.g == color_white.g && color.b == color_white.b) {
        return KEY_WHITE;
    } else {
        return KEY_INVALID_COLOR;
    }
}

static rgb_color_t _get_color_from_key(char key)
{
    switch (key) {
        case KEY_RED: return color_red;
        case KEY_GREEN: return color_green;
        case KEY_BLUE: return color_blue;
        case KEY_YELLOW: return color_yellow;
        case KEY_TURQUOISE: return color_turquoise;
        case KEY_WHITE: return color_white;
        default: return color_off;
    }
}

/**
 * @brief 	Add a new color and intensity to the sequence. 
 * 
 * @param p_fsm_simone Pointer to the simone FSM. 
 */
static void _add_color(fsm_simone_t *p_fsm_simone)
{
    /*generate a random index*/
    uint8_t ind = rand() % NUMBER_OF_COLORS_GAME;

    /*generate a random intensity*/
    uint8_t min;
    switch (p_fsm_simone->level)
    {
    case LEVEL_EASY:
        min = LEVEL_EASY_MIN_INTENSITY;
        break;
    case LEVEL_MEDIUM:
        min = LEVEL_MEDIUM_MIN_INTENSITY;
        break;
    case LEVEL_HARD:
        min = LEVEL_HARD_MIN_INTENSITY;
        break;
    default:
        min = LEVEL_EASY_MIN_INTENSITY;
        break;
    }
    uint8_t intensity = (rand() % (LEVEL_MAX_INTENSITY - min + 1)) + min;

    /*save the values in the arrays*/
    if(p_fsm_simone->seq_idx = SEQUENCE_LENGTH)
    {
        p_fsm_simone->seq_idx = 0;
    }
    p_fsm_simone->seq_colors[p_fsm_simone->seq_idx] = *p_colors_library[ind];
    p_fsm_simone->seq_intensities[p_fsm_simone->seq_idx] = intensity;
    p_fsm_simone->seq_idx++;
}

/* State machine input or transition functions */

/**
 * @brief 	Check if the button has been pressed for the required time to turn ON the Simone system. 
 * 
 * @param p_this Pointer to an fsm_t struct than contains an fsm_simone_t.
 * @return true the button has been pressed for the required time to turn on the system
 * @return false the button has not been pressed for the required time to turn on the system
 */
static bool check_on(fsm_t *p_this)
{
    fsm_simone_t *p_simone =(fsm_simone_t *)p_this;
    return p_simone->p_fsm_button->duration >= p_simone->on_off_press_time_ms;
}

/**
 * @brief 	Check if the button has been pressed for the required time to turn OFF the Simone system. 
 * 
 * @param p_this Pointer to an fsm_t struct than contains an fsm_simone_t.
 * @return true the button has been pressed for the required time to turn off the system
 * @return false the button has not been pressed for the required time to turn off the system
 */
static bool check_off(fsm_t *p_this)
{
    fsm_simone_t *p_simone =(fsm_simone_t *)p_this;
    return p_simone->p_fsm_button->duration >= p_simone->on_off_press_time_ms;
}


/**
 * @brief 	Check if any of the elements of the system is active. 
 * 
 * @param p_this Pointer to an fsm_t struct than contains an fsm_simone_t.
 * @return true if any of the fsm are active
 * @return false 
 */
static bool check_activity(fsm_t *p_this)
{
    fsm_simone_t *p_simone =(fsm_simone_t *)p_this;
    return fsm_button_check_activity(p_simone->p_fsm_button) || fsm_keyboard_check_activity(p_simone->p_fsm_keyboard) || fsm_rgb_light_check_activity(p_simone->p_fsm_rgb_light);
}

/**
 * @brief 	Check if all the elements of the system are inactive. 
 * 
 * @param p_this Pointer to an fsm_t struct than contains an fsm_simone_t.
 * @return true 
 * @return false if any of the fsm are active
 */
static bool check_no_activity (fsm_t *p_this)
{
    return !check_activity(p_this);
}

/**
 * @brief Check if a new color has been added to the sequence. 
 * 
 * @param p_this Pointer to an fsm_t struct than contains an fsm_simone_t.
 * @return true if the seq_idx is different from player_idx
 * @return false 
 */
static bool check_color_added(fsm_t *p_this)
{
    fsm_simone_t *p_simone =(fsm_simone_t *)p_this;
    return p_simone->seq_idx != p_simone->player_idx;
}

/**
 * @brief 	Check if the playback of the sequence is over and thus the player can start inputting the sequence. 
 * 
 * @param p_this Pointer to an fsm_t struct than contains an fsm_simone_t.
 * @return true if the playback has ended(playblack_idx<0) and the timer has expired
 * @return false 
 */
static bool check_playback_over (fsm_t *p_this)
{
    fsm_simone_t *p_simone =(fsm_simone_t *)p_this;
    return (p_simone->playback_idx<0 && port_simone_get_timeout_status());
}

/**
 * @brief Check of the player has won the game
 * 
 * @param p_this Pointer to an fsm_t struct than contains an fsm_simone_t.
 * @return true the player has won the game
 * @return false 
 */
static bool check_winner (fsm_t *p_this)
{
    fsm_simone_t *p_simone =(fsm_simone_t *)p_this;
    bool win = false;
    /*check if the player has finished the sequence*/
    if (p_simone->player_idx >= p_simone->seq_idx)
    {
        /*check of the sequence has rrived to the max lenght*/
        if (p_simone->seq_idx >= SEQUENCE_LENGTH)
        {
            /*check if the difficulty is the max*/
            if (p_simone->level == LEVEL_HARD)
            {
                win = true;
            }
        }
    }
    return win;
}

/**
 * @brief 	Check if the player key timeout has occurred. This is, the player has taken too long to press a key. 
 * 
 * @param p_this Pointer to an fsm_t struct than contains an fsm_simone_t.
 * @return true the player timeout has expired
 * @return false 
 */
static bool check_player_key_timeout (fsm_t *p_this)
{
    /*check of the player timeout has expired*/
    return port_simone_get_timeout_status();
}

/**
 * @brief 	Check if the player has finished the current sequence. 
 * 
 * @param p_this Pointer to an fsm_t struct than contains an fsm_simone_t.
 * @return true the player has finished the round
 * @return false 
 */
static bool check_player_round_end (fsm_t *p_this)
{
    fsm_simone_t *p_simone =(fsm_simone_t *)p_this;

    /*check if the player has inputed all the sequence*/
    if (p_simone->player_idx >= p_simone->seq_idx)
    {
        /*sheck if the player has played the max lenght or the max difficulty*/
        if (p_simone->seq_idx < SEQUENCE_LENGTH || p_simone->level != LEVEL_HARD)
        {
            return false;
        }
    }
    return true;
}

/**
 * @brief 	Check if any key has been pressed by the player. 
 * 
 * @param p_this Pointer to an fsm_t struct than contains an fsm_simone_t.
 * @return true the player has pressed a key
 * @return false 
 */
static bool check_any_key_pressed (fsm_t *p_this)
{
    fsm_simone_t *p_simone =(fsm_simone_t *)p_this;
    return p_simone->p_fsm_keyboard->key_value != p_simone->p_fsm_keyboard->invalid_key;
}


/* State machine output or action functions */

/**
 * @brief 	Initialize the Simone game. 
 * 
 * @param p_this Pointer to an fsm_t struct than contains an fsm_simone_t.
 */
static void do_init_game(fsm_t *p_this)
{
    fsm_simone_t *p_simone =(fsm_simone_t *)p_this;
    /*reset the duration of the user button*/
    p_simone->p_fsm_button->duration = 0;
    /*reset the keyboard key*/
    p_simone->p_fsm_keyboard->key_value = p_simone->p_fsm_keyboard->invalid_key;
    /*reset the index*/
    p_simone->seq_idx = 0;
    p_simone->playback_idx = 0;
    p_simone->player_idx = 0;
    /*reset the playback over*/
    p_simone->playback_over = false;
    /*reset the player key*/
    p_simone->player_key = p_simone->p_fsm_keyboard->invalid_key;

    /*initialize the level difficulty*/
    p_simone->level = LEVEL_EASY;

    /*initialize the color sequence*/
    for (uint8_t i = 0; i < SEQUENCE_LENGTH; i++)
    {
        p_simone->seq_colors[i] = color_off;
    }
    /*initialize the intensity sequence*/
    for (uint8_t i = 0; i < SEQUENCE_LENGTH; i++)
    {
        p_simone->seq_intensities[i] = 0;
    }
    
    /*add a color tto the sequence*/
    _add_color(p_simone);

    /*set the light to active*/
    p_simone->p_fsm_rgb_light->status = true;

    printf("[SIMONE][%ld] Simone game INIT\n", port_system_get_millis());
}

/**
 * @brief Turn the Simone system IDLE. 
 * 
 * @param p_this Pointer to an fsm_t struct than contains an fsm_simone_t.
 */
static void do_stop_simone (fsm_t *p_this)
{
    fsm_simone_t *p_simone =(fsm_simone_t *)p_this;
    /*reset the duration*/
    p_simone->p_fsm_button->duration = 0;
    /*reset led status*/
    p_simone->p_fsm_rgb_light->status = false;
    /*reset level*/
    p_simone->level = LEVEL_EASY;
    printf("[SIMONE][%ld] Simone game ended, you can press the button to start a new game\n", port_system_get_millis());
}

/**
 * @brief 	Start the low power mode while the Simone is IDLE. 
 * 
 * @param p_this Pointer to an fsm_t struct than contains an fsm_simone_t.
 */
static void do_sleep_idle(fsm_t *p_this)
{
    fsm_simone_t *p_simone =(fsm_simone_t *)p_this;
    port_system_sleep();
}

/**
 * @brief 	Handle the playback of the current sequence to the player.
 * 
 * @param p_this Pointer to an fsm_t struct than contains an fsm_simone_t.
 */
static void do_playback (fsm_t *p_this)
{
    fsm_simone_t *p_simone =(fsm_simone_t *)p_this;
    if (p_simone->playback_idx < p_simone->seq_idx)
    {
        return;
    }
    /*reset the flag timer and stop the scan of the keyboard*/
    port_simone_set_timeout_status(false);
    port_keyboard_stop_scan(p_simone->p_fsm_keyboard);

    if(p_simone->playback_over)
    {
        /*Set the light to off*/
        port_rgb_light_set_rgb(p_simone->p_fsm_rgb_light->rgb_light_id,color_off);
        /*Set timer duration for timer between colors*/
        port_simone_set_timer_timeout(SIMONE_TIME_OFF_BETWEEN_COLORS_MS);
        /*increase playback idx*/
        p_simone->playback_idx++;
        /*reset playback over*/
        p_simone->playback_over = false;

        if(p_simone->playback_idx >= p_simone->seq_idx)
        {
            p_simone->playback_idx = -1;
        }
        return;
    }
    else
    {
        /*get color and intensity from arrays*/
        rgb_color_t color = p_simone->seq_colors[p_simone->playback_idx];
        uint8_t intensity = p_simone->seq_intensities[p_simone->playback_idx];
        /*set color and intensity*/
        fsm_rgb_light_set_color_intensity(p_simone->p_fsm_rgb_light,color,intensity);
        /*select on duration depending on difilculty*/
        uint32_t tim;
        switch (p_simone->level)
        {
        case LEVEL_EASY:
            tim = SIMONE_TIME_ON_LEVEL_EASY_MS;
            break;
        case LEVEL_MEDIUM:
            tim = SIMONE_TIME_ON_LEVEL_MEDIUM_MS;
            break;
        case LEVEL_HARD:
            tim = SIMONE_TIME_ON_LEVEL_HARD_MS;
            break;
        default:
            tim = SIMONE_TIME_ON_LEVEL_EASY_MS;
            break;
        }
        port_simone_set_timer_timeout(tim);

        /*set playback over*/
        p_simone->playback_over = true;

        return;
    }
}

/**
 * @brief 	Start the player input sequence. Prepare the system for player input. 
 * 
 * @param p_this Pointer to an fsm_t struct than contains an fsm_simone_t.
 */
static void do_start_player_sequence (fsm_t *p_this)
{
    fsm_simone_t *p_simone =(fsm_simone_t *)p_this;
    /*reset playback_over flag*/
    p_simone->playback_over = false;
    p_simone->playback_idx = 0;
    /*reset player index*/
    p_simone->player_idx = 0;
    /*set off the led*/
    fsm_rgb_light_set_color_intensity(p_simone->p_fsm_rgb_light,color_off,0);
    /*set timeout*/
    port_simone_set_timer_timeout(SIMONE_TIME_WAIT_INPUT_MS);
    /*start keyboard scanning*/
    fsm_keyboard_start_scan(p_simone->p_fsm_keyboard);
    
    printf("[SIMONE][%ld] It is your tourn, you have ", SIMONE_TIME_WAIT_INPUT_MS/1000," seconds. \n");

}

/**
 * @brief 	Start the low power mode while the Simone is PLAYBACK. 
 * 
 * @param p_this Pointer to an fsm_t struct than contains an fsm_simone_t.
 */
static void do_sleep_playback (fsm_t *p_this)
{
    /*call the sleep function*/
    port_system_sleep();
}

/**
 * @brief Handle the winning status of the program
 * 
 * @param p_this Pointer to an fsm_t struct than contains an fsm_simone_t.
 */
static void do_winner (fsm_t *p_this)
{
    /*stop the timer*/
    port_simone_stop_timer();
    printf("[SIMONE][%ld] Congratulations, you have been able to remember ", SEQUENCE_LENGTH," colors\n");
}

/**
 * @brief Handle the game over status of the game. 
 * 
 * @param p_this Pointer to an fsm_t struct than contains an fsm_simone_t.
 */
static void do_game_over_timeout ( fsm_t *p_this) 
{
    fsm_simone_t *p_simone =(fsm_simone_t *)p_this;
    /*stop the timer*/
    port_simone_stop_timer();
    
    printf("[SIMONE][%ld] Game Over, you have been able to remember ", p_simone->seq_idx -1," colors\n");
    /*reset the index and elements of the fsm structure*/
    p_simone->seq_idx = 0;
    p_simone->player_idx = 0;
    p_simone->player_key = p_simone->p_fsm_keyboard->invalid_key;
    p_simone->level = LEVEL_EASY;

    /*stop the scan*/
    fsm_keyboard_stop_scan(p_simone->p_fsm_keyboard);

    port_system_sleep();
}

/**
 * @brief 	Add a new color to the current sequence. 
 * 
 * @param p_this Pointer to an fsm_t struct than contains an fsm_simone_t.
 */
void do_add_color (fsm_t *p_this)
{
    fsm_simone_t *p_simone =(fsm_simone_t *)p_this;
    /*reset the player index, playback index and playback over flag*/
    p_simone->playback_idx = 0;
    p_simone->player_idx = 0;
    p_simone->playback_over = false;
    /*TODO revisar*/
    /*check if the array is full and the difficulty is less tha difficult*/
    if (p_simone->seq_idx >= SEQUENCE_LENGTH && (p_simone->level == LEVEL_EASY) && (p_simone->level == LEVEL_MEDIUM))
    {
        /*increase level*/
        switch (p_simone->level)
        {
        case LEVEL_EASY:
            p_simone->level = LEVEL_MEDIUM;
            break;
        case LEVEL_MEDIUM:
            p_simone->level = LEVEL_HARD;
            break;
        default:
            break;
        }
        /*reset seq_idx*/
        p_simone->seq_idx = 0;
        printf("[SIMONE][%ld] The difficulty has increased to ", p_simone->level,"\n");
    }
    _add_color(p_simone);
    
}

/**
 * @brief 	Capture the player's key input and provide visual feedback. 
 * 
 * @param p_this Pointer to an fsm_t struct than contains an fsm_simone_t.
 */
static void do_capture_input (fsm_t *p_this)
{
    fsm_simone_t *p_simone =(fsm_simone_t *)p_this;
    /*save the key pressed in the player key*/
    p_simone->player_key = p_simone->p_fsm_keyboard->key_value;
    /*reset the value of key presed*/
    p_simone->p_fsm_keyboard->key_value = p_simone->p_fsm_keyboard->invalid_key;

    /*get the color from the key*/
    rgb_color_t color = _get_color_from_key(p_simone->player_key);
    /*set the color in the led*/
    fsm_rgb_light_set_color_intensity(p_simone->p_fsm_rgb_light,color,MAX_LEVEL_INTENSITY);

    /*set the timeout for visual feedback*/
    port_simone_set_timer_timeout(SIMONE_TIME_VISUAL_FEEDBACK_MS);
}

static void fsm_simone_init(fsm_simone_t *p_fsm_simone, fsm_button_t *p_fsm_button, uint32_t on_off_press_time_ms, fsm_keyboard_t *p_fsm_keyboard, fsm_rgb_light_t *p_fsm_rgb_light, uint8_t level)
{
    /* TODO students */

    /*set the seed*/
    srand(port_system_get_millis());
}

fsm_simone_t *fsm_simone_new(fsm_button_t *p_fsm_button, uint32_t on_off_press_time_ms, fsm_keyboard_t *p_fsm_keyboard, fsm_rgb_light_t *p_fsm_rgb_light, uint8_t level)
{
    /* TODO students */
}