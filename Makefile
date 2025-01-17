CFLAGS = -O2 -g -std=c99 -Wall -I include

CFLAGS += -D_POSIX_C_SOURCE=199506L

OUT = bin
EXEC = $(OUT)/server
OBJS = \
	src/main.o \
	src/request.o \
	src/response.o \
	src/server.o

deps := $(OBJS:%.o=%.o.d)

src/%.o: src/%.c
	$(CC) $(CFLAGS) -o $@ -MMD -MF $@.d -c $<

$(EXEC): $(OBJS)
	mkdir -p $(OUT)
	$(CC) $(OBJS) -o $@ -pthread

run: $(EXEC)
	@echo "Starting Server..."
	@./$(EXEC) $(port)

clean:
	$(RM) $(OBJS) $(EXEC) $(deps)

-include $(deps)