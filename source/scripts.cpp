#include "Buggy.hpp"
#include "Game.hpp"
#include "HUD.hpp"
#include "Level.hpp"
#include "LevelComplete.hpp"
#include "LevelDescriptor.hpp"
#include "LevelGenerator.hpp"
#include "MainMenu.hpp"
#include "Map.hpp"
#include "ScrollCamera.hpp"

#include <GDNative.hpp>

extern "C"
{
  auto GDN_EXPORT godot_gdnative_init(godot_gdnative_init_options * o) -> void
  {
    godot::Godot::gdnative_init(o);
  }

  auto GDN_EXPORT godot_gdnative_terminate(godot_gdnative_terminate_options * o) -> void
  {
    godot::Godot::gdnative_terminate(o);
  }

  auto GDN_EXPORT godot_nativescript_init(void * handle) -> void
  {
    godot::Godot::nativescript_init(handle);
    godot::register_class<moon_buggy::Buggy>();
    godot::register_class<moon_buggy::Game>();
    godot::register_class<moon_buggy::HUD>();
    godot::register_class<moon_buggy::Level>();
    godot::register_class<moon_buggy::LevelComplete>();
    godot::register_class<moon_buggy::LevelDescriptor>();
    godot::register_class<moon_buggy::LevelGenerator>();
    godot::register_class<moon_buggy::MainMenu>();
    godot::register_class<moon_buggy::Map>();
    godot::register_class<moon_buggy::ScrollCamera>();
  }
}
