#include <windows.h>
#include <commctrl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sqlite3.h>
#include <uxtheme.h>
#include <commdlg.h>
#include <time.h>
#include "structures.h"
#include "library_crud.h"
#include "sqlite_db.h"
#include "resource.h"

int sql_initDatabase();
void sql_closeDatabase();
Book* sql_getAllBooks(sqlite3* db, int* count);
Author* sql_getAllAuthors(sqlite3* db, int* count);
Publisher* sql_getAllPublishers(sqlite3* db, int* count);
Member* sql_getAllMembers(sqlite3* db, int* count);
Staff* sql_getAllStaff(sqlite3* db, int* count);

// #include "library_crud.c" // Removed to avoid multiple definition errors
#include "book_dialogs.h"
// #include "author_dialogs.h" // Removed because file does not exist
#include "publisher_dialogs.h"
#include "member_dialogs.h"
#include "staff_dialogs.h"

// Window dimensions
#define WINDOW_WIDTH 1024
#define WINDOW_HEIGHT 768

// Colors
#define GUI_COLOR_BACKGROUND RGB(240, 240, 240)
#define GUI_COLOR_HEADER RGB(51, 51, 51)
#define GUI_COLOR_TEXT RGB(33, 33, 33)
#define GUI_COLOR_ACCENT RGB(0, 120, 215)
#define GUI_COLOR_BUTTON RGB(0, 120, 215)
#define GUI_COLOR_BUTTON_HOVER RGB(0, 99, 177)
#define GUI_COLOR_BUTTON_TEXT RGB(255, 255, 255)
#define GUI_COLOR_LIST_HEADER RGB(240, 240, 240)
#define GUI_COLOR_LIST_HEADER_TEXT RGB(51, 51, 51)
#define GUI_COLOR_LIST_ITEM_SELECTED RGB(0, 120, 215)
#define GUI_COLOR_LIST_ITEM_SELECTED_TEXT RGB(255, 255, 255)
#define GUI_COLOR_LIST_ITEM_HOVER RGB(230, 230, 230)

// Control IDs
#define ID_TAB_CONTROL 101
#define ID_BOOKS_TAB 102
#define ID_AUTHORS_TAB 103
#define ID_PUBLISHERS_TAB 104
#define ID_MEMBERS_TAB 105
#define ID_STAFF_TAB 106
#define ID_BORROWINGS_TAB 107
#define ID_FINES_TAB 108

// Book controls
#define ID_BOOK_LIST 201
#define ID_BOOK_ADD 202
#define ID_BOOK_EDIT 203
#define ID_BOOK_DELETE 204
#define ID_BOOK_SEARCH 205

// Author controls
#define ID_AUTHOR_LIST 301
#define ID_AUTHOR_ADD 302
#define ID_AUTHOR_EDIT 303
#define ID_AUTHOR_DELETE 304
#define ID_AUTHOR_SEARCH 305

// Publisher controls
#define ID_PUBLISHER_LIST 401
#define ID_PUBLISHER_ADD 402
#define ID_PUBLISHER_EDIT 403
#define ID_PUBLISHER_DELETE 404
#define ID_PUBLISHER_SEARCH 405

// Member controls
#define ID_MEMBER_LIST 501
#define ID_MEMBER_ADD 502
#define ID_MEMBER_EDIT 503
#define ID_MEMBER_DELETE 504
#define ID_MEMBER_SEARCH 505

// Staff controls
#define ID_STAFF_LIST 601
#define ID_STAFF_ADD 602
#define ID_STAFF_EDIT 603
#define ID_STAFF_DELETE 604
#define ID_STAFF_SEARCH 605

// Borrowing controls
#define ID_BORROWING_LIST 701
#define ID_BORROWING_ADD 702
#define ID_BORROWING_RETURN 703
#define ID_BORROWING_SEARCH 704

// Fine controls
#define ID_FINE_LIST 801
#define ID_FINE_ADD 802
#define ID_FINE_PAY 803
#define ID_FINE_SEARCH 804

// Global variables
static HWND g_hwndMain = NULL;
static HWND g_hwndTab = NULL;
static HWND g_hwndBookList = NULL;
static HWND g_hwndAuthorList = NULL;
static HWND g_hwndPublisherList = NULL;
static HWND g_hwndMemberList = NULL;
static HWND g_hwndStaffList = NULL;
static HWND g_hwndBorrowingList = NULL;
static HWND g_hwndFineList = NULL;
sqlite3* g_db;

// Data arrays
Book* g_bookDataArray = NULL;
int g_bookDataCount = 0;
Author* g_authorDataArray = NULL;
int g_authorDataCount = 0;
Publisher* g_publisherDataArray = NULL;
int g_publisherDataCount = 0;
Member* g_memberDataArray = NULL;
int g_memberDataCount = 0;
Staff* g_staffDataArray = NULL;
int g_staffDataCount = 0;

// Function prototypes
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
void CreateControls(HWND hwnd);
void InitializeDatabase();
void RefreshBookList();
void RefreshAuthorList();
void RefreshPublisherList();
void RefreshMemberList();
void RefreshStaffList();
void RefreshBorrowingList();
void RefreshFineList();

