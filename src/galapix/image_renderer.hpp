/*
**  Galapix - an image viewer for large image collections
**  Copyright (C) 2008 Ingo Ruhnke <grumbel@gmx.de>
**
**  This program is free software: you can redistribute it and/or modify
**  it under the terms of the GNU General Public License as published by
**  the Free Software Foundation, either version 3 of the License, or
**  (at your option) any later version.
**
**  This program is distributed in the hope that it will be useful,
**  but WITHOUT ANY WARRANTY; without even the implied warranty of
**  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
**  GNU General Public License for more details.
**
**  You should have received a copy of the GNU General Public License
**  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef HEADER_GALAPIX_GALAPIX_IMAGE_RENDERER_HPP
#define HEADER_GALAPIX_GALAPIX_IMAGE_RENDERER_HPP

#include <boost/shared_ptr.hpp>

class Image;
class ImageTileCache;
class Rect;
class Rectf;
class Vector2f;

class ImageRenderer
{
private:
  Image& m_image;
  boost::shared_ptr<ImageTileCache> m_cache;

public:
  ImageRenderer(Image& image, boost::shared_ptr<ImageTileCache> cache);

  /** Return true if something was drawn to the screen, false when
   *  image was outside the cliprect */
  bool draw(const Rectf& cliprect, float zoom);

private:
  Vector2f get_vertex(int x, int y, float zoom) const;
  void draw_tile(int x, int y, int tiledb_scale, float zoom);
  void draw_tiles(const Rect& rect, int tiledb_scale, float zoom);

private:
  ImageRenderer(const ImageRenderer&);
  ImageRenderer& operator=(const ImageRenderer&);
};

#endif

/* EOF */
