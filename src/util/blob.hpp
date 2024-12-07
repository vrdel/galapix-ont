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

#ifndef HEADER_GALAPIX_UTIL_BLOB_HPP
#define HEADER_GALAPIX_UTIL_BLOB_HPP

#include <boost/smart_ptr.hpp>
#include <vector>
#include <stdint.h>

class Blob;

typedef boost::shared_ptr<Blob> BlobPtr;

class Blob
{
private:
  boost::scoped_array<uint8_t> m_data;
  int m_len;

private:
  Blob(const std::vector<uint8_t>& data); 
  Blob(const void* data, int len);
  Blob(int len);

public:
  int size() const;
  uint8_t* get_data() const;

  std::string str() const;
  
  void write_to_file(const std::string& filename);

  /** Append data to the given Blob, only possible for non-wrap Blobs
      (will invalidade the pointer returned by get_data() */
  void append(const void* data, int len);

public:
  static BlobPtr create(int len);

  static BlobPtr from_file(const std::string& filename);
 
  /** Copy the given data into a Blob object */
  static BlobPtr copy(const void* data, int len);
  static BlobPtr copy(const std::vector<uint8_t>& data);
};

#endif

/* EOF */
