LOCK_NAME = spinlock
LOCK_PATH = lock/$(LOCK_NAME)
COUNTER_PATH = counter/counter_$(LOCK_NAME)
LIST_PATH = list/list_$(LOCK_NAME)
TIMING_PATH = timing
CC = gcc -I $(LOCK_PATH) -I $(COUNTER_PATH) -I $(LIST_PATH) -I $(TIMING_PATH)


$(TIMING_PATH)/timing_counter_increment: $(TIMING_PATH)/timing_counter_increment.c $(COUNTER_PATH)/counter.so $(TIMING_PATH)/timing.so
	$(CC) -lcounter -L $(COUNTER_PATH) -ltiming -L $(TIMING_PATH) -o $(TIMING_PATH)/timing_counter_increment $(TIMING_PATH)/timing_counter_increment.c

$(COUNTER_PATH)/counter.so: $(COUNTER_PATH)/counter.o $(LOCK_PATH)/$(LOCK_NAME).o
	$(CC) -shared -o $(COUNTER_PATH)/libcounter.so $(COUNTER_PATH)/counter.o $(LOCK_PATH)/$(LOCK_NAME).o

$(COUNTER_PATH)/counter.o: $(COUNTER_PATH)/counter.c
	$(CC) -o $(COUNTER_PATH)/counter.o -c $(COUNTER_PATH)/counter.c

$(LIST_PATH)/list.so: $(LIST_PATH)/list.o $(LOCK_PATH)/$(LOCK_NAME).o
	$(CC) -shared -o $(LIST_PATH)/liblist.so $(LIST_PATH)/list.o $(LOCK_PATH)/$(LOCK_NAME).o

$(LIST_PATH)/list.o: $(LIST_PATH)/list.c
	$(CC) -o $(LIST_PATH)/list.o -c $(LIST_PATH)/list.c

$(LOCK_PATH)/$(LOCK_NAME).o: $(LOCK_PATH)/$(LOCK_NAME).c
	$(CC) -o $(LOCK_PATH)/$(LOCK_NAME).o -c $(LOCK_PATH)/$(LOCK_NAME).c

$(TIMING_PATH)/timing.so: $(TIMING_PATH)/timing.o
	$(CC) -shared -o $(TIMING_PATH)/libtiming.so $(TIMING_PATH)/timing.o

$(TIMING_PATH)/timing.o: $(TIMING_PATH)/timing.c
	$(CC) -o $(TIMING_PATH)/timing.o -c $(TIMING_PATH)/timing.c

clean:
	rm timing/timing_counter_increment
	find . -name '*.o' -delete
	find . -name '*.so' -delete