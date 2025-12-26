#include "event.h"
#include <cstdint>
#include <format>
#include <string>

namespace core {

  struct KeyEvent : Event {
  public:
    int32_t get_key_code() { return m_keycode; }

  protected:
    KeyEvent(int32_t keycode) : m_keycode(keycode) { }
    int32_t m_keycode;
  };

  struct KeyPressedEvent : KeyEvent {
  public:
    KeyPressedEvent(int32_t keycode, bool is_repeating) : KeyEvent(keycode), m_is_repeating(is_repeating) { }
    bool is_repeating() { return m_is_repeating; }
    std::string to_string() const override { return std::format("KeyPressedEvent {} (repeating={})", m_keycode, m_is_repeating); }

    declare_event_base_functions(key_pressed);

  private:
    bool m_is_repeating;
  };

  struct KeyReleasedEvent : KeyEvent {
  public:
    KeyReleasedEvent(int32_t keycode) : KeyEvent(keycode) { }
    std::string to_string() const override { return std::format("KeyReleasedEvent {}", m_keycode); }

    declare_event_base_functions(key_released);
  };

  struct MouseMovedEvent : Event {
  public:
    MouseMovedEvent(double x, double y) : m_x(x), m_y(y) { }

    double get_x() { return m_x; }
    double get_y() { return m_y; }
    std::string to_string() const override { return std::format("MouseMovedEvent ({}, {})", m_x, m_y); }

    declare_event_base_functions(mouse_moved);

  private:
    double m_x, m_y;
  };

  struct MouseScrollEvent : Event {
  public:
    MouseScrollEvent(double x_offset, double y_offset) : m_x_offset(x_offset), m_y_offset(y_offset) { }

    double get_x_offset() { return m_x_offset; }
    double get_y_offset() { return m_y_offset; }
    std::string to_string() const override { return std::format("MouseScrollEvent ({}, {})", m_x_offset, m_y_offset); }

    declare_event_base_functions(mouse_scrolled);

  private:
    double m_x_offset, m_y_offset;
  };

  struct MouseButtonEvent : Event {
  public:
    inline int32_t get_button() { return m_button; }

  protected:
    MouseButtonEvent(int32_t button) : m_button(button) { }
    int32_t m_button;
  };

  struct MouseButtonPressedEvent : MouseButtonEvent {
  public:
    MouseButtonPressedEvent(int32_t button) : MouseButtonEvent(button) { }
    std::string to_string() const override { return std::format("MouseButtonPressedEvent {}", m_button); }
    declare_event_base_functions(mouse_button_pressed);
  };

  struct MouseButtonReleasedEvent : MouseButtonEvent {
  public:
    MouseButtonReleasedEvent(int32_t button) : MouseButtonEvent(button) { }
    std::string to_string() const override { return std::format("MouseButtonReleasedEvent {}", m_button); }
    declare_event_base_functions(mouse_button_released);
  };

}
