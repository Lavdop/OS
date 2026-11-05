#ifndef PARALLEL_H
#define PARALLEL_H

#include <stdint.h>
#include "sequential.h"

SimulationResult simulate_parallel(
    int64_t K,
    int64_t current_turn,
    int64_t player1_score,
    int64_t player2_score,
    int64_t num_experiments,
    int64_t num_threads
);

#endif