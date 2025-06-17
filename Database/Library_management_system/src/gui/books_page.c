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
static GtkWidget *main_window;
static Database *db;

// Function prototypes
static void refresh_books_list(Database *db);
static void edit_book_dialog_show(Database *db, int book_id);
static void delete_book_dialog_show(Database *db, int book_id);
static void search_books(Database *db, const char *query);
static void on_edit_book(GtkWidget *widget, gpointer data);
static gboolean on_delete_book(GtkWidget *widget, GdkEventKey *event, gpointer data);
static void on_add_book_clicked(GtkWidget *widget, gpointer data);
static void on_add_book_response(GtkDialog *dialog, gint response_id, gpointer data);
static void on_search_changed(GtkSearchEntry *entry, gpointer user_data);

// Apply CSS styling with icons and beautiful borders
static void apply_css_styles() {
    GtkCssProvider *provider = gtk_css_provider_new();
    const gchar *css = 
        "* {"
        "    font-family: 'Segoe UI', Roboto, sans-serif;"
        "}"
        ".main-container {"
        "    background-color: #ffffff;"
        "    border-radius: 12px;"
        "    border: 1px solid #e0e0e0;"
        "    box-shadow: 0 2px 8px rgba(0, 0, 0, 0.05);"
        "    padding: 20px;"
        "}"
        ".header-box {"
        "    background-color: #f8f9fa;"
        "    padding: 15px;"
        "    border-radius: 8px;"
        "    border: 1px solid #e0e0e0;"
        "    margin-bottom: 15px;"
        "}"
        ".title-label {"
        "    font-size: 20px;"
        "    font-weight: 600;"
        "    color: #2c3e50;"
        "    margin-left: 8px;"
        "}"
        ".add-button {"
        "    background-color: #3498db;"
        "    color: white;"
        "    border-radius: 6px;"
        "    padding: 8px 18px;"
        "    border: none;"
        "    font-weight: 500;"
        "    transition: all 0.2s ease;"
        "}"
        ".add-button:hover {"
        "    background-color: #2980b9;"
        "    box-shadow: 0 2px 6px rgba(0, 0, 0, 0.1);"
        "}"
        ".add-button:active {"
        "    background-color: #1a6ca8;"
        "}"
        ".search-entry {"
        "    border-radius: 6px;"
        "    border: 1px solid #ced4da;"
        "    padding: 8px 12px;"
        "    font-size: 14px;"
        "    transition: border 0.2s ease;"
        "}"
        ".search-entry:focus {"
        "    border-color: #3498db;"
        "    box-shadow: 0 0 0 3px rgba(52, 152, 219, 0.1);"
        "}"
        ".treeview {"
        "    font-size: 14px;"
        "    border-radius: 8px;"
        "    border: 1px solid #e0e0e0;"
        "}"
        ".treeview-header {"
        "    font-weight: 600;"
        "    background-color: #f8f9fa;"
        "    border-bottom: 1px solid #e0e0e0;"
        "    padding: 8px 12px;"
        "}"
        ".treeview-cell {"
        "    padding: 8px 12px;"
        "    border-bottom: 1px solid #f0f0f0;"
        "}"
        ".action-button {"
        "    background-color: #3498db;"
        "    color: white;"
        "    border-radius: 4px;"
        "    padding: 4px 10px;"
        "    margin-right: 6px;"
        "    border: none;"
        "    font-size: 12px;"
        "    transition: all 0.2s ease;"
        "}"
        ".edit-button {"
        "    background-color: #2ecc71;"
        "}"
        ".edit-button:hover {"
        "    background-color: #27ae60;"
        "}"
        ".delete-button {"
        "    background-color: #e74c3c;"
        "}"
        ".delete-button:hover {"
        "    background-color: #c0392b;"
        "}"
        ".dialog-form {"
        "    padding: 20px;"
        "    background-color: #f8f9fa;"
        "    border-radius: 8px;"
        "    border: 1px solid #e0e0e0;"
        "}"
        ".dialog-label {"
        "    font-weight: 500;"
        "    color: #2c3e50;"
        "    margin-right: 12px;"
        "}"
        ".dialog-entry {"
        "    margin-bottom: 12px;"
        "    padding: 8px 12px;"
        "    border-radius: 6px;"
        "    border: 1px solid #ced4da;"
        "    transition: border 0.2s ease;"
        "}"
        ".dialog-entry:focus {"
        "    border-color: #3498db;"
        "    box-shadow: 0 0 0 3px rgba(52, 152, 219, 0.1);"
        "}"
        ".scrolled-window {"
        "    border-radius: 8px;"
        "    border: 1px solid #e0e0e0;"
        "}";
    
    gtk_css_provider_load_from_data(provider, css, -1, NULL);
    gtk_style_context_add_provider_for_screen(gdk_screen_get_default(),
                                            GTK_STYLE_PROVIDER(provider),
                                            GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
}

// Helper function to create button with icon
static GtkWidget* create_button_with_icon(const gchar *icon_name, const gchar *label) {
    GtkWidget *box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
    GtkWidget *icon = gtk_image_new_from_icon_name(icon_name, GTK_ICON_SIZE_BUTTON);
    GtkWidget *lbl = gtk_label_new(label);
    
    gtk_box_pack_start(GTK_BOX(box), icon, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(box), lbl, FALSE, FALSE, 0);
    gtk_widget_show_all(box);
    
    return box;
}

// Create books page with icons and beautiful design
GtkWidget* create_books_page(Database *database) {
    db = database;
    apply_css_styles();

    // Create main container with border and shadow
    books_page = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_widget_set_name(books_page, "main-container");
    gtk_container_set_border_width(GTK_CONTAINER(books_page), 15);

    // Create header box
    GtkWidget *header_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 12);
    gtk_widget_set_name(header_box, "header-box");
    
    // Title with icon
    GtkWidget *title_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 8);
    GtkWidget *title_icon = gtk_image_new_from_icon_name("book-symbolic", GTK_ICON_SIZE_BUTTON);
    GtkWidget *title = gtk_label_new("Books Management");
    gtk_widget_set_name(title, "title-label");
    gtk_box_pack_start(GTK_BOX(title_box), title_icon, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(title_box), title, FALSE, FALSE, 0);
    
    // Search entry
    search_entry = gtk_search_entry_new();
    gtk_widget_set_name(search_entry, "search-entry");
    gtk_entry_set_placeholder_text(GTK_ENTRY(search_entry), "Search books by title...");
    gtk_widget_set_hexpand(search_entry, TRUE);
    g_signal_connect(search_entry, "search-changed", G_CALLBACK(on_search_changed), NULL);
    
    // Add button with icon
    GtkWidget *add_button = gtk_button_new();
    GtkWidget *add_button_content = create_button_with_icon("list-add-symbolic", "Add Book");
    gtk_container_add(GTK_CONTAINER(add_button), add_button_content);
    gtk_widget_set_name(add_button, "add-button");
    g_signal_connect(add_button, "clicked", G_CALLBACK(on_add_book_clicked), db);
    
    gtk_box_pack_start(GTK_BOX(header_box), title_box, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(header_box), search_entry, TRUE, TRUE, 0);
    gtk_box_pack_end(GTK_BOX(header_box), add_button, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(books_page), header_box, FALSE, FALSE, 0);

    // Create scrolled window
    GtkWidget *scrolled = gtk_scrolled_window_new(NULL, NULL);
    gtk_widget_set_name(scrolled, "scrolled-window");
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrolled),
                                 GTK_POLICY_AUTOMATIC,
                                 GTK_POLICY_AUTOMATIC);
    gtk_scrolled_window_set_shadow_type(GTK_SCROLLED_WINDOW(scrolled), GTK_SHADOW_ETCHED_IN);
    gtk_widget_set_size_request(scrolled, 800, 600);
    gtk_box_pack_start(GTK_BOX(books_page), scrolled, TRUE, TRUE, 0);

    // Create tree view
    books_list = gtk_tree_view_new();
    gtk_widget_set_name(books_list, "treeview");
    gtk_tree_view_set_headers_visible(GTK_TREE_VIEW(books_list), TRUE);
    gtk_tree_view_set_grid_lines(GTK_TREE_VIEW(books_list), GTK_TREE_VIEW_GRID_LINES_HORIZONTAL);
    gtk_container_add(GTK_CONTAINER(scrolled), books_list);

    // Add columns with cell styling
    GtkCellRenderer *renderer;
    GtkTreeViewColumn *column;

    // ID column
    renderer = gtk_cell_renderer_text_new();
    g_object_set(renderer, "ypad", 8, NULL);
    column = gtk_tree_view_column_new_with_attributes("ID", renderer, "text", 0, NULL);
    gtk_tree_view_column_set_resizable(column, TRUE);
    gtk_tree_view_append_column(GTK_TREE_VIEW(books_list), column);

    // Title column
    renderer = gtk_cell_renderer_text_new();
    g_object_set(renderer, "ypad", 8, NULL);
    column = gtk_tree_view_column_new_with_attributes("Title", renderer, "text", 1, NULL);
    gtk_tree_view_column_set_resizable(column, TRUE);
    gtk_tree_view_column_set_min_width(column, 200);
    gtk_tree_view_append_column(GTK_TREE_VIEW(books_list), column);

    // Author column
    renderer = gtk_cell_renderer_text_new();
    g_object_set(renderer, "ypad", 8, NULL);
    column = gtk_tree_view_column_new_with_attributes("Author", renderer, "text", 2, NULL);
    gtk_tree_view_column_set_resizable(column, TRUE);
    gtk_tree_view_column_set_min_width(column, 150);
    gtk_tree_view_append_column(GTK_TREE_VIEW(books_list), column);

    // Publisher column
    renderer = gtk_cell_renderer_text_new();
    g_object_set(renderer, "ypad", 8, NULL);
    column = gtk_tree_view_column_new_with_attributes("Publisher", renderer, "text", 3, NULL);
    gtk_tree_view_column_set_resizable(column, TRUE);
    gtk_tree_view_append_column(GTK_TREE_VIEW(books_list), column);

    // ISBN column
    renderer = gtk_cell_renderer_text_new();
    g_object_set(renderer, "ypad", 8, NULL);
    column = gtk_tree_view_column_new_with_attributes("ISBN", renderer, "text", 4, NULL);
    gtk_tree_view_column_set_resizable(column, TRUE);
    gtk_tree_view_append_column(GTK_TREE_VIEW(books_list), column);

    // Actions column with buttons
    GtkCellRenderer *action_renderer = gtk_cell_renderer_text_new();
    g_object_set(action_renderer, "ypad", 8, NULL);
    column = gtk_tree_view_column_new_with_attributes("Actions", action_renderer, "text", 5, NULL);
    gtk_tree_view_column_set_resizable(column, FALSE);
    gtk_tree_view_column_set_fixed_width(column, 180);
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

    // Enable selection
    GtkTreeSelection *selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(books_list));
    gtk_tree_selection_set_mode(selection, GTK_SELECTION_SINGLE);

    // Connect double-click signal for edit
    g_signal_connect(books_list, "row-activated", G_CALLBACK(on_edit_book), db);

    // Connect key press signal for delete
    g_signal_connect(books_list, "key-press-event", G_CALLBACK(on_delete_book), db);

    // Set header style
    GtkStyleContext *context = gtk_widget_get_style_context(books_list);
    gtk_style_context_add_class(context, "treeview-header");

    // Refresh the list
    refresh_books_list(db);

    return books_page;
}

