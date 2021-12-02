#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include <stdbool.h>
#include <stdlib.h>

/**
 * -----------------------------------------------------------------------------
 * SYSTEM MODULE
 * -----------------------------------------------------------------------------
 */

#define __FILENAME__ (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__)

void print_string(char *s) {
    printf("%s\n", s);
}

void print_string_size(char *s) {
    printf("%lu\n", strlen(s));
}

/**
 * Prints the error message and exits the program.
 * @param filename The name of the file where the error occurred.
 * @param line The line number where the error occurred.
 * @param function The function where the error occurred.
 * @param msg The error message.
 */
void print_error(const char *filename, int line, const char *function,
                 const char *msg) {
    fprintf(stderr, "%s:%d - [%s] %s\n", filename, line, function, msg);
    exit(EXIT_FAILURE);
}

/**
 * -----------------------------------------------------------------------------
 * STRING MODULE
 * -----------------------------------------------------------------------------
 */

char *pad_string(char *str, unsigned int max);

char *replace_char(char *str, char s, char r);

char *remove_char(char *str, char r);

void remove_spaces(char *str);

void remove_newlines(char *str);

/**
 * Pads the string with spaces at the end.
 * @param str The string.
 * @param max The max length of the string.
 * @return The padded string.
 */
char *pad_string(char *str, unsigned int max) {
    char *new_str = (char *) malloc(sizeof(char) * (max + 1));

    if (new_str == NULL) {
        print_error(__FILENAME__, __LINE__, __func__, "Malloc failed");
    }

    strcpy(new_str, str);

    if (strcmp(str, new_str) != 0) {
        print_error(__FILENAME__, __LINE__, __func__, "strcpy failed");
    }

    for (unsigned int i = strlen(str); i < max; i++) {
        new_str[i] = ' ';
    }
    new_str[max] = '\0';
    return new_str;
}

/**
 * Replaces all occurrences of a character in a string with another character.
 * @param str The string.
 * @param s The character to be replaced.
 * @param r The character to replace with.
 * @return The new string.
 */
char *replace_char(char *str, char s, char r) {
    char *new_str = (char *) malloc(sizeof(char) * (strlen(str) + 1));

    if (new_str == NULL) {
        print_error(__FILENAME__, __LINE__, __func__, "Malloc failed");
    }

    strcpy(new_str, str);

    if (strcmp(str, new_str) != 0) {
        print_error(__FILENAME__, __LINE__, __func__, "strcpy failed");
    }

    for (int i = 0; i < strlen(str); i++) {
        if (str[i] == s) {
            new_str[i] = r;
        }
    }
    new_str[strlen(str)] = '\0';
    return new_str;
}

/**
 * Removes spaces everywhere in the string.
 * @param str The string.
 * @return The string without spaces.
 */
char *remove_char(char *str, char r) {
    char *new_str = (char *) malloc(sizeof(char) * (strlen(str) + 1));

    if (new_str == NULL) {
        print_error(__FILENAME__, __LINE__, __func__, "Malloc failed");
        exit(EXIT_FAILURE);
    }

    int j = 0;
    for (int i = 0; i < strlen(str); i++) {
        if (str[i] != r) {
            new_str[j] = str[i];
            j++;
        }
    }
    new_str[j] = '\0';
    return new_str;
}

/**
 * Removes spaces from the string.
 * @param str The string.
 */
void remove_spaces(char *str) {
    char *new_str = remove_char(str, ' ');
    strcpy(str, new_str);

    if (strcmp(str, new_str) != 0)
        print_error(__FILENAME__, __LINE__, __func__, "strcpy failed");

    free(new_str);
}

/**
 * Removes new lines from the string.
 * @param str The string.
 */
void remove_newlines(char *str) {
    char *new_str = remove_char(str, '\n');
    new_str = remove_char(new_str, '\r');
    strcpy(str, new_str);

    if (strcmp(str, new_str) != 0)
        print_error(__FILENAME__, __LINE__, __func__, "strcpy failed");

    free(new_str);
}

/**
 * -----------------------------------------------------------------------------
 * VECTOR MODULE
 * -----------------------------------------------------------------------------
 */

/**
 * Vector data structure type
 */
typedef struct {
    int size;
    int capacity;
    char **elements;
} vector_t;

vector_t *vector_init(int capacity);

unsigned int find_max_vector_element_size(vector_t *v);

void resize_all(vector_t *v, unsigned int max);

void vector_add(vector_t *v, char *s);

void vector_remove(vector_t *v, int index);

void vector_foreach(vector_t *v, void (*f)(char *));

void vector_free(vector_t *v);

