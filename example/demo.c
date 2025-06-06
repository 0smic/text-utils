#include <stdio.h>
#include "text_utils.h"

int main() {
    Content *file = read_file("test.txt");
    if (!file) {
        printf("Failed to read file.\n");
        return 1;
    }

    FreqKVs *table = init_table();
    tokenize_text(file, table);

    FreqKV *most = get_most_frequent(table);
    if (most) {
        printf("Most frequent word: %s (%zu times)\n", most->key, most->freq);
    }

    free_content(file);
    free_table(table);
    return 0;
}
