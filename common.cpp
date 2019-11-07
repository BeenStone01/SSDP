#include "common.h"


//代码转换:从一种编码转为另一种编码
int code_convert(char* from_charset, char* to_charset, char* inbuf, size_t inlen, char* outbuf, size_t outlen)
{
	iconv_t cd;
	char** pin = &inbuf;
	char** pout = &outbuf;

	cd = iconv_open(to_charset, from_charset);
	if (cd == 0) return -1;
	memset(outbuf, 0, outlen);
	if (iconv(cd, (const char**)pin, &inlen, pout, &outlen) == -1) return -1;
	iconv_close(cd);
	return 0;
}

//UNICODE码转为GB2312码
int u2g(char* inbuf, size_t inlen, char* outbuf, size_t outlen)
{
	return code_convert((char*)"utf-8", (char*)"gb2312", inbuf, inlen, outbuf, outlen);
}

//GB2312码转为UNICODE码
int g2u(char* inbuf, size_t inlen, char* outbuf, size_t outlen)
{
	return code_convert((char*)"gb2312", (char*)"utf-8", inbuf, inlen, outbuf, outlen);
}

//申请堆空间创建副本
char* str_new(const char* str)
{
	int len = strlen(str);
	char* ptr = (char*)malloc(sizeof(char) * (len + 1));
	if (NULL == ptr)
	{
		fprintf(stderr, "Failed to malloc for new_str().\n");
		return NULL;
	}
	strcpy_s(ptr, len + 1, str);
	return ptr;
}

//字符串解析
int str_split(char* src, const char* delim, char** rows)
{
	assert(src || delim || rows);
	if (NULL == src || NULL == delim)
	{
		fprintf(stderr, "str_split() param is invalid.\n");
		return 0;
	}
	char* buf = { 0 };
	rows[0] = strtok_s(src, delim, &buf);
	int i = 0;
	while (rows[i++])
	{
		rows[i] = strtok_s(NULL, delim, &buf);
	}
	return i;
}

//字符串截取
char* str_cut(const char* src, int length_left, int length_right)
{
	int l = length_left,
		r = length_right,
		len;
	char* str;

	if (NULL == src)
	{
		fprintf(stderr, "The parament of fuction: strcur() is wrong.\n");
		return NULL;
	}
	len = strlen(src);
	if (len < l + r)
	{
		fprintf(stderr, "The parament of fuction: strcur() is wrong.\n");
		return NULL;
	}
	str = (char*)malloc(len - l - r + 1);
	if (NULL == str)
	{
		fprintf(stderr, "Failed to malloc for ptr @str_cut().\n");
		return NULL;
	}
	memset(str, 0, len - l - r + 1);
	memmove(str, src + l, len - l - r);
	return str;
}

int in_array(char* str, char** array, int count)
{
	int i = 0;
	if (NULL == str)	return 0;
	for (; i < count; i++)
	{
		if (0 == strcmp(str, array[i]))
			return i;
	}
	return -1;
}

//关键字检索
int has_keys(char* str, const char** key_array, int array_size)
{
	int i = 0;
	for (; i < array_size; i++)
	{
		if (0 == strncmp(str, key_array[i], strlen(key_array[i])))
		{
			return 1;
		}
	}
	return 0;
}

//检索地址信息
int is_location(char* str)
{
	if (NULL == str) return 0;
	const char* array_key[] = { "LOCATION:","Location:" };
	return has_keys(str, array_key, sizeof(array_key) / sizeof(array_key[0]));
}

//检索长度信息
int is_length(char* str)
{
	if (NULL == str)	return 0;
	const char* length_keys[] = { "Content-Length:","CONTENT-LENGTH:" };
	if (has_keys(str, length_keys, sizeof(length_keys) / sizeof(length_keys[0])))
		return 1;
	else
		return 0;
}

