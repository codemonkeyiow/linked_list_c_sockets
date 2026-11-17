# linked_list_c_sockets
Client/Server model to work with Linked List data structure.

## Build
```
make
```

## Run
```
./bin/server          # terminal 1 — owns the shared list on port 9090
./bin/client          # terminal 2+ — any client can push/pop
```

## Commands
| Client | Server does |
|--------|-------------|
| `push <n>` | push node onto the shared list |
| `pop` | pop head node (or `EMPTY`) |
| `print` | print list on the server |
| `quit` | disconnect |
