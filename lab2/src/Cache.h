/* Common (in bytes) */
#define WORD_SIZE 4
#define WORDS_PER_BLOCK 2
#define BLOCK_SIZE (WORDS_PER_BLOCK * WORD_SIZE)
#define DRAM_SIZE (1024 * BLOCK_SIZE)
#define MODE_READ 1
#define MODE_WRITE 0

/* L1 Cache */
#define L1_LINE_NUM 16
#define L1_SIZE (L1_LINE_NUM * BLOCK_SIZE) // in bytes

/* L2 Cache */
#define L2_LINE_NUM 512
#define L2_SIZE (L2_LINE_NUM * BLOCK_SIZE) // in bytes

/* Time */
#define DRAM_READ_TIME 100
#define DRAM_WRITE_TIME 50
#define L2_READ_TIME 10
#define L2_WRITE_TIME 5
#define L1_READ_TIME 1
#define L1_WRITE_TIME 1
