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
#include <errno.h>
#include <getopt.h>
#include <dirent.h>
#include <regex.h>

#define PROGRAM_NAME "tagm"
#define VERSION_NUMBER "0.2"

static int filter(const struct dirent *dir);

int main(int argc, char *argv[])
{
	static struct option long_options[] = {
		{"version", no_argument, NULL, 'v'},
		{"help", no_argument, NULL, 'h'},
		{"dir", required_argument, NULL, 'd'},
		{"directory", required_argument, NULL, 'd'}
	};
	int opt, loptind;
	char *path = NULL;
	const char *optstr = "vhd:";
	while ((opt = getopt_long(argc, argv, optstr, long_options, &loptind)) > 0){
		switch (opt){
		case 'v':
			printf("%s %s\n", PROGRAM_NAME, VERSION_NUMBER);
			printf("%s\n", "<COPYRIGHT MESSAGE>");//@TODO write copyright message
			exit(EXIT_SUCCESS);
		case 'h':
			printf("%s\n", "Usage goes here");//@TODO create help menu
			exit(EXIT_SUCCESS);
		case 'd':
			path = malloc(strlen(optarg) * sizeof(*path) + 1);
			strcpy(path, optarg);
			break;
		default:
			exit(EXIT_FAILURE);
		}
	}
	/*
	 * Scans directory entered in the path as an argument to -d 
	 * --directory, or --dir. Otherwise, scans current directory.
	 */
	#define CURRENT_DIR "./"
	int n;
	struct dirent **namelist;
	errno = 0;
	n = scandir(path != NULL ? path : CURRENT_DIR, &namelist,
			*filter, alphasort);
	if (errno){
		perror(path); //@TODO IMPROVE ERROR MESSAGES
		exit(EXIT_FAILURE);
	}
	//@TODO implement actual functionality
	while (n--){
		printf("%s\n", namelist[n]->d_name);
		free(namelist[n]);
	}
	free(namelist);
	
	return EXIT_SUCCESS;
}
/* 
 * Parses name of directory to ensure that it is only *.mp3 files that are
 * passed unto the next section of the program.
 */
static int filter(const struct dirent *dir)
{
	const char *fname = dir->d_name;
	const char *regex = "(.*).mp3";//@TODO may match too many characters
	regex_t preg;

	regcomp(&preg, regex, REG_EXTENDED | REG_NOSUB);
	if (REG_NOMATCH == regexec(&preg, fname, 0, NULL, 0)){
		return 0;
	}else{
		return 1;
	}
}
