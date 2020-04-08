CC = gcc
CFLAGS = 
LDFLAGS =
OBJFILES = shellcmd.o parse.o shell.o  
TARGET = shell.exe
all: $(TARGET)
$(TARGET): $(OBJFILES)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJFILES) $(LDFLAGS)

clean:
	rm -f $(OBJFILES) $(TARGET) *~