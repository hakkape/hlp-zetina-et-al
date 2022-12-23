#include "hlp_reader.h"
#include "def.h"

void read_hlp_instance(const char *hlp_file, const char *hlps_file)
{
    FILE *hlp_input;
    FILE *hlps_input;

    // Indices for iterating
    int i;
    int j;

    // # Read HLP settings
    hlps_input = open_file(hlps_file, "r");
    // TODO;

    fclose(hlps_input);
    // # Read HLP topology
    hlp_input = open_file(hlp_file, "r");

    // ## Set number of customers and allocate storage for customer assignments
    fscanf(hlp_input, "%d", &NN);
    initialize_memory();
    solution.n_customers = NN;
    solution.assigned_hubs = malloc(NN * sizeof(int));

    // Return error if not all assignments are possible as this is not implemented
    char possible_assignments[1000];
    fscanf(hlp_input, "%s\n", possible_assignments);
    if (strcmp(possible_assignments, "ALL"))
    {
        fprintf(stderr, "ERROR: Can currently only deal with instances where all assignments are possible.");
        exit(1);
    }

    // ## Costs
    double cost;
    for (i = 0; i < NN; i++)
    {
        for (j = 0; j < NN; j++)
        {
            fscanf(hlp_input, "%lf", &c[i][j]);
            c_c[i][j] = collect * c[i][j];
            c_t[i][j] = transfer * c[i][j];
            c_d[i][j] = distribute * c[i][j];
        }
    }

    // ## Demands
    for (i = 0; i < NN; i++)
    {
        for (j = 0; j < NN; j++)
        {
            fscanf(hlp_input, "%lf", &W[i][j]);
        }
    }
    fclose(hlp_input);
};
