import threading
import multiprocessing

class Miner:
    def __init__(self, block_header, difficulty, mode='threading'):
        self.block_header = block_header
        self.difficulty = difficulty
        self.target = self.difficulty_to_target(difficulty)
        self.mode = mode
        self.result = None

        if self.mode == 'threading':
            self.worker = threading.Thread
            self.lock = threading.Lock()
            self.found = threading.Event()
        elif self.mode == 'multiprocessing':
            self.worker = multiprocessing.Process

            self.lock = multiprocessing.Lock()  
            self.found = multiprocessing.Event()
            self.result_queue = multiprocessing.Queue()
        else:
            raise ValueError("Invalid mode. Choose 'threading' or 'multiprocessing'.")


    def difficulty_to_target(self, difficulty):
        # TODO 2: Calculate the target from the difficulty
   
        MAX_TARGET = 0x00000000FFFF0000000000000000000000000000000000000000000000000000
        return int(MAX_TARGET / difficulty)

    def mine_single_worker(self, start_nonce=0, end_nonce=2**32, multi=False):
        # TODO 3: Implement the mining algorithm
        nonce = start_nonce 
        while True:
            if multi and self.found.is_set():
                break
            self.block_header.nonce = nonce
            hash_int = int(self.block_header.hash(), 16)
            if hash_int < self.target:
                if multi:
                    self.found.set()
                    if self.mode == "multiprocessing":
                        self.result_queue.put(nonce)
                        break
                self.result = self.block_header
                return self.block_header
            nonce += 1
            if nonce >= end_nonce:
                break

    def mine_multi_worker(self, nonce_ranges):
        # TODO 4: Implement the mining algorithm with multiple workers
        workers = []
        for start, end in nonce_ranges:
            w = self.worker(target=self.mine_single_worker, args=(start, end, True))
            workers.append(w)
            w.start()

        self.found.wait()

        if self.mode == "multiprocessing":
            nonce = self.result_queue.get()
            self.block_header.nonce = nonce
            self.result = self.block_header

        for w in workers:
            if self.mode == "multiprocessing":
                w.terminate()
            w.join()

        return self.result


