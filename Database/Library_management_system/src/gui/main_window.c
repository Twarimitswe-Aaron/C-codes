#include <gtk/gtk.h>
#include "../../include/database.h"
#include "../../include/book.h"
#include "../../include/author.h"
#include "../../include/publisher.h"
#include "../../include/member.h"
#include "../../include/staff.h"
#include "../../include/borrowing.h"
#include "../../include/fine.h"
#include "books_page.h"

// Global variables
GtkWidget *main_window;
GtkWidget *content_area;
Database *db;

// Function prototypes
void create_main_window(void);
static gboolean on_window_destroy(GtkWidget* widget, gpointer user_data);
void show_books_page(void);
void show_authors_page(void);
void show_publishers_page(void);
void show_members_page(void);
void show_staff_page(void);
void show_borrowings_page(void);
void show_fines_page(void);

// Main window creation
void create_main_window(void) {
    // Initialize database connection
    db = db_init();
    if (db == NULL) {
        GtkWidget *dialog = gtk_message_dialog_new(NULL,
                                                 GTK_DIALOG_MODAL | GTK_DIALOG_DESTROY_WITH_PARENT,
                                                 GTK_MESSAGE_ERROR,
                                                 GTK_BUTTONS_OK,
                                                 "Failed to initialize database");
        gtk_dialog_run(GTK_DIALOG(dialog));
        gtk_widget_destroy(dialog);
        return;
    }

    if (!db_connect(db)) {
        GtkWidget *dialog = gtk_message_dialog_new(NULL,
                                                 GTK_DIALOG_MODAL | GTK_DIALOG_DESTROY_WITH_PARENT,
                                                 GTK_MESSAGE_ERROR,
                                                 GTK_BUTTONS_OK,
                                                 "Failed to connect to database");
        gtk_dialog_run(GTK_DIALOG(dialog));
        gtk_widget_destroy(dialog);
        db_close(db);
        return;
    }

    // Create the main window
    main_window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(main_window), "Library Management System");
    gtk_window_set_default_size(GTK_WINDOW(main_window), 1024, 768);
    g_signal_connect(main_window, "destroy", G_CALLBACK(on_window_destroy), NULL);

    // Create main vertical box
    GtkWidget *main_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    gtk_container_add(GTK_CONTAINER(main_window), main_box);

    // Create header
    GtkWidget *header = gtk_header_bar_new();
    gtk_header_bar_set_title(GTK_HEADER_BAR(header), "Library Management System");
    gtk_header_bar_set_show_close_button(GTK_HEADER_BAR(header), TRUE);
    gtk_box_pack_start(GTK_BOX(main_box), header, FALSE, FALSE, 0);

    // Create horizontal box for sidebar and content
    GtkWidget *content_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
    gtk_box_pack_start(GTK_BOX(main_box), content_box, TRUE, TRUE, 0);

    // Create sidebar
    GtkWidget *sidebar = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    gtk_widget_set_size_request(sidebar, 200, -1);
    gtk_box_pack_start(GTK_BOX(content_box), sidebar, FALSE, FALSE, 0);

    // Create content area
    content_area = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    gtk_box_pack_start(GTK_BOX(content_box), content_area, TRUE, TRUE, 0);

    // Create sidebar buttons with icons
    GtkWidget *books_btn = gtk_button_new_with_label("📚 Books");
    GtkWidget *authors_btn = gtk_button_new_with_label("✍️ Authors");
    GtkWidget *publishers_btn = gtk_button_new_with_label("🏢 Publishers");
    GtkWidget *members_btn = gtk_button_new_with_label("👥 Members");
    GtkWidget *staff_btn = gtk_button_new_with_label("👨‍💼 Staff");
    GtkWidget *borrowings_btn = gtk_button_new_with_label("📖 Borrowings");
    GtkWidget *fines_btn = gtk_button_new_with_label("💰 Fines");

    // Style buttons
    GtkStyleContext *context;
    context = gtk_widget_get_style_context(books_btn);
    gtk_style_context_add_class(context, "sidebar-button");
    context = gtk_widget_get_style_context(authors_btn);
    gtk_style_context_add_class(context, "sidebar-button");
    context = gtk_widget_get_style_context(publishers_btn);
    gtk_style_context_add_class(context, "sidebar-button");
    context = gtk_widget_get_style_context(members_btn);
    gtk_style_context_add_class(context, "sidebar-button");
    context = gtk_widget_get_style_context(staff_btn);
    gtk_style_context_add_class(context, "sidebar-button");
    context = gtk_widget_get_style_context(borrowings_btn);
    gtk_style_context_add_class(context, "sidebar-button");
    context = gtk_widget_get_style_context(fines_btn);
    gtk_style_context_add_class(context, "sidebar-button");

    // Add buttons to sidebar
    gtk_box_pack_start(GTK_BOX(sidebar), books_btn, FALSE, FALSE, 5);
    gtk_box_pack_start(GTK_BOX(sidebar), authors_btn, FALSE, FALSE, 5);
    gtk_box_pack_start(GTK_BOX(sidebar), publishers_btn, FALSE, FALSE, 5);
    gtk_box_pack_start(GTK_BOX(sidebar), members_btn, FALSE, FALSE, 5);
    gtk_box_pack_start(GTK_BOX(sidebar), staff_btn, FALSE, FALSE, 5);
    gtk_box_pack_start(GTK_BOX(sidebar), borrowings_btn, FALSE, FALSE, 5);
    gtk_box_pack_start(GTK_BOX(sidebar), fines_btn, FALSE, FALSE, 5);

    // Connect button signals
    g_signal_connect(books_btn, "clicked", G_CALLBACK(show_books_page), NULL);
    g_signal_connect(authors_btn, "clicked", G_CALLBACK(show_authors_page), NULL);
    g_signal_connect(publishers_btn, "clicked", G_CALLBACK(show_publishers_page), NULL);
    g_signal_connect(members_btn, "clicked", G_CALLBACK(show_members_page), NULL);
    g_signal_connect(staff_btn, "clicked", G_CALLBACK(show_staff_page), NULL);
    g_signal_connect(borrowings_btn, "clicked", G_CALLBACK(show_borrowings_page), NULL);
    g_signal_connect(fines_btn, "clicked", G_CALLBACK(show_fines_page), NULL);

    // Add CSS
    GtkCssProvider *provider = gtk_css_provider_new();
    gtk_css_provider_load_from_data(provider,
        ".sidebar-button {"
        "    padding: 10px;"
        "    margin: 2px;"
        "    border-radius: 5px;"
        "}"
        ".sidebar-button:hover {"
        "    background-color: #e0e0e0;"
        "}"
        ".sidebar-button:active {"
        "    background-color: #d0d0d0;"
        "}", -1, NULL);
    gtk_style_context_add_provider_for_screen(gdk_screen_get_default(),
                                            GTK_STYLE_PROVIDER(provider),
                                            GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);

    // Show all widgets
    gtk_widget_show_all(main_window);

    // Show books page by default
    show_books_page();
}

