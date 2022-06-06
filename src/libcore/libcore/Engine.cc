#include <libcore/App.hh>
#include <libcore/Engine.hh>

void run(Engine::App* app) {
  app->start();
  delete app;
}

#if defined(ANDROID)
#include <android_native_app_glue.h>

void android_main(android_app* androidApp) {
  Engine::App* app = createApp(androidApp);
  run(app);
}
#elif !defined(IOS)
int main() {
  Engine::App* app = createApp();
  run(app);
  return 0;
}
#endif