/*
 * TaskMgr.h
 *
 * Created: 18.02.2015 18:17:32
 *  Author: Disgust
 */ 


#ifndef TASKMGR_H_
#define TASKMGR_H_

#define taskStackDepth 16;

typedef void (*task)(void);

typedef struct {
	task routine;
	uint16_t pCounter;
	uint8_t sreg;
	uint8_t stack[taskStackDepth];
	//...
} task_t;

task_t taskQueue[255];

void TASK_(task_t * task) { // task container
	
}

void IDLE() { // this is an empty task
	while (1) {
		;;
	}
}

#endif /* TASKMGR_H_ */