# A (at the moment) quite bad raytracer

![alt text](results/statue.bmp "example image")

It does work though. Built in pure C.  

The goal with this project was to build a raytracer from scratch, without following a guide, only referencing the minimum required math.

### Ideas:
Tree model for scenes (see branch scene_tree)
- each leaf box has 12 object triangles.
- arbritrary number of node boxes from box head to leaf
- ensures minimum level of collision checks for large, high triangle scenes

~~~~
                box
              /     \          split down x-axis
           box       box
          /   \     /   \         split down z-axis
        box   box  box  box
         |     |    |    |           (would split down y-axis, then loop back to x for additional nodes)
        tri   tri  tri  tri
        tri   tri  tri  tri
        tri   tri  tri  tri
        tri   tri  tri  tri
        tri   tri  tri  tri
        tri   tri  tri  tri
        tri   tri  tri  tri
        tri   tri  tri  tri
        tri   tri  tri  tri
        tri   tri  tri  tri
        tri   tri  tri  tri
        tri   tri  tri  tri
~~~~

Probable triangles array for each pixel
- make array of size bounces
- first "exploratory" ray populates array with triangles it visited along its path
- next rays will try those triangles first (depending on current bounce)
- if ray doesn't intersect with candidate triangle
  - will progress upwards throught scene tree to find lowest box it intersects with
  - then start traversing down from there
