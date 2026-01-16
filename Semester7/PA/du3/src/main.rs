use crossbeam::channel::bounded;
use rayon::prelude::*;
use std::collections::HashSet;
use std::fs::File;
use std::io::{BufRead, BufReader};
use std::thread;
use std::time::Instant;

const DAMPING_FACTOR: f64 = 0.85;
const ITERATIONS: usize = 20;
const CHUNK_SIZE: usize = 100_000;

#[derive(Debug, Clone)]
struct Edge {
    from: u32,
    to: u32,
}

fn main() {
    let start_total = Instant::now();
    let file_path = "web-BerkStan.txt";

    println!("Starting PageRank calculation on {}", file_path);

    let (tx, rx) = bounded::<Vec<String>>(100);

    let num_threads = num_cpus::get();
    println!("Using {} threads for parsing", num_threads);

    let mut handles = Vec::new();

    for _ in 0..num_threads {
        let rx = rx.clone();
        handles.push(thread::spawn(move || {
            let mut local_edges = Vec::new();
            let mut local_nodes = HashSet::new();

            while let Ok(lines) = rx.recv() {
                for line in lines {
                    if line.starts_with('#') {
                        continue;
                    }
                    let parts: Vec<&str> = line.split_whitespace().collect();
                    if parts.len() >= 2
                        && let (Ok(u), Ok(v)) = (parts[0].parse::<u32>(), parts[1].parse::<u32>())
                    {
                        local_edges.push(Edge { from: u, to: v });
                        local_nodes.insert(u);
                        local_nodes.insert(v);
                    }
                }
            }
            (local_edges, local_nodes)
        }));
    }

    let reader_handle = thread::spawn(move || {
        let file = File::open(file_path).expect("Could not open file");
        let reader = BufReader::new(file);

        let mut chunk = Vec::with_capacity(CHUNK_SIZE);
        for line_res in reader.lines() {
            let line = line_res.expect("Error reading line");
            chunk.push(line);
            if chunk.len() >= CHUNK_SIZE {
                tx.send(chunk).expect("Send failed");
                chunk = Vec::with_capacity(CHUNK_SIZE);
            }
        }
        if !chunk.is_empty() {
            tx.send(chunk).expect("Send failed");
        }
    });

    reader_handle.join().expect("Reader failed");

    let mut all_edges = Vec::new();
    let mut unique_nodes = HashSet::new();

    for h in handles {
        let (edges, nodes) = h.join().expect("Worker failed");
        all_edges.extend(edges);
        unique_nodes.extend(nodes);
    }

    println!(
        "Graph loaded: {} nodes, {} edges",
        unique_nodes.len(),
        all_edges.len()
    );
    let parsing_duration = start_total.elapsed();
    println!("Parsing took: {:?}", parsing_duration);

    let mut nodes_vec: Vec<u32> = unique_nodes.into_iter().collect();
    nodes_vec.par_sort_unstable();

    let num_nodes = nodes_vec.len();

    let mut out_degree = vec![0usize; num_nodes];

    let internal_edges: Vec<(usize, usize)> = all_edges
        .par_iter()
        .map(|e| {
            let u_idx = nodes_vec.binary_search(&e.from).unwrap();
            let v_idx = nodes_vec.binary_search(&e.to).unwrap();
            (u_idx, v_idx)
        })
        .collect();

    for &(u, _v) in &internal_edges {
        out_degree[u] += 1;
    }

    let mut sorted_edges = internal_edges.clone();

    sorted_edges.par_sort_unstable_by_key(|&(_, v)| v);

    let mut adj_in = vec![Vec::new(); num_nodes];
    for (u, v) in sorted_edges {
        adj_in[v].push(u);
    }

    println!("Graph constructed.");

    let mut pr = vec![1.0 / num_nodes as f64; num_nodes];
    let mut next_pr = vec![0.0; num_nodes];

    let base_score = (1.0 - DAMPING_FACTOR) / num_nodes as f64;

    for i in 0..ITERATIONS {
        let iter_start = Instant::now();

        next_pr.par_iter_mut().enumerate().for_each(|(u_idx, val)| {
            let mut sum = 0.0;
            if u_idx < adj_in.len() {
                for &v_idx in &adj_in[u_idx] {
                    if out_degree[v_idx] > 0 {
                        sum += pr[v_idx] / out_degree[v_idx] as f64;
                    }
                }
            }
            *val = base_score + DAMPING_FACTOR * sum;
        });

        let current_sum: f64 = next_pr.par_iter().sum();
        let missing = 1.0 - current_sum;
        if missing > 0.0 {
            let escape = missing / num_nodes as f64;
            next_pr.par_iter_mut().for_each(|val| *val += escape);
        }

        std::mem::swap(&mut pr, &mut next_pr);

        println!(
            "Iteration {} completed in {:?}",
            i + 1,
            iter_start.elapsed()
        );
    }

    let total_sum: f64 = pr.par_iter().sum();
    println!("Total PR sum: {}", total_sum);

    let mut ranked_nodes: Vec<(usize, f64)> = pr.iter().cloned().enumerate().collect();
    ranked_nodes.par_sort_unstable_by(|a, b| b.1.partial_cmp(&a.1).unwrap());

    println!("Top 5 Nodes:");
    for i in 0..5 {
        if i < ranked_nodes.len() {
            let (idx, score) = ranked_nodes[i];
            let original_id = nodes_vec[idx];
            println!("{}. Node ID: {}, Score: {:.6}", i + 1, original_id, score);
        }
    }

    println!("Total execution time: {:?}", start_total.elapsed());
}
