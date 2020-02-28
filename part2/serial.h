#if (defined _WIN32 || defined _WIN64)
#include <winsock.h>		//sockaddr_in for windows machines
#else
#include <netinet/in.h>		//sockaddr_in for linux/others
#endif

#include <cstdio>
#include <cstdlib>
#include <ctype.h>

#include "string.h"
#include "object.h"
#include "supported.h"
#include <cstring>

/*
	'%' is used as a separator between fields in the serialized 
	form for Messages. DoubleArrays and StringArrays do not get
	the separator.

	*/
const char* SPACER = "%";
size_t SPACER_SIZE = sizeof(SPACER);

/*	
	Serialization - turn an Object (project will use Messages, specifically) in to a 
	char* to pass around in a network.

	Deserialization - turn a char* back into an Object
*/


//done
char* serialize(StringArray* val)
{
	char* size_buffer = new char[sizeof(val->vals_->size_) +
		sizeof(val->vals_->size_) + 1];
	sprintf(size_buffer, "%d", val->vals_->size_);

	char* val_buffer = new char[sizeof(val->vals_->cstr_)];
	memcpy(val_buffer, val->vals_->cstr_, sizeof(val->vals_->cstr_));

	strcat(size_buffer, (const char*)val_buffer);

	delete val_buffer;
	return size_buffer;
}

//done
char* serialize(DoubleArray* val)
{
	char* buffer = new char[sizeof(*val->vals_) + 1];
	sprintf(buffer, "%g", *val->vals_);
	return buffer;
}

/*	
	This payload will look like 
	[[MsgKind as an int][sender][target][id]+optional fields+...]
	*/

/*	done except for Directory case, don't know why there's corrupted
	data in addr_buffer as soon as it's initialized that persists
*/
char* serialize(Message* val)
{
	MsgKind kind = val->kind_;

	char* buffer;

	if (kind == MsgKind::Status)
	{
		Status* s_msg = dynamic_cast<Status*>(val);
		buffer = new char[sizeof(s_msg->kind_)	//MsgKind (int)
			+ sizeof(s_msg->sender_)			//size_t
			+ sizeof(s_msg->target_)			//size_t
			+ sizeof(s_msg->id_)				//size_t
			+ sizeof(s_msg->msg_)				//String*
			+ (SPACER_SIZE * 4)
			+ 1];

		sprintf(buffer, "%d%s%d%s%d%s", 
			s_msg->kind_,
			SPACER,
			s_msg->sender_, 
			SPACER, 
			s_msg->target_,
			SPACER, 
			s_msg->id_,
			SPACER);

		char* str_buffer = new char[sizeof(s_msg->msg_) + 1];
		memcpy(str_buffer, s_msg->msg_->cstr_, sizeof(s_msg->msg_->cstr_));

		strcat(buffer, (const char*)str_buffer);

		delete str_buffer;
	}
	else if (kind == MsgKind::Register)
	{
		Register* r_msg = dynamic_cast<Register*>(val);
		buffer = new char[sizeof(r_msg->kind_)
			+ sizeof(r_msg->sender_)
			+ sizeof(r_msg->target_)
			+ sizeof(r_msg->id_)
			+ sizeof(r_msg->client.sin_family)			//short
			+ sizeof(r_msg->client.sin_port)			//unsigned short
			+ sizeof(r_msg->client.sin_addr.s_addr)		//unsigned long
			+ sizeof(r_msg->client.sin_zero)			//char[8]
			+ sizeof(r_msg->port)						//size_t
			+ (SPACER_SIZE * 6)							//this may need to increase if spaced between sin_zero entries
			+ 1];

		sprintf(buffer, "%d%s%d%s%d%s%d%s%d%s%d%s%d", 
			r_msg->kind_,
			SPACER,
			r_msg->sender_, 
			SPACER,
			r_msg->target_,
			SPACER,
			r_msg->id_,
			SPACER,
			r_msg->client.sin_family,
			SPACER,
			r_msg->client.sin_port,
			SPACER,
			r_msg->client.sin_addr.s_addr);

		char* str_buffer = new char[sizeof(r_msg->client.sin_zero) + 1];
		memcpy(str_buffer, r_msg->client.sin_zero, sizeof(r_msg->client.sin_zero));

		strcat(buffer, SPACER);
		strcat(buffer, (const char*)str_buffer);

		char* port_buffer = new char[sizeof(r_msg->port)];
		sprintf(port_buffer, "%d", r_msg->port);

		strcat(buffer, SPACER);
		strcat(buffer, (const char*)port_buffer);

		delete str_buffer;
		delete port_buffer;
	}
	else if (kind == MsgKind::Directory)
	{
		std::cout << "Directory test-- " << std::endl;
		Directory* d_msg = dynamic_cast<Directory*>(val);

		buffer = new char[sizeof(d_msg->kind_) 
			+ sizeof(d_msg->sender_)
			+ sizeof(d_msg->target_) 
			+ sizeof(d_msg->id_) 
			+ sizeof(d_msg->client)			//size_t
			+ sizeof(d_msg->ports)			//size_t*
			+ sizeof(d_msg->addresses)		//String**
			+ (SPACER_SIZE * 5) 
			+ (SPACER_SIZE * (sizeof(d_msg->addresses)/sizeof(String*)))
			+ 1];

		sprintf(buffer, "%d%s%d%s%d%s%d%s%d%s%d",
			d_msg->kind_, 
			SPACER,
			d_msg->sender_,
			SPACER,
			d_msg->target_,
			SPACER,
			d_msg->id_,
			SPACER,
			d_msg->client,
			SPACER,
			*d_msg->ports);


		char* addr_buffer = new char[sizeof(d_msg->addresses)] ();
		std::cout << "addr_buffer with no load: " << addr_buffer << std::endl;
		//Assumption: addresses is null-terminated

		while (*(d_msg->addresses))
		{
			strcat(addr_buffer, SPACER);
			strcat(addr_buffer, (const char*)(*(d_msg->addresses)++)->cstr_);
		}

		strcat(addr_buffer, SPACER);
		std::cout << "addr_buffer after loading: " << addr_buffer << std::endl;
		strcat(buffer, addr_buffer);
	}
	else //Ack or other unsupported MsgKinds
	{
		buffer = new char[sizeof(val->kind_) 
			+ sizeof(val->sender_)
			+ sizeof(val->target_) 
			+ (SPACER_SIZE * 3)
			+ sizeof(val->id_) + 1];

		sprintf(buffer, "%d%s%d%s%d%s%d", 
			val->kind_, 
			SPACER,
			val->sender_, 
			SPACER,
			val->target_,
			SPACER,
			val->id_);
	}
	
	std::cout << "BUFFER IS " << buffer << std::endl << std::flush;
	return buffer;
}

