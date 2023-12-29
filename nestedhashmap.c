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
    char *primstart;
    char *primcurrent;
    long primcapacity;
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
  uintptr_t key;
  uintptr_t value;
  int set;
  int nested;
  int id;
  long index;
};

struct Hashmap {
    int id;
    uintptr_t start;
    int size;
    struct Arena *arena;
    long offset;
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

void *alloc_prim(struct Arena *a, ptrdiff_t size, ptrdiff_t align, ptrdiff_t count)
{
    ptrdiff_t avail = a->end - a->primcurrent;
    ptrdiff_t padding = -(uintptr_t)a->primcurrent & (align - 1);
    printf("padding is %lx\n", padding);
    if (count > (avail - padding)/size) {
        abort();  // one possible out-of-memory policy
    }
    ptrdiff_t total = size * count;
    char *p = a->primcurrent + padding;
    a->primcurrent += padding + total;
    return memset(p, 0, total);
}

struct Arena *newarena(ptrdiff_t capacity, ptrdiff_t primstart)
{
    struct Arena *a = calloc(1, sizeof(struct Arena));
    a->beg = calloc(1, capacity);
    a->size = capacity;
    a->end = a->beg ? a->beg + capacity : 0;
    a->primstart = a->beg + primstart;
    a->primcurrent = a->primstart;
    a->primcapacity = primstart;
    return a;
}

struct Arena * clonearena(struct Arena arena) {
  struct Arena * copy = calloc(1, sizeof(struct Arena));
  copy->beg = malloc(arena.size); 
  memcpy(copy->beg, arena.beg, arena.size);
  copy->size = arena.size;
  copy->end = arena.end;
  copy->primstart = copy->beg + arena.primcapacity;
  copy->primcurrent = copy->beg + (arena.primcurrent - arena.beg);
  return copy;
}

int set_hashmap(struct Hashmap *hashmap, char *key, int key_size, uintptr_t value, int nested) {
    unsigned long hsh = hash(key) % hashmap->size;
    printf("hashing %s == %ld\n", key, hsh);
    long offset = (hashmap->id * hashmap->size * sizeof(struct hashmap_pair));
    printf("setting hashmap at %ld offset + %ld\n", offset, hsh);
    struct hashmap_pair *item = &((struct hashmap_pair*)(hashmap->arena->beg + offset))[hsh];
    item->key = ((uintptr_t) key) - (uintptr_t)hashmap->arena->primstart;
    if (nested == NESTED) {
      item->value = (uintptr_t) (((struct Hashmap*)value)->start - (uintptr_t)hashmap->arena->beg);
      item->id = ((struct Hashmap*)value)->id;
      printf("newvalue is %p %ld\n", (char*)item->value, item->value);
    } else {
      item->value = (uintptr_t) (value - (uintptr_t)hashmap->arena->primstart);
    }
    printf("key set to %lx\n", item->key);
    printf("value set to %lx\n", item->value);
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
    struct hashmap_pair *item = &((struct hashmap_pair*)(hashmap->arena->beg + (hashmap->id * hashmap->size * sizeof(struct hashmap_pair))))[hsh];
    if (item->nested == 1) {
      uintptr_t newvalue = (uintptr_t) hashmap->arena->beg + item->value; 
      printf("get newvalue is %p %lx\n", (char*)newvalue, newvalue);
      return newvalue;
    } else {
      return (uintptr_t) hashmap->arena->primstart + item->value; 
    }
    return item->value;
}
/*
struct hashmap_value get_hashmap_nested(struct hashmap *hashmaps, struct hashmap *hashmap, char key[], char subkey[]) {
    unsigned long hsh = hash(key) % MAX_SIZE;
    int nested = (*hashmap).value[hsh].nested;
    return get_hashmap(&hashmaps[nested], subkey);
}
*/

int print_hashmap_pair(struct Hashmap *hashmap, int depth, int size, struct hashmap_pair* item) {
    char spaces[depth + 1];
    for (int x = 0 ; x < depth ; x++) {
      spaces[x] = ' ';
    }
    spaces[depth] = 0;
    if (item->set == 1 && item->nested == NESTED) {
      printf("%s%s = %p (nested)\n", spaces, (char*) hashmap->arena->primstart + item->key, (char*)hashmap->arena->beg + item->value);  
      for (int x = 0 ; x < size; x++) {
        // printf("nested hashmap id %d\n", item->id);
        // printf("%p begin\n", hashmap->arena->beg);
        struct hashmap_pair *newitem = &(((struct hashmap_pair*) (hashmap->arena->beg + (item->id * size * sizeof(struct hashmap_pair)))))[x];
        
        // printf("x is %d offset is %ld index is %ld\n", x, (item->id * size * sizeof(struct hashmap_pair)), newitem->index);
        // printf("pointer x is %p\n", newitem);
        // printf("x is %d\n", x);
        print_hashmap_pair(hashmap, depth + 1, size, newitem);
      }

    } else if (item->set == 1) { 
      printf("index %ld is set\n", item->index);
      uintptr_t key_start = ((uintptr_t)hashmap->arena->primstart + item->key);
      uintptr_t value_start = ((uintptr_t)hashmap->arena->primstart + item->value);
      printf("key value %lx %lx\n", item->key, item->value);
      printf("addition %lx %lx\n", key_start, value_start);
      printf("%s%s (%lx) = %s (%lx)\n", spaces,
        (char*)key_start,
        key_start,
        (char*)value_start,  
        value_start);
    }
}
int print_hashmap(struct Hashmap* hashmap) {
  printf("beg is %p\n", (char*) hashmap->arena->beg);
  for (int x = 0 ; x < hashmap->size; x++) {
    struct hashmap_pair *item = &((struct hashmap_pair*) (hashmap->arena->beg + hashmap->id * hashmap->size * sizeof(struct hashmap_pair)))[x];
    print_hashmap_pair(hashmap, 0, hashmap->size, item);
  }
}

struct Hashmap ** clonehashmaps(struct Arena *backing, int hashmaps, struct Hashmap **maps, int size) {

