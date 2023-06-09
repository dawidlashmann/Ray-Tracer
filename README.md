# Ray-Tracer
### Ray Tracer based on geometrical optics

## About
This project was inspired by `Peter Sherley's` book *[Ray Tracing in One Weekend](https://raytracing.github.io/books/RayTracingInOneWeekend.html)*

Despite following the book, it has been a huge learning curve for me, mainly beacuse it was my first big project.

Later on I modified the code by adding multithreading and other various methods.

I am planning on advancing this project in the future, with the two other books made by this author.

## Motivation
Starting this project I planned on bettering myself in c++.

After finishing it I can say that I have not only achieved that, but also I gained a lot of knowledge about physics, optics and photographic theory.

The reason I'm willing to continue this project is my extreme iterest in this topic and I am sure it will teach me much more.

It has allowed me to connect my two passions - programming and 3D graphics (i.e. Blender3D)
## Using the code 
* To change the parameters of a render, you have to look at the beggining of the `main` function
* The parameters not only concern the final image properties (i.e. `resolution`, `sampling`, etc.), but also the camera's (i.e. `aperture`, `focus`, etc.)
* To render it, you have to compile all the files first, and then run the output file
* The `main.cpp` file has most of the needed comments, although rest of the files are without them, so reading these files might be rather challenging
* If the code is not using the desired number of avaiable threads, I advise to check the `.config` files first


## Example Render
The slight grainy feeling is due small number of `sampling` and `depth` I choose, to reduce rendering duration.
Please note that some `spheres` are moving, thus the motion blur.
![Final_img](https://user-images.githubusercontent.com/115370220/232602650-bc3929de-74b2-4e0b-9238-18294c7ad55c.jpg)

If you used my code and found any bugs, please let me know any way possible!
