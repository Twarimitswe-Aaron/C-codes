#ifndef GUI_H
#define GUI_H

#include <windows.h>
#include "structures.h"

// Main GUI functions
void startGUI();
void showMainMenu(HWND hwnd);
void clearRightPanel(HWND hwnd);
void showBookManagement(HWND hwnd);
void showAuthorManagement(HWND hwnd);
void showPublisherManagement(HWND hwnd);
void showMemberManagement(HWND hwnd);
void showStaffManagement(HWND hwnd);
void showBorrowingManagement(HWND hwnd);
void showFineManagement(HWND hwnd);
void clearWindow(HWND hwnd);

// Helper functions
void createModernButton(HWND hwnd, const char* text, int x, int y, int width, int height, int id);
void createModernInput(HWND hwnd, int x, int y, int width, int height, int id);
HWND createModernLabel(HWND hwnd, const char* text, int x, int y, int width, int height);
void createModernListView(HWND hwnd, int x, int y, int width, int height, int id);

// Book management helper functions
void refreshBookList(HWND hwnd);
void clearBookInputs(HWND hwnd);
Book getBookFromInputs(HWND hwnd);

// Author management helper functions
void refreshAuthorList(HWND hwnd);
void clearAuthorInputs(HWND hwnd);
Author getAuthorFromInputs(HWND hwnd);

// Publisher management helper functions
void refreshPublisherList(HWND hwnd);
void clearPublisherInputs(HWND hwnd);
Publisher getPublisherFromInputs(HWND hwnd);

// Member management helper functions
void refreshMemberList(HWND hwnd);
void clearMemberInputs(HWND hwnd);
Member getMemberFromInputs(HWND hwnd);

// Staff management helper functions
void refreshStaffList(HWND hwnd);
void clearStaffInputs(HWND hwnd);
Staff getStaffFromInputs(HWND hwnd);

// Borrowing management helper functions
void refreshBorrowingList(HWND hwnd);
void clearBorrowingInputs(HWND hwnd);
Borrowing getBorrowingFromInputs(HWND hwnd);

// Fine management helper functions
void refreshFineList(HWND hwnd);
void clearFineInputs(HWND hwnd);
Fine getFineFromInputs(HWND hwnd);

// Window procedures
LRESULT CALLBACK MainWindowProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp);
LRESULT CALLBACK BookWindowProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp);
LRESULT CALLBACK AuthorWindowProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp);
LRESULT CALLBACK PublisherWindowProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp);
LRESULT CALLBACK MemberWindowProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp);
LRESULT CALLBACK StaffWindowProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp);
LRESULT CALLBACK BorrowingWindowProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp);
LRESULT CALLBACK FineWindowProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp);

// Layout constants
#define LEFT_PANEL_WIDTH 250
#define RIGHT_PANEL_START_X (LEFT_PANEL_WIDTH + 10)
#define CARD_PADDING 3

#endif // GUI_H 