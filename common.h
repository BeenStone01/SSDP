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

//����ת��:��һ�ֱ���תΪ��һ�ֱ���
int code_convert(char* from_charset, char* to_charset, char* inbuf, size_t inlen, char* outbuf, size_t outlen);

//UNICODE��תΪGB2312��
int u2g(char* inbuf, size_t inlen, char* outbuf, size_t outlen);

//GB2312��תΪUNICODE��
int g2u(char* inbuf, size_t inlen, char* outbuf, size_t outlen);

//�����ѿռ丱��
char* str_new(const char* str);

//�ַ�������
int str_split(char* src, const char* delim, char** rows);

//�ַ�����ȡ
char* str_cut(const char* src, int length_left, int length_right);

//�ַ���ƴ��
int str_splice_http(url_data_t* durl, char* bufSend, int size);

int in_array(char* str, char** array, int count);

//�ؼ��ּ���
int has_keys(char* str, const char** key_array, int array_size);

//������ַ��Ϣ
int is_location(char* str);

//����������Ϣ
int is_length(char* str);

//������ַ�±�
int seek_url_index(url_data_t* durl, url_data_t** array_url, int count);

//����url�����Ψһ���������飬������ָ�룬����������NULL
url_data_t* filter_url(char* url, url_data_t** array_url, int* count);

void retrive_unique_url(char** array);

//��ȡ����Ӧ���е����ĳ���
int retrive_content_length(char* bufRecv);

//��ȡ�豸��������
int retrive_xml_file(url_data_t* durl);

int client(int max_num);

//���1900�˿�ռ��
void killpid();

//��ȡ����IP��ַ
void get_localhost(char** array_url, int size);

//��ȡurl.txt�ļ�����������
int url_read_file(url_data_t** durl_array,int size_array);

void ssdp_discover();

//��ȡ�豸��Ϣ
void get_device_info();