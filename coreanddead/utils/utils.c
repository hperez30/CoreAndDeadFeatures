#include "utils.h"

/* return "a - b" in seconds */
double timeval_diff(struct timeval *a, struct timeval *b) {
	return (double)(a->tv_sec + (double)a->tv_usec / 1000000) -
		   (double)(b->tv_sec + (double)b->tv_usec / 1000000);
} // time_diff

void show_elapsed_time(struct timeval *t_fin, struct timeval *t_ini){
	double secs;

	secs = timeval_diff(t_fin, t_ini);
	std::cout << "time in: " << secs << "secs" << std::endl;
}