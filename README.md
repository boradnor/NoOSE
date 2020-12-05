# NoOSE
No OS Experiment, run your program without OS the easy way

Using Linux as a donor OS the idea is to replace init.o by your own program. It implies booting and kernel are in place. The kernel creates a Hardware Abstraction Layer HAL which is available in the replaced init.o.
This project is about making hardware resources available in "your program" and strip down the donor Linux OS to a bare minimum to boot and install and run "your program".

The concept is free to use.
