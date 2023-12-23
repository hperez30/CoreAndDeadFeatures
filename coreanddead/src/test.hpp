#include <iostream>
#include <fstream>
#include <sys/time.h>
#include <cstdlib>
#include <unistd.h>
#include <filesystem>
#include <vector>
#include <unordered_map>

#include "bdd.h"
#include <utils.hpp>
#include <core.hpp>

const int INITBDDNODES       = 50000;
const int INITBDDCACHE       = 10000000;
const int INITBDDMAXINCREASE = 30000;

void show_results(const char *model, int num_vars, int method, int *num_dead_core, double timediff, const char *outputfile);
void show_results(const char *model, int num_vars, int method, int *num_dead_core, double timediff);
int read_params(int argc, char **argv, int *method, int *in_file, int *out_file); 
