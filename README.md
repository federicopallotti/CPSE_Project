# CPSE_Project
Project for Cyber-Physical Software Engineering
By Albert Walser and Federico Pallotti

For this project we built a baisc 3D Scanner using a Portenta H7 Board, an Arduino Nano 33 BLE board and a z-ranger ToF sensor to mesure the distance of the object placed on a rotating platform, and moving the distance sensor vertically by a discrete distance at every complete revolution of the platform.

We then implemented a mathematical model that given the distance measured, the angle of rotation, and the height of the sensor, will reproduce the pointcloud of the scanned object in a 3D reference system.

<img width="797" alt="Schermata 2022-05-19 alle 16 54 37" src="https://user-images.githubusercontent.com/77103965/169326554-f6a74291-b39b-4460-bfe1-7f2cac489666.png">
