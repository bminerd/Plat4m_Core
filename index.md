### FAQ
__*Doesn't C++ add too much overhead for embedded systems?*__

C++ doesn't force you to incur overhead, no. In fact, most C code can be compiled with a C++ compiler and the result would look the same in most cases. However, certain features of C++ (like virtual functions, templates, and exceptions) will add varying amounts of overhead, depending on usage. To keep things simple and deterministic, Plat4m doesn't use exceptions, RTTI, or STL (Standard Template Library). Think of it as C++ lite.

__*If you're not using exceptions, RTTI, or STL, aren't you basically just using C?*__

C++ has many other nice features over C, including classes, templates, better type safety, namespaces, etc.

__*Can't you do object oriented programming in C?*__

Sort of, but it won't be as pretty or safe as using classes. While you can create handles to structures with function tables in C, the resultant code is much more exposed than creating a class in C++ and proper initialization (via a constructor) can't be guaranteed. Also, polymorphism using this method makes things exponentially worse.

__*Don't C++ objects require dynamic memory allocation?*__

No, and this is where some people get confused. C++ doesn't require you to dynamically allocate memory when instantiating objects. It's perfectly legal to put objects in RAM:
```c++
static Object object;
```
or on the stack:
```c++
{
    Object object;
}
```
The 'new' operator isn't required. However, the MemoryAllocationLite driver allows for easy dynamic memory allocation without de-allocation (sometimes called a "never-freed" heap).