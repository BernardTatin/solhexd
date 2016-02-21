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
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>



#if !defined(SUCCESS)
#define SUCCESS 0
#endif

#if !defined(FAILURE)
#define FAILURE 1
#endif

#define HLEN	16
#define BLEN	(HLEN * 256)
#define LLEN	256

typedef struct {
	int64_t from;
	int64_t to;
	char *fileName;
} TSfileConfig;

static uint8_t buffer[BLEN];
static char line[LLEN];

static void dohelp(const int exitCode) {
	fprintf(stdout, "dohelp\n");
	exit(exitCode);
}

static TSfileConfig *duplicate(TSfileConfig *fileConfig) {
	TSfileConfig *fc = (TSfileConfig *)malloc(sizeof(TSfileConfig));

	if (fc == NULL) {
		fprintf(stdout, "ERROR: cannot allocate memory\n");
		exit(FAILURE);
	}
	fc->from = fileConfig->from;
	fc->to = fileConfig->to;
	fc->fileName = fileConfig->fileName;

	fileConfig->fileName = NULL;

	return fc;
}

static int hexdump(TSfileConfig *fc) {
	int fd = open(fc->fileName, O_RDONLY);
	fprintf(stdout, "File : %s\n", fc->fileName);
	fprintf(stdout, "  from : %08lx to : %08lx\n", fc->from, fc->to);
	if (fd != -1) {
		ssize_t addr = 0;
		ssize_t ptr_in = 0;
		ssize_t read_len;
		while ((read_len = read(fd, buffer, BLEN)) > 0) {
			int ptr_out = 0;
			char *dst = line;
			uint8_t *src = buffer;

			ptr_in += read_len;
			while (ptr_out < ptr_in) {
				int wbytes = sprintf(dst, "%08lx: ", addr);
				uint8_t *old_src = src;
				dst += wbytes;
				for (int i=0; i<HLEN; i++) {
					wbytes = sprintf(dst, "%02x ", *(src++));
					dst += wbytes;
				}
				src = old_src;
				*(dst++) = '\'';
				for (int i=0; i<HLEN; i++) {
					uint8_t b = *(src++);
					if (b < 32 || b > 127) {
						*(dst++) = '.';
					} else {
						*(dst++) = (char)b;
					}
				}
				*(dst++) = '\'';
				*dst = 0;
				dst = line;
				ptr_out += HLEN;
				addr += HLEN;
				fprintf(stdout, "%s\n", line);
			}
		}
	}

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
			i++;
			if (i == argn) {
				dohelp(FAILURE);
			}
			fileConfig.from = atol(argv[i]);
			fprintf(stdout, "	from = %ld\n", fileConfig.from);
		} else if (strcmp(arg, "--to") == 0) {
			i++;
			if (i == argn) {
				dohelp(FAILURE);
			}
			fileConfig.to = (int64_t)atoi(argv[i]);
			fprintf(stdout, "	to = %ld / %s\n", fileConfig.to, argv[i]);
		} else {
			fileConfig.fileName = arg;
			hexdump(duplicate(&fileConfig));
		}
	}
	return retCode;
}
