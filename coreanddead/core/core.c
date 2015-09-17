using namespace std;
#include "dependencies_conflicts.h"

void dead_features(int *var_high, int *var_low, int num_var)
{
	int num_dead, num_core;
	
	num_dead = 0;
	num_core = 0;
	
	//fprintf(stdout, "Core features: \n");
	for (int i=0; i<num_var; ++i)
	{
		if (var_high[i] != var_low[i])
		if (var_high[i] && !var_low[i])
		{
			fprintf(stdout, "core %d \n", i);
			num_core++;
		} else {
			if (!var_high[i] && var_low[i])
			{
				fprintf(stdout, "dead %d \n", i);
				num_dead++;
			}
		}
	}
	
	fprintf(stdout, "Number of core feaures: %d \nNumber of dead features: %d \n", num_core, num_dead);
}//end dead_features

int get_dependencies_conflicts(const bdd r, int *vars_low, int *vars_high, int *marks,  int *res_node)
{
    int level, level_high, level_low;
    int root;
    
	root = r.id();
	if (!marks[root])
	{
		marks[root] = !marks[root];
		level = bdd_var2level(bdd_var(r));
		if (bdd_high(r).id() == 1) 
		{
			vars_high[level] = 1;		
			for (int i=level+1; i < bdd_varnum(); ++i) 
			{							
				vars_high[i] = 1;
				vars_low[i] = 1;
			}
			res_node[root] = 1;
		} else 
		{
			if (bdd_high(r).id() != 0)
			{
				res_node[root] = get_dependencies_conflicts(bdd_high(r), vars_low, vars_high, marks, res_node);
							
				if (res_node[root])
				{								
					vars_high[level] =  1;
					level_high = bdd_var2level(bdd_var(bdd_high(r)));
					for (int i=level+1; i < level_high; ++i) 
					{			
						vars_high[i] = 1;
						vars_low[i] = 1;
					}
				}
			} 
		}
		
		if (bdd_low(r).id() == 1) 
		{
			vars_low[level] = 1;			
			for (int i=level+1; i < bdd_varnum(); ++i)
			{  
				vars_high[i] = 1;
				vars_low[i] = 1;
			}
			res_node[root] = 1;
		} else 
		{
			if (bdd_low(r).id() != 0)
			{		
				res_node[root] = get_dependencies_conflicts(bdd_low(r), vars_low, vars_high, marks, res_node);
				
				if (res_node[root]) 
				{
					vars_low[level] =  1;			
					level_low = bdd_var2level(bdd_var(bdd_low(r)));
					
					for (int i=level+1; i < level_low; ++i) 
					{			
						vars_high[i] = 1;
						vars_low[i] = 1;
					}
				}
			}
		}		
	} 

	return res_node[root];	
}//end get_dependencies_conflicts

