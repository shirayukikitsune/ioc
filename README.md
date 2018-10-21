# ioc

[![Build status][appveyor-badge]][appveyor-url]

Another C++ header-only library for IoC

## Important notes

This implementation requires the use of interfaces (_abstract classes_) and latest C++ features (C++17).

## Usage

Your interface must extend the `::kitsune::ioc::ServiceBase` type, passing itself as the template parameter.
For example:

```c++
class MyService : public ::kitsune::ioc::ServiceBase<MyService> {
public:
  virtual void foo() = 0;
};
```

Then your implementation must extend the `::kitsune::ioc::Service` type, passing the base and the implementation types as parameters.
For example:

```c++
class MyServiceImplementation : public ::kitsune::ioc::Service<MyServiceImplementation, MyService> {
public:
  void foo() {
    std::cout << "MyServiceImplementation::foo()\n";
  }
};
```

You may also annotate with `::kitsune::ioc::InjectionFlags::Primary` if you want your service to be provided in priority.
For example:
```c++
using namespace ::kitsune::ioc;
class MyImportantServiceImplementation : public Service<MyServiceImplementation, MyService, InjectionFlags::Primary> {
public:
  void foo() {
    std::cout << "MyImportantServiceImplementation::foo()\n";
  }
};
```

Please note that if you attempt to register two services using the same interface as primary, a `::std::logic_error` will be thrown.

In order to use a service, you may simply instantiate a `::kitsune::ioc::Injectable`.
For example:

```c++
::kitsune::ioc::Injectable<MyService> myServiceInstance;

// You can check if the service was injected by casting to bool
if (myServiceInstance) {
  myServiceInstance->foo();
}
```

If you want to get a specific implementation, you can add your service as the second parameter.

```c++
::kitsune::ioc::Injectable<MyService, MyServiceImplementation> myServiceImplementationInstance;

if (myServiceImplementationInstance) {
  myServiceImplementationInstance->foo();
}
```

If you need to get all instances of your service, you must call the Injector directly:

```c++
auto services = ::kitsune::ioc::Injector<MyService>::getInstance().findServices();

// services is a std::list of std::weak_ptr to MyService
```

## Compiler notes

Due to a compiler bug on G++ and CLang, you must use the `KITSUNE_INJECTABLE` macro.

You may use it after the declaration of your class:

```c++
class MyServiceImplementation : public ::kitsune::ioc::Service<MyServiceImplementation, MyService> {
public:
  void foo() {
    std::cout << "MyServiceImplementation::foo()\n";
  }
};

KITSUNE_INJECTABLE(MyService, MyServiceImplementation, aVariableNameExclusiveForThisInstance);
```

The macro has these parameters:

* The interface class name;
* The implementation class name;
* A variable name that will be added to the global scope;

[appveyor-badge]: https://ci.appveyor.com/api/projects/status/couqe9ysm5frk7do/branch/master?svg=true
[appveyor-url]: https://ci.appveyor.com/project/shirayukikitsune/ioc/branch/master
