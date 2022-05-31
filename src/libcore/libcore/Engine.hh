#pragma once
#include <libcore/App.hh>

extern Engine::App* createApp(std::any data = {});
#ifndef ANDROID
int main();
#endif