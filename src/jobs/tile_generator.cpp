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

#include "jobs/tile_generator.hpp"

#include <iostream>
#include <sstream>

#include "galapix/tile.hpp"
#include "math/rect.hpp"
#include "math/vector2i.hpp"
#include "plugins/jpeg.hpp"
#include "util/log.hpp"
#include "util/software_surface.hpp"

void
TileGenerator::generate_old(const URL& url,
                            int m_min_scale_in_db, int m_max_scale_in_db,
                            int min_scale, int max_scale,
                            const boost::function<void(Tile)>& callback)
{
  if (true /* verbose */)
  {
    std::ostringstream out;
    out << "TileGenerator::generate(): have ";
    if (m_min_scale_in_db == -1 && m_max_scale_in_db == -1)
    {
      out << "[empty]";
    }
    else
    {
      out << "[" << m_min_scale_in_db << ".." << m_max_scale_in_db << "]";
    }
    out << " generating ["
        << min_scale << ".." << max_scale << "]: " << url << std::endl;
    log_warning << out.str();
  }

  generate(url, min_scale, max_scale, callback);

  if (0)
  {
    log_info << "TileGeneratorThread: processing scales "
             << min_scale << "-" << max_scale << ": " << url << ": done" << std::endl;
  }
}

void
TileGenerator::generate(const URL& url, int min_scale, int max_scale,
                        const boost::function<void(Tile)>& callback)
{
  // Load the image, try to load an already downsized version if possible
  Size original_size;
  SoftwareSurfacePtr surface = load_surface(url, min_scale, &original_size);
  cut_into_tiles(surface, original_size, min_scale, max_scale, callback);
}

SoftwareSurfacePtr
TileGenerator::load_surface(const URL& url, int min_scale, Size* size)
{
  // Load the image
  if (JPEG::filename_is_jpeg(url.str())) // FIXME: filename_is_jpeg() is ugly
  {
    // The JPEG class can only scale down by factor 2,4,8, so we have to
    // limit things (FIXME: is that true? if so, why?)
    int jpeg_scale = Math::min(Math::pow2(min_scale), 8);
              
    if (url.has_stdio_name())
    {
      return JPEG::load_from_file(url.get_stdio_name(), jpeg_scale, size);
    }
    else
    {
      BlobPtr blob = url.get_blob();
      return JPEG::load_from_mem(blob->get_data(), blob->size(), jpeg_scale, size);
    }
  }
  else
  {
    SoftwareSurfacePtr surface = SoftwareSurfaceFactory::current().from_url(url);
    *size = surface->get_size();
    return surface;
  }
}

void
TileGenerator::cut_into_tiles(SoftwareSurfacePtr surface,
                              const Size& original_size,
                              int min_scale, int max_scale,
                              const boost::function<void (Tile)>& callback)
{
  // Scale the image if loading a downsized version was not possible
  // or the downscale wasn't enough
  Size target_size(original_size.width  / Math::pow2(min_scale),
                   original_size.height / Math::pow2(min_scale));

  if (target_size != surface->get_size())
  {
    // JPEG that are downscaled on loading might not match target_size
    // exactly and be one pixel larger
    int x_miss = surface->get_size().width  - target_size.width;
    int y_miss = surface->get_size().height - target_size.height;

    if (0 <= x_miss && x_miss <= 1 &&
        0 <= y_miss && y_miss <= 1)
    {
      log_debug << "image doesn't match target size, ignoring as it is close enough: target=" 
                << target_size << " vs surface=" << surface->get_size() << std::endl;      
    }
    else
    {
      log_debug << "image doesn't match target size, doing scaling: target=" 
                << target_size << " vs surface=" << surface->get_size() << std::endl;
      surface = surface->scale(target_size);
    }
  }

  // Cut the given image into tiles, give created tiles to callback(),
  // surface is expected to be pre-scaled and already at min_scale size
  int scale = min_scale;
  do
  {
    if (scale != min_scale)
    {
      surface = surface->halve();
    }

    for(int y = 0; 256*y < surface->get_height(); ++y)
      for(int x = 0; 256*x < surface->get_width(); ++x)
      {
        SoftwareSurfacePtr croped_surface = surface->crop(Rect(Vector2i(x * 256, y * 256),
                                                               Size(256, 256)));

        callback(Tile(scale, Vector2i(x, y), croped_surface));
      }

    scale += 1;
  }
  while (scale <= max_scale);
}

/* EOF */
