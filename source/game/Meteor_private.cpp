#include "game/Meteor.hpp"

namespace moon_buggy
{

  auto Meteor::on_screen_exited() -> void
  {
    queue_free();
  }

}  // namespace moon_buggy

