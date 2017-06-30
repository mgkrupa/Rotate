/*
 * rotate.h
 *
 *  Created on: Jun 29, 2017
 *      Author: Matthew Gregory Krupa
 */

#ifndef ROTATE_H_
#define ROTATE_H_

#include <algorithm>

/* Assumes that 0 < length_to_move_right_by < one_past_last_index_to_move
 *                                            - start_left.
 * Assumes that the iterator one_past_last_index_to_move + (length_left - 1) is
 *  defined (although it is NOT assumed that the value of this iterator can
 *  be taken, (e.g. as is the case when if iterator is of type long),
 *  where length_left is defined inside this function's body.
 * If _original is the original range of values referenced by start_left and if
 *  _result is this range after this function finishes execution, then
 *  _original[i] = _result[i + length_to_move_right_by] whenever
 *   i + length_to_move_right_by < one_past_last_index_to_move, and otherwise
 *  _original[i] = _result[start_left
 *    + (i + length_to_move_right_by - one_past_last_index_to_move)].
 * i.e. it rotates the range to the right by length_to_move_right_by elements,
 *  with those being "bumped off the right end" of the range being cycled
 *  back to the beginning (i.e. the left side, which start_left references)
 *  of the range.
 */
template<class RAI>
void RotateRight(const RAI start_left, const RAI one_past_last_index_to_move,
                  const long length_to_move_right_by) {
  const auto start_right = one_past_last_index_to_move - length_to_move_right_by;
  const auto length_left = std::distance(start_left, start_right);
  //We want to move all elements with indices >= max_index_moved_due_to_min to
  // the right by length_right elements (modding to get back in range if
  // necessary)

  //The following starts at element *start_left, moves it right into its
  // correct position, and then repeats.
  auto cycle_starting_it = start_left;

  //The first time that execution reaches the inner-most while() loop, it's body
  // will be executed num_first_inner_while_iters times.
  //All subsequent times that execution reaches the inner-most while() loop,
  // it's body will be executed either num_first_inner_while_iters - 1 times OR
  // num_first_inner_while_iters times, where the latter happens if and only if
  // cur_index < additional_swap_threshold.
  //Thus, upon entering the do{}while(); loop, execution will reach the end of
  // do{}'s body only after it has executed swap()
  // num_first_inner_while_iters
  //                 + static_cast<int>(cur_index < additional_swap_threshold)
  // times
  const auto num_first_inner_while_iters =
      (static_cast<long>(std::distance(cycle_starting_it, start_right))
       + (static_cast<long>(length_to_move_right_by) - 1))
          / static_cast<long>(length_to_move_right_by);
  const auto additional_swap_threshold = start_right
      - (static_cast<long>(num_first_inner_while_iters) - 1)
      * length_to_move_right_by;

  const auto total_num_elements_to_move_right
                     = std::distance(start_left, one_past_last_index_to_move);
  std::size_t num_elements_moved_right = 0ul;
  while (num_elements_moved_right < static_cast<std::size_t>(total_num_elements_to_move_right)) {
    auto temp = *cycle_starting_it;
    auto cur_it = cycle_starting_it;
    do {
      num_elements_moved_right
      += static_cast<std::size_t>(num_first_inner_while_iters)
          + static_cast<std::size_t>(cur_it < additional_swap_threshold);
      //Development Note: The following while()'s loop's body will be executed
      // ceil((start_right - cur_it)/length_to_move_right_by) many times.
      //The fastest way to compute this quantity is with the following formula
      //const int inner_while_num_swap_calls = (start_right
      //        + length_to_move_right_by - 1 - cur_it)/length_to_move_right_by;

      //This loop's condition is true iff
      // cur_it + length_to_move_right_by <= max_index_moved_due_to_min

      /************************************************************
      Note that there is NO if() inside this while() loop's body.
      This is one of the key way that this function differs from other rotate
       functions.
      We also do NOT increment num_elements_moved_right within this loop's body.
      ************************************************************* */
      while(cur_it < start_right) {
        cur_it += length_to_move_right_by;
        std::swap(*cur_it, temp);
      }
      cur_it -= length_left;
      std::swap(*cur_it, temp);
    } while (cur_it != cycle_starting_it) ;
    (void)cycle_starting_it++;
  }
  return ;
}

/* Assumes that 0 < length_to_move_right_by < one_past_last_index_to_move
 *                                            - start_left.
 * Assumes that the iterator one_past_last_index_to_move + (length_left - 1) is
 *  defined (although it is NOT assumed that the value of this iterator can
 *  be taken, (e.g. as is the case when if iterator is of type long),
 *  where length_left is defined inside this function's body.
 * If _original is the original range of values referenced by start_left and if
 *  _result is this range after this function finishes execution, then
 *  _original[i] = _result[i - length_to_move_right_by] whenever
 *   i - length_to_move_right_by < start_left, and otherwise
 *  _original[i] = _result[one_past_last_index_to_move
 *                 - ((one_past_last_index_to_move - i) + 1)].
 * i.e. it rotates the range to the left by length_to_move_right_by elements,
 *  with those being "bumped off the beginning" of the range being cycled
 *  back to the end of the range.
 */
template<class RAI>
inline void RotateLeft(const RAI start_left, const RAI one_past_last_index_to_move,
                  const long length_to_move_left_by) {
  auto length_to_move_right_by
              = std::distance(start_left, one_past_last_index_to_move)
                - length_to_move_left_by;
  RotateRight(start_left, one_past_last_index_to_move, length_to_move_right_by);
}

#endif /* ROTATE_H_ */