INT_PTR CALLBACK AddBookDialogProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
INT_PTR CALLBACK EditBookDialogProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
INT_PTR CALLBACK AddAuthorDialogProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
INT_PTR CALLBACK EditAuthorDialogProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
INT_PTR CALLBACK AddPublisherDialogProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
INT_PTR CALLBACK EditPublisherDialogProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
INT_PTR CALLBACK AddMemberDialogProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
INT_PTR CALLBACK EditMemberDialogProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);

// Custom drawing function for list views
void CustomDrawListView(HWND hwnd, LPNMLVCUSTOMDRAW pLVCD) {
    switch (pLVCD->nmcd.dwDrawStage) {
        case CDDS_PREPAINT:
            SetWindowLongPtr(hwnd, DWLP_MSGRESULT, CDRF_NOTIFYITEMDRAW);
            return;
        case CDDS_ITEMPREPAINT:
            if (pLVCD->nmcd.dwItemSpec == ListView_GetNextItem(hwnd, -1, LVNI_SELECTED)) {
                pLVCD->clrText = GUI_COLOR_LIST_ITEM_SELECTED_TEXT;
                pLVCD->clrTextBk = GUI_COLOR_LIST_ITEM_SELECTED;
            } else {
                pLVCD->clrText = GUI_COLOR_TEXT;
                pLVCD->clrTextBk = GUI_COLOR_BACKGROUND;
            }
            SetWindowLongPtr(hwnd, DWLP_MSGRESULT, CDRF_NEWFONT);
            return;
        case CDDS_ITEMPOSTPAINT:
            SetWindowLongPtr(hwnd, DWLP_MSGRESULT, CDRF_DODEFAULT);
            return;
    }
}

