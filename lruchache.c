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
    Node *n = (Node*)malloc(sizeof(Node));
    n->key = key;
    strcpy(n->value, value);
    n->prev = n->next = NULL;
    return n;
}

HashEntry* createHashEntry(int key, Node *node) {
    HashEntry *h = (HashEntry*)malloc(sizeof(HashEntry));
    h->key = key;
    h->node = node;
    h->next = NULL;
    return h;
}

void hashmapInsert(LRUCache *cache, int key, Node *node) {
    unsigned int h = hashFunction(key);
    HashEntry *entry = createHashEntry(key, node);
    entry->next = cache->hash[h];
    cache->hash[h] = entry;
}

Node* hashmapGet(LRUCache *cache, int key) {
    unsigned int h = hashFunction(key);
    HashEntry *cur = cache->hash[h];
    while (cur) {
        if (cur->key == key)
            return cur->node;
        cur = cur->next;
    }
    return NULL;
}

void hashmapDelete(LRUCache *cache, int key) {
    unsigned int h = hashFunction(key);
    HashEntry *cur = cache->hash[h], *prev = NULL;
    while (cur) {
        if (cur->key == key) {
            if (prev) prev->next = cur->next;
            else cache->hash[h] = cur->next;
            free(cur);
            return;
        }
        prev = cur;
        cur = cur->next;
    }
}

void removeNode(LRUCache *cache, Node *node) {
    if (node->prev){
        node->prev->next = node->next;
    }
    else{
        cache->head = node->next;
    }

    if (node->next){
        node->next->prev = node->prev;
    }
    else{
        cache->tail = node->prev;
    }
}

void addToFront(LRUCache *cache, Node *node) {
    node->prev = NULL;
    node->next = cache->head;

    if (cache->head){
        cache->head->prev = node;
    }
    cache->head = node;

    if (!cache->tail){
        cache->tail = node;
    }
}

LRUCache* createCache(int capacity) {
    LRUCache *cache = (LRUCache*)malloc(sizeof(LRUCache));
    cache->capacity = capacity;
    cache->size = 0;
    cache->head = cache->tail = NULL;
    for (int index = 0; index < HASH_SIZE; index++){
        cache->hash[index] = NULL;
    }
    return cache;
}

char* get(LRUCache *cache, int key) {
    Node *node = hashmapGet(cache, key);
    if (!node) return NULL;
    removeNode(cache, node);
    addToFront(cache, node);
    return node->value;
}

void put(LRUCache *cache, int key, const char *value) {
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
            char *res = get(cache, key);
            if (res) printf("%s\n", res);
            else printf("NULL\n");
        }
        else if (strcmp(command, "exit") == 0) {
            break;
        }
    }
    return 0;
}
