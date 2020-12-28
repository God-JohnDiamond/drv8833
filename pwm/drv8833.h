/*
 * @Author: John Diamond
 * @Date: 2020-12-24 12:44:15
 * @LastEditors: John Diamond
 * @LastEditTime: 2020-12-28 16:03:23
 * @FilePath: /pwm/drv8833.h
 */
#ifndef DRV8833_H_
#define DRV8833_H_

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <string.h>

#define PWM_IOCTL_MAGIC				'p'
#define ENABLE_PWM					_IOWR(PWM_IOCTL_MAGIC, 0, unsigned int)
#define DISABLE_PWM					_IOWR(PWM_IOCTL_MAGIC, 1, unsigned int)

#define SET_PWM_DUTY_CYCLE			_IOWR(PWM_IOCTL_MAGIC, 2, unsigned int)
#define GET_PWM_DUTY_CYCLE			_IOWR(PWM_IOCTL_MAGIC, 3, unsigned int)
#define SET_PWM_DUTY_CYCLE_PERCENT	_IOWR(PWM_IOCTL_MAGIC, 4, unsigned int)
#define SET_PWM_ENABLE				_IOWR(PWM_IOCTL_MAGIC, 5, unsigned int)
#define ENABLE_MUL_PWM				_IOWR(PWM_IOCTL_MAGIC, 6, unsigned int)
#define ENABLE_FINSHALL_INTR		_IOWR(PWM_IOCTL_MAGIC, 7, unsigned int)
#define ENABLE_FINSHONCE_INTR		_IOWR(PWM_IOCTL_MAGIC, 8, unsigned int)
#define DISABLE_FINSHALL_INTR		_IOWR(PWM_IOCTL_MAGIC, 9, unsigned int)
#define DISABLE_FINSHONCE_INTR		_IOWR(PWM_IOCTL_MAGIC, 10, unsigned int)
#define WAIT_PWM_FINSHALL			_IOWR(PWM_IOCTL_MAGIC, 12, unsigned int)


#define PERIOD_NS	3000000			// 10ms
#define DUTY_NS		(PERIOD_NS / 2)	// duty 50% 5ms
//#define PULSES	100				// pwm output num  for debug

struct datafile
{
	unsigned int zoom;
	unsigned int focus;
};



struct fh_pwm_config
{
	unsigned int period_ns;
	unsigned int duty_ns;
	unsigned int pulses;
#define FH_PWM_STOPLVL_LOW		(0x0)
#define FH_PWM_STOPLVL_HIGH		(0x3)
#define FH_PWM_STOPLVL_KEEP		(0x1)

#define FH_PWM_STOPCTRL_ATONCE		(0x10)
#define FH_PWM_STOPCTRL_AFTERFINISH	(0x00)
	unsigned int stop;
	unsigned int delay_ns;
	unsigned int phase_ns;
	unsigned int percent;
	unsigned int finish_once;
	unsigned int finish_all;
};

struct fh_pwm_status
{
	unsigned int done_cnt;
	unsigned int total_cnt;
	unsigned int busy;
	unsigned int error;
};

typedef struct { } spinlock_t;

struct list_head {
	struct list_head *next, *prev;
};

struct __wait_queue_head {
	spinlock_t lock;
	struct list_head task_list;
};
typedef struct __wait_queue_head wait_queue_head_t;

struct fh_pwm_chip_data
{
	int id;
	struct fh_pwm_config config;
	struct fh_pwm_status status;
	wait_queue_head_t wait_done;
	void (*finishall_callback)(struct fh_pwm_chip_data *data);
	void (*finishonce_callback)(struct fh_pwm_chip_data *data);
};


int init_fh_pwm(struct datafile *localdata, unsigned int *focus_config);
int exit_fh_pwm(int fd, struct datafile *data);
unsigned int SetZoom(int pwm_fd, struct datafile *localdata, struct datafile *serverdata, unsigned int *focus_config);
unsigned int SetFocus(int pwm_fd, int local, int server);
void ZoomForward(int pwm_fd, int n);
void ZoomReverse(int pwm_fd, int n);
void FocusForward(int pwm_fd, int n);
void FocusReverse(int pwm_fd, int n);

#endif