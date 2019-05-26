# raycaster

A simple c++ raycaster (DOOM/Wolfenstein) style

I still need to clean up the code a bit, but the map generation and raycasting all works. 
I also kind of wanted to add textures and maybe the ability to make your own lightsources but this all comes after code cleanup :)
I'll probably post a little blurb about how the math works as well.

Here's what it looks like:

![Picture 1](img/picture_1.png?raw=true "Picture 1")
![Picture 2](img/picture_2.png?raw=true "Picture 2")


## TODO (in no particular order)
* [ ] Add explanation to README
* [ ] Add texture loading
* [ ] Clean code and segment
* [ ] Add minimap to show 2D representation
* [x] Shading (I realized I don't need light sources? It's a 2D map so I can just calculate the depth)
* [ ] Load maps from text file
* [ ] Multiple lightsources
