#include <gtk/gtk.h>
#include "../../include/book.h"
#include "../../include/author.h"
#include "../../include/publisher.h"
#include "main_window.h"

// Global variables
extern Database *db;
extern GtkWidget *main_window;

// Function prototypes
static void refresh_books_list(GtkWidget *list_box);
static void add_book_dialog(void);
static void edit_book_dialog(int book_id);
static void delete_book_dialog(int book_id);

// Create and show the books page
void show_books_page(void) {
    // Get the content area from the main window
    GtkWidget *content_area = gtk_bin_get_child(GTK_BIN(gtk_bin_get_child(GTK_BIN(main_window))));
    content_area = gtk_bin_get_child(GTK_BIN(content_area));
    content_area = gtk_bin_get_child(GTK_BIN(content_area));

    // Clear the content area
    GList *children = gtk_container_get_children(GTK_CONTAINER(content_area));
    for (GList *iter = children; iter != NULL; iter = iter->next) {
        gtk_widget_destroy(GTK_WIDGET(iter->data));
    }
    g_list_free(children);

    // Create main vertical box
    GtkWidget *main_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    gtk_container_add(GTK_CONTAINER(content_area), main_box);

    // Create toolbar
    GtkWidget *toolbar = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
    gtk_box_pack_start(GTK_BOX(main_box), toolbar, FALSE, FALSE, 0);

    // Add book button
    GtkWidget *add_btn = gtk_button_new_with_label("Add Book");
    g_signal_connect(add_btn, "clicked", G_CALLBACK(add_book_dialog), NULL);
    gtk_box_pack_start(GTK_BOX(toolbar), add_btn, FALSE, FALSE, 5);

    // Create scrolled window for the list
    GtkWidget *scrolled = gtk_scrolled_window_new(NULL, NULL);
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrolled),
                                 GTK_POLICY_AUTOMATIC,
                                 GTK_POLICY_AUTOMATIC);
    gtk_box_pack_start(GTK_BOX(main_box), scrolled, TRUE, TRUE, 0);

    // Create list box
    GtkWidget *list_box = gtk_list_box_new();
    gtk_container_add(GTK_CONTAINER(scrolled), list_box);

    // Refresh the list
    refresh_books_list(list_box);

    // Show all widgets
    gtk_widget_show_all(content_area);
}

// Refresh the books list
static void refresh_books_list(GtkWidget *list_box) {
    // Clear the list
    GList *children = gtk_container_get_children(GTK_CONTAINER(list_box));
    for (GList *iter = children; iter != NULL; iter = iter->next) {
        gtk_widget_destroy(GTK_WIDGET(iter->data));
    }
    g_list_free(children);

    // Get all books
    BookList *books = book_list_all(db);
    if (books != NULL) {
        for (int i = 0; i < books->count; i++) {
            Book *book = books->books[i];
            
            // Create row box
            GtkWidget *row_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
            
            // Create labels
            char id_str[32];
            snprintf(id_str, sizeof(id_str), "%d", book->book_id);
            GtkWidget *id_label = gtk_label_new(id_str);
            GtkWidget *title_label = gtk_label_new(book->title);
            GtkWidget *isbn_label = gtk_label_new(book->isbn);
            
            // Add labels to row
            gtk_box_pack_start(GTK_BOX(row_box), id_label, FALSE, FALSE, 5);
            gtk_box_pack_start(GTK_BOX(row_box), title_label, TRUE, TRUE, 5);
            gtk_box_pack_start(GTK_BOX(row_box), isbn_label, FALSE, FALSE, 5);
            
            // Create buttons
            GtkWidget *edit_btn = gtk_button_new_with_label("Edit");
            GtkWidget *delete_btn = gtk_button_new_with_label("Delete");
            
            // Connect button signals
            g_signal_connect(edit_btn, "clicked", G_CALLBACK(edit_book_dialog), GINT_TO_POINTER(book->book_id));
            g_signal_connect(delete_btn, "clicked", G_CALLBACK(delete_book_dialog), GINT_TO_POINTER(book->book_id));
            
            // Add buttons to row
            gtk_box_pack_start(GTK_BOX(row_box), edit_btn, FALSE, FALSE, 5);
            gtk_box_pack_start(GTK_BOX(row_box), delete_btn, FALSE, FALSE, 5);
            
            // Add row to list
            gtk_container_add(GTK_CONTAINER(list_box), row_box);
        }
        book_list_free(books);
    }
}

