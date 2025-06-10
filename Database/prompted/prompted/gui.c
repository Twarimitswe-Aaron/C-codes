#include <windows.h>
#include <commctrl.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "gui.h"
#include "database.h"
#include "structures.h"
#include "date_utils.h"

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

// Add ListBox macros if not defined
#ifndef ListBox_ResetContent
#define ListBox_ResetContent(hwndCtl) \
    ((int)(DWORD)SNDMSG((hwndCtl), LB_RESETCONTENT, 0L, 0L))
#endif

#ifndef ListBox_AddString
#define ListBox_AddString(hwndCtl, lpsz) \
    ((int)(DWORD)SNDMSG((hwndCtl), LB_ADDSTRING, 0L, (LPARAM)(LPCTSTR)(lpsz)))
#endif

// Global variables
HWND g_hwndMain;
HWND g_hwndCurrent;
BOOL g_isDarkTheme = FALSE;

// Custom colors
COLORREF g_crBackground = RGB(255, 255, 255); // White background
COLORREF g_crText = RGB(20, 20, 100);         // Dark blue text
COLORREF g_crAccent = RGB(0, 120, 215);      // Standard blue accent

HFONT hFontDashboardTitle; // Global font handle for Dashboard title
HFONT hFontCardCount;      // Global font handle for card counts
HFONT hFontCardTitle;      // Global font handle for card titles

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

