#include "test.hpp"

int main(int argc, char **argv)
{	
	struct timeval t_ini, t_fin;
	int8_t method, in_file = -1, out_file = -1;
	const char *outputfile;

	if (read_params(argc, argv, &method, &in_file, &out_file)) { 
		return 1;
	}

	outputfile = (out_file > 0) ? (char *)argv[out_file]: nullptr;
		
	switch (method) {
		case 1: // brute force approach
		{							
			brute_force_approach((char *)argv[2], method, t_ini, t_fin, outputfile);
			break;
		}			

		case 2: // our approach	
		{
			our_approach((char *)argv[2], method, t_ini, t_fin, outputfile);
			break;
		}

		case 3: // all samples approach
		{						   
			all_samples_approach(method, t_ini, t_fin, outputfile);						
			break;
		}

		default:
			std::cerr << "Invalid " << method << " method " << std::endl;
	} 	

} // end main

void brute_force_approach(char *model, uint8_t method, struct timeval t_ini, struct timeval t_fin, const char *outputfile){
	bdd solutionSpace;	
	init_model(model, &solutionSpace);
	int32_t *num_dead_core = (int32_t *)malloc(2*sizeof(uint32_t));
	
	for (uint32_t i=0; i < 2; ++i)
		num_dead_core[i] = 0;

	gettimeofday(&t_ini, NULL);	
	for (int32_t j = 0; j < bdd_varnum(); ++j)
	{
		if ((solutionSpace & bdd_nithvar(j)) == bdd_false())
			num_dead_core[1]++;
		else if ((solutionSpace & bdd_ithvar(j)) == bdd_false())
			num_dead_core[0]++;
	}			
	gettimeofday(&t_fin, NULL);
		
	show_results(model, bdd_varnum(), method, num_dead_core, timeval_diff(&t_fin, &t_ini), outputfile);

	free(num_dead_core);
	bdd_done();
}
void our_approach(char *model, uint8_t method, struct timeval t_ini, struct timeval t_fin, const char *outputfile){
	bdd solutionSpace;	
	init_model(model, &solutionSpace);
	std::unordered_map<uint32_t, bool> marks;
	marks.rehash(bdd_nodecount(solutionSpace));
	std::unordered_map<uint32_t, uint32_t> res_node;
	res_node.rehash(bdd_nodecount(solutionSpace));

	int32_t *num_dead_core;
	int *vars = (int *)calloc(bdd_varnum(), sizeof(int));
				
	gettimeofday(&t_ini, NULL);	
	get_dependencies_conflicts(solutionSpace, vars, marks, res_node);

	num_dead_core = (int32_t *)malloc(2*sizeof(int32_t));
	num_dead_core = dead_features(vars, bdd_varnum());

	gettimeofday(&t_fin, NULL);
						
	show_results(model, bdd_varnum(), method, num_dead_core, timeval_diff(&t_fin, &t_ini), outputfile);

	free(vars);						
	free(num_dead_core);
	bdd_done();
}
void all_samples_approach(uint8_t method, struct timeval t_ini, struct timeval t_fin, const char *outputfile){
	auto archivos = get_dir_files(directorio);
			
	for (const auto& archivo : archivos) {
		std::cerr << "Procesing file: " << archivo.string() << std::endl;
		
		bdd solutionSpace;	
		init_model((char *)archivo.string().c_str(), &solutionSpace);				
		int *vars = (int *)calloc(bdd_varnum(), sizeof(int));				
		
		std::unordered_map<uint32_t, bool> marks;
		marks.rehash(bdd_nodecount(solutionSpace));				
		std::unordered_map<uint32_t, uint32_t> res_node;
		res_node.rehash(bdd_nodecount(solutionSpace));

		gettimeofday(&t_ini, NULL);	
		get_dependencies_conflicts(solutionSpace, vars, marks, res_node);

		int32_t *num_dead_core = (int32_t *)malloc(2*sizeof(int32_t));
		num_dead_core = dead_features(vars, bdd_varnum());

		gettimeofday(&t_fin, NULL);
						
		show_results((char *)archivo.string().c_str(), bdd_varnum(), method, 
			num_dead_core, timeval_diff(&t_fin, &t_ini), outputfile);

		free(vars);							
		free(num_dead_core);
		bdd_done();
	}
}
int read_params(int argc, char **argv, int8_t *method, int8_t *in_file, int8_t *out_file) {
	int8_t option, i;
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
				*method = std::atoi(optarg);
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
void init_model(char *name, bdd *solutionSpace){
	bdd_init(INITBDDNODES, INITBDDCACHE);
	bdd_setmaxincrease(INITBDDMAXINCREASE);

	#ifdef REORDER
		/* compile with REORDER flag if you want to use automatic reordering techniques */
		bdd_autoreorder(BDD_REORDER_SIFT);
	#endif			
		bdd_fnload(name, *solutionSpace);
		std::cerr << "Model " << name << " loaded" << std::endl;

		bdd_setvarnum(bdd_varnum());
		bdd_varblockall();
		bdd_reorder(BDD_REORDER_SIFT);
}
namespace fs = std::filesystem;
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