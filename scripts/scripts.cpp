#include "Actors/Buggy.hpp"
#include "Actors/Meteor.hpp"
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
        godot::ClassDB::register_class<mb2d::MeteorSpawner>();
    }

    auto unregister_classes(godot::ModuleInitializationLevel level) -> void
    {
    }
}

extern "C"
{
    GDExtensionBool GDE_EXPORT scripts_library_init(GDExtensionInterface const *interface, GDExtensionClassLibraryPtr library, GDExtensionInitialization *initialization)
    {
        auto init_object = godot::GDExtensionBinding::InitObject{interface, library, initialization};
        init_object.set_minimum_library_initialization_level(godot::MODULE_INITIALIZATION_LEVEL_SCENE);
        init_object.register_initializer(mb2d::register_classes);
        init_object.register_terminator(mb2d::unregister_classes);
        return init_object.init();
    }
}