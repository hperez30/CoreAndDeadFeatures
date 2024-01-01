#include <iostream>
#include <fstream>
#include <sys/time.h>
#include <cstdlib>
#include <unistd.h>
#include <filesystem>
#include <vector>
#include <unordered_map>
#include <cstdint>

#include "bdd.h"
#include <utils.hpp>
#include <core.hpp>

const int INITBDDNODES       = 50000;
const int INITBDDCACHE       = 10000000;
const int INITBDDMAXINCREASE = 30000;

const std::string directorio = "samples";
namespace fs = std::filesystem;

std::vector<fs::path> get_dir_files(const fs::path& directorio);
int read_params(int argc, char **argv, int8_t *method, int8_t *in_file, int8_t *out_file); 
void init_model(char *name, bdd *solutionSpace);
void all_samples_approach(uint8_t method, struct timeval t_ini, struct timeval t_fin, const char* outputfile);
void our_approach(char *model, uint8_t method, struct timeval t_ini, struct timeval t_fin, const char *outputfile);
void brute_force_approach(char *model, uint8_t method, struct timeval t_ini, struct timeval t_fin, const char *outputfile);