/**
 * Initializes the vector
 * @param capacity The capacity of the vector.
 * @return The initialized vector.
 */
vector_t *vector_init(int capacity) {
    if (capacity < 1) {
        print_error(__FILENAME__, __LINE__, __func__,
                    "Capacity must be greater than 0");
    }

    vector_t *v = (vector_t *) malloc(sizeof(vector_t));

    if (v == NULL) {
        print_error(__FILENAME__, __LINE__, __func__, "Malloc failed");
    }

    v->size = 0;
    v->capacity = capacity;
    v->elements = (char **) malloc(capacity * sizeof(char *));

    if (v->elements == NULL) {
        print_error(__FILENAME__, __LINE__, __func__, "Malloc failed");
    }

    return v;
}

/**
 * Finds the longest element in the vector.
 * @param v The vector.
 * @return Length of the longest element.
 */
unsigned int find_max_vector_element_size(vector_t *v) {
    unsigned int max = 0;
    for (int i = 0; i < v->size; i++) {
        if (strlen(v->elements[i]) > max) {
            max = strlen(v->elements[i]);
        }
    }
    return max;
}

/**
 * Resizes all elements to the max length.
 * @param v The vector.
 * @param max The max length of the elements.
 */
void resize_all(vector_t *v, unsigned int max) {
    for (int i = 0; i < v->size; i++) {
        v->elements[i] = pad_string(v->elements[i], max);
    }
}

/**
 * Adds a string to the vector.
 * @param v The vector.
 * @param s The string.
 */
void vector_add(vector_t *v, char *s) {
    if (v->size == v->capacity) {
        v->capacity *= 2;
        v->elements = (char **) realloc(v->elements,
                                        v->capacity * sizeof(char *));

        if (v->elements == NULL) {
            print_error(__FILENAME__, __LINE__, __func__, "Realloc failed");
        }
    }
    v->elements[v->size] = remove_char(s, ' ');
    v->size++;

    resize_all(v, find_max_vector_element_size(v));
}

/**
 * Remove element from the vector.
 * @param v The vector.
 * @param index The index of the element.
 */
void vector_remove(vector_t *v, int index) {
    if (index < 0 || index >= v->size) {
        print_error(__FILENAME__, __LINE__, __func__, "Index out of bounds");
    }

    for (int i = index; i < v->size - 1; i++) {
        v->elements[i] = v->elements[i + 1];
    }
    v->size--;
}

/**
 * Applies the function to each element of vector.
 * @param v The vector.
 * @param f The function.
 */
void vector_foreach(vector_t *v, void (*f)(char *)) {
    if (v == NULL) {
        print_error(__FILENAME__, __LINE__, __func__, "Vector is NULL");
    }

    if (f == NULL) {
        print_error(__FILENAME__, __LINE__, __func__, "Function is NULL");
    }

    for (int i = 0; i < v->size; i++) {
        f(v->elements[i]);
    }
}

/**
 * Frees the vector.
 * @param v The vector.
 */
void vector_free(vector_t *v) {
    if (v == NULL) {
        print_error(__FILENAME__, __LINE__, __func__, "Vector is NULL");
    }

    if (v->elements == NULL) {
        print_error(__FILENAME__, __LINE__, __func__,
                    "Vector elements are NULL");
    }

    for (int i = 0; i < v->size; i++) {
        if (v->elements[i] == NULL) {
            print_error(__FILENAME__, __LINE__, __func__,
                        "Vector element is NULL");
        }
    }

    for (int i = 0; i < v->size; i++) {
        free(v->elements[i]);
    }
    free(v->elements);
    free(v);
}

/**
 * -----------------------------------------------------------------------------
 * SET MODULE [SET]
 * -----------------------------------------------------------------------------
 */

/**
 * Set type.
 */
typedef struct set {
    unsigned int index;
    int size;
    int capacity;
    char **elements;
} set_t;

set_t *set_init(int capacity);

set_t *set_init_indexed(int index, int capacity);

void set_add(set_t *s, char *e);

void set_add_row(set_t *s, int row);

void set_print(set_t *s);

bool set_is_empty(set_t *s);

set_t *set_union(set_t *s1, set_t *s2);

set_t *set_intersection(set_t *s1, set_t *s2);

set_t *set_difference(set_t *s1, set_t *s2);

bool set_is_subset(set_t *s1, set_t *s2);

bool set_is_equal(set_t *s1, set_t *s2);

/**
 * Creates a new set.
 * @param capacity The initial capacity of the set.
 * @return The new set.
 */
