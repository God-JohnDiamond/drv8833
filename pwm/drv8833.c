/*
 * @Author: John Diamond
 * @Date: 2020-12-24 12:44:02
 * @LastEditors: John Diamond
 * @LastEditTime: 2020-12-24 15:40:07
 * @FilePath: /code/pwm/drv8833.c
 */

#include "drv8833.h"

int init_fh_pwm(void)
{
	int pwm_fd;

	pwm_fd = open("/dev/fh_pwm", O_RDWR | O_SYNC);
	if(pwm_fd == -1)
	{
		printf("open failed\n");
		return -1;
	}
	return pwm_fd;
}

void exit_fh_pwm(int fd)
{
	close(fd);
}

void ZoomForward(int pwm_fd, int n)
{
	int i;
	struct fh_pwm_chip_data chip_data = {0};

	for(i = 0; i < 4; i++){
		chip_data.id = i;
		if(-1 == ioctl(pwm_fd, GET_PWM_DUTY_CYCLE, &chip_data))
		{
			printf("ioctl get param failed\n");
		}
		chip_data.config.period_ns = PERIOD_NS;	// 10ms
		chip_data.config.duty_ns = DUTY_NS;		// duty 50% 5ms
		chip_data.config.pulses = n;			// n * 2 times * 20us
		chip_data.config.stop = FH_PWM_STOPLVL_LOW;

		switch (i)								// set pwm enable time
		{
		case 0:
			chip_data.config.delay_ns = (PERIOD_NS / 4 * 2);
			break;
		case 1:
			chip_data.config.delay_ns = 0;
			break;
		case 2:
			chip_data.config.delay_ns = (PERIOD_NS / 4 * 3);
			break;
		case 3:
			chip_data.config.delay_ns = (PERIOD_NS / 4 * 1);
			break;
		}

		if(-1 == ioctl(pwm_fd, SET_PWM_DUTY_CYCLE, &chip_data))
		{
			printf("ioctl set param failed\n");	
		}

	}

	for(i = 0; i < 4; i++){					// enable pwm 1 0
		chip_data.id = i;
		if(-1 == ioctl(pwm_fd, ENABLE_PWM, &chip_data))
		{
			printf("ioctl enble pwm%d failed\n", i);
		}
	}

	usleep((PERIOD_NS * n / 1000));								// hold pwm time 1s

	for(i = 0; i < 4; i++){						// disable all pwm
		chip_data.id = i;
		if(-1 == ioctl(pwm_fd, DISABLE_PWM, &chip_data))
		{
			printf("ioctl disable pwm%d failed\n", i);
		}
		printf("id %d  done_cnt %d  total_cnt %d\n", chip_data.id, chip_data.status.done_cnt, chip_data.status.total_cnt);
	}
	return;
}

void ZoomReverse(int pwm_fd, int n)
{
	int i;
	struct fh_pwm_chip_data chip_data = {0};

	for(i = 0; i < 4; i++){
		chip_data.id = i;
		if(-1 == ioctl(pwm_fd, GET_PWM_DUTY_CYCLE, &chip_data))
		{
			printf("ioctl get param failed\n");
		}
		chip_data.config.period_ns = PERIOD_NS;	// 10ms
		chip_data.config.duty_ns = DUTY_NS;		// duty 50% 5ms
		chip_data.config.pulses = n;			// n * 2 times * 20us
		chip_data.config.stop = FH_PWM_STOPLVL_LOW;

		switch (i)								// set pwm enable time
		{
		case 0:
			chip_data.config.delay_ns = (PERIOD_NS / 4 * 2);
			break;
		case 1:
			chip_data.config.delay_ns = 0;
			break;
		case 2:
			chip_data.config.delay_ns = (PERIOD_NS / 4 * 1);
			break;
		case 3:
			chip_data.config.delay_ns = (PERIOD_NS / 4 * 3);
			break;
		}

		if(-1 == ioctl(pwm_fd, SET_PWM_DUTY_CYCLE, &chip_data))
		{
			printf("ioctl set param failed\n");	
		}

	}

	for(i = 0; i < 4; i++){					// enable pwm 1 0
		chip_data.id = i;
		if(-1 == ioctl(pwm_fd, ENABLE_PWM, &chip_data))
		{
			printf("ioctl enble pwm%d failed\n", i);
		}
	}

	usleep((PERIOD_NS * n / 1000));								// hold pwm time 1s

	for(i = 0; i < 4; i++){						// disable all pwm
		chip_data.id = i;
		if(-1 == ioctl(pwm_fd, DISABLE_PWM, &chip_data))
		{
			printf("ioctl disable pwm%d failed\n", i);
		}
		printf("id %d  done_cnt %d  total_cnt %d\n", chip_data.id, chip_data.status.done_cnt, chip_data.status.total_cnt);
	}
	return;
}


