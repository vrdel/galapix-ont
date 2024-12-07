/*
**  Galapix - an image viewer for large image collections
**  Copyright (C) 2010 Ingo Ruhnke <grumbel@gmx.de>
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

#include "plugins/seven_zip.hpp"

#include <iostream>

int main(int argc, char** argv)
{
  try
  {
    if (argc == 2)
    {
      const std::vector<std::string>& files = SevenZip::get_filenames(argv[1]);
      for(std::vector<std::string>::const_iterator i = files.begin(); i != files.end(); ++i)
      {
        std::cout << "File: '" << *i << "'" << std::endl;
      }
    }
    else if (argc == 3)
    {
      BlobPtr blob = SevenZip::get_file(argv[1], argv[2]);
      blob->write_to_file("/tmp/out.file");
      std::cout << "Writting /tmp/out.file" << std::endl;
    }
    else 
    {
      std::cout << "Usage: " << argv[0] << " ZIPFILE" << std::endl;
      std::cout << "       " << argv[0] << " ZIPFILE FILETOEXTRACT" << std::endl;
    }
  }
  catch(std::exception& err) 
  {
    std::cout << "Error: " << err.what() << std::endl;
  }
  return 0;
}

/* EOF */
