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
| `samples-per-pixel` | The number of random samples the camera will take per pixel for antialiasing. (This will increase the image quality but also the rendering time.) | 10 |
| `max-depth` | Maximum times a ray will bounce off objects. | 10 |

#### Shapes
Note that the only shape currently available is a sphere.
| Parameter | Function | Default|
| --- | --- | --- |
| `position <x> <y> <z>` | The center of the sphere. | (0, 0, 0) |
| `moving-to <x> <y> <z>` | Use this to add motion  blurs: the sphere will appear to be moving from its original position to this new position. | Same as `position` (i.e. no motion blur) |
| `size <radius>` | The radius of the sphere. | 0 (i.e. the sphere will be too small to render) |
| `material <type> ...` | Set the material of the sphere: <ul> <li>`matt <r> <g> <b>`</li> <li>`shiny <r> <g> <b>`</li> <li>`transparent <refractive index>`</li> </ul>| matt 128 128 128 |

### Example
Check `shapes.txt` for an example.
