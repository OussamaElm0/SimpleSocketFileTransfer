# SimpleSocketFileTransfer

A basic client-server application in C that demonstrates file upload and download over a socket connection. The client can send commands to upload files, download files, or terminate the session. The server handles these commands and facilitates file transfer.

## Features
- **File Upload**: Upload files from the client to the server.
- **File Download**: Download files from the server to the client.
- **Command-Based Communication**: Simple commands to manage the session (`/upload`, `/download`, `/exit`).

## Requirements
- GCC Compiler
- Linux/Unix-based environment (for `sys/socket.h` and related functionalities)

## Usage

### 1. Clone the repository
```bash
git clone https://github.com/yourusername/SimpleSocketFileTransfer.git
cd SimpleSocketFileTransfer
```

### 2. Compile the code
```bash
gcc server.c -o server
gcc client.c -o client
```

### 3. Run the server
```bash
./server
```

### 4. Run the client
```bash
./client
```

### 5. Available Commands
- `/upload`  
  Upload a file from the client to the server.
- `/download`  
  Download a file from the server to the client.
- `/exit`  
  Terminate the client session.

## File Structure
- `server_files/` - Directory where the server stores uploaded files.
- `client_files/` - Directory where the client stores downloaded files.
- `client.c` - Client-side implementation.
- `server.c` - Server-side implementation.

## Note
Ensure the directories `server_files` and `client_files` exist before running the application. You can create them as follows:
```bash
mkdir server_files client_files
```

## Example
  1. Start the server:
     ```bash
     ./server
     ```
2. Connect the client in a separate terminal:
    ```bash
    ./client
    ```
3. Use `/upload` to upload a file from the client to the server :
    - Enter the file name when prompted.
   - The file will be sent to the server's `server_files/` directory.
     
4. Use `/download` to download a file from the server to the client:
   - Specify the file name when prompted.
   - The file will be saved in the client's `client_files/` directory.
5. Use `/exit` to close the client session and disconnect from the server.
     
