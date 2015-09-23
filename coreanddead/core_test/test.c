using namespace std;
#include <iostream>
#include <fstream>
#include <sys/time.h>

#include "bdd.h"
#include "kernel.h"
#include "core.h"

const int INITBDDNODES       = 50000;
const int INITBDDCACHE       = 10000000;
const int INITBDDMAXINCREASE = 30000;

/* return "a - b" in seconds */
double timeval_diff(struct timeval *a, struct timeval *b)
{
  return
    (double)(a->tv_sec + (double)a->tv_usec/1000000) -
    (double)(b->tv_sec + (double)b->tv_usec/1000000);
}//time_diff

int main(int argc, char **argv)  
{
	/* Initilize buddy */
	bdd_init(INITBDDNODES,INITBDDCACHE);
    bdd_setmaxincrease(INITBDDMAXINCREASE);
    
    struct timeval t_ini, t_fin;
    double secs;
    
	bdd solutionSpace;
    
    int core, dead, num_test;
    int *var_low, *var_high;
    int *marks, *res_node;
    
    if (argc != 3) 
    {
       cerr << "Usage: " << argv[0]
            << " <bdd file> <option method>" << endl;
        exit(-1);
    }
   
    ifstream mybdd(argv[1]);
    num_test = atoi(argv[2]);
    
    if (!mybdd) 
    {
        cerr << "File " << argv[1] << " not found" << endl;
        exit(-1);
    }
    
    /* delete comment if you want to use automatic reordering techniques*/
    //bdd_autoreorder(BDD_REORDER_SIFT);
    
    /*load the bdd*/
	bdd_fnload((char *)argv[1], solutionSpace);
	
	cout << "File " << argv[1] << " loaded" << endl;
	
	bdd_varblockall();
	bdd_reorder(BDD_REORDER_SIFT);
		
	//Initialize time	
    gettimeofday(&t_ini, NULL);		
    
    switch(num_test)
    {
	  case 1://brute force approach
	    
		core=0;dead=0;	
	    
		for (int j=0; j<bdd_varnum(); ++j)
		{
			if ((solutionSpace&bdd_nithvar(j)) == bdd_false()) core++;
			else if ((solutionSpace&bdd_ithvar(j)) == bdd_false()) dead++;
		}

		printf("core %d \n",core);
		printf("dead %d\n",dead);
		
		gettimeofday(&t_fin, NULL);
		secs = timeval_diff(&t_fin, &t_ini);
	
		fprintf(stdout, "time in %.16g seconds\n", secs);
		break;
		
	  case 2://our approach
	 
		var_low = (int*)calloc(bdd_varnum(), sizeof(int));
		var_high = (int*)calloc(bdd_varnum(), sizeof(int));
		marks = (int*)calloc(bddnodesize, sizeof(int));
		res_node = (int*)calloc(bddnodesize, sizeof(int));
		
		get_dependencies_conflicts(solutionSpace, var_low, var_high, marks, res_node);
		
		dead_features(var_high, var_low, bdd_varnum());
		gettimeofday(&t_fin, NULL);
		secs = timeval_diff(&t_fin, &t_ini);
	
		fprintf(stdout, "time in %.16g seconds\n", secs);
   	    break;
   	    
   	  default:
		fprintf(stdout, "invalid option %d\n", num_test);
			
	}//end switch
	
	bdd_done();
	
}//end main
