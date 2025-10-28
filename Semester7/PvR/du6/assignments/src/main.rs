//! You can use this file for experiments.

use std::collections::BTreeMap;

/// This is just a type alias, not a new type.
/// It can be useful to start with it if you want to give a new name
/// to an existing type, but don't want to deal with newtype wrapping.
type Address = usize;

#[derive(Default)]
struct MemoryMap {
    map: BTreeMap<Address, Vec<u8>>,
}

/// Allow creating the memory map from an iterator of (address, bytes) tuples.
impl<T> From<T> for MemoryMap
where
    T: Iterator<Item = (Address, Vec<u8>)>,
{
    fn from(value: T) -> Self {
        Self {
            map: value.collect(),
        }
    }
}

impl MemoryMap {
    /// TODO: implement a method that reads the given number of bytes from the specified
    /// address. Note that it is possible that the read will span multiple separate buffers!
    /// In that case, the contents of such buffers have to be combined together.
    /// For example, if you read at `40` with count `8`, and the map stores `[1, 2, 3, 4]` at
    /// `40` and `[5, 6, 7, 8]` at `44`, the read should return `[1, 2, 3, 4, 5, 6, 7, 8]`.
    ///
    /// If **any** bytes are missing in the specified address range
    /// `[address, address + count)`, return `None`.
    /// For example, in the situation described above, if the second buffer started at `45` and
    /// not at `44`, there would be a missing byte at address `44`, and `read` would return `None`.
    ///
    /// Remember: this method should have complexity O(log(n)), where `n` is the number of buffers
    /// stored in the map.
    fn read(&self, address: Address, count: usize) -> Option<Vec<u8>> {
        let mut address = address;
        let end = address + count;

        let closest = self.map.range(..=address);
        let closest = closest.last();

        let mut collected = Vec::with_capacity(count);

        match closest {
            None => {
                return None;
            }
            Some((_address, data)) => {
                let offset = address - *_address;
                address = *_address + data.len(); //this updated address will be used later

                if offset >= data.len() {
                    return None; //missing data :(
                }

                //put into collected
                if end <= *_address + data.len() {
                    collected.extend_from_slice(&data[offset..(offset + count)]);
                    return Some(collected); //we have everything we need
                }

                collected.extend_from_slice(&data[offset..]);
            }
        }

        //now get the rest
        let rest = self.map.range(address..end);
        for (addr, data) in rest {
            if *addr != address {
                return None; //missing data :(
            }

            if end <= *addr + data.len() {
                collected.extend_from_slice(&data[..(end - *addr)]);
                break;
            }

            collected.extend_from_slice(data);
            address = *addr + data.len();
        }

        if collected.len() < count {
            return None;
        }

        Some(collected)
    }

    /// TODO: implement a method that writes the given byte buffer at the specified address.
    /// You will need to overwrite (or even outright remove) existing buffers (or their parts)
    /// if there is any overlap between existing buffers and the specified address range
    /// `[address, address + buffer.len())`.
    ///
    /// Remember: this method should have complexity O(log(n)).
    fn write(&mut self, address: Address, buffer: Vec<u8>) {}
}

fn make_map(data: &[(Address, &[u8])]) -> MemoryMap {
    let data = data.iter().map(|(addr, bytes)| (*addr, bytes.to_vec()));
    data.into()
}

fn main() {
    println!("Hello Rust!");

    let map = make_map(&[(40, &[1, 2, 3, 4])]);
    println!("{:?}", map.read(50, 4));
}
