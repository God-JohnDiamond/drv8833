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

	if(argc != 3){
	printf("Usage: %s [zoom level 1-8] [focus level +-100]\n", argv[0]);
	return -1;
	}
	
	////////////////////// for debug input server data //////////////
	unsigned int x = atoi(argv[1]);
	unsigned int y = atoi(argv[2]);
	struct datafile serverdata = {x, y};
	
	if(argc == 3){
		if(x < 1 || x > 8){
			printf("x=%d y=%d\n", x, y);
			printf("1 <= zoom <= 8  and focus belong +-100\n");
			return -1;
		}
		data_fd = fopen("server_zoom_config", "wb");	
		fwrite(&serverdata, 1, sizeof(struct datafile), data_fd);
		printf("serverdata.zoom=%d  serverdata.focus=%d\n", serverdata.zoom, serverdata.focus);
		fclose(data_fd);
	}

	data_fd = fopen("server_zoom_config", "rb");
	if(NULL == data_fd){
		printf("open datafile failed\n");
		return -1;
	}

	fread(&serverdata, 1, sizeof(struct datafile), data_fd);
	printf("serverdata.zoom=%d  serverdata.focus=%d\n", serverdata.zoom, serverdata.focus);
	fclose(data_fd);
	/////////////////////////////////////////////////////////////////

	pwm_fd = init_fh_pwm(&localdata, focus_config);
	if(pwm_fd != -1){
		printf("localdata.zoom=%d  localdata.focus=%d\n", localdata.zoom, localdata.focus);
		printf("focus_config0=%d\n", *focus_config);
	}

	SetZoom(pwm_fd, &localdata, &serverdata, focus_config);

	SetFocus(pwm_fd, localdata.focus, serverdata.focus);
	
	exit_fh_pwm(pwm_fd, &localdata);
}


