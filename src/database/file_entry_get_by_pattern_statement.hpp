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

#ifndef HEADER_GALAPIX_DATABASE_FILE_ENTRY_GET_BY_PATTERN_STATEMENT_HPP
#define HEADER_GALAPIX_DATABASE_FILE_ENTRY_GET_BY_PATTERN_STATEMENT_HPP

class FileEntryGetByPatternStatement
{
private:
  SQLiteStatement m_stmt;

public:
  FileEntryGetByPatternStatement(SQLiteConnection& db) :
    m_stmt(db, "SELECT * FROM files WHERE url GLOB ?1;")
  {}

  void operator()(const std::string& pattern, std::vector<FileEntry>& entries_out)
  {
    m_stmt.bind_text(1, pattern);
    SQLiteReader reader = m_stmt.execute_query();

    while (reader.next())  
    {
      // FIXME: Use macro definitions instead of numeric constants
      FileEntry entry = FileEntry::create(FileId(reader.get_int(0)),  // fileid
                                          URL::from_string(reader.get_text(1)),  // url
                                          reader.get_int(2),
                                          reader.get_int(3),
                                          reader.get_int(4), // width
                                          reader.get_int(5), // height
                                          reader.get_int(6));
      entries_out.push_back(entry);
    } 
  }

private:
  FileEntryGetByPatternStatement(const FileEntryGetByPatternStatement&);
  FileEntryGetByPatternStatement& operator=(const FileEntryGetByPatternStatement&);
};

#endif

/* EOF */
