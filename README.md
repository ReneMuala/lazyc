# LazyC 💤

Write safe c code with ease!

# Building

> `mkdir build || cd build && cmake .. && make`

# Modules

|Name|files|description|
|:--|:--|:--|
|Memory|src/resource.* |Implements a model of resource management where a set of resources are owned by a `lazy_controller`that if responsible for freeing the set when its not needed |
|mutex| src/mutex.* | basic implementation on a mutex controller for multithreaded routines & functions |
|error| src/error.* | error logger |