void createModernListView(HWND hwnd, int x, int y, int width, int height, int id) {
    HWND hwndList = CreateWindow(WC_LISTVIEW, "",
        WS_VISIBLE | WS_CHILD | LVS_REPORT | LVS_SINGLESEL | LVS_SHOWSELALWAYS | WS_BORDER,
        x, y, width, height,
        hwnd, (HMENU)(INT_PTR)id, NULL, NULL);
    
    // Add columns
    LVCOLUMN lvc = {0};
    lvc.mask = LVCF_TEXT | LVCF_WIDTH | LVCF_SUBITEM;
    
    // Example for books
    const char* columns[] = {"ID", "Title", "Author", "ISBN", "Publisher", "Year", "Copies", "Location"};
    int widths[] = {50, 200, 150, 100, 150, 80, 80, 100};
    
    for (int i = 0; i < 8; i++) {
        lvc.pszText = (LPSTR)columns[i];
        lvc.cx = widths[i];
        ListView_InsertColumn(hwndList, i, &lvc);
    }
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
        28, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE, DEFAULT_CHARSET,
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
                // Book Management Buttons (IDs 201-205)
                case 201: // Add Book
                {
                    Book book = getBookFromInputs(hwnd);
                    if (addBook(book)) {
                        MessageBox(hwnd, "Book added successfully!", "Success", MB_OK | MB_ICONINFORMATION);
                        clearBookInputs(hwnd);
                        refreshBookList(hwnd);
                    } else {
                        MessageBox(hwnd, "Failed to add book!", "Error", MB_OK | MB_ICONERROR);
                    }
                }
                break;
                case 202: // Update Book
                {
                    HWND hwndList = GetDlgItem(hwnd, 100); // Book ListView ID
                    int selectedIndex = ListView_GetNextItem(hwndList, -1, LVNI_SELECTED);
                    if (selectedIndex == -1) {
                        MessageBox(hwnd, "Please select a book to update!", "Error", MB_OK | MB_ICONERROR);
                        break;
                    }
                    char id[10];
                    ListView_GetItemText(hwndList, selectedIndex, 0, id, sizeof(id));
                    int book_id = atoi(id);
                    Book book = getBookFromInputs(hwnd);
                    book.book_id = book_id;
                    if (updateBook(book)) {
                        MessageBox(hwnd, "Book updated successfully!", "Success", MB_OK | MB_ICONINFORMATION);
                        clearBookInputs(hwnd);
                        refreshBookList(hwnd);
                    } else {
                        MessageBox(hwnd, "Failed to update book!", "Error", MB_OK | MB_ICONERROR);
                    }
                }
                break;
                case 203: // Delete Book
                {
                    HWND hwndList = GetDlgItem(hwnd, 100); // Book ListView ID
                    int selectedIndex = ListView_GetNextItem(hwndList, -1, LVNI_SELECTED);
                    if (selectedIndex == -1) {
                        MessageBox(hwnd, "Please select a book to delete!", "Error", MB_OK | MB_ICONERROR);
                        break;
                    }
                    char id[10];
                    ListView_GetItemText(hwndList, selectedIndex, 0, id, sizeof(id));
                    int book_id = atoi(id);
                    if (MessageBox(hwnd, "Are you sure you want to delete this book?", "Confirm Delete", MB_YESNO | MB_ICONQUESTION) == IDYES) {
                        if (deleteBook(book_id)) {
                            MessageBox(hwnd, "Book deleted successfully!", "Success", MB_OK | MB_ICONINFORMATION);
                            clearBookInputs(hwnd);
                            refreshBookList(hwnd);
                        } else {
                            MessageBox(hwnd, "Failed to delete book!", "Error", MB_OK | MB_ICONERROR);
                        }
                    }
                }
                break;
                case 204: // Clear Book Inputs
                    clearBookInputs(hwnd);
                    break;
                case 205: // Back to Main Menu
                    clearRightPanel(hwnd);
                    showMainMenu(hwnd);
                    InvalidateRect(hwnd, NULL, TRUE);
                    UpdateWindow(hwnd);
                    break;
                // Author Management Buttons (IDs 301-305)
                case 301: // Add Author
                {
                    Author author = getAuthorFromInputs(hwnd);
                    if (addAuthor(author)) {
                        MessageBox(hwnd, "Author added successfully!", "Success", MB_OK | MB_ICONINFORMATION);
                        clearAuthorInputs(hwnd);
                        refreshAuthorList(hwnd);
                    } else {
                        MessageBox(hwnd, "Failed to add author!", "Error", MB_OK | MB_ICONERROR);
                    }
                }
                break;
                case 302: // Update Author
                {
                    HWND hwndList = GetDlgItem(hwnd, 200);
                    int selectedIndex = ListView_GetNextItem(hwndList, -1, LVNI_SELECTED);
                    if (selectedIndex == -1) {
                        MessageBox(hwnd, "Please select an author to update!", "Error", MB_OK | MB_ICONERROR);
                        break;
                    }
                    char id[10];
                    ListView_GetItemText(hwndList, selectedIndex, 0, id, sizeof(id));
                    int author_id = atoi(id);
                    Author author = getAuthorFromInputs(hwnd);
                    author.author_id = author_id;
                    if (updateAuthor(author)) {
                        MessageBox(hwnd, "Author updated successfully!", "Success", MB_OK | MB_ICONINFORMATION);
                        clearAuthorInputs(hwnd);
                        refreshAuthorList(hwnd);
                    } else {
                        MessageBox(hwnd, "Failed to update author!", "Error", MB_OK | MB_ICONERROR);
                    }
                }
                break;
                case 303: // Delete Author
                {
                    HWND hwndList = GetDlgItem(hwnd, 200);
                    int selectedIndex = ListView_GetNextItem(hwndList, -1, LVNI_SELECTED);
                    if (selectedIndex == -1) {
                        MessageBox(hwnd, "Please select an author to delete!", "Error", MB_OK | MB_ICONERROR);
                        break;
                    }
                    char id[10];
                    ListView_GetItemText(hwndList, selectedIndex, 0, id, sizeof(id));
                    int author_id = atoi(id);
                    if (MessageBox(hwnd, "Are you sure you want to delete this author?", "Confirm Delete", MB_YESNO | MB_ICONQUESTION) == IDYES) {
                        if (deleteAuthor(author_id)) {
                            MessageBox(hwnd, "Author deleted successfully!", "Success", MB_OK | MB_ICONINFORMATION);
                            clearAuthorInputs(hwnd);
                            refreshAuthorList(hwnd);
                        } else {
                            MessageBox(hwnd, "Failed to delete author!", "Error", MB_OK | MB_ICONERROR);
                        }
                    }
                }
                break;
                case 304: // Clear Author Inputs
                    clearAuthorInputs(hwnd);
                    break;
                case 305: // Back to Main Menu
                    clearRightPanel(hwnd);
                    showMainMenu(hwnd);
                    InvalidateRect(hwnd, NULL, TRUE);
                    UpdateWindow(hwnd);
                    break;
                // Publisher Management Buttons (IDs 401-408)
                case 401: // List box selection
                    if (HIWORD(wp) == LBN_SELCHANGE) {
                        int index = SendDlgItemMessage(hwnd, 401, LB_GETCURSEL, 0, 0);
                        if (index != LB_ERR) {
                            char buffer[256];
                            SendDlgItemMessage(hwnd, 401, LB_GETTEXT, index, (LPARAM)buffer);
                            
                            int id;
                            sscanf(buffer, "%d", &id);
                            
                            Publisher* publisher = getPublisher(id);
                            if (publisher) {
                                SetWindowText(GetDlgItem(hwnd, 402), publisher->name);
                                SetWindowText(GetDlgItem(hwnd, 403), publisher->address);
                                free(publisher);
                            }
                        }
                    }
                    break;
                    
                case 404: // Add Publisher
                    {
                        Publisher publisher = getPublisherFromInputs(hwnd);
                        if (addPublisher(publisher) == 1) {
                            MessageBox(hwnd, "Publisher added successfully!", "Success", MB_OK);
                            refreshPublisherList(hwnd);
                            clearPublisherInputs(hwnd);
                        } else {
                            MessageBox(hwnd, "Failed to add publisher!", "Error", MB_OK);
                        }
                    }
                    break;
                    
                case 405: // Update Publisher
                    {
                        int index = SendDlgItemMessage(hwnd, 401, LB_GETCURSEL, 0, 0);
                        if (index != LB_ERR) {
                            char buffer[256];
                            SendDlgItemMessage(hwnd, 401, LB_GETTEXT, index, (LPARAM)buffer);
                            
                            int id;
                            sscanf(buffer, "%d", &id);
                            
                            Publisher publisher = getPublisherFromInputs(hwnd);
                            publisher.publisher_id = id;
                            
                            if (updatePublisher(publisher) == 0) {
                                MessageBox(hwnd, "Publisher updated successfully!", "Success", MB_OK);
                                refreshPublisherList(hwnd);
                                clearPublisherInputs(hwnd);
                            } else {
                                MessageBox(hwnd, "Failed to update publisher!", "Error", MB_OK);
                            }
                        } else {
                            MessageBox(hwnd, "Please select a publisher to update!", "Error", MB_OK);
                        }
                    }
                    break;
                    
                case 406: // Delete Publisher
                    {
                        int index = SendDlgItemMessage(hwnd, 401, LB_GETCURSEL, 0, 0);
                        if (index != LB_ERR) {
                            char buffer[256];
                            SendDlgItemMessage(hwnd, 401, LB_GETTEXT, index, (LPARAM)buffer);
                            
                            int id;
                            sscanf(buffer, "%d", &id);
                            
                            if (deletePublisher(id) == 0) {
                                MessageBox(hwnd, "Publisher deleted successfully!", "Success", MB_OK);
                                refreshPublisherList(hwnd);
                                clearPublisherInputs(hwnd);
                            } else {
                                MessageBox(hwnd, "Failed to delete publisher!", "Error", MB_OK);
                            }
                        } else {
                            MessageBox(hwnd, "Please select a publisher to delete!", "Error", MB_OK);
                        }
                    }
                    break;
                    
                case 407: // Clear Publisher Inputs
                    clearPublisherInputs(hwnd);
                    break;
                    
                case 408: // Back to Main Menu
                    clearRightPanel(hwnd);
                    showMainMenu(hwnd);
                    InvalidateRect(hwnd, NULL, TRUE);
                    UpdateWindow(hwnd);
                    break;
                // Member Management Buttons (IDs 1106-1110)
                case 1106: // Add Member
                {
                    Member member = getMemberFromInputs(hwnd);
                    if (addMember(member)) {
                        MessageBox(hwnd, "Member added successfully!", "Success", MB_OK | MB_ICONINFORMATION);
                        clearMemberInputs(hwnd);
                        refreshMemberList(hwnd);
                    } else {
                        MessageBox(hwnd, "Failed to add member!", "Error", MB_OK | MB_ICONERROR);
                    }
                }
                break;
                case 1107: // Update Member
                {
                    HWND hwndList = GetDlgItem(hwnd, 1100); // Member ListBox ID
                    int selectedIndex = SendDlgItemMessage(hwndList, 0, LB_GETCURSEL, 0, 0);
                    if (selectedIndex == LB_ERR) {
                        MessageBox(hwnd, "Please select a member to update!", "Error", MB_OK | MB_ICONERROR);
                        break;
                    }
                    char buffer[256];
                    SendDlgItemMessage(hwndList, 0, LB_GETTEXT, selectedIndex, (LPARAM)buffer);
                    int id;
                    sscanf(buffer, "%d", &id);
                    Member member = getMemberFromInputs(hwnd);
                    member.member_id = id;
                    if (updateMember(member)) {
                        MessageBox(hwnd, "Member updated successfully!", "Success", MB_OK | MB_ICONINFORMATION);
                        clearMemberInputs(hwnd);
                        refreshMemberList(hwnd);
                    } else {
                        MessageBox(hwnd, "Failed to update member!", "Error", MB_OK | MB_ICONERROR);
                    }
                }
                break;
                case 1108: // Delete Member
                {
                    HWND hwndList = GetDlgItem(hwnd, 1100); // Member ListBox ID
                    int selectedIndex = SendDlgItemMessage(hwndList, 0, LB_GETCURSEL, 0, 0);
                    if (selectedIndex == LB_ERR) {
                        MessageBox(hwnd, "Please select a member to delete!", "Error", MB_OK | MB_ICONERROR);
                        break;
                    }
                    char buffer[256];
                    SendDlgItemMessage(hwndList, 0, LB_GETTEXT, selectedIndex, (LPARAM)buffer);
                    int id;
                    sscanf(buffer, "%d", &id);
                    if (MessageBox(hwnd, "Are you sure you want to delete this member?", "Confirm Delete", MB_YESNO | MB_ICONQUESTION) == IDYES) {
                        if (deleteMember(id)) {
                            MessageBox(hwnd, "Member deleted successfully!", "Success", MB_OK | MB_ICONINFORMATION);
                            clearMemberInputs(hwnd);
                            refreshMemberList(hwnd);
                        } else {
                            MessageBox(hwnd, "Failed to delete member!", "Error", MB_OK | MB_ICONERROR);
                        }
                    }
                }
                break;
                case 1109: // Clear Member Inputs
                    clearMemberInputs(hwnd);
                    break;
                case 1110: // Back to Main Menu
                    clearRightPanel(hwnd);
                    showMainMenu(hwnd);
                    InvalidateRect(hwnd, NULL, TRUE);
                    UpdateWindow(hwnd);
                    break;
                // Staff Management Buttons (IDs 1205-1209)
                case 1205: // Add Staff
                {
                    Staff staff = getStaffFromInputs(hwnd);
                    if (addStaff(staff)) {
                        MessageBox(hwnd, "Staff added successfully!", "Success", MB_OK | MB_ICONINFORMATION);
                        clearStaffInputs(hwnd);
                        refreshStaffList(hwnd);
                    } else {
                        MessageBox(hwnd, "Failed to add staff!", "Error", MB_OK | MB_ICONERROR);
                    }
                }
                break;
                case 1206: // Update Staff
                {
                    HWND hwndList = GetDlgItem(hwnd, 1200);
                    int selectedIndex = SendDlgItemMessage(hwndList, 0, LB_GETCURSEL, 0, 0);
                    if (selectedIndex == LB_ERR) {
                        MessageBox(hwnd, "Please select a staff member to update!", "Error", MB_OK | MB_ICONERROR);
                        break;
                    }
                    char buffer[256];
                    SendDlgItemMessage(hwndList, 0, LB_GETTEXT, selectedIndex, (LPARAM)buffer);
                    int id;
                    sscanf(buffer, "%d", &id);
                    Staff staff = getStaffFromInputs(hwnd);
                    staff.staff_id = id;
                    if (updateStaff(staff)) {
                        MessageBox(hwnd, "Staff updated successfully!", "Success", MB_OK | MB_ICONINFORMATION);
                        clearStaffInputs(hwnd);
                        refreshStaffList(hwnd);
                    } else {
                        MessageBox(hwnd, "Failed to update staff!", "Error", MB_OK | MB_ICONERROR);
                    }
                }
                break;
                case 1207: // Delete Staff
                {
                    HWND hwndList = GetDlgItem(hwnd, 1200);
                    int selectedIndex = SendDlgItemMessage(hwndList, 0, LB_GETCURSEL, 0, 0);
                    if (selectedIndex == LB_ERR) {
                        MessageBox(hwnd, "Please select a staff member to delete!", "Error", MB_OK | MB_ICONERROR);
                        break;
                    }
                    char buffer[256];
                    SendDlgItemMessage(hwndList, 0, LB_GETTEXT, selectedIndex, (LPARAM)buffer);
                    int id;
                    sscanf(buffer, "%d", &id);
                    if (MessageBox(hwnd, "Are you sure you want to delete this staff member?", "Confirm Delete", MB_YESNO | MB_ICONQUESTION) == IDYES) {
                        if (deleteStaff(id)) {
                            MessageBox(hwnd, "Staff deleted successfully!", "Success", MB_OK | MB_ICONINFORMATION);
                            clearStaffInputs(hwnd);
                            refreshStaffList(hwnd);
                        } else {
                            MessageBox(hwnd, "Failed to delete staff!", "Error", MB_OK | MB_ICONERROR);
                        }
                    }
                }
                break;
                case 1208: // Clear Staff Inputs
                    clearStaffInputs(hwnd);
                    break;
                case 1209: // Back to Main Menu
                    clearRightPanel(hwnd);
                    showMainMenu(hwnd);
                    InvalidateRect(hwnd, NULL, TRUE);
                    UpdateWindow(hwnd);
                    break;
                // Borrowing Management Buttons (IDs 1305-1309)
                case 1305: // Add Borrowing
                {
                    Borrowing borrowing = getBorrowingFromInputs(hwnd);
                    if (addBorrowing(borrowing)) {
                        MessageBox(hwnd, "Borrowing added successfully!", "Success", MB_OK | MB_ICONINFORMATION);
                        clearBorrowingInputs(hwnd);
                        refreshBorrowingList(hwnd);
                    } else {
                        MessageBox(hwnd, "Failed to add borrowing!", "Error", MB_OK | MB_ICONERROR);
                    }
                }
                break;
                case 1306: // Return Borrowing (Update with return date)
                {
                    HWND hwndList = GetDlgItem(hwnd, 1300); // Borrowing ListBox ID
                    int selectedIndex = SendDlgItemMessage(hwndList, 0, LB_GETCURSEL, 0, 0);
                    if (selectedIndex == LB_ERR) {
                        MessageBox(hwnd, "Please select a borrowing to mark as returned!", "Error", MB_OK | MB_ICONERROR);
                        break;
                    }
                    char buffer[256];
                    SendDlgItemMessage(hwndList, 0, LB_GETTEXT, selectedIndex, (LPARAM)buffer);
                    int id;
                    sscanf(buffer, "%d", &id);
                    Borrowing* borrowing = getBorrowing(id);
                    if (borrowing) {
                        borrowing->return_date = time(NULL); // Set current time as return date
                        if (updateBorrowing(*borrowing)) {
                            MessageBox(hwnd, "Borrowing marked as returned successfully!", "Success", MB_OK | MB_ICONINFORMATION);
                            clearBorrowingInputs(hwnd);
                            refreshBorrowingList(hwnd);
                        } else {
                            MessageBox(hwnd, "Failed to mark borrowing as returned!", "Error", MB_OK | MB_ICONERROR);
                        }
                        free(borrowing);
                    } else {
                        MessageBox(hwnd, "Borrowing not found!", "Error", MB_OK | MB_ICONERROR);
                    }
                }
                break;
                case 1307: // Delete Borrowing
                {
                    HWND hwndList = GetDlgItem(hwnd, 1300); // Borrowing ListBox ID
                    int selectedIndex = SendDlgItemMessage(hwndList, 0, LB_GETCURSEL, 0, 0);
                    if (selectedIndex == LB_ERR) {
                        MessageBox(hwnd, "Please select a borrowing to delete!", "Error", MB_OK | MB_ICONERROR);
                        break;
                    }
                    char buffer[256];
                    SendDlgItemMessage(hwndList, 0, LB_GETTEXT, selectedIndex, (LPARAM)buffer);
                    int id;
                    sscanf(buffer, "%d", &id);
                    if (MessageBox(hwnd, "Are you sure you want to delete this borrowing?", "Confirm Delete", MB_YESNO | MB_ICONQUESTION) == IDYES) {
                        if (deleteBorrowing(id)) {
                            MessageBox(hwnd, "Borrowing deleted successfully!", "Success", MB_OK | MB_ICONINFORMATION);
                            clearBorrowingInputs(hwnd);
                            refreshBorrowingList(hwnd);
                        } else {
                            MessageBox(hwnd, "Failed to delete borrowing!", "Error", MB_OK | MB_ICONERROR);
                        }
                    }
                }
                break;
                case 1308: // Clear Borrowing Inputs
                    clearBorrowingInputs(hwnd);
                    break;
                case 1309: // Back to Main Menu
                    clearRightPanel(hwnd);
                    showMainMenu(hwnd);
                    InvalidateRect(hwnd, NULL, TRUE);
                    UpdateWindow(hwnd);
                    break;
                // Fine Management Buttons (IDs 1404-1408)
                case 1404: // Add Fine
                {
                    Fine fine = getFineFromInputs(hwnd);
                    if (addFine(fine)) {
                        MessageBox(hwnd, "Fine added successfully!", "Success", MB_OK | MB_ICONINFORMATION);
                        clearFineInputs(hwnd);
                        refreshFineList(hwnd);
                    } else {
                        MessageBox(hwnd, "Failed to add fine!", "Error", MB_OK | MB_ICONERROR);
                    }
                }
                break;
                case 1405: // Mark Fine as Paid
                {
                    HWND hwndList = GetDlgItem(hwnd, 1400); // Fine ListBox ID
                    int selectedIndex = SendDlgItemMessage(hwndList, 0, LB_GETCURSEL, 0, 0);
                    if (selectedIndex == LB_ERR) {
                        MessageBox(hwnd, "Please select a fine to mark as paid!", "Error", MB_OK | MB_ICONERROR);
                        break;
                    }
                    char buffer[256];
                    SendDlgItemMessage(hwndList, 0, LB_GETTEXT, selectedIndex, (LPARAM)buffer);
                    int id;
                    sscanf(buffer, "%d", &id);
                    Fine* fine = getFine(id);
                    if (fine) {
                        fine->paid = TRUE;
                        if (updateFine(*fine)) {
                            MessageBox(hwnd, "Fine marked as paid successfully!", "Success", MB_OK | MB_ICONINFORMATION);
                            clearFineInputs(hwnd);
                            refreshFineList(hwnd);
                        } else {
                            MessageBox(hwnd, "Failed to mark fine as paid!", "Error", MB_OK | MB_ICONERROR);
                        }
                        free(fine);
                    } else {
                        MessageBox(hwnd, "Fine not found!", "Error", MB_OK | MB_ICONERROR);
                    }
                }
                break;
                case 1406: // Delete Fine
                {
                    HWND hwndList = GetDlgItem(hwnd, 1400); // Fine ListBox ID
                    int selectedIndex = SendDlgItemMessage(hwndList, 0, LB_GETCURSEL, 0, 0);
                    if (selectedIndex == LB_ERR) {
                        MessageBox(hwnd, "Please select a fine to delete!", "Error", MB_OK | MB_ICONERROR);
                        break;
                    }
                    char buffer[256];
                    SendDlgItemMessage(hwndList, 0, LB_GETTEXT, selectedIndex, (LPARAM)buffer);
                    int id;
                    sscanf(buffer, "%d", &id);
                    if (MessageBox(hwnd, "Are you sure you want to delete this fine?", "Confirm Delete", MB_YESNO | MB_ICONQUESTION) == IDYES) {
                        if (deleteFine(id)) {
                            MessageBox(hwnd, "Fine deleted successfully!", "Success", MB_OK | MB_ICONINFORMATION);
                            clearFineInputs(hwnd);
                            refreshFineList(hwnd);
                        } else {
                            MessageBox(hwnd, "Failed to delete fine!", "Error", MB_OK | MB_ICONERROR);
                        }
                    }
                }
                break;
                case 1407: // Clear Fine Inputs
                    clearFineInputs(hwnd);
                    break;
                case 1408: // Back to Main Menu
                    clearRightPanel(hwnd);
                    showMainMenu(hwnd);
                    InvalidateRect(hwnd, NULL, TRUE);
                    UpdateWindow(hwnd);
                    break;
            }
            break;
            
        case WM_CTLCOLORSTATIC:
            SetBkMode((HDC)wp, TRANSPARENT);
            SetTextColor((HDC)wp, g_crText);
            return (LRESULT)GetStockObject(NULL_BRUSH);

        case WM_CTLCOLOREDIT: {
            HDC hdcEdit = (HDC)wp;
            SetTextColor(hdcEdit, RGB(50, 50, 50));   // Dark gray text
            SetBkColor(hdcEdit, RGB(240, 240, 240));   // Light gray background
            return (LRESULT)CreateSolidBrush(RGB(240, 240, 240)); // Return a solid brush for the background
        }

        case WM_DRAWITEM: {
            LPDRAWITEMSTRUCT lpdis = (LPDRAWITEMSTRUCT)lp;
            if (lpdis->CtlType == ODT_BUTTON) {
                // Check if it's one of our dashboard buttons
                if (lpdis->CtlID >= 1 && lpdis->CtlID <= 7) {
                    HBRUSH hBrushBackground = CreateSolidBrush(g_crBackground);
                    HPEN hPenBorder = CreatePen(PS_SOLID, 1, RGB(180, 180, 180));

                    // Draw the rounded rectangle background
                    SelectObject(lpdis->hDC, hBrushBackground);
                    SelectObject(lpdis->hDC, hPenBorder);
                    RoundRect(lpdis->hDC, lpdis->rcItem.left, lpdis->rcItem.top,
                              lpdis->rcItem.right, lpdis->rcItem.bottom, 10, 10);

                    // If the button is focused or clicked, draw a focus rectangle or highlight
                    if (lpdis->itemState & ODS_FOCUS) {
                        DrawFocusRect(lpdis->hDC, &lpdis->rcItem);
                    }

                    // Clean up GDI objects
                    DeleteObject(hBrushBackground);
                    DeleteObject(hPenBorder);
                }
            }
            return TRUE;
        }

        case WM_DESTROY:
            PostQuitMessage(0);
            // Clean up global font handles
            if (hFontDashboardTitle) DeleteObject(hFontDashboardTitle);
            if (hFontCardCount) DeleteObject(hFontCardCount);
            if (hFontCardTitle) DeleteObject(hFontCardTitle);
            break;
            
        case WM_NOTIFY:
            {
                LPNMHDR pnmh = (LPNMHDR)lp;
                // Book Management List View (ID 100)
                if (pnmh->code == NM_CLICK && pnmh->idFrom == 100) {
                    HWND hwndList = GetDlgItem(hwnd, 100);
                    int selectedIndex = ListView_GetNextItem(hwndList, -1, LVNI_SELECTED);
                    if (selectedIndex != -1) {
                        char id[10];
                        ListView_GetItemText(hwndList, selectedIndex, 0, id, sizeof(id));
                        int book_id = atoi(id);
                        Book* book = getBook(book_id);
                        if (book) {
                            char buffer[256];
                            SetWindowText(GetDlgItem(hwnd, 101), book->title);
                            sprintf(buffer, "%d", book->author_id);
                            SetWindowText(GetDlgItem(hwnd, 102), buffer);
                            sprintf(buffer, "%d", book->publisher_id);
                            SetWindowText(GetDlgItem(hwnd, 103), buffer);
                            SetWindowText(GetDlgItem(hwnd, 104), book->isbn);
                            SetWindowText(GetDlgItem(hwnd, 105), book->genre);
                            sprintf(buffer, "%d", book->year_published);
                            SetWindowText(GetDlgItem(hwnd, 106), buffer);
                            sprintf(buffer, "%d", book->copies_available);
                            SetWindowText(GetDlgItem(hwnd, 107), buffer);
                            SetWindowText(GetDlgItem(hwnd, 108), book->shelf_location);
                            free(book);
                        }
                    }
                }
                // Author Management List View (ID 200)
                if (pnmh->code == NM_CLICK && pnmh->idFrom == 200) {
                    HWND hwndList = GetDlgItem(hwnd, 200);
                    int selectedIndex = ListView_GetNextItem(hwndList, -1, LVNI_SELECTED);
                    if (selectedIndex != -1) {
                        char id[10];
                        ListView_GetItemText(hwndList, selectedIndex, 0, id, sizeof(id));
                        int author_id = atoi(id);
                        Author* author = getAuthor(author_id);
                        if (author) {
                            SetWindowText(GetDlgItem(hwnd, 201), author->name);
                            SetWindowText(GetDlgItem(hwnd, 202), author->bio);
                            free(author);
                        }
                    }
                }
                // Publisher Management List Box (ID 401)
                if (pnmh->code == LBN_SELCHANGE && pnmh->idFrom == 401) {
                    int index = SendDlgItemMessage(hwnd, 401, LB_GETCURSEL, 0, 0);
                    if (index != LB_ERR) {
                        char buffer[256];
                        SendDlgItemMessage(hwnd, 401, LB_GETTEXT, index, (LPARAM)buffer);
                        
                        int id;
                        sscanf(buffer, "%d", &id);
                        
                        Publisher* publisher = getPublisher(id);
                        if (publisher) {
                            SetWindowText(GetDlgItem(hwnd, 402), publisher->name);
                            SetWindowText(GetDlgItem(hwnd, 403), publisher->address);
                            free(publisher);
                        }
                    }
                }
            }
            break;
            
        default:
            return DefWindowProc(hwnd, msg, wp, lp);
    }
    return 0;
}

