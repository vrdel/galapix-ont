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

#ifndef HEADER_GALAPIX_DATABASE_TILE_ENTRY_GET_MIN_MAX_SCALE_STATEMENT_HPP
#define HEADER_GALAPIX_DATABASE_TILE_ENTRY_GET_MIN_MAX_SCALE_STATEMENT_HPP

#include <assert.h>

class TileEntryGetMinMaxScaleStatement
{
private:
  SQLiteStatement m_stmt;

public:
  TileEntryGetMinMaxScaleStatement(SQLiteConnection& db) :
    m_stmt(db, "SELECT MIN(scale),MAX(scale) FROM tiles WHERE fileid = ?1;")
  {}

  bool operator()(const FileEntry& file_entry, int& min_scale_out, int& max_scale_out)
  {
    m_stmt.bind_int64(1, file_entry.get_fileid().get_id());
    SQLiteReader reader = m_stmt.execute_query();

    if (reader.next())
    {
      if (reader.is_null(0) || reader.is_null(1))
      {
        // no tiles in the database
        return false;
      }
      else
      {
        min_scale_out = reader.get_int(0);
        max_scale_out = reader.get_int(1);
        return true;
      }
    }
    else
    {
      assert(!"TileEntryGetMinMaxScaleStatement: never reached");
      return false;
    }
  }

private:
  TileEntryGetMinMaxScaleStatement(const TileEntryGetMinMaxScaleStatement&);
  TileEntryGetMinMaxScaleStatement& operator=(const TileEntryGetMinMaxScaleStatement&);
};

#endif

/* EOF */
