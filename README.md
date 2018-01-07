# tracker_ptr
The `tracker_ptr` is used to wrap around a pointer which is managed elsewhere. The `tracker_ptr` pass itself to the pointer that it is warpping, so that it could be notified when the pointer is deleted or should not be tracked anymore.

The class does not guarantee any thread safety.

All implementation reside in the header file, the cpp file is only for testing/example.

To compile the test, use:
`g++ -Wall -std=c++11 -o tracker_ptr_test tracker_ptr_test.cpp`

The concept of `tracker_ptr` is very similar to `std::weak_ptr` (see: http://en.cppreference.com/w/cpp/memory/weak_ptr) or `boost::weak_ptr` (see: http://www.boost.org/doc/libs/1_66_0/libs/smart_ptr/doc/html/smart_ptr.html#weak_ptr). However, it has some differences:
* The actual owner of the pointer does not have to be an `std::shared_ptr` (or `boost::shared_ptr`). It could be any object that implemnts the required interface from `trackable_ptr_concept` (or inherit from it). This is useful in cases where there is no shared ownership of the pointer, hence a shared pointer is not needed for other reasons
* There is no need to convert `tracker_ptr` to anything to gain temporary ownership, or extend the lifetime of the pointer. This feature of `weak_ptr` may not be needed for some usecases
