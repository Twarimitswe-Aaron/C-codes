#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "book_manager.h"
#include "database.h"

// Forward declarations with parameter names
LRESULT CALLBACK WindowProcedure(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp);
void showBookManagementGUI();
void showAuthorManagementGUI();
void showPublisherManagementGUI();
void showMemberManagementGUI();
void showStaffManagementGUI();
void showBorrowingManagementGUI();
void showFineManagementGUI();
LRESULT CALLBACK BookManagementWindowProcedure(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp);
LRESULT CALLBACK AuthorManagementWindowProcedure(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp);
LRESULT CALLBACK PublisherManagementWindowProcedure(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp);
LRESULT CALLBACK MemberManagementWindowProcedure(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp);
LRESULT CALLBACK StaffManagementWindowProcedure(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp);
LRESULT CALLBACK BorrowingManagementWindowProcedure(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp);
LRESULT CALLBACK FineManagementWindowProcedure(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp);

// Global variables
HWND hwnd;
HWND hwndButton[10];
HBRUSH hBrushLight = NULL;  // Will be initialized in WM_CREATE
HBRUSH hBrushDark = NULL;   // Will be initialized in WM_CREATE
BOOL isDarkTheme = FALSE;
HFONT hFont = NULL;
HFONT hFontBold = NULL;  // For headers
HFONT hFontButton = NULL;  // For buttons

// Define colors
COLORREF colorPrimary = RGB(0, 120, 215);      // Windows blue
COLORREF colorPrimaryDark = RGB(0, 99, 177);   // Darker blue
COLORREF colorSuccess = RGB(46, 204, 113);     // Green
COLORREF colorError = RGB(231, 76, 60);        // Red
COLORREF colorWarning = RGB(241, 196, 15);     // Yellow

// Define dimensions
const int WINDOW_WIDTH = 1000;
const int WINDOW_HEIGHT = 700;
const int BUTTON_WIDTH = 180;
const int BUTTON_HEIGHT = 40;
const int BUTTON_SPACING = 15;
const int MARGIN = 20;
const int CONTROL_HEIGHT = 30;
const int LABEL_WIDTH = 150;
const int EDIT_WIDTH = 250;

const char* buttonTexts[] = {
    "Books", "Authors", "Publishers",
    "Members", "Staffs", "Borrowings",
    "Fines", "Exit"
};

// Define IDs
#define ID_BOOK_MGMT     1001
#define ID_AUTHOR_MGMT   1002
#define ID_PUBLISHER_MGMT 1003
#define ID_MEMBER_MGMT   1004
#define ID_STAFF_MGMT    1005
#define ID_BORROWING_MGMT 1006
#define ID_FINE_MGMT     1007
#define ID_EXIT          1008
#define ID_THEME_TOGGLE  1009
#define ID_CLOCK         1010

// Define button IDs
const int buttonIds[] = {
    ID_BOOK_MGMT, ID_AUTHOR_MGMT, ID_PUBLISHER_MGMT,
    ID_MEMBER_MGMT, ID_STAFF_MGMT, ID_BORROWING_MGMT,
    ID_FINE_MGMT, ID_EXIT
};

BOOL CALLBACK SetChildTheme(HWND hwndChild, LPARAM lParam) {
    InvalidateRect(hwndChild, NULL, TRUE);
    UpdateWindow(hwndChild);
    return TRUE;
}

LRESULT CALLBACK WindowProcedure(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp) {
    static HBRUSH hBrushCurrent = NULL;

    switch(msg) {
        case WM_CREATE:
            // Initialize brushes
            hBrushLight = CreateSolidBrush(RGB(245, 245, 245));  // Lighter background
            hBrushDark = CreateSolidBrush(RGB(32, 32, 32));     // Darker background
            hBrushCurrent = isDarkTheme ? hBrushDark : hBrushLight;

            // Create font
            hFont = CreateFont(
                16, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, DEFAULT_CHARSET,
                OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
                DEFAULT_PITCH | FF_SWISS, "Segoe UI"
            );

            // Apply font to buttons
            for (int i = 0; i < 10; i++) {
                if (hwndButton[i]) {
                    SendMessage(hwndButton[i], WM_SETFONT, (WPARAM)hFont, TRUE);
                }
            }

            // Create fonts
            hFontBold = CreateFont(
                18, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE,
                DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
                CLEARTYPE_QUALITY, DEFAULT_PITCH | FF_SWISS, "Segoe UI"
            );

            hFontButton = CreateFont(
                16, 0, 0, 0, FW_SEMIBOLD, FALSE, FALSE, FALSE,
                DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
                CLEARTYPE_QUALITY, DEFAULT_PITCH | FF_SWISS, "Segoe UI"
            );

            // Create header
            CreateWindow("STATIC", "Library Management System",
                WS_VISIBLE | WS_CHILD | SS_CENTER,
                MARGIN, MARGIN, WINDOW_WIDTH - 2 * MARGIN, 40,
                hwnd, NULL, GetModuleHandle(NULL), NULL);

            // Create buttons with improved styling
            int startY = MARGIN + 60;
            int startX = MARGIN;
            int buttonsPerRow = 3;
            int currentRow = 0;

            for(int i = 0; i < 8; i++) {
                hwndButton[i] = CreateWindow(
                    "BUTTON", buttonTexts[i],
                    WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON | BS_FLAT,
                    startX + (i % buttonsPerRow) * (BUTTON_WIDTH + BUTTON_SPACING),
                    startY + (i / buttonsPerRow) * (BUTTON_HEIGHT + BUTTON_SPACING),
                    BUTTON_WIDTH, BUTTON_HEIGHT,
                    hwnd, (HMENU)(intptr_t)buttonIds[i],
                    GetModuleHandle(NULL), NULL
                );
                SendMessage(hwndButton[i], WM_SETFONT, (WPARAM)hFontButton, TRUE);
            }

            // Theme toggle and clock buttons
            hwndButton[8] = CreateWindow(
                "BUTTON", isDarkTheme ? "Light" : "Dark",
                WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON | BS_FLAT,
                WINDOW_WIDTH - 2 * MARGIN - 100, MARGIN,
                40, 40, hwnd, (HMENU)ID_THEME_TOGGLE,
                GetModuleHandle(NULL), NULL
            );

            hwndButton[9] = CreateWindow(
                "BUTTON", "Time",
                WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON | BS_FLAT,
                WINDOW_WIDTH - MARGIN - 100, MARGIN,
                40, 40, hwnd, (HMENU)ID_CLOCK,
                GetModuleHandle(NULL), NULL
            );
            break;

        case WM_COMMAND:
            switch(LOWORD(wp)) {
                case ID_BOOK_MGMT: showBookManagementGUI(); break;
                case ID_AUTHOR_MGMT: showAuthorManagementGUI(); break;
                case ID_PUBLISHER_MGMT: showPublisherManagementGUI(); break;
                case ID_MEMBER_MGMT: showMemberManagementGUI(); break;
                case ID_STAFF_MGMT: showStaffManagementGUI(); break;
                case ID_BORROWING_MGMT: showBorrowingManagementGUI(); break;
                case ID_FINE_MGMT: showFineManagementGUI(); break;
                case ID_EXIT: DestroyWindow(hwnd); break;
                case ID_THEME_TOGGLE:
                    isDarkTheme = !isDarkTheme;
                    hBrushCurrent = isDarkTheme ? hBrushDark : hBrushLight;
                    SetWindowText(hwndButton[8], isDarkTheme ? "Light" : "Moon");
                    InvalidateRect(hwnd, NULL, TRUE);
                    UpdateWindow(hwnd);
                    EnumChildWindows(hwnd, SetChildTheme, 0);
                    break;
                case ID_CLOCK:
                    MessageBox(hwnd, "Current Time: 08:13 PM CAT\nDate: June 03, 2025", "Digital Clock", MB_OK | MB_ICONINFORMATION);
                    break;
            }
            break;

        case WM_CTLCOLORSTATIC:
        case WM_CTLCOLORBTN:
        case WM_CTLCOLOREDIT: {
            HDC hdc = (HDC)wp;
            if (isDarkTheme) {
                SetTextColor(hdc, RGB(255, 255, 255));
                SetBkColor(hdc, RGB(32, 32, 32));
                return (LRESULT)hBrushDark;
            } else {
                SetTextColor(hdc, RGB(0, 0, 0));
                SetBkColor(hdc, RGB(245, 245, 245));
                return (LRESULT)hBrushLight;
            }
        }

        case WM_ERASEBKGND:
            return (LRESULT)1;

        case WM_PAINT: {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);
            RECT rect;
            GetClientRect(hwnd, &rect);
            FillRect(hdc, &rect, hBrushCurrent);
            EndPaint(hwnd, &ps);
            break;
        }

        case WM_DESTROY:
            DeleteObject(hBrushLight);
            DeleteObject(hBrushDark);
            DeleteObject(hFont);
            DeleteObject(hFontBold);
            DeleteObject(hFontButton);
            PostQuitMessage(0);
            break;

        default:
            return DefWindowProc(hwnd, msg, wp, lp);
    }
    return 0;
}

