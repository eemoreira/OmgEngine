#pragma once

#include "event.h"
#include <memory>

namespace core {
  struct Layer {
  public:
    virtual ~Layer() = default;
    virtual void on_update(float time_step) { }
    virtual void on_render() { }
    virtual void on_event(Event &event) { }
    template <std::derived_from<Layer> T, typename... Args>
    void transition_to(Args &&...args) {
      transition(std::move(std::make_unique<T>(std::forward<Args>(args)...)));
    }
    virtual void suspend() { }
    virtual void unsuspend() { }
    virtual bool is_suspended() { return false; }

  private:
    void queue_transition(std::unique_ptr<Layer> layer);
  };
}
