#include "parallel.h"
#include "utils.h"
#include <pthread.h>
#include <stdlib.h>

typedef struct {
    int64_t K;
    int64_t current_turn;
    int64_t player1_score;
    int64_t player2_score;
    int64_t num_experiments;
    int64_t player1_wins;
    int64_t player2_wins;
    int64_t draws;
    pthread_mutex_t *mutex;
} ThreadData;

static int64_t roll_dice(void) {
    uint8_t random_byte;
    read_random_bytes(&random_byte, 1);
    return (random_byte % 6) + 1;
}

static void *thread_worker(void *arg) {
    ThreadData *data = (ThreadData *)arg;
    
    int64_t local_p1_wins = 0;
    int64_t local_p2_wins = 0;
    int64_t local_draws = 0;
    
    int64_t remaining_turns = data->K - data->current_turn;
    
    for (int64_t exp = 0; exp < data->num_experiments; exp++) {
        int64_t score1 = data->player1_score;
        int64_t score2 = data->player2_score;
        
        for (int64_t turn = 0; turn < remaining_turns; turn++) {
            score1 += roll_dice() + roll_dice();
            score2 += roll_dice() + roll_dice();
        }
        
        if (score1 > score2) {
            local_p1_wins++;
        } else if (score2 > score1) {
            local_p2_wins++;
        } else {
            local_draws++;
        }
    }
    
    pthread_mutex_lock(data->mutex);
    data->player1_wins += local_p1_wins;
    data->player2_wins += local_p2_wins;
    data->draws += local_draws;
    pthread_mutex_unlock(data->mutex);
    
    return NULL;
}

SimulationResult simulate_parallel(
    int64_t K,
    int64_t current_turn,
    int64_t player1_score,
    int64_t player2_score,
    int64_t num_experiments,
    int64_t num_threads
) {
    SimulationResult result = {0, 0, 0};
    
    if (num_threads <= 0) {
        num_threads = 1;
    }
    
    int64_t experiments_per_thread = num_experiments / num_threads;
    int64_t remaining_experiments = num_experiments % num_threads;
    
    pthread_t *threads = (pthread_t *)malloc(num_threads * sizeof(pthread_t));
    ThreadData *thread_data = (ThreadData *)malloc(num_threads * sizeof(ThreadData));
    pthread_mutex_t mutex;
    
    pthread_mutex_init(&mutex, NULL);
    
    for (int64_t i = 0; i < num_threads; i++) {
        thread_data[i].K = K;
        thread_data[i].current_turn = current_turn;
        thread_data[i].player1_score = player1_score;
        thread_data[i].player2_score = player2_score;
        thread_data[i].num_experiments = experiments_per_thread + (i < remaining_experiments ? 1 : 0);
        thread_data[i].player1_wins = 0;
        thread_data[i].player2_wins = 0;
        thread_data[i].draws = 0;
        thread_data[i].mutex = &mutex;
        
        pthread_create(&threads[i], NULL, thread_worker, &thread_data[i]);
    }
    
    for (int64_t i = 0; i < num_threads; i++) {
        pthread_join(threads[i], NULL);
    }
    
    for (int64_t i = 0; i < num_threads; i++) {
        result.player1_wins += thread_data[i].player1_wins;
        result.player2_wins += thread_data[i].player2_wins;
        result.draws += thread_data[i].draws;
    }
    
    pthread_mutex_destroy(&mutex);
    free(threads);
    free(thread_data);
    
    return result;
}