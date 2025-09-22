import time
import requests
import multiprocessing as mp

def say_hello(_):
    pid = mp.current_process().pid
    print("Hello world from process: ", pid)
    return pid

def download_page(i: int, times_spent, lock):
    start_time = time.time()
    r = requests.get(f"https://name-service-phi.vercel.app/api/v1/names/{i}.json")
    end_time = time.time()
    time_spent = end_time - start_time
    pid = mp.current_process().pid

    with lock:
        if not pid in times_spent:
            times_spent[pid] = {"accesses": 0, "time": 0.0}

        process_info = times_spent[pid]
        process_info["accesses"] += 1
        process_info["time"] += time_spent
        times_spent[pid] = process_info

    return r.content

if __name__ == "__main__":
    with mp.Manager() as manager:
        start_tile = time.time()
        times_spent = manager.dict()
        lock = manager.Lock()

        with mp.Pool(5) as p:
            result = p.starmap(download_page, [(i, times_spent, lock) for i in range(25)])

        print(result)
        for pid, t in times_spent.items():
            print(f"Process {pid} made {t['accesses']} accesses, spending a total of {t['time']:.2f} seconds")