// Custom drawing function for buttons
void CustomDrawButton(HWND hwnd, LPNMCUSTOMDRAW pNMCD) {
    switch (pNMCD->dwDrawStage) {
        case CDDS_PREPAINT:
            SetWindowLongPtr(hwnd, DWLP_MSGRESULT, CDRF_NOTIFYPOSTPAINT);
            return;
        case CDDS_POSTPAINT: {
            HDC hdc = pNMCD->hdc;
            RECT rc = pNMCD->rc;
            bool isHovered = (pNMCD->uItemState & CDIS_HOT) != 0;
            bool isPressed = (pNMCD->uItemState & CDIS_SELECTED) != 0;
            HBRUSH hBrush = CreateSolidBrush(isPressed ? GUI_COLOR_BUTTON_HOVER : isHovered ? GUI_COLOR_BUTTON_HOVER : GUI_COLOR_BUTTON);
            FillRect(hdc, &rc, hBrush);
            DeleteObject(hBrush);
            char text[256];
            GetWindowText(hwnd, text, sizeof(text));
            SetBkMode(hdc, TRANSPARENT);
            SetTextColor(hdc, GUI_COLOR_BUTTON_TEXT);
            DrawText(hdc, text, -1, &rc, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
            HPEN hPen = CreatePen(PS_SOLID, 1, GUI_COLOR_BUTTON_HOVER);
            HPEN hOldPen = SelectObject(hdc, hPen);
            Rectangle(hdc, rc.left, rc.top, rc.right, rc.bottom);
            SelectObject(hdc, hOldPen);
            DeleteObject(hPen);
            SetWindowLongPtr(hwnd, DWLP_MSGRESULT, CDRF_SKIPDEFAULT);
            return;
        }
    }
}

// Custom drawing function for tab control (basic, no LPNMTCCUSTOMDRAW)
void CustomDrawTabControl(HWND hwnd, LPNMCUSTOMDRAW pNMCD) {
    switch (pNMCD->dwDrawStage) {
        case CDDS_PREPAINT:
            SetWindowLongPtr(hwnd, DWLP_MSGRESULT, CDRF_NOTIFYITEMDRAW);
            return;
        case CDDS_ITEMPREPAINT: {
            BOOL isSelected = (pNMCD->dwItemSpec == TabCtrl_GetCurSel(hwnd));
            HBRUSH hBrush = CreateSolidBrush(isSelected ? GUI_COLOR_ACCENT : GUI_COLOR_BACKGROUND);
            FillRect(pNMCD->hdc, &pNMCD->rc, hBrush);
            DeleteObject(hBrush);
            char text[256];
            TCITEM tie;
            tie.mask = TCIF_TEXT;
            tie.pszText = text;
            tie.cchTextMax = sizeof(text);
            TabCtrl_GetItem(hwnd, pNMCD->dwItemSpec, &tie);
            SetBkMode(pNMCD->hdc, TRANSPARENT);
            SetTextColor(pNMCD->hdc, isSelected ? GUI_COLOR_BUTTON_TEXT : GUI_COLOR_TEXT);
            DrawText(pNMCD->hdc, text, -1, &pNMCD->rc, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
            SetWindowLongPtr(hwnd, DWLP_MSGRESULT, CDRF_SKIPDEFAULT);
            return;
        }
    }
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    // Initialize Common Controls
    INITCOMMONCONTROLSEX icex;
    icex.dwSize = sizeof(INITCOMMONCONTROLSEX);
    icex.dwICC = ICC_TAB_CLASSES | ICC_LISTVIEW_CLASSES;
    InitCommonControlsEx(&icex);

    // Register the window class
    const char CLASS_NAME[] = "Library Management System";
    
    WNDCLASS wc = {0};
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    
    RegisterClass(&wc);

    // Create the window
    HWND hwnd = CreateWindowEx(
        0,
        CLASS_NAME,
        "Library Management System",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, WINDOW_WIDTH, WINDOW_HEIGHT,
        NULL,
        NULL,
        hInstance,
        NULL
    );

    if (hwnd == NULL) {
        return 0;
    }

    // Initialize database
    if (!sql_initDatabase()) {
        MessageBox(NULL, "Failed to initialize database.", "Error", MB_ICONERROR);
        return 1;
    }

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
    if (g_bookDataArray) {
        free(g_bookDataArray);
        g_bookDataArray = NULL;
    }
    if (g_authorDataArray) {
        free(g_authorDataArray);
        g_authorDataArray = NULL;
    }
    if (g_publisherDataArray) {
        free(g_publisherDataArray);
        g_publisherDataArray = NULL;
    }
    if (g_memberDataArray) {
        free(g_memberDataArray);
        g_memberDataArray = NULL;
    }
    if (g_staffDataArray) {
        free(g_staffDataArray);
        g_staffDataArray = NULL;
    }
    sql_closeDatabase();

    return 0;
}

void InitializeDatabase() {
    int rc = sqlite3_open("library.db", &g_db);
    if (rc) {
        MessageBox(NULL, "Can't open database", "Error", MB_ICONERROR);
        exit(1);
    }
}

void CreateControls(HWND hwnd) {
    // Enable visual styles
    InitCommonControls();
    EnableThemeDialogTexture(hwnd, ETDT_ENABLETAB);

    // Create tab control with custom style
    g_hwndTab = CreateWindowEx(
        0,
        WC_TABCONTROL,
        "",
        WS_CHILD | WS_VISIBLE | TCS_FLATBUTTONS,
        10, 10, WINDOW_WIDTH - 20, WINDOW_HEIGHT - 20,
        hwnd,
        (HMENU)ID_TAB_CONTROL,
        NULL,
        NULL
    );

    // Add tabs
    TCITEM tie;
    tie.mask = TCIF_TEXT;

    tie.pszText = "Books";
    TabCtrl_InsertItem(g_hwndTab, 0, &tie);

    tie.pszText = "Authors";
    TabCtrl_InsertItem(g_hwndTab, 1, &tie);

    tie.pszText = "Publishers";
    TabCtrl_InsertItem(g_hwndTab, 2, &tie);

    tie.pszText = "Members";
    TabCtrl_InsertItem(g_hwndTab, 3, &tie);

    tie.pszText = "Staff";
    TabCtrl_InsertItem(g_hwndTab, 4, &tie);

    tie.pszText = "Borrowings";
    TabCtrl_InsertItem(g_hwndTab, 5, &tie);

    tie.pszText = "Fines";
    TabCtrl_InsertItem(g_hwndTab, 6, &tie);

    // Create list views with custom style
    g_hwndBookList = CreateWindowEx(
        WS_EX_CLIENTEDGE,
        WC_LISTVIEW,
        "",
        WS_CHILD | WS_VISIBLE | LVS_REPORT | LVS_SINGLESEL | LVS_SHOWSELALWAYS | LVS_OWNERDRAWFIXED,
        20, 40, WINDOW_WIDTH - 40, WINDOW_HEIGHT - 100,
        hwnd,
        (HMENU)ID_BOOK_LIST,
        NULL,
        NULL
    );

    // Set list view extended styles
    ListView_SetExtendedListViewStyle(g_hwndBookList, 
        LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES | LVS_EX_DOUBLEBUFFER);

    // Add columns to book list
    LVCOLUMN lvc;
    lvc.mask = LVCF_TEXT | LVCF_WIDTH;

    lvc.pszText = "ID";
    lvc.cx = 50;
    ListView_InsertColumn(g_hwndBookList, 0, &lvc);

    lvc.pszText = "Title";
    lvc.cx = 200;
    ListView_InsertColumn(g_hwndBookList, 1, &lvc);

    lvc.pszText = "Author";
    lvc.cx = 150;
    ListView_InsertColumn(g_hwndBookList, 2, &lvc);

    lvc.pszText = "Publisher";
    lvc.cx = 150;
    ListView_InsertColumn(g_hwndBookList, 3, &lvc);

    lvc.pszText = "ISBN";
    lvc.cx = 100;
    ListView_InsertColumn(g_hwndBookList, 4, &lvc);

    lvc.pszText = "Genre";
    lvc.cx = 100;
    ListView_InsertColumn(g_hwndBookList, 5, &lvc);

    lvc.pszText = "Year Published";
    lvc.cx = 100;
    ListView_InsertColumn(g_hwndBookList, 6, &lvc);

    lvc.pszText = "Copies Available";
    lvc.cx = 100;
    ListView_InsertColumn(g_hwndBookList, 7, &lvc);

    lvc.pszText = "Shelf Location";
    lvc.cx = 100;
    ListView_InsertColumn(g_hwndBookList, 8, &lvc);

    // Create buttons with custom style
    CreateWindow(
        "BUTTON",
        "Add Book",
        WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | BS_OWNERDRAW,
        20, WINDOW_HEIGHT - 50, 100, 30,
        hwnd,
        (HMENU)ID_BOOK_ADD,
        NULL,
        NULL
    );

    CreateWindow(
        "BUTTON",
        "Edit Book",
        WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | BS_OWNERDRAW,
        130, WINDOW_HEIGHT - 50, 100, 30,
        hwnd,
        (HMENU)ID_BOOK_EDIT,
        NULL,
        NULL
    );

    CreateWindow(
        "BUTTON",
        "Delete Book",
        WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | BS_OWNERDRAW,
        240, WINDOW_HEIGHT - 50, 100, 30,
        hwnd,
        (HMENU)ID_BOOK_DELETE,
        NULL,
        NULL
    );

    // Authors list
    g_hwndAuthorList = CreateWindowEx(
        WS_EX_CLIENTEDGE,
        WC_LISTVIEW,
        "",
        WS_CHILD | LVS_REPORT | LVS_SINGLESEL | LVS_SHOWSELALWAYS | LVS_OWNERDRAWFIXED,
        20, 40, WINDOW_WIDTH - 40, WINDOW_HEIGHT - 100,
        hwnd,
        (HMENU)ID_AUTHOR_LIST,
        NULL,
        NULL
    );

    // Set list view extended styles
    ListView_SetExtendedListViewStyle(g_hwndAuthorList, 
        LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES | LVS_EX_DOUBLEBUFFER);

    // Add columns to author list
    lvc.mask = LVCF_TEXT | LVCF_WIDTH;

    lvc.pszText = "ID";
    lvc.cx = 50;
    ListView_InsertColumn(g_hwndAuthorList, 0, &lvc);

    lvc.pszText = "Name";
    lvc.cx = 200;
    ListView_InsertColumn(g_hwndAuthorList, 1, &lvc);

    lvc.pszText = "Bio";
    lvc.cx = 400;
    ListView_InsertColumn(g_hwndAuthorList, 2, &lvc);

    // Create buttons with custom style
    CreateWindow(
        "BUTTON",
        "Add Author",
        WS_CHILD | BS_PUSHBUTTON | BS_OWNERDRAW,
        20, WINDOW_HEIGHT - 50, 100, 30,
        hwnd,
        (HMENU)ID_AUTHOR_ADD,
        NULL,
        NULL
    );

    CreateWindow(
        "BUTTON",
        "Edit Author",
        WS_CHILD | BS_PUSHBUTTON | BS_OWNERDRAW,
        130, WINDOW_HEIGHT - 50, 100, 30,
        hwnd,
        (HMENU)ID_AUTHOR_EDIT,
        NULL,
        NULL
    );

    CreateWindow(
        "BUTTON",
        "Delete Author",
        WS_CHILD | BS_PUSHBUTTON | BS_OWNERDRAW,
        240, WINDOW_HEIGHT - 50, 100, 30,
        hwnd,
        (HMENU)ID_AUTHOR_DELETE,
        NULL,
        NULL
    );

    // Publishers list
    g_hwndPublisherList = CreateWindowEx(
        WS_EX_CLIENTEDGE,
        WC_LISTVIEW,
        "",
        WS_CHILD | LVS_REPORT | LVS_SINGLESEL | LVS_SHOWSELALWAYS | LVS_OWNERDRAWFIXED,
        20, 40, WINDOW_WIDTH - 40, WINDOW_HEIGHT - 100,
        hwnd,
        (HMENU)ID_PUBLISHER_LIST,
        NULL,
        NULL
    );

    // Set list view extended styles
    ListView_SetExtendedListViewStyle(g_hwndPublisherList, 
        LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES | LVS_EX_DOUBLEBUFFER);

    // Add columns to publisher list
    lvc.mask = LVCF_TEXT | LVCF_WIDTH;

    lvc.pszText = "ID";
    lvc.cx = 50;
    ListView_InsertColumn(g_hwndPublisherList, 0, &lvc);

    lvc.pszText = "Name";
    lvc.cx = 200;
    ListView_InsertColumn(g_hwndPublisherList, 1, &lvc);

    lvc.pszText = "Address";
    lvc.cx = 250;
    ListView_InsertColumn(g_hwndPublisherList, 2, &lvc);

    lvc.pszText = "Contact Info";
    lvc.cx = 200;
    ListView_InsertColumn(g_hwndPublisherList, 3, &lvc);

    // Create buttons with custom style
    CreateWindow(
        "BUTTON",
        "Add Publisher",
        WS_CHILD | BS_PUSHBUTTON | BS_OWNERDRAW,
        20, WINDOW_HEIGHT - 50, 100, 30,
        hwnd,
        (HMENU)ID_PUBLISHER_ADD,
        NULL,
        NULL
    );

    CreateWindow(
        "BUTTON",
        "Edit Publisher",
        WS_CHILD | BS_PUSHBUTTON | BS_OWNERDRAW,
        130, WINDOW_HEIGHT - 50, 100, 30,
        hwnd,
        (HMENU)ID_PUBLISHER_EDIT,
        NULL,
        NULL
    );

    CreateWindow(
        "BUTTON",
        "Delete Publisher",
        WS_CHILD | BS_PUSHBUTTON | BS_OWNERDRAW,
        240, WINDOW_HEIGHT - 50, 100, 30,
        hwnd,
        (HMENU)ID_PUBLISHER_DELETE,
        NULL,
        NULL
    );

    // Members list
    g_hwndMemberList = CreateWindowEx(
        WS_EX_CLIENTEDGE,
        WC_LISTVIEW,
        "",
        WS_CHILD | LVS_REPORT | LVS_SINGLESEL | LVS_SHOWSELALWAYS | LVS_OWNERDRAWFIXED,
        20, 40, WINDOW_WIDTH - 40, WINDOW_HEIGHT - 100,
        hwnd,
        (HMENU)ID_MEMBER_LIST,
        NULL,
        NULL
    );

    // Set list view extended styles
    ListView_SetExtendedListViewStyle(g_hwndMemberList, 
        LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES | LVS_EX_DOUBLEBUFFER);

    // Add columns to member list
    lvc.mask = LVCF_TEXT | LVCF_WIDTH;

    lvc.pszText = "ID";
    lvc.cx = 50;
    ListView_InsertColumn(g_hwndMemberList, 0, &lvc);

    lvc.pszText = "Name";
    lvc.cx = 150;
    ListView_InsertColumn(g_hwndMemberList, 1, &lvc);

    lvc.pszText = "Address";
    lvc.cx = 150;
    ListView_InsertColumn(g_hwndMemberList, 2, &lvc);

    lvc.pszText = "Phone";
    lvc.cx = 100;
    ListView_InsertColumn(g_hwndMemberList, 3, &lvc);

    lvc.pszText = "Email";
    lvc.cx = 150;
    ListView_InsertColumn(g_hwndMemberList, 4, &lvc);

    lvc.pszText = "Date Joined";
    lvc.cx = 100;
    ListView_InsertColumn(g_hwndMemberList, 5, &lvc);

    lvc.pszText = "Membership Status";
    lvc.cx = 120;
    ListView_InsertColumn(g_hwndMemberList, 6, &lvc);

    // Create buttons with custom style
    CreateWindow(
        "BUTTON",
        "Add Member",
        WS_CHILD | BS_PUSHBUTTON | BS_OWNERDRAW,
        20, WINDOW_HEIGHT - 50, 100, 30,
        hwnd,
        (HMENU)ID_MEMBER_ADD,
        NULL,
        NULL
    );

    CreateWindow(
        "BUTTON",
        "Edit Member",
        WS_CHILD | BS_PUSHBUTTON | BS_OWNERDRAW,
        130, WINDOW_HEIGHT - 50, 100, 30,
        hwnd,
        (HMENU)ID_MEMBER_EDIT,
        NULL,
        NULL
    );

    CreateWindow(
        "BUTTON",
        "Delete Member",
        WS_CHILD | BS_PUSHBUTTON | BS_OWNERDRAW,
        240, WINDOW_HEIGHT - 50, 100, 30,
        hwnd,
        (HMENU)ID_MEMBER_DELETE,
        NULL,
        NULL
    );

    // Create staff list view
    g_hwndStaffList = CreateWindowEx(
        WS_EX_CLIENTEDGE,
        WC_LISTVIEW,
        "",
        WS_CHILD | WS_VISIBLE | LVS_REPORT | LVS_SINGLESEL | LVS_SHOWSELALWAYS,
        10, 50, WINDOW_WIDTH - 20, WINDOW_HEIGHT - 100,
        hwnd,
        (HMENU)ID_STAFF_LIST,
        GetModuleHandle(NULL),
        NULL
    );
    
    // Add columns to staff list view
    lvc.mask = LVCF_TEXT | LVCF_WIDTH | LVCF_SUBITEM;
    
    lvc.iSubItem = 0;
    lvc.pszText = "ID";
    lvc.cx = 50;
    ListView_InsertColumn(g_hwndStaffList, 0, &lvc);
    
    lvc.iSubItem = 1;
    lvc.pszText = "Name";
    lvc.cx = 200;
    ListView_InsertColumn(g_hwndStaffList, 1, &lvc);
    
    lvc.iSubItem = 2;
    lvc.pszText = "Role";
    lvc.cx = 150;
    ListView_InsertColumn(g_hwndStaffList, 2, &lvc);
    
    lvc.iSubItem = 3;
    lvc.pszText = "Email";
    lvc.cx = 200;
    ListView_InsertColumn(g_hwndStaffList, 3, &lvc);
    
    lvc.iSubItem = 4;
    lvc.pszText = "Phone";
    lvc.cx = 150;
    ListView_InsertColumn(g_hwndStaffList, 4, &lvc);
    
    // Create staff buttons
    CreateWindow(
        "BUTTON",
        "Add Staff",
        WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
        10, WINDOW_HEIGHT - 40, 100, 30,
        hwnd,
        (HMENU)ID_STAFF_ADD,
        GetModuleHandle(NULL),
        NULL
    );
    
    CreateWindow(
        "BUTTON",
        "Edit Staff",
        WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
        120, WINDOW_HEIGHT - 40, 100, 30,
        hwnd,
        (HMENU)ID_STAFF_EDIT,
        GetModuleHandle(NULL),
        NULL
    );
    
    CreateWindow(
        "BUTTON",
        "Delete Staff",
        WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
        230, WINDOW_HEIGHT - 40, 100, 30,
        hwnd,
        (HMENU)ID_STAFF_DELETE,
        GetModuleHandle(NULL),
        NULL
    );
    
    CreateWindow(
        "BUTTON",
        "Search Staff",
        WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
        340, WINDOW_HEIGHT - 40, 100, 30,
        hwnd,
        (HMENU)ID_STAFF_SEARCH,
        GetModuleHandle(NULL),
        NULL
    );

    // Set font for all controls
    HFONT hFont = CreateFont(16, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE,
        DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
        DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, "Segoe UI");
    
    SendMessage(g_hwndTab, WM_SETFONT, (WPARAM)hFont, TRUE);
    SendMessage(g_hwndBookList, WM_SETFONT, (WPARAM)hFont, TRUE);
    SendMessage(g_hwndAuthorList, WM_SETFONT, (WPARAM)hFont, TRUE);
    SendMessage(g_hwndPublisherList, WM_SETFONT, (WPARAM)hFont, TRUE);
    SendMessage(g_hwndMemberList, WM_SETFONT, (WPARAM)hFont, TRUE);
    SendMessage(g_hwndStaffList, WM_SETFONT, (WPARAM)hFont, TRUE);

    // Refresh the lists
    RefreshBookList();
    RefreshAuthorList();
    RefreshPublisherList();
    RefreshMemberList();
    RefreshStaffList();

    // Initially hide all other tab controls
    ShowWindow(g_hwndAuthorList, SW_HIDE);
    ShowWindow(GetDlgItem(hwnd, ID_AUTHOR_ADD), SW_HIDE);
    ShowWindow(GetDlgItem(hwnd, ID_AUTHOR_EDIT), SW_HIDE);
    ShowWindow(GetDlgItem(hwnd, ID_AUTHOR_DELETE), SW_HIDE);
    
    ShowWindow(g_hwndPublisherList, SW_HIDE);
    ShowWindow(GetDlgItem(hwnd, ID_PUBLISHER_ADD), SW_HIDE);
    ShowWindow(GetDlgItem(hwnd, ID_PUBLISHER_EDIT), SW_HIDE);
    ShowWindow(GetDlgItem(hwnd, ID_PUBLISHER_DELETE), SW_HIDE);

    ShowWindow(g_hwndMemberList, SW_HIDE);
    ShowWindow(GetDlgItem(hwnd, ID_MEMBER_ADD), SW_HIDE);
    ShowWindow(GetDlgItem(hwnd, ID_MEMBER_EDIT), SW_HIDE);
    ShowWindow(GetDlgItem(hwnd, ID_MEMBER_DELETE), SW_HIDE);

    ShowWindow(g_hwndStaffList, SW_HIDE);
    ShowWindow(GetDlgItem(hwnd, ID_STAFF_ADD), SW_HIDE);
    ShowWindow(GetDlgItem(hwnd, ID_STAFF_EDIT), SW_HIDE);
    ShowWindow(GetDlgItem(hwnd, ID_STAFF_DELETE), SW_HIDE);
    
    // TODO: Hide other list views and buttons for other tabs
}

void RefreshBookList() {
    int count;
    Book* books = sql_getAllBooks(g_db, &count);
    if (books) {
        g_bookDataArray = books;
        g_bookDataCount = count;
        // TODO: Update list view with books
    }
}

void RefreshAuthorList() {
    int count;
    Author* authors = sql_getAllAuthors(g_db, &count);
    if (authors) {
        g_authorDataArray = authors;
        g_authorDataCount = count;
        // TODO: Update list view with authors
    }
}

void RefreshPublisherList() {
    int count;
    Publisher* publishers = sql_getAllPublishers(g_db, &count);
    if (publishers) {
        g_publisherDataArray = publishers;
        g_publisherDataCount = count;
        // TODO: Update list view with publishers
    }
}

void RefreshMemberList() {
    int count;
    Member* members = sql_getAllMembers(g_db, &count);
    if (members) {
        g_memberDataArray = members;
        g_memberDataCount = count;
        // TODO: Update list view with members
    }
}

void RefreshStaffList() {
    int count;
    Staff* staff = sql_getAllStaff(g_db, &count);
    if (staff) {
        g_staffDataArray = staff;
        g_staffDataCount = count;
        // TODO: Update list view with staff
    }
}

void RefreshBorrowingList() { /* TODO */ }
void RefreshFineList() { /* TODO */ }

INT_PTR CALLBACK AddBookDialogProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam) {
    // ... existing code ...
}

INT_PTR CALLBACK EditBookDialogProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam) {
    // ... existing code ...
}

