/*
 * led.c
 *
 *  Created on: 16 May 2023
 *      Author: marcr
*/
/*

                                     .%@@@@@%
                                     .%@@@@@&(,
                                    /@@@@@@@@@/
                                    (@@@@@@@@@/
                                    (@@@@@@@@@/
                                    (@@@@@@@@@/
                                    (@@@@@@@@@/
                                  ./%@@@@@@@@@/
                                  *@@@@@@@@@@@@&,
                                  *@@@@@@@@@@@@@*
                                  *@@@@@@@@@@@@@*
                                 ,(@@@@@@@@@@@@@(,
                                .%@@@@@@@@@@@@@@@%.
                                .%@@@@@@@@@@@@@@@%.
                               /&@@@@@@%. .%@@@@@%.
                               (@@@@@@@#   #@@@@@@@(
                               (@@@@@@@#   #@@@@@@@(
                             *@@@@@@@&,    /%@@@@@@#*.
                             *@@@@@@@&,     ,&@@@@@@@*
                           .%@@@@@@@/       .(&@@@@@@(,
                           .%@@@@@@@/         /@@@@@@@%.
                          (@@@@@@@#   ,*******#@@@@@@@@@@@@@@@@@%.
                          (@@@@@@@@&&&@@@@@@@@@@@@@@@@@@@@@@@@@@%.
                     .**(@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@%.
                 .#&&@@@@@@@@@@@@@@@@@@@@@@@@@(../&@@@@@@@/......
             ,(@@@@@@@@@@@@@@@@&####*            .#&@@@@@@(,
             ./@@@@@@@@@@@@@@%,                    /@@@@@@@%.
                (@@@@@@@@@@@%/                     *%&@@@@@@@(
                .*&@@@@@@@@&,                        #@@@@@@@&%,
                (@@@@@@@@@/                           ,&@@@@@@@(.
              ,%&@@@@@@@%,                             .(@@@@@@@%.
             .(@@@@@@@&,                                /@@@@@@@&*.
           /%@@@@@@@@(,                                 .,%@@@@@@@&%,
         *@@@@@@@@@#                                      /&@@@@@@@@(.
          ,#@@@@@&/.                                        ,(@@@@@@@%.
            .(%%*                                            *%@@@@@@@@#,
                                                               #@@@@@@@@@*
                                                                .#&@@@@@@*
                                                                  /@@&/,,.
*/
#include "led.h"
#include "tim.h"


led_strip leds[NUMBER_OF_LEDS+50]; //	+50 is for reset bits
uint8_t led_tim_FLAG = 0;

S_EFFECTS var_chenillard;
S_EFFECTS pulse;

color current_color;

	/*
	 *
 ____     _____   __       _____   ____        ____    ____    ____    ______   __  __  ______  ______  ______   _____   __  __  ____
/\  _`\  /\  __`\/\ \     /\  __`\/\  _`\     /\  _`\ /\  _`\ /\  _`\ /\__  _\ /\ \/\ \/\__  _\/\__  _\/\__  _\ /\  __`\/\ \/\ \/\  _`\
\ \ \/\_\\ \ \/\ \ \ \    \ \ \/\ \ \ \L\ \   \ \ \/\ \ \ \L\_\ \ \L\_\/_/\ \/ \ \ `\\ \/_/\ \/\/_/\ \/\/_/\ \/ \ \ \/\ \ \ `\\ \ \,\L\_\
 \ \ \/_/_\ \ \ \ \ \ \  __\ \ \ \ \ \ ,  /    \ \ \ \ \ \  _\L\ \  _\/  \ \ \  \ \ , ` \ \ \ \   \ \ \   \ \ \  \ \ \ \ \ \ , ` \/_\__ \
  \ \ \L\ \\ \ \_\ \ \ \L\ \\ \ \_\ \ \ \\ \    \ \ \_\ \ \ \L\ \ \ \/    \_\ \__\ \ \`\ \ \_\ \__ \ \ \   \_\ \__\ \ \_\ \ \ \`\ \/\ \L\ \
   \ \____/ \ \_____\ \____/ \ \_____\ \_\ \_\   \ \____/\ \____/\ \_\    /\_____\\ \_\ \_\/\_____\ \ \_\  /\_____\\ \_____\ \_\ \_\ `\____\
    \/___/   \/_____/\/___/   \/_____/\/_/\/ /    \/___/  \/___/  \/_/    \/_____/ \/_/\/_/\/_____/  \/_/  \/_____/ \/_____/\/_/\/_/\/_____/
	 *
	 */

