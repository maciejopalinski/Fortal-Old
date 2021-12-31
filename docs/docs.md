# access specifiers

- entrypoint - same as public, informs compiler where to start executing the program
- public - accessible to the entire world
- protected - accessible from current package and all derived classes
- default (no keyword) - accessible only from current package
- private - accessible only from current class

# native keyword

- used only by methods that are implemented in another language (i.e. implemented in a compiler or compiler extension/plugin)

# static keyword

- can only be used in class context
- static class member is accessible from class instance and class itself, althought compiler will throw a warning if you access static class member from class instance, you should access it from the class itself
- static class method cannot use this keyword, because it does not refer to an instance, rather a class object

# final keyword

- final class cannot be subclassed
- final method cannot be overridden
- final variable can only be initialized once, you don't have to initialize it while defining

# const keyword

- const unused but reserved
- (consider) alias for static final in class context, alias for final outside class context

# abstract keyword

- abstract class cannot be created, it only can be subclassed
- abstract method cannot be static, it is not possible to call it, it can only be overridden

# interface

- can only contain abstract members or abstract methods

# class

- can only have public of default access specifier
- can implement many interfaces, only if they have no common members or methods
- can only extend one class (other than itself)

# 'this' identifier/keyword

- 'this' **is not** a reserved keyword
- variable with identifier 'this' gets created in class scope when in class context

# default constructors

- there is no such thing as a default constructor
- if variable is not initialized with a call to the class constructor it is null

# pointers

- every object is a pointer
- comparison does not compare pointer addresses, it compares actual values
- every pointer behaves just like a shared_pointer in C++, it keeps the number of references to itself and deletes itself when there are no references left
- when object is returned from function, it is copied, not passed by reference
- when property is accessed it is treated like a reference