// Helper function to clear window
void clearWindow(HWND hwnd) {
    HWND child = GetWindow(hwnd, GW_CHILD);
    while (child) {
        HWND nextChild = GetNextWindow(child, GW_HWNDNEXT);
        DestroyWindow(child);
        child = nextChild;
    }
}

// Helper function to clear content in the right panel
void clearRightPanel(HWND hwnd) {
    HWND child = GetWindow(hwnd, GW_CHILD);
    while (child) {
        HWND nextChild = GetNextWindow(child, GW_HWNDNEXT);

        RECT rectChild;
        GetWindowRect(child, &rectChild);
        ScreenToClient(hwnd, (LPPOINT)&rectChild.left);
        ScreenToClient(hwnd, (LPPOINT)&rectChild.right);

        // Check if the control is in the right panel area
        if (rectChild.left >= RIGHT_PANEL_START_X - 5) { // Small margin for error
            DestroyWindow(child);
        }
        child = nextChild;
    }
}

// Book management window
void showBookManagement(HWND hwnd) {
    // DO NOT clear the entire window here, clearRightPanel handles it
    // Create book management interface
    createModernLabel(hwnd, "Book Management", RIGHT_PANEL_START_X + 20, 20, 200, 30);
    
    // Create list view
    createModernListView(hwnd, RIGHT_PANEL_START_X + 20, 60, 740, 300, 100);
    
    // Create input fields
    createModernLabel(hwnd, "Title:", RIGHT_PANEL_START_X + 20, 380, 100, 20);
    createModernInput(hwnd, RIGHT_PANEL_START_X + 130, 380, 200, 20, 101);
    
    createModernLabel(hwnd, "Author ID:", RIGHT_PANEL_START_X + 20, 410, 100, 20);
    createModernInput(hwnd, RIGHT_PANEL_START_X + 130, 410, 200, 20, 102);
    
    createModernLabel(hwnd, "Publisher ID:", RIGHT_PANEL_START_X + 20, 440, 100, 20);
    createModernInput(hwnd, RIGHT_PANEL_START_X + 130, 440, 200, 20, 103);
    
    createModernLabel(hwnd, "ISBN:", RIGHT_PANEL_START_X + 20, 470, 100, 20);
    createModernInput(hwnd, RIGHT_PANEL_START_X + 130, 470, 200, 20, 104);
    
    createModernLabel(hwnd, "Genre:", RIGHT_PANEL_START_X + 20, 500, 100, 20);
    createModernInput(hwnd, RIGHT_PANEL_START_X + 130, 500, 200, 20, 105);
    
    createModernLabel(hwnd, "Year:", RIGHT_PANEL_START_X + 20, 530, 100, 20);
    createModernInput(hwnd, RIGHT_PANEL_START_X + 130, 530, 200, 20, 106);
    
    createModernLabel(hwnd, "Copies:", RIGHT_PANEL_START_X + 20, 560, 100, 20);
    createModernInput(hwnd, RIGHT_PANEL_START_X + 130, 560, 200, 20, 107);
    
    createModernLabel(hwnd, "Location:", RIGHT_PANEL_START_X + 20, 590, 100, 20);
    createModernInput(hwnd, RIGHT_PANEL_START_X + 130, 590, 200, 20, 108);
    
    // Create buttons
    createModernButton(hwnd, "Add", RIGHT_PANEL_START_X + 20, 630, 100, 30, 201);
    createModernButton(hwnd, "Update", RIGHT_PANEL_START_X + 130, 630, 100, 30, 202);
    createModernButton(hwnd, "Delete", RIGHT_PANEL_START_X + 240, 630, 100, 30, 203);
    createModernButton(hwnd, "Clear", RIGHT_PANEL_START_X + 350, 630, 100, 30, 204);
    createModernButton(hwnd, "Back", RIGHT_PANEL_START_X + 460, 630, 100, 30, 205);
    
    // Load and display books
    refreshBookList(hwnd);
}

