#!/usr/bin/env -S rust-script -d scanf

use scanf::sscanf;

fn is_contained(boundaries: ((u32, u32), (u32, u32))) -> bool {
	return boundaries.0.0 <= boundaries.1.0 && boundaries.0.1 >= boundaries.1.1 
		|| boundaries.0.0 >= boundaries.1.0 && boundaries.0.1 <= boundaries.1.1;
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
		.map(is_contained)
		.map(|x| if x { 1 } else { 0 })
		.sum::<u32>();

	println!("{}", res);
}
