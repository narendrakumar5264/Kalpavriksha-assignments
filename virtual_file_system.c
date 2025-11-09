#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BLOCK_SIZE 512
#define NUM_BLOCKS 1024
#define MAX_NAME_LEN 50
#define MAX_BLOCKS_PER_FILE 100

typedef struct FreeBlock {
    int index;
    struct FreeBlock *next, *prev;
} FreeBlock;

typedef struct FileNode {
    char name[MAX_NAME_LEN];
    int isDirectory;
    int size;
    int blockPointers[MAX_BLOCKS_PER_FILE];
    int blockCount;
    struct FileNode *parent;
    struct FileNode *children;
    struct FileNode *next, *prev;
} FileNode;

char virtualDisk[NUM_BLOCKS][BLOCK_SIZE];
FreeBlock *freeListHead = NULL;
FileNode *root = NULL;
FileNode *cwd = NULL;

void initializeFreeBlocks() {
    FreeBlock *prev = NULL;
    for (int index = 0; index < NUM_BLOCKS; index++) {
        FreeBlock *newBlock = malloc(sizeof(FreeBlock));
        newBlock->index = index;
        newBlock->next = NULL;
        newBlock->prev = prev;
        if (prev){
             prev->next = newBlock;
     }
        else {
            freeListHead = newBlock;
        }
        prev = newBlock;
    }
}

void initializeFileSystem() {
    root = malloc(sizeof(FileNode));
    strcpy(root->name, "/");
    root->isDirectory = 1;
    root->children = NULL;
    root->parent = NULL;
    root->next = root->prev = root;
    cwd = root;
    initializeFreeBlocks();
    printf("Compact VFS - ready. Type 'exit' to quit.\n");
}

void insertIntoDirectory(FileNode **head, FileNode *newNode) {
    if (*head == NULL) {
        *head = newNode;
        newNode->next = newNode->prev = newNode;
    } else {
        FileNode *tail = (*head)->prev;
        tail->next = newNode;
        newNode->prev = tail;
        newNode->next = *head;
        (*head)->prev = newNode;
    }
}

FileNode *findNode(FileNode *dir, const char *name) {
    if (!dir || !dir->children) return NULL;
    FileNode *temp = dir->children;
    do {
        if (strcmp(temp->name, name) == 0)
            return temp;
        temp = temp->next;
    } while (temp != dir->children);
    return NULL;
}

void mkdirCommand(const char *dirname) {
    if (findNode(cwd, dirname)) {
        printf("Directory already exists.\n");
        return;
    }
    FileNode *newDir = malloc(sizeof(FileNode));
    strcpy(newDir->name, dirname);
    newDir->isDirectory = 1;
    newDir->children = NULL;
    newDir->parent = cwd;
    insertIntoDirectory(&cwd->children, newDir);
    printf("Directory '%s' created successfully.\n", dirname);
}

void lsCommand() {
    if (!cwd->children) {
        printf("(empty)\n");
        return;
    }
    FileNode *temp = cwd->children;
    do {
        printf("%s%s\n", temp->name, temp->isDirectory ? "/" : "");
        temp = temp->next;
    } while (temp != cwd->children);
}

void cdCommand(const char *dirname) {
    if (strcmp(dirname, "..") == 0) {
        if (cwd->parent) cwd = cwd->parent;
        printf("Moved to %s\n", cwd->name);
        return;
    }
    FileNode *target = findNode(cwd, dirname);
    if (!target || !target->isDirectory) {
        printf("Directory not found.\n");
        return;
    }
    cwd = target;
    printf("Moved to /%s\n", cwd->name);
}

void createCommand(const char *filename) {
    if (findNode(cwd, filename)) {
        printf("File already exists.\n");
        return;
    }
    FileNode *newFile = malloc(sizeof(FileNode));
    strcpy(newFile->name, filename);
    newFile->isDirectory = 0;
    newFile->size = 0;
    newFile->blockCount = 0;
    newFile->parent = cwd;
    insertIntoDirectory(&cwd->children, newFile);
    printf("File '%s' created successfully.\n", filename);
}

