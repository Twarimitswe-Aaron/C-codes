#include <windows.h>
#include <commctrl.h>
#include <uxtheme.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "gui.h"
#include "database.h"
#include "structures.h"
#include "date_utils.h"

// Global variables (defined once here)
COLORREF g_crBackground = RGB(255, 255, 255); // White background
COLORREF g_crText = RGB(20, 20, 100);         // Dark blue text
COLORREF g_crAccent = RGB(0, 120, 215);      // Standard blue accent
COLORREF g_crAccentDark = RGB(0, 80, 180);   // Darker accent for pressed state
COLORREF g_crAccentLight = RGB(60, 160, 255); // Lighter accent for hover state

HWND g_hwndMain;
HWND g_hwndCurrent;
BOOL g_isDarkTheme = FALSE;

HFONT hFontDashboardTitle; // Global font handle for Dashboard title
HFONT hFontCardCount;      // Global font handle for card counts
HFONT hFontCardTitle;      // Global font handle for card titles

// Add ListBox macros if not defined
#ifndef ListBox_ResetContent
#define ListBox_ResetContent(hwndCtl) \
    ((int)(DWORD)SNDMSG((hwndCtl), LB_RESETCONTENT, 0L, 0L))
#endif

#ifndef ListBox_AddString
#define ListBox_AddString(hwndCtl, lpsz) \
    ((int)(DWORD)SNDMSG((hwndCtl), LB_ADDSTRING, 0L, (LPARAM)(LPCTSTR)(lpsz)))
#endif

// Function declarations
BOOL isEmpty(const char* str);

// Windows entry point
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    // Initialize common controls
    INITCOMMONCONTROLSEX icex;
    icex.dwSize = sizeof(INITCOMMONCONTROLSEX);
    icex.dwICC = ICC_LISTVIEW_CLASSES | ICC_BAR_CLASSES;
    InitCommonControlsEx(&icex);
    
    // Start the GUI
    startGUI();
    return 0;
}

// Helper functions
void createModernButton(HWND hwnd, const char* text, int x, int y, int width, int height, int id) {
    CreateWindow("BUTTON", text,
        WS_VISIBLE | WS_CHILD | BS_OWNERDRAW,
        x, y, width, height,
        hwnd, (HMENU)(INT_PTR)id, NULL, NULL);
}

void createModernInput(HWND hwnd, int x, int y, int width, int height, int id) {
    CreateWindow("EDIT", "",
        WS_VISIBLE | WS_CHILD | WS_BORDER | ES_AUTOHSCROLL,
        x, y, width, height,
        hwnd, (HMENU)(INT_PTR)id, NULL, NULL);
}

HWND createModernLabel(HWND hwnd, const char* text, int x, int y, int width, int height) {
    HWND hLabel = CreateWindow("STATIC", text,
        WS_VISIBLE | WS_CHILD,
        x, y, width, height,
        hwnd, NULL, NULL, NULL);
    
    // Set font
    HFONT hFont = (HFONT)SendMessage(hLabel, WM_GETFONT, 0, 0);
    SendMessage(hLabel, WM_SETFONT, (WPARAM)hFont, TRUE);
    
    return hLabel;
}

// Helper function to create a modern list view
HWND createModernListView(HWND hwnd, int x, int y, int width, int height, int id) {
    HWND hwndList = CreateWindow(WC_LISTVIEW, "",
        WS_VISIBLE | WS_CHILD | LVS_REPORT | LVS_SINGLESEL,
        x, y, width, height,
        hwnd, (HMENU)(INT_PTR)id, NULL, NULL);
    
    // Enable full row selection
    ListView_SetExtendedListViewStyle(hwndList, LVS_EX_FULLROWSELECT);
    
    return hwndList;
}

// Main GUI functions
void startGUI() {
    WNDCLASSEX wc = {0};
    wc.cbSize = sizeof(WNDCLASSEX);
    wc.lpfnWndProc = MainWindowProc;
    wc.hInstance = GetModuleHandle(NULL);
    wc.lpszClassName = "LibrarySystemClass";
    wc.hbrBackground = CreateSolidBrush(g_crBackground); // Set white background for the window class
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    
    RegisterClassEx(&wc);
    
    g_hwndMain = CreateWindowEx(
        0,
        "LibrarySystemClass",
        "Library Management System",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT,
        800, 600,
        NULL, NULL,
        GetModuleHandle(NULL),
        NULL
    );
    
    // Initialize global font handles
    hFontDashboardTitle = CreateFont(
        48, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE, DEFAULT_CHARSET,
        OUT_OUTLINE_PRECIS, CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY, VARIABLE_PITCH, TEXT("Segoe UI"));
    
    hFontCardCount = CreateFont(
        36, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE, DEFAULT_CHARSET,
        OUT_OUTLINE_PRECIS, CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY, VARIABLE_PITCH, TEXT("Segoe UI"));

    hFontCardTitle = CreateFont(
        20, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE, DEFAULT_CHARSET,
        OUT_OUTLINE_PRECIS, CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY, VARIABLE_PITCH, TEXT("Segoe UI"));

    ShowWindow(g_hwndMain, SW_SHOW);
    UpdateWindow(g_hwndMain);
    
    // Message loop
    MSG msg = {0};
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
}

void showMainMenu(HWND hwnd) {
    // Clear existing controls
    HWND child = GetWindow(hwnd, GW_CHILD);
    while (child) {
        DestroyWindow(child);
        child = GetWindow(hwnd, GW_CHILD);
    }

    // Add Dashboard title
    HWND hDashboardTitle = createModernLabel(hwnd, "Dashboard", 50, 10, 400, 50);
    SendMessage(hDashboardTitle, WM_SETFONT, (WPARAM)hFontDashboardTitle, TRUE);

    // Calculate positions for a grid layout within the left panel
    int numColumns = 1;
    int cardWidth = LEFT_PANEL_WIDTH - (2 * CARD_PADDING);
    int cardHeight = 80; // Increased height for better visibility
    int verticalSpacing = 15; // Increased spacing
    int startX = CARD_PADDING;
    int startY = 80;
    
    // Create main menu cards/buttons
    const char* cardTitles[] = {
        "Book Management",
        "Author Management",
        "Publisher Management",
        "Member Management",
        "Staff Management",
        "Borrowing Management",
        "Fine Management"
    };
    
    int numCards = sizeof(cardTitles) / sizeof(cardTitles[0]);

    for (int i = 0; i < numCards; i++) {
        int row = i / numColumns;
        int col = i % numColumns;

        int x = startX + col * (cardWidth + 0);
        int y = startY + row * (cardHeight + verticalSpacing);

        // Create the clickable button as the card background
        HWND hButton = CreateWindow("BUTTON", "",
            WS_VISIBLE | WS_CHILD | BS_OWNERDRAW,
            x, y, cardWidth, cardHeight,
            hwnd, (HMENU)(INT_PTR)(i + 1), NULL, NULL);

        // Create label for the numerical count
        char countText[64];
        int count = 0;

        switch(i + 1) {
            case 1: { Book* books = getAllBooks(&count); if (books) free(books); break; }
            case 2: { Author* authors = getAllAuthors(&count); if (authors) free(authors); break; }
            case 3: { Publisher* publishers = getAllPublishers(&count); if (publishers) free(publishers); break; }
            case 4: { Member* members = getAllMembers(&count); if (members) free(members); break; }
            case 5: { Staff* staff = getAllStaff(&count); if (staff) free(staff); break; }
            case 6: { Borrowing* borrowings = getAllBorrowings(&count); if (borrowings) free(borrowings); break; }
            case 7: { Fine* fines = getAllFines(&count); if (fines) free(fines); break; }
        }

        sprintf(countText, "%d", count);
        HWND hCountLabel = createModernLabel(hwnd, countText, x + CARD_PADDING, y + 10, cardWidth - (2 * CARD_PADDING), 30);
        SendMessage(hCountLabel, WM_SETFONT, (WPARAM)hFontCardCount, TRUE);

        // Create label for the category title
        HWND hTitleLabel = createModernLabel(hwnd, cardTitles[i], x + CARD_PADDING, y + 45, cardWidth - (2 * CARD_PADDING), 20);
        SendMessage(hTitleLabel, WM_SETFONT, (WPARAM)hFontCardTitle, TRUE);
    }
}