// Add book dialog
static void add_book_dialog(void) {
    // Create dialog
    GtkWidget *dialog = gtk_dialog_new_with_buttons("Add Book",
                                                  GTK_WINDOW(main_window),
                                                  GTK_DIALOG_MODAL | GTK_DIALOG_DESTROY_WITH_PARENT,
                                                  "Add",
                                                  GTK_RESPONSE_ACCEPT,
                                                  "Cancel",
                                                  GTK_RESPONSE_REJECT,
                                                  NULL);

    // Create form
    GtkWidget *content_area = gtk_dialog_get_content_area(GTK_DIALOG(dialog));
    GtkWidget *form = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    gtk_container_add(GTK_CONTAINER(content_area), form);

    // Create entry fields
    GtkWidget *title_entry = gtk_entry_new();
    GtkWidget *isbn_entry = gtk_entry_new();
    GtkWidget *category_entry = gtk_entry_new();
    GtkWidget *author_id_entry = gtk_entry_new();
    GtkWidget *publisher_id_entry = gtk_entry_new();
    GtkWidget *year_entry = gtk_entry_new();
    GtkWidget *price_entry = gtk_entry_new();

    // Add fields to form
    gtk_box_pack_start(GTK_BOX(form), gtk_label_new("Title:"), FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(form), title_entry, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(form), gtk_label_new("ISBN:"), FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(form), isbn_entry, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(form), gtk_label_new("Category:"), FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(form), category_entry, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(form), gtk_label_new("Author ID:"), FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(form), author_id_entry, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(form), gtk_label_new("Publisher ID:"), FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(form), publisher_id_entry, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(form), gtk_label_new("Publication Year:"), FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(form), year_entry, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(form), gtk_label_new("Price:"), FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(form), price_entry, FALSE, FALSE, 0);

    // Show dialog
    gtk_widget_show_all(dialog);

    // Handle response
    gint response = gtk_dialog_run(GTK_DIALOG(dialog));
    if (response == GTK_RESPONSE_ACCEPT) {
        // Get values
        const char *title = gtk_entry_get_text(GTK_ENTRY(title_entry));
        const char *isbn = gtk_entry_get_text(GTK_ENTRY(isbn_entry));
        const char *category = gtk_entry_get_text(GTK_ENTRY(category_entry));
        int author_id = atoi(gtk_entry_get_text(GTK_ENTRY(author_id_entry)));
        int publisher_id = atoi(gtk_entry_get_text(GTK_ENTRY(publisher_id_entry)));
        int year = atoi(gtk_entry_get_text(GTK_ENTRY(year_entry)));
        double price = atof(gtk_entry_get_text(GTK_ENTRY(price_entry)));

        // Create and save book
        Book *book = book_create(title, isbn, category, author_id, publisher_id, year, price);
        if (book != NULL) {
            if (book_save(db, book)) {
                // Refresh the list
                GtkWidget *list_box = gtk_bin_get_child(GTK_BIN(gtk_bin_get_child(GTK_BIN(main_window))));
                list_box = gtk_bin_get_child(GTK_BIN(list_box));
                list_box = gtk_bin_get_child(GTK_BIN(list_box));
                list_box = gtk_bin_get_child(GTK_BIN(list_box));
                list_box = gtk_bin_get_child(GTK_BIN(list_box));
                refresh_books_list(list_box);
            }
            book_free(book);
        }
    }

    gtk_widget_destroy(dialog);
}

