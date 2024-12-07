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

#ifndef HEADER_GALAPIX_GALAPIX_WORKSPACE_HPP
#define HEADER_GALAPIX_GALAPIX_WORKSPACE_HPP

#include <boost/scoped_ptr.hpp>
#include <set>

#include "galapix/image.hpp"
#include "galapix/image_collection.hpp"
#include "galapix/layouter.hpp"
#include "galapix/selection.hpp"
#include "galapix/spiral_layouter.hpp"
#include "job/thread_message_queue.hpp"
#include "math/quad_tree.hpp"
#include "util/url.hpp"

class Rectf;
class Layouter;

class ImageRequest
{
public:
  URL      url;
  Vector2f pos;
  float    scale;

public:
  ImageRequest(const URL&      url_,
               const Vector2f& pos_,
               float           scale_) :
    url(url_), pos(pos_), scale(scale_)
  {}
};

class Workspace
{
private:
  typedef std::vector<ImageRequest> ImageRequests;

private:
  ImageCollection m_images;

  SelectionPtr m_selection;

  /** Progress of the animation when relayouting, must be set to 0 to
      start animation */
  float     m_progress;

  ThreadMessageQueue<FileEntry> m_file_queue;

  LayouterPtr m_layouter;

public:
  Workspace();
  // ---------------------------------------------

  // Layout hints
  void set_row_width(int w);

  // Image order function
  void sort();
  void sort_reverse();
  void random_shuffle();

  void layout_spiral();
  void layout_tight(float aspect_w, float aspect_h);
  void layout_aspect(float aspect_w, float aspect_h);
  void layout_vertical();
  void layout_random();
  void solve_overlaps();
  void finish_animation();

  // ---------------------------------------------

  // Image Query Functions (dangerous: need to now when Image position
  // changes) -> Return "const Image"? -> can't do breaks due to impl
  ImageCollection get_images(const Rectf& rect) const;
  ImagePtr get_image(const Vector2f& pos) const;

  // ---------------------------------------------
  void add_image(const ImagePtr& image);

  // Selection Commands
  SelectionPtr get_selection() const { return m_selection; }
  bool selection_clicked(const Vector2f& pos) const;
  void select_images(const ImageCollection& images);
  void clear_selection();
  void move_selection(const Vector2f& rel);
  void clear();

  /** Delete all images that aren't in the selection */
  void isolate_selection();

  /** Delete all images in the selection */
  void delete_selection();

  // ---------------------------------------------

  void draw(const Rectf& cliprect, float scale);
  void update(float delta);

  // ---------------------------------------------

  // Debug stuff
  void clear_cache();
  void cache_cleanup();
  void print_info(const Rectf& rect);
  void print_images(const Rectf& rect);

  // ---------------------------------------------
  void load(const std::string& filename);
  void save(std::ostream& out);

  Rectf get_bounding_rect() const;

  bool is_animated() const;

private:
  void start_animation();
  void animation_finished();

private:
  Workspace (const Workspace&);
  Workspace& operator= (const Workspace&);
};

#endif

/* EOF */