// Helper function to refresh the book list
void refreshBookList(HWND hwnd) {
    HWND hwndList = GetDlgItem(hwnd, 100);
    ListView_DeleteAllItems(hwndList);
    
    int count;
    MessageBox(hwnd, "Before getAllBooks", "Debug", MB_OK);
    Book* books = getAllBooks(&count);
    MessageBox(hwnd, "After getAllBooks", "Debug", MB_OK);
    if (books) {
        for (int i = 0; i < count; i++) {
            LVITEM lvi = {0};
            lvi.mask = LVIF_TEXT;
            lvi.iItem = i;
            
            char id[10];
            sprintf(id, "%d", books[i].book_id);
            
            lvi.iSubItem = 0;
            lvi.pszText = id;
            ListView_InsertItem(hwndList, &lvi);
            
            ListView_SetItemText(hwndList, i, 1, books[i].title);
            
            char authorId[10];
            sprintf(authorId, "%d", books[i].author_id);
            ListView_SetItemText(hwndList, i, 2, authorId);
            
            ListView_SetItemText(hwndList, i, 3, books[i].isbn);
            
            char publisherId[10];
            sprintf(publisherId, "%d", books[i].publisher_id);
            ListView_SetItemText(hwndList, i, 4, publisherId);
            
            char year[10];
            sprintf(year, "%d", books[i].year_published);
            ListView_SetItemText(hwndList, i, 5, year);
            
            char copies[10];
            sprintf(copies, "%d", books[i].copies_available);
            ListView_SetItemText(hwndList, i, 6, copies);
            
            ListView_SetItemText(hwndList, i, 7, books[i].shelf_location);
        }
        free(books);
    }
}

