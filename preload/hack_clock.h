#pragma once

#include <chrono>
#include <filesystem>

struct hack_clock : std::filesystem::file_time_type::clock {
  template <typename TargetDur, typename _Dur>
  static std::chrono::time_point<std::filesystem::file_time_type::clock,
                                 TargetDur>
  from_sys(const std::chrono::time_point<std::chrono::system_clock, _Dur>
               &__t) noexcept {
    return std::chrono::time_point_cast<TargetDur>(_S_from_sys(__t));
  }

  template <typename TargetDur, typename _Dur>
  static std::chrono::time_point<std::chrono::system_clock, TargetDur>
  to_sys(const std::chrono::time_point<std::filesystem::file_time_type::clock,
                                       _Dur> &__t) noexcept {
    return std::chrono::time_point_cast<TargetDur>(_S_to_sys(__t));
  }
};

