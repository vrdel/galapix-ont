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

galapix can also be used to view gigapixel sized images. The Zoomify
ImageProperties.xml file format is support.

galapix supports a wide varity of image formats including JPEG, PNG,
XCF (Gimp), KRA (Krita) and all stuff supported by ImageMagick.

**Actually this is fork from Google code project with small fixes.**

URL: [http://code.google.com/p/galapix/]
