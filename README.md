# ft_p

ft_p is simple file transfer protocol using TCP. Multiple client can access to server with uploading and downloading file. Server and client both can be sender and receiver of files.

When transfer file, to assure right order of file chunk sequence, sender and receiver shares maximum chunk size and count. Receiver doesn't concat chunk if sequence is not serial but save it to prepared buffer. If expected sequence chunk arrives, receiver concat it and pop chunks from buffer that match sequence after arrived one.
Sender receives ACK from receiver and never transfer over `rcwd`(receiver's buffer space). The idea is originated from flow control and accumulated ACK of TCP.

<br><br>

## Requirements

```
# download libft
git clone https://github.com/jebae/libft.git

# directory structure
root/
 libs/
  libft
```

<br><br>

## Installation

```
# server and client in one command
$> make

# only server with macro CHUNK_SIZE and CHUNK_COUNT
$> pwd
ft_p/server
$> make def="-DCHUNK_SIZE=<...> -DCHUNK_COUNT=<...>"

# or simply use default CHUNK_SIZE=1024 and CHUNK_COUNT=32
$> make

# only client with macro CHUNK_SIZE and CHUNK_COUNT
$> pwd
ft_p/client
$> make def="-DCHUNK_SIZE=<...> -DCHUNK_COUNT=<...>"

# or simply use default CHUNK_SIZE=1024 and CHUNK_COUNT=32
$> make
```

<br/><br/>

## Usage

### server

```
$> pwd
ft_p/server
$> ./server [port number]
```

<br/>

### client

```
$> pwd
ft_p/client
$> ./client [server port number]
```

<br/><br/>

## Features

Both server and client has own file storage directory named `.storage`.

```
# server

$> pwd
ft_p/server
$> ls -a
.storage srcs includes ...

# client

$> pwd
ft_p/client
$> ls -a
.storage srcs includes ...
```

Client downloads every file to `.storage` directly. Server can have subdirectory made by client or on your own.

<br/>

### Client commands

- `cd [directory name]`: change directory to `[directory name]`. If no argument, change directory to root(`.storage`).

- `ls [-l] [directory name]`: list files and subdirectories of `[directory name]`. If no argument, list current directory's.

- `pwd`: print current directory path.

- `mkdir [directory name]`: make directory in current directory.

- `rmdir [directory name]`: remove directory.

- `get [file name]`: download file.

- `put [file name]`: upload file.

- `del [file name]`: remove file.

- `lls [directory or file name]`: same as UNIX `ls` command. `lls` shows client directory's contents.

- `quit`: disconnect connection.