// Function to clear all child windows from a given parent window
void clearWindow(HWND hwnd) {
    HWND child = GetWindow(hwnd, GW_CHILD);
    while (child) {
        DestroyWindow(child);
        child = GetWindow(hwnd, GW_CHILD);
    }
}

// Function to clear controls in the right panel
void clearRightPanel(HWND hwnd) {
    // Destroy all controls in the right panel
    HWND child = GetWindow(hwnd, GW_CHILD);
    while (child) {
        RECT rect;
        GetWindowRect(child, &rect);
        MapWindowPoints(HWND_DESKTOP, hwnd, (LPPOINT)&rect, 2); // Convert to client coordinates

        if (rect.left >= RIGHT_PANEL_START_X) {
            DestroyWindow(child);
        }
        child = GetWindow(hwnd, GW_CHILD);
    }
}

void showBookManagement(HWND hwnd) {
    g_hwndCurrent = hwnd; // Set current active window handle
    
    // Add Book Management title
    createModernLabel(hwnd, "Book Management", RIGHT_PANEL_START_X + 10, 10, 400, 30);

    // Create input fields and labels
    createModernLabel(hwnd, "ID:", RIGHT_PANEL_START_X + 10, 50, 80, 25);
    createModernInput(hwnd, RIGHT_PANEL_START_X + 100, 50, 150, 25, 200); // ID

    createModernLabel(hwnd, "Title:", RIGHT_PANEL_START_X + 10, 80, 80, 25);
    createModernInput(hwnd, RIGHT_PANEL_START_X + 100, 80, 200, 25, 206); // Title

    createModernLabel(hwnd, "Author ID:", RIGHT_PANEL_START_X + 10, 110, 80, 25);
    createModernInput(hwnd, RIGHT_PANEL_START_X + 100, 110, 150, 25, 207); // Author ID

    createModernLabel(hwnd, "Publisher ID:", RIGHT_PANEL_START_X + 10, 140, 80, 25);
    createModernInput(hwnd, RIGHT_PANEL_START_X + 100, 140, 150, 25, 208); // Publisher ID

    createModernLabel(hwnd, "ISBN:", RIGHT_PANEL_START_X + 10, 170, 80, 25);
    createModernInput(hwnd, RIGHT_PANEL_START_X + 100, 170, 150, 25, 209); // ISBN

    createModernLabel(hwnd, "Quantity:", RIGHT_PANEL_START_X + 10, 200, 80, 25);
    createModernInput(hwnd, RIGHT_PANEL_START_X + 100, 200, 150, 25, 210); // Quantity

    createModernLabel(hwnd, "Published Date (YYYY-MM-DD):", RIGHT_PANEL_START_X + 10, 230, 180, 25);
    createModernInput(hwnd, RIGHT_PANEL_START_X + 200, 230, 150, 25, 211); // Published Date

    // Create buttons
    createModernButton(hwnd, "Add Book", RIGHT_PANEL_START_X + 10, 270, 100, 30, 201);
    createModernButton(hwnd, "Update Book", RIGHT_PANEL_START_X + 120, 270, 100, 30, 202);
    createModernButton(hwnd, "Delete Book", RIGHT_PANEL_START_X + 230, 270, 100, 30, 203);
    createModernButton(hwnd, "Clear Fields", RIGHT_PANEL_START_X + 340, 270, 100, 30, 204);
    createModernButton(hwnd, "Back to Main", RIGHT_PANEL_START_X + 10, 310, 150, 30, 205);


    // Create list view for displaying books
    HWND hListView = createModernListView(hwnd, RIGHT_PANEL_START_X + 10, 350, 500, 200, 212); // Book List

    // Add columns to the list view
    LVCOLUMN lvc;
    lvc.mask = LVCF_TEXT | LVCF_WIDTH | LVCF_FMT;
    lvc.fmt = LVCFMT_LEFT;

    lvc.pszText = "ID";
    lvc.cx = 50;
    ListView_InsertColumn(hListView, 0, &lvc);

    lvc.pszText = "Title";
    lvc.cx = 150;
    ListView_InsertColumn(hListView, 1, &lvc);

    lvc.pszText = "Author ID";
    lvc.cx = 80;
    ListView_InsertColumn(hListView, 2, &lvc);

    lvc.pszText = "Publisher ID";
    lvc.cx = 90;
    ListView_InsertColumn(hListView, 3, &lvc);

    lvc.pszText = "ISBN";
    lvc.cx = 100;
    ListView_InsertColumn(hListView, 4, &lvc);

    lvc.pszText = "Quantity";
    lvc.cx = 60;
    ListView_InsertColumn(hListView, 5, &lvc);

    lvc.pszText = "Published Date";
    lvc.cx = 120;
    ListView_InsertColumn(hListView, 6, &lvc);

    refreshBookList(hwnd);
}

void refreshBookList(HWND hwnd) {
    HWND hListView = GetDlgItem(hwnd, 212); // Get the list view handle

    // Clear existing items
    ListView_DeleteAllItems(hListView);

    int count = 0;
    Book* books = getAllBooks(&count);

    if (books) {
        for (int i = 0; i < count; i++) {
            LVITEM lvI;
            lvI.mask = LVIF_TEXT;
            lvI.iItem = i;
            lvI.iSubItem = 0;
            char buffer[256];

            sprintf(buffer, "%d", books[i].book_id);
            lvI.pszText = buffer;
            ListView_InsertItem(hListView, &lvI);

            ListView_SetItemText(hListView, i, 1, books[i].title);
            
            sprintf(buffer, "%d", books[i].author_id);
            ListView_SetItemText(hListView, i, 2, buffer);

            sprintf(buffer, "%d", books[i].publisher_id);
            ListView_SetItemText(hListView, i, 3, buffer);

            ListView_SetItemText(hListView, i, 4, books[i].isbn);
            
            sprintf(buffer, "%d", books[i].quantity);
            ListView_SetItemText(hListView, i, 5, buffer);

            ListView_SetItemText(hListView, i, 6, books[i].published_date);
        }
        free(books);
    }
}