//http请求拼接
int str_splice_http(url_data_t* durl, char* bufSend, int size)
{
	char str_tmp[] = "\r\nAccept: text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8\r\n\
User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64) Chrome/77.0.3865.120 Safari/537.36\r\n\
Accept-Encoding: gzip,deflate\r\n\
Accept-Language: zh-CN,en-US;q=0.7,en;q=0.3\r\n\
\r\n";
	strcat_s(bufSend, 1024, "GET /");
	strcat_s(bufSend, 1024, durl->path);
	if (NULL != durl->query)
	{
		strcat_s(bufSend, 1024, "?");
		strcat_s(bufSend, 1024, durl->query);
	}
	if (NULL != durl->fragment)
	{
		strcat_s(bufSend, 1024, "#");
		strcat_s(bufSend, 1024, durl->fragment);
	}
	sprintf_s(bufSend, 1024, "%s%s%s%s%s%s", bufSend, " HTTP/1.1\r\nHOST: ", durl->host, ":", durl->port, str_tmp);
	return strlen(bufSend);
}

//查找网址下标
int seek_url_index(url_data_t* durl, url_data_t** array_url, int count)
{
	int j = 0,
		r1,
		r2;
	for (; j < count; j++)
	{
		r1 = strcmp(array_url[j]->host, durl->host);
		r2 = strcmp(array_url[j]->port, durl->port);
		if (0 == r1 * r2)
		{
			return j;
		}
	}
	return -1;
}

//过滤url，如果唯一，存入数组，返回其指针，否则丢弃返回NULL
url_data_t* filter_url(char* url, url_data_t** array_url, int* count)
{
	assert(url || array_url || count);
	int n = *count;
	if (NULL == url) return 0;

	url_data_t* durl = url_parse(url);	//解析url
	if (seek_url_index(durl, array_url, n) != -1)	//如果重复
	{
		free_url_data(durl);
		return NULL;
	}
	printf("========%d==%s\n", n + 1, durl->href);
	array_url[n++] = durl;
	*count = n;
	return durl;
}

//过滤设备描述地址，对比现存地址消除重复，存放堆空间，地址存入指针数组
void retrive_unique_url(char** array)
{
	int i = 0;
	int len = 0;
	char buf[200] = { 0 };
	FILE* fpr,
		*fpw;
	if (fopen_s(&fpr, "./data.txt", "r"))
	{
		fprintf(stderr, "Failed to open data.txt.\n");
		return;
	}
	;
	if (fopen_s(&fpw, "./url.txt", "w+"))
	{
		fprintf(stderr, "Failed to open service_describe_url.txt.\n");
		return;
	}

	fseek(fpr, 0, SEEK_SET);
	while (fgets(buf, 200, fpr) && (!feof(fpr)))
	{
		if (i == DEVICE_MAX_NUM)	break;
		if (is_location(buf))		//查找地址信息
		{
			char* url = strstr(buf, "http");	//返回url
			if (0 > in_array(url, array, i))	//如果为检索到
			{
				if (NULL == url)
					return;
				len = strlen(url);
				array[i] = (char*)malloc(sizeof(char) * (len + 1));	//申请空间
				if (NULL == array[i])
				{
					fprintf(stderr, "Failed to malloc for array.\n");
					return;
				}
				strcpy_s(array[i], len + 1, url);
				fwrite(url, len, 1, fpw);
				i++;
			}

		}
	}
	fclose(fpw);
	fclose(fpr);
	fpr = fpw = NULL;
	return;
}

