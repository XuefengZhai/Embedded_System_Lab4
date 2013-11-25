#include <types.h>
#include <task.h>

ssize_t read(int fd, void *buf, size_t count);
ssize_t write(int fd, const void *buf, size_t count);
void sleep(unsigned long millis);
unsigned long time();
int mutex_create();
int mutex_lock(int mutex);
int mutex_unlock(int mutex);
int task_create(task_t * tasks, size_t num_tasks);
int event_wait(unsigned int dev);
void invalid_syscall(unsigned int call_num);