set_t *set_init(int capacity) {
    set_t *s = malloc(sizeof(set_t));

    if (s == NULL)
        print_error(__FILENAME__, __LINE__, __func__, "Malloc failed");

    s->index = 0;
    s->size = 0;
    s->capacity = capacity;
    s->elements = malloc(sizeof(char *) * capacity);

    if (s->elements == NULL)
        print_error(__FILENAME__, __LINE__, __func__, "Malloc failed");

    return s;
}
/**
 * Creates a new set.
 * @param index The index of the set.
 * @param capacity The initial capacity of the set.
 * @return The new set.
 */
set_t *set_init_indexed(int index, int capacity) {
    if (index < 1)
        print_error(__FILENAME__, __LINE__, __func__, "Index is negative or zero");

    set_t *s = malloc(sizeof(set_t));

    if (s == NULL)
        print_error(__FILENAME__, __LINE__, __func__, "Malloc failed");

    s->index = index;
    s->size = 0;
    s->capacity = capacity;
    s->elements = malloc(sizeof(char *) * capacity);

    if (s->elements == NULL)
        print_error(__FILENAME__, __LINE__, __func__, "Malloc failed");

    return s;
}

/**
 * Adds an element to the set.
 * @param s The set.
 * @param e The element to add.
 */
void set_add(set_t *s, char *e) {
    if (s->size == s->capacity) {
        s->capacity *= 2;
        s->elements = realloc(s->elements, sizeof(char *) * s->capacity);

        if (s->elements == NULL)
            print_error(__FILENAME__, __LINE__, __func__, "Realloc failed");
    }
    s->elements[s->size] = e;
    s->size++;
}

///**
// * Adds a row number to the set.
// * @param s The set.
// * @param e The element to add.
// */
//void set_add_row(set_t *s, int row) {
//    s->row = row;
//}

/**
 * Prints the set.
 * @param s The set.
 */
void set_print(set_t *s) {
    printf("Set %d: ", s->index);
    for (int i = 0; i < s->size; i++) {
        printf("%s ", s->elements[i]);
    }
    printf("\n");
}

/**
 * Checks if the set is empty.
 * @param s The set.
 * @return true if the set is empty, false otherwise.
 */
bool set_is_empty(set_t *s) {
    return s->size == 0;
}

/**
 * Union of two sets.
 * @param s1 The first set.
 * @param s2 The second set.
 * @return The union of the two sets.
 */
set_t *set_union(set_t *s1, set_t *s2) {
    set_t *s = set_init(s1->capacity + s2->capacity);

    for (int i = 0; i < s1->size; i++) {
        set_add(s, s1->elements[i]);
    }

    for (int i = 0; i < s2->size; i++) {
        bool is_in_set = false;

        for (int j = 0; j < s->size; j++) {
            if (strcmp(s2->elements[i], s->elements[j]) == 0) {
                is_in_set = true;
            }
        }

        if (!is_in_set) {
            set_add(s, s2->elements[i]);
        }
    }

    return s;
}

/**
 * Intersection of two sets.
 * @param s1 The first set.
 * @param s2 The second set.
 * @return The intersection of the two sets.
 */
set_t *set_intersection(set_t *s1, set_t *s2) {
    set_t *s = set_init(s1->capacity + s2->capacity);

    for (int i = 0; i < s1->size; i++) {
        for (int j = 0; j < s2->size; j++) {
            if (strcmp(s1->elements[i], s2->elements[j]) == 0) {
                set_add(s, s1->elements[i]);
            }
        }
    }

    return s;
}

/**
 * Difference of two sets.
 * @param s1 The first set.
 * @param s2 The second set.
 * @return The difference of the two sets.
 */
set_t *set_difference(set_t *s1, set_t *s2) {
    set_t *s = set_init(s1->capacity + s2->capacity);

    for (int i = 0; i < s1->size; i++) {
        bool is_in_set = false;

        for (int j = 0; j < s2->size; j++) {
            if (strcmp(s1->elements[i], s2->elements[j]) == 0) {
                is_in_set = true;
            }
        }

        if (!is_in_set) {
            set_add(s, s1->elements[i]);
        }
    }
    return s;
}

/**
 * Checks if the first set is a subset of the second set.
 * @param s1 The first set.
 * @param s2 The second set.
 * @return true if the first set is a subset of the second set, false otherwise.
 */
bool set_is_subset(set_t *s1, set_t *s2) {
    for (int i = 0; i < s1->size; i++) {

        bool is_in_set = false;

        for (int j = 0; j < s2->size; j++) {
            if (strcmp(s1->elements[i], s2->elements[j]) == 0) {
                is_in_set = true;
            }
        }

        if (!is_in_set) {
            return false;
        }
    }
    return true;
}

