#include<mach/mach.h>
struct message {
mach msg header t header;
int data;
};
mach_port_t client;
mach_port_t server;
/* Client Code */
struct message message;
// construct the header
message.header.msgh size = sizeof(message);
message.header.msgh remote_port = server;
message.header.msgh local_port = client;
// send the message
mach_msg (&message.header, // message header
MACH_SEND_MSG, // sending a message
sizeof(message), // size of message sent
0, // maximum size of received message - unnecessary
MACH_PORT_NULL, // name of receive port - unnecessary
MACH_MSG_TIMEOUT_NONE, // no time outs
MACH_PORT_NULL // no notify port
);
/* Server Code */
struct message message;
// receive the message
mach_msg (&message.header, // message header
MACH_RCV_MSG, // sending a message
0, // size of message sent
sizeof(message), // maximum size of received message
server, // name of receive port
MACH_MSG_TIMEOUT_NONE, // no time outs
MACH_PORT_NULL // no notify port
);


/*
The send and receive operations themselves are flexible. For instance, when
a message is sent to a port, its queue may be full. If the queue is not full,
the message is copied to the queue, and the sending task continues. If the port’s queue is full, the sender has several options (specified via parameters
to mach msg() :

1.  Wait indefinitely until there is room in the queue.
2. Wait at most n milliseconds.
3. Do not wait at all but rather return immediately.
4. Temporarily cache a message. Here, a message is given to the operating
system to keep, even though the queue to which that message is being
sent is full. When the message can be put in the queue, a notification
message is sent back to the sender. Only one message to a full queue can
be pending at any time for a given sending thread.

The final option is meant for server tasks. After finishing a request, a server
task may need to send a one-time reply to the task that requested the service,
but it must also continue with other service requests, even if the reply port for
a client is full.
*/