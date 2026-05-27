#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>

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
        printf("Buffer is empty\n");
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
    printf("Text saved \n");
}


void load_from_file(TextEditor* editor, const char* filename) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        printf("file not found\n");
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
    printf("Text loaded\n");
}

void insert_text(TextEditor* editor, int line_indx, int col_indx, const char* new_text) {
    if (line_indx < 0 || line_indx >= editor->line_count) {
        printf("line does not exist\n");
        return;
    }

    int old_len = editor->line_lengths[line_indx];

    if (col_indx < 0 || col_indx > old_len) {
        printf("position does not exist \n");
        return;
    }

    int insert_len = 0;
    while (new_text[insert_len] != '\0') {
        insert_len++;
    }

    int new_len = old_len + insert_len;

    editor->text[line_indx] = realloc(editor->text[line_indx], (new_len + 1) * sizeof(char));

    for (int i = old_len; i >= col_indx; i--) {
        editor->text[line_indx][i + insert_len] = editor->text[line_indx][i];

    }

    for (int i = 0; i < insert_len; i++) {
        editor->text[line_indx][col_indx + i] = new_text[i];

    }

    editor->line_lengths[line_indx] = new_len;
    printf("Text inserted \n");


}


void search_text(TextEditor* editor, const char* pattern) {
    int found = 0;
    int pat_len = 0;
    while (pattern[pat_len] != '\0') {
        pat_len++;
    }

    for (int i = 0; i < editor->line_count; i++) {
        int line_len = editor->line_lengths[i];

        for (int j = 0; j <= line_len - pat_len; j++) {
            int match = 1;
            for (int k = 0; k < pat_len; k++) {
                if (editor->text[i][j + k] != pattern[k]) {
                    match = 0;
                    break;
                }
            }
            if (match) {
                printf("Text found at position: %d %d\n", i, j);
                found = 1;
            }
        }
    }

    if (!found) {
        printf("Text not found\n");
    }
}



int find_newline_index(const char* str) {
    int i = 0;
    while (str[i] != '\0') {
        if (str[i] == '\n') {
            return i;
        }
        i++;
    }
    return i;
}


int main() {
    TextEditor editor;
    init_editor(&editor);

    int choice;
    char buffer[256];

    do {
        printf("\nText Editor\n");
        printf("1.Append text to current line\n");
        printf("2.Start new line\n");
        printf("3.Print all text\n");
        printf("4.Save to file\n");
        printf("5.Load from file\n");
        printf("6.Insert text at position\n");
        printf("7.Search text\n");
        printf("8.Clear console\n");
        printf("0.Exit\n");
        printf("Choose action: ");

        scanf("%d", &choice);
        getchar();

        if (choice == 1) {
            printf("Enter text to append: ");
            fgets(buffer, sizeof(buffer), stdin);
            buffer[find_newline_index(buffer)] = '\0';
            append_to_current_line(&editor, buffer);
        }
        else if (choice == 2) {
            start_new_line(&editor);
            printf("New line started\n");
        }
        else if (choice == 3) {
            printf("\nYour text:\n");
            print_text(&editor);
        }
        else if (choice == 4) {
            printf("Enter file name to save: ");
            fgets(buffer, sizeof(buffer), stdin);
            buffer[find_newline_index(buffer)] = '\0';
            save_to_file(&editor, buffer);
        }
        else if (choice == 5) {
            printf("Enter file name to load: ");
            fgets(buffer, sizeof(buffer), stdin);
            buffer[find_newline_index(buffer)] = '\0';
            load_from_file(&editor, buffer);
            
        }
        else if (choice == 6) {
            int line_indx, col_indx;
            printf("Enter line and position (space between): ");
            scanf("%d %d", &line_indx, &col_indx);
            getchar();
            printf("Enter text to insert: ");
            fgets(buffer, sizeof(buffer), stdin);
            buffer[find_newline_index(buffer)] = '\0';
            insert_text(&editor, line_indx, col_indx, buffer);

        }
        else if (choice == 7) {
            printf("Enter text to search: ");
            fgets(buffer, sizeof(buffer), stdin);
            buffer[find_newline_index(buffer)] = '\0';
            search_text(&editor, buffer);
        }
        else if (choice == 8) {
            system("cls");
        }
        else if (choice == 0) {
            printf("Thank you for using the editor\n");
        }
        else {
            printf("Wrong choice\n");
        }

    } while (choice != 0);

    clear_editor(&editor);

    return 0;
}