void showGUIMenu() {
    HINSTANCE hInst = GetModuleHandle(NULL);
    WNDCLASSEX wc = {0};
    MSG msg = {0};
    
    wc.cbSize = sizeof(WNDCLASSEX);
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = WindowProcedure;
    wc.hInstance = hInst;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = NULL;
    wc.lpszClassName = "LibraryManagementSystem";
    
    if(!RegisterClassEx(&wc)) return;
    
    hwnd = CreateWindowEx(
        0, "LibraryManagementSystem", "Library Management System",
        WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 900, 600,
        NULL, NULL, hInst, NULL
    );
    if(hwnd == NULL) return;
    
    int buttonIds[] = {ID_BOOK_MGMT, ID_AUTHOR_MGMT, ID_PUBLISHER_MGMT,
                      ID_MEMBER_MGMT, ID_STAFF_MGMT, ID_BORROWING_MGMT,
                      ID_FINE_MGMT, ID_EXIT, ID_THEME_TOGGLE, ID_CLOCK};

    int startX = 10;
    int startY = 10;
    int buttonWidth = 150;
    int buttonHeight = 40;
    int spacingX = 3;

    for(int i = 0; i < 8; i++) {
        hwndButton[i] = CreateWindow(
            "BUTTON", buttonTexts[i], WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON | BS_FLAT,
            startX + (i * (buttonWidth + spacingX)), startY, buttonWidth, buttonHeight, hwnd, (HMENU)(intptr_t)buttonIds[i],
            hInst, NULL
        );
    }

    hwndButton[8] = CreateWindow(
        "BUTTON", isDarkTheme ? "☀" : "☾", WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON | BS_FLAT,
        startX + (8 * (buttonWidth + spacingX)), startY, 40, buttonHeight, hwnd, (HMENU)ID_THEME_TOGGLE,
        hInst, NULL
    );

    hwndButton[9] = CreateWindow(
        "BUTTON", "Clock", WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON | BS_FLAT,
        startX + (8 * (buttonWidth + spacingX)) + 40 + spacingX, startY, 40, buttonHeight, hwnd, (HMENU)ID_CLOCK,
        hInst, NULL
    );
    
    ShowWindow(hwnd, SW_SHOW);
    UpdateWindow(hwnd);
    
    while(GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
}

void showBookManagementGUI() {
    HINSTANCE hInst = GetModuleHandle(NULL);
    WNDCLASSEX wc = {0};
    wc.cbSize = sizeof(WNDCLASSEX);
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = BookManagementWindowProcedure;
    wc.hInstance = hInst;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = NULL;
    wc.lpszClassName = "BookManagementWindow";
    
    if(!RegisterClassEx(&wc)) return;
    
    CreateWindowEx(
        0, "BookManagementWindow", "Book Management",
        WS_OVERLAPPEDWINDOW | WS_VISIBLE, CW_USEDEFAULT, CW_USEDEFAULT,
        600, 400, hwnd, NULL, hInst, NULL
    );
}

LRESULT CALLBACK BookManagementWindowProcedure(HWND hwndBM, UINT msg, WPARAM wp, LPARAM lp) {
    static HWND hEditBookId, hEditTitle, hEditAuthorId, hEditPublisherId, hEditIsbn, hEditGenre, hEditYear, hEditCopies, hEditShelf;
    switch(msg) {
        case WM_CREATE:
            int labelWidth = 120;
            int editWidth = 200;
            int lineHeight = 25;
            int startY = 20;
            int spacingY = 10;
            int startX_label = 20;
            int startX_edit = startX_label + labelWidth + 10;

            CreateWindow("STATIC", "Book ID:", WS_VISIBLE | WS_CHILD, startX_label, startY, labelWidth, 20, hwndBM, NULL, GetModuleHandle(NULL), NULL);
            hEditBookId = CreateWindow("EDIT", "", WS_VISIBLE | WS_CHILD | WS_BORDER | ES_AUTOHSCROLL, startX_edit, startY, 100, 20, hwndBM, NULL, GetModuleHandle(NULL), NULL);

            startY += lineHeight + spacingY;
            CreateWindow("STATIC", "Title:", WS_VISIBLE | WS_CHILD, startX_label, startY, labelWidth, 20, hwndBM, NULL, GetModuleHandle(NULL), NULL);
            hEditTitle = CreateWindow("EDIT", "", WS_VISIBLE | WS_CHILD | WS_BORDER | ES_AUTOHSCROLL, startX_edit, startY, editWidth, 20, hwndBM, NULL, GetModuleHandle(NULL), NULL);

            startY += lineHeight + spacingY;
            CreateWindow("STATIC", "Author ID:", WS_VISIBLE | WS_CHILD, startX_label, startY, labelWidth, 20, hwndBM, NULL, GetModuleHandle(NULL), NULL);
            hEditAuthorId = CreateWindow("EDIT", "", WS_VISIBLE | WS_CHILD | WS_BORDER | ES_AUTOHSCROLL, startX_edit, startY, 100, 20, hwndBM, NULL, GetModuleHandle(NULL), NULL);

            startY += lineHeight + spacingY;
            CreateWindow("STATIC", "Publisher ID:", WS_VISIBLE | WS_CHILD, startX_label, startY, labelWidth, 20, hwndBM, NULL, GetModuleHandle(NULL), NULL);
            hEditPublisherId = CreateWindow("EDIT", "", WS_VISIBLE | WS_CHILD | WS_BORDER | ES_AUTOHSCROLL, startX_edit, startY, 100, 20, hwndBM, NULL, GetModuleHandle(NULL), NULL);

            startY += lineHeight + spacingY;
            CreateWindow("STATIC", "ISBN:", WS_VISIBLE | WS_CHILD, startX_label, startY, labelWidth, 20, hwndBM, NULL, GetModuleHandle(NULL), NULL);
            hEditIsbn = CreateWindow("EDIT", "", WS_VISIBLE | WS_CHILD | WS_BORDER | ES_AUTOHSCROLL, startX_edit, startY, 150, 20, hwndBM, NULL, GetModuleHandle(NULL), NULL);

            startY += lineHeight + spacingY;
            CreateWindow("STATIC", "Genre:", WS_VISIBLE | WS_CHILD, startX_label, startY, labelWidth, 20, hwndBM, NULL, GetModuleHandle(NULL), NULL);
            hEditGenre = CreateWindow("EDIT", "", WS_VISIBLE | WS_CHILD | WS_BORDER | ES_AUTOHSCROLL, startX_edit, startY, editWidth, 20, hwndBM, NULL, GetModuleHandle(NULL), NULL);

            startY += lineHeight + spacingY;
            CreateWindow("STATIC", "Year Published:", WS_VISIBLE | WS_CHILD, startX_label, startY, labelWidth, 20, hwndBM, NULL, GetModuleHandle(NULL), NULL);
            hEditYear = CreateWindow("EDIT", "", WS_VISIBLE | WS_CHILD | WS_BORDER | ES_AUTOHSCROLL, startX_edit, startY, 80, 20, hwndBM, NULL, GetModuleHandle(NULL), NULL);

            startY += lineHeight + spacingY;
            CreateWindow("STATIC", "Copies Available:", WS_VISIBLE | WS_CHILD, startX_label, startY, labelWidth, 20, hwndBM, NULL, GetModuleHandle(NULL), NULL);
            hEditCopies = CreateWindow("EDIT", "", WS_VISIBLE | WS_CHILD | WS_BORDER | ES_AUTOHSCROLL, startX_edit, startY, 80, 20, hwndBM, NULL, GetModuleHandle(NULL), NULL);

            startY += lineHeight + spacingY;
            CreateWindow("STATIC", "Shelf Location:", WS_VISIBLE | WS_CHILD, startX_label, startY, labelWidth, 20, hwndBM, NULL, GetModuleHandle(NULL), NULL);
            hEditShelf = CreateWindow("EDIT", "", WS_VISIBLE | WS_CHILD | WS_BORDER | ES_AUTOHSCROLL, startX_edit, startY, editWidth, 20, hwndBM, NULL, GetModuleHandle(NULL), NULL);

            startY += lineHeight + spacingY + 10;
            int buttonWidth = 120;
            int buttonSpacing = 10;
            int buttonX = startX_label;

            CreateWindow("BUTTON", "Add Book", WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON | BS_FLAT, buttonX, startY, buttonWidth, 30, hwndBM, (HMENU)101, GetModuleHandle(NULL), NULL);
            buttonX += buttonWidth + buttonSpacing;
            CreateWindow("BUTTON", "View All Books", WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON | BS_FLAT, buttonX, startY, buttonWidth + 30, 30, hwndBM, (HMENU)102, GetModuleHandle(NULL), NULL);
            buttonX += buttonWidth + 30 + buttonSpacing;
            CreateWindow("BUTTON", "Update Book", WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON | BS_FLAT, buttonX, startY, buttonWidth, 30, hwndBM, (HMENU)103, GetModuleHandle(NULL), NULL);

            SendMessage(hEditBookId, WM_SETFONT, (WPARAM)hFont, TRUE);
            SendMessage(hEditTitle, WM_SETFONT, (WPARAM)hFont, TRUE);
            SendMessage(hEditAuthorId, WM_SETFONT, (WPARAM)hFont, TRUE);
            SendMessage(hEditPublisherId, WM_SETFONT, (WPARAM)hFont, TRUE);
            SendMessage(hEditIsbn, WM_SETFONT, (WPARAM)hFont, TRUE);
            SendMessage(hEditGenre, WM_SETFONT, (WPARAM)hFont, TRUE);
            SendMessage(hEditYear, WM_SETFONT, (WPARAM)hFont, TRUE);
            SendMessage(hEditCopies, WM_SETFONT, (WPARAM)hFont, TRUE);
            SendMessage(hEditShelf, WM_SETFONT, (WPARAM)hFont, TRUE);
            break;

        case WM_COMMAND:
            if (LOWORD(wp) == 101) { // Add Book
                Book newBook;
                char buffer[MAX_STRING];
                GetWindowText(hEditBookId, buffer, MAX_STRING); newBook.book_id = atoi(buffer);
                GetWindowText(hEditTitle, newBook.title, MAX_STRING);
                GetWindowText(hEditAuthorId, buffer, MAX_STRING); newBook.author_id = atoi(buffer);
                GetWindowText(hEditPublisherId, buffer, MAX_STRING); newBook.publisher_id = atoi(buffer);
                GetWindowText(hEditIsbn, newBook.isbn, MAX_STRING);
                GetWindowText(hEditGenre, newBook.genre, MAX_STRING);
                GetWindowText(hEditYear, buffer, MAX_STRING); newBook.year_published = atoi(buffer);
                GetWindowText(hEditCopies, buffer, MAX_STRING); newBook.copies_available = atoi(buffer);
                GetWindowText(hEditShelf, newBook.shelf_location, MAX_STRING);
                saveBook(newBook);
                MessageBox(hwndBM, "Book added successfully!", "Success", MB_OK);
            } else if (LOWORD(wp) == 102) { // View All Books
                int count;
                Book* books = loadBooks(&count);
                char message[2000];
                strcpy(message, "Books:\n");
                if (count == 0) strcat(message, "No books found.\n");
                else {
                    char temp[500];
                    for(int i = 0; i < count; i++) {
                        sprintf(temp, "Book ID: %d, Title: %s, Author ID: %d, Publisher ID: %d, ISBN: %s, Genre: %s, Year: %d, Copies: %d, Shelf Location: %s\n",
                                books[i].book_id, books[i].title, books[i].author_id, books[i].publisher_id,
                                books[i].isbn, books[i].genre, books[i].year_published,
                                books[i].copies_available, books[i].shelf_location);
                        strncat(message, temp, sizeof(message) - strlen(message) - 1);
                    }
                    free(books);
                }
                MessageBox(hwndBM, message, "View All Books", MB_OK);
            } else if (LOWORD(wp) == 103) { // Update Book
                char buffer[MAX_STRING];
                GetWindowText(hEditBookId, buffer, MAX_STRING);
                int bookId = atoi(buffer);
                int count;
                Book* books = loadBooks(&count);
                int found = 0;
                for(int i = 0; i < count; i++) {
                    if(books[i].book_id == bookId) {
                        GetWindowText(hEditTitle, books[i].title, MAX_STRING);
                        GetWindowText(hEditAuthorId, buffer, MAX_STRING); books[i].author_id = atoi(buffer);
                        GetWindowText(hEditPublisherId, buffer, MAX_STRING); books[i].publisher_id = atoi(buffer);
                        GetWindowText(hEditIsbn, books[i].isbn, MAX_STRING);
                        GetWindowText(hEditGenre, books[i].genre, MAX_STRING);
                        GetWindowText(hEditYear, buffer, MAX_STRING); books[i].year_published = atoi(buffer);
                        GetWindowText(hEditCopies, buffer, MAX_STRING); books[i].copies_available = atoi(buffer);
                        GetWindowText(hEditShelf, books[i].shelf_location, MAX_STRING);
                        FILE* file = fopen("books.dat", "wb");
                        if (file) {
                            fwrite(books, sizeof(Book), count, file);
                            fclose(file);
                            MessageBox(hwndBM, "Book updated successfully!", "Success", MB_OK);
                        } else {
                            MessageBox(hwndBM, "Failed to update book!", "Error", MB_OK);
                        }
                        found = 1;
                        break;
                    }
                }
                if (!found) {
                    MessageBox(hwndBM, "Book not found!", "Error", MB_OK);
                }
                free(books);
            }
            break;

        case WM_CTLCOLORSTATIC:
        case WM_CTLCOLORBTN:
        case WM_CTLCOLOREDIT: {
            HDC hdc = (HDC)wp;
            if (isDarkTheme) {
                SetTextColor(hdc, RGB(255, 255, 255));
                SetBkColor(hdc, RGB(30, 30, 30));
                return (LRESULT)hBrushDark;
            } else {
                SetTextColor(hdc, RGB(0, 0, 0));
                SetBkColor(hdc, RGB(240, 240, 240));
                return (LRESULT)hBrushLight;
            }
        }

        case WM_ERASEBKGND:
            return (LRESULT)1;

        case WM_PAINT: {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwndBM, &ps);
            RECT rect;
            GetClientRect(hwndBM, &rect);
            FillRect(hdc, &rect, isDarkTheme ? hBrushDark : hBrushLight);
            EndPaint(hwndBM, &ps);
            break;
        }

        case WM_CLOSE:
            DestroyWindow(hwndBM);
            break;

        default:
            return DefWindowProc(hwndBM, msg, wp, lp);
    }
    return 0;
}

