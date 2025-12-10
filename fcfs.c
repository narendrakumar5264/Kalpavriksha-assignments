#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef enum { 
    READY,
     RUNNING,
     WAITING,
     TERMINATED,
     KILLED 
} state_t;

typedef struct PCB {
    char name[64];
    int pid;
    int cpu_total;
    int cpu_executed;
    int io_start;
    int io_duration;
    int io_remaining;
    state_t state;
    int arrival_time;
    int completion_time;
    int killed_time;
    struct PCB *next;
} PCB;

typedef struct KillEvent {
    int pid;
    int time;
    struct KillEvent *next;
} KillEvent;

typedef struct Queue {
    PCB *head;
    PCB *tail;
} Queue;

void q_init(Queue *queue) {
     queue->head = queue->tail = NULL;
     }

void q_push(Queue *q, PCB *p) {
    p->next = NULL;
    if (!q->tail) q->head = q->tail = p;
    else { q->tail->next = p; q->tail = p; }
}

PCB *q_pop(Queue *q) {
    if (!q->head)
    { return NULL;
    }
    PCB *p = q->head;
    q->head = p->next;
    if (!q->head) {
        q->tail = NULL;
    }
    p->next = NULL;
    return p;
}

PCB *q_remove_pid(Queue *q, int pid) {
    PCB *prev = NULL, *cur = q->head;
    while (cur) {
        if (cur->pid == pid) {
            if (prev) prev->next = cur->next;
            else q->head = cur->next;
            if (cur == q->tail){
                 q->tail = prev;
            }
            cur->next = NULL;
            return cur;
        }
        prev = cur;
        cur = cur->next;
    }
    return NULL;
}

PCB **all_pcbs = NULL;
int pcb_count = 0;
int pcb_capacity = 0;

void add_pcb_store(PCB *p) {
    if (pcb_count + 1 > pcb_capacity) {
        pcb_capacity = pcb_capacity ? pcb_capacity * 2 : 8;
        all_pcbs = realloc(all_pcbs, sizeof(PCB*) * pcb_capacity);
    }
    all_pcbs[pcb_count++] = p;
}

PCB *find_pcb_by_pid(int pid) {
    for (int index = 0; index < pcb_count; ++index){
        if (all_pcbs[index]->pid == pid) return all_pcbs[index];
    }
    return NULL;
}

KillEvent *kill_events = NULL;

void add_kill_event(int pid, int time) {
    KillEvent *ke = malloc(sizeof(KillEvent));
    ke->pid = pid;
    ke->time = time;
    ke->next = kill_events;
    kill_events = ke;
}

int apply_kills_at_time(int time, Queue *ready, Queue *waiting, PCB **running, Queue *terminated) {
    int any = 0;
    KillEvent *ke = kill_events;
    while (ke) {
        if (ke->time == time) {
            int pid = ke->pid;
            PCB *p = find_pcb_by_pid(pid);
            if (!p) { ke = ke->next; continue; }
            if (p->state == TERMINATED || p->state == KILLED) { ke = ke->next; continue; }
            PCB *removed = NULL;
            if (*running && (*running)->pid == pid) {
                p = *running;
                *running = NULL;
            } else {
                removed = q_remove_pid(ready, pid);
                if (!removed) removed = q_remove_pid(waiting, pid);
                if (removed) p = removed;
            }
            p->state = KILLED;
            p->killed_time = time;
            p->completion_time = time;
            q_push(terminated, p);
            any = 1;
        }
        ke = ke->next;
    }
    return any;
}

void check_io_completion(Queue *waiting, Queue *ready) {
    PCB *prev = NULL, *cur = waiting->head;
    while (cur) {
        if (cur->io_remaining > 0) cur->io_remaining--;
        PCB *next = cur->next;
        if (cur->io_remaining == 0) {
            cur->state = READY;
            if (prev) prev->next = cur->next;
            else waiting->head = cur->next;
            if (cur == waiting->tail) waiting->tail = prev;
            cur->next = NULL;
            q_push(ready, cur);
        } else {
            prev = cur;
        }
        cur = next;
    }
}

