#pragma once

enum class TaskStatus : unsigned char {
  pending = 1,
  running = 2,
  finish = 3,
  fail = 4
};
