NOTE TO TA:
Due to the hardware-centric nature of the classes we’ve implemented so far, in order to run our acceptance tests and verify that our software is working correctly, the actual hardware (and motors/wheels) of our device is required.
After speaking with the Prof. about this, he suggested that we write a note to you, and arrange a time so that we could demonstrate our tests with the hardware, so you can properly mark it.

To arrange a time to meet for testing our software, please email: jnoble26@uwo.ca (Joshua Noble), as he will be holding onto the RPi + associated hardware.

Compiling Instructions (hardware is also required for testing purposes as per note above):

Test Motor Class: 
g++ -o test motor_test.cpp Motor.cpp -lwiringPi
sudo ./test

Test WheelController Class:
g++ -o test wheel_control_test.cpp Motor.cpp MotorController.cpp WheelController.cpp -lwiringPi
sudo ./test

Test Path Class:
g++ -o test path_test.cpp Path.cpp
sudo ./test