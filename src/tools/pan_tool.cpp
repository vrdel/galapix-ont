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

#include "tools/pan_tool.hpp"
#include <util/log.hpp>

#include "galapix/viewer.hpp"

PanTool::PanTool(Viewer* viewer_)
  : Tool(viewer_),
    trackball_mode(false),
    move_active(false),
    mouse_pos()
{
}

PanTool::~PanTool()
{
}

void
PanTool::move(const Vector2i& pos, const Vector2i& rel)
{
  mouse_pos = pos;

  if (trackball_mode)
  {
    viewer->get_state().move(rel * 1);

  }
  else if (move_active)
  { // FIXME: This is of course wrong, since depending on x/yrel will lead to drift
    // Also we shouldn't use 4x speed, but 1x seems so useless
    viewer->get_state().move(rel * 1);
  }
}

void
PanTool::up(const Vector2i& pos)
{
  mouse_pos   = pos;
  move_active = false;
}

void
PanTool::down(const Vector2i& pos)
{
  mouse_pos   = pos;
  move_active = true;
}

bool
PanTool::get_trackball_mode() const
{
  return trackball_mode;
}

void
PanTool::set_trackball_mode(bool mode)
{
  trackball_mode = mode;
}

/* EOF */
