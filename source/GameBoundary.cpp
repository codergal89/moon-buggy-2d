#include "GameBoundary.hpp"

#include <CollisionShape2D.hpp>
#include <Godot.hpp>
#include <Viewport.hpp>

namespace moon_buggy
{

  auto GameBoundary::_register_methods() -> void
  {
    godot::register_method("_ready", &GameBoundary::_ready);
  }

  auto GameBoundary::_init() -> void
  {
  }

  auto GameBoundary::_ready() -> void
  {
    left_edge = get_typed_node<godot::CollisionShape2D>("LeftEdge");
    right_edge = get_typed_node<godot::CollisionShape2D>("RightEdge");

    auto viewport_size = get_viewport()->get_size();

    left_edge->set_position({-viewport_size.x / 2, .0f});
    right_edge->set_position({viewport_size.x / 2, .0f});
  }

}  // namespace moon_buggy