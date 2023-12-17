#include "test.h"

int main(int argc, char **argv)
{	
	if (argc != 3) {
		std::cerr << "Usage: " << argv[0] << " <bdd file> <option method>" << std::endl;
		exit(-1);
	}

	std::ifstream mybdd(argv[1]);
	int method = atoi(argv[2]);

	if (!mybdd) {
		std::cerr << "File " << argv[1] << " not found" << std::endl;
		exit(-1);
	}

	/* Initilize buddy */
	bdd_init(INITBDDNODES, INITBDDCACHE);
	bdd_setmaxincrease(INITBDDMAXINCREASE);
#ifdef REORDER
	/* compile with debug flag if you want to use automatic reordering techniques */
	bdd_autoreorder(BDD_REORDER_SIFT);
#endif

	bdd solutionSpace;

	bdd_fnload((char *)argv[1], solutionSpace);

	std::cerr << "File " << argv[1] << " loaded" << std::endl;

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
			std::cout << "invalid option " << method << std::endl;

	} // end switch

	bdd_done();

} // end main

void show_results(int num_dead, int num_core) {
	std::cout << "core: " << num_core << std::endl;
	std::cout << "dead: " << num_dead << std::endl;
}