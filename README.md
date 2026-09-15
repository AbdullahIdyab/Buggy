# Autonomous Maze-Solving Buggy

An Arduino-based autonomous robot capable of navigating through mazes using real-time spatial mapping and obstacle avoidance. 

![Buggy Image](docs/IMG_3062.jpeg)

## Hardware Components
* **Microcontroller:** Arduino (Uno/Nano)
* **Motor Driver:** Dual H-Bridge (e.g., L298N) controlling two DC motors
* **Sensors:** HC-SR04 Ultrasonic Distance Sensor
* **Actuators:** Micro Servo (pans the ultrasonic sensor 180 degrees)

## How it Works
The buggy uses a "Left-Wall Following" algorithm to navigate mazes independently. 
1. The servo-mounted ultrasonic sensor scans the environment (0°, 90°, 180°).
2. The `mazeMode()` function evaluates the distances:
   * **Path clear on the left (>20cm):** The buggy prioritizes turning left to follow the wall.
   * **Obstacle ahead (<20cm):** The buggy stops and turns right to find an open path.
   * **Otherwise:** The buggy continues moving straight.
3. Motor movements are handled via PWM signals for smooth acceleration and turning.
