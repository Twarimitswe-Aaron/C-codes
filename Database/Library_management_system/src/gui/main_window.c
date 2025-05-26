#include <gtk/gtk.h>
#include "../../include/database.h"
#include "../../include/book.h"
#include "../../include/author.h"
#include "../../include/publisher.h"
#include "../../include/member.h"
#include "../../include/staff.h"
#include "../../include/borrowing.h"
#include "../../include/fine.h"

// Global variables
GtkWidget *main_window;
Database *db;

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

// Main window creation
void create_main_window(void) {
    // Create the main window
    main_window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(main_window), "Library Management System");
    gtk_window_set_default_size(GTK_WINDOW(main_window), 800, 600);
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

    // Create sidebar buttons
    GtkWidget *books_btn = gtk_button_new_with_label("Books");
    GtkWidget *authors_btn = gtk_button_new_with_label("Authors");
    GtkWidget *publishers_btn = gtk_button_new_with_label("Publishers");
    GtkWidget *members_btn = gtk_button_new_with_label("Members");
    GtkWidget *staff_btn = gtk_button_new_with_label("Staff");
    GtkWidget *borrowings_btn = gtk_button_new_with_label("Borrowings");
    GtkWidget *fines_btn = gtk_button_new_with_label("Fines");

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

    // Create content area
    GtkWidget *content_area = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    gtk_box_pack_start(GTK_BOX(content_box), content_area, TRUE, TRUE, 0);

    // Show all widgets
    gtk_widget_show_all(main_window);
}

// Window destroy callback
void on_window_destroy(GtkWidget *widget, gpointer data) {
    if (db != NULL) {
        db_disconnect(db);
    }
    gtk_main_quit();
}

// Placeholder page functions
void show_books_page(void) {
    // TODO: Implement books page
}

void show_authors_page(void) {
    // TODO: Implement authors page
}

void show_publishers_page(void) {
    // TODO: Implement publishers page
}

void show_members_page(void) {
    // TODO: Implement members page
}

void show_staff_page(void) {
    // TODO: Implement staff page
}

void show_borrowings_page(void) {
    // TODO: Implement borrowings page
}

void show_fines_page(void) {
    // TODO: Implement fines page
} 