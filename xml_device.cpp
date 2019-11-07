#pragma once
#include "xml_device.h"

static int parse_root_device_iconList_icon(xmlDocPtr doc, xmlNodePtr cur)
{
	assert(doc || cur);
	xmlChar* key;

	cur = cur->children;
	while (cur != NULL)
	{
		if (!xmlStrcmp(cur->name, BAD_CAST "mimetype"))
		{
			key = xmlNodeListGetString(doc, cur->children, 1);
			printf("------mimetype: %s\n", key);
			xmlFree(key);
		}
		if (!xmlStrcmp(cur->name, BAD_CAST "width"))
		{
			key = xmlNodeListGetString(doc, cur->children, 1);
			printf("------width: %s\n", key);
			xmlFree(key);
		}
		if (!xmlStrcmp(cur->name, BAD_CAST "height"))
		{
			key = xmlNodeListGetString(doc, cur->children, 1);
			printf("------height: %s\n", key);
			xmlFree(key);
		}
		if (!xmlStrcmp(cur->name, BAD_CAST "url"))
		{
			key = xmlNodeListGetString(doc, cur->children, 1);
			printf("------url: %s\n", key);
			xmlFree(key);
		}
		cur = cur->next;
	}
	return 0;
}

static int parse_root_device_serviceList_service(xmlDocPtr doc, xmlNodePtr cur)
{
	assert(doc || cur);
	xmlChar* key;

	cur = cur->children;
	while (cur != NULL)
	{
		/*获取serviceType*/
		if (!xmlStrcmp(cur->name, BAD_CAST "serviceType"))
		{
			key = xmlNodeListGetString(doc, cur->children, 1);
			printf("------serviceType: %s\n", key);
			xmlFree(key);
		}
		if (!xmlStrcmp(cur->name, BAD_CAST "serviceId"))
		{
			key = xmlNodeListGetString(doc, cur->children, 1);
			printf("------serviceId: %s\n", key);
			xmlFree(key);
		}
		if (!xmlStrcmp(cur->name, BAD_CAST "controlURL"))
		{
			key = xmlNodeListGetString(doc, cur->children, 1);
			printf("------controlURL: %s\n", key);
			xmlFree(key);
		}
		if (!xmlStrcmp(cur->name, BAD_CAST "eventSubURL"))
		{
			key = xmlNodeListGetString(doc, cur->children, 1);
			printf("------eventSubURL: %s\n", key);
			xmlFree(key);
		}
		if (!xmlStrcmp(cur->name, BAD_CAST "SCDPURL"))
		{
			key = xmlNodeListGetString(doc, cur->children, 1);
			printf("------SCDPURL: %s\n", key);
			xmlFree(key);
		}
		cur = cur->next;
	}
	return 0;
}

static int parse_root_device(xmlDocPtr doc, xmlNodePtr cur, Device* dtr)
{
	assert(doc || cur);
	xmlChar* key;
	char inbuf[50];
	char outbuf[50];

	cur = cur->children;
	while (cur != NULL)
	{

		if (!xmlStrcmp(cur->name, BAD_CAST "friendlyName"))
		{
			key = xmlNodeListGetString(doc, cur->children, 1);
			sprintf_s(inbuf, 50, "%s", key);
			u2g(inbuf, 50, outbuf, 50);
			//printf("FriendlyName: %s\n", outbuf);
			strcpy_s(dtr->name, 50, outbuf);
			xmlFree(key);
		}
		if (!xmlStrcmp(cur->name, BAD_CAST "UDN"))
		{
			key = xmlNodeListGetString(doc, cur->children, 1);
			//printf("UDN: %s\n", key);
			strcpy_s(dtr->uuid, 80, (char*)key);
			xmlFree(key);
		}
		if (!xmlStrcmp(cur->name, BAD_CAST "deviceType"))
		{
			key = xmlNodeListGetString(doc, cur->children, 1);
			//printf("deviceType: %s\n", key);
			strcpy_s(dtr->deviceType, 80, (char*)key);
			xmlFree(key);
		}

		/*
		if (!xmlStrcmp(cur->name, BAD_CAST "iconList"))
		{
			xmlNodePtr iconList = cur->children;
			int id = 1;
			while (iconList != NULL)
			{
				if (!xmlStrcmp(iconList->name, BAD_CAST "icon"))
				{
					printf("%s_%d\n", iconList->name, id++);
					parse_root_device_iconList_icon(doc, iconList);
				}
				iconList = iconList->next;
			}
		}
		if (!xmlStrcmp(cur->name, BAD_CAST "serviceList"))
		{
			//serviceList
			xmlNodePtr serviceList = cur->children;
			int id = 1;
			while (serviceList != NULL)
			{
				if (!xmlStrcmp(serviceList->name, BAD_CAST "service"))
				{
					printf("%s_%d\n", serviceList->name, id++);
					parse_root_device_serviceList_service(doc, serviceList);
				}
				serviceList = serviceList->next;
			}
		}
		*/
		cur = cur->next;
	}
	return 0;
}

int parse_xml(Device* dtr)
{
	xmlDocPtr doc;	//xml整个文档的树形结构
	xmlNodePtr cur;	//xml节点

	//获取树形结构
	doc = xmlParseFile(XML_FILE);
	if (doc == NULL)
	{
		fprintf(stderr, "Failed to parse xml file: %s\n", XML_FILE);
		goto FAILED;
	}

	//获取根节点
	cur = xmlDocGetRootElement(doc);
	if (cur == NULL)
	{
		fprintf(stderr, "Root is empty.\n");
		goto FAILED;
	}

	if ((xmlStrcmp(cur->name, BAD_CAST "root")))
	{
		fprintf(stderr, "The root is not root.\n");
		goto FAILED;
	}

	//遍历处理根节点的每一个子节点
	cur = cur->children;
	while (cur != NULL)
	{
		if ((!xmlStrcmp(cur->name, BAD_CAST "device")))
		{
			parse_root_device(doc, cur, dtr);
		}
		cur = cur->next;
	}
	xmlFreeDoc(doc);
	return 1;
FAILED:
	if (doc)
	{
		xmlFreeDoc(doc);
	}
	return 0;
}
