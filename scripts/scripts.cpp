#include "Actors/Buggy.hpp"
#include "Actors/Meteor.hpp"
#include "Main.hpp"
#include "Menus/MainMenu.hpp"
#include "MeteorSpawner.hpp"

#include <gdextension_interface.h>

#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/core/defs.hpp>

#include <godot_cpp/godot.hpp>

namespace mb2d
{
  auto register_classes(godot::ModuleInitializationLevel level) -> void
  {
    if (level != godot::ModuleInitializationLevel::MODULE_INITIALIZATION_LEVEL_SCENE)
    {
      return;
    }

    godot::ClassDB::register_class<mb2d::Buggy>();
    godot::ClassDB::register_class<mb2d::Meteor>();
    godot::ClassDB::register_class<mb2d::Main>();
    godot::ClassDB::register_class<mb2d::MainMenu>();
    godot::ClassDB::register_class<mb2d::MeteorSpawner>();
  }

  auto unregister_classes(godot::ModuleInitializationLevel) -> void
  {
  }
}  // namespace mb2d

extern "C"
{
  GDExtensionBool GDE_EXPORT scripts_library_init(GDExtensionInterfaceGetProcAddress get_proc_address,
                                                  GDExtensionClassLibraryPtr library,
                                                  GDExtensionInitialization * initialization)
  {
    auto init_object = godot::GDExtensionBinding::InitObject{get_proc_address, library, initialization};
    init_object.set_minimum_library_initialization_level(godot::MODULE_INITIALIZATION_LEVEL_SCENE);
    init_object.register_initializer(mb2d::register_classes);
    init_object.register_terminator(mb2d::unregister_classes);
    return init_object.init();
  }
}