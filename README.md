galapix - A Zoomable Image Viewer
=================================

galapix is an image viewer that allows you to directly zoom into large
collection of tens of thousand of images from tiny thumbnails down to
the images original size fluently with very short load times and no
loss of interactivity. It accomplishes this by keeping a tile cache
which stores not only thumbnails for one size, but thumbnails of many
different sizes. This thumbnail cache requires at maximum 1.5x times
as much storage as the images themselves, but as tiles are generated
on demand often far less.

galapix supports local JPEG, PNG and ImageMagick-backed image loading
in the current SDL-focused build.

**Actually this is fork from Google code project with small fixes.**

URL: [http://code.google.com/p/galapix/]

Build
-----

The project now uses CMake.

Configure and build:

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build -j4
```

Run:

```bash
./build/galapix.sdl view path/to/image.jpg
```
