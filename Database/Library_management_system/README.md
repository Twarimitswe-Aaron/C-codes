# Library Management System

A comprehensive library management system implemented in C with MySQL database integration. The system provides both console and GUI interfaces for managing library operations.

## Features

- Book management (add, update, delete, search)
- Author management
- Publisher management
- Member management
- Staff management
- Borrowing system
- Fine management

## Database Schema

The system uses the following database tables:
- Books
- Authors
- Publishers
- Members
- Staff
- Borrowings
- Fines

## Requirements

- C compiler (GCC recommended)
- MySQL Server
- MySQL Connector/C
- GTK3 (for GUI interface)
- CMake (for building)

## Building the Project

1. Install dependencies:
```bash
# For Ubuntu/Debian
sudo apt-get install build-essential libmysqlclient-dev libgtk-3-dev cmake
```

2. Build the project:
```bash
mkdir build
cd build
cmake ..
make
```

## Usage

### Console Interface
```bash
./library_management_console
```

### GUI Interface
```bash
./library_management_gui
```

## Project Structure

- `src/` - Source code files
  - `database/` - Database connection and schema
  - `models/` - Data models
  - `console/` - Console interface
  - `gui/` - GUI interface
- `include/` - Header files
- `build/` - Build directory
- `CMakeLists.txt` - CMake configuration 