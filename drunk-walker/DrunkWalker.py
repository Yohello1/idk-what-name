# libraries
import random

# Global variables
mapWidth = 50
mapHeight = 50
default = "#"

# map generation
grid = [default] * mapHeight # initializaing list of 'rows'
for i in range(mapHeight):
    grid[i] = [default] * mapWidth # filling each 'row' with a list of its own

for row in grid:
    print(*row)

# drunkard's walk algorithm
x = mapWidth//2
y = mapHeight//2

for k in range(100):
    grid[y][x] = "."
    randnum = random.randint(1, 4)
    if randnum == 1:
        direction = "north"
        y -= 1
    if randnum == 2:
        direction = "east"
        x += 1
    if randnum == 3:
        direction = "south"
        y += 1
    if randnum == 4:
        direction = "west"
        x -= 1
    print("--------------------------------------")
    for i in range(len(grid)):
        print(*grid[i])