//! Run this file with `cargo test --test 04_merge_slices`.

//! TODO: Implement a function called `merge_slices`, which is useful for the merge sort algorithm.
//! It will take two sorted `u32` slices as inputs and merge them into a sorted vector (Vec).
//! The function will return the vector.
//!
//! The time complexity of this function has to be O(n).
//!
//! Bonus: Can you build a complete merge sort on top of this function? :)

fn merge_slices(first: &[u32], second: &[u32]) -> Vec<u32> {
    let mut result = Vec::with_capacity(first.len() + second.len());
    let mut first_idx = 0;
    let mut second_idx = 0;

    let extend_with: usize; //1 = first, 2 = second

    loop {
        if first_idx == first.len() {
            extend_with = 2;
            break;
        }

        if second_idx == second.len() {
            extend_with = 1;
            break;
        }

        if first[first_idx] < second[second_idx] {
            result.push(first[first_idx]);
            first_idx += 1;
        } else {
            result.push(second[second_idx]);
            second_idx += 1;
        }
    }

    result.extend_from_slice(if extend_with == 1 {
        &first[first_idx..]
    } else {
        &second[second_idx..]
    });

    result
}

/// Below you can find a set of unit tests.
#[cfg(test)]
mod tests {
    use crate::merge_slices;

    #[test]
    fn merge_slices_empty() {
        assert_eq!(merge_slices(&[], &[]), vec![]);
    }

    #[test]
    fn merge_slices_basic() {
        assert_eq!(merge_slices(&[1, 2, 3], &[4, 5, 6]), vec![1, 2, 3, 4, 5, 6]);
    }

    #[test]
    fn merge_slices_interleaved() {
        assert_eq!(merge_slices(&[1, 3, 5], &[2, 4, 6]), vec![1, 2, 3, 4, 5, 6]);
    }

    #[test]
    fn merge_slices_duplicates() {
        assert_eq!(merge_slices(&[1, 1, 3], &[1, 3, 4]), vec![1, 1, 1, 3, 3, 4]);
    }

    #[test]
    fn merge_slices_uneven_size() {
        assert_eq!(
            merge_slices(&[1, 4, 6, 8], &[0, 1, 1, 3, 4, 5, 7, 8, 9]),
            vec![0, 1, 1, 1, 3, 4, 4, 5, 6, 7, 8, 8, 9]
        );
    }

    #[test]
    fn merge_slices_first_empty() {
        assert_eq!(merge_slices(&[], &[1, 4, 8]), vec![1, 4, 8]);
    }

    #[test]
    fn merge_slices_second_empty() {
        assert_eq!(merge_slices(&[1, 9, 11], &[]), vec![1, 9, 11]);
    }
}
