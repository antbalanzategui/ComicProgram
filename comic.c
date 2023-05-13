#include "comic.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>


/*
* Allows for list to be created
*/
void initializeList(struct Comic_List* list, int size) {
    list->list = malloc(size * sizeof(struct Comic));
    list->size = size;
    list->count = 0;
}
/*
* Intializes a comic, rather uses malloc with string length of inputs, to allocate necessary space to those attributes only
*/
void initializeComic(struct Comic* comic, char* date, char* code, char* publisher, char* title, char* cost) {
    comic->date = malloc(strlen(date) + 1);
    strcpy(comic->date, date);
    comic->code = malloc(strlen(code) + 1);
    strcpy(comic->code, code);
    comic->publisher = malloc(strlen(publisher) + 1);
    strcpy(comic->publisher, publisher);
    comic->title = malloc(strlen(title) + 1);
    strcpy(comic->title, title);
    comic->cost = malloc(strlen(cost) + 1);
    strcpy(comic->cost, cost);
    
}
/*
* Basic resize functionality, only is called when checkCapacity is true
* this is a helper function to addComic
*/
struct Comic* resizeComicList(struct Comic_List* list, int size) {
    int newSize = size * 2;
    struct Comic* newList = (struct Comic*) malloc (newSize * sizeof(struct Comic));
    if (newList == NULL) {
        return NULL;
    }
    for (int i = 0; i < list -> count; i++) {
        newList[i] =  list -> list[i];
    }
    free(list -> list);
    list -> list = newList;
    list -> size = newSize;
    return newList;
}
/*
* Checks to see whether there is enough space to add an item, 
* is a helper function to addComic, is responsible for calling resizeComicList
*/
bool checkCapacity(struct Comic_List* list) {
    if (list -> count >= list -> size) {
        struct Comic* newList = resizeComicList(list, list -> size);
        if (newList == NULL) {
            return false;
        }
    }
    return true;
}
/*
* Basic add functionality
*/
bool addComic(struct Comic_List* list, struct Comic* comic) {
    if (!checkCapacity(list)) {
        return false;
    }
    list -> list[list -> count] = *comic;
    list -> count = list -> count + 1;
    return true;
}
/*
* Basic iterative search
*/
struct Comic* find(struct Comic_List list, int index) {
    if (index < 0 || index >= list.count) {
        return NULL;
    }
    return &(list.list[index]);
}
/*
* Basic remove functionality (IMPORTANT also frees comic!!!)
*/
bool removeComic(struct Comic_List* list, int index) {
    if (index < 0 || index >= list->count) {
        return false;
    }

    struct Comic* comicToRemove = &(list->list[index]);
    free(comicToRemove -> date);
    free(comicToRemove -> code);
    free(comicToRemove -> publisher);
    free(comicToRemove -> title);
    free(comicToRemove -> cost);

    for (int i = index; i < list->count - 1; i++) {
        list -> list[i] = list -> list[i+1];
    }

    list -> count = list -> count - 1;
    return true;
}
/*
* Basic clear functionality, used in conjunction with free command can be seen in Main.c
*/
void clear(struct Comic_List* list) {
    list -> count = 0;
}
/*
* Basic file reader, however one thing to note is the counter, which allows the main.c
* to know how many Comics were actually made, in order to match output file
*/
void load(struct Comic_List* list, char filename[], int* counter) {
    // open the file for reading
    FILE* in = fopen(filename, "r");
    if (!in) {
        printf("Error: could not open file %s\n", filename);
        return;
    }
    
    // read the header line and discard it
    char header[256];
    if (fgets(header, 256, in) == NULL) {
        printf("Error: could not read header from file %s\n", filename);
        fclose(in);
        return;
    }
    // Loop to read each line, 
    char line[256];
    while (fgets(line, 256, in) != NULL) {
        // Parses line into different data for char array
        char date[11], code[256], publisher[256], title[256], price[256];
        if (sscanf(line, "%10[^,],%255[^,],%255[^,],%255[^,],%255[^\n]", date, code, publisher, title, price) != 5) {
            printf("Error: could not parse data from line \"%s\"\n", line);
            printf("%s\n", filename);
            continue;
        }
        
        // Creates a new comic
        struct Comic comic;
        initializeComic(&comic, date, code, publisher, title, price);
        // add the comic to the list
        addComic(list, &comic);
        // Updates counter
        *counter = *counter + 1;
    }
    
    // close the file
    fclose(in);
}
/*
* Buy functionality, essentially works with using find as a helper function to determine whether or
* not the comic is NULL, if it is then it will return false, otherwise comic is simply added to the buyList
*/
bool buy(struct Comic_List list, struct Comic_List* buyList, int index) {
    struct Comic* comicAtIndex = find(list, index);
    if (comicAtIndex == NULL) {
        return false;
    }
    struct Comic comicCopy;
    initializeComic(&comicCopy, comicAtIndex -> date, comicAtIndex -> code, comicAtIndex -> publisher, comicAtIndex -> title, comicAtIndex -> cost);
    addComic(buyList, &comicCopy);
    return true;
}
/*
* Ignores "AR", if the cost of another value is not "AR" it will use
* atof to get the value of that price then add it to the total, 
* it is then calculated below. ALONG with this is very important
* to freeComicList prior to clear
*/
void checkout(struct Comic_List* buyList, FILE *out) {
    float preTaxTotal = 0;
    for (int i = 0; i < buyList -> count; i++) {
        if (strcmp(buyList -> list[i].cost, "AR") != 0) {
            float value = atof(buyList -> list[i].cost + 1);
            preTaxTotal = preTaxTotal + value;
        }
    }
    float tax = preTaxTotal * 0.05;
    float postTaxTotal = preTaxTotal + tax;
    fprintf(out, "Comics in Purchase List\n");
    display(buyList, out);
    fprintf(out, " Subtotal:  $%.2f\n", preTaxTotal);
    fprintf(out, "      Tax:  $%.2f\n", tax);
    fprintf(out, "    Total:  $%.2f\n", postTaxTotal);
    freeComicList(buyList);
    clear(buyList);

}
/*
* Simply fprintf(), iteratively through entire list
*/
void display(struct Comic_List* list, FILE *out) {
    if (list -> count == 0) {
        fprintf(out, "List is currently empty.\n");
    }
    for (int i = 0; i < list -> count; i++) {
        struct Comic comic = list -> list[i];
        fprintf(out, "Comic Number: %d\n", i+1);
        fprintf(out, "Date: %s\n", comic.date);
        fprintf(out, "Code: %s\n", comic.code);
        fprintf(out, "Publisher: %s\n", comic.publisher);
        fprintf(out, "Title: %s\n", comic.title);
        fprintf(out, "Cost: %s\n", comic.cost);
    }
}
/*
* Different from display, as it includes a header, and all the parameters are in strings,
* which are on the same line, allowing for one line fprintf
*/
void save(struct Comic_List* list, FILE* out) {
    fprintf(out, "DATE,CODE,PUBLISHER,TITLE,PRICE\n");
    for (int i = 0; i < list -> count; i++) {
        struct Comic comic = list -> list[i];
        fprintf(out, "%s,%s,%s,%s,%s\n", comic.date, comic.code, comic.publisher, comic.title, comic.cost);
    }
}
/*
* Free's everything except for the list itself, which is 
* done at end of main, more or less frees ALL comics within list
*/
void freeComicList(struct Comic_List* list) {
    for (int i = 0; i < list->count; i++) {
        free(list -> list[i].date);
        free(list -> list[i].code);
        free(list -> list[i].publisher);
        free(list -> list[i].title);
        free(list -> list[i].cost);
    }
}