void clearBookInputs(HWND hwnd) {
    SetWindowText(GetDlgItem(hwnd, 200), ""); // ID
    SetWindowText(GetDlgItem(hwnd, 206), ""); // Title
    SetWindowText(GetDlgItem(hwnd, 207), ""); // Author ID
    SetWindowText(GetDlgItem(hwnd, 208), ""); // Publisher ID
    SetWindowText(GetDlgItem(hwnd, 209), ""); // ISBN
    SetWindowText(GetDlgItem(hwnd, 210), ""); // Quantity
    SetWindowText(GetDlgItem(hwnd, 211), ""); // Published Date
}

Book getBookFromInputs(HWND hwnd) {
    Book book;
    char buffer[256];

    GetWindowText(GetDlgItem(hwnd, 200), buffer, sizeof(buffer));
    book.book_id = atoi(buffer);

    GetWindowText(GetDlgItem(hwnd, 206), book.title, sizeof(book.title));
    GetWindowText(GetDlgItem(hwnd, 207), buffer, sizeof(buffer));
    book.author_id = atoi(buffer);

    GetWindowText(GetDlgItem(hwnd, 208), buffer, sizeof(buffer));
    book.publisher_id = atoi(buffer);

    GetWindowText(GetDlgItem(hwnd, 209), book.isbn, sizeof(book.isbn));
    GetWindowText(GetDlgItem(hwnd, 210), buffer, sizeof(buffer));
    book.quantity = atoi(buffer);

    GetWindowText(GetDlgItem(hwnd, 211), book.published_date, sizeof(book.published_date));
    
    return book;
}

void showAuthorManagement(HWND hwnd) {
    g_hwndCurrent = hwnd; // Set current active window handle

    createModernLabel(hwnd, "Author Management", RIGHT_PANEL_START_X + 10, 10, 400, 30);

    // Back to Main Menu button
    createModernButton(hwnd, "Back to Main", RIGHT_PANEL_START_X + 10, 160, 150, 30, 305);
}

void showPublisherManagement(HWND hwnd) {
    g_hwndCurrent = hwnd; // Set current active window handle

    createModernLabel(hwnd, "Publisher Management", RIGHT_PANEL_START_X + 10, 10, 400, 30);

    // Back to Main Menu button
    createModernButton(hwnd, "Back to Main", RIGHT_PANEL_START_X + 10, 160, 150, 30, 405);
}

void showMemberManagement(HWND hwnd) {
    g_hwndCurrent = hwnd; // Set current active window handle

    createModernLabel(hwnd, "Member Management", RIGHT_PANEL_START_X + 10, 10, 400, 30);

    // Back to Main Menu button
    createModernButton(hwnd, "Back to Main", RIGHT_PANEL_START_X + 10, 160, 150, 30, 505);
}

void showStaffManagement(HWND hwnd) {
    g_hwndCurrent = hwnd; // Set current active window handle

    createModernLabel(hwnd, "Staff Management", RIGHT_PANEL_START_X + 10, 10, 400, 30);

    // Back to Main Menu button
    createModernButton(hwnd, "Back to Main", RIGHT_PANEL_START_X + 10, 160, 150, 30, 605);
}

void showBorrowingManagement(HWND hwnd) {
    g_hwndCurrent = hwnd; // Set current active window handle

    createModernLabel(hwnd, "Borrowing Management", RIGHT_PANEL_START_X + 10, 10, 400, 30);

    createModernLabel(hwnd, "Borrow ID:", RIGHT_PANEL_START_X + 10, 50, 80, 25);
    createModernInput(hwnd, RIGHT_PANEL_START_X + 100, 50, 150, 25, 700); // Borrow ID

    createModernLabel(hwnd, "Book ID:", RIGHT_PANEL_START_X + 10, 80, 80, 25);
    createModernInput(hwnd, RIGHT_PANEL_START_X + 100, 80, 150, 25, 706); // Book ID

    createModernLabel(hwnd, "Member ID:", RIGHT_PANEL_START_X + 10, 110, 80, 25);
    createModernInput(hwnd, RIGHT_PANEL_START_X + 100, 110, 150, 25, 707); // Member ID

    createModernLabel(hwnd, "Borrow Date (YYYY-MM-DD):", RIGHT_PANEL_START_X + 10, 140, 180, 25);
    createModernInput(hwnd, RIGHT_PANEL_START_X + 200, 140, 150, 25, 708); // Borrow Date

    createModernLabel(hwnd, "Return Date (YYYY-MM-DD):", RIGHT_PANEL_START_X + 10, 170, 180, 25);
    createModernInput(hwnd, RIGHT_PANEL_START_X + 200, 170, 150, 25, 709); // Return Date

    createModernButton(hwnd, "Add Borrowing", RIGHT_PANEL_START_X + 10, 210, 100, 30, 701);
    createModernButton(hwnd, "Update Borrowing", RIGHT_PANEL_START_X + 120, 210, 100, 30, 702);
    createModernButton(hwnd, "Delete Borrowing", RIGHT_PANEL_START_X + 230, 210, 100, 30, 703);
    createModernButton(hwnd, "Clear Fields", RIGHT_PANEL_START_X + 340, 210, 100, 30, 704);
    createModernButton(hwnd, "Back to Main", RIGHT_PANEL_START_X + 10, 250, 150, 30, 705);

    HWND hListView = createModernListView(hwnd, RIGHT_PANEL_START_X + 10, 290, 500, 200, 710); // Borrowing List

    LVCOLUMN lvc;
    lvc.mask = LVCF_TEXT | LVCF_WIDTH | LVCF_FMT;
    lvc.fmt = LVCFMT_LEFT;

    lvc.pszText = "Borrow ID";
    lvc.cx = 80;
    ListView_InsertColumn(hListView, 0, &lvc);

    lvc.pszText = "Book ID";
    lvc.cx = 80;
    ListView_InsertColumn(hListView, 1, &lvc);

    lvc.pszText = "Member ID";
    lvc.cx = 80;
    ListView_InsertColumn(hListView, 2, &lvc);

    lvc.pszText = "Borrow Date";
    lvc.cx = 120;
    ListView_InsertColumn(hListView, 3, &lvc);

    lvc.pszText = "Return Date";
    lvc.cx = 120;
    ListView_InsertColumn(hListView, 4, &lvc);

    refreshBorrowingList(hwnd);
}

