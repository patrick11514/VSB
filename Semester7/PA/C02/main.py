import itertools as it
import time

import multiprocessing as mp
'''
Check the example below and notice that:
  - there is a module called `itertools` that provides some functions with permutations (e.g., generate next permutation)
  - this can be used as a part of the for cycle
  - it is sometimes useful to generate permutations with a fixed prefix (e.g., [0, 1, ...], [0, 2, ...], etc.)
  - the 2D array D holds the distances between cities in a toy TSP example  
  - the `calc_perm` and `calc_perm_pref` are stubs of functions to compute the length of the path that need to be implemented.



Challenge:
  - implement the `calc_perm, calc_perm_pref` functions and demonstrate their functionality
  - bonus: implement a function that loads a distance matrix from a real TSP instance, 
  https://github.com/pdrozdowski/TSPLib.Net/blob/master/TSPLIB95/tsp/ulysses16.tsp
  - compute the length of a few tours for the ulysses16 instance (why do we not traverse all permutations in this case?)

'''

D = [[0, 5, 40, 11], [5, 0, 9, 6], [40, 9, 0, 8], [11, 6, 8, 0]]


def calc_perm(p, D):
  sum = 0
  n = len(p)
  for i in range(n):
    sum += D[p[i]][p[(i + 1) % n]]
  return sum


def calc_perm_pref(pref, p, D):
  sum = 0
  n = len(p)
  sum += D[pref][p[0]]
  for i in range(n - 1):
    sum += D[p[i]][p[i + 1]]
  sum += D[p[n - 1]][pref]
  return sum


def test_permutations(n):
  for p in it.permutations(range(n)):
    calc_perm(p, D)


def test_permutations_pref(pref, n):
  for p in it.permutations(range(n)[1:]):
    calc_perm_pref(pref, p, D)


#test_permutations(4)
test_permutations_pref(0, 4)


def load_ulysses(max_coords):
  path = "ulysses16.tsp"
  index = -1 - (16 - max_coords)
  with open(path, "r") as f:
    lines = f.readlines()
    coords = []
    coord_lines = lines[7:index]
    for line in coord_lines:
      parts = line.split()
      coords.append((float(parts[1]), float(parts[2])))
  return coords

def compute_distance_matrix(coords):
  import math
  n = len(coords)
  D = [[0]*n for _ in range(n)]
  for i in range(n):
    for j in range(n):
      if i != j:
        D[i][j] = round(math.sqrt((coords[i][0]-coords[j][0])**2 + (coords[i][1]-coords[j][1])**2), 2) 
  return D

def calculate_perm_fixed_two(k, D, distances, lock):
  n = len(D)
  remaining = [i for i in range(n) if i not in (0, k)]

  best_len = None
  best_tour = None  

  for tail in it.permutations(remaining):
    tour = (0, k) + tail
    L = calc_perm(tour, D)
    if best_len is None or L < best_len:
      best_len = L
      best_tour = tour
    
  with lock:
    distances[k] = (best_len, best_tour)
    print(f"Best tour with 0 and {k}: {best_tour} length: {best_len}")


if __name__ == "__main__":
  N = 10
  coords = load_ulysses(N)
  D = compute_distance_matrix(coords)

  with mp.Manager() as manager:
    distances = manager.dict()
    lock = manager.Lock()

    with mp.Pool(N-1) as p:
      result = p.starmap(calculate_perm_fixed_two, [(k, D, distances, lock) for k in range(N-1)])

    global_best_len = None
    global_best_tour = None
    for v, info in distances.items():
      if global_best_len is None or info[0] < global_best_len:
        global_best_len = info[0]
        global_best_tour = info[1]

    print(f"Global best tour: {global_best_tour} length: {global_best_len}")



"""
time_start = time.time()
test_permutations(N)
time_end = time.time()
print("Time taken (s):", time_end - time_start)

time_start = time.time()
test_permutations_pref(0, N)
time_end = time.time()
print("Time taken with prefix (s):", time_end - time_start)
"""


