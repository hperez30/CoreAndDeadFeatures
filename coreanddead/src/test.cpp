#include "test.hpp"

namespace fs = std::filesystem;

std::vector<fs::path> get_dir_files(const fs::path& directorio);

int main(int argc, char **argv)
{	
	int method;	
	int in_file, out_file;
	
	in_file = -1;
	out_file = -1;

	if (read_params(argc, argv, &method, &in_file, &out_file)){
		return 1;
	}			
	
	struct timeval t_ini, t_fin;	
	
	switch (method) {
		case 1: // brute force approach
		{						
			bdd_init(INITBDDNODES, INITBDDCACHE);
			bdd_setmaxincrease(INITBDDMAXINCREASE);

		#ifdef REORDER
			/* compile with REORDER flag if you want to use automatic reordering techniques */
			bdd_autoreorder(BDD_REORDER_SIFT);
		#endif	
			bdd solutionSpace;	
			bdd_fnload((char *)argv[2], solutionSpace);
			std::cerr << "Model " << argv[2] << " loaded" << std::endl;

			bdd_varblockall();
			bdd_reorder(BDD_REORDER_SIFT);

			int *num_dead_core;
			num_dead_core = (int *)malloc(2*sizeof(int));
			
			for (int i=0; i < 2; ++i)
				num_dead_core[i] = 0;

			gettimeofday(&t_ini, NULL);	
			for (int j = 0; j < bdd_varnum(); ++j)
			{
				if ((solutionSpace & bdd_nithvar(j)) == bdd_false())
					num_dead_core[1]++;
				else if ((solutionSpace & bdd_ithvar(j)) == bdd_false())
					num_dead_core[0]++;
			}			
			gettimeofday(&t_fin, NULL);
			
			if (out_file > 0){
				show_results((char *)argv[2], bdd_varnum(), method, num_dead_core, timeval_diff(&t_fin, &t_ini), (char *)argv[out_file]);
			} else{
				show_results((char *)argv[2], bdd_varnum(), method, num_dead_core, timeval_diff(&t_fin, &t_ini));		
			}	
			
			free(num_dead_core);
			bdd_done();
			break;
		}			

		case 2: // our approach	
		{
			bdd_init(INITBDDNODES, INITBDDCACHE);
			bdd_setmaxincrease(INITBDDMAXINCREASE);

		#ifdef REORDER
			/* compile with REORDER flag if you want to use automatic reordering techniques */
			bdd_autoreorder(BDD_REORDER_SIFT);
		#endif	
			bdd solutionSpace;	
			bdd_fnload((char *)argv[2], solutionSpace);
			std::cerr << "Model " << argv[2] << " loaded" << std::endl;

			bdd_varblockall();
			bdd_reorder(BDD_REORDER_SIFT);
			
			std::unordered_map<uint32_t, bool> marks;
			marks.rehash(bdd_nodecount(solutionSpace));
			std::unordered_map<uint32_t, uint32_t> res_node;
			res_node.rehash(bdd_nodecount(solutionSpace));

			int *num_dead_core;
			int *var_low = (int *)calloc(bdd_varnum(), sizeof(int));
			int *var_high = (int *)calloc(bdd_varnum(), sizeof(int));
						
			gettimeofday(&t_ini, NULL);	
			get_dependencies_conflicts(solutionSpace, var_low, var_high, marks, res_node);

			num_dead_core = (int *)malloc(2*sizeof(int));
			num_dead_core = dead_features(var_high, var_low, bdd_varnum());

			gettimeofday(&t_fin, NULL);
			
			if (out_file > 0){
				show_results((char *)argv[2], bdd_varnum(), method, num_dead_core, timeval_diff(&t_fin, &t_ini), (char *)argv[out_file]);
			} else{
				show_results((char *)argv[2], bdd_varnum(), method, num_dead_core, timeval_diff(&t_fin, &t_ini));		
			}					

			free(var_low);
			free(var_high);						
			free(num_dead_core);
			bdd_done();
			break;
		}

		case 3:
		{			
			std::string directorio = "samples";   
			auto archivos = get_dir_files(directorio);
			
			for (const auto& archivo : archivos) {
				std::cerr << "Procesing file: " << archivo.string() << std::endl;
				
				bdd_init(INITBDDNODES, INITBDDCACHE);
				bdd_setmaxincrease(INITBDDMAXINCREASE);
				
				bdd solutionSpace;	
				bdd_fnload((char *)archivo.string().c_str(), solutionSpace);
				std::cerr << "Model " << archivo.string() << " loaded" << std::endl;

				bdd_varblockall();
				bdd_reorder(BDD_REORDER_SIFT);

				int *num_dead_core;
				int *var_low = (int *)calloc(bdd_varnum(), sizeof(int));
				int *var_high = (int *)calloc(bdd_varnum(), sizeof(int));				
				std::unordered_map<uint32_t, bool> marks;
				marks.rehash(bdd_nodecount(solutionSpace));				
				std::unordered_map<uint32_t, uint32_t> res_node;
				res_node.rehash(bdd_nodecount(solutionSpace));

				gettimeofday(&t_ini, NULL);	
				get_dependencies_conflicts(solutionSpace, var_low, var_high, marks, res_node);

				num_dead_core = (int *)malloc(2*sizeof(int));
				num_dead_core = dead_features(var_high, var_low, bdd_varnum());

				gettimeofday(&t_fin, NULL);
				if (out_file > 0){
					show_results((char *)archivo.string().c_str(), bdd_varnum(), method, num_dead_core, timeval_diff(&t_fin, &t_ini), (char *)argv[out_file]);
				} else{
					show_results((char *)archivo.string().c_str(), bdd_varnum(), method, num_dead_core, timeval_diff(&t_fin, &t_ini));		
				}

				free(var_low);
				free(var_high);							
				free(num_dead_core);
				bdd_done();
			}						
			break;
		}

		default:
			std::cerr << "Invalid " << method << " method " << std::endl;
	} 	

} // end main

