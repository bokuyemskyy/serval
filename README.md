# serval
A simple static HTTP server.

### Build
```
cmake -B build
cmake --build build
```

### Run
```
./build/server/serval
```
By default, it serves the current directory at port 8080:
```
http://localhost:8080/...
```

### Options
```
serval [OPTION...]

  --host arg       Host to bind
  -p, --port arg   Port to use
  -d, --directory arg  Root directory to serve
```

Example:
```
./build/server/serval -d ./public -p 8080
```

Features like directory listing and CORS headers are not yet implemented
