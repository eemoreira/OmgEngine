#include "app_layer.h"
#include "core/application.h"
#include "core/logger.h"

int main() {

  Logger::init();

  core::ApplicationSpec app_spec;
  app_spec.window_spec.width = 1920;
  app_spec.window_spec.height = 1080;
  app_spec.window_spec.title = "teste";

  core::Application app(app_spec);
  app.push_layer<AppLayer>();
  app.run();

  CORE_LOG_TRACE("good bye :)");
  return 0;
}
