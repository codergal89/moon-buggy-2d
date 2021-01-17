#include "gui/BuggyCrashed.hpp"

#include <Array.hpp>
#include <Dictionary.hpp>
#include <Godot.hpp>
#include <Label.hpp>
#include <String.hpp>

namespace moon_buggy
{
  auto BuggyCrashed::_register_methods() -> void
  {
    godot::register_method("_ready", &BuggyCrashed::_ready);
    godot::register_method("set_crash_reason", &BuggyCrashed::set_crash_reason);
    godot::register_method("retry_button_clicked", &BuggyCrashed::retry_button_clicked);

    godot::register_signal<BuggyCrashed>("retry_level", godot::Dictionary{});
  }

  auto BuggyCrashed::_init() -> void
  {
  }

  auto BuggyCrashed::_ready() -> void
  {
    crash_reason = get_typed_node<godot::Label>("RowContainer/MessageContainer/CrashReason");
    CRASH_COND(!crash_reason);
  }

  auto BuggyCrashed::set_crash_reason(godot::String reason) -> void
  {
    crash_reason->set_text(reason);
  }

  auto BuggyCrashed::retry_button_clicked() -> void
  {
    emit_signal("retry_level");
  }

}  // namespace moon_buggy