void showAuthorManagementGUI() {
    HINSTANCE hInst = GetModuleHandle(NULL);
    WNDCLASSEX wc = {0};
    wc.cbSize = sizeof(WNDCLASSEX);
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = AuthorManagementWindowProcedure;
    wc.hInstance = hInst;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = NULL;
    wc.lpszClassName = "AuthorManagementWindow";
    
    if(!RegisterClassEx(&wc)) return;
    
    CreateWindowEx(
        0, "AuthorManagementWindow", "Author Management",
        WS_OVERLAPPEDWINDOW | WS_VISIBLE, CW_USEDEFAULT, CW_USEDEFAULT,
        600, 400, hwnd, NULL, hInst, NULL
    );
}

LRESULT CALLBACK AuthorManagementWindowProcedure(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp) {
    static HWND hEditAuthorId, hEditName, hEditBio;
    switch(msg) {
        case WM_CREATE:
            int labelWidth = 100;
            int editWidth = 200;
            int lineHeight = 25;
            int startY = 20;
            int spacingY = 10;
            int startX_label = 20;
            int startX_edit = startX_label + labelWidth + 10;

            CreateWindow("STATIC", "Author ID:", WS_VISIBLE | WS_CHILD, startX_label, startY, labelWidth, 20, hwnd, NULL, GetModuleHandle(NULL), NULL);
            hEditAuthorId = CreateWindow("EDIT", "", WS_VISIBLE | WS_CHILD | WS_BORDER | ES_AUTOHSCROLL, startX_edit, startY, 100, 20, hwnd, NULL, GetModuleHandle(NULL), NULL);

            startY += lineHeight + spacingY;
            CreateWindow("STATIC", "Name:", WS_VISIBLE | WS_CHILD, startX_label, startY, labelWidth, 20, hwnd, NULL, GetModuleHandle(NULL), NULL);
            hEditName = CreateWindow("EDIT", "", WS_VISIBLE | WS_CHILD | WS_BORDER | ES_AUTOHSCROLL, startX_edit, startY, editWidth, 20, hwnd, NULL, GetModuleHandle(NULL), NULL);

            startY += lineHeight + spacingY;
            CreateWindow("STATIC", "Bio:", WS_VISIBLE | WS_CHILD, startX_label, startY, labelWidth, 20, hwnd, NULL, GetModuleHandle(NULL), NULL);
            hEditBio = CreateWindow("EDIT", "", WS_VISIBLE | WS_CHILD | WS_BORDER | ES_AUTOHSCROLL | ES_MULTILINE | ES_WANTRETURN, startX_edit, startY, editWidth, 60, hwnd, NULL, GetModuleHandle(NULL), NULL);

            startY += 60 + spacingY + 10;
            int buttonWidth = 120;
            int buttonSpacing = 10;
            int buttonX = startX_label;

            CreateWindow("BUTTON", "Add Author", WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON | BS_FLAT, buttonX, startY, buttonWidth, 30, hwnd, (HMENU)101, GetModuleHandle(NULL), NULL);
            buttonX += buttonWidth + buttonSpacing;
            CreateWindow("BUTTON", "View All Authors", WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON | BS_FLAT, buttonX, startY, buttonWidth + 30, 30, hwnd, (HMENU)102, GetModuleHandle(NULL), NULL);
            buttonX += buttonWidth + 30 + buttonSpacing;
            CreateWindow("BUTTON", "Update Author", WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON | BS_FLAT, buttonX, startY, buttonWidth, 30, hwnd, (HMENU)103, GetModuleHandle(NULL), NULL);

            SendMessage(hEditAuthorId, WM_SETFONT, (WPARAM)hFont, TRUE);
            SendMessage(hEditName, WM_SETFONT, (WPARAM)hFont, TRUE);
            SendMessage(hEditBio, WM_SETFONT, (WPARAM)hFont, TRUE);
            break;

        case WM_COMMAND:
            if (LOWORD(wp) == 101) {
                Author newAuthor;
                char buffer[MAX_STRING];
                GetWindowText(hEditAuthorId, buffer, MAX_STRING); newAuthor.author_id = atoi(buffer);
                GetWindowText(hEditName, newAuthor.name, MAX_STRING);
                GetWindowText(hEditBio, newAuthor.bio, MAX_TEXT);
                saveAuthor(newAuthor);
                MessageBox(hwnd, "Author added successfully!", "Success", MB_OK);
            } else if (LOWORD(wp) == 102) {
                int count;
                Author* authors = loadAuthors(&count);
                char message[2000];
                strcpy(message, "Authors:\n");
                if (count == 0) strcat(message, "No authors found.\n");
                else {
                    char temp[500];
                    for(int i = 0; i < count; i++) {
                        sprintf(temp, "Author ID: %d, Name: %s, Bio: %s\n",
                                authors[i].author_id, authors[i].name, authors[i].bio);
                        strncat(message, temp, sizeof(message) - strlen(message) - 1);
                    }
                    free(authors);
                }
                MessageBox(hwnd, message, "View All Authors", MB_OK);
            } else if (LOWORD(wp) == 103) {
                char buffer[MAX_STRING];
                GetWindowText(hEditAuthorId, buffer, MAX_STRING);
                int authorId = atoi(buffer);
                int count;
                Author* authors = loadAuthors(&count);
                int found = 0;
                for(int i = 0; i < count; i++) {
                    if(authors[i].author_id == authorId) {
                        GetWindowText(hEditName, authors[i].name, MAX_STRING);
                        GetWindowText(hEditBio, authors[i].bio, MAX_TEXT);
                        FILE* file = fopen("authors.dat", "wb");
                        if (file) {
                            fwrite(authors, sizeof(Author), count, file);
                            fclose(file);
                            MessageBox(hwnd, "Author updated successfully!", "Success", MB_OK);
                        } else {
                            MessageBox(hwnd, "Failed to update author!", "Error", MB_OK);
                        }
                        found = 1;
                        break;
                    }
                }
                if (!found) {
                    MessageBox(hwnd, "Author not found!", "Error", MB_OK);
                }
                free(authors);
            }
            break;

        case WM_CTLCOLORSTATIC:
        case WM_CTLCOLORBTN:
        case WM_CTLCOLOREDIT: {
            HDC hdc = (HDC)wp;
            if (isDarkTheme) {
                SetTextColor(hdc, RGB(255, 255, 255));
                SetBkColor(hdc, RGB(30, 30, 30));
                return (LRESULT)hBrushDark;
            } else {
                SetTextColor(hdc, RGB(0, 0, 0));
                SetBkColor(hdc, RGB(240, 240, 240));
                return (LRESULT)hBrushLight;
            }
        }

        case WM_ERASEBKGND:
            return (LRESULT)1;

        case WM_PAINT: {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);
            RECT rect;
            GetClientRect(hwnd, &rect);
            FillRect(hdc, &rect, isDarkTheme ? hBrushDark : hBrushLight);
            EndPaint(hwnd, &ps);
            break;
        }

        case WM_CLOSE:
            DestroyWindow(hwnd);
            break;

        default:
            return DefWindowProc(hwnd, msg, wp, lp);
    }
    return 0;
}

