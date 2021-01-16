#include "game/Meteor.hpp"

#include "core/Constants.hpp"

#include <Physics2DDirectBodyState.hpp>
#include <Vector2.hpp>
namespace moon_buggy
{

  auto Meteor::on_screen_exited() -> void
  {
    queue_free();
  }

  auto Meteor::_integrate_forces(godot::Physics2DDirectBodyState * body_state) -> void
  {
    auto velocity = body_state->get_linear_velocity().normalized();
    auto angle = velocity.angle_to(direction_down);
    set_rotation(-angle);
  }

}  // namespace moon_buggy
