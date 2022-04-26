#pragma once
#include <libcore/Engine.hh>

int main() {
  Engine::App* app = createApp();
  app->start();
  delete app;
  return 0;
}