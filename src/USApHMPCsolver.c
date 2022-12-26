#include "USApHMPCsolver.h"
#include "hlp_reader.h"
#include "def.h"

Solution *solve(const char *hlp_file, const char *hlps_file)
{
    clock_t start, end;
    float cputime;
    Solution *solutionPtr;
    solutionPtr = &solution;

    // Read input data (it is stored in the globally defined variables from def.h)
    read_hlp_instance(hlp_file, hlps_file);

    // Solve with the benders framework
    UpperBound = MAX_DOUBLE;
    start = clock();
    Benders_framework();
    end = clock();
    cputime = (float)(end - start) / CLOCKS_PER_SEC;

    solutionPtr->cputime = cputime;

    free_memory();
    return solutionPtr;
};