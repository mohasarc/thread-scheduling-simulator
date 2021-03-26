schedule: ./src/schedule.c
	gcc -Wall -g -o schedule ./src/schedule.c ./src/run_queue/pcb.c ./src/run_queue/linked_list.c -lm -pthread 

clean:
	rm -fr schedule 