void refreshBorrowingList(HWND hwnd) {
    HWND hListView = GetDlgItem(hwnd, 710);
    ListView_DeleteAllItems(hListView);
    
    int count = 0;
    Borrowing* borrowings = getAllBorrowings(&count);

    if (borrowings) {
        for (int i = 0; i < count; i++) {
            LVITEM lvI;
            lvI.mask = LVIF_TEXT;
            lvI.iItem = i;
            lvI.iSubItem = 0;
            char buffer[256];

            sprintf(buffer, "%d", borrowings[i].borrow_id);
            lvI.pszText = buffer;
            ListView_InsertItem(hListView, &lvI);

            sprintf(buffer, "%d", borrowings[i].book_id);
            ListView_SetItemText(hListView, i, 1, buffer);

            sprintf(buffer, "%d", borrowings[i].member_id);
            ListView_SetItemText(hListView, i, 2, buffer);

            ListView_SetItemText(hListView, i, 3, borrowings[i].borrow_date);
            ListView_SetItemText(hListView, i, 4, borrowings[i].return_date);
        }
        free(borrowings);
    }
}

void clearBorrowingInputs(HWND hwnd) {
    SetWindowText(GetDlgItem(hwnd, 700), ""); // Borrow ID
    SetWindowText(GetDlgItem(hwnd, 706), ""); // Book ID
    SetWindowText(GetDlgItem(hwnd, 707), ""); // Member ID
    SetWindowText(GetDlgItem(hwnd, 708), ""); // Borrow Date
    SetWindowText(GetDlgItem(hwnd, 709), ""); // Return Date
}

Borrowing getBorrowingFromInputs(HWND hwnd) {
    Borrowing borrowing;
    char buffer[256];
    GetWindowText(GetDlgItem(hwnd, 700), buffer, sizeof(buffer));
    borrowing.borrow_id = atoi(buffer);
    GetWindowText(GetDlgItem(hwnd, 706), buffer, sizeof(buffer));
    borrowing.book_id = atoi(buffer);
    GetWindowText(GetDlgItem(hwnd, 707), buffer, sizeof(buffer));
    borrowing.member_id = atoi(buffer);
    GetWindowText(GetDlgItem(hwnd, 708), borrowing.borrow_date, sizeof(borrowing.borrow_date));
    GetWindowText(GetDlgItem(hwnd, 709), borrowing.return_date, sizeof(borrowing.return_date));
    return borrowing;
}

void showFineManagement(HWND hwnd) {
    g_hwndCurrent = hwnd; // Set current active window handle

    createModernLabel(hwnd, "Fine Management", RIGHT_PANEL_START_X + 10, 10, 400, 30);

    createModernLabel(hwnd, "Fine ID:", RIGHT_PANEL_START_X + 10, 50, 80, 25);
    createModernInput(hwnd, RIGHT_PANEL_START_X + 100, 50, 150, 25, 800); // Fine ID

    createModernLabel(hwnd, "Borrow ID:", RIGHT_PANEL_START_X + 10, 80, 80, 25);
    createModernInput(hwnd, RIGHT_PANEL_START_X + 100, 80, 150, 25, 806); // Borrow ID

    createModernLabel(hwnd, "Amount:", RIGHT_PANEL_START_X + 10, 110, 80, 25);
    createModernInput(hwnd, RIGHT_PANEL_START_X + 100, 110, 150, 25, 807); // Amount

    createModernLabel(hwnd, "Paid (0/1):", RIGHT_PANEL_START_X + 10, 140, 80, 25);
    createModernInput(hwnd, RIGHT_PANEL_START_X + 100, 140, 150, 25, 808); // Paid

    createModernButton(hwnd, "Add Fine", RIGHT_PANEL_START_X + 10, 180, 100, 30, 801);
    createModernButton(hwnd, "Update Fine", RIGHT_PANEL_START_X + 120, 180, 100, 30, 802);
    createModernButton(hwnd, "Delete Fine", RIGHT_PANEL_START_X + 230, 180, 100, 30, 803);
    createModernButton(hwnd, "Clear Fields", RIGHT_PANEL_START_X + 340, 180, 100, 30, 804);
    createModernButton(hwnd, "Back to Main", RIGHT_PANEL_START_X + 10, 220, 150, 30, 805);

    HWND hListView = createModernListView(hwnd, RIGHT_PANEL_START_X + 10, 260, 400, 200, 809); // Fine List

    LVCOLUMN lvc;
    lvc.mask = LVCF_TEXT | LVCF_WIDTH | LVCF_FMT;
    lvc.fmt = LVCFMT_LEFT;

    lvc.pszText = "Fine ID";
    lvc.cx = 80;
    ListView_InsertColumn(hListView, 0, &lvc);

    lvc.pszText = "Borrow ID";
    lvc.cx = 80;
    ListView_InsertColumn(hListView, 1, &lvc);

    lvc.pszText = "Amount";
    lvc.cx = 80;
    ListView_InsertColumn(hListView, 2, &lvc);

    lvc.pszText = "Paid";
    lvc.cx = 80;
    ListView_InsertColumn(hListView, 3, &lvc);

    refreshFineList(hwnd);
}

void refreshFineList(HWND hwnd) {
    HWND hListView = GetDlgItem(hwnd, 809);
    ListView_DeleteAllItems(hListView);
    
    int count = 0;
    Fine* fines = getAllFines(&count);

    if (fines) {
        for (int i = 0; i < count; i++) {
            LVITEM lvI;
            lvI.mask = LVIF_TEXT;
            lvI.iItem = i;
            lvI.iSubItem = 0;
            char buffer[256];

            sprintf(buffer, "%d", fines[i].fine_id);
            lvI.pszText = buffer;
            ListView_InsertItem(hListView, &lvI);

            sprintf(buffer, "%d", fines[i].borrow_id);
            ListView_SetItemText(hListView, i, 1, buffer);

            sprintf(buffer, "%.2f", fines[i].amount);
            ListView_SetItemText(hListView, i, 2, buffer);

            sprintf(buffer, "%d", fines[i].is_paid);
            ListView_SetItemText(hListView, i, 3, buffer);
        }
        free(fines);
    }
}

void clearFineInputs(HWND hwnd) {
    SetWindowText(GetDlgItem(hwnd, 800), ""); // Fine ID
    SetWindowText(GetDlgItem(hwnd, 806), ""); // Borrow ID
    SetWindowText(GetDlgItem(hwnd, 807), ""); // Amount
    SetWindowText(GetDlgItem(hwnd, 808), ""); // Paid
}

