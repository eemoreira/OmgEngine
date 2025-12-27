#include "layer.h"
#include "logger.h"

#include <memory>

namespace core {
  void Layer::queue_transition(std::unique_ptr<Layer> layer) { CORE_LOG_WARN("QUEUE TRANSITION NOT IMPLEMENTED"); }
}
