#ifdef WIN32
#include <time.h>
#include <sys/timeb.h>
#else
#include <sys/time.h>
#include <unistd.h>
#endif

/* Return the current time of day in milliseconds
 * Note that this will jump at midnight, but that's only a small percentage
 * of time.
 */
long get_time() {
#ifdef WIN32
        struct timeb tb;
        ftime(&tb);
        return 1000 * (tb.time % (24*60*60)) + tb.millitm;
#else
        struct timeval tv;
        static struct timezone tz = {0, 0};
        gettimeofday(&tv, &tz);
        return((tv.tv_sec%(24*60*60))*1000 + tv.tv_usec/1000);
#endif
}

