#include "test.hpp"

int main(int argc, char **argv)
{	
	int method;	

	if (read_params(argc, argv, &method)){
		return 1;
	}

	/* Initilize buddy */
	bdd_init(INITBDDNODES, INITBDDCACHE);
	bdd_setmaxincrease(INITBDDMAXINCREASE);

#ifdef REORDER
	/* compile with REORDER flag if you want to use automatic reordering techniques */
	bdd_autoreorder(BDD_REORDER_SIFT);
#endif

	bdd solutionSpace;	
	bdd_fnload((char *)argv[2], solutionSpace);
	std::cerr << "Model " << argv[1] << " loaded" << std::endl;

	bdd_varblockall();
	bdd_reorder(BDD_REORDER_SIFT);
	
	struct timeval t_ini, t_fin;
	gettimeofday(&t_ini, NULL);	
	int *num_dead_core;

	switch (method) {
		case 1: // brute force approach
		{
			num_dead_core = (int *)malloc(2*sizeof(int));
			for (int i=0; i < 2; ++i)
				num_dead_core[i] = 0;

			for (int j = 0; j < bdd_varnum(); ++j)
			{
				if ((solutionSpace & bdd_nithvar(j)) == bdd_false())
					num_dead_core[1]++;
				else if ((solutionSpace & bdd_ithvar(j)) == bdd_false())
					num_dead_core[0]++;
			}

			show_results(num_dead_core[0], num_dead_core[1]);
			gettimeofday(&t_fin, NULL);
			show_elapsed_time(&t_fin, &t_ini);
			
			free(num_dead_core);
			break;
		}			

		case 2: // our approach	
		{
			int *var_low = (int *)calloc(bdd_varnum(), sizeof(int));
			int *var_high = (int *)calloc(bdd_varnum(), sizeof(int));
			int *marks = (int *)calloc(bddnodesize, sizeof(int));
			int *res_node = (int *)calloc(bddnodesize, sizeof(int));

			get_dependencies_conflicts(solutionSpace, var_low, var_high, marks, res_node);

			num_dead_core = (int *)malloc(2*sizeof(int));
			num_dead_core = dead_features(var_high, var_low, bdd_varnum());

			show_results(num_dead_core[0], num_dead_core[1]);
			gettimeofday(&t_fin, NULL);
			show_elapsed_time(&t_fin, &t_ini);

			free(var_low);
			free(var_high);
			free(marks);
			free(res_node);
			free(num_dead_core);
			break;
		}					

		default:
			std::cout << "invalid " << method << " method " << std::endl;
	} 
	bdd_done();

} // end main

void show_results(int num_dead, int num_core) {
	std::cout << "core: " << num_core << std::endl;
	std::cout << "dead: " << num_dead << std::endl;
}

int read_params(int argc, char **argv, int *method) {
	int option;
	std::string filePath;

	while ((option = getopt(argc, argv, "f:m:")) != -1) {
        switch (option) {
            case 'f':
				filePath = optarg;												               
                break;
			case 'm':
				std::cerr << optarg << std::endl;
                *method = atoi(optarg);
                break;
            case '?':
                if (optopt == 'f') {
                    std::cerr << "Option -f requires a valid bdd file path." << std::endl;
                } else {
					if (optopt == 'm') {
						std::cerr << "Option -m requires a valid method (1) brute force approach (2) our method." << std::endl;
					}
					else {
                    	std::cerr << "Uknown Option: -" << static_cast<char>(optopt) << std::endl;
					}
                }
                return 1;
            default:
                std::cerr << "Usage: " << argv[0] << " -f <path_to_bdd_file>" << std::endl;
                return 1;
        }
    }

	std::ifstream mybdd(filePath);
	if (!mybdd.is_open()) {
		std::cerr << "The model could not be opened: " << filePath << std::endl;
		return 1;
	}

	return 0;
}