/*
 * @Author: John Diamond
 * @Date: 2020-12-24 12:44:02
 * @LastEditors: John Diamond
 * @LastEditTime: 2020-12-28 16:23:33
 * @FilePath: /pwm/drv8833.c
 */

#include "drv8833.h"

int init_fh_pwm(struct datafile *localdata, unsigned int *focus_config)
{
	int pwm_fd;
	char buf[20];
	FILE *data_fd;

	pwm_fd = open("/dev/fh_pwm", O_RDWR | O_SYNC);			// pwm device file
	if(pwm_fd == -1)
	{
		printf("open failed\n");
		return -1;
	}
	
	if(-1 == (access("zoom_config", F_OK))){				// local config file no exit
		printf("there is no zoom_config file\n");
		ZoomForward(pwm_fd, 520);							// set two motor to 0
		FocusForward(pwm_fd, 520);
		localdata->zoom = 1;
		localdata->focus = 0;
		FocusReverse(pwm_fd, localdata->focus);
		data_fd = fopen("zoom_config", "wb");
		if(NULL == data_fd){
			printf("open local zoom file failed\n");
			return -1;
		}
		fwrite(localdata, 1, sizeof(struct datafile), data_fd);
	}
	else{
		data_fd = fopen("zoom_config", "rb");
		if(NULL == data_fd){
			printf("open local zoom file failed\n");
		}
		fread(localdata, 1, sizeof(struct datafile), data_fd);
	}

	fclose(data_fd);

	//////////////////// for focus config file ///////////////////////
	FILE *focus_fd;
	int j;

	//unsigned int focus_pwm[8] = {14, 32, 65, 102, 145, 227, 350, 510};
	unsigned int focus_pwm[8] = {0, 35, 65, 102, 148, 222, 355, 497};
	if(-1 == (access("focus_config", F_OK))){				// local config file no exit
		printf("there is no focus_config file\n");
		
		for(j = 0; j < 8; j++){
			focus_config[j] = focus_pwm[j];
		}
		focus_fd = fopen("focus_config", "wb");
		if(NULL == focus_fd){
			printf("open focus_config file failed\n");
			return -1;
		}
		fwrite(focus_pwm, 1, sizeof(focus_pwm), focus_fd);
	}
	else{
		focus_fd = fopen("focus_config", "rb");
		if(NULL == focus_fd){
			printf("open focus_config file failed\n");
		}
		fread(focus_config, 1, sizeof(focus_pwm), focus_fd);
		printf("focus_pwm byte %d", sizeof(focus_pwm));
	}

	fclose(focus_fd);
	/////////////////////// end focus config file ///////////////////////

	return pwm_fd;
}

int exit_fh_pwm(int fd, struct datafile *localdata, unsigned int *focus_pwm)
{
	FILE *data_fd;
	FILE *focus_fd;

	// write server data to local config file
	data_fd = fopen("zoom_config", "wb");
	fwrite(localdata, 1, sizeof(struct datafile), data_fd);
	fclose(data_fd);

	focus_fd = fopen("focus_config", "wb");
	fwrite(focus_pwm, 8, sizeof(focus_pwm), focus_fd);				//数组作为函数参数传进来 便与指针一样 不能获取到数组的长度
	fclose(focus_fd);

	close(fd);

	return 0;
}

