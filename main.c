#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    char** text;
    int* line_lengths;
    int line_count;
} TextEditor;

void init_editor(TextEditor* editor) {
    editor->text = NULL;
    editor->line_lengths = NULL;
    editor->line_count = 0;
}

void start_new_line(TextEditor* editor) {
    int new_count = editor->line_count + 1;
    editor->text = realloc(editor->text, new_count * sizeof(char*));
    editor->line_lengths = realloc(editor->line_lengths, new_count * sizeof(int));

    editor->text[editor->line_count] = malloc(1 * sizeof(char));
    editor->text[editor->line_count][0] = '\0';
    editor->line_lengths[editor->line_count] = 0;
    editor->line_count = new_count;

}

void print_text(TextEditor* editor) {
    for (int i = 0; i < editor->line_count; i++) {
        printf("%s\n", editor->text[i]);
    }
}

void append_to_current_line(TextEditor* editor, const char* new_symbols) {
    if (editor->line_count == 0) {
        start_new_line(editor);
    }

    int current_index = editor->line_count - 1;

    int old_len = editor->line_lengths[current_index];
    
    int new_symbols_len = 0;
    while (new_symbols[new_symbols_len] != '\0') {
        new_symbols_len++;
    }
    int new_size = old_len + new_symbols_len + 1;
    
    editor->line_lengths[current_index] = old_len + new_symbols_len;
    editor->text[current_index] = realloc(editor->text[current_index], (editor->line_lengths[current_index] + 1) * sizeof(char));
    
    for (int i = 0; i < new_symbols_len; i++) {
        editor->text[current_index][old_len + i] = new_symbols[i];
    }

    editor->text[current_index][old_len + new_symbols_len] = '\0';

}

void load_from_file(TextEditor* editor, const char* filename) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        printf("Error opening file\n");
        return;
    }

    char buffer[256];

    while (fgets(buffer, sizeof(buffer), file) != NULL) {

        int len = 0;
        while (buffer[len] != '\0') {
            len++;
        }

        if (len > 0 && buffer[len - 1] == '\n') {
            buffer[len - 1] = '\0';
        }

        start_new_line(editor);
        append_to_current_line(editor, buffer);
    }

    fclose(file);
}