INT_PTR CALLBACK AddAuthorDialogProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam) {
    // ... existing code ...
}

INT_PTR CALLBACK EditAuthorDialogProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam) {
    // ... existing code ...
}

INT_PTR CALLBACK AddPublisherDialogProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam) {
    // ... existing code ...
}

INT_PTR CALLBACK EditPublisherDialogProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam) {
    // ... existing code ...
}

INT_PTR CALLBACK AddMemberDialogProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam) {
    // ... existing code ...
}

INT_PTR CALLBACK EditMemberDialogProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam) {
    // ... existing code ...
}

INT_PTR CALLBACK AddStaffDialogProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message) {
        case WM_INITDIALOG:
            return TRUE;
        case WM_COMMAND:
            if (LOWORD(wParam) == IDOK) {
                char name[256] = {0};
                char role[256] = {0};
                char email[256] = {0};
                char phone[256] = {0};
                GetDlgItemText(hDlg, IDC_STAFF_NAME, name, sizeof(name));
                GetDlgItemText(hDlg, IDC_STAFF_ROLE, role, sizeof(role));
                GetDlgItemText(hDlg, IDC_STAFF_EMAIL, email, sizeof(email));
                GetDlgItemText(hDlg, IDC_STAFF_PHONE, phone, sizeof(phone));
                if (strlen(name) == 0 || strlen(role) == 0) {
                    MessageBox(hDlg, "Name and role are required fields.", "Validation Error", MB_ICONERROR);
                    return TRUE;
                }
                sql_addStaff(g_db, name, role, email, phone);
                RefreshStaffList();
                EndDialog(hDlg, IDOK);
                return TRUE;
            }
            if (LOWORD(wParam) == IDCANCEL) {
                EndDialog(hDlg, IDCANCEL);
                return TRUE;
            }
            break;
    }
    return FALSE;
}

