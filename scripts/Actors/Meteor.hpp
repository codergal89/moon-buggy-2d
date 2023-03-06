#ifndef MB2D_SCRIPTS_ACTORS_METEORITE_HPP
#define MB2D_SCRIPTS_ACTORS_METEORITE_HPP

#include <godot_cpp/classes/animated_sprite2d.hpp>
#include <godot_cpp/classes/collision_shape2d.hpp>
#include <godot_cpp/classes/rigid_body2d.hpp>
#include <godot_cpp/classes/visible_on_screen_notifier2d.hpp>
#include <godot_cpp/classes/engine.hpp>

#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/core/error_macros.hpp>

namespace mb2d
{

    struct Meteor : godot::RigidBody2D
    {
        auto static constexpr cStartingVelocity = 100.0;

        GDCLASS(Meteor, godot::RigidBody2D)

        static auto _bind_methods() -> void
        {
        }

        auto _ready() -> void override
        {
            collision_shape = get_node<godot::CollisionShape2D>("%CollisionShape");
            flying_sprite = get_node<godot::AnimatedSprite2D>("%FlyingSprite");
            impacted_sprite = get_node<godot::AnimatedSprite2D>("%ImpactedSprite");
            visibility_notifier = get_node<godot::VisibleOnScreenNotifier2D>("%VisibilityNotifier");

            ERR_FAIL_NULL(collision_shape);
            ERR_FAIL_NULL(flying_sprite);
            ERR_FAIL_NULL(impacted_sprite);
            ERR_FAIL_NULL(visibility_notifier);

            if (!godot::Engine::get_singleton()->is_editor_hint())
            {
                ready_in_game();
            }
        }

        auto ready_in_game() -> void
        {
            flying_sprite->play();
            impacted_sprite->set_visible(false);

            set_linear_velocity(godot::Vector2{1.0, 1.0} * cStartingVelocity);
        }

    private:
        godot::CollisionShape2D *collision_shape;
        godot::AnimatedSprite2D *flying_sprite;
        godot::AnimatedSprite2D *impacted_sprite;
        godot::VisibleOnScreenNotifier2D *visibility_notifier;
    };
}

#endif