//获取设备服务描述
int retrive_xml_file(url_data_t* durl)
{
	assert(durl);
	int sendLen,
		recvLen,
		nPort,
		len_content,
		times = 0;
	char bufSend[1024] = { 0 };
	char bufRecv[10240] = { 0 };
	FILE* fpw = { 0 };

	if (NULL == durl->host || NULL == durl->port)
	{
		fprintf(stderr, "the url is invaild.\n");
		return 0;
	}

	//初始化DLL
	WSADATA wsaData;
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
	{
		fprintf(stderr, "WSAStartup() error!.\n");
		return 0;
	}

	//创建套接字
	SOCKET clientSock = socket(AF_INET, SOCK_STREAM, 0);
	if (INVALID_SOCKET == clientSock)
	{
		fprintf(stderr, "socket() error\n");
		goto FAILED;
	}

	//向服务器发送请求
	SOCKADDR_IN clientAddr;
	memset(&clientAddr, 0, sizeof(SOCKADDR_IN));
	clientAddr.sin_family = AF_INET;
	inet_pton(AF_INET, durl->host, &clientAddr.sin_addr);
	nPort = atoi(durl->port);
	clientAddr.sin_port = htons(nPort);
	connect(clientSock, (SOCKADDR*)&clientAddr, sizeof(SOCKADDR));

	fopen_s(&fpw, "device.xml", "w+");
	if (NULL == fpw)
	{
		fprintf(stderr, "Failed to create the xml file.\n");
		goto FAILED;
	}

	str_splice_http(durl, bufSend, 1024);
	sendLen = send(clientSock, bufSend, strlen(bufSend), 0);
	if (-1 == sendLen)
	{
		fprintf(stderr, "%s\nFailed to send the http request.\n%s", durl->host, bufSend);
		goto FAILED;
	}

	//接收服务器传回数据
	while (recvLen = recv(clientSock, bufRecv, 10240, 0))
	{
		times++;
		if (-1 == recvLen)
		{
			fprintf(stderr, "retirve_xml_file(): Failed to Recive……\n");
			goto FAILED;
		}
		/*
		*/
		if (1 == times)
		{
			len_content = retrive_content_length(bufRecv);
			//printf("**************** len_content = %d *************\n",len_content);
		}
		if (2 == times)
		{
			fwrite(bufRecv, recvLen, 1, fpw);
			if (len_content == recvLen)
			{
				//printf("----------------recvLen = %d----------------\n", recvLen);
				break;
			}
		}
		if (3 == times)
		{
			fwrite(bufRecv, recvLen, 1, fpw);
			//printf("----------------recvLen = %d----------------\n", recvLen);
			break;
		}
		//printf("----------------recvLen = %d----------------\n", recvLen);
	}

	fclose(fpw);
	fpw = NULL;
	closesocket(clientSock);
	WSACleanup();
	return 1;

FAILED:
	if (NULL != fpw)	fclose(fpw);
	closesocket(clientSock);
	WSACleanup();
	return 0;
}

//获取网络应答中的正文长度
int retrive_content_length(char* bufRecv)
{
	int i,
		size,
		len;
	char* rows[30] = { 0 };
	char delim[3] = "\r\n";

	if (NULL == bufRecv)	return -1;
	size = str_split(bufRecv, delim, rows);	//分解字符串
	for (i = 0; i < size; i++)
	{
		if (NULL != rows[i])
		{
			if (is_length(rows[i]))		//检索长度信息
			{
				char* itr = str_cut(rows[i], 16, 0);
				len = atoi(itr);
				free(itr);
				return len;
			}
		}
	}
	return -1;
}

