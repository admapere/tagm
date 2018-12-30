/* MIT License
 *
 * Copyright (c) 2018 Adrian M. Perez
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>
#include <dirent.h>

#define PROGRAM_NAME "tagm"
#define VERSION_NUMBER "0.1"

int main(int argc, char *argv[])
{
	static struct option long_options[] = {
		{"version", no_argument, NULL, 'v'},
		{"help", no_argument, NULL, 'h'},
		{"path", required_argument, NULL, 'p'}
	};
	/*
	 * Command line argument parsing loop.@TODO
	 */
	int opt, loptind;
	char *pathstr = NULL;
	while ((opt = getopt_long(argc, argv, "vhp:", 
					long_options, &loptind)) > 0){
		switch (opt){
		case 'v':
			printf("%s %s\n", PROGRAM_NAME, VERSION_NUMBER);
			printf("%s\n", "<COPYRIGHT MESSAGE>");//@TODO
			exit(EXIT_FAILURE);
		case 'h':
			printf("%s\n", "Usage goes here");//@TODO
			exit(EXIT_FAILURE);
		case 'p':
			pathstr = optarg;
			break;
		default:
			exit(EXIT_FAILURE);
		}
	}
	/*
	 * Testing scandir  @TODO
	 */
	#define CURRENT_DIR "./"
	int n;
	struct dirent **namelist;
	n = scandir(pathstr != NULL ? pathstr : CURRENT_DIR, &namelist,
			NULL, alphasort);
	while (n--){
		printf("%s\n", namelist[n]->d_name);
		free(namelist[n]);
	}
	free(namelist);
	
	return EXIT_SUCCESS;
}