color AMPERE1 = {103,0,204};
color AMPERE2 = {128,0,128};
color BLACK = {0,0,0};
color BLUE = {0,0,255};
color RED = {255, 0, 0};
color GREEN = {0,255,0};
color WHITE = {255,255,255};
color YELLOW = {255,255,0};
color WHITE_DIM = {100,100,100};



/*
                                                        ▓▓████████████░░▓▓██
                                                      ▓▓░░░░░░░░░░▒▒░░░░░░▒▒▓▓
                                                      ██▓▓████▓▓██████▓▓▓▓▓▓██
                                                      ██░░░░░░░░██▓▓██░░██▓▓██
                                                    ██░░░░░░░░██▓▓██░░░░██▓▓██
                                                    ██░░░░░░░░██▓▓██░░░░██▓▓██
                                                    ▓▓░░░░░░▒▒██▓▓██░░▒▒██▓▓▓▓
                            ▓▓▓▓▓▓                ▓▓▒▒░░░░▒▒██▓▓██▒▒▒▒▒▒██▒▒▓▓▓▓▓▓▓▓
                          ██░░  ██      ████      ██▒▒████████▓▓██▒▒▒▒▒▒▓▓▓▓▓▓██░░░░████████
                          ██  ░░████▓▓▓▓░░░░▓▓▓▓▓▓████░░░░░░░░██▒▒▒▒▒▒▓▓▓▓▓▓▓▓██░░░░░░▒▒░░▒▒██
                          ██  ░░██░░░░██░░░░░░████░░░░░░░░░░░░██▒▒▒▒██▓▓▓▓▓▓▓▓██░░░░░░░░░░░░██
                          ██  ░░░░▓▓████░░░░██░░░░░░░░░░▓▓██▓▓██▓▓▓▓████▓▓████░░░░░░░░░░░░░░██
                        ████░░░░░░██▒▒██░░██  ░░░░░░████░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░██
                      ▓▓░░██░░░░░░██▒▒▒▒██░░░░░░░░▓▓████▓▓▓▓▓▓▒▒▒▒░░░░░░░░░░░░░░▓▓██▓▓▒▒▓▓░░░░██
                    ██  ░░░░██░░░░██▒▒██  ░░░░░░██▒▒▒▒▒▒▒▒▒▒▒▒██▒▒▒▒░░░░░░░░░░██▓▓▒▒▒▒▒▒▒▒██░░██
                  ██  ░░░░▒▒▒▒██░░░░██  ░░░░░░██▒▒▒▒▒▒▓▓▓▓▓▓▓▓▒▒██▒▒▒▒▒▒▒▒▒▒░░██▓▓▓▓▓▓▓▓▓▓▒▒████
          ██▓▓██████████████████▓▓████░░░░░░▒▒██▒▒▒▒▓▓▓▓▓▓██▓▓▓▓██▒▒▒▒░░░░░░░░██▓▓▓▓████▓▓▓▓██
          ██░░▒▒▓▓▓▓▓▓▓▓▓▓▓▓▓▓▒▒▓▓░░▒▒██░░▒▒██▒▒▒▒▒▒▓▓██▒▒▒▒██▓▓▓▓██▒▒▒▒▒▒░░░░▓▓▓▓██▒▒▒▒██▓▓▓▓██
          ██░░▒▒▒▒▓▓▓▓▓▓▓▓▓▓▓▓▒▒▓▓░░▒▒▒▒██▒▒██▒▒▒▒▒▒▓▓██▒▒▒▒██▓▓▓▓██▒▒░░░░░░░░▓▓▓▓██▒▒▒▒██▓▓▓▓██
          ██░░▒▒▒▒▓▓▓▓▓▓▓▓▓▓▓▓▓▓██░░▒▒▒▒██████▒▒▒▒▒▒▓▓██▒▒▒▒██▓▓▓▓██▒▒▒▒▒▒▒▒░░██▓▓██▒▒▒▒██▓▓▓▓██
        ██░░▒▒▒▒▒▒▓▓▓▓▓▓▓▓▓▓▓▓██░░▒▒▒▒▒▒██  ██▒▒▒▒▓▓▓▓██▒▒░░██▓▓▓▓████░░░░░░░░██▓▓██▒▒░░██▓▓▓▓██
        ██░░▒▒▒▒▒▒▓▓▓▓▓▓▓▓▓▓▓▓██░░▒▒▒▒▓▓██  ██▓▓▓▓▓▓▓▓██░░░░██▓▓▓▓██  ████████▓▓▓▓██░░░░██▓▓▓▓██
      ██░░▒▒▒▒▒▒▒▒▓▓▓▓▓▓▓▓▓▓██░░▒▒▒▒▒▒▓▓██    ██▓▓▓▓▓▓▓▓████▓▓▓▓██        ██▓▓▓▓▓▓▓▓████▓▓▓▓██
  ████░░▒▒▒▒▒▒▓▓▓▓▓▓▓▓▓▓▓▓██░░▒▒▒▒▒▒▒▒▓▓██    ██▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓██        ██▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓██
██░░░░░░░░░░░░▒▒▒▒▒▒░░░░░░░░▒▒▒▒▒▒▒▒▓▓██        ██▓▓▓▓▓▓▓▓▓▓▓▓██            ██▓▓▓▓▓▓▓▓▓▓▓▓██
██████████████████████████████████████            ████████████                ████████████
 */

