#include "Buggy.hpp"

#include <Camera2D.hpp>
#include <Godot.hpp>
#include <Input.hpp>
#include <KinematicCollision2D.hpp>
#include <ProjectSettings.hpp>
#include <Vector2.hpp>

#include <algorithm>

namespace moon_buggy
{

  auto Buggy::_register_methods() -> void
  {
    godot::register_method("_ready", &Buggy::_ready);
    godot::register_method("_physics_process", &Buggy::_physics_process);
    godot::register_property("speed_limit", &Buggy::speed_limit, default_speed_limit);
  }

  auto Buggy::_init() -> void
  {
    gravity = godot::ProjectSettings::get_singleton()->get_setting("physics/2d/default_gravity_vector");
    gravity *= static_cast<real_t>(godot::ProjectSettings::get_singleton()->get_setting("physics/2d/default_gravity"));

    speed_limit = default_speed_limit;
  }

  auto Buggy::_ready() -> void
  {
    viewport = get_viewport();

    camera = get_typed_node<godot::Camera2D>("Camera");
    camera->set("limit_top", 0);
    camera->set("limit_bottom", viewport->get_size().y);
    camera->make_current();
  }

  auto Buggy::_physics_process(float delta) -> void
  {
    handle_gravity(delta);
    handle_input();
    move_and_slide(velocity, godot::Vector2{0.f, -1.f});
  }

  auto Buggy::handle_gravity(real_t delta) -> void
  {
    velocity += gravity * delta;
    if (is_on_floor())
    {
      velocity.y = 0.f;
    }
  }

  auto Buggy::handle_input() -> void
  {
    auto input = godot::Input::get_singleton();

    if (input->is_action_pressed("ui_left") && is_on_floor())
    {
      accelerate(10.f);
    }
    else if (input->is_action_pressed("ui_right") && is_on_floor())
    {
      accelerate(-10.f);
    }
    else
    {
      decelerate(2.f);
    }
  }

  auto Buggy::accelerate(real_t difference) -> void
  {
    velocity.x = std::clamp(velocity.x - difference, -speed_limit, speed_limit);
  }

  auto Buggy::decelerate(real_t difference) -> void
  {
    if (velocity.x < 0.f)
    {
      velocity.x = std::clamp(velocity.x + difference, -speed_limit, 0.f);
    }
    else if (velocity.x > 0.f)
    {
      velocity.x = std::clamp(velocity.x - difference, 0.f, speed_limit);
    }
  }

}  // namespace moon_buggy