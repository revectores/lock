# LOCK_IMPL = spinlock
# LOCK_IMPL = pthread_mutex
LOCK_IMPL = yield_mutex
# LOCK_IMPL = futex_mutex


LOCK_PATH = lock/$(LOCK_IMPL)

ifeq ($(LOCK_IMPL), spinlock)
	LOCK_NAME = spinlock
else ifeq ($(LOCK_IMPL), pthread_mutex)
	LOCK_NAME = pthread_mutex
else
	LOCK_NAME = mutex
endif

ifneq ($(LOCK_NAME), pthread_mutex)
	LOCK_OBJ = $(LOCK_PATH)/$(LOCK_NAME).o
endif


COUNTER_PATH = counter/counter_$(LOCK_NAME)
LIST_PATH = list/list_$(LOCK_NAME)
HASH_PATH = hash/hash_$(LOCK_NAME)
LIB_PATH = lib
TIMING_PATH = timing
CC = gcc -std=gnu99 -pthread -I $(LOCK_PATH) -I $(COUNTER_PATH) -I $(LIST_PATH) -I $(HASH_PATH) -I $(TIMING_PATH)
OBJECTS = $(TIMING_PATH)/timing_counter_increment $(TIMING_PATH)/timing_list_insert $(TIMING_PATH)/timing_hash_insert



.PHONY: all


all: $(OBJECTS)

$(TIMING_PATH)/timing_counter_increment: $(TIMING_PATH)/timing_counter_increment.c $(LIB_PATH)/counter.so $(LIB_PATH)/timing.so
	$(CC) -L $(LIB_PATH) -lcounter -ltiming -o $(TIMING_PATH)/timing_counter_increment $(TIMING_PATH)/timing_counter_increment.c

$(TIMING_PATH)/timing_list_insert: $(TIMING_PATH)/timing_list_insert.c $(LIB_PATH)/list.so $(LIB_PATH)/timing.so
	$(CC) -L $(LIB_PATH) -llist -ltiming -o $(TIMING_PATH)/timing_list_insert $(TIMING_PATH)/timing_list_insert.c

$(TIMING_PATH)/timing_hash_insert: $(TIMING_PATH)/timing_hash_insert.c $(LIB_PATH)/hash.so $(LIB_PATH)/timing.so
	$(CC) -L $(LIB_PATH) -lhash -ltiming -o $(TIMING_PATH)/timing_hash_insert $(TIMING_PATH)/timing_hash_insert.c

$(LIB_PATH)/timing.so: $(TIMING_PATH)/timing.o
	$(CC) -shared -o $(LIB_PATH)/libtiming.so $(TIMING_PATH)/timing.o

$(LIB_PATH)/counter.so: $(COUNTER_PATH)/counter.o $(LOCK_OBJ)
	$(CC) -shared -o $(LIB_PATH)/libcounter.so $(COUNTER_PATH)/counter.o $(LOCK_OBJ)

$(LIB_PATH)/list.so: $(LIST_PATH)/list.o $(LOCK_OBJ)
	$(CC) -shared -o $(LIB_PATH)/liblist.so $(LIST_PATH)/list.o $(LOCK_OBJ)

$(LIB_PATH)/hash.so: $(HASH_PATH)/hash.o $(LOCK_OBJ) $(LIST_PATH)/list.o
	$(CC) -shared -o $(LIB_PATH)/libhash.so $(HASH_PATH)/hash.o $(LOCK_OBJ) $(LIST_PATH)/list.o

ifdef LOCK_OBJ
$(LOCK_OBJ): $(LOCK_PATH)/$(LOCK_NAME).c
	$(CC) -o $(LOCK_PATH)/$(LOCK_NAME).o -c -fPIC $(LOCK_PATH)/$(LOCK_NAME).c
endif

$(COUNTER_PATH)/counter.o: $(COUNTER_PATH)/counter.c
	$(CC) -o $(COUNTER_PATH)/counter.o -c -fPIC $(COUNTER_PATH)/counter.c

$(LIST_PATH)/list.o: $(LIST_PATH)/list.c
	$(CC) -o $(LIST_PATH)/list.o -c -fPIC $(LIST_PATH)/list.c

$(HASH_PATH)/hash.o: $(HASH_PATH)/hash.c
	$(CC) -o $(HASH_PATH)/hash.o -c -fPIC $(HASH_PATH)/hash.c

$(TIMING_PATH)/timing.o: $(TIMING_PATH)/timing.c
	$(CC) -o $(TIMING_PATH)/timing.o -c -fPIC $(TIMING_PATH)/timing.c

clean:
	-rm $(OBJECTS)
	-find . -name '*.o' -delete
	-find . -name '*.so' -delete
