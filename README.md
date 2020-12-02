# Earth_Surface_Rendering_From_Hyperspectral_Measurement

This project is the final project for the course IDIG4002 - Introduction to Computer Graphic Fundementals and Applications

This is implemented by:
- Thong Nguyen
- Thao Nguyen 
- Maria Jose Rueda Montes

NTNU, Gjovik, Norway


1. Setup 

1.1 Hyperspectral data processing

1.2 Rendering 

- Install visual studio
- Clone this reposition:  git clone https://github.com/ThongNguyen551/Earth_Surface_Rendering_From_Hyperspectral_Measurement.git
- Download dependencies (glm, glew, glfw) and 3D object from this link https://bit.ly/39BnFlR, then place all downloaded thinsg in the solution directory Earth_Surface_Rendering_From_Hyperspectral_Measurement/

2. Running

- Run the main.cpp by visual studio

2.1 Change view postion (camera position) 

The keyboard is processed to conduct these changes:
- Move forward: W
- Move backward: S
- Move to the right: D
- Move to the left: A
- Move up: U
- Move down: P

2.2 Change the light color

The light color is defined with 3 values of R G B ranging from 0.0f to 1.0f
- Increase the R/G/B value of color light: KEY_UP + R / KEY_UP + G / KEY_UP + B respectively. 
- Decrease the R/G/B value of color light: KEY_DOWN + R / KEY_DOWN + G / KEY_DOWN + B respectively. 

2.3 Change the light position

The light position is defined with 3 values of X Y Z in the world space ranging from -100.0f to 100.0f
- Increase the X/Y/Z value of light position: KEY_UP + X / KEY_UP + Y / KEY_UP + Z respectively. 
- Decrease the X/Y/Z value of light posotion: KEY_DOWN + X / KEY_DOWN + Y / KEY_DOWN + Z respectively. 

References 

1. Process hyperspectral data

2. 
