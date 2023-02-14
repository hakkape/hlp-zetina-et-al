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
    fscanf(hlps_input, "%d", &p_hubs);
    fscanf(hlps_input, "%lf", &collect);
    fscanf(hlps_input, "%lf", &transfer);
    fscanf(hlps_input, "%lf", &distribute);

    fclose(hlps_input);

    // # Read HLP topology
    hlp_input = open_file(hlp_file, "r");
    fscanf(hlp_input, "%d", &NN);
    w_fixed_costs = 0;
    w_p_median_constr = 1;

    // ## Set number of customers and allocate storage for customer assignments
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
        O[i] = 0; // Total out demand
        D[i] = 0; // Total in demand
        for (j = 0; j < NN; j++)
        {
            fscanf(hlp_input, "%lf", &c[i][j]);
            c_c[i][j] = collect * c[i][j];
            c_t[i][j] = transfer * c[i][j];
            c_d[i][j] = distribute * c[i][j];
        }
    }

    // ## Demands
    AggregatedDemand = 0;
    menor_O = MAX_DOUBLE; // The smallest out demand of any customer (i think)
    Q = 0;                // we have no capacities for facilities
    Capacitated_instances = 0;
    for (i = 0; i < NN; i++)
    {
        for (j = 0; j < NN; j++)
        {
            fscanf(hlp_input, "%lf", &W[i][j]);
            O[i] += W[i][j];
            D[j] += W[i][j];
        }
        if (O[i] < menor_O)
            menor_O = O[i];
        AggregatedDemand += O[i];

        // ## No fixed costs for facilities
        f[i][0] = 0;
    }
    // ## Capacity levels -  we don't have any limits
    for (i = 0; i < NN; i++)
        b[i][0] = AggregatedDemand;

    fclose(hlp_input);
};
