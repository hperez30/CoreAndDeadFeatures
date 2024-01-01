#include <iostream>
#include <filesystem>
#include <fstream>
#include <sys/time.h>

double timeval_diff(struct timeval *a, struct timeval *b);
void show_elapsed_time(struct timeval *t_fin, struct timeval *t_ini);
void show_results(const char *model, int num_vars, uint8_t method, int32_t *num_dead_core, double timediff, const char *outputfile);