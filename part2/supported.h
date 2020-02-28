#if (defined _WIN32 || defined _WIN64)
#include <winsock.h>
#else
#include <netinet/in.h>
#endif

#include "string.h"

class StringArray {
public:
	String* vals_;
};



class DoubleArray {
public:
	double* vals_;
};



enum class MsgKind {
	Ack, Nack, Put,

	Reply, Get, WaitAndGet, Status,

	Kill, Register, Directory
};



class Message : public Object {
public:
	MsgKind kind_;  // the message kind

	size_t sender_; // the index of the sender node

	size_t target_; // the index of the receiver node

	size_t id_;     // an id t unique within the node
};



class Ack : public Message {

};



class Status : public Message {
public:
	String* msg_; // owned

};



class Register : public Message {
public:

	sockaddr_in client;

	size_t port;

};



class Directory : public Message {
public:

	size_t client;

	size_t * ports;  // owned

	String ** addresses;  // owned; strings owned

};