// Edit book dialog
static void edit_book_dialog(int book_id) {
    // Get book
    Book *book = book_find_by_id(db, book_id);
    if (book == NULL) {
        return;
    }

    // Create dialog
    GtkWidget *dialog = gtk_dialog_new_with_buttons("Edit Book",
                                                  GTK_WINDOW(main_window),
                                                  GTK_DIALOG_MODAL | GTK_DIALOG_DESTROY_WITH_PARENT,
                                                  "Save",
                                                  GTK_RESPONSE_ACCEPT,
                                                  "Cancel",
                                                  GTK_RESPONSE_REJECT,
                                                  NULL);

    // Create form
    GtkWidget *content_area = gtk_dialog_get_content_area(GTK_DIALOG(dialog));
    GtkWidget *form = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    gtk_container_add(GTK_CONTAINER(content_area), form);

    // Create entry fields
    GtkWidget *title_entry = gtk_entry_new();
    GtkWidget *isbn_entry = gtk_entry_new();
    GtkWidget *category_entry = gtk_entry_new();
    GtkWidget *author_id_entry = gtk_entry_new();
    GtkWidget *publisher_id_entry = gtk_entry_new();
    GtkWidget *year_entry = gtk_entry_new();
    GtkWidget *price_entry = gtk_entry_new();

    // Set initial values
    gtk_entry_set_text(GTK_ENTRY(title_entry), book->title);
    gtk_entry_set_text(GTK_ENTRY(isbn_entry), book->isbn);
    gtk_entry_set_text(GTK_ENTRY(category_entry), book->category);
    char author_id_str[32], publisher_id_str[32], year_str[32], price_str[32];
    snprintf(author_id_str, sizeof(author_id_str), "%d", book->author_id);
    snprintf(publisher_id_str, sizeof(publisher_id_str), "%d", book->publisher_id);
    snprintf(year_str, sizeof(year_str), "%d", book->publication_year);
    snprintf(price_str, sizeof(price_str), "%.2f", book->price);
    gtk_entry_set_text(GTK_ENTRY(author_id_entry), author_id_str);
    gtk_entry_set_text(GTK_ENTRY(publisher_id_entry), publisher_id_str);
    gtk_entry_set_text(GTK_ENTRY(year_entry), year_str);
    gtk_entry_set_text(GTK_ENTRY(price_entry), price_str);

    // Add fields to form
    gtk_box_pack_start(GTK_BOX(form), gtk_label_new("Title:"), FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(form), title_entry, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(form), gtk_label_new("ISBN:"), FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(form), isbn_entry, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(form), gtk_label_new("Category:"), FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(form), category_entry, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(form), gtk_label_new("Author ID:"), FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(form), author_id_entry, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(form), gtk_label_new("Publisher ID:"), FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(form), publisher_id_entry, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(form), gtk_label_new("Publication Year:"), FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(form), year_entry, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(form), gtk_label_new("Price:"), FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(form), price_entry, FALSE, FALSE, 0);

    // Show dialog
    gtk_widget_show_all(dialog);

    // Handle response
    gint response = gtk_dialog_run(GTK_DIALOG(dialog));
    if (response == GTK_RESPONSE_ACCEPT) {
        // Get values
        const char *title = gtk_entry_get_text(GTK_ENTRY(title_entry));
        const char *isbn = gtk_entry_get_text(GTK_ENTRY(isbn_entry));
        const char *category = gtk_entry_get_text(GTK_ENTRY(category_entry));
        int author_id = atoi(gtk_entry_get_text(GTK_ENTRY(author_id_entry)));
        int publisher_id = atoi(gtk_entry_get_text(GTK_ENTRY(publisher_id_entry)));
        int year = atoi(gtk_entry_get_text(GTK_ENTRY(year_entry)));
        double price = atof(gtk_entry_get_text(GTK_ENTRY(price_entry)));

        // Update book
        strncpy(book->title, title, sizeof(book->title) - 1);
        strncpy(book->isbn, isbn, sizeof(book->isbn) - 1);
        strncpy(book->category, category, sizeof(book->category) - 1);
        book->author_id = author_id;
        book->publisher_id = publisher_id;
        book->publication_year = year;
        book->price = price;

        if (book_update(db, book)) {
            // Refresh the list
            GtkWidget *list_box = gtk_bin_get_child(GTK_BIN(gtk_bin_get_child(GTK_BIN(main_window))));
            list_box = gtk_bin_get_child(GTK_BIN(list_box));
            list_box = gtk_bin_get_child(GTK_BIN(list_box));
            list_box = gtk_bin_get_child(GTK_BIN(list_box));
            list_box = gtk_bin_get_child(GTK_BIN(list_box));
            refresh_books_list(list_box);
        }
    }

    gtk_widget_destroy(dialog);
    book_free(book);
}

// Delete book dialog
static void delete_book_dialog(int book_id) {
    // Create confirmation dialog
    GtkWidget *dialog = gtk_dialog_new_with_buttons("Delete Book",
                                                  GTK_WINDOW(main_window),
                                                  GTK_DIALOG_MODAL | GTK_DIALOG_DESTROY_WITH_PARENT,
                                                  "Delete",
                                                  GTK_RESPONSE_ACCEPT,
                                                  "Cancel",
                                                  GTK_RESPONSE_REJECT,
                                                  NULL);

    // Add message
    GtkWidget *content_area = gtk_dialog_get_content_area(GTK_DIALOG(dialog));
    GtkWidget *label = gtk_label_new("Are you sure you want to delete this book?");
    gtk_container_add(GTK_CONTAINER(content_area), label);

    // Show dialog
    gtk_widget_show_all(dialog);

    // Handle response
    gint response = gtk_dialog_run(GTK_DIALOG(dialog));
    if (response == GTK_RESPONSE_ACCEPT) {
        if (book_delete(db, book_id)) {
            // Refresh the list
            GtkWidget *list_box = gtk_bin_get_child(GTK_BIN(gtk_bin_get_child(GTK_BIN(main_window))));
            list_box = gtk_bin_get_child(GTK_BIN(list_box));
            list_box = gtk_bin_get_child(GTK_BIN(list_box));
            list_box = gtk_bin_get_child(GTK_BIN(list_box));
            list_box = gtk_bin_get_child(GTK_BIN(list_box));
            refresh_books_list(list_box);
        }
    }

    gtk_widget_destroy(dialog);
} 