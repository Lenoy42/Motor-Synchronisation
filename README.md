# Motor-Synchronisation
This is a project that aims to control two dc motors such that they are in phase with each other.

This project is actually rrequired as a part of an ornithopter build. According to the design, the wings of the ornithopter are independantly controlled by two separate DC motors. Thus for proper flight, it is essential that the motors are in sync with each other.

The main challenge of this project is the form factor of the components. As an ornithopter should be able to have a sustained flight, all the components that have been used in the build have to be as small and light as possible.

For keeping the motors in sync, it can be done by using motors that have encoders with them. However, as we searched for such motors, we found that motors with in - built encoders are either too slow (around 100 RPM) or too heavy. What would be an ideal motor is something that can dish out around 500 - 600 RPM under a small voltage, like 3.3 - 5V.

Although such motors were availiable, they were hard to acquire and also had no inbuilt encoders. Thus, we had to design our own encoders. What we did here was design an incremental optical encoder for each motor, and by trying to match the incremental counter values, we could synchronise the motors.

We opted for optical encoders rather than Hall - Effect encoders, as firstly, it is hard to encorporate a Hall - effect encoder in such a small scale. Moreover, optical encoders are also more accurate than its magnetic counterparts.

For this build, we designed the encoder using IR led source and an IR detector, which went to an LM358 comparator module to give an output. The encoder wheel that we designed had 12 holes on it; more holes means better accuracy, but at smaller scales, it can be problematic because of the use of the IR detector.

The schematic of the encoder circuit is also uploaded along with the code for the syncronization. I have used an Arduino Mega 2560 for this project, but it would work with any Arduino with at least two interrupt pins.

We are aiming for a more advanced circuit, which includes incorporating an accelerometer and a gyroscope in the ornithopter such that it will be able to sustain flight and even rebalance itself in case of any unintended roll.
