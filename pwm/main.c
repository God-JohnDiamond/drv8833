/*
 * @Author: John Diamond
 * @Date: 2020-12-23 11:51:03
 * @LastEditors: John Diamond
 * @LastEditTime: 2020-12-24 17:51:56
 * @FilePath: /code/pwm/main.c
 */

#include "drv8833.h"

int main(int argc, char *argv[])
{
	int pwm_fd;
	int i, n, dir = 0, zf = 0;
	if(argc != 4)
		printf("Usage %s [zoom 1 or focus 2] [dirction] [pwm num]\n", argv[0]);

	pwm_fd = init_fh_pwm();

	//while (1){
		//for(i = 0; i < 2; i++)
		if(argc == 4)
		{
			zf = atoi(argv[1]);
			dir = atoi(argv[2]);
			n = atoi(argv[3]);
			if(dir == 1){
				if(zf == 1)
					ZoomForward(pwm_fd, n);
				else if(zf == 2)
					FocusForward(pwm_fd, n);
			}
			else if(dir == 0){
				if(zf == 1)
					ZoomReverse(pwm_fd, n);
				else if(zf == 2)
					FocusReverse(pwm_fd, n);
			}
		}
		//FocusForward(pwm_fd, 1);
		//sleep(1);

		//ZoomReverse(pwm_fd, 1);
		//FocusReverse(pwm_fd, 1);
		//sleep(1);
	//}
	
	
	exit_fh_pwm(pwm_fd);

}


