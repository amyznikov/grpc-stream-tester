# grpc-stream-tester

This is a simple client and server pair for testing grpc bug #999 https://github.com/grpc/grpc/issues/999.

Server listens on localhost:7000, client connects and requests a stream of server events.
Each event is genetrated in a loop with 1 sec interval.

To check if bug #999 is present in grpc build, just run server, then run client, wait for a few events, then kill client (using Ctrl+C for example) and see if server is crashed.




$ git clone https://github.com/amyznikov/grpc-stream-tester.git
$ cd grpc-stream-tester
$ make


Client output:
$ ./client 
begin read
event: event #0
event: event #1
event: event #2
event: event #3
^C
$



Server output:
$ ./server 
sending 'event #0'
sending 'event #1'
sending 'event #2'
sending 'event #3'
sending 'event #4'
sending 'event #5'
E0519 17:41:46.704052458   21090 server.cc:291] assertion failed: GRPC_CALL_OK == grpc_call_start_batch(call->call(), ops, nops, buf)
Aborted (core dumped)



