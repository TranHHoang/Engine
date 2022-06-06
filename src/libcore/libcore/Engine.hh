#pragma once
#include <any>

namespace Engine {
class App;
}

extern Engine::App* createApp(std::any data = {});
void run(Engine::App* app);

#if !defined(ANDROID) && !defined(IOS)
int main();
#endif