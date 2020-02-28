/*
#if (defined _WIN32 || defined _WIN64)
#include <winsock.h>
#else
#include <netinet/in.h>
#endif
*/

#include <netinet/in.h>
#include <iostream>

#include <cstring>
#include "string.h"
#include "object.h"
#include "serial.h"

/* This is the driver class for testing serial.h and its supported classes	*/

bool test_String_Array(StringArray* s_array)
{
	char* serialized = serialize(s_array);
	std::cout << "serialized sarray: " << serialized << std::endl;
	StringArray* deserialized = deserialize_sarray(serialized);

	if (!deserialized->vals_->equals(s_array->vals_))
	{
		return false;
	}
	else {
		return true;
	}
}

bool test_Double_Array(DoubleArray* d_array)
{
	char* serialized = serialize(d_array);
	DoubleArray* deserialized = deserialize_darray(serialized);

	if (!*d_array->vals_ == *deserialized->vals_)
	{
		return false;
	}
	return true;
}

bool test__Ack_Message(Message* ack_msg)
{
	char* serialized = serialize(ack_msg);

	return true;
}

bool test__Status_Message(Status* stat_msg)
{
	char* serialized = serialize(stat_msg);

	return true;
}

bool test__Register_Message(Register* regi_msg)
{
	char* serialized = serialize(regi_msg);
	return true;
}

bool test__Directory_Message(Directory* dir_msg)
{
	char* serialized = serialize(dir_msg);
	return true;
}

int main()
{
	String* abc = new String("abc");
	StringArray* str_a = new StringArray();
	str_a->vals_ = abc;

	double d = 4.5;
	double* p_d = &d;
	DoubleArray* dou_a = new DoubleArray();
	dou_a->vals_ = p_d;

	Message* msg = new Message();
	msg->kind_ = MsgKind::Ack;
	msg->sender_ = 0;
	msg->target_ = 1;
	msg->id_ = 99999;

	Status* s_msg = new Status();
	s_msg->kind_ = MsgKind::Status;
	s_msg->sender_ = 1;
	s_msg->target_ = 2;
	s_msg->id_ = 88888;
	s_msg->msg_ = abc;

	Register* r_msg = new Register();
	r_msg->kind_ = MsgKind::Register;
	r_msg->sender_ = 2;
	r_msg->target_ = 3;
	r_msg->id_ = 77777;

	sockaddr_in m_saddr;
	m_saddr.sin_family = 44;
	m_saddr.sin_port = 55;

	in_addr m_inaddr;
	m_inaddr.s_addr = 66;

	m_saddr.sin_addr = m_inaddr;
	for (int i = 0; i < 7; i++)
	{
		m_saddr.sin_zero[i] = 'a';
	}

	r_msg->client = m_saddr;
	r_msg->port = 77;


	Directory* d_msg = new Directory();
	d_msg->kind_ = MsgKind::Directory;
	d_msg->sender_ = 3;
	d_msg->target_ = 4;
	d_msg->id_ = 66666;
	d_msg->client = 5;

	size_t port = 6;
	d_msg->ports = &port;

	String* addr1 = new String("ADDRESS1-321");
	String* addr2 = new String("ADDRESS2-123");
	String* dirs[] = { addr1, addr2, NULL };
	d_msg->addresses = dirs;


	if (test_String_Array(str_a) == false)
	{
		std::cout << "Failed string array serialization/deserialization" << std::endl;
	}
	if (test_Double_Array(dou_a) == false)
	{
		std::cout << "Failed double array serialization/deserialization" << std::endl;
	}
	if (test__Ack_Message(msg) == false)
	{
		std::cout << "Failed ack mesg serialization/deserialization" << std::endl;
	}
	if (test__Status_Message(s_msg) == false)
	{
		std::cout << "Failed status msg serialization/deserialization" << std::endl;
	}
	if (test__Register_Message(r_msg) == false)
	{
		std::cout << "Failed register msg serialization/deserialization" << std::endl;
	}
	if (test__Directory_Message(d_msg) == false)
	{
		std::cout << "Failed directory msg serialization/deserialization" << std::endl;
	}

	std::cout << "Finished all tests" << std::endl;

	return 0;
}
