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

#include "sdl/sdl_viewer.hpp"

#include <iostream>
#include <boost/format.hpp>
#include <boost/thread.hpp>
#include <boost/bind.hpp>

#include "database/file_entry.hpp"
#include "display/framebuffer.hpp"
#include "galapix/database_thread.hpp"
#include "galapix/viewer.hpp"
#include "galapix/viewer_state.hpp"
#include "galapix/workspace.hpp"
#include "math/rgba.hpp"
#include "plugins/png.hpp"
#include "sdl/sdl_framebuffer.hpp"
#include "util/filesystem.hpp"
#include "util/log.hpp"

SDLViewer::SDLViewer(const Size& geometry, bool fullscreen, int  anti_aliasing,
                     Viewer& viewer, const std::string &title) :
  m_geometry(geometry),
  m_fullscreen(fullscreen),
  m_anti_aliasing(anti_aliasing),
  m_quit(false),
  m_viewer(viewer)
{
  if (SDL_Init(SDL_INIT_VIDEO) != 0)
  {
    log_error << "Unable to initialize SDL: " << SDL_GetError() << std::endl;
    exit(1);
  }
  atexit(SDL_Quit);

  SDLFramebuffer::set_video_mode(geometry, fullscreen, anti_aliasing, title);
}

SDLViewer::~SDLViewer()
{
}

