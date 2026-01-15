import numpy as np
import matplotlib.pyplot as plt
import multiprocessing
import time
import sys


N = 500              
D = 2                
RADIUS = 10.0        
THRESHOLD = 0.05     
WORKERS = 4          
MAX_ITER = 100       

def update_chunk(args):
    """
    Funkce, kterou vykonává každý worker pro svou přidělenou část bodů.
    Vstup:
        args: n-tice obsahující (indices, all_points, radius)
    Výstup:
        numpy pole nových pozic pro body v 'indices'
    """
    indices, all_points, radius = args
    new_positions = np.zeros((len(indices), all_points.shape[1]))
    
    for local_i, global_i in enumerate(indices):
        current_point = all_points[global_i]
        
        
        
        distances = np.linalg.norm(all_points - current_point, axis=1)
        
        
        
        neighbors = all_points[distances <= radius]
        
        
        if len(neighbors) > 0:
            new_center = np.mean(neighbors, axis=0)
            new_positions[local_i] = new_center
        else:
            
            new_positions[local_i] = current_point
            
    return new_positions

def parallel_mean_shift(points, pool, workers_count, radius):
    """
    Rozdělí práci mezi workery a spojí výsledky.
    """
    total_points = len(points)
    chunk_size = (total_points + workers_count - 1) // workers_count
    
    
    
    
    
    
    tasks = []
    for i in range(0, total_points, chunk_size):
        indices_range = range(i, min(i + chunk_size, total_points))
        tasks.append((indices_range, points, radius))
    
    
    
    results = pool.map(update_chunk, tasks)
    
    
    new_points = np.vstack(results)
    return new_points

def main():
    
    np.random.seed(42)  
    data = np.random.rand(N, D) * 100
    
    
    plt.ion()
    fig, ax = plt.subplots(figsize=(8, 8))
    
    
    
    colors = ['yellow' if i % 2 == 0 else 'blue' for i in range(N)]
    scat = ax.scatter(data[:, 0], data[:, 1], c=colors, edgecolor='black', linewidth=0.5)
    
    
    circles = []
    for i in range(N):
        
        c = 'yellow' if i % 2 == 0 else 'blue'
        circle = plt.Circle((data[i, 0], data[i, 1]), RADIUS, 
                            color=c, fill=False, alpha=0.3)
        circles.append(circle)
        ax.add_artist(circle)
        
    ax.set_xlim(-20, 120)
    ax.set_ylim(-20, 120)
    ax.set_title(f"Mean Shift (Parallel): N={N}, R={RADIUS}")
    
    print(f"Spouštím simulaci s {WORKERS} workery...")
    
    
    
    with multiprocessing.Pool(processes=WORKERS) as pool:
        
        for iteration in range(MAX_ITER):
            start_time = time.time()
            
            
            new_data = parallel_mean_shift(data, pool, WORKERS, RADIUS)
            
            
            shifts = np.linalg.norm(new_data - data, axis=1)
            max_shift = np.max(shifts)
            
            
            data = new_data
            
            
            scat.set_offsets(data)
            
            
            for i in range(N):
                circles[i].center = (data[i, 0], data[i, 1])
            
            fig.canvas.draw_idle()
            plt.pause(0.5) 
            
            end_time = time.time()
            print(f"Iterace {iteration}: Max posun = {max_shift:.4f} (čas kroku: {end_time - start_time:.4f}s)")
            
            if max_shift < THRESHOLD:
                print("Konvergováno!")
                break
                
            
            
            
    print("Simulace dokončena.")
    plt.ioff()
    plt.show()

if __name__ == "__main__":
    main()
