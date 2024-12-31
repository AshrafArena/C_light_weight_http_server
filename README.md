# Lightweight HTTP Server

This project is a lightweight HTTP server that I wrote in C. I've always found lower-level languages far more intriguing, as they are the building blocks of interpreted ones. So, here is my first complete project using C. It demonstrates basic networking and file-serving capabilities, whih has been an  an excellent starting point for understanding how web servers work at a lower level for me. 

## Features

- Handles **GET** requests.
- Serves static files from the current directory.
- Returns a **404 Not Found** response for missing files.
- Simple, clean implementation using sockets and basic HTTP parsing.

## Getting Started

### Prerequisites

- A Linux or Unix-like system.
- GCC compiler (or any C compiler).

### Compilation

Compile the server using the following command:

```bash
gcc -o light_http_server light_http_server.c
```

### Usage

1. Place your static files (e.g., `index.html`) in the same directory as the compiled server.
2. Start the server:

```bash
./light_http_server
```

3. Access the server in your browser or with a tool like `curl`:

- **Browser**: `http://localhost:8080`
- **Curl**: `curl http://localhost:8080`

### Stopping the Server

Press `Ctrl+C` to stop the server.

## Code Overview

- **Libraries**: Utilises standard C libraries for networking (`<sys/socket.h>`, `<arpa/inet.h>`) and file handling (`<fcntl.h>`, `<sys/stat.h>`).
- **Main Components**:
  - `socket()`, `bind()`, `listen()`, `accept()` for handling connections.
  - Serves files by reading their contents and responding with HTTP headers.

## Limitations

- Supports only **GET** requests.
- No support for HTTPS or advanced HTTP features.
- No concurrency; handles one request at a time.

## Future Improvements

- I plan to add support for other HTTP methods (e.g., POST, DELETE).
- Implement multi-threading for handling concurrent clients.
- Add HTTPS support using OpenSSL.
- This is what I’m hoping for... let’s C what I actually accomplish :S

