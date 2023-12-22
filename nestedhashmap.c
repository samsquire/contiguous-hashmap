#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>

#define PRIMITIVE 0
#define NESTED 1

struct Arena {
    char *beg;
    char *end;
    ptrdiff_t size;
};

struct hashmap_key {
    char key[1024];
    int len;
};
struct hashmap_value {
    char value[1024];
    int nested;
};

struct hashmap_pair {
  char * key;
  uintptr_t value;
  int set;
  int nested;
};

struct Hashmap {
    int id;
    uintptr_t start;
    int size;
};

unsigned long
hash(unsigned char *str)
{
    unsigned long hash = 5381;
    int c;

    while (c = *str++)
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */

    return hash;
}

void *alloc(struct Arena *a, ptrdiff_t size, ptrdiff_t align, ptrdiff_t count)
{
    ptrdiff_t avail = a->end - a->beg;
    ptrdiff_t padding = -(uintptr_t)a->beg & (align - 1);
    if (count > (avail - padding)/size) {
        abort();  // one possible out-of-memory policy
    }
    ptrdiff_t total = size * count;
    char *p = a->beg + padding;
    a->beg += padding + total;
    return memset(p, 0, total);
}

struct Arena newarena(ptrdiff_t capacity)
{
    struct Arena a = {0};
    a.beg = malloc(capacity);
    a.size = capacity;
    a.end = a.beg ? a.beg + capacity : 0;
    return a;
}

int set_hashmap(struct Hashmap *hashmap, char *key, int key_size, uintptr_t value, int nested) {
    unsigned long hsh = hash(key) % hashmap->size;
    struct hashmap_pair *item = &((struct hashmap_pair*)hashmap->start)[hsh];
    item->key = key;
    item->value = value;
    item->set = 1;
    item->nested = nested;
}

/*
int set_hashmap_nested(struct hashmap *hashmap, char key[], struct hashmap *nested) {
    unsigned long hsh = hash(key) % MAX_SIZE;
    memcpy(&(*hashmap).key[hsh], key, MAX_SIZE); 
    hashmap->value[hsh].nested = nested->id;
}
*/
uintptr_t get_hashmap(struct Hashmap *hashmap, char *key) {
    unsigned long hsh = hash(key) % hashmap->size;
    struct hashmap_pair *item = &((struct hashmap_pair*)hashmap->start)[hsh];
    return item->value;
}
/*
struct hashmap_value get_hashmap_nested(struct hashmap *hashmaps, struct hashmap *hashmap, char key[], char subkey[]) {
    unsigned long hsh = hash(key) % MAX_SIZE;
    int nested = (*hashmap).value[hsh].nested;
    return get_hashmap(&hashmaps[nested], subkey);
}
*/

int main() {
  int hashmaps = 5;
  int size = 1024;
  int count = 0;
  struct Hashmap **maps = calloc(hashmaps, sizeof(struct Hashmap*));
  struct Arena backing = newarena(hashmaps * size * (sizeof(struct hashmap_pair)));
  for (int x = 0 ; x < hashmaps ; x++) {
    uintptr_t hashmap_start = (uintptr_t) backing.beg + ((x * hashmaps) * sizeof(struct hashmap_pair));
    for (int n = 0 ; n < size ; n++) {
      // printf("%p\n", &((struct hashmap_pair*)hashmap_start)[n]);
      ((struct hashmap_pair*)hashmap_start)[n].value = 6;
    }
    struct Hashmap *hashmap = calloc(1, sizeof(struct Hashmap));
    hashmap->id = x;
    hashmap->start = hashmap_start;
    printf("%p\n", (char*) hashmap_start); 
    
    hashmap->size = size;
    maps[count++] = hashmap;
  }

  char * data = "hello";
  char * key1 = "hello";
  char * key2 = "world";
  set_hashmap(maps[0], key1, strlen(key1), (uintptr_t) data, PRIMITIVE);
  set_hashmap(maps[0], key2, strlen(key2), maps[1]->start, NESTED);
  // printf("%s\n", (char*)get_hashmap(maps[0], "hello"));

  for (int x = 0 ; x < maps[0]->size; x++) {
    struct hashmap_pair *item = &(((struct hashmap_pair*)maps[0]->start))[x];
    if (item->set == 1 && item->nested) {
      printf("%s = %p (nested)\n", item->key, (char*)item->value);  
    } else if (item->set == 1) {    
      printf("%s = %s\n", item->key, (char*)item->value);  
    }
  }
}
