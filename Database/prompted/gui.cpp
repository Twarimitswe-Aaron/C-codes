#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <dwmapi.h>
#include <gdiplus.h>
#include <commctrl.h>
#include "manager.h"

#pragma comment(lib, "dwmapi.lib")
#pragma comment(lib, "gdiplus.lib")
#pragma comment(lib, "comctl32.lib")

#include "database.h"
#include "interface.h"

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
HBRUSH hBrushLight = NULL;
HBRUSH hBrushDark = NULL;
HBRUSH hBrushAccent = NULL;
BOOL isDarkTheme = FALSE;
HFONT hFont = NULL;
HFONT hFontBold = NULL;
HFONT hFontButton = NULL;
HFONT hFontTitle = NULL;
GdiplusStartupInput gdiplusStartupInput;
ULONG_PTR gdiplusToken;

// Modern color scheme
COLORREF colorPrimary = RGB(0, 120, 215);
COLORREF colorPrimaryDark = RGB(0, 99, 177);
COLORREF colorAccent = RGB(0, 174, 239);
COLORREF colorSuccess = RGB(46, 204, 113);
COLORREF colorError = RGB(231, 76, 60);
COLORREF colorWarning = RGB(241, 196, 15);
COLORREF colorLightBg = RGB(250, 250, 250);
COLORREF colorDarkBg = RGB(25, 25, 30);
COLORREF colorCardLight = RGB(255, 255, 255);
COLORREF colorCardDark = RGB(40, 40, 45);

// Define dimensions
const int WINDOW_WIDTH = 1100;
const int WINDOW_HEIGHT = 750;
const int BUTTON_WIDTH = 200;
const int BUTTON_HEIGHT = 50;
const int BUTTON_SPACING = 20;
const int MARGIN = 30;
const int CONTROL_HEIGHT = 30;
const int LABEL_WIDTH = 150;
const int EDIT_WIDTH = 250;

