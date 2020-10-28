# CarND-Controls-PID
Self-Driving Car Engineer Nanodegree Program

---
## PID Tuning
From [Wiki](https://en.wikipedia.org/wiki/PID_controller)

Parameter|Rise time|Overshoot|Settling time|Steady-state error|Stability
---------|---------|---------|---------|---------|---------
K_p|	Decrease	|Increase|	Small change|	Decrease	|Degrade
K_i| Decrease	|Increase	|Increase	|Eliminate	|Degrade
K_d|	Minor change|	Decrease|	Decrease	|No effect in theory	|Improve if K_d small

### Twiddle logic
I am using a modified twiddle logic in this PID tuner. To achieve run twiddle only once every time gets the error readings, a state machine is used inside of for loop.
There are three states, INIT: Initialize best err and update PID values by adding dp values, INCREMENT: Increment dp if a better error found, vis versa, DECREMENT: if fail second times in a row, decrement dp with 0.9


However, I was first thinking to run this logic every time I got a cte from the simulator. This fails because twiddle and PID controller would not react fast enough and result in full left and right turns every step like following showed:

![swiggle](./images/run_every_step.gif)

As the result, more errors need to be added together to feed into the twiddle. At beginning of the tuning, I used the average of the error to find a range of PID values and reach the values faster. Later when I have a value, I will use the sum of all errors to feed into the twiddle. 
### Tuning Process 
Starting from  P=0,I=0,D=0 and stepping dp 1,1,1. After about 20 iteration of twiddle, I was able get PID value of P:2.5 I:0 D:10.

I was able to observe a lot of overshoot and oscillation at a slight left turn. Like below 

![2.3-0.01-10](./images/2.3-0.01-10.gif)

Tuning down P and I to 0.5 and 0.0001 gives a better result. D is at 5.  However, after this I observed during a sharp turn, the car would oscillate. I decide to increase D to decrease overshoot and increase P a bit to reduce rise time. Then I run twiddle with the initial value of
`P= 0.531 I = 0.0001 D =9.6` At this point I value should be good, I only run P and D with dp value of [1,1] 

![0.5-0.0001-9.6](./images/0.5-0.0001-9.6.gif)

After 7 Iteration I get following value


Kp|Ki|Kd
---|---|---
0.8389|1.9e-5|10.6961

![0.8-1e9-10](./images/0.8-0.0009-10.gif)

### Conclusion
In summary, the two values below would all work. I feel the twiddle would prefer more aggressive P and D to achieve faster error corrections. The lower P value should be more pleasant to sit in.

Kp|Ki|Kd
---|---|---
0.8389|1.9e-5|10.6961
0.5|0.0001|9.6
## Dependencies

* cmake >= 3.5
 * All OSes: [click here for installation instructions](https://cmake.org/install/)
* make >= 4.1(mac, linux), 3.81(Windows)
  * Linux: make is installed by default on most Linux distros
  * Mac: [install Xcode command line tools to get make](https://developer.apple.com/xcode/features/)
  * Windows: [Click here for installation instructions](http://gnuwin32.sourceforge.net/packages/make.htm)
* gcc/g++ >= 5.4
  * Linux: gcc / g++ is installed by default on most Linux distros
  * Mac: same deal as make - [install Xcode command line tools]((https://developer.apple.com/xcode/features/)
  * Windows: recommend using [MinGW](http://www.mingw.org/)
* [uWebSockets](https://github.com/uWebSockets/uWebSockets)
  * Run either `./install-mac.sh` or `./install-ubuntu.sh`.
  * If you install from source, checkout to commit `e94b6e1`, i.e.
    ```
    git clone https://github.com/uWebSockets/uWebSockets 
    cd uWebSockets
    git checkout e94b6e1
    ```
    Some function signatures have changed in v0.14.x. See [this PR](https://github.com/udacity/CarND-MPC-Project/pull/3) for more details.
* Simulator. You can download these from the [project intro page](https://github.com/udacity/self-driving-car-sim/releases) in the classroom.

Fellow students have put together a guide to Windows set-up for the project [here](https://s3-us-west-1.amazonaws.com/udacity-selfdrivingcar/files/Kidnapped_Vehicle_Windows_Setup.pdf) if the environment you have set up for the Sensor Fusion projects does not work for this project. There's also an experimental patch for windows in this [PR](https://github.com/udacity/CarND-PID-Control-Project/pull/3).

## Basic Build Instructions

1. Clone this repo.
2. Make a build directory: `mkdir build && cd build`
3. Compile: `cmake .. && make`
4. Run it: `./pid`. 

Tips for setting up your environment can be found [here](https://classroom.udacity.com/nanodegrees/nd013/parts/40f38239-66b6-46ec-ae68-03afd8a601c8/modules/0949fca6-b379-42af-a919-ee50aa304e6a/lessons/f758c44c-5e40-4e01-93b5-1a82aa4e044f/concepts/23d376c7-0195-4276-bdf0-e02f1f3c665d)

