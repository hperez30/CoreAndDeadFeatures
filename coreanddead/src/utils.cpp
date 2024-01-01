#include "utils.hpp"

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

void show_results_cli(const char *model, int num_vars, uint8_t method, int32_t *num_dead_core, double timediff) {
	std::cout << "model: " << model << std::endl;
	std::cout << "var num: " << num_vars << std::endl;
	std::cout << "method: " << static_cast<int32_t>(method) << std::endl;
	std::cout << "core: " << num_dead_core[1] <<  std::endl;
	std::cout << "dead: " << num_dead_core[0] << std::endl;
	std::cout << "time(secs)" << timediff << std::endl;	
}

void show_results_file(const char *model, int num_vars, uint8_t method, int32_t *num_dead_core, double timediff, const char *outputfile) {
	auto original_stdout = std::cout.rdbuf();		
	std::ofstream file(outputfile, std::ios_base::app);
	std::cout.rdbuf(file.rdbuf());					
	std::cout << model << ", " << num_vars << ", " << method << ", " << num_dead_core[1] << ", " << num_dead_core[0] << ", " << timediff << std::endl;
	std::cout.rdbuf(original_stdout);
}

void show_results(const char *model, int num_vars, uint8_t method, int32_t *num_dead_core, double timediff, const char *outputfile) {
	if (outputfile != nullptr){
		show_results_file(model, num_vars, method, num_dead_core, timediff, outputfile);
	} else{
		show_results_cli(model, num_vars, method, num_dead_core, timediff);
	}				
}