// Helper function to clear input fields
void clearBookInputs(HWND hwnd) {
    SetWindowText(GetDlgItem(hwnd, 101), "");
    SetWindowText(GetDlgItem(hwnd, 102), "");
    SetWindowText(GetDlgItem(hwnd, 103), "");
    SetWindowText(GetDlgItem(hwnd, 104), "");
    SetWindowText(GetDlgItem(hwnd, 105), "");
    SetWindowText(GetDlgItem(hwnd, 106), "");
    SetWindowText(GetDlgItem(hwnd, 107), "");
    SetWindowText(GetDlgItem(hwnd, 108), "");
}

// Helper function to get book from input fields
Book getBookFromInputs(HWND hwnd) {
    Book book = {0};
    char buffer[256];
    
    GetWindowText(GetDlgItem(hwnd, 101), book.title, sizeof(book.title));
    GetWindowText(GetDlgItem(hwnd, 102), buffer, sizeof(buffer));
    book.author_id = atoi(buffer);
    GetWindowText(GetDlgItem(hwnd, 103), buffer, sizeof(buffer));
    book.publisher_id = atoi(buffer);
    GetWindowText(GetDlgItem(hwnd, 104), book.isbn, sizeof(book.isbn));
    GetWindowText(GetDlgItem(hwnd, 105), book.genre, sizeof(book.genre));
    GetWindowText(GetDlgItem(hwnd, 106), buffer, sizeof(buffer));
    book.year_published = atoi(buffer);
    GetWindowText(GetDlgItem(hwnd, 107), buffer, sizeof(buffer));
    book.copies_available = atoi(buffer);
    GetWindowText(GetDlgItem(hwnd, 108), book.shelf_location, sizeof(book.shelf_location));
    
    return book;
}

// Author management window
void showAuthorManagement(HWND hwnd) {
    // Clear existing controls
    clearRightPanel(hwnd);

    // Create author management interface
    createModernLabel(hwnd, "Author Management", RIGHT_PANEL_START_X + 20, 20, 200, 30);

    // Create list view
    HWND hwndList = CreateWindow(WC_LISTVIEW, "",
        WS_VISIBLE | WS_CHILD | LVS_REPORT | LVS_SINGLESEL | LVS_SHOWSELALWAYS | WS_BORDER,
        RIGHT_PANEL_START_X + 20, 60, 740, 300,
        hwnd, (HMENU)200, NULL, NULL);

    // Add columns
    LVCOLUMN lvc = {0};
    lvc.mask = LVCF_TEXT | LVCF_WIDTH | LVCF_SUBITEM;
    const char* columns[] = {"ID", "Name", "Bio"};
    int widths[] = {50, 200, 450};
    for (int i = 0; i < 3; i++) {
        lvc.pszText = (LPSTR)columns[i];
        lvc.cx = widths[i];
        ListView_InsertColumn(hwndList, i, &lvc);
    }

    // Create input fields
    createModernLabel(hwnd, "Name:", RIGHT_PANEL_START_X + 20, 380, 100, 20);
    createModernInput(hwnd, RIGHT_PANEL_START_X + 130, 380, 200, 20, 201);

    createModernLabel(hwnd, "Bio:", RIGHT_PANEL_START_X + 20, 410, 100, 20);
    createModernInput(hwnd, RIGHT_PANEL_START_X + 130, 410, 400, 60, 202);

    // Create buttons
    createModernButton(hwnd, "Add", RIGHT_PANEL_START_X + 20, 490, 100, 30, 301);
    createModernButton(hwnd, "Update", RIGHT_PANEL_START_X + 130, 490, 100, 30, 302);
    createModernButton(hwnd, "Delete", RIGHT_PANEL_START_X + 240, 490, 100, 30, 303);
    createModernButton(hwnd, "Clear", RIGHT_PANEL_START_X + 350, 490, 100, 30, 304);
    createModernButton(hwnd, "Back", RIGHT_PANEL_START_X + 460, 490, 100, 30, 305);

    // Load and display authors
    refreshAuthorList(hwnd);
}

void refreshAuthorList(HWND hwnd) {
    HWND hwndList = GetDlgItem(hwnd, 200);
    ListView_DeleteAllItems(hwndList);

    int count;
    MessageBox(hwnd, "Before getAllAuthors", "Debug", MB_OK);
    Author* authors = getAllAuthors(&count);
    MessageBox(hwnd, "After getAllAuthors", "Debug", MB_OK);
    if (authors) {
        for (int i = 0; i < count; i++) {
            LVITEM lvi = {0};
            lvi.mask = LVIF_TEXT;
            lvi.iItem = i;

            char id[10];
            sprintf(id, "%d", authors[i].author_id);
            lvi.iSubItem = 0;
            lvi.pszText = id;
            ListView_InsertItem(hwndList, &lvi);

            ListView_SetItemText(hwndList, i, 1, authors[i].name);
            ListView_SetItemText(hwndList, i, 2, authors[i].bio);
        }
        free(authors);
    }
}

void clearAuthorInputs(HWND hwnd) {
    SetWindowText(GetDlgItem(hwnd, 201), "");
    SetWindowText(GetDlgItem(hwnd, 202), "");
}

Author getAuthorFromInputs(HWND hwnd) {
    Author author = {0};
    GetWindowText(GetDlgItem(hwnd, 201), author.name, sizeof(author.name));
    GetWindowText(GetDlgItem(hwnd, 202), author.bio, sizeof(author.bio));
    return author;
}

