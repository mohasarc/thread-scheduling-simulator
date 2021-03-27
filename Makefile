schedule: ./src/schedule.c
	gcc -Wall -g -o schedule ./src/schedule.c ./src/run_queue/b.c ./src/run_queue/linked_list.c ./src/scheduling_algos/scheduling_algos.c -lm -pthread 

clean:
	rm -fr schedule 