#!/usr/bin/env -S rust-script -d scanf

use scanf::sscanf;

fn do_overlap(boundaries: ((u32, u32), (u32, u32))) -> bool {
	let (a, b, c, d) = (boundaries.0.0, boundaries.0.1, boundaries.1.0, boundaries.1.1);
	return a <= c && b >= c || a <= d && b >= d || a >= c && b <= d || a <= c && b >= d;
}

fn main() {
	let input = std::fs::read_to_string("input").unwrap();

	let res = input
		.split_terminator("\n")
		.map(|line| {
			let (mut a, mut b, mut c, mut d): (u32, u32, u32, u32) = (0, 0, 0, 0);
			sscanf!(line, "{}-{},{}-{}" , a, b, c, d).ok();

			((a, b), (c, d))
		})
		.map(do_overlap)
		.map(|x| if x { 1 } else { 0 })
		.sum::<u32>();

	println!("{}", res);
}
