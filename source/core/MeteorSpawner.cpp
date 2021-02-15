#include "core/MeteorSpawner.hpp"

#include "core/Constants.hpp"
#include "core/Meteor.hpp"

#include <Array.hpp>
#include <Godot.hpp>
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

  auto MeteorSpawner::_register_methods() -> void
  {
    godot::register_method("_ready", &MeteorSpawner::_ready);
    godot::register_method("on_spawn_timer_expired", &MeteorSpawner::on_spawn_timer_expired);
    godot::register_method("start", &MeteorSpawner::start);
    godot::register_method("stop", &MeteorSpawner::stop);

    godot::register_property("live_meteor_limit",
                             &MeteorSpawner::set_live_meteor_limit,
                             &MeteorSpawner::get_live_meteor_limit,
                             default_live_meteor_limit,
                             GODOT_METHOD_RPC_MODE_DISABLED,
                             GODOT_PROPERTY_USAGE_DEFAULT,
                             GODOT_PROPERTY_HINT_RANGE,
                             godot::String{"{0},{1},1"}.format(godot::Array::make(minimum_live_meteor_limit, maximum_live_meteor_limit)));
    godot::register_property("meteor_scene", &MeteorSpawner::meteor_scene, {});
  }

  auto MeteorSpawner::_init() -> void
  {
    rng.instance();
    rng->randomize();

    spawn_timer = godot::Timer::_new();
    spawn_timer->set_name("MeteorSpawnTimer");
    spawn_timer->connect("timeout", this, "on_spawn_timer_expired");
    add_child(spawn_timer);

    live_meteor_limit = default_live_meteor_limit;
  }

  auto MeteorSpawner::_ready() -> void
  {
    CRASH_COND(!meteor_scene.is_valid());
  }

  auto MeteorSpawner::on_spawn_timer_expired() -> void
  {
    set_unit_offset(rng->randf());
    spawn_meteor();
    spawn_timer->start(rng->randf_range(0.5f, 1.5f));
  }

  auto MeteorSpawner::get_live_meteor_limit() const -> unsigned
  {
    return live_meteor_limit;
  }

  auto MeteorSpawner::set_live_meteor_limit(unsigned limit) -> void
  {
    static_assert(minimum_live_meteor_limit < maximum_live_meteor_limit);
    live_meteor_limit = std::clamp(limit, minimum_live_meteor_limit, maximum_live_meteor_limit);
  }

  auto MeteorSpawner::start() -> void
  {
    spawn_timer->start(0);
  }

  auto MeteorSpawner::stop(bool clear) -> void
  {
    spawn_timer->stop();
    if (clear)
    {
      get_tree()->call_group(meteor_group_name, "queue_free");
    }
  }

  auto MeteorSpawner::spawn_meteor() -> void
  {
    auto live_meteors = get_tree()->get_nodes_in_group(meteor_group_name);
    if (static_cast<unsigned>(live_meteors.size()) >= live_meteor_limit)
    {
      return;
    }

    auto meteor = cast_to<Meteor>(meteor_scene->instance());
    meteor->set_position(get_position());

    meteor->set("entry_angle", rng->randf_range(minimum_meteor_angle, maximum_meteor_angle));
    meteor->set("entry_speed", rng->randf_range(minimum_meteor_speed, maximum_meteor_speed));
    meteor->add_to_group(meteor_group_name);
    add_child(meteor);
  }

}  // namespace moon_buggy