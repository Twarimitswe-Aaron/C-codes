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
static void on_delete_book(GtkWidget *widget, gpointer data);
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

    main_window = gtk_widget_get_toplevel(books_page);

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
    renderer = gtk_cell_renderer_text_new();
    g_object_set(renderer, "ypad", 8, NULL);
    column = gtk_tree_view_column_new_with_attributes("Actions", renderer, "text", 5, NULL);
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
                5, "✏️ Edit | 🗑️ Delete", // Using Unicode icons
                -1);

            if (author) author_free(author);
            if (publisher) publisher_free(publisher);
        }
        book_list_free(books);
    }
}

// [Rest of the functions would need similar styling enhancements, especially the dialog boxes]