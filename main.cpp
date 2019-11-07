#include "common.h"
#include <iostream>
#include <thread>
#include <mutex>

using namespace std;

mutex mt;

void write()
{
	mt.lock();
	ssdp_discover();
	mt.unlock();
}

void read()
{
	mt.lock();
	get_device_info();
	mt.unlock();
}

int main()
{
	ssdp_discover();
	/*
	get_device_info();
	*/

	system("pause");
	return 0;
}