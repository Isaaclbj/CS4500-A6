#if (defined _WIN32 || defined _WIN64)
#include <winsock.h>		//sockaddr_in for windows machines
#else
#include <netinet/in.h>		//sockaddr_in for linux/others
#endif

#include "string.h"
#include "object.h"
#include "supported.h"
#include <cstring>

/*	
	Serialization - turn an Object (project will use Messages, specifically) in to a 
	char* to pass around in a network.

	Deserialization - turn a char* back into an Object
*/

/* In this case, we'll need to get put together size_ and cstr_ of the 
	String* into the buffer. 
	IN PROGRESS */
char* serialize(StringArray* val)
{

	//a buffer for the data to load into
	char* buffer;

	//get the size of the cstr contained in the String
	size_t size_of_string = sizeof(val->vals_->cstr_);

	//get the size of the size of the String (is this necessary?)
	size_t size_of_size = val->vals_->size_;

	//TODO, need to advance the buffer sizeof(val->vals_) first, then memcpy the size field onto buffer.
	memcpy(buffer, &(val->vals_->size), size_of_size);
	memcpy(buffer, val->vals_, size_of_string);			//should I add a nullpointer value to the end of this?

	return buffer;
}

/*	In this case, we can just memcpy the double* into the buffer.
	DONE 
	*/
char* serialize(DoubleArray* val)
{
	char* buffer;
	size_t val_size = sizeof(val->vals_);

	memcpy(buffer, val->vals_, val_size);
	return buffer;
}

/*	In this case, we'll need to put together all of the fields of the Message,
	in addition to additional fields based on the MsgKind.
	IN PROGRESS
	*/
char* serialize(Message* val, MsgKind kind)
{
	switch (kind)
	{
	case MsgKind::Ack:
		break;
	case MsgKind::Status:
		break;
	case MsgKind::Register:
		break;
	case MsgKind::Directory:
		break;
	default:					//other message kinds
		break;
	}

	char a = 'a';
	char* pa = &a;
	return pa;
}

/*	TODO */
StringArray* deserialize_sarray(char* val)
{
	StringArray sa;
	StringArray* psa = &sa;
	return psa;
}

/*	TODO */
DoubleArray* deserialize_darray(char* val)
{
	DoubleArray da;
	DoubleArray* pda = &da;
	return pda;
}

/*	TODO */
Message* deserialize_msg(char* val)
{
	Message m;
	Message* pm = &m;
	return pm;
}