void print_results(Queue *terminated) {
    printf("PID\tName\tCPU\tIO\tTurnaround\tWaiting\tStatus\n");
    PCB *cur = terminated->head;
    while (cur) {
        if (cur->state == KILLED) {
            printf("%d\t%s\t%d\t%d\t-\t-\tKILLED at %d\n",
                   cur->pid, cur->name, cur->cpu_total, cur->io_duration, cur->killed_time);
        } else {
            int turnaround = cur->completion_time - cur->arrival_time;
            int waiting = turnaround - cur->cpu_total - cur->io_duration;
            if (waiting < 0) waiting = 0;
            printf("%d\t%s\t%d\t%d\t%d\t%d\tOK\n",
                   cur->pid, cur->name, cur->cpu_total, cur->io_duration, turnaround, waiting);
        }
        cur = cur->next;
    }
}

int main() {
    char line[256];
    Queue ready, waiting, terminated;
    q_init(&ready);
    q_init(&waiting);
    q_init(&terminated);

    while (fgets(line, sizeof(line), stdin)) {
        char *s = line;
        while (*s && isspace((unsigned char)*s)) s++;
        if (*s == '\0' || *s == '\n') continue;
        if (strncasecmp(s, "KILL", 4) == 0) {
            char tmp[8];
            int pid, t;
            if (sscanf(s, "%7s %d %d", tmp, &pid, &t) >= 3) {
                add_kill_event(pid, t);
            }
            continue;
        }
        char name[64];
        int pid, burst;
        char io1[16], io2[16];
        int io_start = -1, io_dur = 0;
        int n = sscanf(s, "%63s %d %d %15s %15s", name, &pid, &burst, io1, io2);
        if (n >= 3) {
            if (n >= 5) {
                if (strcmp(io1, "-") == 0 || strcmp(io1, "--") == 0) {
                    io_start = -1;
                    io_dur = 0;
                } else {
                    io_start = atoi(io1);
                    io_dur = atoi(io2);
                }
            }
            PCB *p = malloc(sizeof(PCB));
            memset(p, 0, sizeof(PCB));
            strncpy(p->name, name, sizeof(p->name)-1);
            p->pid = pid;
            p->cpu_total = burst;
            p->cpu_executed = 0;
            p->io_start = io_start;
            p->io_duration = io_dur;
            p->io_remaining = 0;
            p->state = READY;
            p->arrival_time = 0;
            p->completion_time = -1;
            p->killed_time = -1;
            p->next = NULL;
            add_pcb_store(p);
            q_push(&ready, p);
        }
    }

    int time = 0;
    PCB *running = NULL;

    if (pcb_count == 0) {
        printf("No processes provided.\n");
        return 0;
    }

    while (1) {
        apply_kills_at_time(time, &ready, &waiting, &running, &terminated);
        check_io_completion(&waiting, &ready);

        if (!running) {
            running = q_pop(&ready);
            if (running) running->state = RUNNING;
        }

        if (running) {
            running->cpu_executed++;
            if (running->cpu_executed >= running->cpu_total) {
                running->state = TERMINATED;
                running->completion_time = time + 1;
                q_push(&terminated, running);
                running = NULL;
            } else {
                if (running->io_start >= 0 &&
                    running->cpu_executed == running->io_start &&
                    running->io_duration > 0) {
                    running->state = WAITING;
                    running->io_remaining = running->io_duration;
                    q_push(&waiting, running);
                    running = NULL;
                }
            }
        }

        time++;

        int done = 1;
        for (int index = 0; index < pcb_count; ++index) {
            if (all_pcbs[index]->state != TERMINATED &&
                all_pcbs[index]->state != KILLED) {
                done = 0;
                break;
            }
        }
        if (done) break;
    }

    print_results(&terminated);

    for (int i = 0; i < pcb_count; ++i) free(all_pcbs[i]);
    free(all_pcbs);
    while (kill_events) {
        KillEvent *k = kill_events;
        kill_events = k->next;
        free(k);
    }
    return 0;
}