//####################################################################################################
//######################################	INIT_VAR	##############################################
//####################################################################################################

void Init_var()
{
	var_chenillard.led_direction = 1;
	var_chenillard.led_counter = 0;
	pulse.first_loop = 1;
}

void set_all_leds(color led_color)
{
	for(uint8_t cur_led=0; cur_led<NUMBER_OF_LEDS;cur_led++)
	{
		set_specific_led(leds, cur_led, led_color);
	}
}
void Dim(color dim_color, uint8_t speed)
{
	TIM6->ARR = 1000/speed;	// speed unit is in 1/255 intensity change per second
	if(pulse.first_loop == 1) current_color = dim_color;
	pulse.first_loop = 0;

	if(led_tim_FLAG == 1)
	{
		if(dim_color.r>0)
		{
			current_color.r--;
		}
		if(dim_color.g>0)
		{
			current_color.g--;
		}
		if(dim_color.b>0)
		{
			current_color.b--;
		}
		set_all_leds(current_color);
		led_tim_FLAG = 0;
	}
}
void Brighten(color brighten_color, uint8_t speed)
{
	TIM6->ARR = 1000/speed;	// speed unit is in 1/255 intensity change per second
	if(pulse.first_loop == 1) current_color = BLACK;
	pulse.first_loop = 0;

	if(led_tim_FLAG == 1)
	{
		if(brighten_color.r>current_color.r)
		{
			current_color.r++;
		}
		if(brighten_color.g>current_color.g)
		{
			current_color.g++;
		}
		if(brighten_color.b>current_color.b)
		{
			current_color.b++;
		}
		set_all_leds(current_color);
		led_tim_FLAG = 0;
	}
}
void Pulse(color pulse_color, uint8_t speed)
{
	if( (current_color.r==pulse_color.r) & (current_color.g==pulse_color.g) & (current_color.b==pulse_color.b) )
	{
		pulse.led_direction = 0;
	}
	if( (current_color.r==BLACK.r) & (current_color.g==BLACK.g) & (current_color.b==BLACK.b) )
	{
		pulse.led_direction = 1;
	}

	if (pulse.led_direction == 0)
	{
		Dim(pulse_color, speed);
	}
	if (pulse.led_direction == 1)
	{
		Brighten(pulse_color, speed);
	}
}
/*
                                                                          ▒▒▒▒
                                                          ▒▒▒▒        ▒▒▒▒▒▒▒▒
                                                          ▒▒▒▒▒▒    ▒▒
                                                                ▒▒  ▒▒
                                                              ▒▒▒▒▒▒▒▒▒▒▒▒
                                                            ▒▒░░░░░░░░░░░░▒▒
                                                          ▒▒░░░░░░░░░░░░░░░░▒▒
                      ▒▒▒▒▒▒                ▒▒▒▒          ▒▒░░░░░░░░░░░░░░░░▒▒
                    ▒▒░░░░░░▒▒  ▒▒▒▒▒▒▒▒  ▒▒░░░░▒▒  ▒▒▒▒▒▒▒▒░░░░██░░░░██░░░░▒▒
              ▒▒▒▒▒▒░░░░░░░░░░▒▒░░░░░░░░▒▒░░░░░░░░▒▒░░░░░░▒▒░░░░░░░░░░░░░░░░▒▒
            ▒▒░░░░▒▒░░░░░░░░▒▒░░░░░░░░░░▒▒░░░░░░▒▒░░░░░░░░▒▒░░░░░░░░░░░░░░░░▒▒
          ▒▒░░░░░░▒▒░░░░░░░░▒▒░░░░░░░░░░▒▒░░░░░░▒▒░░░░░░░░▒▒░░░░░░░░░░░░░░░░▒▒
          ▒▒░░░░░░░░▒▒░░░░░░▒▒░░░░░░░░░░░░▒▒░░░░▒▒░░░░░░░░░░▒▒░░██░░░░██░░▒▒
            ▒▒░░░░░░░░▒▒▒▒▒▒  ▒▒░░░░░░░░▒▒  ▒▒▒▒  ▒▒░░░░░░▒▒  ▒▒░░████░░▒▒
              ▒▒▒▒▒▒▒▒    ░░    ▒▒▒▒▒▒▒▒    ░░    ░░▒▒▒▒▒▒░░    ▒▒▒▒▒▒▒▒
                ░░  ░░    ░░░░  ░░    ░░    ░░░░      ░░  ░░░░
                ░░  ░░░░        ░░░░  ░░░░            ░░░░
                ░░░░
 */
