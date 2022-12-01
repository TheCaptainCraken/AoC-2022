with open('input', 'r') as input_file:
    input_file_lines = input_file.readlines()

best_ones = [0, 0, 0]
calories_sum = 0
for line in input_file_lines:
    if len(line) == 1:
        if calories_sum > best_ones[0]:
            best_ones[2] = best_ones[1]
            best_ones[1] = best_ones[0]
            best_ones[0] = calories_sum
        elif calories_sum > best_ones[1]:
            best_ones[2] = best_ones[1]
            best_ones[1] = calories_sum
        elif calories_sum > best_ones[2]:
            best_ones[2] = calories_sum
        calories_sum = 0
    else:
        calories_sum += int(line)

print(sum(best_ones))