void showPublisherManagementGUI() {
    HINSTANCE hInst = GetModuleHandle(NULL);
    WNDCLASSEX wc = {0};
    wc.cbSize = sizeof(WNDCLASSEX);
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = PublisherManagementWindowProcedure;
    wc.hInstance = hInst;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = NULL;
    wc.lpszClassName = "PublisherManagementWindow";
    
    if(!RegisterClassEx(&wc)) return;
    
    CreateWindowEx(
        0, "PublisherManagementWindow", "Publisher Management",
        WS_OVERLAPPEDWINDOW | WS_VISIBLE, CW_USEDEFAULT, CW_USEDEFAULT,
        600, 400, hwnd, NULL, hInst, NULL
    );
}

LRESULT CALLBACK PublisherManagementWindowProcedure(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp) {
    static HWND hEditPublisherId, hEditName, hEditAddress, hEditContact;
    switch(msg) {
        case WM_CREATE:
            // Define spacing and control dimensions
            int labelWidth = 100;
            int editWidth = 200;
            int lineHeight = 25;
            int startY = 20;
            int spacingY = 10;
            int startX_label = 20;
            int startX_edit = startX_label + labelWidth + 10;

            // Publisher ID
            CreateWindow("STATIC", "Publisher ID:", WS_VISIBLE | WS_CHILD, startX_label, startY, labelWidth, 20, hwnd, NULL, GetModuleHandle(NULL), NULL);
            hEditPublisherId = CreateWindow("EDIT", "", WS_VISIBLE | WS_CHILD | WS_BORDER | ES_AUTOHSCROLL, startX_edit, startY, 100, 20, hwnd, NULL, GetModuleHandle(NULL), NULL);

            // Name
            startY += lineHeight + spacingY;
            CreateWindow("STATIC", "Name:", WS_VISIBLE | WS_CHILD, startX_label, startY, labelWidth, 20, hwnd, NULL, GetModuleHandle(NULL), NULL);
            hEditName = CreateWindow("EDIT", "", WS_VISIBLE | WS_CHILD | WS_BORDER | ES_AUTOHSCROLL, startX_edit, startY, editWidth, 20, hwnd, NULL, GetModuleHandle(NULL), NULL);

            // Address
            startY += lineHeight + spacingY;
            CreateWindow("STATIC", "Address:", WS_VISIBLE | WS_CHILD, startX_label, startY, labelWidth, 20, hwnd, NULL, GetModuleHandle(NULL), NULL);
            hEditAddress = CreateWindow("EDIT", "", WS_VISIBLE | WS_CHILD | WS_BORDER | ES_AUTOHSCROLL, startX_edit, startY, editWidth, 20, hwnd, NULL, GetModuleHandle(NULL), NULL);

            // Contact Info
            startY += lineHeight + spacingY;
            CreateWindow("STATIC", "Contact Info:", WS_VISIBLE | WS_CHILD, startX_label, startY, labelWidth, 20, hwnd, NULL, GetModuleHandle(NULL), NULL);
            hEditContact = CreateWindow("EDIT", "", WS_VISIBLE | WS_CHILD | WS_BORDER | ES_AUTOHSCROLL, startX_edit, startY, editWidth, 20, hwnd, NULL, GetModuleHandle(NULL), NULL);

            // Buttons
            startY += lineHeight + spacingY + 10; // Add extra space before buttons
            int buttonWidth = 120;
            int buttonSpacing = 10;
            int buttonX = startX_label;

            CreateWindow("BUTTON", "Add Publisher", WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON | BS_FLAT, buttonX, startY, buttonWidth, 30, hwnd, (HMENU)101, GetModuleHandle(NULL), NULL);
            buttonX += buttonWidth + buttonSpacing;
            CreateWindow("BUTTON", "View All Publishers", WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON | BS_FLAT, buttonX, startY, buttonWidth + 30, 30, hwnd, (HMENU)102, GetModuleHandle(NULL), NULL);
            buttonX += buttonWidth + 30 + buttonSpacing;
            CreateWindow("BUTTON", "Update Publisher", WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON | BS_FLAT, buttonX, startY, buttonWidth, 30, hwnd, (HMENU)103, GetModuleHandle(NULL), NULL);

            SendMessage(hEditPublisherId, WM_SETFONT, (WPARAM)hFont, TRUE);
            SendMessage(hEditName, WM_SETFONT, (WPARAM)hFont, TRUE);
            SendMessage(hEditAddress, WM_SETFONT, (WPARAM)hFont, TRUE);
            SendMessage(hEditContact, WM_SETFONT, (WPARAM)hFont, TRUE);
            break;
        case WM_COMMAND:
            if (LOWORD(wp) == 101) {
                Publisher newPublisher;
                char buffer[MAX_STRING];
                GetWindowText(hEditPublisherId, buffer, MAX_STRING); newPublisher.publisher_id = atoi(buffer);
                GetWindowText(hEditName, newPublisher.name, MAX_STRING);
                GetWindowText(hEditAddress, newPublisher.address, MAX_STRING);
                GetWindowText(hEditContact, newPublisher.contact_info, MAX_STRING);
                savePublisher(newPublisher);
                MessageBox(hwnd, "Publisher added successfully!", "Success", MB_OK);
            } else if (LOWORD(wp) == 102) {
                int count;
                Publisher* publishers = loadPublishers(&count);
                char message[2000];
                strcpy(message, "Publishers:\n");
                if (count == 0) strcat(message, "No publishers found.\n");
                else {
                    char temp[500];
                    for(int i = 0; i < count; i++) {
                        sprintf(temp, "Publisher ID: %d, Name: %s, Address: %s, Contact Info: %s\n",
                                publishers[i].publisher_id, publishers[i].name, publishers[i].address, publishers[i].contact_info);
                        strncat(message, temp, sizeof(message) - strlen(message) - 1);
                    }
                    free(publishers);
                }
                MessageBox(hwnd, message, "View All Publishers", MB_OK);
            } else if (LOWORD(wp) == 103) {
                char buffer[MAX_STRING];
                GetWindowText(hEditPublisherId, buffer, MAX_STRING);
                int publisherId = atoi(buffer);
                int count;
                Publisher* publishers = loadPublishers(&count);
                int found = 0;
                for(int i = 0; i < count; i++) {
                    if(publishers[i].publisher_id == publisherId) {
                        GetWindowText(hEditName, publishers[i].name, MAX_STRING);
                        GetWindowText(hEditAddress, publishers[i].address, MAX_STRING);
                        GetWindowText(hEditContact, publishers[i].contact_info, MAX_STRING);
                        FILE* file = fopen("publishers.dat", "wb");
                        if (file) {
                            fwrite(publishers, sizeof(Publisher), count, file);
                            fclose(file);
                            MessageBox(hwnd, "Publisher updated successfully!", "Success", MB_OK);
                        } else {
                            MessageBox(hwnd, "Failed to update publisher!", "Error", MB_OK);
                        }
                        found = 1;
                        break;
                    }
                }
                if (!found) {
                    MessageBox(hwnd, "Publisher not found!", "Error", MB_OK);
                }
                free(publishers);
            }
            break;

        case WM_CTLCOLORSTATIC:
        case WM_CTLCOLORBTN:
        case WM_CTLCOLOREDIT: {
            HDC hdc = (HDC)wp;
            if (isDarkTheme) {
                SetTextColor(hdc, RGB(255, 255, 255));
                SetBkColor(hdc, RGB(30, 30, 30));
                return (LRESULT)hBrushDark;
            } else {
                SetTextColor(hdc, RGB(0, 0, 0));
                SetBkColor(hdc, RGB(240, 240, 240));
                return (LRESULT)hBrushLight;
            }
        }

        case WM_ERASEBKGND:
            return (LRESULT)1;

        case WM_PAINT: {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);
            RECT rect;
            GetClientRect(hwnd, &rect);
            FillRect(hdc, &rect, isDarkTheme ? hBrushDark : hBrushLight);
            EndPaint(hwnd, &ps);
            break;
        }

        case WM_CLOSE:
            DestroyWindow(hwnd);
            break;
        default:
            return DefWindowProc(hwnd, msg, wp, lp);
    }
    return 0;
}

