#pragma once

#include "logger.h"
#include <functional>
#include <string>

namespace core {

  enum struct EventType {
    none = 0,
    window_closed,
    window_resized,
    key_pressed,
    key_released,
    mouse_button_pressed,
    mouse_button_released,
    mouse_moved,
    mouse_scrolled,
  };

#define declare_event_base_functions(type)                                                                             \
  static EventType get_static_type() { return EventType::type; }                                                       \
  virtual EventType get_event_type() const override { return get_static_type(); }                                      \
  virtual const char *get_name() const override { return #type; }

  struct Event {
  public:
    bool handled = false;

    virtual ~Event() { }
    virtual EventType get_event_type() const = 0;
    virtual const char *get_name() const = 0;
    virtual std::string to_string() const = 0;
  };

  struct EventDispatcher {

  public:
    EventDispatcher(Event &event) : m_event(event) { }

    template <typename T>
    bool dispatch(std::function<bool(T &)> callback_handler) {
      if (m_event.get_event_type() == T::get_static_type() && !m_event.handled) {
        if (callback_handler(static_cast<T &>(m_event))) {
          m_event.handled = true;
        }
        return true;
      }

      return false;
    }

  private:
    Event &m_event;
  };

}
