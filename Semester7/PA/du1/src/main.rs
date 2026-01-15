use anyhow::Result;
use itertools::Itertools;
use rayon::prelude::*;
use std::{
    fmt::Display,
    fs,
    sync::{Arc, Mutex},
    time::Instant,
};

struct Data {
    weights: Vec<f32>,

    matrix: Vec<Vec<f32>>,

    n: usize,
}

impl Data {
    fn new(path: &str) -> Result<Self> {
        let input = fs::read_to_string(path)?;

        let lines = input.lines();

        let mut lines = lines.filter(|l| !l.trim().is_empty());

        let n: usize = lines
            .next()
            .ok_or(anyhow::anyhow!("File empty"))?
            .trim()
            .parse()?;

        let weights: Vec<f32> = lines
            .next()
            .ok_or(anyhow::anyhow!("Missing weights"))?
            .split_whitespace()
            .map(|x| x.parse().unwrap())
            .collect();

        if weights.len() != n {
            eprintln!("Warning: Weights count {} != N {}", weights.len(), n);
        }

        let mut matrix = Vec::new();

        for _ in 0..n {
            let row: Vec<f32> = lines
                .next()
                .ok_or(anyhow::anyhow!("Missing matrix row"))?
                .split_whitespace()
                .map(|x| x.parse().unwrap())
                .collect();

            matrix.push(row);
        }

        for i in 0..n {
            for j in 0..n {
                let v = matrix[i][j].max(matrix[j][i]);

                matrix[i][j] = v;

                matrix[j][i] = v;
            }
        }

        Ok(Data { weights, matrix, n })
    }
}

impl Display for Data {
    fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        writeln!(f, "N: {}", self.n)?;

        writeln!(f, "Weights: {:?}", self.weights)?;

        writeln!(f, "Matrix:")?;

        for row in &self.matrix {
            writeln!(f, "{:?}", row)?;
        }

        Ok(())
    }
}

struct Solution {
    cost: f32,

    perm: Vec<usize>,
}

fn calculate_added_cost(data: &Data, current_perm: &[usize], new_elem: usize) -> f32 {
    let mut cost = 0.0;

    let width_new = data.weights[new_elem];

    for (idx, &prev_elem) in current_perm.iter().enumerate() {
        let weight = data.matrix[prev_elem][new_elem];

        if weight.abs() < f32::EPSILON {
            continue;
        }

        let mut dist = 1.5 * data.weights[prev_elem] + 1.5 * width_new;

        for &k in &current_perm[idx + 1..] {
            dist += data.weights[k];
        }

        cost += weight * dist;
    }

    cost
}

fn solve_recursive(
    data: &Data,

    current_perm: &mut Vec<usize>,

    remaining: &mut Vec<usize>,

    current_cost: f32,

    local_best_cost: &mut f32,

    global_best: &Arc<Mutex<Solution>>,
) {
    if current_cost >= *local_best_cost {
        return;
    }

    if remaining.is_empty() {
        if current_cost < *local_best_cost {
            *local_best_cost = current_cost;

            let mut g = global_best.lock().unwrap();

            if current_cost < g.cost {
                g.cost = current_cost;

                g.perm = current_perm.clone();
            } else {
                if g.cost < *local_best_cost {
                    *local_best_cost = g.cost;
                }
            }
        }

        return;
    }

    for i in 0..remaining.len() {
        let next_elem = remaining[i];

        let added = calculate_added_cost(data, current_perm, next_elem);

        let new_cost = current_cost + added;

        if new_cost >= *local_best_cost {
            continue;
        }

        current_perm.push(next_elem);

        remaining.remove(i);

        solve_recursive(
            data,
            current_perm,
            remaining,
            new_cost,
            local_best_cost,
            global_best,
        );

        remaining.insert(i, next_elem);

        current_perm.pop();
    }
}

fn solve(data: Data) -> Result<()> {
    let start = Instant::now();

    let n = data.n;

    let global_best = Arc::new(Mutex::new(Solution {
        cost: f32::MAX,

        perm: Vec::new(),
    }));

    let k = if n < 2 { 1 } else { 2 };

    let prefixes = (0..n).permutations(k).collect::<Vec<_>>();

    println!(
        "Generated {} tasks (prefixes of length {}) using {} threads",
        prefixes.len(),
        k,
        rayon::current_num_threads()
    );

    prefixes.par_iter().for_each(|prefix| {
        let mut current_perm = Vec::with_capacity(n);

        let mut current_cost = 0.0;

        for &p in prefix {
            current_cost += calculate_added_cost(&data, &current_perm, p);

            current_perm.push(p);
        }

        let mut remaining: Vec<usize> = (0..n).filter(|&x| !prefix.contains(&x)).collect();

        let mut local_best_cost = { global_best.lock().unwrap().cost };

        solve_recursive(
            &data,
            &mut current_perm,
            &mut remaining,
            current_cost,
            &mut local_best_cost,
            &global_best,
        );
    });

    let duration = start.elapsed();

    let result = global_best.lock().unwrap();

    println!("Best Cost: {}", result.cost);

    println!("Best Permutation: {:?}", result.perm);

    println!("Time: {:?}", duration);

    Ok(())
}

fn main() -> Result<()> {
    let data = Data::new("input.txt")?;

    println!("Loaded Data N={}", data.n);

    solve(data)?;

    Ok(())
}
