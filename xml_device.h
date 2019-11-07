#pragma once
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
//#include <iconv.h>
#include <libxml/xmlmemory.h>
#include <libxml/parser.h>
#include "common.h"
#define XML_FILE "device.xml"

typedef struct device_info {
	char host[INET_ADDRSTRLEN];
	char name[50];
	char deviceType[80];
	char uuid[80];
}Device;

static int parse_root_device_iconList_icon(xmlDocPtr doc, xmlNodePtr cur);
static int parse_root_device_serviceList_service(xmlDocPtr doc, xmlNodePtr cur);
static int parse_root_device(xmlDocPtr doc, xmlNodePtr cur, Device* dtr);
int parse_xml(Device* dtr);