void showMemberManagementGUI() {
    HINSTANCE hInst = GetModuleHandle(NULL);
    WNDCLASSEX wc = {0};
    wc.cbSize = sizeof(WNDCLASSEX);
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = MemberManagementWindowProcedure;
    wc.hInstance = hInst;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = NULL;
    wc.lpszClassName = "MemberManagementWindow";
    
    if(!RegisterClassEx(&wc)) return;
    
    CreateWindowEx(
        0, "MemberManagementWindow", "Member Management",
        WS_OVERLAPPEDWINDOW | WS_VISIBLE, CW_USEDEFAULT, CW_USEDEFAULT,
        600, 400, hwnd, NULL, hInst, NULL
    );
}

LRESULT CALLBACK MemberManagementWindowProcedure(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp) {
    static HWND hEditMemberId, hEditName, hEditAddress, hEditPhone, hEditEmail, hEditDateJoined, hEditStatus;
    switch(msg) {
        case WM_CREATE:
            int labelWidth = 120;
            int editWidth = 200;
            int lineHeight = 25;
            int startY = 20;
            int spacingY = 10;
            int startX_label = 20;
            int startX_edit = startX_label + labelWidth + 10;

            CreateWindow("STATIC", "Member ID:", WS_VISIBLE | WS_CHILD, startX_label, startY, labelWidth, 20, hwnd, NULL, GetModuleHandle(NULL), NULL);
            hEditMemberId = CreateWindow("EDIT", "", WS_VISIBLE | WS_CHILD | WS_BORDER | ES_AUTOHSCROLL, startX_edit, startY, 100, 20, hwnd, NULL, GetModuleHandle(NULL), NULL);

            startY += lineHeight + spacingY;
            CreateWindow("STATIC", "Name:", WS_VISIBLE | WS_CHILD, startX_label, startY, labelWidth, 20, hwnd, NULL, GetModuleHandle(NULL), NULL);
            hEditName = CreateWindow("EDIT", "", WS_VISIBLE | WS_CHILD | WS_BORDER | ES_AUTOHSCROLL, startX_edit, startY, editWidth, 20, hwnd, NULL, GetModuleHandle(NULL), NULL);

            startY += lineHeight + spacingY;
            CreateWindow("STATIC", "Address:", WS_VISIBLE | WS_CHILD, startX_label, startY, labelWidth, 20, hwnd, NULL, GetModuleHandle(NULL), NULL);
            hEditAddress = CreateWindow("EDIT", "", WS_VISIBLE | WS_CHILD | WS_BORDER | ES_AUTOHSCROLL, startX_edit, startY, editWidth, 20, hwnd, NULL, GetModuleHandle(NULL), NULL);

            startY += lineHeight + spacingY;
            CreateWindow("STATIC", "Phone:", WS_VISIBLE | WS_CHILD, startX_label, startY, labelWidth, 20, hwnd, NULL, GetModuleHandle(NULL), NULL);
            hEditPhone = CreateWindow("EDIT", "", WS_VISIBLE | WS_CHILD | WS_BORDER | ES_AUTOHSCROLL, startX_edit, startY, 150, 20, hwnd, NULL, GetModuleHandle(NULL), NULL);

            startY += lineHeight + spacingY;
            CreateWindow("STATIC", "Email:", WS_VISIBLE | WS_CHILD, startX_label, startY, labelWidth, 20, hwnd, NULL, GetModuleHandle(NULL), NULL);
            hEditEmail = CreateWindow("EDIT", "", WS_VISIBLE | WS_CHILD | WS_BORDER | ES_AUTOHSCROLL, startX_edit, startY, editWidth, 20, hwnd, NULL, GetModuleHandle(NULL), NULL);

            startY += lineHeight + spacingY;
            CreateWindow("STATIC", "Date Joined:", WS_VISIBLE | WS_CHILD, startX_label, startY, labelWidth, 20, hwnd, NULL, GetModuleHandle(NULL), NULL);
            hEditDateJoined = CreateWindow("EDIT", "", WS_VISIBLE | WS_CHILD | WS_BORDER | ES_AUTOHSCROLL, startX_edit, startY, 100, 20, hwnd, NULL, GetModuleHandle(NULL), NULL);

            startY += lineHeight + spacingY;
            CreateWindow("STATIC", "Membership Status:", WS_VISIBLE | WS_CHILD, startX_label, startY, labelWidth, 20, hwnd, NULL, GetModuleHandle(NULL), NULL);
            hEditStatus = CreateWindow("EDIT", "", WS_VISIBLE | WS_CHILD | WS_BORDER | ES_AUTOHSCROLL, startX_edit, startY, 150, 20, hwnd, NULL, GetModuleHandle(NULL), NULL);

            startY += lineHeight + spacingY + 10;
            int buttonWidth = 120;
            int buttonSpacing = 10;
            int buttonX = startX_label;

            CreateWindow("BUTTON", "Add Member", WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON | BS_FLAT, buttonX, startY, buttonWidth, 30, hwnd, (HMENU)101, GetModuleHandle(NULL), NULL);
            buttonX += buttonWidth + buttonSpacing;
            CreateWindow("BUTTON", "View All Members", WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON | BS_FLAT, buttonX, startY, buttonWidth + 30, 30, hwnd, (HMENU)102, GetModuleHandle(NULL), NULL);
            buttonX += buttonWidth + 30 + buttonSpacing;
            CreateWindow("BUTTON", "Update Member", WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON | BS_FLAT, buttonX, startY, buttonWidth, 30, hwnd, (HMENU)103, GetModuleHandle(NULL), NULL);

            SendMessage(hEditMemberId, WM_SETFONT, (WPARAM)hFont, TRUE);
            SendMessage(hEditName, WM_SETFONT, (WPARAM)hFont, TRUE);
            SendMessage(hEditAddress, WM_SETFONT, (WPARAM)hFont, TRUE);
            SendMessage(hEditPhone, WM_SETFONT, (WPARAM)hFont, TRUE);
            SendMessage(hEditEmail, WM_SETFONT, (WPARAM)hFont, TRUE);
            SendMessage(hEditDateJoined, WM_SETFONT, (WPARAM)hFont, TRUE);
            SendMessage(hEditStatus, WM_SETFONT, (WPARAM)hFont, TRUE);
            break;

        case WM_COMMAND:
            if (LOWORD(wp) == 101) {
                Member newMember;
                char buffer[MAX_STRING];
                GetWindowText(hEditMemberId, buffer, MAX_STRING); newMember.member_id = atoi(buffer);
                GetWindowText(hEditName, newMember.name, MAX_STRING);
                GetWindowText(hEditAddress, newMember.address, MAX_STRING);
                GetWindowText(hEditPhone, newMember.phone, MAX_STRING);
                GetWindowText(hEditEmail, newMember.email, MAX_STRING);
                GetWindowText(hEditDateJoined, newMember.date_joined, 11);
                GetWindowText(hEditStatus, newMember.membership_status, MAX_STRING);
                saveMember(newMember);
                MessageBox(hwnd, "Member added successfully!", "Success", MB_OK);
            } else if (LOWORD(wp) == 102) {
                int count;
                Member* members = loadMembers(&count);
                char message[2000];
                strcpy(message, "Members:\n");
                if (count == 0) strcat(message, "No members found.\n");
                else {
                    char temp[500];
                    for(int i = 0; i < count; i++) {
                        sprintf(temp, "Member ID: %d, Name: %s, Address: %s, Phone: %s, Email: %s, Date Joined: %s, Membership Status: %s\n",
                                members[i].member_id, members[i].name, members[i].address, members[i].phone,
                                members[i].email, members[i].date_joined, members[i].membership_status);
                        strncat(message, temp, sizeof(message) - strlen(message) - 1);
                    }
                    free(members);
                }
                MessageBox(hwnd, message, "View All Members", MB_OK);
            } else if (LOWORD(wp) == 103) {
                char buffer[MAX_STRING];
                GetWindowText(hEditMemberId, buffer, MAX_STRING);
                int memberId = atoi(buffer);
                int count;
                Member* members = loadMembers(&count);
                int found = 0;
                for(int i = 0; i < count; i++) {
                    if(members[i].member_id == memberId) {
                        GetWindowText(hEditName, members[i].name, MAX_STRING);
                        GetWindowText(hEditAddress, members[i].address, MAX_STRING);
                        GetWindowText(hEditPhone, members[i].phone, MAX_STRING);
                        GetWindowText(hEditEmail, members[i].email, MAX_STRING);
                        GetWindowText(hEditDateJoined, members[i].date_joined, 11);
                        GetWindowText(hEditStatus, members[i].membership_status, MAX_STRING);
                        FILE* file = fopen("members.dat", "wb");
                        if (file) {
                            fwrite(members, sizeof(Member), count, file);
                            fclose(file);
                            MessageBox(hwnd, "Member updated successfully!", "Success", MB_OK);
                        } else {
                            MessageBox(hwnd, "Failed to update member!", "Error", MB_OK);
                        }
                        found = 1;
                        break;
                    }
                }
                if (!found) {
                    MessageBox(hwnd, "Member not found!", "Error", MB_OK);
                }
                free(members);
            }
            break;

        case WM_CTLCOLORSTATIC:
        case WM_CTLCOLORBTN:
        case WM_CTLCOLOREDIT: {
            HDC hdc = (HDC)wp;
            if (isDarkTheme) {
                SetTextColor(hdc, RGB(255, 255, 255));
                SetBkColor(hdc, RGB(30, 30, 30));
                return (LRESULT)hBrushDark;
            } else {
                SetTextColor(hdc, RGB(0, 0, 0));
                SetBkColor(hdc, RGB(240, 240, 240));
                return (LRESULT)hBrushLight;
            }
        }

        case WM_ERASEBKGND:
            return (LRESULT)1;

        case WM_PAINT: {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);
            RECT rect;
            GetClientRect(hwnd, &rect);
            FillRect(hdc, &rect, isDarkTheme ? hBrushDark : hBrushLight);
            EndPaint(hwnd, &ps);
            break;
        }

        case WM_CLOSE:
            DestroyWindow(hwnd);
            break;

        default:
            return DefWindowProc(hwnd, msg, wp, lp);
    }
    return 0;
}

