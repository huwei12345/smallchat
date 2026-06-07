# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Project Overview

smallchat is a C/S instant messaging application in early development (~50% functional). It consists of:
- A Linux epoll-based C++ server (root directory)
- A Qt5 GUI client (`qtClient/`)
- A simple C++ test client (`client/`)
- MySQL database backend with FTP-based file transfer

## Build Commands

### Server (Linux, requires MySQL Connector/C++)
```bash
make            # Build server binary
make clean      # Clean build artifacts
./server        # Run server (default port in code)
```

### Test Client
```bash
cd client && make && ./client
```

### Database Initialization
```bash
cd sql && make && ./main    # Creates mIM database schema
```

### Qt Client
Open `qtClient/untitled2.pro` in Qt Creator with Qt 5.14 kit. Configure `qtClient/config.ini` with server IP and FTP credentials.

## Prerequisites

- Ubuntu 22.04 (Linux)
- Qt 5.9–5.14.2
- MySQL 8.0 (`sudo apt install mysql`)
- MySQL Connector/C++ (`.deb` packages in `sql/sql.zip`)
- `sudo apt-get install unixodbc unixodbc-dev`
- vsftpd for file transfer (use project's `vsftpd.conf`)
- MySQL credentials: root/123456, database: `mIM`

## Architecture

### Server (Reactor Pattern)
- **Event Loop**: `EventLoop.cpp` uses Linux epoll with edge-triggered mode. One main thread accepts connections, dispatches to IO worker threads via `socketpair` wakeup mechanism.
- **Threading**: Configurable IO threads (default 2). Main thread uses round-robin (`selectAlgorithm()`) to distribute connections.
- **Request Processing**: Command pattern via `RequestProcessor` subclasses. Each of ~40 `FunctionCode` values has a dedicated processor with `Exec(Connection*, Request&, Response&)` interface.
- **Singletons**: `Server`, `MysqlPool`, `FriendCache`, `SetTime` all use `GetInstance()`.
- **Connection/Session**: `Connection` = socket fd + read buffer. `Session` = Connection + userId + login state. Stored in `Server::mConnectionMap` (by fd) and `Server::mUserSessionMap` (by userId).

### Custom Binary Protocol (`MyProtocolStream`)
- 4-byte length-prefixed packets
- 7-bit encoded variable-length integers
- Optional 2-byte checksum
- Streaming operators (`<<`/`>>`) for typed serialization

### Shared Code (Server ↔ Qt Client)
`Protocol.h`, `MyProtocolStream.cpp/.h`, `Trans.h` are shared. `soft.h` defines `#define SERVER` for conditional compilation (server uses pthreads via `mutex.h`; client uses QMutex).

### Qt Client
- Qt Widgets + QML hybrid (`PersonCard.qml`)
- Frameless window design (`framelessWidget`)
- Built-in FTP client (`ftp/` subdirectory)
- Pages: login, register, main, friend, chat, group chat, settings, file management
- Network layer: `network.cpp/.h`

## Key Files

| File | Purpose |
|---|---|
| `Protocol.h` | Data models, Request/Response, FunctionCode enum (~40 RPC codes) |
| `RequestProcessor.cpp/.h` | All request handler classes (command pattern) |
| `server.cpp/.h` | Server singleton, listener, connection management |
| `EventLoop.cpp/.h` | Epoll event loop (main + IO threads) |
| `MyProtocolStream.cpp/.h` | Binary serialization protocol |
| `MysqlPool.cpp/.h` | MySQL connection pool |
| `cache/friendCache.cpp/.h` | In-memory friend list cache |
| `soft.h` | `#define SERVER` conditional compilation flag |
| `txt.md` | Developer notes and TODOs |

## Features (Implemented/In-Progress)

- User registration/login
- Friend search, add, request/accept
- 1-on-1 and group messaging (text, picture, file)
- Group creation/join/management
- FTP-based file upload/download (avatars, images, files)
- Cloud file storage ("Space") with directory tree
- User online/offline state notification
- Message read status tracking
- Offline file delivery