// Enhanced refresh_books_list with icons for actions
void refresh_books_list(Database *db) {
    gtk_list_store_clear(books_store);

    BookList *books = book_list_all(db);
    if (books != NULL) {
        for (int i = 0; i < books->count; i++) {
            Book *book = books->books[i];
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
                5, "Double-click to edit, Press Delete to remove",
                -1);

            if (author) author_free(author);
            if (publisher) publisher_free(publisher);
        }
        book_list_free(books);
    }
}

// Function to create a dialog with entry fields for book details
static GtkWidget* create_book_dialog(const char *title, const char *button_text, GtkWidget **out_grid) {
    GtkWidget *dialog = gtk_dialog_new_with_buttons(title,
        NULL,  // Use NULL as parent
        GTK_DIALOG_MODAL | GTK_DIALOG_DESTROY_WITH_PARENT,
        button_text, GTK_RESPONSE_ACCEPT,
        "Cancel", GTK_RESPONSE_CANCEL,
        NULL);
    
    // Set dialog properties
    gtk_window_set_modal(GTK_WINDOW(dialog), TRUE);
    gtk_window_set_position(GTK_WINDOW(dialog), GTK_WIN_POS_CENTER_ON_PARENT);
    gtk_dialog_set_default_response(GTK_DIALOG(dialog), GTK_RESPONSE_ACCEPT);
    gtk_window_set_resizable(GTK_WINDOW(dialog), FALSE);

    GtkWidget *content_area = gtk_dialog_get_content_area(GTK_DIALOG(dialog));
    GtkWidget *grid = gtk_grid_new();
    gtk_grid_set_row_spacing(GTK_GRID(grid), 10);
    gtk_grid_set_column_spacing(GTK_GRID(grid), 10);
    gtk_container_set_border_width(GTK_CONTAINER(grid), 10);
    gtk_container_add(GTK_CONTAINER(content_area), grid);

    // Title entry
    GtkWidget *title_label = gtk_label_new("Title:");
    GtkWidget *title_entry = gtk_entry_new();
    gtk_grid_attach(GTK_GRID(grid), title_label, 0, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), title_entry, 1, 0, 1, 1);

    // Author entry
    GtkWidget *author_label = gtk_label_new("Author:");
    GtkWidget *author_entry = gtk_entry_new();
    gtk_grid_attach(GTK_GRID(grid), author_label, 0, 1, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), author_entry, 1, 1, 1, 1);

    // Publisher entry
    GtkWidget *publisher_label = gtk_label_new("Publisher:");
    GtkWidget *publisher_entry = gtk_entry_new();
    gtk_grid_attach(GTK_GRID(grid), publisher_label, 0, 2, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), publisher_entry, 1, 2, 1, 1);

    // ISBN entry
    GtkWidget *isbn_label = gtk_label_new("ISBN:");
    GtkWidget *isbn_entry = gtk_entry_new();
    gtk_grid_attach(GTK_GRID(grid), isbn_label, 0, 3, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), isbn_entry, 1, 3, 1, 1);

    // Genre entry
    GtkWidget *genre_label = gtk_label_new("Genre:");
    GtkWidget *genre_entry = gtk_entry_new();
    gtk_grid_attach(GTK_GRID(grid), genre_label, 0, 4, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), genre_entry, 1, 4, 1, 1);

    // Year published entry
    GtkWidget *year_label = gtk_label_new("Year Published:");
    GtkWidget *year_entry = gtk_entry_new();
    gtk_grid_attach(GTK_GRID(grid), year_label, 0, 5, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), year_entry, 1, 5, 1, 1);

    // Copies available entry
    GtkWidget *copies_label = gtk_label_new("Copies Available:");
    GtkWidget *copies_entry = gtk_entry_new();
    gtk_grid_attach(GTK_GRID(grid), copies_label, 0, 6, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), copies_entry, 1, 6, 1, 1);

    // Shelf location entry
    GtkWidget *shelf_label = gtk_label_new("Shelf Location:");
    GtkWidget *shelf_entry = gtk_entry_new();
    gtk_grid_attach(GTK_GRID(grid), shelf_label, 0, 7, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), shelf_entry, 1, 7, 1, 1);

    // Set entry properties
    gtk_entry_set_activates_default(GTK_ENTRY(title_entry), TRUE);
    gtk_entry_set_activates_default(GTK_ENTRY(author_entry), TRUE);
    gtk_entry_set_activates_default(GTK_ENTRY(publisher_entry), TRUE);
    gtk_entry_set_activates_default(GTK_ENTRY(isbn_entry), TRUE);
    gtk_entry_set_activates_default(GTK_ENTRY(genre_entry), TRUE);
    gtk_entry_set_activates_default(GTK_ENTRY(year_entry), TRUE);
    gtk_entry_set_activates_default(GTK_ENTRY(copies_entry), TRUE);
    gtk_entry_set_activates_default(GTK_ENTRY(shelf_entry), TRUE);

    // Set minimum width for entries
    gtk_widget_set_size_request(title_entry, 300, -1);
    gtk_widget_set_size_request(author_entry, 300, -1);
    gtk_widget_set_size_request(publisher_entry, 300, -1);
    gtk_widget_set_size_request(isbn_entry, 300, -1);
    gtk_widget_set_size_request(genre_entry, 300, -1);
    gtk_widget_set_size_request(year_entry, 300, -1);
    gtk_widget_set_size_request(copies_entry, 300, -1);
    gtk_widget_set_size_request(shelf_entry, 300, -1);

    gtk_widget_show_all(dialog);
    if (out_grid) *out_grid = grid;
    return dialog;
}