Fine getFineFromInputs(HWND hwnd) {
    Fine fine;
    char buffer[256];
    GetWindowText(GetDlgItem(hwnd, 800), buffer, sizeof(buffer));
    fine.fine_id = atoi(buffer);
    GetWindowText(GetDlgItem(hwnd, 806), buffer, sizeof(buffer));
    fine.borrow_id = atoi(buffer);
    GetWindowText(GetDlgItem(hwnd, 807), buffer, sizeof(buffer));
    fine.amount = atof(buffer);
    GetWindowText(GetDlgItem(hwnd, 808), buffer, sizeof(buffer));
    fine.is_paid = atoi(buffer);
    return fine;
}

// Function to draw modern style buttons
void drawButton(LPDRAWITEMSTRUCT lpdis, COLORREF accentColor, COLORREF accentDarkColor, COLORREF accentLightColor) {
    HBRUSH hBrush;
    COLORREF textColor;

    // Set text color based on button state (pressed, hovered, or normal)
    if (lpdis->itemState & ODS_SELECTED) {
        hBrush = CreateSolidBrush(accentDarkColor); // Darker on click
        textColor = RGB(255, 255, 255); // White text on dark accent
    } else if (lpdis->itemState & ODS_HOTLIGHT) {
        hBrush = CreateSolidBrush(accentLightColor); // Lighter on hover
        textColor = RGB(255, 255, 255); // White text on light accent
    } else {
        hBrush = CreateSolidBrush(accentColor); // Standard accent color
        textColor = RGB(255, 255, 255); // White text on accent
    }

    FillRect(lpdis->hDC, &lpdis->rcItem, hBrush); // Fill button background
    DeleteObject(hBrush);

    // Draw button text
    SetBkMode(lpdis->hDC, TRANSPARENT);
    SetTextColor(lpdis->hDC, textColor);
    
    // Get button text
    char buttonText[256];
    GetWindowText(lpdis->hwndItem, buttonText, sizeof(buttonText));

    // Draw text centered
    DrawText(lpdis->hDC, buttonText, -1, &lpdis->rcItem, DT_SINGLELINE | DT_CENTER | DT_VCENTER);

    // Draw focus rectangle if button has focus
    if (lpdis->itemState & ODS_FOCUS) {
        DrawFocusRect(lpdis->hDC, &lpdis->rcItem);
    }
}

