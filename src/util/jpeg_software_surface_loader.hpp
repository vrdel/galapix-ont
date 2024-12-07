/*
**  Galapix - an image viewer for large image collections
**  Copyright (C) 2009 Ingo Ruhnke <grumbel@gmx.de>
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

#ifndef HEADER_GALAPIX_UTIL_JPEG_SOFTWARE_SURFACE_LOADER_HPP
#define HEADER_GALAPIX_UTIL_JPEG_SOFTWARE_SURFACE_LOADER_HPP

#include "plugins/jpeg.hpp"

class JPEGSoftwareSurfaceLoader : public SoftwareSurfaceLoader
{
public:
  JPEGSoftwareSurfaceLoader()
  {}

  std::string get_name() const
  {
    return "jpeg";
  }

  void register_loader(SoftwareSurfaceFactory& factory) const
  {
    factory.register_by_magick(this, 0, "\xff\xd8");
    factory.register_by_mime_type(this, "image/jpeg");
    factory.register_by_extension(this, "jpeg");
    factory.register_by_extension(this, "jpg");
  }

  bool supports_from_file() const { return true; }
  bool supports_from_mem()  const { return true; }

  SoftwareSurfacePtr from_file(const std::string& filename) const
  {
    return JPEG::load_from_file(filename);
  }

  SoftwareSurfacePtr from_mem(uint8_t* data, int len) const
  {
    return JPEG::load_from_mem(data, len);
  }
  
private:
  JPEGSoftwareSurfaceLoader(const JPEGSoftwareSurfaceLoader&);
  JPEGSoftwareSurfaceLoader& operator=(const JPEGSoftwareSurfaceLoader&);
};

#endif

/* EOF */
