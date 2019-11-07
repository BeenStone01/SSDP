#pragma once
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <iconv.h>
#include <WinSock2.h>
#include <WS2tcpip.h>
#pragma comment(lib,"ws2_32.lib")
#include "url_parse.h"
#include "xml_device.h"
#define DEVICE_MAX_NUM 5

//代码转换:从一种编码转为另一种编码
int code_convert(char* from_charset, char* to_charset, char* inbuf, size_t inlen, char* outbuf, size_t outlen);

//UNICODE码转为GB2312码
int u2g(char* inbuf, size_t inlen, char* outbuf, size_t outlen);

//GB2312码转为UNICODE码
int g2u(char* inbuf, size_t inlen, char* outbuf, size_t outlen);

//创建堆空间副本
char* str_new(const char* str);

//字符串解析
int str_split(char* src, const char* delim, char** rows);

//字符串截取
char* str_cut(const char* src, int length_left, int length_right);

//字符串拼接
int str_splice_http(url_data_t* durl, char* bufSend, int size);

int in_array(char* str, char** array, int count);

//关键字检索
int has_keys(char* str, const char** key_array, int array_size);

//检索地址信息
int is_location(char* str);

//检索长度信息
int is_length(char* str);

//查找网址下标
int seek_url_index(url_data_t* durl, url_data_t** array_url, int count);

//过滤url，如果唯一，存入数组，返回其指针，否则丢弃返回NULL
url_data_t* filter_url(char* url, url_data_t** array_url, int* count);

void retrive_unique_url(char** array);

//获取网络应答中的正文长度
int retrive_content_length(char* bufRecv);

//获取设备服务描述
int retrive_xml_file(url_data_t* durl);

int client(int max_num);

//解除1900端口占用
void killpid();

//获取本地IP地址
void get_localhost(char** array_url, int size);

//读取url.txt文件，存入数组
int url_read_file(url_data_t** durl_array,int size_array);

void ssdp_discover();

//获取设备信息
void get_device_info();