#include "USApHMPCsolver.h"
#include "hlp_reader.h"
#include "def.h"

Solution *solve(const char *hlp_file, const char *hlps_file)
{
    clock_t start, end;
    float cputime;

    // Read input data (it is stored in the globally defined variables from def.h)
    printf(hlp_file);
    read_hlp_instance(hlp_file, hlps_file);

    // We do not want to write to an output file, as we return all results directly to the calling function
    write_outfile = false;

    // Solve with the benders framework
    printf("Start solving \n");
    UpperBound = MAX_DOUBLE;
    start = clock();
    Benders_framework();
    end = clock();
    cputime = (float)(end - start) / CLOCKS_PER_SEC;

    solution.cputime = cputime;

    free_memory();
    return &solution;
};

void freeSolution(Solution *solutionPtr)
{
    free(solutionPtr->assigned_hubs);
    free(solutionPtr);
};