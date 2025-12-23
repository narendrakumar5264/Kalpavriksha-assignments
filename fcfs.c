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
} ProcessState;

typedef struct ProcessControlBlock {
    char process_name[64];
    int process_id;
    int total_cpu_time;
    int executed_cpu_time;
    int io_start_time;
    int io_total_duration;
    int io_remaining_time;
    ProcessState state;
    int arrival_time;
    int completion_time;
    int killed_time;
    struct ProcessControlBlock *next;
} ProcessControlBlock;

typedef struct KillEvent {
    int process_id;
    int kill_time;
    struct KillEvent *next;
} KillEvent;

typedef struct ProcessQueue {
    ProcessControlBlock *head;
    ProcessControlBlock *tail;
} ProcessQueue;

void init_queue(ProcessQueue *queue) {
    queue->head = NULL;
    queue->tail = NULL;
}

void enqueue(ProcessQueue *queue, ProcessControlBlock *process) {
    process->next = NULL;
    if (!queue->tail) {
        queue->head = queue->tail = process;
    } else {
        queue->tail->next = process;
        queue->tail = process;
    }
}

ProcessControlBlock *dequeue(ProcessQueue *queue) {
    if (!queue->head) return NULL;
    ProcessControlBlock *process = queue->head;
    queue->head = process->next;
    if (!queue->head) queue->tail = NULL;
    process->next = NULL;
    return process;
}

ProcessControlBlock *remove_process_by_id(ProcessQueue *queue, int process_id) {
    ProcessControlBlock *previous = NULL;
    ProcessControlBlock *current = queue->head;

    while (current) {
        if (current->process_id == process_id) {
            if (previous) previous->next = current->next;
            else queue->head = current->next;
            if (current == queue->tail) queue->tail = previous;
            current->next = NULL;
            return current;
        }
        previous = current;
        current = current->next;
    }
    return NULL;
}

ProcessControlBlock **process_table = NULL;
int process_count = 0;
int process_capacity = 0;

void store_process(ProcessControlBlock *process) {
    if (process_count >= process_capacity) {
        process_capacity = process_capacity ? process_capacity * 2 : 8;
        process_table = realloc(process_table, sizeof(ProcessControlBlock *) * process_capacity);
    }
    process_table[process_count++] = process;
}

ProcessControlBlock *get_process_by_id(int process_id) {
    for (int index = 0; index < process_count; index++) {
        if (process_table[index]->process_id == process_id)
            return process_table[index];
    }
    return NULL;
}

KillEvent *kill_event_list = NULL;

void add_kill_event(int process_id, int kill_time) {
    KillEvent *event = malloc(sizeof(KillEvent));
    event->process_id = process_id;
    event->kill_time = kill_time;
    event->next = kill_event_list;
    kill_event_list = event;
}

void apply_kill_events(
    int current_time,
    ProcessQueue *ready_queue,
    ProcessQueue *waiting_queue,
    ProcessControlBlock **running_process,
    ProcessQueue *terminated_queue
) {
    KillEvent *event = kill_event_list;

    while (event) {
        if (event->kill_time == current_time) {
            ProcessControlBlock *process = get_process_by_id(event->process_id);
            if (!process || process->state == TERMINATED || process->state == KILLED) {
                event = event->next;
                continue;
            }

            if (*running_process && (*running_process)->process_id == event->process_id) {
                process = *running_process;
                *running_process = NULL;
            } else {
                process = remove_process_by_id(ready_queue, event->process_id);
                if (!process)
                    process = remove_process_by_id(waiting_queue, event->process_id);
            }

            process->state = KILLED;
            process->killed_time = current_time;
            process->completion_time = current_time;
            enqueue(terminated_queue, process);
        }
        event = event->next;
    }
}

void update_io(ProcessQueue *waiting_queue, ProcessQueue *ready_queue) {
    ProcessControlBlock *previous = NULL;
    ProcessControlBlock *current = waiting_queue->head;

    while (current) {
        if (current->io_remaining_time > 0)
            current->io_remaining_time--;

        ProcessControlBlock *next = current->next;

        if (current->io_remaining_time == 0) {
            current->state = READY;
            if (previous) previous->next = current->next;
            else waiting_queue->head = current->next;
            if (current == waiting_queue->tail) waiting_queue->tail = previous;
            current->next = NULL;
            enqueue(ready_queue, current);
        } else {
            previous = current;
        }
        current = next;
    }
}

