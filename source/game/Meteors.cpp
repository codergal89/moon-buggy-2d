#include "game/Meteors.hpp"

#include "core/Constants.hpp"
#include "game/Meteor.hpp"

#include <Array.hpp>
#include <Godot.hpp>
#include <PathFollow2D.hpp>
#include <SceneTree.hpp>
#include <String.hpp>
#include <Timer.hpp>

#include <algorithm>

namespace moon_buggy
{

  namespace
  {
    auto constexpr meteor_group_name = "meteors";

    auto constexpr default_live_meteor_limit = 5u;
    auto constexpr minimum_live_meteor_limit = 1u;
    auto constexpr maximum_live_meteor_limit = 16u;

    auto constexpr absolute_meteor_angle_limit = pi / 2 - (15.0 * pi) / 180.0;
    auto constexpr minimum_meteor_angle = static_cast<real_t>(-absolute_meteor_angle_limit);
    auto constexpr maximum_meteor_angle = static_cast<real_t>(absolute_meteor_angle_limit);
    auto constexpr minimum_meteor_speed = static_cast<real_t>(300.0);
    auto constexpr maximum_meteor_speed = static_cast<real_t>(500.0);
  }  // namespace

  auto Meteors::_register_methods() -> void
  {
    godot::register_method("_ready", &Meteors::_ready);
    godot::register_method("on_spawn_timer_expired", &Meteors::on_spawn_timer_expired);
    godot::register_method("start", &Meteors::start);
    godot::register_method("stop", &Meteors::stop);

    godot::register_property("live_meteor_limit",
                             &Meteors::set_live_meteor_limit,
                             &Meteors::get_live_meteor_limit,
                             default_live_meteor_limit,
                             GODOT_METHOD_RPC_MODE_DISABLED,
                             GODOT_PROPERTY_USAGE_DEFAULT,
                             GODOT_PROPERTY_HINT_RANGE,
                             godot::String{"{0},{1},1"}.format(godot::Array::make(minimum_live_meteor_limit, maximum_live_meteor_limit)));
    godot::register_property("meteor_scene", &Meteors::meteor_scene, {});
  }

  auto Meteors::_init() -> void
  {
    rng.instance();
    rng->randomize();

    live_meteor_limit = default_live_meteor_limit;
  }

  auto Meteors::_ready() -> void
  {
    CRASH_COND(!meteor_scene.is_valid());

    spawn_timer = get_typed_node<godot::Timer>("SpawnTimer");
    CRASH_COND(!spawn_timer);

    spawn_location = get_typed_node<godot::PathFollow2D>("SpawnPath/SpawnLocation");
    CRASH_COND(!spawn_location);
  }

  auto Meteors::on_spawn_timer_expired() -> void
  {
    spawn_location->set_unit_offset(rng->randf());
    spawn_meteor();
    spawn_timer->start(rng->randf_range(0.5f, 1.5f));
  }

  auto Meteors::get_live_meteor_limit() const -> unsigned
  {
    return live_meteor_limit;
  }

  auto Meteors::set_live_meteor_limit(unsigned limit) -> void
  {
    static_assert(minimum_live_meteor_limit < maximum_live_meteor_limit);
    live_meteor_limit = std::clamp(limit, minimum_live_meteor_limit, maximum_live_meteor_limit);
  }

  auto Meteors::start() -> void
  {
    spawn_timer->start(0);
  }

  auto Meteors::stop(bool clear) -> void
  {
    spawn_timer->stop();
    if (clear)
    {
      get_tree()->call_group(meteor_group_name, "queue_free");
    }
  }

  auto Meteors::spawn_meteor() -> void
  {
    auto live_meteors = get_tree()->get_nodes_in_group(meteor_group_name);
    if (static_cast<unsigned>(live_meteors.size()) >= live_meteor_limit)
    {
      return;
    }

    auto meteor = cast_to<Meteor>(meteor_scene->instance());
    meteor->set_position(spawn_location->get_position());

    meteor->set("entry_angle", rng->randf_range(minimum_meteor_angle, maximum_meteor_angle));
    meteor->set("entry_speed", rng->randf_range(minimum_meteor_speed, maximum_meteor_speed));
    meteor->add_to_group(meteor_group_name);
    add_child(meteor);
  }

}  // namespace moon_buggy