// Publisher management helper functions
void refreshPublisherList(HWND hwnd) {
    HWND list = GetDlgItem(hwnd, 401);
    ListBox_ResetContent(list);
    
    int count;
    MessageBox(hwnd, "Before getAllPublishers", "Debug", MB_OK);
    Publisher* publishers = getAllPublishers(&count);
    MessageBox(hwnd, "After getAllPublishers", "Debug", MB_OK);
    if (publishers) {
        for (int i = 0; i < count; i++) {
            char buffer[256];
            sprintf(buffer, "%d - %s", publishers[i].publisher_id, publishers[i].name);
            ListBox_AddString(list, buffer);
        }
        free(publishers);
    }
}

void clearPublisherInputs(HWND hwnd) {
    SetWindowText(GetDlgItem(hwnd, 1001), "");
    SetWindowText(GetDlgItem(hwnd, 1002), "");
}

Publisher getPublisherFromInputs(HWND hwnd) {
    Publisher publisher = {0};
    GetWindowText(GetDlgItem(hwnd, 1001), publisher.name, sizeof(publisher.name));
    GetWindowText(GetDlgItem(hwnd, 1002), publisher.address, sizeof(publisher.address));
    return publisher;
}

void showPublisherManagement(HWND hwnd) {
    // Clear existing controls
    clearRightPanel(hwnd);
    
    // Create list box
    CreateWindow("LISTBOX", "", 
                WS_VISIBLE | WS_CHILD | WS_BORDER | LBS_NOTIFY,
                RIGHT_PANEL_START_X + 10, 10, 200, 300, hwnd, (HMENU)401, NULL, NULL);
    
    // Create input fields
    CreateWindow("STATIC", "Name:", 
                WS_VISIBLE | WS_CHILD,
                RIGHT_PANEL_START_X + 220, 10, 100, 20, hwnd, NULL, NULL, NULL);
    CreateWindow("EDIT", "", 
                WS_VISIBLE | WS_CHILD | WS_BORDER,
                RIGHT_PANEL_START_X + 220, 30, 200, 20, hwnd, (HMENU)402, NULL, NULL);
    
    CreateWindow("STATIC", "Address:", 
                WS_VISIBLE | WS_CHILD,
                RIGHT_PANEL_START_X + 220, 60, 100, 20, hwnd, NULL, NULL, NULL);
    CreateWindow("EDIT", "", 
                WS_VISIBLE | WS_CHILD | WS_BORDER,
                RIGHT_PANEL_START_X + 220, 80, 200, 20, hwnd, (HMENU)403, NULL, NULL);
    
    // Create buttons
    CreateWindow("BUTTON", "Add", 
                WS_VISIBLE | WS_CHILD,
                RIGHT_PANEL_START_X + 220, 120, 80, 30, hwnd, (HMENU)404, NULL, NULL);
    CreateWindow("BUTTON", "Update", 
                WS_VISIBLE | WS_CHILD,
                RIGHT_PANEL_START_X + 310, 120, 80, 30, hwnd, (HMENU)405, NULL, NULL);
    CreateWindow("BUTTON", "Delete", 
                WS_VISIBLE | WS_CHILD,
                RIGHT_PANEL_START_X + 400, 120, 80, 30, hwnd, (HMENU)406, NULL, NULL);
    CreateWindow("BUTTON", "Clear", 
                WS_VISIBLE | WS_CHILD,
                RIGHT_PANEL_START_X + 220, 160, 80, 30, hwnd, (HMENU)407, NULL, NULL);
    CreateWindow("BUTTON", "Back", 
                WS_VISIBLE | WS_CHILD,
                RIGHT_PANEL_START_X + 310, 160, 80, 30, hwnd, (HMENU)408, NULL, NULL);
    
    // Refresh publisher list
    refreshPublisherList(hwnd);
}

// Member management helper functions
void refreshMemberList(HWND hwnd) {
    HWND list = GetDlgItem(hwnd, 1100);
    ListBox_ResetContent(list);
    
    int count;
    MessageBox(hwnd, "Before getAllMembers", "Debug", MB_OK);
    Member* members = getAllMembers(&count);
    MessageBox(hwnd, "After getAllMembers", "Debug", MB_OK);
    if (members) {
        for (int i = 0; i < count; i++) {
            char buffer[256];
            sprintf(buffer, "%d - %s", members[i].member_id, members[i].name);
            ListBox_AddString(list, buffer);
        }
        free(members);
    }
}

void clearMemberInputs(HWND hwnd) {
    SetWindowText(GetDlgItem(hwnd, 1101), ""); // Name
    SetWindowText(GetDlgItem(hwnd, 1102), ""); // Address
    SetWindowText(GetDlgItem(hwnd, 1103), ""); // Phone
    SetWindowText(GetDlgItem(hwnd, 1104), ""); // Email
    SetWindowText(GetDlgItem(hwnd, 1105), ""); // Membership Status
}

Member getMemberFromInputs(HWND hwnd) {
    Member member = {0};
    GetWindowText(GetDlgItem(hwnd, 1101), member.name, sizeof(member.name));
    GetWindowText(GetDlgItem(hwnd, 1102), member.address, sizeof(member.address));
    GetWindowText(GetDlgItem(hwnd, 1103), member.phone, sizeof(member.phone));
    GetWindowText(GetDlgItem(hwnd, 1104), member.email, sizeof(member.email));
    GetWindowText(GetDlgItem(hwnd, 1105), member.membership_status, sizeof(member.membership_status));
    member.date_joined = time(NULL); // Set current date as join date
    return member;
}

void showMemberManagement(HWND hwnd) {
    // Clear existing controls
    clearRightPanel(hwnd);
    
    // Create list box
    CreateWindow("LISTBOX", "", 
                WS_VISIBLE | WS_CHILD | WS_BORDER | LBS_NOTIFY,
                RIGHT_PANEL_START_X + 10, 10, 200, 300, hwnd, (HMENU)1100, NULL, NULL);
    
    // Create input fields
    CreateWindow("STATIC", "Name:", 
                WS_VISIBLE | WS_CHILD,
                RIGHT_PANEL_START_X + 220, 10, 100, 20, hwnd, NULL, NULL, NULL);
    CreateWindow("EDIT", "", 
                WS_VISIBLE | WS_CHILD | WS_BORDER,
                RIGHT_PANEL_START_X + 220, 30, 200, 20, hwnd, (HMENU)1101, NULL, NULL);
    
    CreateWindow("STATIC", "Address:", 
                WS_VISIBLE | WS_CHILD,
                RIGHT_PANEL_START_X + 220, 60, 100, 20, hwnd, NULL, NULL, NULL);
    CreateWindow("EDIT", "", 
                WS_VISIBLE | WS_CHILD | WS_BORDER,
                RIGHT_PANEL_START_X + 220, 80, 200, 20, hwnd, (HMENU)1102, NULL, NULL);
    
    CreateWindow("STATIC", "Phone:", 
                WS_VISIBLE | WS_CHILD,
                RIGHT_PANEL_START_X + 220, 110, 100, 20, hwnd, NULL, NULL, NULL);
    CreateWindow("EDIT", "", 
                WS_VISIBLE | WS_CHILD | WS_BORDER,
                RIGHT_PANEL_START_X + 220, 130, 200, 20, hwnd, (HMENU)1103, NULL, NULL);
    
    CreateWindow("STATIC", "Email:", 
                WS_VISIBLE | WS_CHILD,
                RIGHT_PANEL_START_X + 220, 160, 100, 20, hwnd, NULL, NULL, NULL);
    CreateWindow("EDIT", "", 
                WS_VISIBLE | WS_CHILD | WS_BORDER,
                RIGHT_PANEL_START_X + 220, 180, 200, 20, hwnd, (HMENU)1104, NULL, NULL);
    
    CreateWindow("STATIC", "Membership Status:", 
                WS_VISIBLE | WS_CHILD,
                RIGHT_PANEL_START_X + 220, 210, 100, 20, hwnd, NULL, NULL, NULL);
    CreateWindow("EDIT", "", 
                WS_VISIBLE | WS_CHILD | WS_BORDER,
                RIGHT_PANEL_START_X + 220, 230, 200, 20, hwnd, (HMENU)1105, NULL, NULL);
    
    // Create buttons
    CreateWindow("BUTTON", "Add", 
                WS_VISIBLE | WS_CHILD,
                RIGHT_PANEL_START_X + 220, 270, 80, 30, hwnd, (HMENU)1106, NULL, NULL);
    CreateWindow("BUTTON", "Update", 
                WS_VISIBLE | WS_CHILD,
                RIGHT_PANEL_START_X + 310, 270, 80, 30, hwnd, (HMENU)1107, NULL, NULL);
    CreateWindow("BUTTON", "Delete", 
                WS_VISIBLE | WS_CHILD,
                RIGHT_PANEL_START_X + 400, 270, 80, 30, hwnd, (HMENU)1108, NULL, NULL);
    CreateWindow("BUTTON", "Clear", 
                WS_VISIBLE | WS_CHILD,
                RIGHT_PANEL_START_X + 220, 310, 80, 30, hwnd, (HMENU)1109, NULL, NULL);
    CreateWindow("BUTTON", "Back", 
                WS_VISIBLE | WS_CHILD,
                RIGHT_PANEL_START_X + 310, 310, 80, 30, hwnd, (HMENU)1110, NULL, NULL);
    
    // Refresh member list
    refreshMemberList(hwnd);
}

