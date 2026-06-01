# C++ User Management API

A lightweight RESTful web service built using C++ and the Crow web framework. This application provides a thread-safe API for managing user data in memory.

## Project Structure

```text
cpp_server/
├── CMakeLists.txt          # Build configuration
├── external/               # Third-party libraries
│   └── crow_all.h          # Crow framework header
├── include/                # Header files
│   └── user_model.hpp      # User data structures and global state
└── src/                    # Source files
    └── main.cpp            # Application entry point and routes
```

## Prerequisites

* C++17 compatible compiler (GCC 7+, Clang 5+, or MSVC 2017+)
* CMake 3.28 or higher
* Pthreads library (standard on Linux/macOS)

## Build Instructions

1. Navigate to the project root directory:
   ```bash
   cd cpp_server
   ```

2. Create a build directory and enter it:
   ```bash
   mkdir build
   cd build
   ```

3. Configure the project using CMake:
   ```bash
   cmake ..
   ```

4. Compile the source code:
   ```bash
   make
   ```

## Running the Application

After a successful build, start the server by executing the binary:

```bash
./cpp_server
```

The server will initialize and begin listening for requests on `http://0.0.0.0:18080`.

## API Documentation

### Create User
Registers a new user in the system.

* **URL:** `/user`
* **Method:** `POST`
* **Data Params:**
  ```json
  {
    "name": "string",
    "email": "string"
  }
  ```
* **Success Response:**
  * **Code:** 201 Created
  * **Content:**
    ```json
    {
      "id": 1,
      "name": "John Doe",
      "email": "john@example.com"
    }
    ```

### Testing with cURL
You can test the API using the following terminal command:

```bash
curl -X POST http://localhost:18080/user \
     -H "Content-Type: application/json" \
     -d '{"name": "Tanishq", "email": "tanishq@example.com"}'
```

## Technical Implementation Details

### Concurrency
The application is configured to run in multithreaded mode. To prevent data corruption and race conditions during simultaneous requests, a `std::mutex` is implemented. Every access to the internal user map is protected by a `std::lock_guard`, ensuring thread-safe operations.

### Data Serialization
Internal C++ objects are serialized to JSON format using the `crow::json::wvalue` class. This allows the API to communicate effectively with web browsers and other external services.

### Build System
The project uses CMake for dependency management and build automation. Header search paths are explicitly defined in `target_include_directories` to include the `external` and `include` folders.
