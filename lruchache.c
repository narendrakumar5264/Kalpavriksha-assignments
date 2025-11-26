#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_STRING 100
#define HASH_SIZE 2003

typedef struct Node {
    int key;
    char value[MAX_STRING];
    struct Node *prev, *next;
} Node;

typedef struct HashEntry {
    int key;
    Node *node;
    struct HashEntry *next;
} HashEntry;

typedef struct {
    int capacity;
    int size;
    Node *head;
    Node *tail;
    HashEntry *hash[HASH_SIZE];
} LRUCache;

unsigned int hashFunction(int key) {
    return key % HASH_SIZE;
}

Node* createNode(int key, const char *value) {
    Node *node = (Node*)malloc(sizeof(Node));
    if (!node) return NULL;
    node->key = key;
    strcpy(node->value, value);
    node->prev = node->next = NULL;
    return node;
}

HashEntry* createHashEntry(int key, Node *nodePtr) {
    HashEntry *entry = (HashEntry*)malloc(sizeof(HashEntry));
    if (!entry) return NULL;
    entry->key = key;
    entry->node = nodePtr;
    entry->next = NULL;
    return entry;
}

void hashmapInsert(LRUCache *cache, int key, Node *nodePtr) {
    unsigned int index = hashFunction(key);
    HashEntry *entry = createHashEntry(key, nodePtr);
    if (!entry) return;
    entry->next = cache->hash[index];
    cache->hash[index] = entry;
}

Node* hashmapGet(LRUCache *cache, int key) {
    unsigned int index = hashFunction(key);
    HashEntry *current = cache->hash[index];
    while (current) {
        if (current->key == key)
            return current->node;
        current = current->next;
    }
    return NULL;
}

void hashmapDelete(LRUCache *cache, int key) {
    unsigned int index = hashFunction(key);
    HashEntry *current = cache->hash[index], *previous = NULL;
    while (current) {
        if (current->key == key) {
            if (previous) previous->next = current->next;
            else cache->hash[index] = current->next;
            free(current);
            return;
        }
        previous = current;
        current = current->next;
    }
}

void removeNode(LRUCache *cache, Node *node) {
    if (node->prev)
        node->prev->next = node->next;
    else
        cache->head = node->next;

    if (node->next)
        node->next->prev = node->prev;
    else
        cache->tail = node->prev;
}

void addToFront(LRUCache *cache, Node *node) {
    node->prev = NULL;
    node->next = cache->head;

    if (cache->head)
        cache->head->prev = node;

    cache->head = node;

    if (!cache->tail)
        cache->tail = node;
}

LRUCache* createCache(int capacity) {
    LRUCache *cache = (LRUCache*)malloc(sizeof(LRUCache));
    if (!cache) return NULL;
    cache->capacity = capacity;
    cache->size = 0;
    cache->head = cache->tail = NULL;
    for (int i = 0; i < HASH_SIZE; i++)
        cache->hash[i] = NULL;
    return cache;
}

char* get(LRUCache *cache, int key) {
    if (!cache) return NULL;
    Node *node = hashmapGet(cache, key);
    if (!node) return NULL;
    removeNode(cache, node);
    addToFront(cache, node);
    return node->value;
}

void put(LRUCache *cache, int key, const char *value) {
    if (!cache) return;

    Node *node = hashmapGet(cache, key);

    if (node) {
        strcpy(node->value, value);
        removeNode(cache, node);
        addToFront(cache, node);
        return;
    }

    if (cache->size == cache->capacity) {
        Node *lru = cache->tail;
        hashmapDelete(cache, lru->key);
        removeNode(cache, lru);
        free(lru);
        cache->size--;
    }

    Node *newNode = createNode(key, value);
    if (!newNode) return;

    addToFront(cache, newNode);
    hashmapInsert(cache, key, newNode);
    cache->size++;
}

int main() {
    LRUCache *cache = NULL;
    char command[20];

    while (1) {
        scanf("%s", command);

        if (strcmp(command, "createCache") == 0) {
            int cap;
            scanf("%d", &cap);
            cache = createCache(cap);
            if (!cache) return 1;
        }
        else if (strcmp(command, "put") == 0) {
            int key;
            char val[MAX_STRING];
            scanf("%d %s", &key, val);
            put(cache, key, val);
        }
        else if (strcmp(command, "get") == 0) {
            int key;
            scanf("%d", &key);
            char *result = get(cache, key);
            if (result) printf("%s\n", result);
            else printf("NULL\n");
        }
        else if (strcmp(command, "exit") == 0) {
            break;
        }
    }

    return 0;
}
