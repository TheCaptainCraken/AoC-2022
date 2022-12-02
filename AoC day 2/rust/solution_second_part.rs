#!/usr/bin/env rust-script

fn main() {
	let input = std::fs::read_to_string("input").unwrap();

	let res = input
		.split_terminator("\n") // per evitare l'ultima riga vuota
		.map(|line| {
			let mut split = line.split(' ');
			
			( split.next().unwrap(), split.next().unwrap() )
		})
		.map(|(first, second)| {
			( 
				first.chars().next().unwrap() as u8 - 'A' as u8, 
				second.chars().next().unwrap() as u8 - 'X' as u8
			)
		})
		.map(|(first, second)| {
			(
				first, 
				(first + second + 2) % 3
			)
		})
		.map(|(first, second)| {
			let play_diff = (3 + second - first) % 3; // +3 per evitare numeri negativi
			
			let won_pts = match play_diff {
				0 => 3,
				1 => 6,
				2 => 0,
				_ => panic!("Invalid play diff"),
			};

			let play_pts = second + 1;

			(won_pts + play_pts) as u32
		})
		.sum::<u32>();
		
	println!("{res}");
}
