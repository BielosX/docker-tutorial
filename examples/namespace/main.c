#define _GNU_SOURCE
#include <stdio.h>
#include <sched.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>
#include <sys/mount.h>

#define STACK_SIZE 4096

typedef struct new_proc_args {
    char** args;
    int pipe_fd[2];
} new_proc_args;

int namespace_func(void* arg) {
    new_proc_args* proc_args = (new_proc_args*)arg;
    char** a = proc_args->args;
    printf("PID: %d\n", getpid());
    printf("Running process %s\n", a[0]);
    dup2(proc_args->pipe_fd[1], STDOUT_FILENO);
    close(proc_args->pipe_fd[0]);
    close(proc_args->pipe_fd[1]);
    if(mount("proc", "/proc", "proc", MS_NOSUID|MS_NODEV|MS_NOEXEC, NULL) < 0) {
        perror("Unable to mount proc\n");
        _exit(-1);
    }
    if(execv(a[0], a) < 0) {
        perror("Execv failed\n");
        _exit(-1);
    }
    umount("/proc");
    return 0;
}

int main(int argc, char* argv[]) {
    char* stack;
    char* stack_top;
    int pid;
    int result;
    int pipe_fd[2];
    new_proc_args proc_args;
    char proc_out[4096];

    if (pipe(pipe_fd) < 0) {
        perror("Unable to create pipe\n");
        return -1;
    }
    stack = (char*)malloc(STACK_SIZE);
    if (stack == NULL) {
        return -1;
    }
    stack_top = stack + STACK_SIZE;
    proc_args.args = &argv[1];
    proc_args.pipe_fd[0] = pipe_fd[0];
    proc_args.pipe_fd[1] = pipe_fd[1];
    if (mount("none", "/", NULL, MS_REC|MS_PRIVATE, NULL) < 0) {
        perror("Unable to mount none to root\n");
        return -1;
    }
    if(mount("none", "/proc", NULL, MS_REC|MS_PRIVATE, NULL) < 0) {
        umount("/");
        perror("Unable to mount none to proc\n");
        return -1;
    }
    pid = clone(namespace_func, stack_top, CLONE_NEWPID | CLONE_NEWUSER | CLONE_NEWNS, &proc_args);
    if (pid < 0) {
        perror("Unable to clone process\n");
        free(stack);
        return -1;
    }
    read(pipe_fd[0], proc_out, sizeof(proc_out));
    printf("%s\n", proc_out);
    waitpid(pid, NULL, 0);
    free(stack);
    umount("/");
    umount("/proc");
    return 0;
}
