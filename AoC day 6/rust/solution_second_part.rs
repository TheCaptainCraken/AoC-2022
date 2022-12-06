#!/usr/bin/env -S rust-script

use std::collections::HashSet;

fn main() {
	let input = std::fs::read_to_string("input").unwrap().chars().collect::<Vec<_>>();

	let position = input
		.windows(14)
		.position(|window| {
			window.iter().collect::<HashSet<_>>().len() == window.len()
		})
		.unwrap();

	println!("{}", position + 14);
}
