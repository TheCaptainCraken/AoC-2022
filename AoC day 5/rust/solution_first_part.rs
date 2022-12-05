#!/usr/bin/env -S rust-script -d scanf

use scanf::sscanf;

struct Move {
	count: u32,
	from: usize,
	to: usize,
}

fn main() {
	let input = std::fs::read_to_string("input").unwrap();

	let mut iterator = input.split("\n\n");
	let (initial_state, moves) = (iterator.next().unwrap(), iterator.next().unwrap());

	let mut state = parse_initial_state(initial_state);
	let moves = parse_moves(moves);
	compute_moves(&mut state, moves);
	print_result(state);
}

fn parse_initial_state(input: &str) -> [Vec<char>; 9] {
	let mut state: [Vec<char>; 9] = Default::default();

	for line in input.lines() {
		for (i, c) in line.chars().enumerate() {
			if (c as u8) >= b'A' && (c as u8) <= b'Z' {
				state[(i - 1) / 4].push(c);
			}
		}
	}

	for vec in state.iter_mut() {
		vec.reverse();
	}

	return state
}

fn parse_moves(input: &str) -> Vec<Move> {
	input.lines()
		.map(|line| {
			let (mut count, mut from, mut to) = (0, 0, 0);
			sscanf!(line, "move {} from {} to {}", count, from, to).ok();

			Move { count: count, from: from - 1, to: to - 1 }
		})
		.collect()
}

fn compute_moves(state: &mut [Vec<char>; 9], moves: Vec<Move>) {
	for mov in moves {
		for _i in 0..mov.count {
			let c = state[mov.from].pop().unwrap();
			state[mov.to].push(c);
		}
	}
}

fn print_result(state: [Vec<char>; 9]) {
	for i in 0..9 {
		print!("{}", state[i].last().unwrap());
	}
	print!("\n");
}
