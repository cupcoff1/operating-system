/*
 *  fork_test.c
 *  version 3
 *  Created on: 2010-5-29
 *      Author: wangth
 */
#include <unistd.h>
#include <stdio.h>
int main(void)
{
    int i = 0;
    for (i = 0; i < 3; i++) {
        pid_t fpid = fork();
        if (fpid == 0)
            printf("son\n");
        else
            printf("father\n");
    }
    return 0;

}