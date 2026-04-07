Messing around with [this](https://raytracing.github.io/books/RayTracingInOneWeekend.html) tutorial because I'm bored.

# Notes for myself once I inevitably forget half of this
## Use
Run `make` to build the project, and run `make image` to render the image, which is stored at `img.ppm`.
Or run `build/render > <filepath>.ppm` to store it in an arbitrary location.

For other formats, use `make jpg` or `make png`, which convert the ppm file to the respective format using ffmpeg.

## Configuration
Put stuff in `shapes.txt` to have it show up in the render.

### Parameters

#### Camera Config
| Parameter | Function | Default|
| --- | --- | --- |
| `image-size <width> <height>` | Set the size of the resultant image (in pixels). | 640x360 (16:9) |
| `look-from <x> <y> <z>` | Set the camera position. | (0, 0, 0) |
| `look-at <x> <y> <z>` | Set the focal point. | (0, 0, -1) |
| `up-direction` | The direction that the camera treats as "up" in its image. | (0, 1, 0) |
| `vertical-fov` | The vertical FOV of the camera (in degrees). | 90° |
| `samples-per-pixel` | The number of random samples the camera will take per pixel for antialiasing. | 10 |
| `max-depth` | Maximum times a ray will bounce off objects. | 10 |

#### Shapes
Note that the only shape currently available is a sphere.
| Parameter | Function | Default|
| --- | --- | --- |
| `position <x> <y> <z>` | The center of the sphere. | (0, 0, 0) |
| `size <radius>` | The radius of the sphere. | 0 (i.e. the sphere will be too small to render) |
| `material <type> ...` | Set the material of the sphere: <ul> <li>`matt <r> <g> <b>`</li> <li>`shiny <r> <g> <b>`</li> <li>`transparent <refractive index>`</li> </ul>| matt 128 128 128 |

#### Example
```
# camera config
image-size 640 360

look-from 0 0.25 0
look-at 0 0 -1
up-direction 0 1 0

vertical-fov 90

# shape configs

# ground
sphere:
    position 0 -100.5 -1.0
    size 100
    material matt 200 200 0

# other shapes
sphere:
    position -1 0 -1
    size .5
    material transparent 1.5

sphere:
    position 1 0 -1
    size .5
    material shiny 200 150 50

sphere:
    position 0 0 -1.2
    size 0.5
    material matt 25 50 125
```
