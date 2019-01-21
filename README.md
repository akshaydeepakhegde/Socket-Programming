# Socket Programming

In this project I will implement a simple model of computational offloading where a single
client offloads some computation to a server (edge server) which in turn distributes the load
over 2 backend servers. The server facing the client then collects the results from the backend
and communicates the same to the client in the required format. As a result, the system is
comprised of three different parts:

1. Client: submits jobs to Google Compute Engine for processing.
2. Edge Server: Communicates with the client, receives the job, dispatches the job to back
end servers, receives their responses, builds the final output and sends the result back
to client.
3. Backend Servers: They perform specific computations they are assigned to. In our case,
one server performs bitwise "and" and the other performs bitwise "or" operations.

The servers together constitute “Google Compute Engine” (edge and backend servers, in total 3
servers). The client and the edge server communicate over a TCP connection while the
communication between the edge server and the Back-Servers is connectionless and over
UDP. This setup is illustrated in Figure 1.


