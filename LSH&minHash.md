##minHash (for local sensetive hashing)
1. Find similar document, brute force method O(n^2)
2. Use hash function:

disadvantages:Two similar documents may have very different hash value
=> should find a new hash algorithm to 
* Calculate document similarity
Jacccard
SIM(S,T) = |S intersect T|/|S U T|
* similarity using mareix

```
S1 = "I eat"
S2 = "want"
S3 = "I want to eat"
S4 = "He want to success"
```

vocabulary | S1 | S2 | S3 | S4
------------ | ------------- | ------------- | ------------- | -------------
I | 1 | 0 | 1 | 0
he | 0 | 0 | 0 | 1
want | 0 | 1 | 1 | 1
to | 0 | 0 | 1 | 1
eat | 1 | 0 | 1 | 0
success | 0 | 0 | 0 | 1



* minHashing
randomlize the row of matrix

vocabulary | S1 | S2 | S3 | S4
------------ | ------------- | ------------- | ------------- | -------------
to | 0 | 0 | 1 | 1
he | 0 | 0 | 0 | 1
success | 0 | 0 | 0 | 1
want | 0 | 1 | 1 | 1
eat | 1 | 0 | 1 | 0
I | 1 | 0 | 1 | 0


minimun hash value:
h(S1) = 5, h(S2) = 4, h(S3) = 1, h(S4) = 1

The minimum hash value is the estimation of Jaccard similarity. P(h(Si) = h(Sj)) = sim(Si,Sj)
3. Minimun hash signature
One minimum hash is a dependent random event. According to the central-limit theorem, repeatly random event can cause inevitability
Select n random rank for characteristic Matrix to get a n-dimention vector consist of n minimum hash value, which is the minimumsignature.

How to calculate minimum signatire
* waste time to randomlize a huge characteristic matrix
* find a hash function put h(r) at the rth SIG(i,c): The ith hash function on the column c

```python
if matrix[r][c] == 1:
  for i in xrange(n): # for each hash function
  SIG(i,c) = min(SIG(i, C), hi(r))
else:
  continue
```

vocabulary | set1 | set 2 | H1(x+1) % 7 | H2(3X+1) % 7
------------ | ------------- | ------------- | ------------- | -------------
wo 0 | 1 | 0 | 1 | 1
or 1 | 1 | 0 | 2 | 4
rl 2 | 1 | 0 | 3 | 0
ld 3 | 1 | 1 | 4 | 3
co 4 | 0 | 1 | 5 | 6
ou 5 | 0 | 1 | 6 | 2
ul 6 | 0 | 1 | 0 | 5

So the matrix is convert to following:

row | set1 | set 2
------------ | ------------- | ------------- 
H1 | 1 | 0
H2 | 0 | 2


4. Local sensitive hashing based on min-hash
* divide  the signature mareix into small group, each group with r rows

p | 1 - (1 -p^5)^20
------------ | -------------
0.1 | 0.9992
0.2 | 0.0064
0.3 | 0.0475
0.4 | 0.1860
0.5 | 0.4701
0.6 | 0.8019
0.7 | 0.9748
0.8 | 0.9996
0.9 | 1.0000

When the similarity of two documents is 0.8, the probobility that they hashed
to the same slot is 0.9996, but when their similarity is 0.3, the probability to become candidate pair is only 0.0475. So the
local sensetive hashing solve the problem.

For the same slot in the hash table, we should also test due to the collision.