//获取服务描述地址
int client(int max_num)
{
	const int opt = 1;
	int i = 0;
	WSADATA wsaData;
	if (0 != WSAStartup(MAKEWORD(2, 2), &wsaData))
	{
		fprintf(stderr, "WSAStartup() error!.\n");
		return -1;
	}

	SOCKET sock = socket(AF_INET, SOCK_DGRAM, 0);
	if (INVALID_SOCKET == sock)
	{
		printf("socket() error\n");
		return -1;
	}

	if (setsockopt(sock, SOL_SOCKET, SO_BROADCAST, (char*)&opt, sizeof(opt)) == -1)
	{
		printf("set socket error\n");
		return -1;
	}
	SOCKADDR_IN servAddr;
	memset(&servAddr, 0, sizeof(SOCKADDR_IN));
	servAddr.sin_family = AF_INET;
	inet_pton(AF_INET, "239.255.255.250", &servAddr.sin_addr);
	servAddr.sin_port = htons(1900);

	SOCKADDR fromAddr;
	int addrLen = sizeof(fromAddr);
	char bufRecv[1024] = { 0 };
	char bufSend[] = "M-SEARCH * HTTP/1.1\r\n\
HOST: 239.255.255.250:1900\r\n\
MAN: \"ssdp:discover\"\r\n\
ST: urn:schemas-upnp-org:device:MediaServer:1\r\n\
MX: 5\r\n\
\r\n";

	int sendLen = sendto(sock, bufSend, strlen(bufSend), 0, (SOCKADDR*)&servAddr, sizeof(servAddr));
	printf("sendLen = %d\n", sendLen);

	FILE* fpw;
	fopen_s(&fpw, "./data.txt", "w+");
	if (NULL == fpw)
	{
		fprintf(stderr, "Failed to create data.txt.\n");
		return -1;
	}
	while (i < max_num)
	{
		i++;
		int recvLen = recvfrom(sock, bufRecv, 1024, 0, &fromAddr, &addrLen);
		if (recvLen < 1024)
			bufRecv[recvLen] = 0;
		fwrite(bufRecv, recvLen, 1, fpw);
		printf("======recvLen = %d\n", recvLen);
		printf("%s\n", bufRecv);
	}
	fclose(fpw);
	fpw = NULL;
	closesocket(sock);
	WSACleanup();
	return 0;
}

//解除1900端口占用
void killpid()
{
	int i = 0,
		size,
		len;
	char* rows[5] = { 0 };
	char* array_pid[5] = { 0 };
	char buf[100] = { 0 };
	const char* delim = " ";
	FILE* fpr;
	system("netstat -ano|findstr 1900 > pid.txt");
	fopen_s(&fpr, "./pid.txt", "r");
	if (NULL == fpr)
	{
		fprintf(stderr, "Failed to open pid.txt.\n");
		return;
	}
	while (fgets(buf, 100, fpr) && (!feof(fpr)))
	{
		//分解row
		len = strlen(buf);
		buf[len - 1] = 0;
		size = str_split(buf, delim, rows);

		if (in_array(rows[size - 2], array_pid, i) < 0)
		{
			array_pid[i++] = str_new(rows[size - 2]);
		}
	}
	i--;
	for (; i >= 0; i--)
	{
		sprintf_s(buf, 100, "taskkill -PID %s -F", array_pid[i]);
		system(buf);
		free(array_pid[i]);
	}
	fclose(fpr);
	return;
}

//获取本地IP地址
void get_localhost(char** array_url, int size)
{
	WSADATA WSAData;
	if (WSAStartup(MAKEWORD(2, 2), &WSAData) != 0)
	{
		printf("WSAStartup() error!\n");
		return;
	}

	int i = 0;
	struct addrinfo hints;
	struct addrinfo* res, *cur;
	struct sockaddr_in* addr;

	memset(&hints, 0, sizeof(struct addrinfo));
	hints.ai_family = AF_INET;     /* Allow IPv4 */
	hints.ai_flags = AI_PASSIVE;/* For wildcard IP address */
	hints.ai_protocol = 0;         /* Any protocol */
	hints.ai_socktype = SOCK_DGRAM;

	if (getaddrinfo("", NULL, &hints, &res) == -1) {
		fprintf(stderr, "get_host_ip(): Failed to getaddrinfo().\n");
		return;
	}

	for (cur = res; cur != NULL && i < size; cur = cur->ai_next) {
		addr = (struct sockaddr_in*)cur->ai_addr;
		array_url[i] = (char*)malloc(sizeof(char) * 16);
		if (NULL == array_url[i])
		{
			fprintf(stderr, "get_host_ip() : Failed to malloc for array_url.\n");
			return;
		}
		memset(array_url[i], 0, 16);
		sprintf_s(array_url[i++], 16, "%d.%d.%d.%d",
			(*addr).sin_addr.S_un.S_un_b.s_b1,
			(*addr).sin_addr.S_un.S_un_b.s_b2,
			(*addr).sin_addr.S_un.S_un_b.s_b3,
			(*addr).sin_addr.S_un.S_un_b.s_b4);
	}
	freeaddrinfo(res);
	WSACleanup();
}

