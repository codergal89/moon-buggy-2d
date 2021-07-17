#include "core/Space.hpp"

#include "core/Theming.hpp"

#include <Defs.hpp>
#include <Dictionary.hpp>
#include <Godot.hpp>
#include <GodotGlobal.hpp>
#include <ImageTexture.hpp>
#include <Resource.hpp>
#include <ResourceLoader.hpp>
#include <String.hpp>
#include <Texture.hpp>
#include <TextureRect.hpp>
#include <Vector2.hpp>

namespace moon_buggy
{

  namespace
  {
    auto constexpr default_autoscrolling_speed = real_t{100.f};
    auto constexpr default_enable_autoscrolling = false;

    auto constexpr space_background_path_pattern = "res:///assets/backgrounds/space_{0}.png";
    auto constexpr stars_background_path_pattern = "res:///assets/backgrounds/stars_{0}.png";

    auto load_image(godot::String pattern, godot::String theme) -> godot::Ref<godot::Image>
    {
      auto path = pattern.format(godot::Array::make(theme));
      auto loader = godot::ResourceLoader::get_singleton();
      auto const image = loader->load(path);
      if (!image.is_valid())
      {
        ERR_PRINT("Failed to find space background!");
        CRASH_NOW();
      }
      return image;
    }
  }  // namespace

  auto Space::_register_methods() -> void
  {
    godot::register_method("_process", &Space::_process);
    godot::register_method("_ready", &Space::_ready);
    godot::register_method("select_theme", &Space::select_theme);

    godot::register_property("autoscrolling_speed", &Space::autoscrolling_speed, default_autoscrolling_speed);
    godot::register_property("enable_autoscrolling", &Space::enable_autoscrolling, default_enable_autoscrolling);
  }

  auto Space::_init() -> void
  {
    autoscrolling_speed = default_autoscrolling_speed;
    enable_autoscrolling = default_enable_autoscrolling;
    space_images = godot::Dictionary{};
    stars_images = godot::Dictionary{};
  }

  auto Space::_ready() -> void
  {
    space_texture = get_typed_node<godot::TextureRect>("Space/TextureRect");
    stars_texture = get_typed_node<godot::TextureRect>("Stars/TextureRect");
    load_images();
  }

  auto Space::_process(real_t delta) -> void
  {
    if (enable_autoscrolling)
    {
      scroll(delta);
    }
  }

  auto Space::select_theme(godot::String name) -> void
  {
    auto theme = theme_name(find_theme(name).value_or(Theme::blue));
    auto space_image = space_images[theme];
    auto stars_image = stars_images[theme];
    cast_to<godot::ImageTexture>(*space_texture->get_texture())->set_data(space_image);
    cast_to<godot::ImageTexture>(*stars_texture->get_texture())->set_data(stars_image);
  }

  auto Space::load_images() -> void
  {
    for (auto const & theme : available_themes())
    {
      godot::Godot::print("[core::Space] Loading background images for theme '{0}' ...", theme);
      space_images[theme] = load_image(space_background_path_pattern, theme);
      stars_images[theme] = load_image(stars_background_path_pattern, theme);
      godot::Godot::print("[core::Space] Background images loaded successfully");
    }
  }

  auto Space::scroll(real_t delta) -> void
  {
    auto current_offset = get_scroll_offset();
    auto offset_delta = godot::Vector2::RIGHT * autoscrolling_speed * delta;
    auto target_offset = current_offset + offset_delta;

    set_scroll_offset(target_offset);
  }

}  // namespace moon_buggy