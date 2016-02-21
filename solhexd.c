/*
 * File:   solhexd.c
 * Author: Bernard TATIN <bernard dot tatin at outlook dot org>
 *
 * Created on 20 février 2016, 23:20
 */

/*
    The MIT License (MIT)

    Copyright (c) 2016 Bernard TATIN <bernard dot tatin at outlook dot org>

    Permission is hereby granted, free of charge, to any person obtaining a copy
    of this software and associated documentation files (the "Software"), to deal
    in the Software without restriction, including without limitation the rights
    to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
    copies of the Software, and to permit persons to whom the Software is
    furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included in all
    copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
    SOFTWARE.

 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#if !defined(SUCCESS)
#define SUCCESS 0
#endif

#if !defined(FAILURE)
#define FAILURE 1
#endif

typedef struct {
	int64_t from;
	int64_t to;
	char *fileName;
} TSfileConfig;

static void dohelp(const int exitCode) {
	fprintf(stdout, "dohelp\n");
	exit(exitCode);
}

static TSfileConfig *duplicate(TSfileConfig *fileConfig) {
	TSfileConfig *fc = (TSfileConfig *)malloc(sizeof(TSfileConfig));

	fc->from = fileConfig->from;
	fc->to = fileConfig->to;
	fc->fileName = fileConfig->fileName;

	fileConfig->from = 0l;
	fileConfig->to = 0l;
	fileConfig->fileName = NULL;

	return fc;
}

static int hexdump(TSfileConfig *fc) {
	fprintf(stdout, "File : %s\n", fc->fileName);
	fprintf(stdout, "  from : %08lx to : %08lx\n", fc->from, fc->to);

	free(fc);
	return SUCCESS;
}

int main(int argn, char *argv[]) {
	int retCode = SUCCESS;
	TSfileConfig fileConfig = {
		0l,
		0l,
		NULL
	};

	for (int i=1; i<argn; i++) {
		char *arg = argv[i];

		fprintf(stdout, "arg -> %s\n", arg);

		if (strcmp(arg, "--help") == 0) {
			dohelp(SUCCESS);
		} else if (strcmp(arg, "--from") == 0) {
			fileConfig.from = atol(arg);
		} else if (strcmp(arg, "--to") == 0) {
			fileConfig.to = atol(arg);
		} else {
			fileConfig.fileName = arg;
			fprintf(stdout, "	call hexdump %s\n", arg);
			hexdump(duplicate(&fileConfig));
		}
	}
	return retCode;
}
