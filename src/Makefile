CC = gcc

CFLAGS = -std=gnu99 -g -MD -rdynamic

AM_CFLAGS = -Wall

#LIBS =

TARGET = bebe_server

OBJS = bebe_main.o bb_util.o bb_connection.o bb_conn.o bb_malloc.o bb_config.o

all: $(TARGET)
# ----------- Application --------------------------------------------------
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS) $(LIBS)
# ----------- OBJS ---------------------------------------------------------
.c.o:
	$(CC) ${CFLAGS} $(AM_CFLAGS)  -c -o $@ $< $(LIBS)

sources = *.c
-include $(sources:.c=.d)
# ----------- CLEAN --------------------------------------------------------
clean:
	$(RM) *.o *.d bebe_server
