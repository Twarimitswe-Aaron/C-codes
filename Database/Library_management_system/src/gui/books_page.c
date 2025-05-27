#include <gtk/gtk.h>
#include "../../include/database.h"
#include "../../include/book.h"
#include "../../include/author.h"
#include "../../include/publisher.h"
#include "books_page.h"

// Global variables
static GtkWidget *books_page;
static GtkWidget *books_list;
static GtkWidget *search_entry;
static GtkListStore *books_store;
static GtkWidget *add_book_dialog;
static GtkWidget *edit_book_dialog;
static GtkWidget *main_window;  // Add main window reference
static Database *db;  // Add database reference

// Function prototypes
static void refresh_books_list(Database *db);
static void add_book_dialog_show(Database *db);
static void edit_book_dialog_show(Database *db, int book_id);
static void delete_book_dialog_show(Database *db, int book_id);
static void search_books(Database *db, const char *query);
static void show_popup_menu(GtkWidget *treeview, GdkEventButton *event);
static void on_edit_book(GtkWidget *widget, gpointer data);
static void on_delete_book(GtkWidget *widget, gpointer data);
static void on_add_book_clicked(GtkWidget *widget, gpointer data);
static void on_add_book_response(GtkDialog *dialog, gint response_id, gpointer data);

// Create books page
GtkWidget* create_books_page(Database *database) {
    db = database;  // Store database reference
    main_window = gtk_widget_get_toplevel(books_page);  // Get main window reference

    // Create main vertical box
    books_page = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    gtk_container_set_border_width(GTK_CONTAINER(books_page), 10);

    // Create header with title and add button
    GtkWidget *header_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
    GtkWidget *title = gtk_label_new("Books");
    gtk_label_set_xalign(GTK_LABEL(title), 0);
    gtk_widget_set_hexpand(title, TRUE);
    
    GtkWidget *add_button = gtk_button_new_with_label("Add Book");
    g_signal_connect(add_button, "clicked", G_CALLBACK(on_add_book_clicked), db);
    
    gtk_box_pack_start(GTK_BOX(header_box), title, FALSE, FALSE, 0);
    gtk_box_pack_end(GTK_BOX(header_box), add_button, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(books_page), header_box, FALSE, FALSE, 0);

    // Create scrolled window for the list
    GtkWidget *scrolled = gtk_scrolled_window_new(NULL, NULL);
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrolled),
                                 GTK_POLICY_AUTOMATIC,
                                 GTK_POLICY_AUTOMATIC);
    gtk_box_pack_start(GTK_BOX(books_page), scrolled, TRUE, TRUE, 0);

    // Create tree view
    books_list = gtk_tree_view_new();
    gtk_tree_view_set_headers_visible(GTK_TREE_VIEW(books_list), TRUE);
    gtk_container_add(GTK_CONTAINER(scrolled), books_list);

    // Add columns
    GtkCellRenderer *renderer;
    GtkTreeViewColumn *column;

    // ID column
    renderer = gtk_cell_renderer_text_new();
    column = gtk_tree_view_column_new_with_attributes("ID", renderer, "text", 0, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(books_list), column);

    // Title column
    renderer = gtk_cell_renderer_text_new();
    column = gtk_tree_view_column_new_with_attributes("Title", renderer, "text", 1, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(books_list), column);

    // Author column
    renderer = gtk_cell_renderer_text_new();
    column = gtk_tree_view_column_new_with_attributes("Author", renderer, "text", 2, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(books_list), column);

    // Publisher column
    renderer = gtk_cell_renderer_text_new();
    column = gtk_tree_view_column_new_with_attributes("Publisher", renderer, "text", 3, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(books_list), column);

    // ISBN column
    renderer = gtk_cell_renderer_text_new();
    column = gtk_tree_view_column_new_with_attributes("ISBN", renderer, "text", 4, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(books_list), column);

    // Actions column
    renderer = gtk_cell_renderer_text_new();
    column = gtk_tree_view_column_new_with_attributes("Actions", renderer, "text", 5, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(books_list), column);

    // Create list store
    books_store = gtk_list_store_new(6,
        G_TYPE_INT,    // ID
        G_TYPE_STRING, // Title
        G_TYPE_STRING, // Author
        G_TYPE_STRING, // Publisher
        G_TYPE_STRING, // ISBN
        G_TYPE_STRING  // Actions
    );
    gtk_tree_view_set_model(GTK_TREE_VIEW(books_list), GTK_TREE_MODEL(books_store));

    // Refresh the list
    refresh_books_list(db);

    return books_page;
}

