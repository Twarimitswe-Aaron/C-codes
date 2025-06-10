#include <windows.h>
#include <commctrl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sqlite3.h>

// Include the schema generator code
#include "schema_generator.c"

// Window dimensions
#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

// Control IDs
#define ID_LOAD_BUTTON 101
#define ID_SAVE_BUTTON 102
#define ID_GENERATE_BUTTON 103
#define ID_EXECUTE_BUTTON 104
#define ID_SCHEMA_EDIT 105
#define ID_OUTPUT_EDIT 106

// Global variables
HWND g_hwndSchemaEdit;
HWND g_hwndOutputEdit;
Schema g_schema;

// Function prototypes
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
void CreateControls(HWND hwnd);
void LoadSchemaFile(HWND hwnd);
void SaveSchemaFile(HWND hwnd);
void GenerateSQL(HWND hwnd);
void ExecuteSQL(HWND hwnd);
void AppendOutput(const char* text);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    // Initialize Common Controls
    INITCOMMONCONTROLSEX icex;
    icex.dwSize = sizeof(INITCOMMONCONTROLSEX);
    icex.dwICC = ICC_STANDARD_CLASSES | ICC_WIN95_CLASSES;
    InitCommonControlsEx(&icex);

    // Register the window class
    const char CLASS_NAME[] = "Schema Generator Window Class";
    
    WNDCLASS wc = {0};
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    
    if (!RegisterClass(&wc)) {
        MessageBox(NULL, "Window Registration Failed!", "Error", MB_ICONEXCLAMATION | MB_OK);
        return 0;
    }

    // Create the window
    HWND hwnd = CreateWindowEx(
        WS_EX_CLIENTEDGE,           // Extended window style
        CLASS_NAME,                 // Window class
        "Database Schema Generator", // Window text
        WS_OVERLAPPEDWINDOW,        // Window style
        CW_USEDEFAULT, CW_USEDEFAULT, WINDOW_WIDTH, WINDOW_HEIGHT,
        NULL,       // Parent window    
        NULL,       // Menu
        hInstance,  // Instance handle
        NULL        // Additional application data
    );

    if (hwnd == NULL) {
        MessageBox(NULL, "Window Creation Failed!", "Error", MB_ICONEXCLAMATION | MB_OK);
        return 0;
    }

    // Initialize schema
    init_schema(&g_schema);

    // Create controls
    CreateControls(hwnd);

    // Show the window
    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);

    // Message loop
    MSG msg = {0};
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    // Cleanup
    free_schema(&g_schema);

    return 0;
}

void CreateControls(HWND hwnd) {
    // Create buttons
    CreateWindow(
        "BUTTON", "Load Schema",
        WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
        10, 10, 100, 30,
        hwnd, (HMENU)ID_LOAD_BUTTON, NULL, NULL
    );

    CreateWindow(
        "BUTTON", "Save Schema",
        WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
        120, 10, 100, 30,
        hwnd, (HMENU)ID_SAVE_BUTTON, NULL, NULL
    );

    CreateWindow(
        "BUTTON", "Generate SQL",
        WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
        230, 10, 100, 30,
        hwnd, (HMENU)ID_GENERATE_BUTTON, NULL, NULL
    );

    CreateWindow(
        "BUTTON", "Execute SQL",
        WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
        340, 10, 100, 30,
        hwnd, (HMENU)ID_EXECUTE_BUTTON, NULL, NULL
    );

    // Create schema edit control
    g_hwndSchemaEdit = CreateWindowEx(
        WS_EX_CLIENTEDGE,
        "EDIT",
        "",
        WS_CHILD | WS_VISIBLE | WS_VSCROLL | ES_MULTILINE | ES_AUTOVSCROLL,
        10, 50, WINDOW_WIDTH - 20, (WINDOW_HEIGHT - 100) / 2,
        hwnd,
        (HMENU)ID_SCHEMA_EDIT,
        NULL,
        NULL
    );

    // Create output edit control
    g_hwndOutputEdit = CreateWindowEx(
        WS_EX_CLIENTEDGE,
        "EDIT",
        "",
        WS_CHILD | WS_VISIBLE | WS_VSCROLL | ES_MULTILINE | ES_AUTOVSCROLL | ES_READONLY,
        10, (WINDOW_HEIGHT - 100) / 2 + 60, WINDOW_WIDTH - 20, (WINDOW_HEIGHT - 100) / 2,
        hwnd,
        (HMENU)ID_OUTPUT_EDIT,
        NULL,
        NULL
    );

    // Set fonts
    HFONT hFont = CreateFont(16, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE,
        DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
        DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, "Consolas");
    
    SendMessage(g_hwndSchemaEdit, WM_SETFONT, (WPARAM)hFont, TRUE);
    SendMessage(g_hwndOutputEdit, WM_SETFONT, (WPARAM)hFont, TRUE);

    // Add some initial text to the schema editor
    SetWindowText(g_hwndSchemaEdit, "# Sample database schema\n# Format: TABLE table_name\n#        column_name type [constraints]\n\nTABLE users\nid INTEGER PRIMARY KEY\nusername TEXT NOT NULL UNIQUE\nemail TEXT NOT NULL UNIQUE\npassword_hash TEXT NOT NULL\ncreated_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP");
}