// Staff management helper functions
void refreshStaffList(HWND hwnd) {
    HWND list = GetDlgItem(hwnd, 1200);
    ListBox_ResetContent(list);
    
    int count;
    MessageBox(hwnd, "Before getAllStaff", "Debug", MB_OK);
    Staff* staff = getAllStaff(&count);
    MessageBox(hwnd, "After getAllStaff", "Debug", MB_OK);
    if (staff) {
        for (int i = 0; i < count; i++) {
            char buffer[256];
            sprintf(buffer, "%d - %s (%s)", staff[i].staff_id, staff[i].name, staff[i].role);
            ListBox_AddString(list, buffer);
        }
        free(staff);
    }
}

void clearStaffInputs(HWND hwnd) {
    SetWindowText(GetDlgItem(hwnd, 1201), ""); // Name
    SetWindowText(GetDlgItem(hwnd, 1202), ""); // Role
    SetWindowText(GetDlgItem(hwnd, 1203), ""); // Email
    SetWindowText(GetDlgItem(hwnd, 1204), ""); // Phone
}

Staff getStaffFromInputs(HWND hwnd) {
    Staff staff = {0};
    GetWindowText(GetDlgItem(hwnd, 1201), staff.name, sizeof(staff.name));
    GetWindowText(GetDlgItem(hwnd, 1202), staff.role, sizeof(staff.role));
    GetWindowText(GetDlgItem(hwnd, 1203), staff.email, sizeof(staff.email));
    GetWindowText(GetDlgItem(hwnd, 1204), staff.phone, sizeof(staff.phone));
    return staff;
}

void showStaffManagement(HWND hwnd) {
    // Clear existing controls
    clearRightPanel(hwnd);
    
    // Create list box
    CreateWindow("LISTBOX", "", 
                WS_VISIBLE | WS_CHILD | WS_BORDER | LBS_NOTIFY,
                RIGHT_PANEL_START_X + 10, 10, 200, 300, hwnd, (HMENU)1200, NULL, NULL);
    
    // Create input fields
    CreateWindow("STATIC", "Name:", 
                WS_VISIBLE | WS_CHILD,
                RIGHT_PANEL_START_X + 220, 10, 100, 20, hwnd, NULL, NULL, NULL);
    CreateWindow("EDIT", "", 
                WS_VISIBLE | WS_CHILD | WS_BORDER,
                RIGHT_PANEL_START_X + 220, 30, 200, 20, hwnd, (HMENU)1201, NULL, NULL);
    
    CreateWindow("STATIC", "Role:", 
                WS_VISIBLE | WS_CHILD,
                RIGHT_PANEL_START_X + 220, 60, 100, 20, hwnd, NULL, NULL, NULL);
    CreateWindow("EDIT", "", 
                WS_VISIBLE | WS_CHILD | WS_BORDER,
                RIGHT_PANEL_START_X + 220, 80, 200, 20, hwnd, (HMENU)1202, NULL, NULL);
    
    CreateWindow("STATIC", "Email:", 
                WS_VISIBLE | WS_CHILD,
                RIGHT_PANEL_START_X + 220, 110, 100, 20, hwnd, NULL, NULL, NULL);
    CreateWindow("EDIT", "", 
                WS_VISIBLE | WS_CHILD | WS_BORDER,
                RIGHT_PANEL_START_X + 220, 130, 200, 20, hwnd, (HMENU)1203, NULL, NULL);
    
    CreateWindow("STATIC", "Phone:", 
                WS_VISIBLE | WS_CHILD,
                RIGHT_PANEL_START_X + 220, 160, 100, 20, hwnd, NULL, NULL, NULL);
    CreateWindow("EDIT", "", 
                WS_VISIBLE | WS_CHILD | WS_BORDER,
                RIGHT_PANEL_START_X + 220, 180, 200, 20, hwnd, (HMENU)1204, NULL, NULL);
    
    // Create buttons
    CreateWindow("BUTTON", "Add", 
                WS_VISIBLE | WS_CHILD,
                RIGHT_PANEL_START_X + 220, 220, 80, 30, hwnd, (HMENU)1205, NULL, NULL);
    CreateWindow("BUTTON", "Update", 
                WS_VISIBLE | WS_CHILD,
                RIGHT_PANEL_START_X + 310, 220, 80, 30, hwnd, (HMENU)1206, NULL, NULL);
    CreateWindow("BUTTON", "Delete", 
                WS_VISIBLE | WS_CHILD,
                RIGHT_PANEL_START_X + 400, 220, 80, 30, hwnd, (HMENU)1207, NULL, NULL);
    CreateWindow("BUTTON", "Clear", 
                WS_VISIBLE | WS_CHILD,
                RIGHT_PANEL_START_X + 220, 260, 80, 30, hwnd, (HMENU)1208, NULL, NULL);
    CreateWindow("BUTTON", "Back", 
                WS_VISIBLE | WS_CHILD,
                RIGHT_PANEL_START_X + 310, 260, 80, 30, hwnd, (HMENU)1209, NULL, NULL);
    
    // Refresh staff list
    refreshStaffList(hwnd);
}

// Borrowing management helper functions
void refreshBorrowingList(HWND hwnd) {
    HWND list = GetDlgItem(hwnd, 1300);
    ListBox_ResetContent(list);
    
    int count;
    MessageBox(hwnd, "Before getAllBorrowings", "Debug", MB_OK);
    Borrowing* borrowings = getAllBorrowings(&count);
    MessageBox(hwnd, "After getAllBorrowings", "Debug", MB_OK);
    if (borrowings) {
        for (int i = 0; i < count; i++) {
            char buffer[256];
            char borrowDate[20], dueDate[20], returnDate[20];
            formatDate(borrowings[i].borrow_date, borrowDate);
            formatDate(borrowings[i].due_date, dueDate);
            formatDate(borrowings[i].return_date, returnDate);
            
            sprintf(buffer, "%d - Book: %d, Member: %d (Due: %s)", 
                    borrowings[i].borrowing_id,
                    borrowings[i].book_id,
                    borrowings[i].member_id,
                    dueDate);
            ListBox_AddString(list, buffer);
        }
        free(borrowings);
    }
}

void clearBorrowingInputs(HWND hwnd) {
    SetWindowText(GetDlgItem(hwnd, 1301), ""); // Book ID
    SetWindowText(GetDlgItem(hwnd, 1302), ""); // Member ID
    SetWindowText(GetDlgItem(hwnd, 1303), ""); // Staff ID
    SetWindowText(GetDlgItem(hwnd, 1304), ""); // Due Date
}

Borrowing getBorrowingFromInputs(HWND hwnd) {
    Borrowing borrowing = {0};
    char buffer[256];
    
    GetWindowText(GetDlgItem(hwnd, 1301), buffer, sizeof(buffer));
    borrowing.book_id = atoi(buffer);
    
    GetWindowText(GetDlgItem(hwnd, 1302), buffer, sizeof(buffer));
    borrowing.member_id = atoi(buffer);
    
    GetWindowText(GetDlgItem(hwnd, 1303), buffer, sizeof(buffer));
    borrowing.staff_id = atoi(buffer);
    
    GetWindowText(GetDlgItem(hwnd, 1304), buffer, sizeof(buffer));
    borrowing.due_date = parseDate(buffer);
    
    borrowing.borrow_date = time(NULL);
    borrowing.return_date = 0; // Not returned yet
    
    return borrowing;
}

