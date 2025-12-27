#pragma once

#include "event.h"
#include <cstdint>
#include <format>
#include <string>

namespace core {

  struct WindowClosedEvent : Event {
  public:
    std::string to_string() const override { return std::format("WindowClosedEvent!"); }
    WindowClosedEvent() { }
    declare_event_base_functions(window_closed);
  };

  struct WindowResizedEvent : Event {
  public:
    WindowResizedEvent(uint32_t width, uint32_t height) : m_width(width), m_height(height) { }
    uint32_t get_width() { return m_width; }
    uint32_t get_height() { return m_height; }
    std::string to_string() const override { return std::format("WindowResizedEvent ({}, {})", m_width, m_height); }
    declare_event_base_functions(window_resized);
  private:
    uint32_t m_width, m_height;
  };

}
