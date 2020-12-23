#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>

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


int main(int argc, char *argv[])
{
	int pwm_fd;
	struct fh_pwm_chip_data chip_data = {0};
	
	chip_data.id = 3;

	pwm_fd = open("/dev/fh_pwm", O_RDWR | O_SYNC);
	if(pwm_fd == -1)
	{
		printf("open failed\n");
		return -1;
	}
	if(-1 == ioctl(pwm_fd, GET_PWM_DUTY_CYCLE, &chip_data))
	{
		printf("ioctl0 failed\n");
		return -1;
	}

	chip_data.config.period_ns = 20000;
	chip_data.config.duty_ns = 10000;
	chip_data.config.pulses = 5;
	chip_data.config.stop = FH_PWM_STOPLVL_HIGH;
	//chip_data.config.delay_ns = 10000;
	chip_data.config.phase_ns = 10000;
	//chip_data.config.percent = 50;

	if(-1 == ioctl(pwm_fd, SET_PWM_DUTY_CYCLE, &chip_data))
	{
		printf("ioctl1 failed\n");
		return -1;		
	}

	if(-1 == ioctl(pwm_fd, ENABLE_PWM, &chip_data))
	{
		printf("ioctl2 failed\n");
		return -1;
	}
	usleep(150);

	if(-1 == ioctl(pwm_fd, DISABLE_PWM, &chip_data))
	{
		printf("ioctl3 failed\n");
		return -1;
	}

	printf("done_cnt=%d   total_cnt=%d   busy=%d \n", chip_data.status.done_cnt, chip_data.status.total_cnt, chip_data.status.busy);

	close(pwm_fd);

}


