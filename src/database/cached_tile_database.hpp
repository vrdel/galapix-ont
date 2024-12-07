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

#ifndef HEADER_GALAPIX_DATABASE_CACHED_TILE_DATABASE_HPP
#define HEADER_GALAPIX_DATABASE_CACHED_TILE_DATABASE_HPP

#include <memory>

#include "database/tile_database_interface.hpp"

class TileCache;
class Database;

class CachedTileDatabase : public TileDatabaseInterface
{
private:
  Database& m_db;
  std::auto_ptr<TileCache> m_tile_cache;
  std::auto_ptr<TileDatabaseInterface> m_tile_database;

public:
  CachedTileDatabase(Database& db, TileDatabaseInterface* tile_database);
  ~CachedTileDatabase();
  
  bool has_tile(const FileEntry& file_entry, const Vector2i& pos, int scale);
  bool get_tile(const FileEntry& file_entry, int scale, const Vector2i& pos, TileEntry& tile_out);
  void get_tiles(const FileEntry& file_entry, std::vector<TileEntry>& tiles);
  bool get_min_max_scale(const FileEntry& file_entry, int& min_scale_out, int& max_scale_out);

  void store_tile(const FileEntry& file_entry, const Tile& tile);
  void store_tiles(const std::vector<TileEntry>& tiles);

  void delete_tiles(const FileId& fileid);
  void flush_cache();

private:
  CachedTileDatabase(const CachedTileDatabase&);
  CachedTileDatabase& operator=(const CachedTileDatabase&);
};

#endif

/* EOF */
