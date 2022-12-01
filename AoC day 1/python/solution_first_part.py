with open('input', 'r') as input_file:
    input_file_lines = input_file.readlines()

maximum_sum = 0
calories_sum = 0
for line in input_file_lines:
    if len(line) == 1:
        maximum_sum = max(calories_sum, maximum_sum)
        calories_sum = 0
    else:
        calories_sum += int(line)

print(maximum_sum)
