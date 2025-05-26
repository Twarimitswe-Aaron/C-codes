#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include <gtk/gtk.h>
#include "database.h"

// Function prototypes
void create_main_window(void);
void on_window_destroy(GtkWidget *widget, gpointer data);
void show_books_page(void);
void show_authors_page(void);
void show_publishers_page(void);
void show_members_page(void);
void show_staff_page(void);
void show_borrowings_page(void);
void show_fines_page(void);

#endif // MAIN_WINDOW_H 