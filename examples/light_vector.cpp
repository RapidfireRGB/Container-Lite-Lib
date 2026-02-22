#include "../include/container_lite/light_vector.hpp"
#include <iostream>

int main () {

    // Initialization

    // Initialized by templated type like std::vector:
    container_lite::light_vector<int> foo;

    // Light Vectors are copy assignable:
    foo.push_back(5);
    container_lite::light_vector<int> foo_bar = foo;

    // You can also enter the number of bytes the vector will take up.
    // Note that it will be at least 8 bytes larger since 'size_' variable is 8 bytes.
    container_lite::light_vector<int, 512> bar;    // ~0.5kb vector
    std::cout << "'bar' size: " << sizeof(bar) << "\n\n";

    // Public Member functions

    // Adding Elements

    // push_back() inserts an element at the end:
    foo.push_back(10);
    std::cout << "'foo' back element: " << foo.back() << "\n";

    // Increment operator (+=) is functionally the same as push_back():
    foo += 2;
    std::cout << "'foo' back element: " << foo.back() << "\n";

    // insert() parameters:
    // size_t pos   - Index position before which the element will be inserted
    // T value      - Element to be inserted
    // size_t count - Optional number of times to insert element. Defaults to 1.
    foo.insert(1, 20, 2);
    std::cout << "'foo' first after insertion: " << foo.front() << "\n";
    std::cout << "'foo' second after insertion: " << foo[1] << "\n\n";

    // Removing Elements

    // pop_back() removes element at the end:
    foo.pop_back();
    std::cout << "'foo' back after removal: " << foo.back() << "\n";

    // Decrement operator (-=) is functionally the same as pop_back():
    foo -= 10;
    std::cout << "'foo' back after decrement: " << foo.back() << "\n";

    // erase() parameter:
    // size_t index - Index position at which the element will be removed.
    foo.erase(0);
    std::cout << "'foo' first after erasure: " << foo.front() << "\n";

    // clear() calls destructors of all elements to delete them:
    foo.clear();
    std::cout << "'foo' current size after clearing: " << foo.size() << "\n";

    // All comparison operators are also functional.
    // Note that different sized vectors cannot be compared:
    if (foo != bar) {/*...*/}       // Invalid
    if (foo == foo_bar) {           // Valid
        std::cout << "'foo' and 'foo_bar' are currently equivalent.\n\n";
    }

    // Element Search

    // Light Vectors can be indexed:
    foo.push_back(5);
    std::cout << "'foo' at index 0: " << foo[0] << "\n";

    // at() is functionally the same:
    foo.push_back(5);
    std::cout << "'foo' at index 1: " << foo[1] << "\n\n";

    // Utility

    // empty() checks if a Light Vector has no elements:
    if (foo.empty()) {/*...*/}

    // full() checks if a Light Vector is at capacity:
    if (foo.full()) {/*...*/}

    // contains() checks if an element is present:
    if (foo.contains(5)) {/*...*/}

    // index_of() returns the first index position of an element:
    std::cout << "'5' at index: " << foo.index_of(5) << "\n";

    // size() and capacity() return the size/capacity respectively:
    std::cout << "'foo' current size: " << foo.size() << "\n";
    std::cout << "'foo_bar' max number of elements: " << foo_bar.capacity() << "\n";

    return 0;
}