void
SDLViewer::process_event(const SDL_Event& event)
{
  const float wheel_zoom_factor = 1.1f;
  const float ctrl_wheel_zoom_factor = 1.25f;
  const float shift_wheel_zoom_factor = 1.5f;

  Uint8* keystate = SDL_GetKeyState(0);

  switch(event.type)
  {
    case SDL_USEREVENT:
      if (event.user.code == 1)
      {
        // New tile arrived
      }
      break;

    case SDL_QUIT:
      std::cout << "Viewer: SDL_QUIT received" << std::endl;
      m_quit = true;
      break;

    case SDL_VIDEOEXPOSE:
      break;

    case SDL_VIDEORESIZE:
      SDLFramebuffer::reshape(Size(event.resize.w, event.resize.h));
      break;

    case SDL_MOUSEMOTION:
      m_viewer.on_mouse_motion(Vector2i(event.motion.x,    event.motion.y),
                               Vector2i(event.motion.xrel, event.motion.yrel));
      break;

      // FIXME: When the mouse is set to left-hand mode, SDL reverses
      // the mouse buttons when a grab is active!
    case SDL_MOUSEBUTTONDOWN:
      switch(event.button.button)
      {
        case SDL_BUTTON_WHEELUP:
        {
          const SDLMod modifiers = SDL_GetModState();
          const float zoom_factor =
            (modifiers & KMOD_SHIFT) ? shift_wheel_zoom_factor :
            (modifiers & KMOD_CTRL)  ? ctrl_wheel_zoom_factor :
                                       wheel_zoom_factor;
          m_viewer.get_state().zoom(zoom_factor, Vector2i(event.button.x, event.button.y));
          break;
        }

        case SDL_BUTTON_WHEELDOWN:
        {
          const SDLMod modifiers = SDL_GetModState();
          const float zoom_factor =
            (modifiers & KMOD_SHIFT) ? shift_wheel_zoom_factor :
            (modifiers & KMOD_CTRL)  ? ctrl_wheel_zoom_factor :
                                       wheel_zoom_factor;
          m_viewer.get_state().zoom(1.0f / zoom_factor, Vector2i(event.button.x, event.button.y));
          break;
        }

        default:
          m_viewer.on_mouse_button_down(Vector2i(event.button.x, event.button.y),
                                        event.button.button);
          break;
      }
      break;

    case SDL_MOUSEBUTTONUP:
      switch(event.button.button)
      {
        default:
          m_viewer.on_mouse_button_up(Vector2i(event.button.x, event.button.y),
                                      event.button.button);
          break;
      }
      break;

    case SDL_KEYDOWN:
      switch(event.key.keysym.sym)
      {
        case SDLK_ESCAPE:
          m_quit = true;
          break;

        /*
        case SDLK_d:
          m_viewer.zoom_to_selection();
          break;
        */

        case SDLK_x:
          m_viewer.zoom_to_selection();
          break;

        /*
        case SDLK_KP_PLUS:
          m_viewer.get_state().zoom(1.25f);
          break;

        case SDLK_KP_MINUS:
          m_viewer.get_state().zoom(1.0f/1.25f);
          break;
        */

        case SDLK_p:
          m_viewer.set_pan_tool();
          break;

        /*
        case SDLK_r:
          m_viewer.set_move_rotate_tool();
          break;
        */

        case SDLK_k:
          m_viewer.cleanup_cache();
          break;

        case SDLK_z:
          m_viewer.set_zoom_tool();
          break;

        case SDLK_m:
          m_viewer.set_move_resize_tool();
          break;

        case SDLK_h:
          m_viewer.zoom_home();
          break;

        case SDLK_n:
          m_viewer.zoom_native_size();
          break;

        case SDLK_F12:
        {
          SoftwareSurfacePtr surface = Framebuffer::screenshot();
          // FIXME: Could do this in a worker thread to avoid pause on screenshotting
          for(int i = 0; ; ++i)
          {
            std::string outfile = (boost::format("/tmp/galapix-screenshot-%04d.png") % i).str();
            if (!Filesystem::exist(outfile.c_str()))
            {
              PNG::save(surface, outfile);
              std::cout << "Screenshot written to " << outfile << std::endl;
              break;
            }
          }
        }
        break;

        case SDLK_i:
          m_viewer.isolate_selection();
          break;

        case SDLK_DELETE:
          m_viewer.delete_selection();
          break;

        case SDLK_y:
          m_viewer.set_grid_tool();
          break;

        case SDLK_0:
          m_viewer.layout_tight();
          break;

        case SDLK_1:
          if (keystate[SDLK_LSHIFT] || keystate[SDLK_RSHIFT])
          {
            m_viewer.sort_reverse_image_list();
          }
          else
          {
            m_viewer.sort_image_list();
          }
          m_viewer.layout_tight();
          break;

        case SDLK_2:
          if (keystate[SDLK_LSHIFT] || keystate[SDLK_RSHIFT])
          {
            m_viewer.sort_reverse_image_list_by_mtime();
          }
          else
          {
            m_viewer.sort_image_list_by_mtime();
          }
          m_viewer.layout_tight();
          break;

        case SDLK_F11:
          SDLFramebuffer::toggle_fullscreen();
          break;

        case SDLK_F2:
          m_viewer.load();
          break;

        case SDLK_F3:
          m_viewer.save();
          break;

        case SDLK_c:
          m_viewer.clear_cache();
          break;

        case SDLK_F5:
          m_viewer.refresh_selection();
          break;

        case SDLK_b:
          if (keystate[SDLK_LSHIFT] || keystate[SDLK_RSHIFT])
          {
            m_viewer.toggle_background_color(true);
          }
          else
          {
            m_viewer.toggle_background_color();
          }
          break;

        case SDLK_SPACE:
          m_viewer.print_images();
          break;

        case SDLK_l:
          m_viewer.print_state();
          break;

        case SDLK_9:
          m_viewer.print_info();
          break;

        default:
          m_viewer.on_key_down(event.key.keysym.sym);
          break;
      }
      break;

    case SDL_KEYUP:
      m_viewer.on_key_up(event.key.keysym.sym);
      break;

    default:
      break;
  }
}