/**
 * Checks if the first set is equal to the second set.
 * @param s1 The first set.
 * @param s2 The second set.
 * @return true if the first set is equal to the second set, false otherwise.
 */
bool set_is_equal(set_t *s1, set_t *s2) {
    if (s1->size != s2->size) {
        return false;
    }

    for (int i = 0; i < s1->size; i++) {
        bool is_in_set = false;

        for (int j = 0; j < s2->size; j++) {
            if (strcmp(s1->elements[i], s2->elements[j]) == 0) {
                is_in_set = true;
            }
        }

        if (!is_in_set) {
            return false;
        }
    }
    return true;
}

/**
 * -----------------------------------------------------------------------------
 * SET MODULE [SET MAP]
 * -----------------------------------------------------------------------------
 */

/**
 * Set map type.
 */
typedef struct set_map_t set_map_t;
typedef struct set_map_t {
    int index;
    set_t *s;
    set_map_t *next;
};

// create a function to init set_t map
set_map_t *set_map_init(set_t *s) {
    set_map_t *sm = (set_map_t *) malloc(sizeof(set_map_t));

    if (sm == NULL)
        print_error(__FILENAME__, __LINE__, __func__, "Malloc failed");

    sm->index = 0;
    sm->s = s;
    sm->next = NULL;

    printf("%s\n", "Set map initialized");

    return sm;
}

// create a function to add to set_t map. check if map is inited
void set_map_add(set_map_t *sm, set_t *s) {
    if (sm == NULL)
        print_error(__FILENAME__, __LINE__, __func__,
                    "Set map is not initialized");

    set_map_t *new_sm = set_map_init(s);
    new_sm->index = sm->index + 1;
    new_sm->next = sm->next;
    sm->next = new_sm;
}

void set_map_print(set_map_t *sm) {
    if (sm == NULL) {
        printf("\n");
        return;
    }

    printf("%d: ", sm->index);
    set_print(sm->s);
    set_map_print(sm->next);
}


/**
 * -----------------------------------------------------------------------------
 * COMMAND MODULE [COMMAND]
 * -----------------------------------------------------------------------------
 */

/**
 * Enumerates command types
 */
typedef enum {
    U = 'U',
    S = 'S',
    R = 'R',
    C = 'C',
} commands;

/**
 * Command operations type
 */
typedef struct {
    char *name;
    void (*function)(char *);
} command_operation_t;

/**
 * Command type
 */
typedef struct {
    commands type;
    command_operation_t *operation;
    vector_t args;
} command_t;

command_t *init_command();

void set_command_args(command_t *c, vector_t *args);

void add_command_arg(command_t *c, char *arg);

void print_command(command_t *c);

void free_command(command_t *c);

command_t *parse_command(char *s);

/**
 * Initializes a command.
 * @return The initialized command.
 */
command_t *init_command() {
    command_t *c = (command_t *) malloc(sizeof(command_t));

    if (c == NULL) {
        print_error(__FILENAME__, __LINE__, __func__, "Malloc failed");
    }

    c->args = *vector_init(1);
    return c;
}

/**
 * Sets the command arguments.
 * @param c The command.
 * @param args The arguments.
 */
void set_command_args(command_t *c, vector_t *args) {
    if (args == NULL)
        print_error(__FILENAME__, __LINE__, __func__, "Args is NULL");

    if (&(c->args) == NULL)
        print_error(__FILENAME__, __LINE__, __func__, "Command args is NULL");

    c->args = *args;
}

/**
 * Adds a command argument.
 * @param c The command.
 * @param arg The argument.
 */
void add_command_arg(command_t *c, char *arg) {
    if (&(c->args) == NULL)
        print_error(__FILENAME__, __LINE__, __func__, "Command args is NULL");

    vector_add(&c->args, arg);
}

/**
 * Prints the command.
 * @param c The command.
 */
void print_command(command_t *c) {
    switch (c->type) {
        case U:
            printf("U ");
            break;
        case S:
            printf("S ");
            break;
        case R:
            printf("R ");
            break;
        case C:
            printf("C ");
            break;
    }
    for (int i = 0; i < c->args.size; i++) {
        printf("%s ", c->args.elements[i]);
    }
    printf("\n");
}

/**
 * Frees the command.
 * @param c The command.
 */
void free_command(command_t *c) {
    if (&(c->args) == NULL)
        print_error(__FILENAME__, __LINE__, __func__, "Command args is NULL");

    if (c->args.size == 0)
        print_error(__FILENAME__, __LINE__, __FUNCTION__, "Invalid args");


    vector_free(&c->args);
    free(c);
}

/**
 * Parses a command.
 * @param s The command string.
 * @return The command.
 */