// Window procedures
LRESULT CALLBACK MainWindowProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp) {
    switch (msg) {
        case WM_CREATE:
            showMainMenu(hwnd);
            break;
            
        case WM_COMMAND:
            switch (LOWORD(wp)) {
                case 1: // Book Management
                    clearRightPanel(hwnd);
                    showBookManagement(hwnd);
                    InvalidateRect(hwnd, NULL, TRUE);
                    UpdateWindow(hwnd);
                    break;
                case 2: // Author Management
                    clearRightPanel(hwnd);
                    showAuthorManagement(hwnd);
                    InvalidateRect(hwnd, NULL, TRUE);
                    UpdateWindow(hwnd);
                    break;
                case 3: // Publisher Management
                    clearRightPanel(hwnd);
                    showPublisherManagement(hwnd);
                    InvalidateRect(hwnd, NULL, TRUE);
                    UpdateWindow(hwnd);
                    break;
                case 4: // Member Management
                    clearRightPanel(hwnd);
                    showMemberManagement(hwnd);
                    InvalidateRect(hwnd, NULL, TRUE);
                    UpdateWindow(hwnd);
                    break;
                case 5: // Staff Management
                    clearRightPanel(hwnd);
                    showStaffManagement(hwnd);
                    InvalidateRect(hwnd, NULL, TRUE);
                    UpdateWindow(hwnd);
                    break;
                case 6: // Borrowing Management
                    clearRightPanel(hwnd);
                    showBorrowingManagement(hwnd);
                    InvalidateRect(hwnd, NULL, TRUE);
                    UpdateWindow(hwnd);
                    break;
                case 7: // Fine Management
                    clearRightPanel(hwnd);
                    showFineManagement(hwnd);
                    InvalidateRect(hwnd, NULL, TRUE);
                    UpdateWindow(hwnd);
                    break;

                // Book Management Buttons
                case 201: { // Add Book
                    Book book = getBookFromInputs(hwnd);
                    if (!isEmpty(book.title) && book.author_id > 0 && book.publisher_id > 0 && 
                        !isEmpty(book.isbn) && book.copies_available >= 0 && book.year_published != 0) {
                        addBook(book);
                        refreshBookList(hwnd);
                        clearBookInputs(hwnd);
                    } else {
                        MessageBox(hwnd, "Please fill in all book fields correctly.", "Input Error", MB_OK | MB_ICONERROR);
                    }
                        break;
                    }
                case 202: { // Update Book
                    Book book = getBookFromInputs(hwnd);
                    if (book.book_id > 0 && !isEmpty(book.title) && book.author_id > 0 && book.publisher_id > 0 && 
                        !isEmpty(book.isbn) && book.copies_available >= 0 && book.year_published != 0) {
                        updateBook(book);
                        refreshBookList(hwnd);
                        clearBookInputs(hwnd);
                    } else {
                        MessageBox(hwnd, "Please provide a valid Book ID and all other fields for update.", "Input Error", MB_OK | MB_ICONERROR);
                    }
                        break;
                    }
                case 203: { // Delete Book
                    char buffer[256];
                    GetWindowText(GetDlgItem(hwnd, 200), buffer, sizeof(buffer));
                    int book_id = atoi(buffer);
                    if (book_id > 0) {
                        deleteBook(book_id);
                            refreshBookList(hwnd);
                        clearBookInputs(hwnd);
                        } else {
                        MessageBox(hwnd, "Please enter a valid Book ID to delete.", "Input Error", MB_OK | MB_ICONERROR);
                }
                break;
                }
                case 204: { // Clear Book Fields
                    clearBookInputs(hwnd);
                    break;
                }
                case 205: { // Back to Main Menu from Book Management
                    clearWindow(hwnd);
                    showMainMenu(hwnd);
                    InvalidateRect(hwnd, NULL, TRUE);
                    UpdateWindow(hwnd);
                        break;
                    }

                // Author Management Buttons
                case 301: { // Add Author
                    Author author = getAuthorFromInputs(hwnd);
                    if (!isEmpty(author.name)) {
                        addAuthor(author);
                        refreshAuthorList(hwnd);
                        clearAuthorInputs(hwnd);
                    } else {
                        MessageBox(hwnd, "Please enter the author's name.", "Input Error", MB_OK | MB_ICONERROR);
                    }
                        break;
                    }
                case 302: { // Update Author
                    Author author = getAuthorFromInputs(hwnd);
                    if (author.author_id > 0 && !isEmpty(author.name)) {
                        updateAuthor(author);
                        refreshAuthorList(hwnd);
                        clearAuthorInputs(hwnd);
                    } else {
                        MessageBox(hwnd, "Please provide a valid Author ID and name for update.", "Input Error", MB_OK | MB_ICONERROR);
                    }
                        break;
                    }
                case 303: { // Delete Author
                    char buffer[256];
                    GetWindowText(GetDlgItem(hwnd, 300), buffer, sizeof(buffer));
                    int author_id = atoi(buffer);
                    if (author_id > 0) {
                        deleteAuthor(author_id);
                            refreshAuthorList(hwnd);
                        clearAuthorInputs(hwnd);
                        } else {
                        MessageBox(hwnd, "Please enter a valid Author ID to delete.", "Input Error", MB_OK | MB_ICONERROR);
                }
                break;
                }
                case 304: { // Clear Author Fields
                    clearAuthorInputs(hwnd);
                    break;
                }
                case 305: { // Back to Main Menu from Author Management
                    clearWindow(hwnd);
                    showMainMenu(hwnd);
                    InvalidateRect(hwnd, NULL, TRUE);
                    UpdateWindow(hwnd);
                    break;
                }

                // Publisher Management Buttons
                case 401: { // Add Publisher
                        Publisher publisher = getPublisherFromInputs(hwnd);
                    if (!isEmpty(publisher.name)) {
                        addPublisher(publisher);
                            refreshPublisherList(hwnd);
                            clearPublisherInputs(hwnd);
                        } else {
                        MessageBox(hwnd, "Please enter the publisher\'s name.", "Input Error", MB_OK | MB_ICONERROR);
                    }
                    break;
                }
                case 402: { // Update Publisher
                            Publisher publisher = getPublisherFromInputs(hwnd);
                    if (publisher.publisher_id > 0 && !isEmpty(publisher.name)) {
                        updatePublisher(publisher);
                                refreshPublisherList(hwnd);
                                clearPublisherInputs(hwnd);
                            } else {
                        MessageBox(hwnd, "Please provide a valid Publisher ID and name for update.", "Input Error", MB_OK | MB_ICONERROR);
                    }
                    break;
                }
                case 403: { // Delete Publisher
                            char buffer[256];
                    GetWindowText(GetDlgItem(hwnd, 400), buffer, sizeof(buffer));
                    int publisher_id = atoi(buffer);
                    if (publisher_id > 0) {
                        deletePublisher(publisher_id);
                                refreshPublisherList(hwnd);
                                clearPublisherInputs(hwnd);
                            } else {
                        MessageBox(hwnd, "Please enter a valid Publisher ID to delete.", "Input Error", MB_OK | MB_ICONERROR);
                    }
                    break;
                }
                case 404: { // Clear Publisher Fields
                    clearPublisherInputs(hwnd);
                    break;
                }
                case 405: { // Back to Main Menu from Publisher Management
                    clearWindow(hwnd);
                    showMainMenu(hwnd);
                    InvalidateRect(hwnd, NULL, TRUE);
                    UpdateWindow(hwnd);
                    break;
                }

                // Member Management Buttons
                case 501: { // Add Member
                    Member member = getMemberFromInputs(hwnd);
                    if (!isEmpty(member.member_name) && !isEmpty(member.address) && 
                        !isEmpty(member.phone_number) && !isEmpty(member.email)) {
                        addMember(member.member_name, member.address, member.phone_number, member.email);
                        refreshMemberList(hwnd);
                        clearMemberInputs(hwnd);
                    } else {
                        MessageBox(hwnd, "Please fill in all member fields.", "Input Error", MB_OK | MB_ICONERROR);
                    }
                        break;
                    }
                case 502: { // Update Member
                    Member member = getMemberFromInputs(hwnd);
                    if (member.member_id > 0 && !isEmpty(member.member_name) && 
                        !isEmpty(member.address) && !isEmpty(member.phone_number) && !isEmpty(member.email)) {
                        updateMember(member.member_id, member.member_name, member.address, member.phone_number, member.email);
                        refreshMemberList(hwnd);
                        clearMemberInputs(hwnd);
                    } else {
                        MessageBox(hwnd, "Please provide a valid Member ID and all other fields for update.", "Input Error", MB_OK | MB_ICONERROR);
                    }
                        break;
                    }
                case 503: { // Delete Member
                    char buffer[256];
                    GetWindowText(GetDlgItem(hwnd, 500), buffer, sizeof(buffer));
                    int member_id = atoi(buffer);
                    if (member_id > 0) {
                        deleteMember(member_id);
                            refreshMemberList(hwnd);
                        clearMemberInputs(hwnd);
                        } else {
                        MessageBox(hwnd, "Please enter a valid Member ID to delete.", "Input Error", MB_OK | MB_ICONERROR);
                }
                break;
                }
                case 504: { // Clear Member Fields
                    clearMemberInputs(hwnd);
                    break;
                }
                case 505: { // Back to Main Menu from Member Management
                    clearWindow(hwnd);
                    showMainMenu(hwnd);
                    InvalidateRect(hwnd, NULL, TRUE);
                    UpdateWindow(hwnd);
                    break;
                }

                // Staff Management Buttons
                case 601: { // Add Staff
                    Staff staff = getStaffFromInputs(hwnd);
                    if (!isEmpty(staff.staff_name) && !isEmpty(staff.role) && 
                        !isEmpty(staff.phone_number) && !isEmpty(staff.email)) {
                        addStaff(staff.staff_name, staff.role, staff.phone_number, staff.email);
                        refreshStaffList(hwnd);
                        clearStaffInputs(hwnd);
                    } else {
                        MessageBox(hwnd, "Please fill in all staff fields.", "Input Error", MB_OK | MB_ICONERROR);
                    }
                        break;
                    }
                case 602: { // Update Staff
                    Staff staff = getStaffFromInputs(hwnd);
                    if (staff.staff_id > 0 && !isEmpty(staff.staff_name) && !isEmpty(staff.role) && 
                        !isEmpty(staff.phone_number) && !isEmpty(staff.email)) {
                        updateStaff(staff.staff_id, staff.staff_name, staff.role, staff.phone_number, staff.email);
                        refreshStaffList(hwnd);
                        clearStaffInputs(hwnd);
                    } else {
                        MessageBox(hwnd, "Please provide a valid Staff ID and all other fields for update.", "Input Error", MB_OK | MB_ICONERROR);
                    }
                        break;
                    }
                case 603: { // Delete Staff
                    char buffer[256];
                    GetWindowText(GetDlgItem(hwnd, 600), buffer, sizeof(buffer));
                    int staff_id = atoi(buffer);
                    if (staff_id > 0) {
                        deleteStaff(staff_id);
                            refreshStaffList(hwnd);
                        clearStaffInputs(hwnd);
                        } else {
                        MessageBox(hwnd, "Please enter a valid Staff ID to delete.", "Input Error", MB_OK | MB_ICONERROR);
                }
                break;
                }
                case 604: { // Clear Staff Fields
                    clearStaffInputs(hwnd);
                    break;
                }
                case 605: { // Back to Main Menu from Staff Management
                    clearWindow(hwnd);
                    showMainMenu(hwnd);
                    InvalidateRect(hwnd, NULL, TRUE);
                    UpdateWindow(hwnd);
                    break;
                }

                // Borrowing Management Buttons
                case 701: { // Add Borrowing
                    Borrowing borrowing = getBorrowingFromInputs(hwnd);
                    if (borrowing.book_id > 0 && borrowing.member_id > 0 && 
                        !isEmpty(borrowing.borrow_date) && isValidDateFormat(borrowing.borrow_date)) {
                        addBorrowing(borrowing.book_id, borrowing.member_id, borrowing.borrow_date);
                        refreshBorrowingList(hwnd);
                        clearBorrowingInputs(hwnd);
                    } else {
                        MessageBox(hwnd, "Please fill in all borrowing fields correctly. Dates should be YYYY-MM-DD.", "Input Error", MB_OK | MB_ICONERROR);
                    }
                        break;
                    }
                case 702: { // Update Borrowing
                    Borrowing borrowing = getBorrowingFromInputs(hwnd);
                    if (borrowing.borrow_id > 0 && borrowing.book_id > 0 && borrowing.member_id > 0 && 
                        !isEmpty(borrowing.borrow_date) && isValidDateFormat(borrowing.borrow_date) &&
                        (!isEmpty(borrowing.return_date) ? isValidDateFormat(borrowing.return_date) : TRUE)) {
                        updateBorrowing(borrowing.borrow_id, borrowing.book_id, borrowing.member_id, 
                                        borrowing.borrow_date, !isEmpty(borrowing.return_date) ? borrowing.return_date : NULL);
                            refreshBorrowingList(hwnd);
                        clearBorrowingInputs(hwnd);
                        } else {
                        MessageBox(hwnd, "Please provide valid Borrow ID and all other fields for update. Dates should be YYYY-MM-DD.", "Input Error", MB_OK | MB_ICONERROR);
                    }
                        break;
                    }
                case 703: { // Delete Borrowing
                    char buffer[256];
                    GetWindowText(GetDlgItem(hwnd, 700), buffer, sizeof(buffer));
                    int borrow_id = atoi(buffer);
                    if (borrow_id > 0) {
                        deleteBorrowing(borrow_id);
                            refreshBorrowingList(hwnd);
                        clearBorrowingInputs(hwnd);
                        } else {
                        MessageBox(hwnd, "Please enter a valid Borrow ID to delete.", "Input Error", MB_OK | MB_ICONERROR);
                }
                break;
                }
                case 704: { // Clear Borrowing Fields
                    clearBorrowingInputs(hwnd);
                    break;
                }
                case 705: { // Back to Main Menu from Borrowing Management
                    clearWindow(hwnd);
                    showMainMenu(hwnd);
                    InvalidateRect(hwnd, NULL, TRUE);
                    UpdateWindow(hwnd);
                    break;
                }

                // Fine Management Buttons
                case 801: { // Add Fine
                    Fine fine = getFineFromInputs(hwnd);
                    if (fine.borrow_id > 0 && fine.amount >= 0) {
                        addFine(fine.borrow_id, fine.amount);
                        refreshFineList(hwnd);
                        clearFineInputs(hwnd);
                    } else {
                        MessageBox(hwnd, "Please fill in all fine fields correctly.", "Input Error", MB_OK | MB_ICONERROR);
                    }
                        break;
                    }
                case 802: { // Update Fine
                    Fine fine = getFineFromInputs(hwnd);
                    if (fine.fine_id > 0 && fine.borrow_id > 0 && fine.amount >= 0) {
                        updateFine(fine.fine_id, fine.borrow_id, fine.amount, fine.is_paid);
                            refreshFineList(hwnd);
                        clearFineInputs(hwnd);
                        } else {
                        MessageBox(hwnd, "Please provide a valid Fine ID and all other fields for update.", "Input Error", MB_OK | MB_ICONERROR);
                    }
                        break;
                    }
                case 803: { // Delete Fine
                    char buffer[256];
                    GetWindowText(GetDlgItem(hwnd, 800), buffer, sizeof(buffer));
                    int fine_id = atoi(buffer);
                    if (fine_id > 0) {
                        deleteFine(fine_id);
                            refreshFineList(hwnd);
                        clearFineInputs(hwnd);
                        } else {
                        MessageBox(hwnd, "Please enter a valid Fine ID to delete.", "Input Error", MB_OK | MB_ICONERROR);
                }
                break;
                }
                case 804: { // Clear Fine Fields
                    clearFineInputs(hwnd);
                    break;
                }
                case 805: { // Back to Main Menu from Fine Management
                    clearWindow(hwnd);
                    showMainMenu(hwnd);
                    InvalidateRect(hwnd, NULL, TRUE);
                    UpdateWindow(hwnd);
                    break;
            }
            }
            break;
            
        case WM_NOTIFY: {
            LPNMHDR lpnmh = (LPNMHDR)lp;
            if (lpnmh->code == LVN_ITEMCHANGED) {
                LPNMLISTVIEW lpnmlv = (LPNMLISTVIEW)lp;
                if (lpnmlv->uNewState & LVIS_SELECTED) {
                    if (lpnmh->idFrom == 212) { // Book List
                        int selectedIndex = lpnmlv->iItem;
                            char buffer[256];

                        ListView_GetItemText(lpnmh->hwndFrom, selectedIndex, 0, buffer, sizeof(buffer));
                        SetWindowText(GetDlgItem(hwnd, 200), buffer); // ID

                        ListView_GetItemText(lpnmh->hwndFrom, selectedIndex, 1, buffer, sizeof(buffer));
                        SetWindowText(GetDlgItem(hwnd, 206), buffer); // Title

                        ListView_GetItemText(lpnmh->hwndFrom, selectedIndex, 2, buffer, sizeof(buffer));
                        SetWindowText(GetDlgItem(hwnd, 207), buffer); // Author ID

                        ListView_GetItemText(lpnmh->hwndFrom, selectedIndex, 3, buffer, sizeof(buffer));
                        SetWindowText(GetDlgItem(hwnd, 208), buffer); // Publisher ID

                        ListView_GetItemText(lpnmh->hwndFrom, selectedIndex, 4, buffer, sizeof(buffer));
                        SetWindowText(GetDlgItem(hwnd, 209), buffer); // ISBN

                        ListView_GetItemText(lpnmh->hwndFrom, selectedIndex, 5, buffer, sizeof(buffer));
                        SetWindowText(GetDlgItem(hwnd, 210), buffer); // Quantity

                        ListView_GetItemText(lpnmh->hwndFrom, selectedIndex, 6, buffer, sizeof(buffer));
                        SetWindowText(GetDlgItem(hwnd, 211), buffer); // Published Date
                    } else if (lpnmh->idFrom == 307) { // Author List
                        int selectedIndex = lpnmlv->iItem;
            char buffer[256];
                        ListView_GetItemText(lpnmh->hwndFrom, selectedIndex, 0, buffer, sizeof(buffer));
                        SetWindowText(GetDlgItem(hwnd, 300), buffer); // ID
                        ListView_GetItemText(lpnmh->hwndFrom, selectedIndex, 1, buffer, sizeof(buffer));
                        SetWindowText(GetDlgItem(hwnd, 306), buffer); // Name
                    } else if (lpnmh->idFrom == 407) { // Publisher List
                        int selectedIndex = lpnmlv->iItem;
            char buffer[256];
                        ListView_GetItemText(lpnmh->hwndFrom, selectedIndex, 0, buffer, sizeof(buffer));
                        SetWindowText(GetDlgItem(hwnd, 400), buffer); // ID
                        ListView_GetItemText(lpnmh->hwndFrom, selectedIndex, 1, buffer, sizeof(buffer));
                        SetWindowText(GetDlgItem(hwnd, 406), buffer); // Name
                    } else if (lpnmh->idFrom == 510) { // Member List
                        int selectedIndex = lpnmlv->iItem;
            char buffer[256];
                        ListView_GetItemText(lpnmh->hwndFrom, selectedIndex, 0, buffer, sizeof(buffer));
                        SetWindowText(GetDlgItem(hwnd, 500), buffer); // ID
                        ListView_GetItemText(lpnmh->hwndFrom, selectedIndex, 1, buffer, sizeof(buffer));
                        SetWindowText(GetDlgItem(hwnd, 506), buffer); // Name
                        ListView_GetItemText(lpnmh->hwndFrom, selectedIndex, 2, buffer, sizeof(buffer));
                        SetWindowText(GetDlgItem(hwnd, 507), buffer); // Address
                        ListView_GetItemText(lpnmh->hwndFrom, selectedIndex, 3, buffer, sizeof(buffer));
                        SetWindowText(GetDlgItem(hwnd, 508), buffer); // Phone
                        ListView_GetItemText(lpnmh->hwndFrom, selectedIndex, 4, buffer, sizeof(buffer));
                        SetWindowText(GetDlgItem(hwnd, 509), buffer); // Email
                    } else if (lpnmh->idFrom == 610) { // Staff List
                        int selectedIndex = lpnmlv->iItem;
            char buffer[256];
                        ListView_GetItemText(lpnmh->hwndFrom, selectedIndex, 0, buffer, sizeof(buffer));
                        SetWindowText(GetDlgItem(hwnd, 600), buffer); // ID
                        ListView_GetItemText(lpnmh->hwndFrom, selectedIndex, 1, buffer, sizeof(buffer));
                        SetWindowText(GetDlgItem(hwnd, 606), buffer); // Name
                        ListView_GetItemText(lpnmh->hwndFrom, selectedIndex, 2, buffer, sizeof(buffer));
                        SetWindowText(GetDlgItem(hwnd, 607), buffer); // Role
                        ListView_GetItemText(lpnmh->hwndFrom, selectedIndex, 3, buffer, sizeof(buffer));
                        SetWindowText(GetDlgItem(hwnd, 608), buffer); // Phone
                        ListView_GetItemText(lpnmh->hwndFrom, selectedIndex, 4, buffer, sizeof(buffer));
                        SetWindowText(GetDlgItem(hwnd, 609), buffer); // Email
                    } else if (lpnmh->idFrom == 710) { // Borrowing List
                        int selectedIndex = lpnmlv->iItem;
                        char buffer[256];
                        ListView_GetItemText(lpnmh->hwndFrom, selectedIndex, 0, buffer, sizeof(buffer));
                        SetWindowText(GetDlgItem(hwnd, 700), buffer); // Borrow ID
                        ListView_GetItemText(lpnmh->hwndFrom, selectedIndex, 1, buffer, sizeof(buffer));
                        SetWindowText(GetDlgItem(hwnd, 706), buffer); // Book ID
                        ListView_GetItemText(lpnmh->hwndFrom, selectedIndex, 2, buffer, sizeof(buffer));
                        SetWindowText(GetDlgItem(hwnd, 707), buffer); // Member ID
                        ListView_GetItemText(lpnmh->hwndFrom, selectedIndex, 3, buffer, sizeof(buffer));
                        SetWindowText(GetDlgItem(hwnd, 708), buffer); // Borrow Date
                        ListView_GetItemText(lpnmh->hwndFrom, selectedIndex, 4, buffer, sizeof(buffer));
                        SetWindowText(GetDlgItem(hwnd, 709), buffer); // Return Date
                    } else if (lpnmh->idFrom == 809) { // Fine List
                        int selectedIndex = lpnmlv->iItem;
                        char buffer[256];
                        ListView_GetItemText(lpnmh->hwndFrom, selectedIndex, 0, buffer, sizeof(buffer));
                        SetWindowText(GetDlgItem(hwnd, 800), buffer); // Fine ID
                        ListView_GetItemText(lpnmh->hwndFrom, selectedIndex, 1, buffer, sizeof(buffer));
                        SetWindowText(GetDlgItem(hwnd, 806), buffer); // Borrow ID
                        ListView_GetItemText(lpnmh->hwndFrom, selectedIndex, 2, buffer, sizeof(buffer));
                        SetWindowText(GetDlgItem(hwnd, 807), buffer); // Amount
                        ListView_GetItemText(lpnmh->hwndFrom, selectedIndex, 3, buffer, sizeof(buffer));
                        SetWindowText(GetDlgItem(hwnd, 808), buffer); // Paid
                    }
                }
            }
            break;
        }

        case WM_DRAWITEM: {
            LPDRAWITEMSTRUCT lpdis = (LPDRAWITEMSTRUCT)lp;
            if (lpdis->CtlType == ODT_BUTTON) {
                drawButton(lpdis, g_crAccent, g_crAccentDark, g_crAccentLight);
            }
            return TRUE;
        }

        case WM_CTLCOLORSTATIC: {
            HDC hdcStatic = (HDC)wp;
            SetTextColor(hdcStatic, g_crText);
            SetBkColor(hdcStatic, g_crBackground);
            return (LRESULT)GetStockObject(NULL_BRUSH); // Return a null brush to make background transparent
        }

        case WM_SIZE: {
            // Reposition and resize controls based on window size
            // This is a simplified example, you might need to implement more complex logic
            // InvalidateRect(hwnd, NULL, TRUE); // Request repaint
            break;
        }

        case WM_DESTROY:
            // Clean up font handles
            if (hFontDashboardTitle) DeleteObject(hFontDashboardTitle);
            if (hFontCardCount) DeleteObject(hFontCardCount);
            if (hFontCardTitle) DeleteObject(hFontCardTitle);
            PostQuitMessage(0);
            break;

        default:
            return DefWindowProc(hwnd, msg, wp, lp);
    }
    return 0;
}

// Utility function to check if a string is empty
BOOL isEmpty(const char* str) {
    return str == NULL || strlen(str) == 0;
} 