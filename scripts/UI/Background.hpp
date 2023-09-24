#ifndef MB2D_SCRIPTS_UI_BACKGROUND_HPP
#define MB2D_SCRIPTS_UI_BACKGROUND_HPP

#include "Helpers/DontWarn.hpp"
#include "Helpers/PropertiesGetSet.hpp"

#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/variant/vector2.hpp>

#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/classes/global_constants.hpp>
#include <godot_cpp/classes/parallax_background.hpp>
#include <godot_cpp/classes/parallax_layer.hpp>

namespace mb2d
{

  struct Background
      : godot::ParallaxBackground
      , helpers::EasyProperties<Background>
  {
    ENABLE_EASY_PROPERTIES();
    DONT_WARN(GDCLASS(Background, godot::ParallaxBackground))

    auto static _bind_methods() -> void
    {
      godot::ClassDB::bind_method(godot::D_METHOD("start", "speed"), &Background::start);
      godot::ClassDB::bind_method(godot::D_METHOD("stop"), &Background::stop);

      add_property("speed", &Background::speed, 0, 500, 1.f);
      add_property("scrolling", &Background::scrolling, godot::PROPERTY_HINT_NONE);
    }

    auto _notification(int notification) -> void
    {
      bool in_game = !godot::Engine::get_singleton()->is_editor_hint();

      switch (notification)
      {
      case NOTIFICATION_READY:
        ready(in_game);
        set_process(true);
        break;
      case NOTIFICATION_PROCESS:
        process(get_process_delta_time(), in_game);
        break;
      }
    }

    /**
     * @brief Start auto-scrolling at the given speed.
     *
     * @param speed The auto-scrolling speed.
     */
    auto start(float speed) -> void
    {
      this->speed = speed;
      scrolling = true;
    }

    /**
     * @brief Stop auto-scrolling.
     */
    auto stop() -> void
    {
      scrolling = false;
    }

  private:
    auto ready([[maybe_unused]] bool in_game) -> void
    {
      space_layer = get_node<godot::ParallaxLayer>("%Space");
      stars_layer = get_node<godot::ParallaxLayer>("%Stars");
    }

    auto process(double delta, bool in_game) -> void
    {
      if (!in_game)
      {
        return;
      }

      if (scrolling)
      {
        auto offset = get_scroll_offset();
        set_scroll_offset(offset + godot::Vector2{speed, 0.0} * delta);
      }
    }

    godot::ParallaxLayer * space_layer;
    godot::ParallaxLayer * stars_layer;
    float speed{};
    bool scrolling{};
  };

}  // namespace mb2d

#endif