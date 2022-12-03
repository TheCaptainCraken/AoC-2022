#!/usr/bin/env -S rust-script -d array_tool

use std::fs;
use array_tool::vec::Intersect;

fn main() {
	let input = fs::read_to_string("input").unwrap();

	let result = input.split_terminator('\n')
		.map(|line| {
			line.split_at(line.len() / 2)
		})
		.map(|(first, second)| {
			first.chars().collect::<Vec<_>>().intersect(second.chars().collect::<Vec<_>>())
		})
		.map(|intersection| {
			let c = intersection.first().unwrap().clone();

			c as u8
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