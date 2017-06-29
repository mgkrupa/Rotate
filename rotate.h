/*
 * rotate.h
 *
 *  Created on: Jun 29, 2017
 *      Author: Matthew Gregory Krupa
 */

#ifndef SRC_ROTATE_H_
#define SRC_ROTATE_H_

#include <algorithm>

/* Assumes that 0 < length_to_move_right_by < one_past_last_index_to_move
 *                                            - start_left.
 * Assumes that the iterator one_past_last_index_to_move + (length_left - 1) is
 *  defined (although it is NOT assumed that the value of this iterator can
 *  be taken, e.g. if iterator is of type long), where length_left is defined
 *  inside this function's body.
 * If _original is the original range of values inputed and if _result is this
 * range after this function finishes execution, then
 *  _original[i] = _result[i + length_to_move_right_by] whenever
 *   i + length_to_move_right_by < one_past_last_index_to_move, and otherwise
 *  _original[i] = _result[start_left
 *    + (i + length_to_move_right_by - one_past_last_index_to_move)].
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
       //NOTE: cur_it > start_right IS possible (in fact, even cur_it
      // > 200 + start_right is possible).

      //This loop's condition is true iff
      // cur_it + length_to_move_right_by <= max_index_moved_due_to_min
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


/* Does the equivalent of: while(*end_right >= *end_left) end_right--;
 * NOTE: This means that it assumes that such an index exists exists within
 *  [start_it : end_it] so IT DOES NO ITERATOR BOUNDS CHECKING.
 * Does the same thing as LargestIndexWithValueLessThan(), except that its
 *  inputs are different and also, in addition to performing a binary search,
 *  it simultaneously performs a linear search starting from the end_it.
 * Assumes that start_it <= end_it.
 * SE means that the inputs are start_index and end_index.
 * This algorithm find d by performing <= 3 * min {dist(start_it, d) + 1,
 *  dist(d, end_it) + 1, ceil(log_2(dist(start_it, end_it + 1)))} comparisons.
 */
template<class RAI, class T>
inline RAI LargestIteratorWithValueLessThan_KnownToExist(RAI start_it,
                                                    RAI end_it, const T value) {
  while (true) {//Use: while(start_it < end_it){ if the d is not known to exist.
    //The below three lines are optional. They are performed at most
    // std::distance(d, end_it) + 1 times.
    if (*end_it < value) {
      return end_it;
    }
    (void)end_it--;

    //The below three lines are optional. They are performed at most
    // std::distance(start_it, d) + 1 times.
    if (*(start_it + 1) >= value) {
      return start_it;
    }
    (void)start_it++;

    auto d = start_it + std::distance(start_it, end_it) / 2;
    if (*d < value)
      start_it = d;
    else
      end_it   = d - 1;
  }
}

/* Finds the SMALLEST iterator d such start_index <= d < start_index + length
 * such that *d >= value IF it exists, otherwise it returns
 * start_index + length - 1.
 * Assumes that [start_index : start_index + length - 1] is non-decreasing,
 * that all these elements exist, and that length > 0.
 * WARNING: POTENTIAL FALSE POSITIVE: Ways of this function returning
 * start_index + length - 1:
 *  1) Such a d does NOT exist.
 *  2) Such a d exists and happens to equal start_index + length - 1.
 */
template<class RAI, class T>
RAI SmallestIteratorWithValueGreaterThanOrEqual_KnownToExist(RAI start_it,
                                                    int length, const T value) {
  start_it += length - 1; //start_index will now represent the last index
  auto d = start_it - length / 2;

  do {
    if (*d >= value) {
      start_it  = d;
      length = (length + 1) / 2;
    } else {
      length = length / 2;
    }
    d = start_it - length / 2;
  } while (length > 1) ;
  return d;
}

