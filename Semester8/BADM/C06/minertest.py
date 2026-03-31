from unittest import TestCase
from blockheader import BlockHeader
from miner import Miner
from main import NONCE_START, NONCE_END, nonce_generator

class MinerTest(TestCase):
    def setUp(self):
        self.block_hdr = BlockHeader(
            version=0x20800000,
            prev_hash="00000000000000000000b70be3652e4396ecacbc7a37aa2931343d7ee86bd82c",
            merkle_root="6137b61d88a19e5bdca7b0edc2e343c814ef18bce81931357ecbc9133a3154eb",
            ntime=1707755784,
            nbits=0x1703ba5d,
            nonce=0,
            mode="threading"
        )
        self.block_hdrP = BlockHeader(
            version=0x20800000,
            prev_hash="00000000000000000000b70be3652e4396ecacbc7a37aa2931343d7ee86bd82c",
            merkle_root="6137b61d88a19e5bdca7b0edc2e343c814ef18bce81931357ecbc9133a3154eb",
            ntime=1707755784,
            nbits=0x1703ba5d,
            nonce=0,
            mode="multiprocessing"
        )
        self.difficulty = 75502165623893.72

    def test_single_worker_threading(self):
        miner = Miner(self.block_hdr, self.difficulty, mode="threading")
        block_hdr = miner.mine_single_worker(NONCE_START, NONCE_END)
        self.assertEqual(block_hdr.nonce, 2964565117)
        self.assertEqual(block_hdr.hash(), "000000000000000000025879c120878cd817342b778fb23071f8f83a4c61e61d")

    def test_single_worker_process(self):
        miner = Miner(self.block_hdrP, self.difficulty, mode="multiprocessing")
        block_hdrP = miner.mine_single_worker(NONCE_START, NONCE_END)
        self.assertEqual(block_hdrP.nonce, 2964565117)
        self.assertEqual(block_hdrP.hash(), "000000000000000000025879c120878cd817342b778fb23071f8f83a4c61e61d")

    def test_multi_worker_threading(self):
        miner = Miner(self.block_hdr, self.difficulty, mode="threading")
        block_hdr = miner.mine_multi_worker(nonce_ranges=nonce_generator(NONCE_START, NONCE_END, 4))
        self.assertEqual(block_hdr.nonce, 2964565117)
        self.assertEqual(block_hdr.hash(), "000000000000000000025879c120878cd817342b778fb23071f8f83a4c61e61d")

    def test_multi_worker_process(self):
        miner = Miner(self.block_hdrP, self.difficulty, mode="multiprocessing")
        block_hdrP = miner.mine_multi_worker(nonce_ranges=nonce_generator(NONCE_START, NONCE_END, 4))
        self.assertEqual(block_hdrP.nonce, 2964565117)
        self.assertEqual(block_hdrP.hash(), "000000000000000000025879c120878cd817342b778fb23071f8f83a4c61e61d")