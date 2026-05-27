#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#pragma execution_character_set("utf-8")

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
    if (editor->line_count == 0) {
        printf("Текст порожній\n");
        return;
    }

    for (int i = 0; i < editor->line_count; i++) {
        printf("%s\n", editor->text[i]);
    }
}

void clear_editor(TextEditor* editor) {
    for (int i = 0; i < editor->line_count; i++) {
        free(editor->text[i]);
    }

    free(editor->text);
    free(editor->line_lengths);
    editor->text = NULL;
    editor->line_lengths = NULL;
    editor->line_count = 0;
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
    
    editor->line_lengths[current_index] = old_len + new_symbols_len;
    editor->text[current_index] = realloc(editor->text[current_index], (editor->line_lengths[current_index] + 1) * sizeof(char));
    
    for (int i = 0; i < new_symbols_len; i++) {
        editor->text[current_index][old_len + i] = new_symbols[i];
    }

    editor->text[current_index][old_len + new_symbols_len] = '\0';

}

void save_to_file(TextEditor* editor, const char* filename) {
    FILE* file = fopen(filename, "w");
    if (file == NULL) {
        printf("Error opening file\n");
        return;
    }

    for (int i = 0; i < editor->line_count; i++) {
        fputs(editor->text[i], file);
        if (i < editor->line_count - 1) {
            fputc('\n', file);
        }
    }

    fclose(file);
    printf("Текст збережено успішно\n");
}


void load_from_file(TextEditor* editor, const char* filename) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        printf("Файл не знайдений\n");
        return;
    }

    clear_editor(editor);
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
    printf("Текст завантажено\n");
}