/* Assumes that
 *  (1) start_left < start_right
 *  (2) *end_right < *end_left
 *  (3) *start_left > *start_right
 *  (4) both subranges are non-decreasing
 * If length_right = end_right + 1 - start_right, _sorted =
 *  std:sort(original_ranges), and i is the unique index satisfying
 *  (a) _sorted[i] == *end_right and if i > start_left then also
 *  (b) _sorted[i - 1] < *end_right
 *  then this algorithm will have the same effect as shifting all elements in
 *  [i + 1 - length_right : end_right] to the right by length_right positions,
 *  copying the right subrange (i.e. range's last length_right elements) into
 *  positions original_ranges[i : i + length_right - 1].
 * It will also update any input references that need changing, such as
 * start_right and end_right.
 */
template<class RAI>
void RotateLeftEndOfRight(const RAI start_left, RAI &start_right_out,
                           RAI &end_right_out) {
  auto start_right = start_right_out, end_right = end_right_out;
  int length_right = std::distance(start_right, end_right + 1);
  auto min_index_moved_due_to_max
  = SmallestIteratorWithValueGreaterThanOrEqual_KnownToExist(start_left,
                          std::distance(start_left, start_right), *end_right);

  //Note that (min_index_moved_due_to_max + length - 1) and all elements
  // right of it are now correctly placed so ignore them.
  end_right_out    = min_index_moved_due_to_max + (length_right - 2);
  //Since the original *start_right has been moved to
  // min_index_moved_due_to_max.
  start_right_out  = min_index_moved_due_to_max;

  //We want to move all elements with indices >= min_index_moved_due_to_max to
  // the right by length_right elements (modding to get back in range if
  // necessary).
  //Shift right by length_to_move_right_by indices.
  RotateRight(min_index_moved_due_to_max, end_right + 1, length_right);

  return ;
}

/* Assumes that
 *  (1) start_left < start_right
 *  (2) *end_right < *end_left
 *  (3) *start_left > *start_right
 *  (4) both subranges are non-decreasing
 * If length_left = start_right - start_left, _sorted =
 *  std:sort(original_ranges), and i is the unique index satisfying
 *  (a) _sorted[i] == *start_left and if i < end_right then also
 *  (b) _sorted[i + 1] > *start_left
 *  then this algorithm will have the same effect as shifting all elements
 *  in original_ranges[start_left : i] to the left by length_left positions,
 *  copying the left subrange (i.e. range's first length_left elements) into
 *  positions original_ranges[length_left + 1 : i].
 * It will also update any input references that need changing, such as
 * start_left and start_right.
 */
template<class RAI>
void RotateRightStartOfLeft(RAI &start_left_out, RAI &start_right_out,
                             const RAI end_right) {
  auto start_left = start_left_out, start_right = start_right_out;
  auto one_past_max_index_moved_due_to_min
     = LargestIteratorWithValueLessThan_KnownToExist(start_right,
          std::distance(start_right, end_right + 1), *start_left) + 1;
  //We want to move all elements with indices >= max_index_moved_due_to_min to
  // the right by length_right elements (modding to get back in range if
  // necessary)

  start_right_out = one_past_max_index_moved_due_to_min;

  //The following starts at element *start_left, moves it right into its
  // correct position, and then repeats.
  //Note that this is the same as is done in
  // ShiftLeftIncorrectlyPlacedEndOfRightSubvector(), which may
  // reduce the amount of code used in function that use both this function
  // and ShiftLeftIncorrectlyPlacedEndOfRightSubvector().

  auto length_to_move_right_by
             = std::distance(start_right, one_past_max_index_moved_due_to_min);
  start_left_out   += length_to_move_right_by + 1;
  RotateRight(start_left, one_past_max_index_moved_due_to_min,
               length_to_move_right_by);
  return ;
}

#include <iostream>

void RotateTest() {
  std::vector<int> vec = {1, 2, 3, 4, 5, 6, 7, 8};
  RotateRight(vec.begin() + 1, vec.begin() + 6, 2);

  for (auto i = 0u; i < vec.size(); i++)
    std::cout << vec[i] << " ";
  std::cout << std::endl;
}

#endif /* SRC_ROTATE_H_ */
