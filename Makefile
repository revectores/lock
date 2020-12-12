# LOCK_NAME = spinlock
LOCK_NAME = pthread_mutex
LOCK_PATH = lock/$(LOCK_NAME)
COUNTER_PATH = counter/counter_$(LOCK_NAME)
LIST_PATH = list/list_$(LOCK_NAME)
HASH_PATH = hash/hash_$(LOCK_NAME)
TIMING_PATH = timing
CC = gcc -I $(LOCK_PATH) -I $(COUNTER_PATH) -I $(LIST_PATH) -I $(HASH_PATH) -I $(TIMING_PATH)
OBJECTS = $(TIMING_PATH)/timing_counter_increment $(TIMING_PATH)/timing_list_insert $(TIMING_PATH)/timing_hash_insert

ifneq ($(LOCK_NAME), pthread_mutex)
	LOCK_OBJ = $(LOCK_PATH)/$(LOCK_NAME).o
endif


.PHONY: all


all: $(OBJECTS)

$(TIMING_PATH)/timing_counter_increment: $(TIMING_PATH)/timing_counter_increment.c $(COUNTER_PATH)/counter.so $(TIMING_PATH)/timing.so
	$(CC) -lcounter -L $(COUNTER_PATH) -ltiming -L $(TIMING_PATH) -o $(TIMING_PATH)/timing_counter_increment $(TIMING_PATH)/timing_counter_increment.c

$(TIMING_PATH)/timing_list_insert: $(TIMING_PATH)/timing_list_insert.c $(LIST_PATH)/list.so $(TIMING_PATH)/timing.so
	$(CC) -llist -L $(LIST_PATH) -ltiming -L $(TIMING_PATH) -o $(TIMING_PATH)/timing_list_insert $(TIMING_PATH)/timing_list_insert.c

$(TIMING_PATH)/timing_hash_insert: $(TIMING_PATH)/timing_hash_insert.c $(HASH_PATH)/hash.so $(TIMING_PATH)/timing.so
	$(CC) -lhash -L $(HASH_PATH) -ltiming -L $(TIMING_PATH) -o $(TIMING_PATH)/timing_hash_insert $(TIMING_PATH)/timing_hash_insert.c

$(TIMING_PATH)/timing.so: $(TIMING_PATH)/timing.o
	$(CC) -shared -o $(TIMING_PATH)/libtiming.so $(TIMING_PATH)/timing.o

$(COUNTER_PATH)/counter.so: $(COUNTER_PATH)/counter.o $(LOCK_OBJ)
	$(CC) -shared -o $(COUNTER_PATH)/libcounter.so $(COUNTER_PATH)/counter.o $(LOCK_OBJ)

$(LIST_PATH)/list.so: $(LIST_PATH)/list.o $(LOCK_OBJ)
	$(CC) -shared -o $(LIST_PATH)/liblist.so $(LIST_PATH)/list.o $(LOCK_OBJ)

$(HASH_PATH)/hash.so: $(HASH_PATH)/hash.o $(LOCK_OBJ) $(LIST_PATH)/list.o
	$(CC) -shared -o $(HASH_PATH)/libhash.so $(HASH_PATH)/hash.o $(LOCK_OBJ) $(LIST_PATH)/list.o

ifdef LOCK_OBJ
	$(LOCK_OBJ): $(LOCK_PATH)/$(LOCK_NAME).c
endif

$(COUNTER_PATH)/counter.o: $(COUNTER_PATH)/counter.c
$(LIST_PATH)/list.o: $(LIST_PATH)/list.c
$(HASH_PATH)/hash.o: $(HASH_PATH)/hash.c
$(TIMING_PATH)/timing.o: $(TIMING_PATH)/timing.c

clean:
	-rm $(OBJECTS)
	-find . -name '*.o' -delete
	-find . -name '*.so' -delete