command_t *parse_command(char *s) {
    command_t *c = init_command();
    vector_t *args = vector_init(10);

    if (s == NULL)
        print_error(__FILENAME__, __LINE__, __func__, "String is NULL");

    char *copy = strdup(s);
    char *token = strtok(copy, " ");

    while (token != NULL) {
        vector_add(args, token);
        token = strtok(NULL, " ");
    }

    if (args->size == 0)
        print_error(__FILENAME__, __LINE__, __FUNCTION__, "Invalid command");

    vector_foreach(args, remove_spaces);
    vector_foreach(args, remove_newlines);

    if (strcmp(args->elements[0], "U") == 0) {
        c->type = U;
    } else if (strcmp(args->elements[0], "S") == 0) {
        c->type = S;
    } else if (strcmp(args->elements[0], "R") == 0) {
        c->type = R;
    } else if (strcmp(args->elements[0], "C") == 0) {
        c->type = C;
    } else {
        print_error(__FILENAME__, __LINE__, __FUNCTION__, "Invalid command");
    }

    vector_remove(args, 0);

    c->args = *args;
    return c;
}

/**
 * -----------------------------------------------------------------------------
 * COMMAND MODULE [COMMAND VECTOR]
 * -----------------------------------------------------------------------------
 */

/**
 * Command vector type
 */
typedef struct {
    int size;
    int capacity;
    command_t *commands;
} command_vector_t;

command_vector_t *command_vector_init(int capacity);

void command_vector_add(command_vector_t *cv, command_t c);

void command_vector_replace(command_vector_t *cv, command_t c, int index);

bool validate_command_vector(command_vector_t *cv);

void command_vector_print(command_vector_t *cv);

void command_vector_free(command_vector_t *cv);

command_vector_t *parse_file(char *filename);

/**
 * Initializes a command vector.
 * @param capacity The capacity.
 * @return The initialized command vector.
 */
command_vector_t *command_vector_init(int capacity) {
    command_vector_t *cv = (command_vector_t *) malloc(
            sizeof(command_vector_t));

    if (cv == NULL)
        print_error(__FILENAME__, __LINE__, __func__, "Malloc failed");

    cv->size = 0;
    cv->capacity = capacity;
    cv->commands = (command_t *) malloc(capacity * sizeof(command_t));

    if (cv->commands == NULL)
        print_error(__FILENAME__, __LINE__, __func__, "Malloc failed");

    return cv;
}

/**
 * Adds a command to the command vector.
 * @param cv The command vector.
 * @param c The command.
 */
void command_vector_add(command_vector_t *cv, command_t c) {
    if (cv->size == cv->capacity) {
        cv->capacity *= 2;
        cv->commands = (command_t *) realloc(cv->commands,
                                             cv->capacity * sizeof(command_t));

        if (cv->commands == NULL)
            print_error(__FILENAME__, __LINE__, __func__, "Realloc failed");
    }
    cv->commands[cv->size] = c;
    cv->size++;
}

/**
 * Replaces a command in the command vector.
 * @param cv The command vector.
 * @param c The command.
 * @param index The index.
 */
void command_vector_replace(command_vector_t *cv, command_t c, int index) {
    if (index < 0 || index >= cv->size)
        print_error(__FILENAME__, __LINE__, __FUNCTION__,
                    "Index out of bounds");

    cv->commands[index] = c;
}

/**
 * Validate command vector.
 * @param cv The command vector.
 * @return true if valid, false otherwise.
 */
bool validate_command_vector(command_vector_t *cv) {
    if (cv == NULL)
        print_error(__FILENAME__, __LINE__, __func__, "Command vector is NULL");

    if (cv->size == 0)
        print_error(__FILENAME__, __LINE__, __FUNCTION__,
                    "Invalid command vector");

    /**
     * Command vector must contain only one U command.
     */
    int u_count = 0;
    for (int i = 0; i < cv->size; i++) {
        if (cv->commands[i].type == U) {
            u_count++;
        }
    }
    if (u_count != 1) {
        return false;
    }

    /**
     * Commands vector must contain only the following commands: U, S, R, C.
     */
    for (int i = 0; i < cv->size; i++) {
        if (cv->commands[i].type != U && cv->commands[i].type != S &&
            cv->commands[i].type != R && cv->commands[i].type != C) {
            return false;
        }
    }

}

/**
 * Prints the command vector.
 * @param cv The command vector.
 */
void command_vector_print(command_vector_t *cv) {
    for (int i = 0; i < cv->size; i++) {
        print_command(&cv->commands[i]);
    }
}

/**
 * Frees the command vector.
 * @param cv The command vector.
 */