void print_summary(ProcessQueue *terminated_queue) {
    printf("PID\tName\tCPU\tIO\tTurnaround\tWaiting\tStatus\n");
    ProcessControlBlock *current = terminated_queue->head;

    while (current) {
        if (current->state == KILLED) {
            printf("%d\t%s\t%d\t%d\t-\t-\tKILLED at %d\n",
                   current->process_id,
                   current->process_name,
                   current->total_cpu_time,
                   current->io_total_duration,
                   current->killed_time);
        } else {
            int turnaround_time = current->completion_time - current->arrival_time;
            int waiting_time = turnaround_time - current->total_cpu_time - current->io_total_duration;
            if (waiting_time < 0) waiting_time = 0;
            printf("%d\t%s\t%d\t%d\t%d\t%d\tOK\n",
                   current->process_id,
                   current->process_name,
                   current->total_cpu_time,
                   current->io_total_duration,
                   turnaround_time,
                   waiting_time);
        }
        current = current->next;
    }
}

int main() {
    char input_line[256];
    ProcessQueue ready_queue, waiting_queue, terminated_queue;
    init_queue(&ready_queue);
    init_queue(&waiting_queue);
    init_queue(&terminated_queue);

    while (fgets(input_line, sizeof(input_line), stdin)) {
        char *cursor = input_line;
        while (*cursor && isspace((unsigned char)*cursor)) cursor++;
        if (*cursor == '\0' || *cursor == '\n') continue;

        if (strncasecmp(cursor, "KILL", 4) == 0) {
            char command[8];
            int process_id, kill_time;
            if (sscanf(cursor, "%7s %d %d", command, &process_id, &kill_time) == 3)
                add_kill_event(process_id, kill_time);
            continue;
        }

        char process_name[64];
        char io_start_text[16], io_duration_text[16];
        int process_id, cpu_burst;
        int io_start_time = -1, io_duration = 0;

        int parsed = sscanf(cursor, "%63s %d %d %15s %15s",
                            process_name, &process_id, &cpu_burst,
                            io_start_text, io_duration_text);

        if (parsed >= 3) {
            if (parsed == 5 && strcmp(io_start_text, "-") != 0 && strcmp(io_start_text, "--") != 0) {
                io_start_time = atoi(io_start_text);
                io_duration = atoi(io_duration_text);
            }

            ProcessControlBlock *process = calloc(1, sizeof(ProcessControlBlock));
            strncpy(process->process_name, process_name, sizeof(process->process_name) - 1);
            process->process_id = process_id;
            process->total_cpu_time = cpu_burst;
            process->io_start_time = io_start_time;
            process->io_total_duration = io_duration;
            process->state = READY;
            process->arrival_time = 0;
            process->completion_time = -1;
            process->killed_time = -1;

            store_process(process);
            enqueue(&ready_queue, process);
        }
    }

    if (process_count == 0) {
        printf("No processes provided.\n");
        return 0;
    }

    int current_time = 0;
    ProcessControlBlock *running_process = NULL;

    while (1) {
        apply_kill_events(current_time, &ready_queue, &waiting_queue, &running_process, &terminated_queue);
        update_io(&waiting_queue, &ready_queue);

        if (!running_process) {
            running_process = dequeue(&ready_queue);
            if (running_process) running_process->state = RUNNING;
        }

        if (running_process) {
            running_process->executed_cpu_time++;

            if (running_process->executed_cpu_time >= running_process->total_cpu_time) {
                running_process->state = TERMINATED;
                running_process->completion_time = current_time + 1;
                enqueue(&terminated_queue, running_process);
                running_process = NULL;
            } else if (running_process->io_start_time >= 0 &&
                       running_process->executed_cpu_time == running_process->io_start_time &&
                       running_process->io_total_duration > 0) {
                running_process->state = WAITING;
                running_process->io_remaining_time = running_process->io_total_duration;
                enqueue(&waiting_queue, running_process);
                running_process = NULL;
            }
        }

        current_time++;

        int all_done = 1;
        for (int index = 0; index < process_count; index++) {
            if (process_table[index]->state != TERMINATED &&
                process_table[index]->state != KILLED) {
                all_done = 0;
                break;
            }
        }
        if (all_done) break;
    }

    print_summary(&terminated_queue);

    for (int index = 0; index < process_count; index++)
        free(process_table[index]);
    free(process_table);

    while (kill_event_list) {
        KillEvent *temp = kill_event_list;
        kill_event_list = kill_event_list->next;
        free(temp);
    }

    return 0;
}
