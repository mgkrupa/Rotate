//============================================================================
// Name        : main.cpp
// Author      : Matthew Gregory Krupa
// Version     :
// Copyright   : MIT License
// Description : Hello World in C++, Ansi-style
//============================================================================

#include "rotate.h"

#include <iostream>
#include <vector>

void RotateRightTest(std::size_t vec_size, std::size_t displacement_right,
                     std::size_t start_disp = 0,
                     std::size_t one_past_end_disp = 0) {
  if (vec_size == 0)
    return ;
  if (one_past_end_disp == 0)
    one_past_end_disp = vec_size;
  std::vector<int> vec(vec_size, 0);
  for (auto i = 0u; i < vec.size(); i++) {
    vec[i] = i;
    std::cout << vec[i];
    if (i != vec.size() - 1)
      std::cout << " ";
    else
      std::cout << std::endl;
  }

  RotateRight(vec.begin() + start_disp, vec.begin() + one_past_end_disp,
              displacement_right);

  for (auto i = 0u; i < vec.size(); i++) {
    std::cout << vec[i];
    if (i != vec.size() - 1)
      std::cout << " ";
    else
      std::cout << std::endl;
  }
}

void RotateLeftTest(std::size_t vec_size, std::size_t displacement_left,
                    std::size_t start_disp = 0,
                    std::size_t one_past_end_disp = 0) {
  if (vec_size == 0)
    return ;
  if (one_past_end_disp == 0)
    one_past_end_disp = vec_size;
  std::vector<int> vec(vec_size, 0);
  for (auto i = 0u; i < vec.size(); i++) {
    vec[i] = i;
    std::cout << vec[i];
    if (i != vec.size() - 1)
      std::cout << " ";
    else
      std::cout << std::endl;
  }

  RotateLeft(vec.begin() + start_disp, vec.begin() + one_past_end_disp,
             displacement_left);

  for (auto i = 0u; i < vec.size(); i++) {
    std::cout << vec[i];
    if (i != vec.size() - 1)
      std::cout << " ";
    else
      std::cout << std::endl;
  }
}


int main() {
  std::cout << "RotateRight() [1, 8) by 2" << std::endl;
  RotateRightTest(10, 2, 1, 8);
  std::cout << "RotateLeft() [1, 8) by 2" << std::endl;
  RotateLeftTest(10, 2, 1, 8);
  return 0;
}
