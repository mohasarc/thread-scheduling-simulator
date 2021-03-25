all: schedule # producer consumer

# producer: ./src/producer.c
# 	gcc -Wall -g -o producer ./src/producer.c

# consumer: ./src/consumer.c
# 	gcc -Wall -g -o consumer ./src/consumer.c

schedule: ./src/schedule.c
	gcc -Wall -g -o schedule ./src/schedule.c ./src/run_queue/pcb.c ./src/run_queue/linked_list.c -lm

clean:
	rm -fr schedule 
	#producer consumer *dSYM