int read_params(int argc, char **argv, int *method, int *in_file, int *out_file) {
	int option, i;
	std::string filePath, outfile;

	i = 0;
	while ((option = getopt(argc, argv, "f:m:o:")) != -1) {
		i++;
        switch (option) {
            case 'f':				
				*in_file = ++i;					
				filePath = optarg;											               
                break;
			case 'm':				
                *method = atoi(optarg);
				++i;				
                break;
			case 'o':
				*out_file = ++i;		
				outfile = optarg;											               												               
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

	if (*in_file > 0){
		std::ifstream mybdd(filePath);
		if (!mybdd.is_open()) {
			std::cerr << "The model could not be opened: " << filePath << std::endl;
			return 1;
		}
	}
	
	if (*out_file > 0){
		std::ifstream outstream(outfile);
		if (!outstream.is_open()) {
			std::cerr << "The output file could not be opened: " << outfile << std::endl;
			return 1;
		}
	}

	return 0;
}

std::vector<fs::path> get_dir_files(const fs::path& directorio) {
    std::vector<fs::path> archivos;

    try {        
        for (const auto& entry : fs::directory_iterator(directorio)) {
            if (fs::is_regular_file(entry)) {                
                archivos.push_back(entry.path());				
            } else if (fs::is_directory(entry)) {                
                auto archivosSubdirectorio = get_dir_files(entry.path());                
                archivos.insert(archivos.end(), archivosSubdirectorio.begin(), archivosSubdirectorio.end());
            }
        }
    } catch (const fs::filesystem_error& ex) {
        std::cerr << "Error, the directory could not be read: " << ex.what() << std::endl;
    }

    return archivos;
}

void show_results(const char *model, int num_vars, int method, int *num_dead_core, double timediff) {
	std::cout << "model, num vars, method, core, dead, time(secs)" << std::endl;
	std::cout << model << ", "<< num_vars << ", " << method << ", " << num_dead_core[1] << ", " << num_dead_core[0] << ", " << timediff << std::endl;
}

void show_results(const char *model, int num_vars, int method, int *num_dead_core, double timediff, const char *outputfile) {	
	auto original_stdout = std::cout.rdbuf();		
	std::ofstream file(outputfile, std::ios_base::app);
	std::cout.rdbuf(file.rdbuf());					
	std::cout << model << ", " << num_vars << ", " << method << ", " << num_dead_core[1] << ", " << num_dead_core[0] << ", " << timediff << std::endl;
	std::cout.rdbuf(original_stdout);
}