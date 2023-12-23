#include <iostream>
#include <fstream>
#include <sys/time.h>
#include <cstdlib>
#include <unistd.h>
#include <filesystem>
#include <vector>

#include "bdd.h"
#include "kernel.h"
#include <utils.hpp>
#include <core.hpp>

const int INITBDDNODES       = 50000;
const int INITBDDCACHE       = 10000000;
const int INITBDDMAXINCREASE = 30000;

void show_results(const char *model, int method, int *num_dead_core, double timediff, int out_file, char **argv);
void show_results(const char *model, int method, int *num_dead_core, double timediff);
int read_params(int argc, char **argv, int *method, int *in_file, int *out_file); 