// Handler for the Add Book button
static void on_add_book_clicked(GtkWidget *widget, gpointer data) {
    (void)widget;
    (void)data;

    // Create dialog with NULL parent
    GtkWidget *dialog = gtk_dialog_new_with_buttons("Add New Book",
        NULL,  // Use NULL as parent
        GTK_DIALOG_MODAL | GTK_DIALOG_DESTROY_WITH_PARENT,
        "Cancel", GTK_RESPONSE_CANCEL,
        "Add", GTK_RESPONSE_ACCEPT,
        NULL);

    // Set dialog properties
    gtk_window_set_resizable(GTK_WINDOW(dialog), FALSE);
    gtk_dialog_set_default_response(GTK_DIALOG(dialog), GTK_RESPONSE_ACCEPT);

    // Create form container
    GtkWidget *form_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_widget_set_name(form_box, "dialog-form");
    gtk_container_set_border_width(GTK_CONTAINER(form_box), 15);

    // Create entry fields with minimum width
    GtkWidget *title_entry = gtk_entry_new();
    gtk_widget_set_size_request(title_entry, 300, -1);
    gtk_entry_set_placeholder_text(GTK_ENTRY(title_entry), "Enter book title");
    gtk_widget_set_name(title_entry, "dialog-entry");

    GtkWidget *author_entry = gtk_entry_new();
    gtk_widget_set_size_request(author_entry, 300, -1);
    gtk_entry_set_placeholder_text(GTK_ENTRY(author_entry), "Enter author name");
    gtk_widget_set_name(author_entry, "dialog-entry");

    GtkWidget *publisher_entry = gtk_entry_new();
    gtk_widget_set_size_request(publisher_entry, 300, -1);
    gtk_entry_set_placeholder_text(GTK_ENTRY(publisher_entry), "Enter publisher name");
    gtk_widget_set_name(publisher_entry, "dialog-entry");

    GtkWidget *isbn_entry = gtk_entry_new();
    gtk_widget_set_size_request(isbn_entry, 300, -1);
    gtk_entry_set_placeholder_text(GTK_ENTRY(isbn_entry), "Enter ISBN");
    gtk_widget_set_name(isbn_entry, "dialog-entry");

    GtkWidget *year_entry = gtk_entry_new();
    gtk_widget_set_size_request(year_entry, 300, -1);
    gtk_entry_set_placeholder_text(GTK_ENTRY(year_entry), "Enter year published");
    gtk_widget_set_name(year_entry, "dialog-entry");

    GtkWidget *copies_entry = gtk_entry_new();
    gtk_widget_set_size_request(copies_entry, 300, -1);
    gtk_entry_set_placeholder_text(GTK_ENTRY(copies_entry), "Enter number of copies");
    gtk_widget_set_name(copies_entry, "dialog-entry");

    // Add fields to form
    gtk_box_pack_start(GTK_BOX(form_box), gtk_label_new("Title:"), FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(form_box), title_entry, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(form_box), gtk_label_new("Author:"), FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(form_box), author_entry, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(form_box), gtk_label_new("Publisher:"), FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(form_box), publisher_entry, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(form_box), gtk_label_new("ISBN:"), FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(form_box), isbn_entry, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(form_box), gtk_label_new("Year Published:"), FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(form_box), year_entry, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(form_box), gtk_label_new("Copies Available:"), FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(form_box), copies_entry, FALSE, FALSE, 0);

    // Add form to dialog
    gtk_container_add(GTK_CONTAINER(gtk_dialog_get_content_area(GTK_DIALOG(dialog))), form_box);
    gtk_widget_show_all(dialog);

    // Connect response signal
    g_signal_connect(dialog, "response", G_CALLBACK(on_add_book_response), NULL);
}

