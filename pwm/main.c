/*
 * @Author: John Diamond
 * @Date: 2020-12-23 11:51:03
 * @LastEditors: John Diamond
 * @LastEditTime: 2020-12-24 12:46:12
 * @FilePath: /code/pwm/main.c
 */

#include "drv8833.h"

int main(int argc, char *argv[])
{
	int pwm_fd;
	int i;

	pwm_fd = init_fh_pwm();

	while (1){
		//for(i = 0; i < 2; i++)
		ZoomForward(pwm_fd, 1);
		sleep(1);

		ZoomReverse(pwm_fd, 1);
		sleep(1);
	}
	
	
	exit_fh_pwm(pwm_fd);

}