const char* buttonTexts[] = {
    "Books", "Authors", "Publishers",
    "Members", "Staff", "Borrowings",
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

// Icons (using Unicode characters as simple icons)
const WCHAR* icons[] = {
    L"\U0001F4D6", L"\U0001F464", L"\U0001F4D2",
    L"\U0001F465", L"\U0001F46E", L"\U0001F4D5",
    L"\U0001F4B0", L"\U0001F6AA"
};

// Helper function to create modern buttons
HWND CreateModernButton(HWND hParent, LPCWSTR text, LPCWSTR icon, int x, int y, int width, int height, HMENU id, HINSTANCE hInst)
{
    HWND hButton = CreateWindowW(
        L"BUTTON", L"",
        WS_VISIBLE | WS_CHILD | BS_OWNERDRAW,
        x, y, width, height,
        hParent, id, hInst, NULL);
    
    return hButton;
}

// Function to draw rounded rectangle
void DrawRoundedRect(HDC hdc, int x, int y, int width, int height, int radius, HBRUSH hBrush)
{
    Gdiplus::Graphics graphics(hdc);
    Gdiplus::Color color;
    GetHBRUSHColor(hBrush, &color);
    Gdiplus::SolidBrush brush(color);
    
    graphics.SetSmoothingMode(Gdiplus::SmoothingModeAntiAlias);
    graphics.FillRectangle(&brush, x, y, width, height);
    
    // Draw rounded corners
    Gdiplus::GraphicsPath path;
    path.AddLine(x + radius, y, x + width - radius, y);
    path.AddArc(x + width - radius, y, radius, radius, 270, 90);
    path.AddLine(x + width, y + radius, x + width, y + height - radius);
    path.AddArc(x + width - radius, y + height - radius, radius, radius, 0, 90);
    path.AddLine(x + width - radius, y + height, x + radius, y + height);
    path.AddArc(x, y + height - radius, radius, radius, 90, 90);
    path.AddLine(x, y + height - radius, x, y + radius);
    path.AddArc(x, y, radius, radius, 180, 90);
    
    graphics.FillPath(&brush, &path);
}

// Function to draw modern button
void DrawModernButton(HWND hwnd, LPDRAWITEMSTRUCT lpDrawItem, LPCWSTR text, LPCWSTR icon, BOOL isDark)
{
    HDC hdc = lpDrawItem->hDC;
    RECT rc = lpDrawItem->rcItem;
    int state = lpDrawItem->itemState;
    
    // Set colors based on theme and button state
    COLORREF bgColor, textColor, borderColor;
    if (state & ODS_SELECTED) {
        bgColor = isDark ? RGB(0, 99, 177) : RGB(0, 120, 215);
        textColor = RGB(255, 255, 255);
        borderColor = isDark ? RGB(0, 99, 177) : RGB(0, 120, 215);
    } else {
        bgColor = isDark ? RGB(40, 40, 45) : RGB(240, 240, 245);
        textColor = isDark ? RGB(255, 255, 255) : RGB(0, 0, 0);
        borderColor = isDark ? RGB(70, 70, 75) : RGB(220, 220, 225);
    }
    
    // Create brushes
    HBRUSH hBrushBg = CreateSolidBrush(bgColor);
    HBRUSH hBrushBorder = CreateSolidBrush(borderColor);
    HPEN hPenBorder = CreatePen(PS_SOLID, 1, borderColor);
    
    // Draw background with rounded corners
    SelectObject(hdc, hPenBorder);
    SelectObject(hdc, hBrushBg);
    RoundRect(hdc, rc.left, rc.top, rc.right, rc.bottom, 8, 8);
    
    // Draw icon and text
    HFONT hOldFont = (HFONT)SelectObject(hdc, hFontButton);
    SetBkMode(hdc, TRANSPARENT);
    SetTextColor(hdc, textColor);
    
    // Calculate text position
    RECT textRect = rc;
    textRect.left += 10;
    
    // Draw icon
    if (icon) {
        SetTextColor(hdc, isDark ? colorAccent : colorPrimary);
        DrawTextW(hdc, icon, -1, &textRect, DT_LEFT | DT_SINGLELINE | DT_VCENTER);
        textRect.left += 30; // Space for icon
    }
    
    // Draw text
    SetTextColor(hdc, textColor);
    DrawTextW(hdc, text, -1, &textRect, DT_LEFT | DT_SINGLELINE | DT_VCENTER);
    
    // Clean up
    SelectObject(hdc, hOldFont);
    DeleteObject(hBrushBg);
    DeleteObject(hBrushBorder);
    DeleteObject(hPenBorder);
}

BOOL CALLBACK SetChildTheme(HWND hwndChild, LPARAM lParam) {
    InvalidateRect(hwndChild, NULL, TRUE);
    UpdateWindow(hwndChild);
    return TRUE;
}

LRESULT CALLBACK WindowProcedure(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp) {
    static HBRUSH hBrushCurrent = NULL;
    static HBRUSH hBrushCard = NULL;

    switch(msg) {
        case WM_CREATE:
            // Initialize GDI+
            GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);
            
            // Initialize brushes
            hBrushLight = CreateSolidBrush(colorLightBg);
            hBrushDark = CreateSolidBrush(colorDarkBg);
            hBrushAccent = CreateSolidBrush(colorAccent);
            hBrushCard = CreateSolidBrush(isDarkTheme ? colorCardDark : colorCardLight);
            hBrushCurrent = isDarkTheme ? hBrushDark : hBrushLight;

            // Create fonts
            hFont = CreateFont(
                16, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, DEFAULT_CHARSET,
                OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY,
                DEFAULT_PITCH | FF_SWISS, L"Segoe UI");

            hFontBold = CreateFont(
                18, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE,
                DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
                CLEARTYPE_QUALITY, DEFAULT_PITCH | FF_SWISS, L"Segoe UI");

            hFontButton = CreateFont(
                16, 0, 0, 0, FW_SEMIBOLD, FALSE, FALSE, FALSE,
                DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
                CLEARTYPE_QUALITY, DEFAULT_PITCH | FF_SWISS, L"Segoe UI");

            hFontTitle = CreateFont(
                28, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE,
                DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
                CLEARTYPE_QUALITY, DEFAULT_PITCH | FF_SWISS, L"Segoe UI");

            // Create header
            HWND hTitle = CreateWindowW(L"STATIC", L"Library Management System",
                WS_VISIBLE | WS_CHILD | SS_CENTER,
                MARGIN, MARGIN, WINDOW_WIDTH - 2 * MARGIN, 60,
                hwnd, NULL, GetModuleHandle(NULL), NULL);
            SendMessage(hTitle, WM_SETFONT, (WPARAM)hFontTitle, TRUE);

            // Create buttons with modern style
            int startY = MARGIN + 80;
            int startX = MARGIN;
            int buttonsPerRow = 3;
            int currentRow = 0;

            for(int i = 0; i < 8; i++) {
                hwndButton[i] = CreateModernButton(
                    hwnd, 
                    (LPCWSTR)CA2W(buttonTexts[i]), 
                    icons[i],
                    startX + (i % buttonsPerRow) * (BUTTON_WIDTH + BUTTON_SPACING),
                    startY + (i / buttonsPerRow) * (BUTTON_HEIGHT + BUTTON_SPACING),
                    BUTTON_WIDTH, BUTTON_HEIGHT,
                    (HMENU)(intptr_t)buttonIds[i],
                    GetModuleHandle(NULL));
            }

            // Theme toggle and clock buttons
            hwndButton[8] = CreateModernButton(
                hwnd, 
                isDarkTheme ? L"☀ Light" : L"☾ Dark", 
                NULL,
                WINDOW_WIDTH - 2 * MARGIN - 100, MARGIN,
                100, 40, 
                (HMENU)ID_THEME_TOGGLE,
                GetModuleHandle(NULL));

            hwndButton[9] = CreateModernButton(
                hwnd, 
                L"⏱ Time", 
                NULL,
                WINDOW_WIDTH - MARGIN - 100, MARGIN,
                100, 40, 
                (HMENU)ID_CLOCK,
                GetModuleHandle(NULL));
            
            // Enable window shadow effect
            const DWORD DWMWA_USE_IMMERSIVE_DARK_MODE = 20;
            BOOL darkMode = isDarkTheme;
            DwmSetWindowAttribute(hwnd, DWMWA_USE_IMMERSIVE_DARK_MODE, &darkMode, sizeof(darkMode));
            break;

        case WM_DRAWITEM:
            if (wp >= ID_BOOK_MGMT && wp <= ID_CLOCK) {
                int index = -1;
                for (int i = 0; i < 10; i++) {
                    if (buttonIds[i] == wp) {
                        index = i;
                        break;
                    }
                }
                if (index != -1) {
                    LPDRAWITEMSTRUCT lpDrawItem = (LPDRAWITEMSTRUCT)lp;
                    if (index < 8) {
                        DrawModernButton(hwnd, lpDrawItem, (LPCWSTR)CA2W(buttonTexts[index]), icons[index], isDarkTheme);
                    } else if (wp == ID_THEME_TOGGLE) {
                        DrawModernButton(hwnd, lpDrawItem, isDarkTheme ? L"☀ Light" : L"☾ Dark", NULL, isDarkTheme);
                    } else if (wp == ID_CLOCK) {
                        DrawModernButton(hwnd, lpDrawItem, L"⏱ Time", NULL, isDarkTheme);
                    }
                }
            }
            return TRUE;

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
                    hBrushCard = CreateSolidBrush(isDarkTheme ? colorCardDark : colorCardLight);
                    
                    // Update theme for all windows
                    const DWORD DWMWA_USE_IMMERSIVE_DARK_MODE = 20;
                    BOOL darkMode = isDarkTheme;
                    DwmSetWindowAttribute(hwnd, DWMWA_USE_IMMERSIVE_DARK_MODE, &darkMode, sizeof(darkMode));
                    
                    // Update theme toggle button text
                    SetWindowTextW(hwndButton[8], isDarkTheme ? L"☀ Light" : L"☾ Dark");
                    
                    InvalidateRect(hwnd, NULL, TRUE);
                    UpdateWindow(hwnd);
                    EnumChildWindows(hwnd, SetChildTheme, 0);
                    break;
                case ID_CLOCK:
                    {
                        time_t now = time(0);
                        struct tm tstruct;
                        char timeStr[80];
                        char dateStr[80];
                        localtime_s(&tstruct, &now);
                        strftime(timeStr, sizeof(timeStr), "%I:%M %p", &tstruct);
                        strftime(dateStr, sizeof(dateStr), "%B %d, %Y", &tstruct);
                        
                        char message[200];
                        sprintf_s(message, "Current Time: %s\nDate: %s", timeStr, dateStr);
                        
                        // Create a modern-looking message box
                        MSGBOXPARAMS mbp = {0};
                        mbp.cbSize = sizeof(MSGBOXPARAMS);
                        mbp.hwndOwner = hwnd;
                        mbp.hInstance = GetModuleHandle(NULL);
                        mbp.lpszText = message;
                        mbp.lpszCaption = "Digital Clock";
                        mbp.dwStyle = MB_USERICON | MB_OK;
                        mbp.lpszIcon = MAKEINTRESOURCE(32518); // IDI_INFORMATION
                        mbp.dwContextHelpId = 0;
                        mbp.lpfnMsgBoxCallback = NULL;
                        mbp.dwLanguageId = MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT);
                        
                        MessageBoxIndirect(&mbp);
                    }
                    break;
            }
            break;

        case WM_CTLCOLORSTATIC:
            {
                HDC hdc = (HDC)wp;
                if (isDarkTheme) {
                    SetTextColor(hdc, RGB(255, 255, 255));
                    SetBkColor(hdc, colorDarkBg);
                    return (LRESULT)hBrushDark;
                } else {
                    SetTextColor(hdc, RGB(0, 0, 0));
                    SetBkColor(hdc, colorLightBg);
                    return (LRESULT)hBrushLight;
                }
            }

        case WM_ERASEBKGND:
            {
                HDC hdc = (HDC)wp;
                RECT rc;
                GetClientRect(hwnd, &rc);
                
                // Fill background with gradient
                Gdiplus::Graphics graphics(hdc);
                Gdiplus::LinearGradientBrush brush(
                    Gdiplus::Point(0, 0), 
                    Gdiplus::Point(0, rc.bottom),
                    isDarkTheme ? Gdiplus::Color(30, 30, 35) : Gdiplus::Color(240, 245, 250),
                    isDarkTheme ? Gdiplus::Color(20, 20, 25) : Gdiplus::Color(230, 235, 240));
                
                graphics.FillRectangle(&brush, 0, 0, rc.right, rc.bottom);
                
                // Add some digital-style decoration
                Gdiplus::Pen pen(isDarkTheme ? Gdiplus::Color(60, 60, 65) : Gdiplus::Color(220, 225, 230), 1);
                for (int i = 0; i < rc.right; i += 20) {
                    graphics.DrawLine(&pen, i, 0, i, rc.bottom);
                }
                for (int i = 0; i < rc.bottom; i += 20) {
                    graphics.DrawLine(&pen, 0, i, rc.right, i);
                }
                
                // Add a subtle grid effect
                Gdiplus::Pen gridPen(isDarkTheme ? Gdiplus::Color(40, 40, 45, 50) : Gdiplus::Color(210, 215, 220, 50), 0.5f);
                for (int i = 0; i < rc.right; i += 5) {
                    graphics.DrawLine(&gridPen, i, 0, i, rc.bottom);
                }
                for (int i = 0; i < rc.bottom; i += 5) {
                    graphics.DrawLine(&gridPen, 0, i, rc.right, i);
                }
                
                return TRUE;
            }

        case WM_PAINT:
            {
                PAINTSTRUCT ps;
                HDC hdc = BeginPaint(hwnd, &ps);
                
                // Draw a card-like background for the buttons
                RECT cardRect = {
                    MARGIN - 10,
                    MARGIN + 70,
                    WINDOW_WIDTH - MARGIN + 10,
                    MARGIN + 70 + (2 * (BUTTON_HEIGHT + BUTTON_SPACING)) + 20
                };
                
                Gdiplus::Graphics graphics(hdc);
                Gdiplus::SolidBrush cardBrush(isDarkTheme ? Gdiplus::Color(40, 40, 45) : Gdiplus::Color(240, 240, 245));
                graphics.FillRectangle(&cardBrush, cardRect.left, cardRect.top, 
                                     cardRect.right - cardRect.left, cardRect.bottom - cardRect.top);
                
                // Add a subtle shadow
                Gdiplus::Color shadowColor(0, 0, 0, 30);
                Gdiplus::SolidBrush shadowBrush(shadowColor);
                graphics.FillRectangle(&shadowBrush, cardRect.left + 3, cardRect.top + 3, 
                                      cardRect.right - cardRect.left, cardRect.bottom - cardRect.top);
                
                // Add a border
                Gdiplus::Pen borderPen(isDarkTheme ? Gdiplus::Color(60, 60, 65) : Gdiplus::Color(220, 220, 225), 1);
                graphics.DrawRectangle(&borderPen, cardRect.left, cardRect.top, 
                                     cardRect.right - cardRect.left, cardRect.bottom - cardRect.top);
                
                EndPaint(hwnd, &ps);
                break;
            }

        case WM_DESTROY:
            DeleteObject(hBrushLight);
            DeleteObject(hBrushDark);
            DeleteObject(hBrushAccent);
            DeleteObject(hBrushCard);
            DeleteObject(hFont);
            DeleteObject(hFontBold);
            DeleteObject(hFontButton);
            DeleteObject(hFontTitle);
            GdiplusShutdown(gdiplusToken);
            PostQuitMessage(0);
            break;

        default:
            return DefWindowProc(hwnd, msg, wp, lp);
    }
    return 0;
}

// [Rest of the code remains the same for the management windows, 
// but you would apply similar modern styling techniques to each of them]

// Note: For brevity, I've shown the main window procedure updates. 
// Similar modern styling should be applied to all the management windows
// (BookManagementWindowProcedure, AuthorManagementWindowProcedure, etc.)
// following the same pattern of using GDI+ for rounded corners, 
// gradients, shadows, and modern controls.