#include <libcore/Engine.hh>

#ifdef ANDROID
#include <android_native_app_glue.h>

void android_main(android_app* androidApp) {
  Engine::App* app = createApp(androidApp);
  app->start();
  delete app;
}
#else
int main() {
  Engine::App* app = createApp();
  app->start();
  delete app;
  return 0;
}
#endif