// Window destroy callback
static gboolean on_window_destroy(GtkWidget* widget, gpointer user_data) {
    (void)widget; // Suppress unused parameter warning
    (void)user_data; // Suppress unused parameter warning
    if (db != NULL) {
        db_close(db);
    }
    gtk_main_quit();
    return TRUE;
}

// Placeholder page functions
void show_books_page(void) {
    // Clear content area
    GList *children = gtk_container_get_children(GTK_CONTAINER(content_area));
    GList *iter;
    for (iter = children; iter != NULL; iter = iter->next) {
        gtk_widget_destroy(GTK_WIDGET(iter->data));
    }
    g_list_free(children);
    
    // Create and show books page
    GtkWidget *books_page = create_books_page(db);
    gtk_container_add(GTK_CONTAINER(content_area), books_page);
    gtk_widget_show_all(content_area);
}

void show_authors_page(void) {
    // Clear content area
    GList *children = gtk_container_get_children(GTK_CONTAINER(content_area));
    GList *iter;
    for (iter = children; iter != NULL; iter = iter->next) {
        gtk_widget_destroy(GTK_WIDGET(iter->data));
    }
    g_list_free(children);
    
    // Create a placeholder label
    GtkWidget *label = gtk_label_new("Authors Page - Coming Soon");
    gtk_container_add(GTK_CONTAINER(content_area), label);
    gtk_widget_show_all(content_area);
}

void show_publishers_page(void) {
    // Clear content area
    GList *children = gtk_container_get_children(GTK_CONTAINER(content_area));
    GList *iter;
    for (iter = children; iter != NULL; iter = iter->next) {
        gtk_widget_destroy(GTK_WIDGET(iter->data));
    }
    g_list_free(children);
    
    // Create a placeholder label
    GtkWidget *label = gtk_label_new("Publishers Page - Coming Soon");
    gtk_container_add(GTK_CONTAINER(content_area), label);
    gtk_widget_show_all(content_area);
}

void show_members_page(void) {
    // Clear content area
    GList *children = gtk_container_get_children(GTK_CONTAINER(content_area));
    GList *iter;
    for (iter = children; iter != NULL; iter = iter->next) {
        gtk_widget_destroy(GTK_WIDGET(iter->data));
    }
    g_list_free(children);
    
    // Create a placeholder label
    GtkWidget *label = gtk_label_new("Members Page - Coming Soon");
    gtk_container_add(GTK_CONTAINER(content_area), label);
    gtk_widget_show_all(content_area);
}

void show_staff_page(void) {
    // Clear content area
    GList *children = gtk_container_get_children(GTK_CONTAINER(content_area));
    GList *iter;
    for (iter = children; iter != NULL; iter = iter->next) {
        gtk_widget_destroy(GTK_WIDGET(iter->data));
    }
    g_list_free(children);
    
    // Create a placeholder label
    GtkWidget *label = gtk_label_new("Staff Page - Coming Soon");
    gtk_container_add(GTK_CONTAINER(content_area), label);
    gtk_widget_show_all(content_area);
}

void show_borrowings_page(void) {
    // Clear content area
    GList *children = gtk_container_get_children(GTK_CONTAINER(content_area));
    GList *iter;
    for (iter = children; iter != NULL; iter = iter->next) {
        gtk_widget_destroy(GTK_WIDGET(iter->data));
    }
    g_list_free(children);
    
    // Create a placeholder label
    GtkWidget *label = gtk_label_new("Borrowings Page - Coming Soon");
    gtk_container_add(GTK_CONTAINER(content_area), label);
    gtk_widget_show_all(content_area);
}

void show_fines_page(void) {
    // Clear content area
    GList *children = gtk_container_get_children(GTK_CONTAINER(content_area));
    GList *iter;
    for (iter = children; iter != NULL; iter = iter->next) {
        gtk_widget_destroy(GTK_WIDGET(iter->data));
    }
    g_list_free(children);
    
    // Create a placeholder label
    GtkWidget *label = gtk_label_new("Fines Page - Coming Soon");
    gtk_container_add(GTK_CONTAINER(content_area), label);
    gtk_widget_show_all(content_area);
} 