void FocusForward(int pwm_fd, int n)
{
	int i;
	struct fh_pwm_chip_data chip_data = {0};

	for(i = 4; i < 8; i++){
		chip_data.id = i;
		if(-1 == ioctl(pwm_fd, GET_PWM_DUTY_CYCLE, &chip_data))
		{
			printf("ioctl get param failed\n");
		}
		chip_data.config.period_ns = PERIOD_NS;	// 10ms
		chip_data.config.duty_ns = DUTY_NS;		// duty 50% 5ms
		chip_data.config.pulses = n;			// n * 2 times * 20us
		chip_data.config.stop = FH_PWM_STOPLVL_LOW;

		switch (i)								// set pwm enable time
		{
		case 4:
			chip_data.config.delay_ns = (PERIOD_NS / 4 * 2);
			break;
		case 5:
			chip_data.config.delay_ns = 0;
			break;
		case 6:
			chip_data.config.delay_ns = (PERIOD_NS / 4 * 3);
			break;
		case 7:
			chip_data.config.delay_ns = (PERIOD_NS / 4 * 1);
			break;
		}

		if(-1 == ioctl(pwm_fd, SET_PWM_DUTY_CYCLE, &chip_data))
		{
			printf("ioctl set param failed\n");	
		}

	}

	for(i = 4; i < 8; i++){					// enable pwm 1 0
		chip_data.id = i;
		if(-1 == ioctl(pwm_fd, ENABLE_PWM, &chip_data))
		{
			printf("ioctl enble pwm%d failed\n", i);
		}
	}

	usleep((PERIOD_NS * n / 1000));						// hold pwm time 1s

	for(i = 4; i < 8; i++){						// disable all pwm
		chip_data.id = i;
		if(-1 == ioctl(pwm_fd, DISABLE_PWM, &chip_data))
		{
			printf("ioctl disable pwm%d failed\n", i);
		}
		printf("id %d  done_cnt %d  total_cnt %d\n", chip_data.id, chip_data.status.done_cnt, chip_data.status.total_cnt);
	}
	return;
}

void FocusReverse(int pwm_fd, int n)
{
		int i;
	struct fh_pwm_chip_data chip_data = {0};

	for(i = 4; i < 8; i++){
		chip_data.id = i;
		if(-1 == ioctl(pwm_fd, GET_PWM_DUTY_CYCLE, &chip_data))
		{
			printf("ioctl get param failed\n");
		}
		chip_data.config.period_ns = PERIOD_NS;	// 10ms
		chip_data.config.duty_ns = DUTY_NS;		// duty 50% 5ms
		chip_data.config.pulses = n;			// n * 2 times * 20us
		chip_data.config.stop = FH_PWM_STOPLVL_LOW;

		switch (i)								// set pwm enable time
		{
		case 4:
			chip_data.config.delay_ns = (PERIOD_NS / 4 * 2);
			break;
		case 5:
			chip_data.config.delay_ns = 0;
			break;
		case 6:
			chip_data.config.delay_ns = (PERIOD_NS / 4 * 1);
			break;
		case 7:
			chip_data.config.delay_ns = (PERIOD_NS / 4 * 3);
			break;
		}

		if(-1 == ioctl(pwm_fd, SET_PWM_DUTY_CYCLE, &chip_data))
		{
			printf("ioctl set param failed\n");	
		}

	}

	for(i = 4; i < 8; i++){					// enable pwm 1 0
		chip_data.id = i;
		if(-1 == ioctl(pwm_fd, ENABLE_PWM, &chip_data))
		{
			printf("ioctl enble pwm%d failed\n", i);
		}
	}

	usleep((PERIOD_NS * n / 1000));								// hold pwm time 1s

	for(i = 4; i < 8; i++){						// disable all pwm
		chip_data.id = i;
		if(-1 == ioctl(pwm_fd, DISABLE_PWM, &chip_data))
		{
			printf("ioctl disable pwm%d failed\n", i);
		}
		printf("id %d  done_cnt %d  total_cnt %d\n", chip_data.id, chip_data.status.done_cnt, chip_data.status.total_cnt);
	}
	return;
}
