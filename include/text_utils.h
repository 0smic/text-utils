#ifndef TEXT_UTILS_H
#define TEXT_UTILS_H

#include <stddef.h>

typedef struct freq {
    char *key;
    size_t freq;
} FreqKV;

typedef struct {
    FreqKV *items;
    size_t count;
    size_t capacity;
} FreqKVs;

typedef struct {
    char *data;
    size_t size;
} Content;

// File reading
Content *read_file(const char *filename);
void free_content(Content *file);

// Table management
FreqKVs *init_table();
void free_table(FreqKVs *table);

// Tokenization and insertion
FreqKVs *tokenize_text(Content *file, FreqKVs *table);

// Analysis
FreqKV *get_most_frequent(FreqKVs *table);

#endif
