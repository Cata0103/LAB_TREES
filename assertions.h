#define CHECK(cond) \
    do { \
        if (!(cond)) { \
            fprintf(stderr, "CHECK failed: %s (%s:%d)\n", \
                    #cond, __FILE__, __LINE__); \
            exit(EXIT_FAILURE); \
        } \
    } while (0)
#define DEBUG_PRINT(fmt, ...) \
    fprintf(stderr, "[DEBUG] " fmt "\n", __VA_ARGS__)
/*
Example Usage
CHECK(node->left != node);
DEBUG_PRINT("rank(%zu) = %zu", i, r);
*/