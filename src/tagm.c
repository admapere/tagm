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
#include <id3v2lib.h>
#include <unistd.h>

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
			//@TODO sanitize input
			chdir(optarg);
			break;
		default:
			exit(EXIT_FAILURE);
		}
	}
	/*
	 * Scans directory entered in the path as an argument to -d 
	 * --directory, or --dir. Otherwise, scans current directory.
	 */
	int file_n;
	struct dirent **namelist;
	errno = 0;
	file_n = scandir("./", &namelist, filter, alphasort);
	if (errno){
		perror(""); //@TODO IMPROVE ERROR MESSAGES
		exit(EXIT_FAILURE);
	}
	/*
	 * Go through list of  mp3 files and print out the meta data
	 */
	enum mp3_frames {ART, ALB, TRK, TIT, CMNT};
	char *list[] = {"Artist:", "Album:", "Track:", "Title:"};
	char *filename = NULL;
	ID3v2_tag *tag = NULL;
	ID3v2_frame *frame_list[] = {NULL, NULL, NULL, NULL};
	size_t frame_list_len = sizeof(frame_list) / sizeof(*frame_list);

	while (file_n--){
		filename = namelist[file_n]->d_name;
		printf("%s\n", filename);
		
		tag = load_tag(filename);
		frame_list[ART] = tag_get_artist(tag);
		frame_list[ALB] = tag_get_album(tag);
		frame_list[TRK] = tag_get_track(tag);
		frame_list[TIT] = tag_get_title(tag);

		for (int i = 0; i < frame_list_len; ++i){
			ID3v2_frame_text_content *text = NULL;
			text = parse_text_frame_content(frame_list[i]);
			printf("\t%s %s\n", list[i], text->data);
			free(text);
			free(frame_list[i]);
		}
		free(namelist[file_n]);
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
