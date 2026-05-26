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

    editor->text[editor->line_count][0] = '/0';
    editor->line_lengths[editor->line_count] = 0;
    editor->line_count = new_count;

}

void print_text(TextEditor* editor) {
    for (int i = 0; i < editor->line_count; i++) {
        printf("%s\n", editor->text[i]);
    }
}
