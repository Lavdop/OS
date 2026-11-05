#ifndef SEQUENTIAL_H
#define SEQUENTIAL_H

#include <stdint.h>

typedef struct {
    int64_t player1_wins;
    int64_t player2_wins;
    int64_t draws;
} SimulationResult;

SimulationResult simulate_sequential(
    int64_t K,
    int64_t current_turn,
    int64_t player1_score,
    int64_t player2_score,
    int64_t num_experiments
);

#endif