// Refresh books list
void refresh_books_list(Database *db) {
    GtkListStore *store = GTK_LIST_STORE(gtk_tree_view_get_model(GTK_TREE_VIEW(books_list)));
    gtk_list_store_clear(store);

    BookList *books = book_list_all(db);
    if (books != NULL) {
        for (int i = 0; i < books->count; i++) {
            Book *book = books->books[i];
            Author *author = author_find_by_id(db, book->author_id);
            Publisher *publisher = publisher_find_by_id(db, book->publisher_id);

            GtkTreeIter iter;
            gtk_list_store_append(store, &iter);
            gtk_list_store_set(store, &iter,
                0, book->book_id,
                1, book->title,
                2, author ? author->name : "Unknown",
                3, publisher ? publisher->name : "Unknown",
                4, book->isbn,
                5, "Edit | Delete",
                -1);

            if (author) author_free(author);
            if (publisher) publisher_free(publisher);
        }
        book_list_free(books);
    }
}

// Add book dialog
void add_book_dialog_show(Database *db) {
    (void)db; // Suppress unused parameter warning
    // Create dialog
    add_book_dialog = gtk_dialog_new_with_buttons("Add Book",
        GTK_WINDOW(main_window),
        GTK_DIALOG_MODAL | GTK_DIALOG_DESTROY_WITH_PARENT,
        "Cancel",
        GTK_RESPONSE_CANCEL,
        "Add",
        GTK_RESPONSE_ACCEPT,
        NULL);

    // Create form
    GtkWidget *content_area = gtk_dialog_get_content_area(GTK_DIALOG(add_book_dialog));
    GtkWidget *form = gtk_grid_new();
    gtk_grid_set_row_spacing(GTK_GRID(form), 5);
    gtk_grid_set_column_spacing(GTK_GRID(form), 5);
    gtk_container_add(GTK_CONTAINER(content_area), form);

    // Add form fields
    GtkWidget *title_label = gtk_label_new("Title:");
    GtkWidget *title_entry = gtk_entry_new();
    gtk_grid_attach(GTK_GRID(form), title_label, 0, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(form), title_entry, 1, 0, 1, 1);

    GtkWidget *isbn_label = gtk_label_new("ISBN:");
    GtkWidget *isbn_entry = gtk_entry_new();
    gtk_grid_attach(GTK_GRID(form), isbn_label, 0, 1, 1, 1);
    gtk_grid_attach(GTK_GRID(form), isbn_entry, 1, 1, 1, 1);

    GtkWidget *author_label = gtk_label_new("Author ID:");
    GtkWidget *author_entry = gtk_entry_new();
    gtk_grid_attach(GTK_GRID(form), author_label, 0, 2, 1, 1);
    gtk_grid_attach(GTK_GRID(form), author_entry, 1, 2, 1, 1);

    GtkWidget *publisher_label = gtk_label_new("Publisher ID:");
    GtkWidget *publisher_entry = gtk_entry_new();
    gtk_grid_attach(GTK_GRID(form), publisher_label, 0, 3, 1, 1);
    gtk_grid_attach(GTK_GRID(form), publisher_entry, 1, 3, 1, 1);

    // Connect response signal
    g_signal_connect(add_book_dialog, "response", G_CALLBACK(on_add_book_response), NULL);

    // Show dialog
    gtk_widget_show_all(add_book_dialog);
}

