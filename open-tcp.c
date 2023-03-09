#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
#include <time.h>
#define PORT 514
#define RCVBUF_SIZE (1024 * 1024 * 16) // 16 MB

int main()
{
    int server_fd, new_socket, valread;
    struct sockaddr_in address;
    int opt = 1;
    int RCVBUF_SIZE2 = 1024 * 1024 * 16 ;
    int addrlen = sizeof(address);

    char buffer[1024] = {0};

    // Create socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    // Set socket options to reuse the address and port
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)))
    {
        perror("setsockopt failed");
        exit(EXIT_FAILURE);
    }

    // Set socket receive buffer size
    if (setsockopt(server_fd, SOL_SOCKET, SO_RCVBUF, &RCVBUF_SIZE2, sizeof(RCVBUF_SIZE)) < 0)
    {
        perror("setsockopt failed");
        exit(EXIT_FAILURE);
    }

    // Set socket address
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    // Bind socket to address
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0)
    {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    // Listen for incoming connections
    if (listen(server_fd, 3) < 0)
    {
        perror("listen failed");
        exit(EXIT_FAILURE);
    }

    // Accept a single connection
    if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen)) < 0)
    {
        perror("accept failed");
        exit(EXIT_FAILURE);
    }

int total_bytes_received = 0;
int num_packets_received = 0;
time_t start_time = time(NULL);

while (1)
{
    char buffer[1024];
    int valread = recv(new_socket, buffer, 1024, 0);
    if (valread == 0)
    {
        printf("Connection closed by peer\n");
        break;
    }
    else if (valread == -1)
    {
        perror("recv failed");
        exit(EXIT_FAILURE);
    }
    else
    {
        // Increment the counter and total bytes received
        num_packets_received++;
        total_bytes_received += valread;
    }

    // Check if 5 seconds have elapsed
    time_t current_time = time(NULL);
    if (current_time - start_time >= 5)
    {
        // Calculate the average data received rate
        double average_rate = (double)total_bytes_received / (current_time - start_time);

        // Calculate the logs per second
        double logs_per_sec = average_rate / 312.0;

	    // Format the current time as a string
        char time_string[80];
        strftime(time_string, sizeof(time_string), "%Y-%m-%d %H:%M:%S", localtime(&current_time));

    // Print the rates and timestamp to stdout
       printf("%s - Received : %.2f logs/sec, Data: %.2f bytes/sec\n", time_string, logs_per_sec, average_rate);

        // Reset the counter and start time
        total_bytes_received = 0;
        num_packets_received = 0;
        start_time = current_time;
    }

    // Acknowledge packets by sending an empty ACK packet
    send(new_socket, "", 0, 0);
}

    close(new_socket);
    close(server_fd);

    return 0;
}