void
SDLViewer::run()
{
  Uint32 ticks = SDL_GetTicks();


  while(!m_quit)
  {
    if (m_viewer.is_active())
    {
      SDL_Event event;

      while (SDL_PollEvent(&event))
      {
        process_event(event);
      }

      Uint32 cticks = SDL_GetTicks();
      float delta = static_cast<float>(cticks - ticks) / 1000.0f;
      ticks = cticks;

      m_viewer.update(delta);
      m_viewer.draw();
    }
    else
    {
      SDL_Event event;
      Uint32 cticks = SDL_GetTicks();
      float delta = static_cast<float>(cticks - ticks) / 1000.0f;
      ticks = cticks;

      SDL_PumpEvents();

      Uint8* keystate = SDL_GetKeyState(NULL);

      if (keystate[SDLK_LEFT] && !(keystate[SDLK_LALT] || keystate[SDLK_RALT]))
      {
        if (keystate[SDLK_LSHIFT] || keystate[SDLK_RSHIFT])
        {
          m_viewer.get_state().set_offset(m_viewer.get_state().get_offset() + Vector2f(+64.0f, 0.0f));
        }
        else if (keystate[SDLK_LCTRL] || keystate[SDLK_RCTRL])
        {
          m_viewer.get_state().set_offset(m_viewer.get_state().get_offset() + Vector2f(+32.0f, 0.0f));
        }
        else
        {
          m_viewer.get_state().set_offset(m_viewer.get_state().get_offset() + Vector2f(+16.0f, 0.0f));
        }
      }
      if (keystate[SDLK_RIGHT] && !(keystate[SDLK_LALT] || keystate[SDLK_RALT]))
      {
        if (keystate[SDLK_LSHIFT] || keystate[SDLK_RSHIFT])
        {
          m_viewer.get_state().set_offset(m_viewer.get_state().get_offset() + Vector2f(-64.0f, 0.0f));
        }
        else if (keystate[SDLK_LCTRL] || keystate[SDLK_RCTRL])
        {
          m_viewer.get_state().set_offset(m_viewer.get_state().get_offset() + Vector2f(-32.0f, 0.0f));
        }
        else
        {
          m_viewer.get_state().set_offset(m_viewer.get_state().get_offset() + Vector2f(-16.0f, 0.0f));
        }
      }

      if (keystate[SDLK_UP] && !(keystate[SDLK_LALT] || keystate[SDLK_RALT]))
      {
        if (keystate[SDLK_LSHIFT] || keystate[SDLK_RSHIFT])
        {
          m_viewer.get_state().set_offset(m_viewer.get_state().get_offset() + Vector2f(0.0f, +64.0f));
        }
        else if (keystate[SDLK_LCTRL] || keystate[SDLK_RCTRL])
        {
          m_viewer.get_state().set_offset(m_viewer.get_state().get_offset() + Vector2f(0.0f, +32.0f));
        }
        else
        {
          m_viewer.get_state().set_offset(m_viewer.get_state().get_offset() + Vector2f(0.0f, +16.0f));
        }
      }

      if (keystate[SDLK_DOWN] && !(keystate[SDLK_LALT] || keystate[SDLK_RALT]))
      {
        if (keystate[SDLK_LSHIFT] || keystate[SDLK_RSHIFT])
        {
          m_viewer.get_state().set_offset(m_viewer.get_state().get_offset() + Vector2f(0.0f, -64.0f));
        }
        else if (keystate[SDLK_LCTRL] || keystate[SDLK_RCTRL])
        {
          m_viewer.get_state().set_offset(m_viewer.get_state().get_offset() + Vector2f(0.0f, -32.0f));
        }
        else
        {
          m_viewer.get_state().set_offset(m_viewer.get_state().get_offset() + Vector2f(0.0f, -16.0f));
        }
      }

      if (keystate[SDLK_w] && !(keystate[SDLK_LALT] || keystate[SDLK_RALT]))
      {
        if (keystate[SDLK_LSHIFT] || keystate[SDLK_RSHIFT])
        {
          m_viewer.get_state().zoom(1.5f);
        }
        else if (keystate[SDLK_LCTRL] || keystate[SDLK_RCTRL])
        {
          m_viewer.get_state().zoom(1.15f);
        }
        else
        {
          m_viewer.get_state().zoom(1.05f);
        }
      }
      if (keystate[SDLK_s] && !(keystate[SDLK_LALT] || keystate[SDLK_RALT]))
      {
        if (keystate[SDLK_LSHIFT] || keystate[SDLK_RSHIFT])
        {
          m_viewer.get_state().zoom(1.0f / 1.5f);
        }
        else if (keystate[SDLK_LCTRL] || keystate[SDLK_RCTRL])
        {
          m_viewer.get_state().zoom(1.0f/1.15f);
        }
        else
        {
          m_viewer.get_state().zoom(1.0f/1.05f);
        }
      }

      while (SDL_PollEvent(&event))
      {
        process_event(event);
      }

      m_viewer.update(delta);

      // FIXME: We should try to detect if we need a redraw and
      // only draw then, else we will redraw on each mouse motion
      m_viewer.draw();
    }

    SDLFramebuffer::flip();

    // std::cout << "." << std::flush;

    // FIXME: Higher values mean less CPU use for drawing and more
    // for loading, which is nice for low-end CPUs, but not so nice
    // for high-end CPUs, maybe make this configurable
    SDL_Delay(10);
  }

  log_info << "done" << std::endl;
}

/* EOF */