// Edit book dialog
void edit_book_dialog_show(Database *db, int book_id) {
    Book *book = book_find_by_id(db, book_id);
    if (book == NULL) {
        return;
    }

    edit_book_dialog = gtk_dialog_new_with_buttons("Edit Book",
                                                  GTK_WINDOW(main_window),
                                                  GTK_DIALOG_MODAL | GTK_DIALOG_DESTROY_WITH_PARENT,
                                                  "Cancel",
                                                  GTK_RESPONSE_CANCEL,
                                                  "Save",
                                                  GTK_RESPONSE_ACCEPT,
                                                  NULL);

    GtkWidget *content_area = gtk_dialog_get_content_area(GTK_DIALOG(edit_book_dialog));
    GtkWidget *grid = gtk_grid_new();
    gtk_grid_set_row_spacing(GTK_GRID(grid), 5);
    gtk_grid_set_column_spacing(GTK_GRID(grid), 5);
    gtk_container_add(GTK_CONTAINER(content_area), grid);

    // Create entry fields
    GtkWidget *title_entry = gtk_entry_new();
    GtkWidget *author_entry = gtk_entry_new();
    GtkWidget *publisher_entry = gtk_entry_new();
    GtkWidget *isbn_entry = gtk_entry_new();

    // Set initial values
    gtk_entry_set_text(GTK_ENTRY(title_entry), book->title);
    gtk_entry_set_text(GTK_ENTRY(isbn_entry), book->isbn);
    
    char author_id_str[32];
    char publisher_id_str[32];
    snprintf(author_id_str, sizeof(author_id_str), "%d", book->author_id);
    snprintf(publisher_id_str, sizeof(publisher_id_str), "%d", book->publisher_id);
    gtk_entry_set_text(GTK_ENTRY(author_entry), author_id_str);
    gtk_entry_set_text(GTK_ENTRY(publisher_entry), publisher_id_str);

    // Add labels and entries to grid
    gtk_grid_attach(GTK_GRID(grid), gtk_label_new("Title:"), 0, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), title_entry, 1, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), gtk_label_new("Author ID:"), 0, 1, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), author_entry, 1, 1, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), gtk_label_new("Publisher ID:"), 0, 2, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), publisher_entry, 1, 2, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), gtk_label_new("ISBN:"), 0, 3, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), isbn_entry, 1, 3, 1, 1);

    gtk_widget_show_all(edit_book_dialog);

    if (gtk_dialog_run(GTK_DIALOG(edit_book_dialog)) == GTK_RESPONSE_ACCEPT) {
        strncpy(book->title, gtk_entry_get_text(GTK_ENTRY(title_entry)), sizeof(book->title) - 1);
        strncpy(book->isbn, gtk_entry_get_text(GTK_ENTRY(isbn_entry)), sizeof(book->isbn) - 1);
        book->author_id = atoi(gtk_entry_get_text(GTK_ENTRY(author_entry)));
        book->publisher_id = atoi(gtk_entry_get_text(GTK_ENTRY(publisher_entry)));

        if (book_update(db, book)) {
            refresh_books_list(db);
        } else {
            GtkWidget *error_dialog = gtk_message_dialog_new(GTK_WINDOW(edit_book_dialog),
                                                           GTK_DIALOG_MODAL | GTK_DIALOG_DESTROY_WITH_PARENT,
                                                           GTK_MESSAGE_ERROR,
                                                           GTK_BUTTONS_OK,
                                                           "Failed to update book");
            gtk_dialog_run(GTK_DIALOG(error_dialog));
            gtk_widget_destroy(error_dialog);
        }
    }

    book_free(book);
    gtk_widget_destroy(edit_book_dialog);
}

// Delete book dialog
void delete_book_dialog_show(Database *db, int book_id) {
    GtkWidget *dialog = gtk_message_dialog_new(GTK_WINDOW(main_window),
                                             GTK_DIALOG_MODAL | GTK_DIALOG_DESTROY_WITH_PARENT,
                                             GTK_MESSAGE_QUESTION,
                                             GTK_BUTTONS_YES_NO,
                                             "Are you sure you want to delete this book?");
    gtk_window_set_title(GTK_WINDOW(dialog), "Confirm Delete");

    if (gtk_dialog_run(GTK_DIALOG(dialog)) == GTK_RESPONSE_YES) {
        if (book_delete(db, book_id)) {
            refresh_books_list(db);
        } else {
            GtkWidget *error_dialog = gtk_message_dialog_new(GTK_WINDOW(dialog),
                                                           GTK_DIALOG_MODAL | GTK_DIALOG_DESTROY_WITH_PARENT,
                                                           GTK_MESSAGE_ERROR,
                                                           GTK_BUTTONS_OK,
                                                           "Failed to delete book");
            gtk_dialog_run(GTK_DIALOG(error_dialog));
            gtk_widget_destroy(error_dialog);
        }
    }

    gtk_widget_destroy(dialog);
}

// Search books
void search_books(Database *db, const char *query) {
    gtk_list_store_clear(books_store);

    Book *book = book_find_by_title(db, query);
    if (book != NULL) {
        Author *author = author_find_by_id(db, book->author_id);
        Publisher *publisher = publisher_find_by_id(db, book->publisher_id);

        GtkTreeIter iter;
        gtk_list_store_append(books_store, &iter);
        gtk_list_store_set(books_store, &iter,
                         0, book->book_id,
                         1, book->title,
                         2, author ? author->name : "Unknown",
                         3, publisher ? publisher->name : "Unknown",
                         4, book->isbn,
                         5, "Edit | Delete",
                         -1);

        if (author) author_free(author);
        if (publisher) publisher_free(publisher);
        book_free(book);
    }
}

// Show popup menu
void show_popup_menu(GtkWidget *treeview, GdkEventButton *event) {
    if (event->type == GDK_BUTTON_PRESS && event->button == 3) {
        GtkTreeSelection *selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(treeview));
        GtkTreeModel *model;
        GtkTreeIter iter;

        if (gtk_tree_selection_get_selected(selection, &model, &iter)) {
            GtkWidget *menu = gtk_menu_new();
            GtkWidget *edit_item = gtk_menu_item_new_with_label("Edit");
            GtkWidget *delete_item = gtk_menu_item_new_with_label("Delete");

            gtk_menu_shell_append(GTK_MENU_SHELL(menu), edit_item);
            gtk_menu_shell_append(GTK_MENU_SHELL(menu), delete_item);

            g_signal_connect(edit_item, "activate", G_CALLBACK(on_edit_book), &iter);
            g_signal_connect(delete_item, "activate", G_CALLBACK(on_delete_book), &iter);

            gtk_widget_show_all(menu);
            gtk_menu_popup_at_pointer(GTK_MENU(menu), (GdkEvent *)event);
        }
    }
}