void showStaffManagementGUI() {
    HINSTANCE hInst = GetModuleHandle(NULL);
    WNDCLASSEX wc = {0};
    wc.cbSize = sizeof(WNDCLASSEX);
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = StaffManagementWindowProcedure;
    wc.hInstance = hInst;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = NULL;
    wc.lpszClassName = "StaffManagementWindow";
    
    if(!RegisterClassEx(&wc)) return;
    
    CreateWindowEx(
        0, "StaffManagementWindow", "Staff Management",
        WS_OVERLAPPEDWINDOW | WS_VISIBLE, CW_USEDEFAULT, CW_USEDEFAULT,
        600, 400, hwnd, NULL, hInst, NULL
    );
}

LRESULT CALLBACK StaffManagementWindowProcedure(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp) {
    static HWND hEditStaffId, hEditName, hEditRole, hEditEmail, hEditPhone;
    switch(msg) {
        case WM_CREATE:
            int labelWidth = 100;
            int editWidth = 200;
            int lineHeight = 25;
            int startY = 20;
            int spacingY = 10;
            int startX_label = 20;
            int startX_edit = startX_label + labelWidth + 10;

            CreateWindow("STATIC", "Staff ID:", WS_VISIBLE | WS_CHILD, startX_label, startY, labelWidth, 20, hwnd, NULL, GetModuleHandle(NULL), NULL);
            hEditStaffId = CreateWindow("EDIT", "", WS_VISIBLE | WS_CHILD | WS_BORDER | ES_AUTOHSCROLL, startX_edit, startY, 100, 20, hwnd, NULL, GetModuleHandle(NULL), NULL);

            startY += lineHeight + spacingY;
            CreateWindow("STATIC", "Name:", WS_VISIBLE | WS_CHILD, startX_label, startY, labelWidth, 20, hwnd, NULL, GetModuleHandle(NULL), NULL);
            hEditName = CreateWindow("EDIT", "", WS_VISIBLE | WS_CHILD | WS_BORDER | ES_AUTOHSCROLL, startX_edit, startY, editWidth, 20, hwnd, NULL, GetModuleHandle(NULL), NULL);

            startY += lineHeight + spacingY;
            CreateWindow("STATIC", "Role:", WS_VISIBLE | WS_CHILD, startX_label, startY, labelWidth, 20, hwnd, NULL, GetModuleHandle(NULL), NULL);
            hEditRole = CreateWindow("EDIT", "", WS_VISIBLE | WS_CHILD | WS_BORDER | ES_AUTOHSCROLL, startX_edit, startY, 100, 20, hwnd, NULL, GetModuleHandle(NULL), NULL);

            startY += lineHeight + spacingY;
            CreateWindow("STATIC", "Email:", WS_VISIBLE | WS_CHILD, startX_label, startY, labelWidth, 20, hwnd, NULL, GetModuleHandle(NULL), NULL);
            hEditEmail = CreateWindow("EDIT", "", WS_VISIBLE | WS_CHILD | WS_BORDER | ES_AUTOHSCROLL, startX_edit, startY, editWidth, 20, hwnd, NULL, GetModuleHandle(NULL), NULL);

            startY += lineHeight + spacingY;
            CreateWindow("STATIC", "Phone:", WS_VISIBLE | WS_CHILD, startX_label, startY, labelWidth, 20, hwnd, NULL, GetModuleHandle(NULL), NULL);
            hEditPhone = CreateWindow("EDIT", "", WS_VISIBLE | WS_CHILD | WS_BORDER | ES_AUTOHSCROLL, startX_edit, startY, 100, 20, hwnd, NULL, GetModuleHandle(NULL), NULL);

            startY += lineHeight + spacingY + 10;
            int buttonWidth = 120;
            int buttonSpacing = 10;
            int buttonX = startX_label;

            CreateWindow("BUTTON", "Add Staff", WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON | BS_FLAT, buttonX, startY, buttonWidth, 30, hwnd, (HMENU)101, GetModuleHandle(NULL), NULL);
            buttonX += buttonWidth + buttonSpacing;
            CreateWindow("BUTTON", "View All Staff", WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON | BS_FLAT, buttonX, startY, buttonWidth + 30, 30, hwnd, (HMENU)102, GetModuleHandle(NULL), NULL);
            buttonX += buttonWidth + 30 + buttonSpacing;
            CreateWindow("BUTTON", "Update Staff", WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON | BS_FLAT, buttonX, startY, buttonWidth, 30, hwnd, (HMENU)103, GetModuleHandle(NULL), NULL);

            SendMessage(hEditStaffId, WM_SETFONT, (WPARAM)hFont, TRUE);
            SendMessage(hEditName, WM_SETFONT, (WPARAM)hFont, TRUE);
            SendMessage(hEditRole, WM_SETFONT, (WPARAM)hFont, TRUE);
            SendMessage(hEditEmail, WM_SETFONT, (WPARAM)hFont, TRUE);
            SendMessage(hEditPhone, WM_SETFONT, (WPARAM)hFont, TRUE);
            break;

        case WM_COMMAND:
            if (LOWORD(wp) == 101) {
                Staff newStaff;
                char buffer[MAX_STRING];
                GetWindowText(hEditStaffId, buffer, MAX_STRING); newStaff.staff_id = atoi(buffer);
                GetWindowText(hEditName, newStaff.name, MAX_STRING);
                GetWindowText(hEditRole, newStaff.role, MAX_STRING);
                GetWindowText(hEditEmail, newStaff.email, MAX_STRING);
                GetWindowText(hEditPhone, newStaff.phone, MAX_STRING);
                saveStaff(newStaff);
                MessageBox(hwnd, "Staff added successfully!", "Success", MB_OK);
            } else if (LOWORD(wp) == 102) {
                int count;
                Staff* staff = loadStaff(&count);
                char message[2000];
                strcpy(message, "Staff:\n");
                if (count == 0) strcat(message, "No staff found.\n");
                else {
                    char temp[500];
                    for(int i = 0; i < count; i++) {
                        sprintf(temp, "Staff ID: %d, Name: %s, Role: %s, Email: %s, Phone: %s\n",
                                staff[i].staff_id, staff[i].name, staff[i].role, staff[i].email, staff[i].phone);
                        strncat(message, temp, sizeof(message) - strlen(message) - 1);
                    }
                    free(staff);
                }
                MessageBox(hwnd, message, "View All Staff", MB_OK);
            } else if (LOWORD(wp) == 103) {
                char buffer[MAX_STRING];
                GetWindowText(hEditStaffId, buffer, MAX_STRING);
                int staffId = atoi(buffer);
                int count;
                Staff* staff = loadStaff(&count);
                int found = 0;
                for(int i = 0; i < count; i++) {
                    if(staff[i].staff_id == staffId) {
                        GetWindowText(hEditName, staff[i].name, MAX_STRING);
                        GetWindowText(hEditRole, staff[i].role, MAX_STRING);
                        GetWindowText(hEditEmail, staff[i].email, MAX_STRING);
                        GetWindowText(hEditPhone, staff[i].phone, MAX_STRING);
                        FILE* file = fopen("staff.dat", "wb");
                        if (file) {
                            fwrite(staff, sizeof(Staff), count, file);
                            fclose(file);
                            MessageBox(hwnd, "Staff updated successfully!", "Success", MB_OK);
                        } else {
                            MessageBox(hwnd, "Failed to update staff!", "Error", MB_OK);
                        }
                        found = 1;
                        break;
                    }
                }
                if (!found) {
                    MessageBox(hwnd, "Staff not found!", "Error", MB_OK);
                }
                free(staff);
            }
            break;

        case WM_CTLCOLORSTATIC:
        case WM_CTLCOLORBTN:
        case WM_CTLCOLOREDIT: {
            HDC hdc = (HDC)wp;
            if (isDarkTheme) {
                SetTextColor(hdc, RGB(255, 255, 255));
                SetBkColor(hdc, RGB(30, 30, 30));
                return (LRESULT)hBrushDark;
            } else {
                SetTextColor(hdc, RGB(0, 0, 0));
                SetBkColor(hdc, RGB(240, 240, 240));
                return (LRESULT)hBrushLight;
            }
        }

        case WM_ERASEBKGND:
            return (LRESULT)1;

        case WM_PAINT: {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);
            RECT rect;
            GetClientRect(hwnd, &rect);
            FillRect(hdc, &rect, isDarkTheme ? hBrushDark : hBrushLight);
            EndPaint(hwnd, &ps);
            break;
        }

        case WM_CLOSE:
            DestroyWindow(hwnd);
            break;

        default:
            return DefWindowProc(hwnd, msg, wp, lp);
    }
    return 0;
}

void showBorrowingManagementGUI() {
    HINSTANCE hInst = GetModuleHandle(NULL);
    WNDCLASSEX wc = {0};
    wc.cbSize = sizeof(WNDCLASSEX);
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = BorrowingManagementWindowProcedure;
    wc.hInstance = hInst;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = NULL;
    wc.lpszClassName = "BorrowingManagementWindow";
    
    if(!RegisterClassEx(&wc)) return;
    
    CreateWindowEx(
        0, "BorrowingManagementWindow", "Borrowing Management",
        WS_OVERLAPPEDWINDOW | WS_VISIBLE, CW_USEDEFAULT, CW_USEDEFAULT,
        600, 400, hwnd, NULL, hInst, NULL
    );
}