void LoadSchemaFile(HWND hwnd) {
    char filename[MAX_PATH] = {0};
    OPENFILENAME ofn = {0};
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = hwnd;
    ofn.lpstrFilter = "Schema Files\0*.txt\0All Files\0*.*\0";
    ofn.lpstrFile = filename;
    ofn.nMaxFile = MAX_PATH;
    ofn.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;
    ofn.lpstrDefExt = "txt";

    if (GetOpenFileName(&ofn)) {
        // Clear previous schema
        free_schema(&g_schema);
        init_schema(&g_schema);

        // Read file content
        FILE* file = fopen(filename, "r");
        if (file) {
            fseek(file, 0, SEEK_END);
            long size = ftell(file);
            fseek(file, 0, SEEK_SET);

            char* buffer = malloc(size + 1);
            if (buffer) {
                fread(buffer, 1, size, file);
                buffer[size] = 0;
                SetWindowText(g_hwndSchemaEdit, buffer);
                free(buffer);
            }
            fclose(file);
        }
    }
}

void SaveSchemaFile(HWND hwnd) {
    char filename[MAX_PATH] = {0};
    OPENFILENAME ofn = {0};
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = hwnd;
    ofn.lpstrFilter = "Schema Files\0*.txt\0All Files\0*.*\0";
    ofn.lpstrFile = filename;
    ofn.nMaxFile = MAX_PATH;
    ofn.Flags = OFN_EXPLORER | OFN_OVERWRITEPROMPT;
    ofn.lpstrDefExt = "txt";

    if (GetSaveFileName(&ofn)) {
        int length = GetWindowTextLength(g_hwndSchemaEdit);
        char* buffer = malloc(length + 1);
        if (buffer) {
            GetWindowText(g_hwndSchemaEdit, buffer, length + 1);
            FILE* file = fopen(filename, "w");
            if (file) {
                fwrite(buffer, 1, length, file);
                fclose(file);
            }
            free(buffer);
        }
    }
}

void GenerateSQL(HWND hwnd) {
    // Get schema text
    int length = GetWindowTextLength(g_hwndSchemaEdit);
    char* buffer = malloc(length + 1);
    if (buffer) {
        GetWindowText(g_hwndSchemaEdit, buffer, length + 1);
        
        // Save to temporary file
        FILE* file = fopen("temp_schema.txt", "w");
        if (file) {
            fwrite(buffer, 1, length, file);
            fclose(file);

            // Parse schema
            free_schema(&g_schema);
            init_schema(&g_schema);
            parse_schema_file("temp_schema.txt", &g_schema);

            // Generate SQL
            char sql[MAX_LINE_LENGTH * 10];
            sql[0] = 0;
            for (int i = 0; i < g_schema.table_count; i++) {
                Table* table = &g_schema.tables[i];
                strcat(sql, "CREATE TABLE ");
                strcat(sql, table->name);
                strcat(sql, " (\n");

                for (int j = 0; j < table->column_count; j++) {
                    Column* col = &table->columns[j];
                    strcat(sql, "    ");
                    strcat(sql, col->name);
                    strcat(sql, " ");
                    strcat(sql, col->type);
                    if (strlen(col->constraints) > 0) {
                        strcat(sql, " ");
                        strcat(sql, col->constraints);
                    }
                    if (j < table->column_count - 1) {
                        strcat(sql, ",");
                    }
                    strcat(sql, "\n");
                }
                strcat(sql, ");\n\n");
            }

            // Display SQL
            SetWindowText(g_hwndOutputEdit, sql);
            remove("temp_schema.txt");
        }
        free(buffer);
    }
}

void ExecuteSQL(HWND hwnd) {
    char filename[MAX_PATH] = {0};
    OPENFILENAME ofn = {0};
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = hwnd;
    ofn.lpstrFilter = "SQLite Database\0*.db\0All Files\0*.*\0";
    ofn.lpstrFile = filename;
    ofn.nMaxFile = MAX_PATH;
    ofn.Flags = OFN_EXPLORER | OFN_OVERWRITEPROMPT;
    ofn.lpstrDefExt = "db";

    if (GetSaveFileName(&ofn)) {
        execute_schema(&g_schema, filename);
        AppendOutput("\nDatabase created successfully!");
    }
}

void AppendOutput(const char* text) {
    int length = GetWindowTextLength(g_hwndOutputEdit);
    char* buffer = malloc(length + strlen(text) + 2);
    if (buffer) {
        GetWindowText(g_hwndOutputEdit, buffer, length + 1);
        strcat(buffer, text);
        strcat(buffer, "\n");
        SetWindowText(g_hwndOutputEdit, buffer);
        free(buffer);
    }
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
        case WM_COMMAND:
            switch (LOWORD(wParam)) {
                case ID_LOAD_BUTTON:
                    LoadSchemaFile(hwnd);
                    break;
                case ID_SAVE_BUTTON:
                    SaveSchemaFile(hwnd);
                    break;
                case ID_GENERATE_BUTTON:
                    GenerateSQL(hwnd);
                    break;
                case ID_EXECUTE_BUTTON:
                    ExecuteSQL(hwnd);
                    break;
            }
            break;
        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
} 