void command_vector_free(command_vector_t *cv) {
    if (cv->commands == NULL)
        print_error(__FILENAME__, __LINE__, __FUNCTION__, "Invalid pointer");

    free(cv->commands);
    free(cv);
}

/**
 * Parses file to command vector.
 * @param filename The file name.
 * @return The command vector.
 */
command_vector_t *parse_file(char *filename) {
    command_vector_t *cv = command_vector_init(1);
    FILE *fp = fopen(filename, "r");

    if (fp == NULL)
        print_error(__FILENAME__, __LINE__, __func__, "File not found");

    char *line = NULL;
    size_t len = 0;

    while (getline(&line, &len, fp) != -1) {
        command_t *c = parse_command(line);
        command_vector_add(cv, *c);
    }

    fclose(fp);

    return cv;
}

/**
 * -----------------------------------------------------------------------------
 * COMMAND MODULE [OPERATION]
 * -----------------------------------------------------------------------------
 */

/**
 * Operation type
 */
typedef struct {
    char *name;
    void *pointer;
} operation;

/**
 * Initializes an operation.
 * @param name The name.
 * @param pointer The pointer.
 * @return The initialized operation.
 */
operation *operation_init(char *name, void *pointer) {
    if (pointer == NULL)
        print_error(__FILENAME__, __LINE__, __func__, "Invalid pointer");

    operation *o = (operation *) malloc(sizeof(operation));

    if (o == NULL)
        print_error(__FILENAME__, __LINE__, __func__, "Malloc failed");

    o->name = name;
    o->pointer = pointer;

    return o;
}


/**
 * Frees an operation.
 * @param o The operation.
 */
void operation_free(operation *o) {
    if (o == NULL)
        print_error(__FILENAME__, __LINE__, __func__, "Invalid pointer");

    free(o);
}

/**
 * -----------------------------------------------------------------------------
 * COMMAND MODULE [OPERATION MAP]
 * -----------------------------------------------------------------------------
 */

typedef struct operation_map_t operation_map_t;

typedef struct operation_map_t {
    char *name;

    void (*function)(char *);

    operation_map_t *next;
};

operation_map_t *operation_map_init(char *name, void (*function)(char *)) {
    if (function == NULL)
        print_error(__FILENAME__, __LINE__, __func__, "Invalid pointer");

    operation_map_t *om = (operation_map_t *) malloc(sizeof(operation_map_t));

    if (om == NULL)
        print_error(__FILENAME__, __LINE__, __func__, "Malloc failed");

    om->name = name;
    om->function = function;
    om->next = NULL;

    return om;
}

void
operation_map_add(operation_map_t *om, char *name, void (*function)(char *)) {
    if (om == NULL)
        print_error(__FILENAME__, __LINE__, __func__, "Invalid pointer");

    if (function == NULL)
        print_error(__FILENAME__, __LINE__, __func__, "Invalid pointer");

    operation_map_t *temp = om;

    while (temp->next != NULL) {
        temp = temp->next;
    }

    temp->next = operation_map_init(name, function);
}

// create a function to get the first element of the map
operation_map_t *operation_map_get_first(operation_map_t *om) {
    if (om == NULL)
        print_error(__FILENAME__, __LINE__, __func__, "Invalid pointer");

    return om;
}

// create a function to get the last element of the map
operation_map_t *operation_map_get_last(operation_map_t *om) {
    if (om == NULL)
        print_error(__FILENAME__, __LINE__, __func__, "Invalid pointer");

    operation_map_t *temp = om;

    while (temp->next != NULL) {
        temp = temp->next;
    }

    return temp;
}

// create a function to get size of the map
int operation_map_size(operation_map_t *om) {
    if (om == NULL)
        print_error(__FILENAME__, __LINE__, __func__, "Invalid pointer");

    int size = 0;
    operation_map_t *temp = om;

    while (temp != NULL) {
        size++;
        temp = temp->next;
    }

    return size;
}

operation_map_t *find_operation(char *name, operation_map_t *om) {
    if (om == NULL)
        print_error(__FILENAME__, __LINE__, __func__, "Invalid pointer");

    while (om != NULL) {
        if (strcmp(om->name, name) == 0)
            return om;
        om = om->next;
    }

    return NULL;
}

// create a function to free the map
void operation_map_free(operation_map_t *om) {
    if (om == NULL)
        print_error(__FILENAME__, __LINE__, __func__, "Invalid pointer");

    operation_map_t *temp = om;

    while (temp != NULL) {
        om = om->next;
        free(temp);
        temp = om;
    }
}


