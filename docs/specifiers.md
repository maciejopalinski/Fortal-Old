# Access Specifiers
## Class
- public - accessible from any package
- <default> - accessible from current package

## Class Member / Method
- public - accessible from any package
- <default> - accessible from current package
- protected - accessible from derived class, current package
- private - accessible from current class

# Non-Access Specifiers
## Class
- final - class cannot be inherited
- abstract - class cannot be used to create instance, must be inherited

## Class Member
- final / const - cannot be modified after initialization
- static - belongs to a class rather than an instance
- transient - non-static member is skipped when serializing the instance, cannot be used on static member

## Class Method
- abstract - only used in abstract class, doesn't have a body, has to be overridden
- final - cannot be overridden