LRESULT CALLBACK BorrowingManagementWindowProcedure(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp) {
    static HWND hEditBorrowingId, hEditBookId, hEditMemberId, hEditBorrowDate, hEditDueDate, hEditReturnDate, hEditStaffId;
    switch(msg) {
        case WM_CREATE:
            int labelWidth = 100;
            int editWidth = 150;
            int lineHeight = 25;
            int startY = 20;
            int spacingY = 10;
            int startX_label = 20;
            int startX_edit = startX_label + labelWidth + 10;

            CreateWindow("STATIC", "Borrowing ID:", WS_VISIBLE | WS_CHILD, startX_label, startY, labelWidth, 20, hwnd, NULL, GetModuleHandle(NULL), NULL);
            hEditBorrowingId = CreateWindow("EDIT", "", WS_VISIBLE | WS_CHILD | WS_BORDER | ES_AUTOHSCROLL, startX_edit, startY, 100, 20, hwnd, NULL, GetModuleHandle(NULL), NULL);

            startY += lineHeight + spacingY;
            CreateWindow("STATIC", "Book ID:", WS_VISIBLE | WS_CHILD, startX_label, startY, labelWidth, 20, hwnd, NULL, GetModuleHandle(NULL), NULL);
            hEditBookId = CreateWindow("EDIT", "", WS_VISIBLE | WS_CHILD | WS_BORDER | ES_AUTOHSCROLL, startX_edit, startY, 100, 20, hwnd, NULL, GetModuleHandle(NULL), NULL);

            startY += lineHeight + spacingY;
            CreateWindow("STATIC", "Member ID:", WS_VISIBLE | WS_CHILD, startX_label, startY, labelWidth, 20, hwnd, NULL, GetModuleHandle(NULL), NULL);
            hEditMemberId = CreateWindow("EDIT", "", WS_VISIBLE | WS_CHILD | WS_BORDER | ES_AUTOHSCROLL, startX_edit, startY, 100, 20, hwnd, NULL, GetModuleHandle(NULL), NULL);

            startY += lineHeight + spacingY;
            CreateWindow("STATIC", "Borrow Date:", WS_VISIBLE | WS_CHILD, startX_label, startY, labelWidth, 20, hwnd, NULL, GetModuleHandle(NULL), NULL);
            hEditBorrowDate = CreateWindow("EDIT", "", WS_VISIBLE | WS_CHILD | WS_BORDER | ES_AUTOHSCROLL, startX_edit, startY, editWidth, 20, hwnd, NULL, GetModuleHandle(NULL), NULL);

            startY += lineHeight + spacingY;
            CreateWindow("STATIC", "Due Date:", WS_VISIBLE | WS_CHILD, startX_label, startY, labelWidth, 20, hwnd, NULL, GetModuleHandle(NULL), NULL);
            hEditDueDate = CreateWindow("EDIT", "", WS_VISIBLE | WS_CHILD | WS_BORDER | ES_AUTOHSCROLL, startX_edit, startY, editWidth, 20, hwnd, NULL, GetModuleHandle(NULL), NULL);

            startY += lineHeight + spacingY;
            CreateWindow("STATIC", "Return Date:", WS_VISIBLE | WS_CHILD, startX_label, startY, labelWidth, 20, hwnd, NULL, GetModuleHandle(NULL), NULL);
            hEditReturnDate = CreateWindow("EDIT", "", WS_VISIBLE | WS_CHILD | WS_BORDER | ES_AUTOHSCROLL, startX_edit, startY, editWidth, 20, hwnd, NULL, GetModuleHandle(NULL), NULL);

            startY += lineHeight + spacingY;
            CreateWindow("STATIC", "Staff ID:", WS_VISIBLE | WS_CHILD, startX_label, startY, labelWidth, 20, hwnd, NULL, GetModuleHandle(NULL), NULL);
            hEditStaffId = CreateWindow("EDIT", "", WS_VISIBLE | WS_CHILD | WS_BORDER | ES_AUTOHSCROLL, startX_edit, startY, 100, 20, hwnd, NULL, GetModuleHandle(NULL), NULL);

            startY += lineHeight + spacingY + 10;
            int buttonWidth = 120;
            int buttonSpacing = 10;
            int buttonX = startX_label;

            CreateWindow("BUTTON", "Add Borrowing", WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON | BS_FLAT, buttonX, startY, buttonWidth, 30, hwnd, (HMENU)101, GetModuleHandle(NULL), NULL);
            buttonX += buttonWidth + buttonSpacing;
            CreateWindow("BUTTON", "View All Borrowings", WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON | BS_FLAT, buttonX, startY, buttonWidth + 30, 30, hwnd, (HMENU)102, GetModuleHandle(NULL), NULL);
            buttonX += buttonWidth + 30 + buttonSpacing;
            CreateWindow("BUTTON", "Update Borrowing", WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON | BS_FLAT, buttonX, startY, buttonWidth, 30, hwnd, (HMENU)103, GetModuleHandle(NULL), NULL);

            SendMessage(hEditBorrowingId, WM_SETFONT, (WPARAM)hFont, TRUE);
            SendMessage(hEditBookId, WM_SETFONT, (WPARAM)hFont, TRUE);
            SendMessage(hEditMemberId, WM_SETFONT, (WPARAM)hFont, TRUE);
            SendMessage(hEditBorrowDate, WM_SETFONT, (WPARAM)hFont, TRUE);
            SendMessage(hEditDueDate, WM_SETFONT, (WPARAM)hFont, TRUE);
            SendMessage(hEditReturnDate, WM_SETFONT, (WPARAM)hFont, TRUE);
            SendMessage(hEditStaffId, WM_SETFONT, (WPARAM)hFont, TRUE);
            break;
        case WM_COMMAND:
            if (LOWORD(wp) == 101) {
                Borrowing newBorrowing;
                char buffer[MAX_STRING];
                GetWindowText(hEditBorrowingId, buffer, MAX_STRING); newBorrowing.borrowing_id = atoi(buffer);
                GetWindowText(hEditBookId, buffer, MAX_STRING); newBorrowing.book_id = atoi(buffer);
                GetWindowText(hEditMemberId, buffer, MAX_STRING); newBorrowing.member_id = atoi(buffer);
                GetWindowText(hEditBorrowDate, newBorrowing.borrow_date, 11);
                GetWindowText(hEditDueDate, newBorrowing.due_date, 11);
                GetWindowText(hEditReturnDate, newBorrowing.return_date, 11);
                GetWindowText(hEditStaffId, buffer, MAX_STRING); newBorrowing.staff_id = atoi(buffer);
                saveBorrowing(newBorrowing);
                MessageBox(hwnd, "Borrowing added successfully!", "Success", MB_OK);
            } else if (LOWORD(wp) == 102) {
                int count;
                Borrowing* borrowings = loadBorrowings(&count);
                char message[2000];
                strcpy(message, "Borrowings:\n");
                if (count == 0) strcat(message, "No borrowings found.\n");
                else {
                    char temp[500];
                    for(int i = 0; i < count; i++) {
                        sprintf(temp, "Borrowing ID: %d, Book ID: %d, Member ID: %d, Borrow Date: %s, Due Date: %s, Return Date: %s, Staff ID: %d\n",
                                borrowings[i].borrowing_id, borrowings[i].book_id, borrowings[i].member_id,
                                borrowings[i].borrow_date, borrowings[i].due_date, borrowings[i].return_date, borrowings[i].staff_id);
                        strncat(message, temp, sizeof(message) - strlen(message) - 1);
                    }
                    free(borrowings);
                }
                MessageBox(hwnd, message, "View All Borrowings", MB_OK);
            } else if (LOWORD(wp) == 103) {
                char buffer[MAX_STRING];
                GetWindowText(hEditBorrowingId, buffer, MAX_STRING);
                int borrowingId = atoi(buffer);
                int count;
                Borrowing* borrowings = loadBorrowings(&count);
                int found = 0;
                for(int i = 0; i < count; i++) {
                    if(borrowings[i].borrowing_id == borrowingId) {
                        GetWindowText(hEditBookId, buffer, MAX_STRING); borrowings[i].book_id = atoi(buffer);
                        GetWindowText(hEditMemberId, buffer, MAX_STRING); borrowings[i].member_id = atoi(buffer);
                        GetWindowText(hEditBorrowDate, borrowings[i].borrow_date, 11);
                        GetWindowText(hEditDueDate, borrowings[i].due_date, 11);
                        GetWindowText(hEditReturnDate, borrowings[i].return_date, 11);
                        GetWindowText(hEditStaffId, buffer, MAX_STRING); borrowings[i].staff_id = atoi(buffer);
                        FILE* file = fopen("borrowings.dat", "wb");
                        if (file) {
                            fwrite(borrowings, sizeof(Borrowing), count, file);
                            fclose(file);
                            MessageBox(hwnd, "Borrowing updated successfully!", "Success", MB_OK);
                        } else {
                            MessageBox(hwnd, "Failed to update borrowing!", "Error", MB_OK);
                        }
                        found = 1;
                        break;
                    }
                }
                if (!found) {
                    MessageBox(hwnd, "Borrowing not found!", "Error", MB_OK);
                }
                free(borrowings);
            }
            break;

        case WM_CTLCOLORSTATIC:
        case WM_CTLCOLORBTN:
        case WM_CTLCOLOREDIT: {
            HDC hdc = (HDC)wp;
            if (isDarkTheme) {
                SetTextColor(hdc, RGB(255, 255, 255));
                SetBkColor(hdc, RGB(30, 30, 30));
                return (LRESULT)hBrushDark;
            } else {
                SetTextColor(hdc, RGB(0, 0, 0));
                SetBkColor(hdc, RGB(240, 240, 240));
                return (LRESULT)hBrushLight;
            }
        }

        case WM_ERASEBKGND:
            return (LRESULT)1;

        case WM_PAINT: {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);
            RECT rect;
            GetClientRect(hwnd, &rect);
            FillRect(hdc, &rect, isDarkTheme ? hBrushDark : hBrushLight);
            EndPaint(hwnd, &ps);
            break;
        }

        case WM_CLOSE:
            DestroyWindow(hwnd);
            break;
        default:
            return DefWindowProc(hwnd, msg, wp, lp);
    }
    return 0;
}

