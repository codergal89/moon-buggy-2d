#include "core/Level.hpp"
#include "core/LevelDescriptor.hpp"
#include "game/Buggy.hpp"
#include "game/Game.hpp"
#include "game/LevelGenerator.hpp"
#include "game/Map.hpp"
#include "game/Meteor.hpp"
#include "game/ScrollCamera.hpp"
#include "gui/HUD.hpp"
#include "gui/LevelComplete.hpp"
#include "gui/MainMenu.hpp"

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
    godot::register_class<moon_buggy::Meteor>();
    godot::register_class<moon_buggy::ScrollCamera>();
  }
}
