/*
 * file_test.c
 *
 *  Created on: 26 Apr 2020
 *      Author: Simon Barth
 */
#define __USE_GNU

#include <stdlib.h>
#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <sys/stat.h>
#include <time.h>

typedef struct {
	 char filepath[256];
	 time_t modtime;
} filedata_t;

typedef struct {
	filedata_t* files;
	size_t used;
	size_t size;
} files_t;

void initarr(files_t *a, size_t initialSize) {
  a->files = (filedata_t *)malloc(initialSize * sizeof(filedata_t));
  a->used = 0;
  a->size = initialSize;
}

void insertarr(files_t *a, filedata_t element) {
  if (a->used == a->size) {
    a->size *= 2;
    a->files = (filedata_t *)realloc(a->files, a->size * sizeof(filedata_t));
  }
  a->files[a->used++] = element;
}

void freearr(files_t *a) {
  free(a->files);
  a->files = NULL;
  a->used = 0;
  a->size = 0;
}

int main(int argc, char *args[]) {
	if (argc < 1) {
		printf("No path provided\n");
		return 0;
	}

	DIR *dir;
	struct dirent *ent;
	struct stat filestats;

	filedata_t tmp;
	files_t files;

	initarr(&files, 2);

	if ((dir = opendir(args[1])) != NULL) {
		while ((ent = readdir(dir)) != NULL) {
			if (strcmp(ent->d_name, ".") != 0 && strcmp(ent->d_name, "..") != 0) {
				strcpy(tmp.filepath, args[1]);
				strcat(tmp.filepath, ent->d_name);
				stat(tmp.filepath, &filestats);
				tmp.modtime = filestats.st_ctime;
				insertarr(&files, tmp);
			}
		}
		closedir(dir);
		for (int i = 0; i < files.used; ++i) {
			printf("%s\t-%s\n", files.files[i].filepath, ctime(&files.files[i].modtime));
		}
		freearr(&files);
	} else {
		/* could not open directory */
		perror("");
		return EXIT_FAILURE;
	}
}
