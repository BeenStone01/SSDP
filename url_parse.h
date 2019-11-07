#pragma once
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

typedef struct url_data {
	char* href;
	char* protocol;
	char* host;
	char* port;
	char* path;
	char* query;
	char* fragment;
}url_data_t;

url_data_t* url_parse(char* url);
void free_url_data(url_data_t* durl);