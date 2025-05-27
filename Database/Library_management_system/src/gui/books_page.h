#ifndef BOOKS_PAGE_H
#define BOOKS_PAGE_H

#include <gtk/gtk.h>
#include "../../include/database.h"

// Function to create the books page
GtkWidget* create_books_page(Database *db);

#endif // BOOKS_PAGE_H 