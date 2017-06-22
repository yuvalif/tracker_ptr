# tracker_ptr
The tracker_ptr is used to wrap around a pointer which is managed elsewhere. The tracker_ptr pass itself to the pointer that it is warpping, so that it could be notified when the pointer is deleted or should not be tracked anymore.

The class does not guarantee any thread safety.

All implementation reside in the header file, the cpp file is only for testing/example.

To compile the test, use:
g++ -Wall -std=c++11 -o tracker_ptr_test tracker_ptr_test.cpp
