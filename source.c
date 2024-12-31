#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <fcntl.h>

#define PORT 8080
#define BUFFER_SIZE 1024

void handle_client(int client_socket);
void send_response(int client_socket, const char *status, const char *content_type, const char *content);
void send_404(int client_socket);

int main() {
    int server_socket, client_socket;
    struct sockaddr_in server_addr, client_addr;
    socklen_t addr_len = sizeof(client_addr);

    // socket //
    if ((server_socket = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    // Bind socket //
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    if (bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        perror("Bind failed");
        close(server_socket);
        exit(EXIT_FAILURE);
    }

    // Listen for connections //
    if (listen(server_socket, 10) == -1) {
        perror("Listen failed");
        close(server_socket);
        exit(EXIT_FAILURE);
    }

    printf("HTTP Server is running on port %d\n", PORT);

    while (1) {
        // Accept a client connection //
        client_socket = accept(server_socket, (struct sockaddr *)&client_addr, &addr_len);
        if (client_socket == -1) {
            perror("Accept failed");
            continue;
        }

        handle_client(client_socket);
        close(client_socket);
    }

    close(server_socket);
    return 0;
}

void handle_client(int client_socket) {
    char buffer[BUFFER_SIZE], method[16], path[256], protocol[16];
    int bytes_read;

    // Read client request //
    bytes_read = recv(client_socket, buffer, sizeof(buffer) - 1, 0);
    if (bytes_read <= 0) return;

    buffer[bytes_read] = '\0';
    sscanf(buffer, "%15s %255s %15s", method, path, protocol);

    printf("Request: %s %s %s\n", method, path, protocol);

    if (strcmp(method, "GET") != 0) {
        send_response(client_socket, "405 Method Not Allowed", "text/plain", "Only GET is supported.");
        return;
    }

    // Remove leading '/' from path
    if (path[0] == '/') memmove(path, path + 1, strlen(path));

    // If no path, serve index.html
    if (strlen(path) == 0) strcpy(path, "index.html");

    // Open and read file
    int file = open(path, O_RDONLY);
    if (file == -1) {
        send_404(client_socket);
        return;
    }

    struct stat file_stat;
    fstat(file, &file_stat);

    char *content = malloc(file_stat.st_size);
    read(file, content, file_stat.st_size);

    send_response(client_socket, "200 OK", "text/html", content);
    free(content);
    close(file);
}

void send_response(int client_socket, const char *status, const char *content_type, const char *content) {
    char header[BUFFER_SIZE];
    snprintf(header, sizeof(header),
             "HTTP/1.1 %s\r\n"
             "Content-Type: %s\r\n"
             "Content-Length: %ld\r\n"
             "\r\n",
             status, content_type, strlen(content));

    send(client_socket, header, strlen(header), 0);
    send(client_socket, content, strlen(content), 0);
}

void send_404(int client_socket) {
    const char *not_found = "<html><body><h1>404 Nothing found dear friend!</h1></body></html>";
    send_response(client_socket, "404 Not Found", "text/html", not_found);
}
