#include <iostream>
#include <fstream>
#include <sys/time.h>
#include <cstdlib>

#include "bdd.h"
#include "kernel.h"
#include "utils.h"
#include <core.h>

const int INITBDDNODES       = 50000;
const int INITBDDCACHE       = 10000000;
const int INITBDDMAXINCREASE = 30000;

void show_results(int num_dead, int num_core);