unsigned int SetZoom(int pwm_fd, struct datafile *localdata, struct datafile *serverdata, unsigned int *focus_config)
{
	unsigned int zoom_pwm_fac = 74;											// 74.2=520/7
	unsigned int focus_pwm_num = 1;
	unsigned int zoom_num = 0;
	unsigned int level_8_pwm_num = 35;
	
	if(localdata->zoom == serverdata->zoom){
		printf("zoom config no update, do nothing\n");
		return 0;
	}

	if(localdata->zoom < serverdata->zoom){											// config increase
		if(serverdata->zoom == 8){
			zoom_num = (serverdata->zoom - localdata->zoom - 1) * zoom_pwm_fac + level_8_pwm_num;	// zoom local -> server.zoom,(-1 ... +65 for without level 8)
		}
		else{
			zoom_num = (serverdata->zoom - localdata->zoom) * zoom_pwm_fac;
		}

		ZoomReverse(pwm_fd, zoom_num);											// zoom level 1 --> 8
		FocusForward(pwm_fd, localdata->focus);									// foucus reset

		switch (serverdata->zoom){
			case 1: localdata->focus = focus_config[serverdata->zoom - 1]; break;
			case 2: localdata->focus = focus_config[serverdata->zoom - 1]; break;
			case 3: localdata->focus = focus_config[serverdata->zoom - 1]; break;
			case 4: localdata->focus = focus_config[serverdata->zoom - 1]; break;
			case 5: localdata->focus = focus_config[serverdata->zoom - 1]; break;
			case 6: localdata->focus = focus_config[serverdata->zoom - 1]; break;
			case 7: localdata->focus = focus_config[serverdata->zoom - 1]; break;
			case 8: localdata->focus = focus_config[serverdata->zoom - 1]; break;
			default: localdata->focus = 0; break;
		}
		//printf("focus_config0=%d\n", localdata->focus);

		FocusReverse(pwm_fd, localdata->focus);										// focus 
		localdata->zoom = serverdata->zoom;
		return 0;
	}
	else if(localdata->zoom > serverdata->zoom){									// config dec
		if(localdata->zoom == 8){
			zoom_num = (localdata->zoom - serverdata->zoom - 1) * zoom_pwm_fac + level_8_pwm_num;	// zoom serverlocal -> local.zoom,(-1 ... +65 for without level 8)
		}
		else{
			zoom_num = (localdata->zoom - serverdata->zoom) * zoom_pwm_fac;
		}
		ZoomForward(pwm_fd, zoom_num);												// zoom level 8 --> 1
		FocusForward(pwm_fd, localdata->focus);										// foucus reset

		switch (serverdata->zoom){
			case 1: localdata->focus = focus_config[serverdata->zoom - 1]; break;
			case 2: localdata->focus = focus_config[serverdata->zoom - 1]; break;
			case 3: localdata->focus = focus_config[serverdata->zoom - 1]; break;
			case 4: localdata->focus = focus_config[serverdata->zoom - 1]; break;
			case 5: localdata->focus = focus_config[serverdata->zoom - 1]; break;
			case 6: localdata->focus = focus_config[serverdata->zoom - 1]; break;
			case 7: localdata->focus = focus_config[serverdata->zoom - 1]; break;
			case 8: localdata->focus = focus_config[serverdata->zoom - 1]; break;
			default: localdata->focus = 0; break;
		}
		localdata->focus += 3;														// 8-->1 gain
		FocusReverse(pwm_fd, localdata->focus);										// foucus forward
		localdata->zoom = serverdata->zoom;
		return 0;
	}
	
	return 0;
}

unsigned int SetFocus(int pwm_fd, struct datafile *localdata, struct datafile *server, unsigned int *focus_config)
{
	int focus_pwm_num = 1;
	if(server->focus == 0){
		printf("focus config no update, do nothing\n");
		return 0;
	}
	else if(server->focus > 0){
		FocusReverse(pwm_fd, server->focus);				// focus forward
		localdata->focus += server->focus;
		focus_config[server->zoom - 1] = localdata->focus;
		return 0;
	}
	else if(server->focus < 0){
		FocusForward(pwm_fd, (-server->focus));			// focus back
		if(localdata->focus != 0)
			localdata->focus += server->focus;
		focus_config[server->zoom - 1] = localdata->focus;
		return 0;
	}
	
	return 0;
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
		chip_data.config.period_ns = PERIOD_NS;	// 
		chip_data.config.duty_ns = DUTY_NS;		// duty 50% 
		chip_data.config.pulses = n;			// n times
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
		if(-1 == ioctl(pwm_fd, GET_PWM_DUTY_CYCLE, &chip_data))
		{
			printf("ioctl get failed\n");
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
		if(-1 == ioctl(pwm_fd, GET_PWM_DUTY_CYCLE, &chip_data))
		{
			printf("ioctl get failed\n");
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

	usleep((PERIOD_NS * n / 1000));						// hold pwm time 1s

	for(i = 4; i < 8; i++){						// disable all pwm
		chip_data.id = i;
		if(-1 == ioctl(pwm_fd, DISABLE_PWM, &chip_data))
		{
			printf("ioctl disable pwm%d failed\n", i);
		}
		if(-1 == ioctl(pwm_fd, GET_PWM_DUTY_CYCLE, &chip_data))
		{
			printf("ioctl get failed\n");
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

	usleep((PERIOD_NS * n / 1000));								// hold pwm time 1s

	for(i = 4; i < 8; i++){						// disable all pwm
		chip_data.id = i;
		if(-1 == ioctl(pwm_fd, DISABLE_PWM, &chip_data))
		{
			printf("ioctl disable pwm%d failed\n", i);
		}
		if(-1 == ioctl(pwm_fd, GET_PWM_DUTY_CYCLE, &chip_data))
		{
			printf("ioctl get failed\n");
		}
		printf("id %d  done_cnt %d  total_cnt %d\n", chip_data.id, chip_data.status.done_cnt, chip_data.status.total_cnt);
	}
	return;
}