//读取url.txt文件，存入数组
int url_read_file(url_data_t** durl_array, int size_array)
{
	int len,
		url_count = 0;
	FILE* fpr;
	char buffer[200] = { 0 };
	url_data_t* durl;

	fopen_s(&fpr, "./url.txt", "r");
	if (NULL == fpr)
	{
		fprintf(stderr, "url.txt con't be opened.\n");
	}

	while (fgets(buffer, 200, fpr) && !feof(fpr) && url_count != size_array)
	{
		printf("%s", buffer);
		len = strlen(buffer);
		buffer[len - 1] = 0;
		durl = url_parse(buffer);
		if (NULL == durl)
		{
			fprintf(stderr, "Failed to parse the url: \n ---%s", buffer);
			break;
		}
		durl_array[url_count++] = durl;
	}
	fclose(fpr);
	return url_count;
}

void ssdp_discover()
{
	int ret,
		fromLen,
		recvLen,
		count_row,
		count_url;
	char* row,
		*url;
	char* local_host = { 0 };
	char* rows[20] = { 0 };
	char delim[3] = "\r\n";
	char bufRecv[1024] = { 0 };
	FILE* fpw1,
		*fpw2;
	url_data_t* durl_array[DEVICE_MAX_NUM] = { 0 };
	struct ip_mreq multiCast;

	WSADATA wsaData;
	if (WSAStartup(MAKEWORD(2, 2), &wsaData))
		fprintf(stderr, "%d: WSAStartup() error!.\n", WSAGetLastError());

	killpid();
	count_url = url_read_file(durl_array, DEVICE_MAX_NUM);
	//创建文件保存记录
	fopen_s(&fpw1, "./data.txt", "w+");
	if (NULL == fpw1)
	{
		fprintf(stderr, "Failed to create data.txt.\n");
		return;
	}
	fopen_s(&fpw2, "./url.txt", "w+");
	if (NULL == fpw2)
	{
		fprintf(stderr, "Failed to create data.txt.\n");
		return;
	}

	//创建套接字，设置参数
	SOCKET newSock = socket(AF_INET, SOCK_DGRAM, 0);
	if (INVALID_SOCKET == newSock)
	{
		fprintf(stderr, "%d: error! socket is not created.\n", WSAGetLastError());
		return;
	}
	/*
	int optVal = 1;
	ret = setsockopt(newSock, SOL_SOCKET, SO_REUSEADDR, (char*)&optVal, sizeof(optVal));
	if (0 != ret)
	{
		fprintf(stderr, "%d setsockopt error1.\n", WSAGetLastError());
		return;
	}
	int optval = 1;
	ret = setsockopt(newSock, IPPROTO_IP, IP_TTL, &optval, sizeof(optval));
	if (0 != ret)
	{
		fprintf(stderr, "%d: setsockopt error2.\n", WSAGetLastError());
		return;
	}
	*/

	//绑定本地端口
	SOCKADDR_IN servAddr;
	memset(&servAddr, 0, sizeof(servAddr));
	servAddr.sin_family = AF_INET;
	inet_pton(AF_INET, INADDR_ANY, &servAddr.sin_addr);
	servAddr.sin_port = htons(1900);
	ret = bind(newSock, (SOCKADDR*)&servAddr, sizeof(SOCKADDR));
	if (0 != ret)
	{
		fprintf(stderr, "%d: bind() error.\n", WSAGetLastError());
		goto FAILED;
	}
	/*
	SOCKADDR_IN peerAddr;
	memset(&peerAddr, 0, sizeof(peerAddr));
	peerAddr.sin_family = AF_INET;
	inet_pton(AF_INET, "239.255.255.250", &peerAddr.sin_addr);
	peerAddr.sin_port = htons(1900);
	*/

	//加入组播
	get_localhost(&local_host, 1);
	//printf("%s\n", local_host);
	inet_pton(AF_INET, "192.168.1.88", &multiCast.imr_interface.S_un.S_addr);
	inet_pton(AF_INET, "239.255.255.250", &multiCast.imr_multiaddr.S_un.S_addr);
	ret = setsockopt(newSock, IPPROTO_IP, IP_ADD_MEMBERSHIP, (char*)&multiCast, sizeof(multiCast));
	if (0 != ret)
	{
		fprintf(stderr, "%d: setSockopt() error3. \n", WSAGetLastError());
		goto FAILED;
	}

	//接收数据
	SOCKADDR fromAddr;
	fromLen = sizeof(SOCKADDR);
	while (1)
	{
		memset(bufRecv, 0, sizeof(bufRecv));
		recvLen = recvfrom(newSock, bufRecv, 1024, 0, &fromAddr, &fromLen);
		if (recvLen <= 0)
		{
			fprintf(stderr, "%d: recvfrom() error.\n", WSAGetLastError());
			goto FAILED;
		}
		fwrite(bufRecv, recvLen, 1, fpw1);
		printf("%s\n", bufRecv);
		count_row = str_split(bufRecv, delim, rows);
		while (count_row--)
		{
			row = rows[count_row];
			if (is_location(row))
			{
				if (NULL == row) break;
				url = strstr(row, "http");
				url_data_t* durl = filter_url(url, durl_array, &count_url);
				if (NULL != durl)
				{
					fprintf(fpw2, "%s\n", url);
				}
			}
		}
	}

	//结束后释放资源
	for (; count_url >= 0; count_url--)
	{
		if (NULL != durl_array[count_url])
			free_url_data(durl_array[count_url]);
	}
	closesocket(newSock);
	WSACleanup();
	fclose(fpw1);
	fclose(fpw2);
	if (NULL != local_host) free(local_host);
	return;

FAILED:
	closesocket(newSock);
	WSACleanup();
	if (NULL != fpw1)
	{
		fclose(fpw1);
		fpw1 = NULL;
	}
	if (NULL != fpw2)
	{
		fclose(fpw2);
		fpw2 = NULL;
	}
	if (NULL != local_host) free(local_host);
	return;
}