// Handler for the Edit Book button
static void on_edit_book(GtkWidget *widget, gpointer data) {
    (void)widget;
    Database *db = (Database *)data;
    GtkTreeSelection *selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(books_list));
    GtkTreeModel *model;
    GtkTreeIter iter;
    
    if (!gtk_tree_selection_get_selected(selection, &model, &iter)) {
        GtkWidget *message_dialog = gtk_message_dialog_new(NULL,
            GTK_DIALOG_MODAL | GTK_DIALOG_DESTROY_WITH_PARENT,
            GTK_MESSAGE_ERROR,
            GTK_BUTTONS_OK,
            "Please select a book to edit");
        gtk_dialog_run(GTK_DIALOG(message_dialog));
        gtk_widget_destroy(message_dialog);
        return;
    }

    int book_id;
    gtk_tree_model_get(model, &iter, 0, &book_id, -1);
    
    Book *book = book_find_by_id(db, book_id);
    if (!book) {
        GtkWidget *message_dialog = gtk_message_dialog_new(NULL,
            GTK_DIALOG_MODAL | GTK_DIALOG_DESTROY_WITH_PARENT,
            GTK_MESSAGE_ERROR,
            GTK_BUTTONS_OK,
            "Failed to find book with ID: %d", book_id);
        gtk_dialog_run(GTK_DIALOG(message_dialog));
        gtk_widget_destroy(message_dialog);
        return;
    }

    // Get author and publisher names
    Author *author = author_find_by_id(db, book->author_id);
    Publisher *publisher = publisher_find_by_id(db, book->publisher_id);
    
    GtkWidget *grid = NULL;
    GtkWidget *dialog = create_book_dialog("Edit Book", "Save", &grid);
    
    // Get all entry widgets
    GtkWidget *title_entry = gtk_grid_get_child_at(GTK_GRID(grid), 1, 0);
    GtkWidget *author_entry = gtk_grid_get_child_at(GTK_GRID(grid), 1, 1);
    GtkWidget *publisher_entry = gtk_grid_get_child_at(GTK_GRID(grid), 1, 2);
    GtkWidget *isbn_entry = gtk_grid_get_child_at(GTK_GRID(grid), 1, 3);
    GtkWidget *genre_entry = gtk_grid_get_child_at(GTK_GRID(grid), 1, 4);
    GtkWidget *year_entry = gtk_grid_get_child_at(GTK_GRID(grid), 1, 5);
    GtkWidget *copies_entry = gtk_grid_get_child_at(GTK_GRID(grid), 1, 6);
    GtkWidget *shelf_entry = gtk_grid_get_child_at(GTK_GRID(grid), 1, 7);

    // Set initial values
    gtk_entry_set_text(GTK_ENTRY(title_entry), book->title);
    gtk_entry_set_text(GTK_ENTRY(author_entry), author ? author->name : "");
    gtk_entry_set_text(GTK_ENTRY(publisher_entry), publisher ? publisher->name : "");
    gtk_entry_set_text(GTK_ENTRY(isbn_entry), book->isbn);
    gtk_entry_set_text(GTK_ENTRY(genre_entry), book->genre);
    gtk_entry_set_text(GTK_ENTRY(year_entry), g_strdup_printf("%d", book->year_published));
    gtk_entry_set_text(GTK_ENTRY(copies_entry), g_strdup_printf("%d", book->copies_available));
    gtk_entry_set_text(GTK_ENTRY(shelf_entry), book->shelf_location);

    // Show dialog and handle response
    gint response = gtk_dialog_run(GTK_DIALOG(dialog));
    
    if (response == GTK_RESPONSE_ACCEPT) {
        // Get updated values
        const gchar *title = gtk_entry_get_text(GTK_ENTRY(title_entry));
        const gchar *author_name = gtk_entry_get_text(GTK_ENTRY(author_entry));
        const gchar *publisher_name = gtk_entry_get_text(GTK_ENTRY(publisher_entry));
        const gchar *isbn = gtk_entry_get_text(GTK_ENTRY(isbn_entry));
        const gchar *genre = gtk_entry_get_text(GTK_ENTRY(genre_entry));
        const gchar *year = gtk_entry_get_text(GTK_ENTRY(year_entry));
        const gchar *copies = gtk_entry_get_text(GTK_ENTRY(copies_entry));
        const gchar *shelf = gtk_entry_get_text(GTK_ENTRY(shelf_entry));

        // Validate required fields
        if (!title || !author_name || !publisher_name || !isbn) {
            GtkWidget *message_dialog = gtk_message_dialog_new(NULL,
                GTK_DIALOG_MODAL | GTK_DIALOG_DESTROY_WITH_PARENT,
                GTK_MESSAGE_ERROR,
                GTK_BUTTONS_OK,
                "Please fill in all required fields (Title, Author, Publisher, ISBN)");
            gtk_dialog_run(GTK_DIALOG(message_dialog));
            gtk_widget_destroy(message_dialog);
            gtk_widget_destroy(dialog);
            book_free(book);
            if (author) author_free(author);
            if (publisher) publisher_free(publisher);
            return;
        }

        // Validate numeric fields
        char *endptr;
        int year_num = strtol(year, &endptr, 10);
        if (*endptr != '\0' || year_num < 0) {
            GtkWidget *message_dialog = gtk_message_dialog_new(NULL,
                GTK_DIALOG_MODAL | GTK_DIALOG_DESTROY_WITH_PARENT,
                GTK_MESSAGE_ERROR,
                GTK_BUTTONS_OK,
                "Year Published must be a positive number");
            gtk_dialog_run(GTK_DIALOG(message_dialog));
            gtk_widget_destroy(message_dialog);
            gtk_widget_destroy(dialog);
            book_free(book);
            if (author) author_free(author);
            if (publisher) publisher_free(publisher);
            return;
        }

        int copies_num = strtol(copies, &endptr, 10);
        if (*endptr != '\0' || copies_num < 0) {
            GtkWidget *message_dialog = gtk_message_dialog_new(NULL,
                GTK_DIALOG_MODAL | GTK_DIALOG_DESTROY_WITH_PARENT,
                GTK_MESSAGE_ERROR,
                GTK_BUTTONS_OK,
                "Copies Available must be a positive number");
            gtk_dialog_run(GTK_DIALOG(message_dialog));
            gtk_widget_destroy(message_dialog);
            gtk_widget_destroy(dialog);
            book_free(book);
            if (author) author_free(author);
            if (publisher) publisher_free(publisher);
            return;
        }

        // Look up or create author
        Author *new_author = author_find_by_name(db, author_name);
        if (!new_author) {
            new_author = author_create(author_name, "No biography available");
            if (new_author) {
                author_save(db, new_author);
            }
        }

        // Look up or create publisher
        Publisher *new_publisher = publisher_find_by_name(db, publisher_name);
        if (!new_publisher) {
            new_publisher = publisher_create(publisher_name, "No address available", "No contact info available");
            if (new_publisher) {
                publisher_save(db, new_publisher);
            }
        }

        if (!new_author || !new_publisher) {
            GtkWidget *message_dialog = gtk_message_dialog_new(NULL,
                GTK_DIALOG_MODAL | GTK_DIALOG_DESTROY_WITH_PARENT,
                GTK_MESSAGE_ERROR,
                GTK_BUTTONS_OK,
                "Failed to create or find author/publisher");
            gtk_dialog_run(GTK_DIALOG(message_dialog));
            gtk_widget_destroy(message_dialog);
            gtk_widget_destroy(dialog);
            book_free(book);
            if (author) author_free(author);
            if (publisher) publisher_free(publisher);
            if (new_author) author_free(new_author);
            if (new_publisher) publisher_free(new_publisher);
            return;
        }

        // Update book details
        strncpy(book->title, title, sizeof(book->title) - 1);
        book->author_id = new_author->author_id;
        book->publisher_id = new_publisher->publisher_id;
        strncpy(book->isbn, isbn, sizeof(book->isbn) - 1);
        strncpy(book->genre, genre, sizeof(book->genre) - 1);
        book->year_published = year_num;
        book->copies_available = copies_num;
        strncpy(book->shelf_location, shelf, sizeof(book->shelf_location) - 1);

        // Save changes
        if (book_save(db, book) == 0) {
            GtkWidget *message_dialog = gtk_message_dialog_new(NULL,
                GTK_DIALOG_MODAL | GTK_DIALOG_DESTROY_WITH_PARENT,
                GTK_MESSAGE_INFO,
                GTK_BUTTONS_OK,
                "Book updated successfully: %s", title);
            gtk_dialog_run(GTK_DIALOG(message_dialog));
            gtk_widget_destroy(message_dialog);
        } else {
            GtkWidget *message_dialog = gtk_message_dialog_new(NULL,
                GTK_DIALOG_MODAL | GTK_DIALOG_DESTROY_WITH_PARENT,
                GTK_MESSAGE_ERROR,
                GTK_BUTTONS_OK,
                "Failed to update book in database");
            gtk_dialog_run(GTK_DIALOG(message_dialog));
            gtk_widget_destroy(message_dialog);
        }

        // Clean up
        if (new_author) author_free(new_author);
        if (new_publisher) publisher_free(new_publisher);
    }

    // Clean up
    gtk_widget_destroy(dialog);
    book_free(book);
    if (author) author_free(author);
    if (publisher) publisher_free(publisher);
    
    // Refresh the list
    refresh_books_list(db);
}

