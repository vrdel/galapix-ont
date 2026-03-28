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

#ifndef HEADER_GALAPIX_GALAPIX_OPTIONS_HPP
#define HEADER_GALAPIX_GALAPIX_OPTIONS_HPP

class Options
{
public:
  std::string database;
  std::vector<std::string> patterns;
  int         threads;
  int         spacing;
  int         jpeg_quality;
  std::string sort;
  bool        show_filenames;
  bool        auto_refresh_visible;
  std::vector<std::string> rest;
  std::string title;

  Options() :
    database(),
    patterns(),
    threads(),
    spacing(1),
    jpeg_quality(75),
    sort(),
    show_filenames(false),
    auto_refresh_visible(false),
    title(),
    rest()
  {}
};

#endif

/* EOF */