INT_PTR CALLBACK EditStaffDialogProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam) {
    static Staff* staff = NULL;
    switch (message) {
        case WM_INITDIALOG:
            staff = (Staff*)lParam;
            if (staff) {
                char id_str[32];
                sprintf(id_str, "%d", staff->staff_id);
                SetDlgItemText(hDlg, IDC_STAFF_ID, id_str);
                SetDlgItemText(hDlg, IDC_STAFF_NAME, staff->name);
                SetDlgItemText(hDlg, IDC_STAFF_ROLE, staff->role);
                SetDlgItemText(hDlg, IDC_STAFF_EMAIL, staff->email);
                SetDlgItemText(hDlg, IDC_STAFF_PHONE, staff->phone);
            }
            return TRUE;
        case WM_COMMAND:
            if (LOWORD(wParam) == IDOK) {
                char name[256] = {0};
                char role[256] = {0};
                char email[256] = {0};
                char phone[256] = {0};
                GetDlgItemText(hDlg, IDC_STAFF_NAME, name, sizeof(name));
                GetDlgItemText(hDlg, IDC_STAFF_ROLE, role, sizeof(role));
                GetDlgItemText(hDlg, IDC_STAFF_EMAIL, email, sizeof(email));
                GetDlgItemText(hDlg, IDC_STAFF_PHONE, phone, sizeof(phone));
                if (strlen(name) == 0 || strlen(role) == 0) {
                    MessageBox(hDlg, "Name and role are required fields.", "Validation Error", MB_ICONERROR);
                    return TRUE;
                }
                sql_updateStaff(g_db, staff->staff_id, name, role, email, phone);
                RefreshStaffList();
                EndDialog(hDlg, IDOK);
                return TRUE;
            }
            if (LOWORD(wParam) == IDCANCEL) {
                EndDialog(hDlg, IDCANCEL);
                return TRUE;
            }
            break;
    }
    return FALSE;
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
        case WM_COMMAND:
            switch (LOWORD(wParam)) {
                // ... existing code for book commands ...
                // ... existing code for author commands ...
                // ... existing code for publisher commands ...
                // ... existing code for member commands ...
                case ID_STAFF_ADD:
                    DialogBox(GetModuleHandle(NULL), MAKEINTRESOURCE(IDD_ADD_STAFF), hwnd, AddStaffDialogProc);
                    break;
                case ID_STAFF_EDIT: {
                    int selectedIndex = ListView_GetNextItem(g_hwndStaffList, -1, LVNI_SELECTED);
                    if (selectedIndex >= 0 && selectedIndex < g_staffDataCount) {
                        Staff* staff = &g_staffDataArray[selectedIndex];
                        DialogBoxParam(GetModuleHandle(NULL), MAKEINTRESOURCE(IDD_EDIT_STAFF), 
                                     hwnd, EditStaffDialogProc, (LPARAM)staff);
                    } else {
                        MessageBox(hwnd, "Please select a staff member to edit.", "No Selection", 
                                 MB_ICONINFORMATION);
                    }
                    break;
                }
                case ID_STAFF_DELETE: {
                    int selectedIndex = ListView_GetNextItem(g_hwndStaffList, -1, LVNI_SELECTED);
                    if (selectedIndex >= 0 && selectedIndex < g_staffDataCount) {
                        Staff* staff = &g_staffDataArray[selectedIndex];
                        int result = MessageBox(hwnd, 
                            "Are you sure you want to delete this staff member?",
                            "Confirm Delete", MB_YESNO | MB_ICONQUESTION);
                            
                        if (result == IDYES) {
                            sql_deleteStaff(g_db, staff->staff_id);
                            RefreshStaffList();
                        }
                    } else {
                        MessageBox(hwnd, "Please select a staff member to delete.", "No Selection", 
                                 MB_ICONINFORMATION);
                    }
                    break;
                }
            }
            break;
        case WM_NOTIFY:
            {
                LPNMHDR pnmh = (LPNMHDR)lParam;
                if (pnmh->code == TCN_SELCHANGE) {
                    int iSelTab = TabCtrl_GetCurSel(g_hwndTab);
                    // Hide all list views and buttons first
                    ShowWindow(g_hwndBookList, SW_HIDE);
                    ShowWindow(GetDlgItem(hwnd, ID_BOOK_ADD), SW_HIDE);
                    ShowWindow(GetDlgItem(hwnd, ID_BOOK_EDIT), SW_HIDE);
                    ShowWindow(GetDlgItem(hwnd, ID_BOOK_DELETE), SW_HIDE);
                    
                    ShowWindow(g_hwndAuthorList, SW_HIDE);
                    ShowWindow(GetDlgItem(hwnd, ID_AUTHOR_ADD), SW_HIDE);
                    ShowWindow(GetDlgItem(hwnd, ID_AUTHOR_EDIT), SW_HIDE);
                    ShowWindow(GetDlgItem(hwnd, ID_AUTHOR_DELETE), SW_HIDE);

                    ShowWindow(g_hwndPublisherList, SW_HIDE);
                    ShowWindow(GetDlgItem(hwnd, ID_PUBLISHER_ADD), SW_HIDE);
                    ShowWindow(GetDlgItem(hwnd, ID_PUBLISHER_EDIT), SW_HIDE);
                    ShowWindow(GetDlgItem(hwnd, ID_PUBLISHER_DELETE), SW_HIDE);

                    ShowWindow(g_hwndMemberList, SW_HIDE);
                    ShowWindow(GetDlgItem(hwnd, ID_MEMBER_ADD), SW_HIDE);
                    ShowWindow(GetDlgItem(hwnd, ID_MEMBER_EDIT), SW_HIDE);
                    ShowWindow(GetDlgItem(hwnd, ID_MEMBER_DELETE), SW_HIDE);

                    ShowWindow(g_hwndStaffList, SW_HIDE);
                    ShowWindow(GetDlgItem(hwnd, ID_STAFF_ADD), SW_HIDE);
                    ShowWindow(GetDlgItem(hwnd, ID_STAFF_EDIT), SW_HIDE);
                    ShowWindow(GetDlgItem(hwnd, ID_STAFF_DELETE), SW_HIDE);
                    
                    // TODO: Hide other list views and buttons for other tabs

                    // Show controls for the selected tab
                    switch (iSelTab) {
                        case 0: // Books tab
                            ShowWindow(g_hwndBookList, SW_SHOW);
                            ShowWindow(GetDlgItem(hwnd, ID_BOOK_ADD), SW_SHOW);
                            ShowWindow(GetDlgItem(hwnd, ID_BOOK_EDIT), SW_SHOW);
                            ShowWindow(GetDlgItem(hwnd, ID_BOOK_DELETE), SW_SHOW);
                            RefreshBookList();
                            break;
                        case 1: // Authors tab
                            ShowWindow(g_hwndAuthorList, SW_SHOW);
                            ShowWindow(GetDlgItem(hwnd, ID_AUTHOR_ADD), SW_SHOW);
                            ShowWindow(GetDlgItem(hwnd, ID_AUTHOR_EDIT), SW_SHOW);
                            ShowWindow(GetDlgItem(hwnd, ID_AUTHOR_DELETE), SW_SHOW);
                            RefreshAuthorList();
                            break;
                        case 2: // Publishers tab
                            ShowWindow(g_hwndPublisherList, SW_SHOW);
                            ShowWindow(GetDlgItem(hwnd, ID_PUBLISHER_ADD), SW_SHOW);
                            ShowWindow(GetDlgItem(hwnd, ID_PUBLISHER_EDIT), SW_SHOW);
                            ShowWindow(GetDlgItem(hwnd, ID_PUBLISHER_DELETE), SW_SHOW);
                            RefreshPublisherList();
                            break;
                        case 3: // Members tab
                            ShowWindow(g_hwndMemberList, SW_SHOW);
                            ShowWindow(GetDlgItem(hwnd, ID_MEMBER_ADD), SW_SHOW);
                            ShowWindow(GetDlgItem(hwnd, ID_MEMBER_EDIT), SW_SHOW);
                            ShowWindow(GetDlgItem(hwnd, ID_MEMBER_DELETE), SW_SHOW);
                            RefreshMemberList();
                            break;
                        case 4: // Staff tab
                            ShowWindow(g_hwndStaffList, SW_SHOW);
                            ShowWindow(GetDlgItem(hwnd, ID_STAFF_ADD), SW_SHOW);
                            ShowWindow(GetDlgItem(hwnd, ID_STAFF_EDIT), SW_SHOW);
                            ShowWindow(GetDlgItem(hwnd, ID_STAFF_DELETE), SW_SHOW);
                            RefreshStaffList();
                            break;
                        // TODO: Add cases for other tabs
                    }
                }
            }
            break;
        case WM_CTLCOLORSTATIC: {
            HDC hdcStatic = (HDC)wParam;
            SetTextColor(hdcStatic, GUI_COLOR_TEXT);
            SetBkColor(hdcStatic, GUI_COLOR_BACKGROUND);
            return (LRESULT)GetStockObject(NULL_BRUSH);
        }

        case WM_DRAWITEM: {
            LPDRAWITEMSTRUCT lpDIS = (LPDRAWITEMSTRUCT)lParam;
            if (lpDIS->CtlType == ODT_BUTTON) {
                CustomDrawButton(lpDIS->hwndItem, (LPNMCUSTOMDRAW)lpDIS);
                return TRUE;
            }
            break;
        }

        case WM_DESTROY:
            // Free the dynamically allocated data arrays
            if (g_bookDataArray) {
                free(g_bookDataArray);
                g_bookDataArray = NULL;
            }
            if (g_authorDataArray) {
                free(g_authorDataArray);
                g_authorDataArray = NULL;
            }
            if (g_publisherDataArray) {
                free(g_publisherDataArray);
                g_publisherDataArray = NULL;
            }
            if (g_memberDataArray) {
                free(g_memberDataArray);
                g_memberDataArray = NULL;
            }
            if (g_staffDataArray) {
                free(g_staffDataArray);
                g_staffDataArray = NULL;
            }
            PostQuitMessage(0);
            return 0;
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
} 