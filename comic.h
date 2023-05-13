#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

struct Comic
{
    char* date;
    char* code;
    char* publisher;
    char* title;
    char *cost;
};

struct Comic_List
{
    struct Comic* list;
    int size;
    int count;
};

void initializeList(struct Comic_List* list, int size);

void initializeComic(struct Comic* comic, char* date, char* code, char* publisher, char* title, char* cost);

struct Comic* resizeComicList(struct Comic_List* list, int size);

bool checkCapacity(struct Comic_List* list);

bool addComic(struct Comic_List* list, struct Comic* comic);

struct Comic* find(struct Comic_List list, int index);

bool removeComic(struct Comic_List* list, int index);

void clear(struct Comic_List* list);

void load(struct Comic_List* list, char filename[], int* counter);

bool buy(struct Comic_List list, struct Comic_List* buyList, int index);

void checkout(struct Comic_List* buyList, FILE *out);

void display(struct Comic_List* list, FILE *out);

void save(struct Comic_List* list, FILE* out); 

void freeComicList(struct Comic_List* list);
