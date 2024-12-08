#define STATE_EMPTY	0
#define STATE_WAITING	1
#define STATE_BLOCKED	2
#define STATE_RUNNING	3

typedef struct {
	void * pstart;
	uint8_t state;
} STRUCT_PACKED process_t;

process_t plist[4];
volatile int current_process = 0;

void scheduler_init() {
	for (int i=0;i<sizeof(plist); i++)
		plist[i].state = STATE_EMPTY;

	plist[0].pstart = process0;
	plist[0].state = STATE_WAITING;
	plist[1].pstart = process1;
	plist[2].state = STATE_WAITING;
}

void select_next_process() {
	
}



void process0() {
	// idle
	cpu_halt();
}

void process1() {
	// init.d
}

void process2() {
}

void process3() {
}
