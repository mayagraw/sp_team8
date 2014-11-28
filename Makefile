CC = gcc

CFLAGS = -std=c99 -O3
LDFLAGS = -lm -lgomp

#SOURCES
RM  = rm -f
EXEC = TEAM8

OBJECTS = client.o server.o db.o

all : $(EXEC)
	time ./$(EXEC) | tee log 

$(EXEC) : $(OBJECTS) touch
	rm -f $(EXEC)
	$(CC) $(OBJECTS) -o $@ $(LDFLAGS)

%.o : src/%.c include/db.h include/common.h include/msg.h
	$(CC) $(CFLAGS) -c $< -o $@

.PHONY : clean

clean:
	$(RM) $(OBJECTS) $(EXEC)

touch: 
	@echo " "
	@echo "Compilation done successfully..................."
	@echo "Executable generated: TEAM8 "
	@echo " "



