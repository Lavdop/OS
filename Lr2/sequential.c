#include "sequential.h"
#include "utils.h"

static int64_t roll_dice(void) {
    uint8_t random_byte;
    read_random_bytes(&random_byte, 1);
    return (random_byte % 6) + 1;
}

SimulationResult simulate_sequential(
    int64_t K,
    int64_t current_turn,
    int64_t player1_score,
    int64_t player2_score,
    int64_t num_experiments
) {
    SimulationResult result = {0, 0, 0};
    
    int64_t remaining_turns = K - current_turn;
    
    for (int64_t exp = 0; exp < num_experiments; exp++) {
        int64_t score1 = player1_score;
        int64_t score2 = player2_score;
        
        for (int64_t turn = 0; turn < remaining_turns; turn++) {
            score1 += roll_dice() + roll_dice();
            score2 += roll_dice() + roll_dice();
        }
        
        if (score1 > score2) {
            result.player1_wins++;
        } else if (score2 > score1) {
            result.player2_wins++;
        } else {
            result.draws++;
        }
    }
    
    return result;
}