from blockheader import BlockHeader
from miner import Miner
from time import time
from unittest import TestCase


MODE = 'threading'
NONCE_START = 2_964_565_000 
# 2_964_565_117
NONCE_END = 2_970_000_000

def nonce_generator(start, end, number) -> list:
    step = (end - start) // number
    return [(start + step * i, start + step * (i + 1)) for i in range(number)]

if __name__ == '__main__':
    # https://www.blockchain.com/explorer/blocks/btc/830166
    # 000000000000000000025879c120878cd817342b778fb23071f8f83a4c61e61d
    block_hdr = BlockHeader(
        version=0x20800000,
        prev_hash="00000000000000000000b70be3652e4396ecacbc7a37aa2931343d7ee86bd82c",
        merkle_root="6137b61d88a19e5bdca7b0edc2e343c814ef18bce81931357ecbc9133a3154eb",
        ntime=1707755784,
        nbits=0x1703ba5d,
        nonce=0,
        mode=MODE
    )
    difficulty = 75502165623893.72
    """
    # Worker = 1
    start = time()
    miner = Miner(block_hdr, difficulty, mode=MODE)
    x = miner.mine_single_worker(NONCE_START, NONCE_END)
    print(x)
    print("x.nonce: ", x.nonce)   
    print(f"Time elapsed for 1 processes: {time() - start} seconds \n")
    """
    
    # Worker = 2
    start = time()
    miner = Miner(block_hdr, difficulty, mode=MODE)
    x = miner.mine_multi_worker(nonce_ranges=nonce_generator(NONCE_START, NONCE_END, 2))
    print(x)
    print("x.nonce: ", x.nonce)   
    print(f"Time elapsed for 2 processes: {time() - start} seconds \n")