//获取设备信息
void get_device_info()
{
	int i = 0,
		n = 0,
		count = 0;
	FILE* fpw;
	url_data_t* durl;
	url_data_t* durl_array[DEVICE_MAX_NUM] = { 0 };
	Device* pDevice;
	Device* device_array[DEVICE_MAX_NUM] = { 0 };

	//获取设备信息
	count = url_read_file(durl_array, DEVICE_MAX_NUM);
	for(;i<count;i++)
	{
		durl = durl_array[i];
		pDevice = (Device*)malloc(sizeof(Device));
		if (NULL == pDevice)
		{
			fprintf(stderr, "Failed to malloc for device* .\n");
			break;
		}
		sprintf_s(pDevice->host, INET_ADDRSTRLEN, "%s%s%s", durl->host, ":", durl->port);
		if (retrive_xml_file(durl))				//获取单个xml文件
		{
			if (parse_xml(pDevice))				//解析xml,获取设备信息
				device_array[n++] = pDevice;	//存入指针数组
			else
				break;
			Sleep(100);
		}
	}

	fopen_s(&fpw, "./device.txt", "w");
	if (NULL == fpw)
		fprintf(stderr, "Failed to create the device.txt.\n");

	//打印设备信息
	for (int i = 0; i < n; i++)
	{
		printf("\n=================device %d=================\n", i + 1);
		fprintf(fpw, "\n=================device %d=================\n", i + 1);
		printf("\n%s\n", device_array[i]->name);
		fprintf(fpw, "\n%s\n", device_array[i]->name);
		printf("%s\n", device_array[i]->host);
		fprintf(fpw, "%s\n", device_array[i]->host);
		printf("%s\n", device_array[i]->deviceType);
		fprintf(fpw, "%s\n", device_array[i]->deviceType);
		printf("%s\n", device_array[i]->uuid);
		fprintf(fpw, "%s\n", device_array[i]->uuid);
		free(device_array[i]);
	}
	fclose(fpw);
}
