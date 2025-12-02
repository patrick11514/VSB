use itertools::Itertools;
use rayon::prelude::*;
use std::{
    fmt::{Display, Write},
    fs,
    sync::{Arc, Mutex},
};

struct Data {
    weights: Vec<u32>,
    matrix: Vec<Vec<u32>>,
}

impl Data {
    fn new(path: &str) -> anyhow::Result<Self> {
        let input = fs::read_to_string(path)?;
        let data = input
            .lines()
            .skip(1)
            .map(|l| {
                l.split(' ')
                    .map(|num| num.parse::<u32>().unwrap())
                    .collect::<Vec<_>>()
            })
            .collect::<Vec<_>>();

        let mut matrix = data[1..].to_vec();
        Data::fix_matrix(&mut matrix);

        Ok(Data {
            weights: data[0].to_vec(),
            matrix,
        })
    }

    fn fix_matrix(matrix: &mut Vec<Vec<u32>>) {
        let len = matrix.len();

        for i in 0..len {
            for j in 0..len {
                matrix[j][i] = matrix[i][j];
            }
        }
    }

    fn len(&self) -> usize {
        self.matrix.len()
    }
}

impl Display for Data {
    fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        f.write_str("Weights: ")?;
        for w in &self.weights {
            f.write_fmt(format_args!("{w} "))?;
        }
        f.write_char('\n')?;

        f.write_str("Matrix:\n")?;
        for row in &self.matrix {
            for n in row {
                f.write_fmt(format_args!("{n} "))?;
            }
            f.write_char('\n')?;
        }

        Ok(())
    }
}

fn d(i: usize, j: usize, it: &Vec<usize>, data: &Data) -> f32 {
    let mut frac = (data.weights[it[i]] as f32 + data.weights[it[j]] as f32) / 2f32;

    for k in i..=j {
        frac += data.weights[it[k]] as f32;
    }

    frac
}

fn srflp(it: &Vec<usize>, data: &Data) -> f32 {
    let mut sum = 0f32;
    for i in 0..data.len() {
        for j in (i + 1)..data.len() {
            sum += data.matrix[it[i]][it[j]] as f32 * d(i, j, it, data);
        }
    }

    sum
}

fn solve_prefix(previx: &Vec<usize>, data: &Data)

fn solve(data: Data) -> anyhow::Result<()> {
    let size = data.len();

    let cores = std::thread::available_parallelism()?;

    println!("{:?}", (0..size).permutations(2).collect::<Vec<_>>());

    /*     let best = (0..size)
        .permutations(size)
        .into_iter()
        .par_bridge()
        .map(|iter| (iter.clone(), srflp(&iter, &data)))
        .min_by(|a, b| a.1.partial_cmp(&b.1).unwrap());

    println!("{:?}", best);*/

    Ok(())
}

fn main() -> anyhow::Result<()> {
    let data = Data::new("input.txt")?;

    solve(data)?;

    Ok(())
}
