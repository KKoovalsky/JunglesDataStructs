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

---

##  jungles::message_sink

This template is intended to be used in situtations when one can put only single elements to an object and wants to
group the elements in messages. Such scenario is found e.g. in interrupts where single bytes are received and those
bytes can form a message. The user can specify the message terminators (e.g. CR, LF or \0). The message sink also allows
to specify exceptional sequences which are messages which doesn't need terminators.

The sink uses statically allocated internal buffer whose size is specified at instantiation time.

Beware that the method `put_element_and_get_message()` is not reentrant nor thread-safe. Moreover you must copy the
obtained range for further use before the method is called next time.

Example usage:

```
constexpr std::array terminators{'\r', '\n', '\0'};
constexpr auto exceptional_sequence1{jungles::utils::to_array(">")};
constexpr auto exceptional_sequence2{jungles::utils::to_array("HELLO")};

// Deduces that sink contains chars.
auto char_sink{make_message_sink<256>(terminators, exceptional_sequence1, exceptional_sequence2)};

...
std::string terminated_msg{"HI MAN"};
for(auto c : terminated_msg)
    assert(char_sink.put_element_and_get_message(c).has_value());
auto v = char_sink.put_element_and_get_message('\0');
assert(!v.has_value());
auto [beg, end] = *v;
std::cout << std::string(beg, end); // Prints: HI MAN (without the terminator '\0')

...
assert(char_sink.put_element_ang_get_message('H').has_value());
assert(char_sink.put_element_ang_get_message('E').has_value());
assert(char_sink.put_element_ang_get_message('L').has_value());
assert(char_sink.put_element_ang_get_message('L').has_value());
// No terminator needed for an exceptional sequence
assert(!char_sink.put_element_ang_get_message('O').has_value());

...

std::string exceptional_sequence_inside{"HI HELLO CIAO"};
for(auto c : exceptional_sequence_inside)
    // Exceptional sequences can be only standalone messages.
    assert(char_sink.put_element_and_get_message(c).has_value());

```
