CC = gcc
CFLAGS = -Wall -Wextra
LDFLAGS = -lcomctl32

SRC = main.c database.c console_ui.c gui_ui.c
OBJ = $(SRC:.c=.o)
TARGET = library_management_system.exe

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) $(SRC) -o $(TARGET) $(LDFLAGS) -mwindows

clean:
	del /Q *.o $(TARGET) 