void showBorrowingManagement(HWND hwnd) {
    // Clear existing controls
    clearRightPanel(hwnd);
    
    // Create list box
    CreateWindow("LISTBOX", "", 
                WS_VISIBLE | WS_CHILD | WS_BORDER | LBS_NOTIFY,
                RIGHT_PANEL_START_X + 10, 10, 200, 300, hwnd, (HMENU)1300, NULL, NULL);
    
    // Create input fields
    CreateWindow("STATIC", "Book ID:", 
                WS_VISIBLE | WS_CHILD,
                RIGHT_PANEL_START_X + 220, 10, 100, 20, hwnd, NULL, NULL, NULL);
    CreateWindow("EDIT", "", 
                WS_VISIBLE | WS_CHILD | WS_BORDER,
                RIGHT_PANEL_START_X + 220, 30, 200, 20, hwnd, (HMENU)1301, NULL, NULL);
    
    CreateWindow("STATIC", "Member ID:", 
                WS_VISIBLE | WS_CHILD,
                RIGHT_PANEL_START_X + 220, 60, 100, 20, hwnd, NULL, NULL, NULL);
    CreateWindow("EDIT", "", 
                WS_VISIBLE | WS_CHILD | WS_BORDER,
                RIGHT_PANEL_START_X + 220, 80, 200, 20, hwnd, (HMENU)1302, NULL, NULL);
    
    CreateWindow("STATIC", "Staff ID:", 
                WS_VISIBLE | WS_CHILD,
                RIGHT_PANEL_START_X + 220, 110, 100, 20, hwnd, NULL, NULL, NULL);
    CreateWindow("EDIT", "", 
                WS_VISIBLE | WS_CHILD | WS_BORDER,
                RIGHT_PANEL_START_X + 220, 130, 200, 20, hwnd, (HMENU)1303, NULL, NULL);
    
    CreateWindow("STATIC", "Due Date (YYYY-MM-DD):", 
                WS_VISIBLE | WS_CHILD,
                RIGHT_PANEL_START_X + 220, 160, 150, 20, hwnd, NULL, NULL, NULL);
    CreateWindow("EDIT", "", 
                WS_VISIBLE | WS_CHILD | WS_BORDER,
                RIGHT_PANEL_START_X + 220, 180, 200, 20, hwnd, (HMENU)1304, NULL, NULL);
    
    // Create buttons
    CreateWindow("BUTTON", "Add", 
                WS_VISIBLE | WS_CHILD,
                RIGHT_PANEL_START_X + 220, 220, 80, 30, hwnd, (HMENU)1305, NULL, NULL);
    CreateWindow("BUTTON", "Return", 
                WS_VISIBLE | WS_CHILD,
                RIGHT_PANEL_START_X + 310, 220, 80, 30, hwnd, (HMENU)1306, NULL, NULL);
    CreateWindow("BUTTON", "Delete", 
                WS_VISIBLE | WS_CHILD,
                RIGHT_PANEL_START_X + 400, 220, 80, 30, hwnd, (HMENU)1307, NULL, NULL);
    CreateWindow("BUTTON", "Clear", 
                WS_VISIBLE | WS_CHILD,
                RIGHT_PANEL_START_X + 220, 260, 80, 30, hwnd, (HMENU)1308, NULL, NULL);
    CreateWindow("BUTTON", "Back", 
                WS_VISIBLE | WS_CHILD,
                RIGHT_PANEL_START_X + 310, 260, 80, 30, hwnd, (HMENU)1309, NULL, NULL);
    
    // Refresh borrowing list
    refreshBorrowingList(hwnd);
}

// Fine management helper functions
void refreshFineList(HWND hwnd) {
    HWND list = GetDlgItem(hwnd, 1400);
    ListBox_ResetContent(list);
    
    int count;
    MessageBox(hwnd, "Before getAllFines", "Debug", MB_OK);
    Fine* fines = getAllFines(&count);
    MessageBox(hwnd, "After getAllFines", "Debug", MB_OK);
    if (fines) {
        for (int i = 0; i < count; i++) {
            char buffer[256];
            char datePaid[20];
            formatDate(fines[i].date_paid, datePaid);
            
            sprintf(buffer, "%d - Borrowing: %d, Amount: $%.2f (%s)", 
                    fines[i].fine_id,
                    fines[i].borrowing_id,
                    fines[i].amount,
                    fines[i].paid ? "Paid" : "Unpaid");
            ListBox_AddString(list, buffer);
        }
        free(fines);
    }
}

void clearFineInputs(HWND hwnd) {
    SetWindowText(GetDlgItem(hwnd, 1401), ""); // Borrowing ID
    SetWindowText(GetDlgItem(hwnd, 1402), ""); // Amount
    SetWindowText(GetDlgItem(hwnd, 1403), ""); // Payment Date
}

Fine getFineFromInputs(HWND hwnd) {
    Fine fine = {0};
    char buffer[256];
    
    GetWindowText(GetDlgItem(hwnd, 1401), buffer, sizeof(buffer));
    fine.borrowing_id = atoi(buffer);
    
    GetWindowText(GetDlgItem(hwnd, 1402), buffer, sizeof(buffer));
    fine.amount = atof(buffer);
    
    GetWindowText(GetDlgItem(hwnd, 1403), buffer, sizeof(buffer));
    fine.date_paid = parseDate(buffer);
    
    fine.paid = (fine.date_paid > 0); // If date is set, consider it paid
    
    return fine;
}

void showFineManagement(HWND hwnd) {
    // Clear existing controls
    clearRightPanel(hwnd);
    
    // Create list box
    CreateWindow("LISTBOX", "", 
                WS_VISIBLE | WS_CHILD | WS_BORDER | LBS_NOTIFY,
                RIGHT_PANEL_START_X + 10, 10, 200, 300, hwnd, (HMENU)1400, NULL, NULL);
    
    // Create input fields
    CreateWindow("STATIC", "Borrowing ID:", 
                WS_VISIBLE | WS_CHILD,
                RIGHT_PANEL_START_X + 220, 10, 100, 20, hwnd, NULL, NULL, NULL);
    CreateWindow("EDIT", "", 
                WS_VISIBLE | WS_CHILD | WS_BORDER,
                RIGHT_PANEL_START_X + 220, 30, 200, 20, hwnd, (HMENU)1401, NULL, NULL);
    
    CreateWindow("STATIC", "Amount ($):", 
                WS_VISIBLE | WS_CHILD,
                RIGHT_PANEL_START_X + 220, 60, 100, 20, hwnd, NULL, NULL, NULL);
    CreateWindow("EDIT", "", 
                WS_VISIBLE | WS_CHILD | WS_BORDER,
                RIGHT_PANEL_START_X + 220, 80, 200, 20, hwnd, (HMENU)1402, NULL, NULL);
    
    CreateWindow("STATIC", "Payment Date (YYYY-MM-DD):", 
                WS_VISIBLE | WS_CHILD,
                RIGHT_PANEL_START_X + 220, 110, 150, 20, hwnd, NULL, NULL, NULL);
    CreateWindow("EDIT", "", 
                WS_VISIBLE | WS_CHILD | WS_BORDER,
                RIGHT_PANEL_START_X + 220, 130, 200, 20, hwnd, (HMENU)1403, NULL, NULL);
    
    // Create buttons
    CreateWindow("BUTTON", "Add", 
                WS_VISIBLE | WS_CHILD,
                RIGHT_PANEL_START_X + 220, 170, 80, 30, hwnd, (HMENU)1404, NULL, NULL);
    CreateWindow("BUTTON", "Mark Paid", 
                WS_VISIBLE | WS_CHILD,
                RIGHT_PANEL_START_X + 310, 170, 80, 30, hwnd, (HMENU)1405, NULL, NULL);
    CreateWindow("BUTTON", "Delete", 
                WS_VISIBLE | WS_CHILD,
                RIGHT_PANEL_START_X + 400, 170, 80, 30, hwnd, (HMENU)1406, NULL, NULL);
    CreateWindow("BUTTON", "Clear", 
                WS_VISIBLE | WS_CHILD,
                RIGHT_PANEL_START_X + 220, 210, 80, 30, hwnd, (HMENU)1407, NULL, NULL);
    CreateWindow("BUTTON", "Back", 
                WS_VISIBLE | WS_CHILD,
                RIGHT_PANEL_START_X + 310, 210, 80, 30, hwnd, (HMENU)1408, NULL, NULL);
    
    // Refresh fine list
    refreshFineList(hwnd);
} 