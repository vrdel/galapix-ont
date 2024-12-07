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

#ifndef HEADER_GALAPIX_DATABASE_FILE_ENTRY_STORE_STATEMENT_HPP
#define HEADER_GALAPIX_DATABASE_FILE_ENTRY_STORE_STATEMENT_HPP

#include <iostream>
#include <assert.h>

class FileEntryStoreStatement
{
private:
  SQLiteConnection& m_db;
  SQLiteStatement   m_stmt;

public:
  FileEntryStoreStatement(SQLiteConnection& db) :
    m_db(db),
    m_stmt(db, "INSERT OR REPLACE INTO files (url, size, mtime, width, height) VALUES (?1, ?2, ?3, ?4, ?5);")
  {}

  void operator()(FileEntry file_entry)
  {
    if (file_entry.get_fileid())
    {
      std::cout << "FileEntryStoreStatement: Warning file_entry already has fileid: " << file_entry.get_fileid() << std::endl;
      assert(!"Should never happen");
    }

    m_stmt.bind_text(1, file_entry.get_url().str());
    m_stmt.bind_int (2, file_entry.get_size());
    m_stmt.bind_int (3, file_entry.get_mtime());
    m_stmt.bind_int (4, file_entry.get_width());
    m_stmt.bind_int (5, file_entry.get_height());

    m_stmt.execute();
  
    file_entry.set_fileid(FileId(sqlite3_last_insert_rowid(m_db.get_db())));
  }

private:
  FileEntryStoreStatement(const FileEntryStoreStatement&);
  FileEntryStoreStatement& operator=(const FileEntryStoreStatement&);
};

#endif

/* EOF */
