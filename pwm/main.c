/*
 * @Author: John Diamond
 * @Date: 2020-12-23 11:51:03
 * @LastEditors: John Diamond
 * @LastEditTime: 2020-12-28 16:11:56
 * @FilePath: /pwm/main.c
 */

#include "drv8833.h"

int main(int argc, char *argv[])
{
	int pwm_fd;
	FILE *data_fd;
	struct datafile localdata = {0};
	unsigned int focus_config[8] = {0};
	struct datafile serverdata = {0};

	unsigned int cnt = 20;

	int sel;
	unsigned int pwm_num;
	int saved_data[2] = {0}; // zoom[0]  focus [1]
	FILE *debug;

	if(argc != 3){
	printf("Usage: %s [zoom level 1-8] [focus level +-]\n", argv[0]);
	return -1;
	}

	//serverdata.zoom = atoi(argv[1]);
	//serverdata.focus = atoi(argv[2]);
	sel = atoi(argv[1]);
	pwm_num = atoi(argv[2]);
	
	system("echo 1 > /sys/class/gpio/GPIO62/value");			// drv8833 power on
	system("echo 1 > /sys/class/gpio/GPIO25/value");			// select 1
	system("echo 1 > /sys/class/gpio/GPIO52/value");			// select 2

	pwm_fd = init_fh_pwm(&localdata, focus_config);
	if(pwm_fd == -1){
		printf("open pwm failed\n");
		return -1;
	}

	//SetZoom(pwm_fd, &localdata, &serverdata, focus_config);
	//SetFocus(pwm_fd, &localdata, &serverdata, focus_config);;

	switch (sel)
	{
		case -1: ZoomForward(pwm_fd, pwm_num); break;
		case 1: ZoomReverse(pwm_fd, pwm_num); break;
		case -2: FocusForward(pwm_fd, pwm_num); break;
		case 2: FocusReverse(pwm_fd, pwm_num); break;
		default: 
			ZoomForward(pwm_fd, 520);
			ZoomReverse(pwm_fd, 20);
			ZoomForward(pwm_fd, 20);		// find 0

			FocusForward(pwm_fd, 520);
			FocusReverse(pwm_fd, 20);
			FocusForward(pwm_fd, 20);
	break;
	}

/*	while(cnt--){
		ZoomForward(pwm_fd, 100);
		ZoomReverse(pwm_fd, 100);
	}
*/
	exit_fh_pwm(pwm_fd, &localdata, focus_config);

	system("echo 0 > /sys/class/gpio/GPIO62/value");
	system("echo 0 > /sys/class/gpio/GPIO25/value");
	system("echo 0 > /sys/class/gpio/GPIO52/value");			// select 1
}