// Handler for the Delete Book button
static gboolean on_delete_book(GtkWidget *widget, GdkEventKey *event, gpointer data) {
    if (event->keyval == GDK_KEY_Delete) {
        Database *db = (Database *)data;
        GtkTreeSelection *selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(books_list));
        GtkTreeModel *model;
        GtkTreeIter iter;
        if (gtk_tree_selection_get_selected(selection, &model, &iter)) {
            int book_id;
            gtk_tree_model_get(model, &iter, 0, &book_id, -1);
            g_print("Deleting book with ID: %d\n", book_id);
            GtkWidget *dialog = gtk_message_dialog_new(NULL,
                GTK_DIALOG_MODAL | GTK_DIALOG_DESTROY_WITH_PARENT,
                GTK_MESSAGE_QUESTION,
                GTK_BUTTONS_YES_NO,
                "Are you sure you want to delete this book?");
            gint response = gtk_dialog_run(GTK_DIALOG(dialog));
            if (response == GTK_RESPONSE_YES) {
                if (book_delete(db, book_id) == 0) {
                    g_print("Book deleted successfully\n");
                    GtkWidget *message_dialog = gtk_message_dialog_new(NULL,
                        GTK_DIALOG_MODAL | GTK_DIALOG_DESTROY_WITH_PARENT,
                        GTK_MESSAGE_INFO,
                        GTK_BUTTONS_OK,
                        "Book deleted successfully");
                    gtk_dialog_run(GTK_DIALOG(message_dialog));
                    gtk_widget_destroy(message_dialog);
                } else {
                    g_print("Failed to delete book from database\n");
                    GtkWidget *message_dialog = gtk_message_dialog_new(NULL,
                        GTK_DIALOG_MODAL | GTK_DIALOG_DESTROY_WITH_PARENT,
                        GTK_MESSAGE_ERROR,
                        GTK_BUTTONS_OK,
                        "Failed to delete book from database");
                    gtk_dialog_run(GTK_DIALOG(message_dialog));
                    gtk_widget_destroy(message_dialog);
                }
            }
            gtk_widget_destroy(dialog);
        }
        refresh_books_list(db);
        return TRUE;
    }
    return FALSE;
}

