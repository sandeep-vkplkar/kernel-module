#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/wait.h>
void *thread_function(void *arg)
{
printf("Thread active. PID: %d, TID: %ld\n", getpid(), pthread_self());
sleep(1);
return NULL;
}
int main()
{
pid_t pid;
pthread_t tid;
int i, j;
for (i = 0; i < 3; i++) {
pid = fork();

if (pid < 0) {
perror("fork error");
exit(EXIT_FAILURE);
}
else if (pid == 0) {
printf("New child process initiated. PID: %d\n", getpid());
for (j = 0; j < 2; j++) {
if (pthread_create(&tid, NULL, thread_function, NULL) != 0) {
perror("pthread creation failed");
exit(EXIT_FAILURE);
}
}
for (j = 0; j < 2; j++) {
pthread_join(tid, NULL);
}
exit(EXIT_SUCCESS);
}
}
for (i = 0; i < 3; i++) {
wait(NULL);
}
printf("All child processes and threads have completed.\n");
return 0;
}
