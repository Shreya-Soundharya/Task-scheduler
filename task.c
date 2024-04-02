#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Task {
    int id;
    char *description;
    int task_type;
    int deadline;
    int is_completed;
} Task;

typedef struct Node {
    Task *task;
    struct Node *next;
} Node;

typedef struct Queue {
    Node *front;
    Node *rear;
} Queue;

void insert(Queue *queue, Task *task) {
    Node *new_node = (Node *)malloc(sizeof(Node));
    new_node->task = task;
    new_node->next = NULL;

    // If the queue is empty or the new task has an earlier deadline than the front task
    if (queue->front == NULL || task->deadline < queue->front->task->deadline) {
        new_node->next = queue->front;
        queue->front = new_node;

        // If the queue was initially empty, update the rear as well
        if (queue->rear == NULL) {
            queue->rear = new_node;
        }

        return;
    }

    // Traverse the queue to find the correct position based on the deadline
    Node *current = queue->front;
    Node *prev = NULL;
    while (current != NULL && current->task->deadline <= task->deadline) {
        prev = current;
        current = current->next;
    }

    // Insert the new task in the middle or at the end of the queue
    prev->next = new_node;
    new_node->next = current;

    // If the new task is inserted at the end, update the rear
    if (current == NULL) {
        queue->rear = new_node;
    }
}


void delete_queue(Queue *queue) {
    Node *current = queue->front;
    Node *next;

    while (current != NULL) {
        next = current->next;
        free(current->task->description);
        free(current->task);
        free(current);
        current = next;
    }

    queue->front = queue->rear = NULL;
}

void print_task(Task *task) {
    char *task_type_str;
    switch (task->task_type) {
        case 1: task_type_str = "Exam"; break;
        case 2: task_type_str = "Project"; break;
        case 3: task_type_str = "Assignment"; break;
        case 4: task_type_str = "Miscellaneous"; break;
        default: task_type_str = "Unknown"; break;
    }

    printf("%-15s| %-20s| %d days\n", task_type_str, task->description, task->deadline);
}

void print_reverse(Node *current) {
    if (current == NULL) {
        return;
    }
    print_reverse(current->next);
    print_task(current->task);
}
int main() {
    printf("Task scheduler using Priority Queue\n");
    Queue queue;
    queue.front = queue.rear = NULL;

    while (1) {
        int option;
        printf("\n1. Add Task\n");
        printf("2. Delete Task\n");
        printf("3. Update Task\n");
        printf("4. View Tasks\n");
        printf("5. Exit\n");
        printf("\nEnter option: ");
        scanf("%d", &option);
        while (getchar() != '\n');
        switch (option) {
            case 1: {
    Task *task = (Task *)malloc(sizeof(Task));
    
    // Assigning task ID
    task->id = (queue.rear != NULL) ? queue.rear->task->id + 1 : 1;

    // Input for task type
    printf("Enter task type (1 for Exam, 2 for Project, 3 for Assignment, 4 for Miscellaneous): ");
    scanf("%d", &task->task_type);

    // Consume the newline character left in the buffer
    while (getchar() != '\n');

    // Input for task description
    task->description = (char *)malloc(100 * sizeof(char));
    printf("Enter task description: ");
    fgets(task->description, 100, stdin);
    task->description[strcspn(task->description, "\n")] = '\0';

    // Input for deadline
    printf("Enter deadline (in days): ");
    scanf("%d", &task->deadline);

    // Initializing the completion status
    task->is_completed = 0;

    // Inserting the task into the queue
    insert(&queue, task);
    break;
}

            case 2: {
    if (queue.front == NULL) {
        printf("Queue is empty. No task to delete.\n");
    } else {
        int id;
        printf("Enter task ID: ");
        scanf("%d", &id);
        Node *current = queue.front;
        Node *prev = NULL;
        int task_found = 0;

        while (current != NULL) {
            if (current->task->id == id) {
                if (prev != NULL) {
                    prev->next = current->next;
                } else {
                    queue.front = current->next;
                }

                if (current->next == NULL) {
                    queue.rear = prev;
                }

                printf("Task \"%s\" is deleted.\n", current->task->description);

                free(current->task->description);
                free(current->task);
                free(current);
                task_found = 1;
                break;
            }
            prev = current;
            current = current->next;
        }

        if (!task_found) {
            printf("Task not found.\n");
        }
    }
    break;
}


case 3: {
    if (queue.front == NULL) {
        printf("Queue is empty. No task to update.\n");
    } else {
        int id;
        printf("Enter task ID: ");
        scanf("%d", &id);
        int task_found = 0;
        Node *current = queue.front;

        while (current != NULL) {
            if (current->task->id == id) {
                printf("Task found.\n");
                printf("Enter new task description: ");
                fgets(current->task->description, 100, stdin);
                current->task->description[strlen(current->task->description) - 1] = '\0'; // Remove newline character
                printf("Enter new task type (1 for Exam, 2 for Project, 3 for Assignment, 4 for Miscellaneous): ");
                scanf("%d", &current->task->task_type);
                printf("Enter new deadline (in days): ");
                scanf("%d", &current->task->deadline);
                task_found = 1;
                break;
            }
            current = current->next;
        }

        if (!task_found) {
            printf("Task not found.\n");
        }
    }
    break;
}

            case 4: {
                    printf("--------------------------------------------------------\n");
    printf("%-15s| %-20s| %-10s\n", "Task", "Description", "Deadline");
    printf("--------------------------------------------------------\n");
     Node *current = queue.front;
                while (current != NULL) {
                    print_task(current->task);
                    current = current->next;
                }
    printf("--------------------------------------------------------\n");
    break;
}
            case 5: {
                delete_queue(&queue);
                exit(0);
            }
            default: {
                printf("Invalid option\n");
            }
        }
    }
    return 0;
}