// Handler for the search entry
static void on_search_changed(GtkSearchEntry *entry, gpointer user_data) {
    (void)user_data;
    const gchar *query = gtk_entry_get_text(GTK_ENTRY(entry));
    // TODO: Implement search logic
    // For now, just refresh the list
    refresh_books_list(db);
}

// Stub for edit_book_dialog_show
static void edit_book_dialog_show(Database *db, int book_id) {
    (void)db;
    (void)book_id;
    GtkWidget *dialog = gtk_message_dialog_new(NULL,
        GTK_DIALOG_MODAL | GTK_DIALOG_DESTROY_WITH_PARENT,
        GTK_MESSAGE_INFO,
        GTK_BUTTONS_OK,
        "Edit Book dialog would appear here for book ID: %d", book_id);
    gtk_dialog_run(GTK_DIALOG(dialog));
    gtk_widget_destroy(dialog);
    refresh_books_list(db);
}

// Stub for delete_book_dialog_show
static void delete_book_dialog_show(Database *db, int book_id) {
    (void)db;
    (void)book_id;
    GtkWidget *dialog = gtk_message_dialog_new(NULL,
        GTK_DIALOG_MODAL | GTK_DIALOG_DESTROY_WITH_PARENT,
        GTK_MESSAGE_INFO,
        GTK_BUTTONS_OK,
        "Delete Book dialog would appear here for book ID: %d", book_id);
    gtk_dialog_run(GTK_DIALOG(dialog));
    gtk_widget_destroy(dialog);
    refresh_books_list(db);
}

