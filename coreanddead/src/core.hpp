#include "bdd.h"
#include <unordered_map>

int get_dependencies_conflicts(const bdd r, int *vars_low, int *vars_high, std::unordered_map<uint32_t, bool> &marks, std::unordered_map<uint32_t, uint32_t> &res_node);
int* dead_features(int *var_high, int *var_low, int num_var);

