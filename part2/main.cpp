#if (defined _WIN32 || defined _WIN64)
#include <winsock.h>
#else
#include <netinet/in.h>
#endif

#include <iostream>

#include "string.h"
#include "object.h"
#include "serial.h"

/* This is the driver class for testing serial.h and its supported classes	*/

bool test_String_Array(StringArray* s_array)
{
	char* serialized = serialize(s_array);
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

	if (!d_array->vals_[0] == deserialized->vals_[0])
	{
		return false;
	}
	return true;
}

bool test_Message(Message* msg)
{
	char* serialized = serialize(msg, msg->kind_);
	Message* deserialized = deserialize_msg(serialized);

	return true;
}

bool test__Ack_Message(Ack* ack_msg)
{
	char* serialized = serialize(ack_msg, ack_msg->kind_);
	return true;
}

bool test__Status_Message(Status* stat_msg)
{
	char* serialized = serialize(stat_msg, stat_msg->kind_);
	return true;
}

bool test__Register_Message(Register* regi_msg)
{
	char* serialized = serialize(regi_msg, regi_msg->kind_);
	return true;
}

bool test__Directory_Message(Directory* dir_msg)
{
	char* serialized = serialize(dir_msg, dir_msg->kind_);
	return true;
}

int main()
{
	StringArray* str_a = new StringArray();
	DoubleArray* dou_a = new DoubleArray();
	Message* msg = new Message();
	Ack* a_msg = new Ack();
	Status* s_msg = new Status();
	Register* r_msg = new Register();
	Directory* d_msg = new Directory();

	if (test_String_Array(str_a) == false)
	{
		std::cout << "Failed string array serialization/deserialization" << std::endl;
	}
	if (test_Double_Array(dou_a) == false)
	{
		std::cout << "Failed double array serialization/deserialization" << std::endl;
	}
	if (test_Message(msg) == false)
	{
		std::cout << "Failed general msg serialization/deserialization" << std::endl;
	}
	if (test__Ack_Message(a_msg) == false)
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
