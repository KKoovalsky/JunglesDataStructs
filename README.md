# Jungles data structs

Documentation of data structures defined within this library. C++17 is necessary.

## jungles::make_and_enable_member_as_isr_handler

Enable a member function as an ISR handler

Typical approach for such scenario would be like that for such a class:

```
class some_class
{
  public:
    void isr_handler()
    {
    }
};
```

And when you want to call it from a specific ISR like this one (from `extern "C"` context):

```
void RTC_IRQ_Handler()
{
    // some hardware specific code goes here 

    extern void rtc_isr_handler();
    rtc_isr_handler();

    // some hardware specific code goes here 
}
```

you must create an object of type `some_class` and call it from the `rtc_isr_handler`:

```
some_class some_object;

extern "C" rtc_isr_handler()
{
    some_object.isr_handler();
}

```

In the file [member_isr_enable.hpp](inc/member_isr_enable.hpp) there is a function:

```
template <auto &PointerToIsrHandler, typename ObjectType, auto MemberFunction, typename... ConstructorParams>
ObjectType &make_and_enable_member_as_isr_handler(ConstructorParams &&... params);
```

which does the same but with some metaprogramming included. To use it you must firstly define the `IRQ_Handler` like
that:


```
void RTC_IRQ_Handler()
{
    // some hardware specific code goes here 

    extern void (*rtc_isr_handler_ptr)();
    (*rtc_isr_handler_ptr)();

    // some hardware specific code goes here 
}
```
 
From some `cpp` file:

```
void (*rtc_isr_handler_ptr)();
auto some_object = make_and_enable_member_as_isr_handler<rtc_isr_handler, some_class, &some_class::isr_handler>();
```

