LOCK_NAME = spinlock
LOCK_PATH = lock/$(LOCK_NAME)
COUNTER_PATH = counter/counter_$(LOCK_NAME)
CC = gcc -I $(LOCK_PATH) -I $(COUNTER_PATH)


main: main.c $(COUNTER_PATH)/counter.so
	$(CC) -lcounter -L $(COUNTER_PATH) -o main main.c

$(COUNTER_PATH)/counter.so: $(COUNTER_PATH)/counter.o $(LOCK_PATH)/$(LOCK_NAME).o
	$(CC) -shared -o $(COUNTER_PATH)/libcounter.so $(COUNTER_PATH)/counter.o $(LOCK_PATH)/$(LOCK_NAME).o

$(COUNTER_PATH)/counter.o: $(COUNTER_PATH)/counter.c
	$(CC) -o $(COUNTER_PATH)/counter.o -c $(COUNTER_PATH)/counter.c

$(LOCK_PATH)/$(LOCK_NAME).o: $(LOCK_PATH)/$(LOCK_NAME).c
	$(CC) -o $(LOCK_PATH)/$(LOCK_NAME).o -c $(LOCK_PATH)/$(LOCK_NAME).c

clean:
	rm main
	find . -name '*.o' -delete
	find . -name '*.so' -delete