/**
 * -----------------------------------------------------------------------------
 * COMMAND MODULE [COMMAND SYSTEM]
 * -----------------------------------------------------------------------------
 */

/**
 * Command system type.
 */
typedef struct {
    char *filename;
    command_vector_t *cv;
    operation_map_t *operation_map;
    set_t universe;
} command_system_t;

/**
 * Initializes a command system.
 * @param filename The file name.
 * @return The initialized command system.
 */
command_system_t *command_system_init(char *filename) {
    command_system_t *cs = (command_system_t *) malloc(
            sizeof(command_system_t));

    if (cs == NULL)
        print_error(__FILENAME__, __LINE__, __func__, "Malloc failed");

    cs->filename = filename;
    cs->cv = parse_file(filename);

    return cs;
}

void command_system_free(command_system_t *cs) {
    if (cs == NULL)
        print_error(__FILENAME__, __LINE__, __func__, "Invalid pointer");

    command_vector_free(cs->cv);
    free(cs);
}

/**
 * Definition for relations
 */
typedef struct {
    char ***pairs;
    int count_pairs;
} relations;

/**
 * Prototypes of functions
 */
void rel_table(int **table, relations *rel_arr, set_t *univerzum);

bool reflexive(relations *rel_arr, set_t *univerzum);

bool symmetric(relations *rel_arr, set_t *univerzum);

bool antisymmetric(relations *rel_arr, set_t *univerzum);

bool transitive(relations *rel_arr, set_t *univerzum);

bool function(relations *rel_arr, set_t *univerzum);


/**
 * Main function.
 * @param argc The number of arguments.
 * @param argv The arguments.
 * @return 0 if the program ran successfully, 1 otherwise.
 */
int main(int argc, char *argv[]) {
    command_system_t *cs = command_system_init("test_001.txt");
    command_vector_print(cs->cv);

    command_system_free(cs);

//    FILE *file = fopen(argv[1], "r");
//    if (file == NULL) {
//        fprintf(stderr, "Error during opening the file!\n");
//        return 1;
//    }
//    char row[101];
//    char *istr;
//    univerzum *univerzum;
//    univerzum = set_init(1);
//    set_t **sets = NULL;
//    int set_amount = 0;
//    while (fgets(row, 101, file)) {
//        istr = strtok(row, " \n");
//        if (strcmp(istr, "U") == 0) {
//            istr = strtok(NULL, " \n");
//            while (istr != NULL) {
//                set_add(univerzum, istr);
//                istr = strtok(NULL, " \n");
//            }
//        } else if (strcmp(istr, "S") == 0) {
//            set_t *s;
//            s = set_init(1);
//            istr = strtok(NULL, " \n");
//            while (istr != NULL) {
//                set_add(s, istr);
//                istr = strtok(NULL, " \n");
//            }
//        }
//    }
//
//    for (int i = 0; i < set_amount; i++) {
//        for (int j = 0; j < sets[i]->size; j++) {
//            printf("%s ", sets[i]->elements[j]);
//        }
//        printf("\n");
//    }
//
//    fclose(file);
    return 0;
}


/**
 * function for file opening
 */


/**
 * table of 0 and 1 for relations
 */
void rel_table(int **table, relations *rel_arr, set_t *univerzum) {
    for (int i = 0; i < univerzum->size; i++) {
        for (int j = 0; j < univerzum->size; j++) {
            table[i][j] = 0;
        }
    }
    for (int i = 0; i < rel_arr->count_pairs; i++) {
        for (int j = 0; j < univerzum->size; j++) {
            if (strcmp(rel_arr->pairs[i][0], univerzum->elements[j]) == 0) {
                for (int k = 0; k < univerzum->size; k++) {
                    if (strcmp(rel_arr->pairs[i][1], univerzum->elements[k]) ==
                        0) {
                        table[j][k] = 1;
                    }
                }
            }
        }
    }
    for (int i = 0; i < univerzum->size; i++) {
        for (int j = 0; j < univerzum->size; j++) {
            printf("%d ", table[i][j]);
        }
        printf("\n");
    }
}

/**
 * Checks if the relation is reflexive.
 * @param rel_arr The array of relation pairs.
 * @param univerzum The univerzum.
 * @return true if the relation is reflexive, false otherwise.
 */
bool reflexive(relations *rel_arr, set_t *univerzum) {
    int table[univerzum->size][univerzum->size];
    rel_table(table, rel_arr, univerzum);
    for (int i = 0; i < univerzum->size; i++) {
        if (table[i][i] == 0) {
            return false;
        }
    }
    return true;
}

/**
 * Checks if the relation is symmetric.
 * @param rel_arr The array of relation pairs.
 * @param univerzum The univerzum.
 * @return true if the relation is symmetric, false otherwise.
 */
