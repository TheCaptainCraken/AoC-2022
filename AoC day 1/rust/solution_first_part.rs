#!/usr/bin/env rust-script

use std::fs;

fn main() {
	let content = fs::read_to_string("input")
		.expect("Something went wrong reading the file");

	let mut numbers = Vec::new();
	let mut temp: i32 = 0;

	for line in content.lines() {
		if line.len() > 0 {
			temp += line.parse::<i32>().unwrap();
		} else {
			numbers.push(temp);
			temp = 0;
		}
	}

	let max: i32 = numbers.iter().max().unwrap().clone();

	println!("{}", max);
}
