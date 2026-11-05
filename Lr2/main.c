#include "utils.h"
#include "sequential.h"
#include "parallel.h"
#include <stdlib.h>

static void print_result(const char *label, SimulationResult result, uint64_t time_ns) {
    write_string(label);
    write_string("Player 1 wins: ");
    write_int(result.player1_wins);
    write_string("Player 2 wins: ");
    write_int(result.player2_wins);
    write_string("Draws: ");
    write_int(result.draws);
    write_string("Time (ns): ");
    write_int((int64_t)time_ns);
    write_string("\n");
}

int main(int argc, char *argv[]) {
    if (argc != 7) {
        write_string("Usage: ./program K turn score1 score2 experiments max_threads\n");
        return 1;
    }

    int64_t K = parse_int(argv[1]);
    int64_t turn = parse_int(argv[2]);
    int64_t score1 = parse_int(argv[3]);
    int64_t score2 = parse_int(argv[4]);
    int64_t experiments = parse_int(argv[5]);
    int64_t max_threads = parse_int(argv[6]);

    uint64_t start_time;
    uint64_t end_time;
    uint64_t seq_time;
    uint64_t par_time;

    SimulationResult seq_result;
    SimulationResult par_result;

    write_string("Sequential version:\n");
    start_time = get_time_ns();
    seq_result = simulate_sequential(K, turn, score1, score2, experiments);
    end_time = get_time_ns();
    seq_time = end_time - start_time;
    print_result("Sequential ", seq_result, seq_time);

    int64_t thread_counts[] = {1, 2, 4, 8, 16};
    int num_thread_configs = 5;

    write_string("\nParallel version:\n");
    for (int i = 0; i < num_thread_configs; i++) {
        int64_t threads = thread_counts[i];
        if (threads > max_threads) {
            break;
        }
        start_time = get_time_ns();
        par_result = simulate_parallel(K, turn, score1, score2, experiments, threads);
        end_time = get_time_ns();
        par_time = end_time - start_time;

        write_string("Threads: ");
        write_int(threads);
        print_result("Parallel ", par_result, par_time);

        if (par_time > 0) {
            int64_t speedup = (seq_time * 100) / par_time;
            write_string("Speedup (x100): ");
            write_int(speedup);
        }
        write_string("\n");
    }

    return 0;
}