#include "url_parse.h"

url_data_t* url_parse(char* url)
{
	int i,
		len;
	const char* parseptr1;
	const char* parseptr2;

	if (NULL == url)
	{
		fprintf(stderr, "url_parse(): the param: url is invalid.\n");
		return NULL;
	}

	//准备存入解析数据的空间
	url_data_t* durl = (url_data_t*)malloc(sizeof(url_data_t));
	if (NULL == durl)
	{
		fprintf(stderr, "Failed to malloc for [url_data_t]");
		return NULL;
	}
	memset(durl, 0, sizeof(url_data_t));

	// 保存durl->herf
	len = strlen(url);
	durl->href = (char*)malloc(len + 1);
	if (NULL == durl->href)
	{
		fprintf(stderr, "Failed to malloc for [href].\n");
		goto FAILED;
	}
	memset(durl->href, 0, len + 1);
	strcpy_s(durl->href, len + 1, url);

	//解析保存durl->protocol
	parseptr2 = url;
	parseptr1 = strchr(parseptr2, ':');
	if (NULL == parseptr1) {
		fprintf(stderr, "URL错误01!\n");
		goto FAILED;
	}
	len = parseptr1 - parseptr2;
	durl->protocol = (char*)malloc(len + 1);
	if (NULL == durl->protocol)
	{
		fprintf(stderr, "Failed to malloc for [protocol].\n");
		goto FAILED;
	}
	memset(durl->protocol, 0, len + 1);
	strncpy_s(durl->protocol, len + 1, parseptr2, len);

	//解析判断
	for (i = 0; i < len; i++) {
		if (!isalpha(parseptr2[i])) {	//是否为英文字母
			fprintf(stderr, "URL错误02!\n");
			goto FAILED;
		}
	}
	parseptr1++;
	parseptr2 = parseptr1;
	for (i = 0; i < 2; i++) {
		if ('/' != *parseptr2) {
			fprintf(stderr, "URL错误03!\n");
			goto FAILED;
		}
		parseptr2++;
	}
	parseptr1 = strchr(parseptr2, ':');
	len = parseptr1 - parseptr2;

	if (NULL == parseptr1 || len > 15)//判断有无端口号
	{
		//解析保存durl->host
		parseptr1 = strchr(parseptr2, '/');
		if (NULL == parseptr1) {
			fprintf(stderr, "URL错误04!\n");
			goto FAILED;
		}
		len = parseptr1 - parseptr2;
		durl->host = (char*)malloc(len + 1);
		if (NULL == durl->host)
		{
			fprintf(stderr, "Failed to malloc for [host].\n");
			goto FAILED;
		}
		memset(durl->host, 0, len + 1);
		strncpy_s(durl->host, len + 1, parseptr2, len);
	}
	else
	{
		//解析保存durl->host
		len = parseptr1 - parseptr2;
		durl->host = (char*)malloc(len + 1);
		if (NULL == durl->host)
		{
			fprintf(stderr, "Failed to malloc for [host].\n");
			goto FAILED;
		}
		memset(durl->host, 0, len + 1);
		strncpy_s(durl->host, len + 1, parseptr2, len);

		//解析保存durl->port
		parseptr1++;
		parseptr2 = parseptr1;
		parseptr1 = strchr(parseptr2, '/');
		if (NULL == parseptr1) {
			fprintf(stderr, "URL错误05!\n");
			goto FAILED;
		}
		len = parseptr1 - parseptr2;
		durl->port = (char*)malloc(len + 1);
		if (NULL == durl->port)
		{
			fprintf(stderr, "Failed to malloc for [port].\n");
			goto FAILED;
		}
		memset(durl->port, 0, len + 1);
		strncpy_s(durl->port, len + 1, parseptr2, len);
	}

	//解析保存durl->path
	parseptr1++;
	parseptr2 = parseptr1;
	while ('\0' != *parseptr1 && '?' != *parseptr1 && '#' != *parseptr1) {
		parseptr1++;
	}
	len = parseptr1 - parseptr2;
	durl->path = (char*)malloc(len + 1);
	if (NULL == durl->path)
	{
		fprintf(stderr, "Failed to malloc for [path].\n");
		goto FAILED;
	}
	memset(durl->path, 0, len + 1);
	strncpy_s(durl->path, len + 1, parseptr2, len);

	//解析保存durl->query
	parseptr2 = parseptr1;
	if ('?' == *parseptr1) {
		parseptr2++;
		parseptr1 = parseptr2;
		while ('\0' != *parseptr1 && '#' != *parseptr1) {
			parseptr1++;
		}
		len = parseptr1 - parseptr2;
		durl->query = (char*)malloc(len + 1);
		if (NULL == durl->query)
		{
			fprintf(stderr, "Failed to malloc for [query].\n");
			goto FAILED;
		}
		memset(durl->query, 0, len + 1);
		strncpy_s(durl->query, len + 1, parseptr2, len);
	}

	//解析保存durl->fragment
	parseptr2 = parseptr1;
	if ('#' == *parseptr1) {
		parseptr2++;
		parseptr1 = parseptr2;
		while ('\0' != *parseptr1) {
			parseptr1++;
		}
		len = parseptr1 - parseptr2;
		durl->fragment = (char*)malloc(len + 1);
		if (NULL == durl->fragment)
		{
			fprintf(stderr, "Failed to malloc for [fragment].\n");
			goto FAILED;
		}
		memset(durl->fragment, 0, len + 1);
		strncpy_s(durl->fragment, len + 1, parseptr2, len);
	}
	return durl;

FAILED:
	free_url_data(durl);
	return NULL;
}

void free_url_data(url_data_t* data)
{
	if (NULL == data) return;
	if (NULL != data->href)	free(data->href);
	if (NULL != data->protocol)	free(data->protocol);
	if (NULL != data->host)	free(data->host);
	if (NULL != data->port)	free(data->port);
	if (NULL != data->path)	free(data->path);
	if (NULL != data->query) free(data->query);
	if (NULL != data->fragment) free(data->fragment);
	free(data);
	data = NULL;
}