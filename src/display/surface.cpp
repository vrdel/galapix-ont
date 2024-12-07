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

#include "display/surface.hpp"

#include "display/framebuffer.hpp"
#include "math/rect.hpp"

class SurfaceImpl
{
public:
  TexturePtr texture;
  Rectf   uv;
  Size    size;
  
  SurfaceImpl(const TexturePtr& texture_, const Rectf& uv_, const Size& size_)
    : texture(texture_),
      uv(uv_),
      size(size_)
  {
    //std::cout << uv << std::endl;
  }

  SurfaceImpl(const SoftwareSurfacePtr& src, const Rect& srcrect) :
    texture(),
    uv(),
    size()
  {
    assert(src);

    texture = Texture::create(src, srcrect);
    
    uv = Rectf(Vector2f(0, 0), srcrect.get_size());

    size = Size(srcrect.get_size());
  }
  
  ~SurfaceImpl()
  {
  }

  void draw(const Rectf& srcrect, const Rectf& dstrect)
  {
    if (texture)
    {
      texture->bind();
      glEnable(GL_BLEND);
      glEnable(GL_TEXTURE_RECTANGLE_ARB);
      glColor3f(1.0f, 1.0f, 1.0f);       

      glBegin(GL_QUADS);
      glTexCoord2f(srcrect.left, srcrect.top);
      glVertex2f(dstrect.left, dstrect.top);

      glTexCoord2f(srcrect.right, srcrect.top);
      glVertex2f(dstrect.right, dstrect.top);

      glTexCoord2f(srcrect.right, srcrect.bottom);
      glVertex2f(dstrect.right, dstrect.bottom);

      glTexCoord2f(srcrect.left, srcrect.bottom);
      glVertex2f(dstrect.left, dstrect.bottom);
      glEnd();
    }    
  }

  void draw(const Rectf& rect)
  {
    if (texture)
    {
      texture->bind();
      glEnable(GL_BLEND);
      glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
      glEnable(GL_TEXTURE_RECTANGLE_ARB);
      glColor3f(1.0f, 1.0f, 1.0f);       

      glBegin(GL_QUADS);
      glTexCoord2f(uv.left, uv.top);
      glVertex2f(rect.left, rect.top);

      glTexCoord2f(uv.right, uv.top);
      glVertex2f(rect.right, rect.top);

      glTexCoord2f(uv.right, uv.bottom);
      glVertex2f(rect.right, rect.bottom);

      glTexCoord2f(uv.left, uv.bottom);
      glVertex2f(rect.left, rect.bottom);
      glEnd();
    }
  }

  void draw(const Vector2f& pos)
  {
    draw(Rectf(pos, size));
  }
};

SurfacePtr
Surface::create(const SoftwareSurfacePtr& src, const Rect& srcrect)
{
  return SurfacePtr(new Surface(src, srcrect));
}

SurfacePtr
Surface::create(const SoftwareSurfacePtr& src)
{
  return SurfacePtr(new Surface(src));
}

Surface::Surface(const SoftwareSurfacePtr& src) :
  impl(new SurfaceImpl(src, Rect(Vector2i(0, 0), src->get_size())))
{
}

Surface::Surface(const SoftwareSurfacePtr& src, const Rect& srcrect)
  : impl(new SurfaceImpl(src, srcrect))
{
}

void
Surface::draw(const Vector2f& pos)
{
  if (impl.get())
    impl->draw(pos);
}

void
Surface::draw(const Rectf& srcrect, const Rectf& dstrect)
{
  if (impl.get())
    impl->draw(srcrect, dstrect);  
}

void
Surface::draw(const Rectf& rect)
{
  if (impl.get())
    impl->draw(rect);
}

int
Surface::get_width() const 
{
  if (impl.get())
    return impl->size.width; 
  else
    return 0;
}

int
Surface::get_height() const
{
  if (impl.get())
    return impl->size.height; 
  else
    return 0;
}

Size
Surface::get_size() const
{
  if (impl.get())
    return impl->size;
  else
    return Size();
}

/* EOF */
