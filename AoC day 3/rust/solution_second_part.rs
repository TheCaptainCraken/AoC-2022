#!/usr/bin/env -S rust-script -d array_tool

use std::fs;
use array_tool::vec::Intersect;

fn main() {
	let input = fs::read_to_string("input").unwrap();

	let result = input.split_terminator('\n')
		.collect::<Vec<_>>()
		.chunks(3)
		.map(|chunks| {
			let c = chunks[0].chars().collect::<Vec<_>>()
				.intersect(chunks[1].chars().collect::<Vec<_>>())
				.intersect(chunks[2].chars().collect::<Vec<_>>());

			c.first().unwrap().clone() as u8
		})
		.map(|c| {
			let score = if c >= 'a' as u8 {
				c - ('a' as u8) + 1
			} else {
				c - ('A' as u8) + 27
			};

			score as u32
		})
		.sum::<u32>();

	println!("{result}");
}