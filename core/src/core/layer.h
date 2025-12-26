#pragma once

#include "event.h"
#include <memory>

namespace core {
  struct Layer {
  public:
    virtual ~Layer() = default;
    virtual void on_update() {}
    virtual void on_render() {}
    virtual void on_event(Event &event) {}
    template <std::derived_from<Layer> T, typename... Args>
    void transition_to(Args &&...args) {
      queue_transition(std::move(std::make_unique<T>(std::forward<Args>(args)...)));
    }

  private:
    void queue_transition(std::unique_ptr<Layer> layer);
  };
}