void chenillard(color bgrd_color, color moving_color, uint8_t speed)
{
	TIM6->ARR = LED_SPEED_RATIO/speed;		// speed unit is led/second

	if(led_tim_FLAG == 1)
	{
		if(var_chenillard.led_counter == 0)
		{
		  var_chenillard.led_direction = 1;
		}
		else if(var_chenillard.led_counter == NUMBER_OF_LEDS-1)
		{
		  var_chenillard.led_direction = 0;
		}
		if(var_chenillard.led_direction == 1)
		{
		  set_specific_led(leds, var_chenillard.led_counter, bgrd_color);
		  var_chenillard.led_counter++;
		  set_specific_led(leds, var_chenillard.led_counter, moving_color);
		}
		else
		{
		  set_specific_led(leds, var_chenillard.led_counter, bgrd_color);
		  var_chenillard.led_counter--;
		  set_specific_led(leds, var_chenillard.led_counter, moving_color);
		}
		led_tim_FLAG = 0;
	}
}
void reset_all_leds(led_strip * leds, uint16_t number_leds)
{
	for(uint16_t i=0; i<number_leds; i++)
	{
		for(uint8_t j=0; j<8; j++)
		{
			(leds+i) -> r[j] = LOGICAL_ZERO;
			(leds+i) -> g[j] = LOGICAL_ZERO;
			(leds+i) -> b[j] = LOGICAL_ZERO;
		}
	}
}
void set_specific_led(led_strip * leds, uint16_t led_position, color rgb_color)
{
	for(uint8_t j = 0; j < 8; j++)
	{
		if(rgb_color.r & 0x1 << j)	//	if bit is 1 (for red)
		{
			(leds+led_position) -> r[7-j] = LOGICAL_ONE;
		}
		else
		{
			(leds+led_position) -> r[7-j] = LOGICAL_ZERO;
		}

		if(rgb_color.g & 0x1 << j)	//	if bit is 1 (for green)
		{
			(leds+led_position) -> g[7-j] = LOGICAL_ONE;
		}
		else
		{
			(leds+led_position) -> g[7-j] = LOGICAL_ZERO;
		}

		if(rgb_color.b & 0x1 << j)	//	if bit is 1 (for blue)
		{
			(leds+led_position) -> b[7-j] = LOGICAL_ONE;
		}
		else
		{
			(leds+led_position) -> b[7-j] = LOGICAL_ZERO;
		}
	}
}

//###########################################################################################################
//########################################	CALLBACK FUNCTIONS	#############################################
//###########################################################################################################

void HAL_TIM_PWM_PulseFinishedCallback(TIM_HandleTypeDef *htim)
{
	__HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_1, 0);	//	sets pin state to 0 by lowering alpha to 0 -> alows for reset bits to be sent
	HAL_TIM_PWM_Stop_DMA(&htim4, TIM_CHANNEL_1);		//	stops sending data
}
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	if(htim==&htim6)
	{
		led_tim_FLAG = 1;
	}
}

//###########################################################################################################
//###################################	FUNCTIONS TO CALL IN MAIN	#########################################
//###########################################################################################################

void led_effect()
{
	set_specific_led(leds, 0, GREEN);
	set_specific_led(leds, 1, RED);
	set_specific_led(leds, 2, BLUE);
	//chenillard(BLACK, AMPERE2, 5);
//	Pulse(BLUE, 100);
}
void led_init()
{
	reset_all_leds(leds, NUMBER_OF_LEDS);
}
void led_send()
{
	HAL_TIM_PWM_Start_DMA(&htim4, TIM_CHANNEL_1, (uint32_t *)leds, (NUMBER_OF_LEDS+50)*24);
}
void led_set_test()
{
	set_specific_led(leds, 0, RED);
}
