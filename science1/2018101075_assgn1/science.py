import matplotlib.pyplot as pyplot
import random

# We want to check probability of two drunk men reaching to same place after N steps given there is equal probability to move forward and backward
# We check for N = 1 to 50
# For each N, we check position of two drunk men x, y after N steps 100 times and checks if x equals y
# Probability of x=y for any N is (no. of times x=y)/100
# Graph is ploted to show probabilty for N

arr = []
for i in range(1, 50):
    ans = 0
    for j in range(1, 100):
        x = 0
        y = 0
        for k in range(0, i):
            if random.random() >= 0.5:
                x = x + 1
            else:
                x = x - 1
            if random.random() >= 0.5:
                y = y + 1
            else:
                y = y - 1
        if x == y:
            ans = ans + 1
    arr.append(ans/100)

pyplot.plot(arr)
pyplot.show()