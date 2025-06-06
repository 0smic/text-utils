#include "../include/text_utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define INIT_TABLE_SIZE 100

size_t hash(char *key, FreqKVs table) {
    size_t index = 0;
    for (size_t i = 0; i < strlen(key); i++) {
        index += (unsigned char)key[i];
    }
    return index % table.capacity;
}

int insert_into_table(FreqKVs *table, FreqKV *data, size_t index) {
    int start_idx = index;
    do {
        if (table->items[index].key == NULL) {
            table->items[index].key = strdup(data->key);
            table->items[index].freq = 1;
            table->count++;
            free(data->key);
            free(data);
            return 0;
        } else if (strcmp(table->items[index].key, data->key) == 0) {
            table->items[index].freq++;
            free(data->key);
            free(data);
            return 0;
        }
        index = (index + 1) % table->capacity;
    } while (index != start_idx);
    return 1;
}

FreqKVs *resize_table_and_insert(FreqKVs *table, char *data) {
    if (table->count >= table->capacity / 2) {
        table->capacity *= 2;
        table->items = realloc(table->items, sizeof(FreqKV) * table->capacity);
        for (size_t i = table->count; i < table->capacity; i++) {
            table->items[i].key = NULL;
            table->items[i].freq = 0;
        }
    }
    size_t index = hash(data, *table);
    FreqKV *dataKV = malloc(sizeof(FreqKV));
    dataKV->key = strdup(data);
    dataKV->freq = 1;
    if (insert_into_table(table, dataKV, index) != 0) return NULL;
    return table;
}

FreqKVs *init_table() {
    FreqKVs *table = malloc(sizeof(FreqKVs));
    table->capacity = INIT_TABLE_SIZE;
    table->count = 0;
    table->items = calloc(table->capacity, sizeof(FreqKV));
    return table;
}

FreqKVs *tokenize_text(Content *file, FreqKVs *table) {
    if (file->size == 0) return NULL;
    char *start;
    char *str = file->data;
    while (*str != '\0') {
        while (*str && (isspace((unsigned char)*str) || *str == ',' || *str == '.')) {
            str++;
        }
        start = str;
        while (*str && (!isspace((unsigned char)*str) && *str != ',' && *str != '.')) {
            str++;
        }
        if (*start && *str != start) {
            if (*str) {
                *str = '\0';
                str++;
            }
            table = resize_table_and_insert(table, start);
        }
    }
    return table;
}

Content *read_file(const char *filename) {
    Content *file = malloc(sizeof(Content));
    FILE *fp = fopen(filename, "rb");
    if (!fp) {
        free(file);
        return NULL;
    }

    fseek(fp, 0, SEEK_END);
    file->size = ftell(fp);
    rewind(fp);

    file->data = malloc(file->size + 1);
    if (!file->data) {
        fclose(fp);
        free(file);
        return NULL;
    }

    size_t read_size = fread(file->data, 1, file->size, fp);
    file->data[read_size] = '\0';
    fclose(fp);

    return file;
}

void free_content(Content *file) {
    if (!file) return;
    free(file->data);
    free(file);
}

void free_table(FreqKVs *table) {
    if (!table) return;
    for (size_t i = 0; i < table->capacity; i++) {
        if (table->items[i].key != NULL) {
            free(table->items[i].key);
        }
    }
    free(table->items);
    free(table);
}

FreqKV *get_most_frequent(FreqKVs *table) {
    if (!table) return NULL;
    FreqKV *most = NULL;
    size_t max = 0;
    for (size_t i = 0; i < table->capacity; i++) {
        if (table->items[i].key && table->items[i].freq > max) {
            max = table->items[i].freq;
            most = &table->items[i];
        }
    }
    return most;
}