  struct Arena *clarena = clonearena(*backing);

  struct Hashmap **cloned = calloc(hashmaps, sizeof(struct Hashmap*));
  int count = 0;
  for (int x = 0 ; x < hashmaps ; x++) {
    long offset = x * (size * sizeof(struct hashmap_pair));
    printf("%ld offset\n", offset);
    printf("%ld clone beg\n", (uintptr_t) clarena->beg);
    uintptr_t hashmap_start = (uintptr_t) clarena->beg + (uintptr_t) maps[x]->arena->beg - maps[x]->start;
    
    for (int n = 0 ; n < size ; n++) {
      // printf("%p\n", &((struct hashmap_pair*)hashmap_start)[n]);

    }
    
    printf("%d\n", x);
    struct Hashmap *hashmap = calloc(1, sizeof(struct Hashmap));
    hashmap->arena = clarena;
    hashmap->id = x;
    hashmap->start = offset;
    hashmap->offset = offset;
    printf("start %p\n", (char*) hashmap_start); 
    
    hashmap->size = size;
    cloned[count++] = hashmap;
  }
  return cloned;
}

int main() {
  int hashmaps = 5;
  int size = 1024;
  int count = 0;
  int primitive_store = 512000;
  struct Hashmap **maps = calloc(hashmaps, sizeof(struct Hashmap*));
  long memory = hashmaps * size * (sizeof(struct hashmap_pair)) + primitive_store;
  printf("%ld bytes\n", memory);
  int primitive_start = hashmaps * size * (sizeof(struct hashmap_pair));

  struct Arena *backing = newarena(memory, primitive_start);
  printf("beg is %ld\n", (uintptr_t) backing->beg);

  for (int x = 0 ; x < hashmaps ; x++) {
    long offset = x * (size * sizeof(struct hashmap_pair));
    printf("%ld offset\n", offset);
    uintptr_t hashmap_start = (uintptr_t) backing->beg + offset;
    for (int n = 0 ; n < size ; n++) {
      // printf("%p\n", &((struct hashmap_pair*)hashmap_start)[n]);

      ((struct hashmap_pair*)hashmap_start)[n].value = 0;
      ((struct hashmap_pair*)hashmap_start)[n].key = 0;
      ((struct hashmap_pair*)hashmap_start)[n].nested = 0;
      ((struct hashmap_pair*)hashmap_start)[n].set = 0;
      ((struct hashmap_pair*)hashmap_start)[n].index = n;
    }
    printf("creating hashmap %d\n", x);
    struct Hashmap *hashmap = calloc(1, sizeof(struct Hashmap));
    hashmap->arena = backing;
    hashmap->offset = offset;
    hashmap->id = x;
    hashmap->start = /*(uintptr_t) backing->beg -*/ hashmap_start;
    printf("%p\n", (char*) hashmap_start); 
    
    hashmap->size = size;
    maps[count++] = hashmap;
  }

  char * data = "world";
  char * key1 = "hello";
  char * key2 = "world";
  char * key3 = "oblong";
  char * key4 = "nested";
  char * key5 = "rectangle";

  char * _data = alloc_prim(backing, strlen(data) + 1, 64, 1);
  memcpy(_data, data, strlen(data));

  char * _key1 = alloc_prim(backing, strlen(key1) + 1, 64, 1);
  memcpy(_key1, key1, strlen(key1));

  char * _key2 = alloc_prim(backing, strlen(key2) + 1, 64, 1);
  memcpy(_key2, key2, strlen(key2));

  char * _key3 = alloc_prim(backing, strlen(key3) + 1, 64, 1);
  memcpy(_key3, key3, strlen(key3));

  char * _key4 = alloc_prim(backing, strlen(key4) + 1, 64, 1);
  memcpy(_key4, key4, strlen(key4));

  char * _key5 = alloc_prim(backing, strlen(key5) + 1, 64, 1);
  memcpy(_key5, key5, strlen(key5));

  printf("%s is %p\n", _data, _data);
  printf("%s is %p\n", _key1, _key1);
  printf("%s is %p\n", _key2, _key2);
  printf("%s is %p\n", _key3, _key3);
  printf("%s is %p\n", _key4, _key4);
  printf("%s is %p\n", _key5, _key5);

  set_hashmap(maps[0], _key1, strlen(_key1), (uintptr_t) _data, PRIMITIVE);
  set_hashmap(maps[0], _key2, strlen(_key2), (uintptr_t) maps[1], NESTED);

  set_hashmap(maps[1], _key2, strlen(_key2), (uintptr_t) _data, PRIMITIVE);
  set_hashmap(maps[1], _key3, strlen(_key3), (uintptr_t) maps[2], NESTED);

  // printf("get value %s\n", (char*)get_hashmap(maps[0], "hello"));
  // printf("get value %s\n", (char*)get_hashmap(maps[1], "world"));

  set_hashmap(maps[2], _key4, strlen(_key4), (uintptr_t) _key5, PRIMITIVE);


  printf("diff %ld\n", maps[1]->start - maps[0]->start);
  print_hashmap(maps[0]);
  print_hashmap(maps[1]);
  struct Hashmap **clonedmaps = clonehashmaps(backing, hashmaps, maps, size);
  print_hashmap(clonedmaps[0]);
}
