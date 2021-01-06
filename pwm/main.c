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
	

	if(argc != 3){
	printf("Usage: %s [zoom level 1-8] [focus level +-100]\n", argv[0]);
	return -1;
	}

	serverdata.zoom = atoi(argv[1]);
	serverdata.focus = atoi(argv[2]);
	

	pwm_fd = init_fh_pwm(&localdata, focus_config);
	if(pwm_fd != -1){
		printf("localdata.zoom=%d  localdata.focus=%d\n", localdata.zoom, localdata.focus);
	}

	SetZoom(pwm_fd, &localdata, &serverdata, focus_config);

	SetFocus(pwm_fd, &localdata, &serverdata, focus_config);

	exit_fh_pwm(pwm_fd, &localdata, focus_config);
	
	int i;
	printf("focus value be save to focus_config file\n values are:\n");
	for(i = 0; i < 8; i++){
		printf("zoom %d --> focus %d\n", (i+1), focus_config[i]);
	}

}


