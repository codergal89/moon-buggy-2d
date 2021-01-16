#ifndef MOON_BUGGY_2D_GAME_GAME_MACHINE_HPP
#define MOON_BUGGY_2D_GAME_GAME_MACHINE_HPP

#include "game/Buggy.hpp"
#include "game/Game.hpp"

#include <boost/sml.hpp>

namespace moon_buggy
{
  // clang-format off
  struct animation_finished {};
  struct crash_occured { Buggy * buggy{}; };
  struct game_start_requested {};
  struct next_level_requested {};
  struct reached_goal {};
  struct readied_up {};
  struct retry_requested {};

  auto constexpr init = boost::sml::state<struct init>;
  auto constexpr in_main_menu = boost::sml::state<struct in_main_menu>;
  auto constexpr level_running = boost::sml::state<struct level_running>;
  auto constexpr crashed = boost::sml::state<struct crashed>;
  auto constexpr success_animation_playing = boost::sml::state<struct success_animation_playing>;
  auto constexpr level_finished = boost::sml::state<struct level_finished>;
  auto constexpr last_level_finished = boost::sml::state<struct last_level_finished>;

  auto constexpr has_next_level = [](Game & game){ return static_cast<bool>(game.call("has_next_level")); };

  auto constexpr play_success_animation = [](Game & game){ game.call("play_success_animation"); };
  auto constexpr restart_level = [](Game & game){ game.call("restart_level"); };
  auto constexpr show_buggy_crashed_screen = [](Game & game){ game.call("show_buggy_crashed_screen"); };
  auto constexpr show_level_complete_screen = [](Game & game){ game.call("show_level_complete_screen"); };
  auto constexpr show_main_menu = [](Game & game){ game.call("show_main_menu"); };
  auto constexpr start_next_level = [](Game & game){ game.call("start_next_level"); };
  // clang-format on

  struct GameMachine
  {
    auto operator()() const
    {
      using namespace boost::sml;

      // clang-format off
      return make_transition_table(
        *init                     + event<readied_up>                               / show_main_menu             = in_main_menu,

        in_main_menu              + event<game_start_requested> [ has_next_level ]  / start_next_level           = level_running,

        level_running             + event<crash_occured>                                                         = crashed,
        level_running             + event<reached_goal>                             / play_success_animation     = success_animation_playing,

        crashed                   + event<retry_requested>                          / restart_level              = level_running,

        success_animation_playing + event<animation_finished>   [ has_next_level ]  / show_level_complete_screen = level_finished,
        success_animation_playing + event<animation_finished>   [ !has_next_level ] / show_main_menu             = last_level_finished,
        
        level_finished            + event<next_level_requested>                     / start_next_level           = level_running
      );
      // clang-format on
    }
  };
}  // namespace moon_buggy

#endif