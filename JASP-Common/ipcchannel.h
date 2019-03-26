//
// Copyright (C) 2013-2018 University of Amsterdam
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
//

#ifndef IPCCHANNEL_H
#define IPCCHANNEL_H

/* for reasons unclear to me, the boost semaphores use 100% CPU on OS X
 * and sometimes under windows too. hence, there are platform specific
 * implementations below */

#ifdef __APPLE__
#include <semaphore.h>
#elif defined _WIN32

#undef Realloc
#undef Free

#include <windows.h>
#else
#include <boost/interprocess/sync/named_semaphore.hpp>
#endif

#include <boost/interprocess/managed_shared_memory.hpp>
#include <boost/container/string.hpp>

typedef boost::interprocess::allocator<char,	boost::interprocess::managed_shared_memory::segment_manager	> CharAllocator;
typedef boost::container::basic_string<char,	std::char_traits<char>, CharAllocator						> String;
typedef boost::interprocess::allocator<String,	boost::interprocess::managed_shared_memory::segment_manager	> StringAllocator;

class IPCChannel
{
public:
	IPCChannel(std::string name, int channelNumber, bool isSlave = false);
	~IPCChannel();

	void send(std::string &data,	bool alreadyLockedMutex = false);
	void send(std::string &&data,	bool alreadyLockedMutex = false);
	bool receive(std::string &data, int timeout = 0);

	int channelNumber() { return _channelNumber; }

private:

	bool tryWait(int timeout = 0);

	std::string _baseName, _nameControl, _nameMtS, _nameStM;
	int _channelNumber;
	bool _isSlave;

	void doubleMemoryOut();
	void rebindMemoryInIfSizeChanged();

	boost::interprocess::managed_shared_memory *_memoryControl, *_memoryMasterToSlave, *_memorySlaveToMaster, *_memoryIn, *_memoryOut;

	boost::interprocess::interprocess_mutex *_mutexOut;
	boost::interprocess::interprocess_mutex *_mutexIn;

	String *_dataOut, * _dataIn;


	size_t *_sizeMtoS, *_sizeStoM, *_sizeIn, *_sizeOut, _previousSizeIn, _previousSizeOut;

	void generateNames();
	std::string _mutexInName,
				_mutexOutName,
				_dataInName,
				_dataOutName,
				_semaphoreInName,
				_semaphoreOutName;

#ifdef __APPLE__
	sem_t* _semaphoreOut;
	sem_t* _semaphoreIn;
#elif defined _WIN32
    HANDLE _semaphoreOut;
    HANDLE _semaphoreIn;

#else
	boost::interprocess::named_semaphore* _semaphoreOut;
	boost::interprocess::named_semaphore* _semaphoreIn;
#endif



};

#endif // IPCCHANNEL_H