void writeCommand(const char *filename, const char *data) {
    FileNode *file = findNode(cwd, filename);
    if (!file || file->isDirectory) {
        printf("File not found.\n");
        return;
    }
    int dataSize = strlen(data);
    int blocksNeeded = (dataSize + BLOCK_SIZE - 1) / BLOCK_SIZE;
    if (!freeListHead || blocksNeeded > NUM_BLOCKS) {
        printf("Disk full.\n");
        return;
    }
    FreeBlock *temp = freeListHead;
    for (int i = 0; i < blocksNeeded; i++) {
        if (!temp) {
            printf("Not enough free blocks.\n");
            return;
        }
        file->blockPointers[i] = temp->index;
        memcpy(virtualDisk[temp->index], data + i * BLOCK_SIZE, BLOCK_SIZE);
        temp = temp->next;
        file->blockCount++;
    }
    freeListHead = temp;
    file->size = dataSize;
    printf("Data written successfully (size=%d bytes).\n", dataSize);
}

void readCommand(const char *filename) {
    FileNode *file = findNode(cwd, filename);
    if (!file || file->isDirectory) {
        printf("File not found.\n");
        return;
    }
    for (int i = 0; i < file->blockCount; i++)
        printf("%s", virtualDisk[file->blockPointers[i]]);
    printf("\n");
}

void deleteCommand(const char *filename) {
    FileNode *file = findNode(cwd, filename);
    if (!file || file->isDirectory) {
        printf("File not found.\n");
        return;
    }
    for (int i = 0; i < file->blockCount; i++) {
        FreeBlock *newBlock = malloc(sizeof(FreeBlock));
        newBlock->index = file->blockPointers[i];
        newBlock->next = NULL;
        if (!freeListHead) freeListHead = newBlock;
        else {
            FreeBlock *temp = freeListHead;
            while (temp->next) temp = temp->next;
            temp->next = newBlock;
            newBlock->prev = temp;
        }
    }
    if (file->next == file) cwd->children = NULL;
    else {
        file->prev->next = file->next;
        file->next->prev = file->prev;
        if (cwd->children == file) cwd->children = file->next;
    }
    free(file);
    printf("File deleted successfully.\n");
}

void rmdirCommand(const char *dirname) {
    FileNode *dir = findNode(cwd, dirname);
    if (!dir || !dir->isDirectory) {
        printf("Directory not found.\n");
        return;
    }
    if (dir->children) {
        printf("Directory not empty.\n");
        return;
    }
    if (dir->next == dir) cwd->children = NULL;
    else {
        dir->prev->next = dir->next;
        dir->next->prev = dir->prev;
        if (cwd->children == dir) cwd->children = dir->next;
    }
    free(dir);
    printf("Directory removed successfully.\n");
}

void pwdCommand() {
    char path[1024] = "";
    FileNode *temp = cwd;
    while (temp) {
        char buffer[1024];
        sprintf(buffer, "/%s%s", temp->name, path);
        strcpy(path, buffer);
        temp = temp->parent;
        if (temp && strcmp(temp->name, "/") == 0) break;
    }
    printf("%s\n", path);
}

void dfCommand() {
    int freeCount = 0;
    FreeBlock *temp = freeListHead;
    while (temp) {
        freeCount++;
        temp = temp->next;
    }
    printf("Total Blocks: %d\nUsed Blocks: %d\nFree Blocks: %d\n", NUM_BLOCKS, NUM_BLOCKS - freeCount, freeCount);
}

void exitCommand() {
    printf("Memory released. Exiting program...\n");
    exit(0);
}

int main() {
    initializeFileSystem();
    char command[1000];
    while (1) {
        printf("%s > ", cwd->name);
        fgets(command, sizeof(command), stdin);
        command[strcspn(command, "\n")] = 0;
        if (strncmp(command, "mkdir ", 6) == 0) mkdirCommand(command + 6);
        else if (strcmp(command, "ls") == 0) lsCommand();
        else if (strncmp(command, "cd ", 3) == 0) cdCommand(command + 3);
        else if (strncmp(command, "create ", 7) == 0) createCommand(command + 7);
        else if (strncmp(command, "write ", 6) == 0) {
            char *file = strtok(command + 6, " ");
            char *data = strtok(NULL, "\"");
            if (file && data) writeCommand(file, data);
            else printf("Usage: write <file> \"data\"\n");
        } else if (strncmp(command, "read ", 5) == 0) readCommand(command + 5);
        else if (strncmp(command, "delete ", 7) == 0) deleteCommand(command + 7);
        else if (strncmp(command, "rmdir ", 6) == 0) rmdirCommand(command + 6);
        else if (strcmp(command, "pwd") == 0) pwdCommand();
        else if (strcmp(command, "df") == 0) dfCommand();
        else if (strcmp(command, "exit") == 0) exitCommand();
        else if (strcmp(command, "") == 0) continue;
        else printf("Unknown command.\n");
    }
    return 0;
}
