#ifndef HNSW_H
#define HNSW_H

#include <vector>
#include <map>
#include <fstream>
#include <queue>
#include <random>
#include <functional>
#include <immintrin.h>
#include "../config.h"
#include "utils.h"

extern std::ofstream* debug_file;

class Edge {
public:
    // HNSW
    int target;
    float distance;

    // GraSP
    Edge* prev_edge;
    float weight; 
    float stinky;
    bool ignore;
    float probability_edge;
    unsigned int num_of_updates; 
    

    // Cost-Benefit
    int benefit;
    int cost;

    Edge();
    Edge(int target, float distance, int initial_cost = 0, int initial_benefit = 0);
};

class HNSW {
    friend std::ostream& operator<<(std::ostream& os, const HNSW& hnsw);
public:
    // Stores nodes by node index, then dimensions
    float** nodes;
    // Stores edges in adjacency list by node index, then layer number, then connection pair
    std::vector<std::vector<std::vector<Edge>>> mappings;
    int entry_point;
    int num_layers;
    int num_nodes;
    int num_dimensions;

    // Probability function
    std::mt19937 gen;
    std::uniform_real_distribution<double> dis;
    double normal_factor;

    // Statistics
    int layer0_dist_comps_per_q; 
    long long int layer0_dist_comps;
    long long int upper_dist_comps;
    long long int processed_neighbors;
    long long int total_neighbors;
    long long int num_distance_termination; 
    long long int num_original_termination;
    long long int total_path_size;
    long long int candidates_popped;
    std::vector<float> percent_neighbors;
    std::vector<int> cur_groundtruth;

    HNSW(Config* config, float** nodes);
    void reset_statistics();
    void search_queries(Config* config, float** queries);
    std::vector<Edge*> get_layer_edges(Config* config, int layer);
    void find_direct_path(std::vector<Edge*>& path, std::vector<std::pair<float, int>>& entry_points);
    bool should_terminate(Config* config, std::priority_queue<std::pair<float, int>>& top_k, std::pair<float, int>& top_1, float close_squared, float far_squared, bool is_querying, int layer_num, int candidates_popped_per_q);
    float calculate_average_clustering_coefficient();
    float calculate_global_clustering_coefficient();
    float calculate_distance(float* a, float* b, int size, int layer);

    // Main algorithms
    void insert(Config* config, int query);
    void search_layer(Config* config, float* query, std::vector<Edge*>& path, std::vector<std::pair<float, int>>& entry_points, int num_to_return, int layer_num, bool is_querying = false, bool is_training = false, bool is_ignoring = false, int* total_cost = nullptr);
    void select_neighbors_heuristic(Config* config, float* query, std::vector<Edge>& candidates, int num_to_return, int layer_num, bool extend_candidates = false, bool keep_pruned = true);
    std::vector<std::pair<float, int>> nn_search(Config* config, std::vector<Edge*>& path, std::pair<int, float*>& query, int num_to_return, bool is_querying = true, bool is_training = false, bool is_ignoring = false, int* total_cost = nullptr);
};

// Helper functions
HNSW* init_hnsw(Config* config, float** nodes);
void load_hnsw_files(Config* config, HNSW* hnsw, float** nodes, bool is_benchmarking = false);
void load_hnsw_graph(Config* config, HNSW* hnsw, std::ifstream& graph_file, float** nodes, int num_nodes, int num_layers);
void save_hnsw_files(Config* config, HNSW* hnsw, const std::string& name, long int duration);

#endif