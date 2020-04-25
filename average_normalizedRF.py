file = open("normalizedRF.txt")

total = 0
sum = 0
for i in file:
    line = i.strip().split(" ")
    sum += float(line[3])
    total += 1

print(sum/total)