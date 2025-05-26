#ifndef CONSOLE_UI_H
#define CONSOLE_UI_H

#include "database.h"

// Initialize the console interface
void init_console(void);

// Run the main console loop
void run_console(Database *database);

#endif // CONSOLE_UI_H 