// Stub for search_books
static void search_books(Database *db, const char *query) {
    (void)db;
    (void)query;
    // For now, just refresh the list (implement search logic as needed)
    refresh_books_list(db);
}

// Handler for the Add Book response
static void on_add_book_response(GtkDialog *dialog, gint response_id, gpointer data) {
    (void)data;

    if (response_id == GTK_RESPONSE_ACCEPT) {
        // Get the form box from the dialog
        GtkWidget *form_box = gtk_bin_get_child(GTK_BIN(gtk_dialog_get_content_area(dialog)));
        
        // Get all entry widgets
        GtkWidget *title_entry = gtk_bin_get_child(GTK_BIN(gtk_bin_get_child(GTK_BIN(form_box))));
        GtkWidget *author_entry = gtk_bin_get_child(GTK_BIN(gtk_bin_get_child(GTK_BIN(gtk_bin_get_child(GTK_BIN(form_box))))));
        GtkWidget *publisher_entry = gtk_bin_get_child(GTK_BIN(gtk_bin_get_child(GTK_BIN(gtk_bin_get_child(GTK_BIN(gtk_bin_get_child(GTK_BIN(form_box))))))));
        GtkWidget *isbn_entry = gtk_bin_get_child(GTK_BIN(gtk_bin_get_child(GTK_BIN(gtk_bin_get_child(GTK_BIN(gtk_bin_get_child(GTK_BIN(gtk_bin_get_child(GTK_BIN(form_box))))))))));
        GtkWidget *year_entry = gtk_bin_get_child(GTK_BIN(gtk_bin_get_child(GTK_BIN(gtk_bin_get_child(GTK_BIN(gtk_bin_get_child(GTK_BIN(gtk_bin_get_child(GTK_BIN(gtk_bin_get_child(GTK_BIN(form_box))))))))))));
        GtkWidget *copies_entry = gtk_bin_get_child(GTK_BIN(gtk_bin_get_child(GTK_BIN(gtk_bin_get_child(GTK_BIN(gtk_bin_get_child(GTK_BIN(gtk_bin_get_child(GTK_BIN(gtk_bin_get_child(GTK_BIN(gtk_bin_get_child(GTK_BIN(form_box))))))))))))));

        // Get text from entries
        const gchar *title = gtk_entry_get_text(GTK_ENTRY(title_entry));
        const gchar *author_name = gtk_entry_get_text(GTK_ENTRY(author_entry));
        const gchar *publisher_name = gtk_entry_get_text(GTK_ENTRY(publisher_entry));
        const gchar *isbn = gtk_entry_get_text(GTK_ENTRY(isbn_entry));
        const gchar *year = gtk_entry_get_text(GTK_ENTRY(year_entry));
        const gchar *copies = gtk_entry_get_text(GTK_ENTRY(copies_entry));

        // Validate required fields
        if (!title || !author_name || !publisher_name || !isbn) {
            GtkWidget *message_dialog = gtk_message_dialog_new(NULL,
                GTK_DIALOG_MODAL | GTK_DIALOG_DESTROY_WITH_PARENT,
                GTK_MESSAGE_ERROR,
                GTK_BUTTONS_OK,
                "Please fill in all required fields (Title, Author, Publisher, ISBN)");
            gtk_dialog_run(GTK_DIALOG(message_dialog));
            gtk_widget_destroy(message_dialog);
            return;
        }

        // Validate numeric fields
        char *endptr;
        int year_num = strtol(year, &endptr, 10);
        if (*endptr != '\0' || year_num < 0) {
            GtkWidget *message_dialog = gtk_message_dialog_new(NULL,
                GTK_DIALOG_MODAL | GTK_DIALOG_DESTROY_WITH_PARENT,
                GTK_MESSAGE_ERROR,
                GTK_BUTTONS_OK,
                "Year Published must be a positive number");
            gtk_dialog_run(GTK_DIALOG(message_dialog));
            gtk_widget_destroy(message_dialog);
            return;
        }

        int copies_num = strtol(copies, &endptr, 10);
        if (*endptr != '\0' || copies_num < 0) {
            GtkWidget *message_dialog = gtk_message_dialog_new(NULL,
                GTK_DIALOG_MODAL | GTK_DIALOG_DESTROY_WITH_PARENT,
                GTK_MESSAGE_ERROR,
                GTK_BUTTONS_OK,
                "Copies Available must be a positive number");
            gtk_dialog_run(GTK_DIALOG(message_dialog));
            gtk_widget_destroy(message_dialog);
            return;
        }

        // Look up author and publisher IDs
        Author *author = author_find_by_name(db, author_name);
        Publisher *publisher = publisher_find_by_name(db, publisher_name);
        int author_id = author ? author->author_id : 1; // Default to 1 if not found
        int publisher_id = publisher ? publisher->publisher_id : 1; // Default to 1 if not found

        // Create a new book with the entered values
        Book *book = book_create(
            title,           // title
            author_id,       // author_id
            publisher_id,    // publisher_id
            isbn,           // isbn
            "General",      // genre (default)
            year_num,       // year_published
            copies_num,     // copies_available
            "A1"            // shelf_location (default)
        );

        if (book) {
            if (book_save(db, book) == 0) {
                g_print("Book added successfully: %s\n", title);
                GtkWidget *message_dialog = gtk_message_dialog_new(NULL,
                    GTK_DIALOG_MODAL | GTK_DIALOG_DESTROY_WITH_PARENT,
                    GTK_MESSAGE_INFO,
                    GTK_BUTTONS_OK,
                    "Book added successfully: %s", title);
                gtk_dialog_run(GTK_DIALOG(message_dialog));
                gtk_widget_destroy(message_dialog);
            } else {
                g_print("Failed to save book to database\n");
                GtkWidget *message_dialog = gtk_message_dialog_new(NULL,
                    GTK_DIALOG_MODAL | GTK_DIALOG_DESTROY_WITH_PARENT,
                    GTK_MESSAGE_ERROR,
                    GTK_BUTTONS_OK,
                    "Failed to save book to database");
                gtk_dialog_run(GTK_DIALOG(message_dialog));
                gtk_widget_destroy(message_dialog);
            }
            book_free(book);
        } else {
            g_print("Failed to create book\n");
            GtkWidget *message_dialog = gtk_message_dialog_new(NULL,
                GTK_DIALOG_MODAL | GTK_DIALOG_DESTROY_WITH_PARENT,
                GTK_MESSAGE_ERROR,
                GTK_BUTTONS_OK,
                "Failed to create book");
            gtk_dialog_run(GTK_DIALOG(message_dialog));
            gtk_widget_destroy(message_dialog);
        }

        if (author) author_free(author);
        if (publisher) publisher_free(publisher);
    }
    gtk_widget_destroy(GTK_WIDGET(dialog));
    refresh_books_list(db);
}

// [Rest of the functions would need similar styling enhancements, especially the dialog boxes]