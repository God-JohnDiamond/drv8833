/*
 * @Author: John Diamond
 * @Date: 2020-12-23 11:51:03
 * @LastEditors: John Diamond
 * @LastEditTime: 2020-12-23 16:07:17
 * @FilePath: /code/pwm/pwm.c
 */
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>
#include "pwm.h"

void forward(int fd)
{

}

void reverse(int fd)
{
	
}


int main(int argc, char *argv[])
{
	int pwm_fd;
	int j;
	struct fh_pwm_chip_data chip_data = {0};
	int i;

	pwm_fd = open("/dev/fh_pwm", O_RDWR | O_SYNC);
	if(pwm_fd == -1)
	{
		printf("open failed\n");
		return -1;
	}

	for(j = 0; j < 100; j++){
		for(i = 0; i < 4; i++){
			switch(i){
				case 0: chip_data.id = 2; break;
				case 1: chip_data.id = 1; break;
				case 2: chip_data.id = 3; break;
				case 3: chip_data.id = 0; break;
			}
			if(-1 == ioctl(pwm_fd, GET_PWM_DUTY_CYCLE, &chip_data))
			{
				printf("ioctl0 failed\n");
			}
			chip_data.config.period_ns = 20000;		// 20us
			chip_data.config.duty_ns = 19999;		// duty 100%
			chip_data.config.pulses = 500;			// 500 times * 20us = 10ms
			chip_data.config.stop = FH_PWM_STOPLVL_LOW;
			chip_data.config.delay_ns = 0;

			if(-1 == ioctl(pwm_fd, SET_PWM_DUTY_CYCLE, &chip_data))
			{
				printf("ioctl1 failed\n");	
			}

			if(-1 == ioctl(pwm_fd, ENABLE_MUL_PWM, 0xff))
			{
				printf("ioctl1 failed\n");
			}

			if(-1 == ioctl(pwm_fd, ENABLE_PWM, &chip_data))
			{
				printf("ioctl2 failed\n");
			}
			usleep(1000);
			//while(chip_data.status.done_cnt != chip_data.config.pulses);

			if(-1 == ioctl(pwm_fd, DISABLE_PWM, &chip_data))
			{
				printf("ioctl3 failed\n");
			}
			printf("id = %d  done_cnt=%d   total_cnt=%d\n", chip_data.id, chip_data.status.done_cnt, chip_data.status.total_cnt);
		}
	}

	//sleep(2);

	close(pwm_fd);

}


