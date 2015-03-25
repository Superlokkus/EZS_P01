//
//  main.c
//  EZS_P1
//
//  Created by Markus Klemm on 25.03.15.
//  Copyright (c) 2015 net.markusklemm. All rights reserved.
//

#include <stdio.h>
#include <time.h>


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
    
    sprintf(time_string,"%d h %d min %d sec\n",tm.tm_hour,tm.tm_min,tm.tm_sec);
    return time_string;
}

int main(int argc, const char * argv[]) {
    
    char output[64];
    
    while (1) {
        printf("%s",display_time(output));
    }
    
    return 0;
}