//done
StringArray* deserialize_sarray(char* val)
{
	size_t size = 0;
	while (isdigit(*val))
	{
		++val;
		//size += (size_t)atoi(val[0]);
	}

	String* reformed = new String(val);

	StringArray* sa_reformed = new StringArray();
	sa_reformed->vals_ = reformed;

	return sa_reformed;
}

//done
DoubleArray* deserialize_darray(char* val)
{
	double val_ = strtod((const char*)val, NULL);

	double* reformed = &val_;

	DoubleArray* da_reformed = new DoubleArray();
	da_reformed->vals_ = reformed;

	return da_reformed;
}

//todo: use the % separator to pull fields out of the serialization
//instead of incrementing
Message* deserialize_msg(char* val)
{
	if ((int)val[0] == static_cast<int>(MsgKind::Ack))
	{
		Ack* reformed = new Ack();
		char* token;
		token = strtok(val, SPACER);
		reformed->kind_ = (MsgKind)atoi(token);
		token = strtok(val, SPACER);
		reformed->sender_ = (size_t)atoi(token);
		token = strtok(val, SPACER);
		reformed->target_ = (size_t)atoi(token);
		token = strtok(val, SPACER);
		reformed->id_ = (size_t)atoi(token);
	}
	else if ((int)val[0] == static_cast<int>(MsgKind::Status))
	{

	}
	else if ((int)val[0] == static_cast<int>(MsgKind::Register))
	{

	}
	else if ((int)val[0] == static_cast<int>(MsgKind::Directory))
	{

	}
	else if ((int)val[0] == static_cast<int>(MsgKind::Nack))
	{

	}
	else if ((int)val[0] == static_cast<int>(MsgKind::Put))
	{

	}
	else if ((int)val[0] == static_cast<int>(MsgKind::Reply))
	{

	}
	else if ((int)val[0] == static_cast<int>(MsgKind::Get))
	{

	}
	else if ((int)val[0] == static_cast<int>(MsgKind::WaitAndGet))
	{

	}
	else if ((int)val[0] == static_cast<int>(MsgKind::Kill))
	{

	}

	Message m;
	Message* pm = &m;
	return pm;
}