#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "comic.h"

//gcc -o comics -Wall -Wextra -Werror -std=c11 -g main.c comic.c
//./comics commands.txt myoutput.txt
//valgrind --leak-check=full --show-leak-kinds=all -s ./comics commands.txt myoutput.txt


int main(int argc, char **argv) {
    if (argc != 3) {
        fprintf(stderr, "USAGE: %s <input file> <output file>\n", argv[0]);
        exit(1);
    }

    FILE *in = fopen(argv[1], "r");
    FILE *out = fopen(argv[2], "w");

    if (in == NULL) {
        fprintf(stderr, "ERROR: Could not open input file %s\n", argv[1]);
        exit(1);
    }

    if (out == NULL) {
        fprintf(stderr, "ERROR: Could not open output file %s\n", argv[2]);
        exit(1);
    }

    struct Comic_List list;
    initializeList(&list, 10);
    struct Comic_List buyList;
    struct Comic_List foundList;
    initializeList(&buyList, 10);
    initializeList(&foundList, 10);

    char command[100] = {'\0'};
    int numberParameter; 
    char nameOfCSV[100];

    int counter = 0;

    fscanf( in, "%s",command );
	while( !feof(in) )
	{
		fprintf( out, "Command: %s ", command );
		if ( strcmp( command, "load" ) == 0 )
{
	fscanf(in, "%s", nameOfCSV );
	fprintf( out, "%s\n", nameOfCSV );
	load(&list, nameOfCSV, &counter);
	fprintf(out, "Number of comics: %d\n", counter);
    counter = 0;
}
else if ( strcmp( command, "find" ) == 0 )
{
	fscanf( in, "%d", &numberParameter);
	fprintf( out, "%d\n", numberParameter);
	struct Comic* comic = find(list, numberParameter);
	if (comic == NULL) {
		fprintf(out, "\tThere is no comic at index #%d in the list.\n", numberParameter);
    }
	else {
        fprintf(out, "Date: %s\n", comic -> date);
        fprintf(out, "Code: %s\n", comic -> code);
        fprintf(out, "Publisher: %s\n", comic -> publisher);
        fprintf(out, "Title: %s\n", comic -> title);
        fprintf(out, "Cost: %s\n", comic -> cost);
    }
}
else if ( strcmp( command, "save" ) == 0 )
{
	fscanf(in, "%s", nameOfCSV );
	fprintf( out, "%s", nameOfCSV );
    FILE* saveOut;
    saveOut = fopen(nameOfCSV, "w");
    save(&list, saveOut);
    fputs( "\n", out );
    fclose(saveOut);
}
else if ( strcmp( command, "remove" ) == 0 )
{
	fscanf( in, "%d", &numberParameter );
	fprintf( out, "%d\n", numberParameter );
	bool removed = removeComic(&list, numberParameter );
	if (removed)
		fprintf( out, "\tComic at index %d successfully removed\n", numberParameter);
	else
		fprintf( out, "\tComic at index %d was not removed\n", numberParameter);
}
else if ( strcmp( command, "display" ) == 0 )
{
	fputs( "\n", out );
	display(&list, out);
}
else if ( strcmp( command, "clear" ) == 0 )
{
	fputs( "\n", out );
    freeComicList(&list);
	clear(&list);
}
else if(strcmp(command, "buy") == 0) {
    fscanf(in, "%d", &numberParameter);
    fprintf(out, "%d\n", numberParameter);
    bool bought = buy(list, &buyList, numberParameter);
    if (bought) {
        fprintf(out, "\tComic #%d added to purchase list\n", numberParameter);
    }
    else {
        fprintf(out, "\tUnable to buy comic #%d\n", numberParameter);
    }
}
else if(strcmp(command, "checkout") == 0) {
    fputs("\n", out);
    checkout(&buyList, out);
}
fscanf( in, "%s",command );

	}
    freeComicList(&list);
    freeComicList(&buyList);
    freeComicList(&foundList);

    free(list.list);
    free(buyList.list);
    free(foundList.list);


    fclose(in);
    fclose(out);

    return 0;
}
