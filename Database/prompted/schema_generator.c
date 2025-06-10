#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <sqlite3.h>

#define MAX_LINE_LENGTH 1024
#define MAX_WORDS 20
#define MAX_WORD_LENGTH 100

typedef struct {
    char name[MAX_WORD_LENGTH];
    char type[MAX_WORD_LENGTH];
    char constraints[MAX_LINE_LENGTH];
} Column;

typedef struct {
    char name[MAX_WORD_LENGTH];
    Column* columns;
    int column_count;
    int column_capacity;
} Table;

typedef struct {
    Table* tables;
    int table_count;
    int table_capacity;
} Schema;

// Function prototypes
void init_schema(Schema* schema);
void free_schema(Schema* schema);
void add_table(Schema* schema, const char* name);
void add_column(Schema* schema, const char* table_name, const char* name, const char* type, const char* constraints);
void parse_schema_file(const char* filename, Schema* schema);
void generate_create_statements(Schema* schema);
void execute_schema(Schema* schema, const char* db_path);
void print_usage(const char* program_name);

int main(int argc, char* argv[]) {
    if (argc < 2) {
        print_usage(argv[0]);
        return 1;
    }

    const char* schema_file = argv[1];
    const char* db_path = (argc > 2) ? argv[2] : "database.db";

    Schema schema;
    init_schema(&schema);

    // Parse the schema file
    parse_schema_file(schema_file, &schema);

    // Generate and print CREATE statements
    generate_create_statements(&schema);

    // Ask user if they want to execute the statements
    printf("\nDo you want to execute these statements? (y/n): ");
    char response[10];
    if (fgets(response, sizeof(response), stdin) && (response[0] == 'y' || response[0] == 'Y')) {
        execute_schema(&schema, db_path);
        printf("Schema created successfully!\n");
    }

    free_schema(&schema);
    return 0;
}

void init_schema(Schema* schema) {
    schema->tables = NULL;
    schema->table_count = 0;
    schema->table_capacity = 0;
}

void free_schema(Schema* schema) {
    for (int i = 0; i < schema->table_count; i++) {
        free(schema->tables[i].columns);
    }
    free(schema->tables);
}

void add_table(Schema* schema, const char* name) {
    if (schema->table_count >= schema->table_capacity) {
        schema->table_capacity = schema->table_capacity == 0 ? 4 : schema->table_capacity * 2;
        schema->tables = realloc(schema->tables, schema->table_capacity * sizeof(Table));
    }

    Table* table = &schema->tables[schema->table_count++];
    strncpy(table->name, name, MAX_WORD_LENGTH - 1);
    table->name[MAX_WORD_LENGTH - 1] = '\0';
    table->columns = NULL;
    table->column_count = 0;
    table->column_capacity = 0;
}

void add_column(Schema* schema, const char* table_name, const char* name, const char* type, const char* constraints) {
    // Find the table
    Table* table = NULL;
    for (int i = 0; i < schema->table_count; i++) {
        if (strcmp(schema->tables[i].name, table_name) == 0) {
            table = &schema->tables[i];
            break;
        }
    }

    if (!table) return;

    // Add the column
    if (table->column_count >= table->column_capacity) {
        table->column_capacity = table->column_capacity == 0 ? 4 : table->column_capacity * 2;
        table->columns = realloc(table->columns, table->column_capacity * sizeof(Column));
    }

    Column* column = &table->columns[table->column_count++];
    strncpy(column->name, name, MAX_WORD_LENGTH - 1);
    column->name[MAX_WORD_LENGTH - 1] = '\0';
    strncpy(column->type, type, MAX_WORD_LENGTH - 1);
    column->type[MAX_WORD_LENGTH - 1] = '\0';
    strncpy(column->constraints, constraints, MAX_LINE_LENGTH - 1);
    column->constraints[MAX_LINE_LENGTH - 1] = '\0';
}

void parse_schema_file(const char* filename, Schema* schema) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        printf("Error: Could not open file %s\n", filename);
        exit(1);
    }

    char line[MAX_LINE_LENGTH];
    char current_table[MAX_WORD_LENGTH] = "";
    char* words[MAX_WORDS];
    char line_copy[MAX_LINE_LENGTH];

    while (fgets(line, sizeof(line), file)) {
        // Skip empty lines and comments
        if (line[0] == '\n' || line[0] == '#' || line[0] == '\0') continue;

        // Remove newline
        line[strcspn(line, "\n")] = 0;

        // Skip empty lines after newline removal
        if (strlen(line) == 0) continue;

        // Split line into words
        strcpy(line_copy, line);
        char* word = strtok(line_copy, " \t");
        int word_count = 0;

        while (word && word_count < MAX_WORDS) {
            words[word_count++] = word;
            word = strtok(NULL, " \t");
        }

        if (word_count == 0) continue;

        // Check if this is a table definition
        if (strcmp(words[0], "TABLE") == 0 && word_count >= 2) {
            strcpy(current_table, words[1]);
            add_table(schema, current_table);
        }
        // Check if this is a column definition
        else if (strlen(current_table) > 0 && word_count >= 2) {
            char constraints[MAX_LINE_LENGTH] = "";
            for (int i = 2; i < word_count; i++) {
                if (i > 2) strcat(constraints, " ");
                strcat(constraints, words[i]);
            }
            add_column(schema, current_table, words[0], words[1], constraints);
        }
    }

    fclose(file);
}

void generate_create_statements(Schema* schema) {
    printf("Generated CREATE TABLE statements:\n");
    
    for (int i = 0; i < schema->table_count; i++) {
        Table* table = &schema->tables[i];
        printf("\nCREATE TABLE %s (\n", table->name);
        
        for (int j = 0; j < table->column_count; j++) {
            Column* col = &table->columns[j];
            printf("    %s %s", col->name, col->type);
            if (strlen(col->constraints) > 0) {
                printf(" %s", col->constraints);
            }
            if (j < table->column_count - 1) {
                printf(",");
            }
            printf("\n");
        }
        printf(");\n");
    }
}

void execute_schema(Schema* schema, const char* db_path) {
    sqlite3* db;
    char* err_msg = 0;
    int rc;

    rc = sqlite3_open(db_path, &db);
    if (rc != SQLITE_OK) {
        printf("Cannot open database: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return;
    }

    for (int i = 0; i < schema->table_count; i++) {
        Table* table = &schema->tables[i];
        char sql[MAX_LINE_LENGTH * 10] = "CREATE TABLE ";
        strcat(sql, table->name);
        strcat(sql, " (");

        for (int j = 0; j < table->column_count; j++) {
            Column* col = &table->columns[j];
            strcat(sql, col->name);
            strcat(sql, " ");
            strcat(sql, col->type);
            if (strlen(col->constraints) > 0) {
                strcat(sql, " ");
                strcat(sql, col->constraints);
            }
            if (j < table->column_count - 1) {
                strcat(sql, ", ");
            }
        }
        strcat(sql, ");");

        rc = sqlite3_exec(db, sql, 0, 0, &err_msg);
        if (rc != SQLITE_OK) {
            printf("SQL error: %s\n", err_msg);
            sqlite3_free(err_msg);
        }
    }

    sqlite3_close(db);
}

void print_usage(const char* program_name) {
    printf("Usage: %s <schema_file> [database_file]\n", program_name);
    printf("  schema_file: Path to the schema definition file\n");
    printf("  database_file: Path to the SQLite database file (default: database.db)\n");
} 