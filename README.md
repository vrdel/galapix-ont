galapix - SDL Image Viewer
==========================

This fork keeps `galapix` focused on a single SDL viewer build with a simpler
feature set than the original project.

It is intended for browsing large local image sets with a tile cache, tight
layout, explicit startup sorting, configurable layout spacing, and optional
filename overlays.

Current Scope
-------------

The current tree is intentionally trimmed down:

- SDL viewer only
- CMake build only
- local image loading only
- local cache/database only

Removed from this fork:

- GTK frontend
- SpaceNavigator / `spnav`
- archive and remote URL loading
- Zoomify
- extra legacy layout modes such as auto/random/spiral/vertical
- viewer controls for shuffle, rotate, grid toggling, gamma, contrast, and brightness

Supported Image Loading
-----------------------

The current SDL build supports local files through:

- JPEG loader
- PNG loader
- ImageMagick-backed formats

The plugin stack for archive, remote, RAW, XCF, KRA, DDS, SVG, and related
legacy loaders was removed from the active build.

Build
-----

The project now uses CMake.

Dependencies are resolved through:

- Boost (`filesystem`, `thread`)
- GLEW
- OpenGL
- libjpeg
- libpng
- SQLite3
- SDL 1.2 via `pkg-config`
- ImageMagick++ via `pkg-config`
- libexif via `pkg-config`

Configure and build:

```bash
cmake -S . -B cmake-build -DCMAKE_BUILD_TYPE=Release
cmake --build cmake-build -j4
```

The binary is:

```bash
./cmake-build/galapix.sdl
```

Basic Usage
-----------

Open images:

```bash
./cmake-build/galapix.sdl view path/to/image.jpg
```

Open multiple files in the exact input order:

```bash
./cmake-build/galapix.sdl view img3.jpg img1.jpg img2.jpg
```

Show filenames and increase spacing:

```bash
./cmake-build/galapix.sdl view --show-filenames --spacing 2 path/to/images
```

Sort on startup:

```bash
./cmake-build/galapix.sdl view --sort name path/to/images
./cmake-build/galapix.sdl view --sort name-reverse path/to/images
./cmake-build/galapix.sdl view --sort mtime path/to/images
./cmake-build/galapix.sdl view --sort mtime-reverse path/to/images
```

Prepare cache tiles:

```bash
./cmake-build/galapix.sdl prepare --threads 8 --jpeg-quality 90 path/to/images
```

Commands
--------

The current CLI supports:

- `view`
- `prepare`
- `filegen`
- `check`
- `list`
- `cleanup`
- `merge`

`thumbgen` was removed as a public command. `prepare` is the supported tile
generation entrypoint.

View Options
------------

`view` currently supports these relevant options:

- `--spacing N`
  `1` keeps the base spacing, `2` doubles it, `3` triples it, and so on
- `--sort MODE`
  `name`, `name-reverse`, `mtime`, or `mtime-reverse`
- `--show-filenames`
  shows a filename label above visible images
- `--auto-refresh-visible`
  checks visible file-backed images for mtime changes and reloads them
- `-g`, `--geometry WxH`
- `-f`, `--fullscreen`
- `-p`, `--pattern GLOB`
- `-F`, `--files-from FILE`
- `-d`, `--database FILE`

Prepare Options
---------------

`prepare` currently supports:

- `--threads N`
  number of worker threads used during cache generation
- `--jpeg-quality N`
  JPEG quality for generated cache tiles, from `1` to `100`
- `-d`, `--database FILE`

Current Defaults
----------------

- default layout is tight
- default startup order preserves the input order you passed in
- no startup sorting is applied unless `--sort` is provided
- cache tile JPEG quality defaults to `75`
- layout base spacing is `40`, then multiplied by `--spacing`

Runtime Shortcuts
-----------------

The most relevant SDL shortcuts in this fork are:

- `0` tight layout
- `1` tight layout sorted by filename
- `Shift+1` tight layout reverse-sorted by filename
- `2` tight layout sorted by mtime
- `Shift+2` tight layout reverse-sorted by mtime
- `F11` toggle fullscreen
- `F5` refresh selection
- `c` clear cache

Notes
-----

- The filename overlay is only shown when `--show-filenames` is passed.
- The default view is no longer serpentine; each row is laid out left to right.
- This fork is a practical cleanup of the old Google Code project, not a
  feature-complete preservation of every historical backend and UI path.