bool symmetric(relations *rel_arr, set_t *univerzum) {
    int table[univerzum->size][univerzum->size];
    rel_table(table, rel_arr, univerzum);
    for (int i = 0; i < univerzum->size; i++) {
        for (int j = 0; j < univerzum->size; j++) {
            if (table[i][j] == 1 && table[j][i] == 0) {
                return false;
            }
        }
    }
    return true;
}

/**
 * Checks if the relation is antisymmetric.
 * @param rel_arr The array of relation pairs.
 * @param univerzum The univerzum.
 * @return true if the relation is antisymmetric, false otherwise.
 */
bool antisymmetric(relations *rel_arr, set_t *univerzum) {
    int table[univerzum->size][univerzum->size];
    rel_table(table, rel_arr, univerzum);
    for (int i = 0; i < univerzum->size; i++) {
        for (int j = 0; j < univerzum->size; j++) {
            if (table[i][j] == 1 && table[j][i] == 1 && i != j) {
                return false;
            }
        }
    }
    return true;
}

/**
 * Checks if the relation is transitive.
 * @param rel_arr The array of relation pairs.
 * @param univerzum The univerzum.
 * @return true if the relation is transitive, false otherwise.
 */
bool transitive(relations *rel_arr, set_t *univerzum) {
    int table[univerzum->size][univerzum->size];
    rel_table(table, rel_arr, univerzum);
    for (int i = 0; i < univerzum->size; i++) {
        for (int j = 0; j < univerzum->size; j++) {
            for (int k = 0; k < univerzum->size; k++) {
                if (table[i][j] == 1 && table[j][k] == 1 && table[i][k] == 0) {
                    return false;
                }
            }
        }
    }
    return true;
}

/**
 * Checks if the relation is function.
 * @param rel_arr The array of relation pairs.
 * @param univerzum The univerzum.
 * @return true if the relation is function, false otherwise.
 */
bool function(relations *rel_arr, set_t *univerzum) {
    int table[univerzum->size][univerzum->size];
    rel_table(table, rel_arr, univerzum);
    for (int i = 0; i < univerzum->size; i++) {
        int pocet_relaci = 0;
        for (int j = 0; j < univerzum->size; j++) {
            if (table[i][j] == 1) {
                pocet_relaci++;
            }
            if (pocet_relaci == 2) {
                return false;
            }
        }
    }
    return true;
}


/** muj main, zatim je blby, ale umi spocitat univerzu
 *
 int main (int argc, char *argv[]){
    FILE *fp;
    if ((fp = fopen(argv[1], "r")) == NULL){
        printf("Nepodarilo se otevrit soubor!\n");
        return 0;
    }
    char radek[101];
    char *istr;
    set_t univerzum;
    univerzum.pocet = 0;
    univerzum.prvky = NULL;
    mnozina_t *mnoziny;
    mnoziny = NULL;
    int pocet_mnozin = 0;
    while (fgets(radek, 101, fp)){
        istr = strtok(radek, " \n");
        if (strcmp(istr,"U") == 0){
            istr = strtok(NULL, " \n");
            while (istr != NULL){
                set_add(&univerzum, istr);
                istr = strtok(NULL, " \n");
            }
        }
        else if (strcmp(istr,"S") == 0){
            mnozina_t mnozina;
            mnozina.prvky = NULL;
            mnozina.pocet = 0;
            istr = strtok(NULL, " \n");
            while(istr != NULL){
                mnozina_add_prvek(&mnozina, istr);
                istr = strtok(NULL, " \n");
            }
            pole_add_mnozina(mnoziny, mnozina, &pocet_mnozin);
        }
    }
    for (int i = 0; i < univerzum.pocet; i++){
        printf("%s\n",univerzum.prvky[i]);
    }
    //printf("\n%s\n", mnoziny[0].prvky[1]);
    //for (int i = 0; i < pocet_mnozin; i++){
    //    for (int j = 0; j < mnoziny[i].pocet; j++){
    //        printf("%s ", mnoziny[i].prvky[j]);
    //    }
    //    printf("\n");
    //}
    printf("dazhe syuda doshel\n");
    for (int i = 0; i < univerzum.pocet; i++){
        free(univerzum.prvky[i]);
    }
    //for (int i = 0; i < pocet_mnozin;  i++){
    //    for (int j = 0; j < mnoziny[i].pocet; j++){
    //        free(mnoziny[i].prvky[j]);
    //    }
    //}
    free(univerzum.prvky);
    //free(mnoziny);
    fclose(fp);
    return 0;
}
*/