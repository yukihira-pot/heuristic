import random

N, K = map(int, input().split())
print(N, K)
for i in range(N):
    for j in range(N):
        print(random.randint(0, K), end="")
    print()