//
//  main.c
//  EZS_P1
//
//  Created by Markus Klemm on 25.03.15.
//  Copyright (c) 2015 net.markusklemm. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include <errno.h>
#include <unistd.h>
#include <signal.h>
#include <ncurses.h>
#include <assert.h>


/* Following function is a wrapper due missing C11 implementation on OSX and is not my work, but is just a wrapper for missing osx functionality from https://gist.github.com/jbenet/1087739*/
#ifdef __MACH__
#include <mach/clock.h>
#include <mach/mach.h>
#endif
#ifdef __MACH__ // OS X does not have clock_gettime, use clock_get_time
#define CLOCK_REALTIME 42
int clock_gettime(int i,struct timespec *ts) {
    if (i != 42)
    {
        errno = EINVAL;
        return -1;
    }
    clock_serv_t cclock;
    mach_timespec_t mts;
    host_get_clock_service(mach_host_self(), CALENDAR_CLOCK, &cclock);
    clock_get_time(cclock, &mts);
    mach_port_deallocate(mach_task_self(), cclock);
    ts->tv_sec = mts.tv_sec;
    ts->tv_nsec = mts.tv_nsec;
    return 0;
    }
#endif
/*End of foreign work*/


/*! Prints out current hour, minute, second, and 10th of a second as null terminated string into given buffer
 @param time_string Buffer to write time string into, at least 32 bytes long
 @attention Buffer must be at least 32 bytes long.
 @return Also returns a pointer to the given buffer
 */
char* display_time (char *time_string)
{
    time_t t = time(NULL);
    
    struct tm tm;
    localtime_r(&t, &tm);
    struct timespec ts;
   #ifdef __MACH__
    clock_gettime(CLOCK_REALTIME,&ts);
#else
    timespec_get(&ts, TIME_UTC);
#endif
    
    sprintf(time_string,"%d h %d min %d.%.0f sec \n",tm.tm_hour,tm.tm_min,tm.tm_sec,ts.tv_nsec / (float)100000000);
    return time_string;
}

void my_beep(int i)
{
    assert(i == SIGALRM);
    static int count = 0;
    count++;
    fprintf(stderr,"%d. Sekunde\n",count);
//    beep(); Linking Problems OSX
//    flash();
    const int max = 5;
    if (count == max)
    {
        fprintf(stderr, "%d. Sekunde erreicht, beende.\n",max);
        exit(EXIT_SUCCESS);
    }
}

int main(int argc, const char * argv[]) {
    
    if (signal(SIGALRM, my_beep) == SIG_ERR)
    {
        perror("Cannot set alarm handler");
    }
    struct itimerval itimerval = {{1,0},{1,0}};
    
    if (setitimer(ITIMER_REAL, &itimerval, NULL) != 0) {
        perror("Cannot set timer");
    }
    
    char output[64];
    
    while (1) {
        printf("%s",display_time(output));
        sleep(1);
    }
    
    return 0;
}