// Edit book callback
void on_edit_book(GtkWidget *widget, gpointer data) {
    GtkTreeIter *iter = (GtkTreeIter *)data;
    int book_id;
    gtk_tree_model_get(GTK_TREE_MODEL(books_store), iter, 0, &book_id, -1);
    edit_book_dialog_show(db, book_id);
}

// Delete book callback
void on_delete_book(GtkWidget *widget, gpointer data) {
    GtkTreeIter *iter = (GtkTreeIter *)data;
    int book_id;
    gtk_tree_model_get(GTK_TREE_MODEL(books_store), iter, 0, &book_id, -1);
    delete_book_dialog_show(db, book_id);
}

void on_add_book_clicked(GtkWidget *widget, gpointer data) {
    // Create dialog
    add_book_dialog = gtk_dialog_new_with_buttons("Add Book",
        GTK_WINDOW(main_window),
        GTK_DIALOG_MODAL | GTK_DIALOG_DESTROY_WITH_PARENT,
        "Cancel",
        GTK_RESPONSE_CANCEL,
        "Add",
        GTK_RESPONSE_ACCEPT,
        NULL);

    // Create form
    GtkWidget *content_area = gtk_dialog_get_content_area(GTK_DIALOG(add_book_dialog));
    GtkWidget *form = gtk_grid_new();
    gtk_grid_set_row_spacing(GTK_GRID(form), 5);
    gtk_grid_set_column_spacing(GTK_GRID(form), 5);
    gtk_container_add(GTK_CONTAINER(content_area), form);

    // Add form fields
    GtkWidget *title_label = gtk_label_new("Title:");
    GtkWidget *title_entry = gtk_entry_new();
    gtk_grid_attach(GTK_GRID(form), title_label, 0, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(form), title_entry, 1, 0, 1, 1);

    GtkWidget *isbn_label = gtk_label_new("ISBN:");
    GtkWidget *isbn_entry = gtk_entry_new();
    gtk_grid_attach(GTK_GRID(form), isbn_label, 0, 1, 1, 1);
    gtk_grid_attach(GTK_GRID(form), isbn_entry, 1, 1, 1, 1);

    GtkWidget *author_label = gtk_label_new("Author ID:");
    GtkWidget *author_entry = gtk_entry_new();
    gtk_grid_attach(GTK_GRID(form), author_label, 0, 2, 1, 1);
    gtk_grid_attach(GTK_GRID(form), author_entry, 1, 2, 1, 1);

    GtkWidget *publisher_label = gtk_label_new("Publisher ID:");
    GtkWidget *publisher_entry = gtk_entry_new();
    gtk_grid_attach(GTK_GRID(form), publisher_label, 0, 3, 1, 1);
    gtk_grid_attach(GTK_GRID(form), publisher_entry, 1, 3, 1, 1);

    // Connect response signal
    g_signal_connect(add_book_dialog, "response", G_CALLBACK(on_add_book_response), NULL);

    // Show dialog
    gtk_widget_show_all(add_book_dialog);
}

void on_add_book_response(GtkDialog *dialog, gint response_id, gpointer data) {
    if (response_id == GTK_RESPONSE_ACCEPT) {
        // Get form values
        GtkWidget *content_area = gtk_dialog_get_content_area(dialog);
        GtkWidget *form = gtk_bin_get_child(GTK_BIN(content_area));
        
        GtkWidget *title_entry = gtk_grid_get_child_at(GTK_GRID(form), 1, 0);
        GtkWidget *isbn_entry = gtk_grid_get_child_at(GTK_GRID(form), 1, 1);
        GtkWidget *author_entry = gtk_grid_get_child_at(GTK_GRID(form), 1, 2);
        GtkWidget *publisher_entry = gtk_grid_get_child_at(GTK_GRID(form), 1, 3);

        const char *title = gtk_entry_get_text(GTK_ENTRY(title_entry));
        const char *isbn = gtk_entry_get_text(GTK_ENTRY(isbn_entry));
        int author_id = atoi(gtk_entry_get_text(GTK_ENTRY(author_entry)));
        int publisher_id = atoi(gtk_entry_get_text(GTK_ENTRY(publisher_entry)));

        // Create and save book
        Book *book = book_create(title, author_id, publisher_id, isbn, "", 0, 1, "");
        if (book != NULL) {
            if (book_save(db, book)) {
                refresh_books_list(db);
            }
            book_free(book);
        }
    }
    gtk_widget_destroy(GTK_WIDGET(dialog));
} 