void showFineManagementGUI() {
    HINSTANCE hInst = GetModuleHandle(NULL);
    WNDCLASSEX wc = {0};
    wc.cbSize = sizeof(WNDCLASSEX);
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = FineManagementWindowProcedure;
    wc.hInstance = hInst;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = NULL;
    wc.lpszClassName = "FineManagementWindow";
    
    if(!RegisterClassEx(&wc)) return;
    
    CreateWindowEx(
        0, "FineManagementWindow", "Fine Management",
        WS_OVERLAPPEDWINDOW | WS_VISIBLE, CW_USEDEFAULT, CW_USEDEFAULT,
        600, 400, hwnd, NULL, hInst, NULL
    );
}

LRESULT CALLBACK FineManagementWindowProcedure(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp) {
    static HWND hEditFineId, hEditBorrowingId, hEditAmount, hEditPaid, hEditDatePaid;
    switch(msg) {
        case WM_CREATE:
            int labelWidth = 100;
            int editWidth = 150;
            int lineHeight = 25;
            int startY = 20;
            int spacingY = 10;
            int startX_label = 20;
            int startX_edit = startX_label + labelWidth + 10;

            CreateWindow("STATIC", "Fine ID:", WS_VISIBLE | WS_CHILD, startX_label, startY, labelWidth, 20, hwnd, NULL, GetModuleHandle(NULL), NULL);
            hEditFineId = CreateWindow("EDIT", "", WS_VISIBLE | WS_CHILD | WS_BORDER | ES_AUTOHSCROLL, startX_edit, startY, 100, 20, hwnd, NULL, GetModuleHandle(NULL), NULL);

            startY += lineHeight + spacingY;
            CreateWindow("STATIC", "Borrowing ID:", WS_VISIBLE | WS_CHILD, startX_label, startY, labelWidth, 20, hwnd, NULL, GetModuleHandle(NULL), NULL);
            hEditBorrowingId = CreateWindow("EDIT", "", WS_VISIBLE | WS_CHILD | WS_BORDER | ES_AUTOHSCROLL, startX_edit, startY, 100, 20, hwnd, NULL, GetModuleHandle(NULL), NULL);

            startY += lineHeight + spacingY;
            CreateWindow("STATIC", "Amount:", WS_VISIBLE | WS_CHILD, startX_label, startY, labelWidth, 20, hwnd, NULL, GetModuleHandle(NULL), NULL);
            hEditAmount = CreateWindow("EDIT", "", WS_VISIBLE | WS_CHILD | WS_BORDER | ES_AUTOHSCROLL, startX_edit, startY, editWidth, 20, hwnd, NULL, GetModuleHandle(NULL), NULL);

            startY += lineHeight + spacingY;
            CreateWindow("STATIC", "Paid (0/1):", WS_VISIBLE | WS_CHILD, startX_label, startY, labelWidth, 20, hwnd, NULL, GetModuleHandle(NULL), NULL);
            hEditPaid = CreateWindow("EDIT", "", WS_VISIBLE | WS_CHILD | WS_BORDER | ES_AUTOHSCROLL, startX_edit, startY, 100, 20, hwnd, NULL, GetModuleHandle(NULL), NULL);

            startY += lineHeight + spacingY;
            CreateWindow("STATIC", "Date Paid:", WS_VISIBLE | WS_CHILD, startX_label, startY, labelWidth, 20, hwnd, NULL, GetModuleHandle(NULL), NULL);
            hEditDatePaid = CreateWindow("EDIT", "", WS_VISIBLE | WS_CHILD | WS_BORDER | ES_AUTOHSCROLL, startX_edit, startY, editWidth, 20, hwnd, NULL, GetModuleHandle(NULL), NULL);

            startY += lineHeight + spacingY + 10;
            int buttonWidth = 120;
            int buttonSpacing = 10;
            int buttonX = startX_label;

            CreateWindow("BUTTON", "Add Fine", WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON | BS_FLAT, buttonX, startY, buttonWidth, 30, hwnd, (HMENU)101, GetModuleHandle(NULL), NULL);
            buttonX += buttonWidth + buttonSpacing;
            CreateWindow("BUTTON", "View All Fines", WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON | BS_FLAT, buttonX, startY, buttonWidth + 30, 30, hwnd, (HMENU)102, GetModuleHandle(NULL), NULL);
            buttonX += buttonWidth + 30 + buttonSpacing;
            CreateWindow("BUTTON", "Update Fine", WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON | BS_FLAT, buttonX, startY, buttonWidth, 30, hwnd, (HMENU)103, GetModuleHandle(NULL), NULL);

            SendMessage(hEditFineId, WM_SETFONT, (WPARAM)hFont, TRUE);
            SendMessage(hEditBorrowingId, WM_SETFONT, (WPARAM)hFont, TRUE);
            SendMessage(hEditAmount, WM_SETFONT, (WPARAM)hFont, TRUE);
            SendMessage(hEditPaid, WM_SETFONT, (WPARAM)hFont, TRUE);
            SendMessage(hEditDatePaid, WM_SETFONT, (WPARAM)hFont, TRUE);
            break;
        case WM_COMMAND:
            if (LOWORD(wp) == 101) {
                Fine newFine;
                char buffer[MAX_STRING];
                GetWindowText(hEditFineId, buffer, MAX_STRING); newFine.fine_id = atoi(buffer);
                GetWindowText(hEditBorrowingId, buffer, MAX_STRING); newFine.borrowing_id = atoi(buffer);
                GetWindowText(hEditAmount, buffer, MAX_STRING); newFine.amount = atof(buffer);
                GetWindowText(hEditPaid, buffer, MAX_STRING); newFine.paid = atoi(buffer);
                GetWindowText(hEditDatePaid, newFine.date_paid, 11);
                saveFine(newFine);
                MessageBox(hwnd, "Fine added successfully!", "Success", MB_OK);
            } else if (LOWORD(wp) == 102) {
                int count;
                Fine* fines = loadFines(&count);
                char message[2000];
                strcpy(message, "Fines:\n");
                if (count == 0) strcat(message, "No fines found.\n");
                else {
                    char temp[500];
                    for(int i = 0; i < count; i++) {
                        sprintf(temp, "Fine ID: %d, Borrowing ID: %d, Amount: %.2f, Paid: %d, Date Paid: %s\n",
                                fines[i].fine_id, fines[i].borrowing_id, fines[i].amount, fines[i].paid, fines[i].date_paid);
                        strncat(message, temp, sizeof(message) - strlen(message) - 1);
                    }
                    free(fines);
                }
                MessageBox(hwnd, message, "View All Fines", MB_OK);
            } else if (LOWORD(wp) == 103) {
                char buffer[MAX_STRING];
                GetWindowText(hEditFineId, buffer, MAX_STRING);
                int fineId = atoi(buffer);
                int count;
                Fine* fines = loadFines(&count);
                int found = 0;
                for(int i = 0; i < count; i++) {
                    if(fines[i].fine_id == fineId) {
                        GetWindowText(hEditBorrowingId, buffer, MAX_STRING); fines[i].borrowing_id = atoi(buffer);
                        GetWindowText(hEditAmount, buffer, MAX_STRING); fines[i].amount = atof(buffer);
                        GetWindowText(hEditPaid, buffer, MAX_STRING); fines[i].paid = atoi(buffer);
                        GetWindowText(hEditDatePaid, fines[i].date_paid, 11);
                        FILE* file = fopen("fines.dat", "wb");
                        if (file) {
                            fwrite(fines, sizeof(Fine), count, file);
                            fclose(file);
                            MessageBox(hwnd, "Fine updated successfully!", "Success", MB_OK);
                        } else {
                            MessageBox(hwnd, "Failed to update fine!", "Error", MB_OK);
                        }
                        found = 1;
                        break;
                    }
                }
                if (!found) {
                    MessageBox(hwnd, "Fine not found!", "Error", MB_OK);
                }
                free(fines);
            }
            break;

        case WM_CTLCOLORSTATIC:
        case WM_CTLCOLORBTN:
        case WM_CTLCOLOREDIT: {
            HDC hdc = (HDC)wp;
            if (isDarkTheme) {
                SetTextColor(hdc, RGB(255, 255, 255));
                SetBkColor(hdc, RGB(30, 30, 30));
                return (LRESULT)hBrushDark;
            } else {
                SetTextColor(hdc, RGB(0, 0, 0));
                SetBkColor(hdc, RGB(240, 240, 240));
                return (LRESULT)hBrushLight;
            }
        }

        case WM_ERASEBKGND:
            return (LRESULT)1;

        case WM_PAINT: {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);
            RECT rect;
            GetClientRect(hwnd, &rect);
            FillRect(hdc, &rect, isDarkTheme ? hBrushDark : hBrushLight);
            EndPaint(hwnd, &ps);
            break;
        }

        case WM_CLOSE:
            DestroyWindow(hwnd);
            break;
        default:
            return DefWindowProc(hwnd, msg, wp, lp);
    }
    return 0;
}

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    showGUIMenu();
    return 0;
}