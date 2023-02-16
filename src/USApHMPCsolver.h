#pragma once
#include "def.h"

Solution *solve(const char *hlp_file, const char *hlps_file, int time_limit);

void freeSolution(Solution *solutionPtr);
