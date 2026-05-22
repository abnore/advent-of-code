Sprinkles:      capacity 2, durability 0, flavor -2, texture 0, calories 3
Butterscotch:   capacity 0, durability 5, flavor -3, texture 0, calories 3
Chocolate:      capacity 0, durability 0, flavor 5, texture -1, calories 8
Candy:          capacity 0, durability -1, flavor 0, texture 5, calories 8


2  0 -2  0  3
0  5 -3  0  3
0  0  5 -1  8
0 -1  0  5  8

     a  b  c  d
c |  2  0  0  0
d |  0  5  0 -1
f | -2 -3  5  0
t |  0  0 -1  5

x |  3  3  8  8

    (2a) (5b - d)(5c -2a -3b)(5d -c)

    (2a)(5c -2a -3b)(5d-c)(5b-d)
    2a(5c -2a -3b) (5d-c)(5b-d)
    (5d-c)(5b-d) = d(20b - 5d + c)

    2ad(5c - 2a - 3b)(20b - 5d + c)

    ad(10c - 4a -6b)(20b - 5d + c)
    ad(10c - 4a -6b)(20b - 5d + c)


Butterscotch:   capacity -1, durability -2, flavor 6, texture 3, calories 8
Cinnamon:       capacity 2, durability 3, flavor -2, texture -1, calories 3


c | -1x +  2y ||
d | -2x +  3y ||
f |  6x + -2y ||
t |  3x + -1y ||
x |  8x +  3y ||

(2y - x)(3y - 2x)(6x - 2y)(3x - y)(8x + 3y)

(2y - x)(3y - 2x) (6x - 2y)(3x - y)
(6yy - 4xy - 3xy + 2xx) (18xx - 6xy - 6xy + 2yy)

(6yy - 7xy + 2xx)(18xx - 12xy + 2yy)

  6yy(18xx - 12xy + 2yy)
- 7xy(18xx - 12xy + 2yy)
+ 2xx(18xx - 12xy + 2yy)
====
  108xxyy - 72yyxy + 12yyyy -126xyxx + 84xyxy - 14xyyy +36xxxx - 24xxxy + 4xxyy
===
  12yyyy + 36xxxx + 196xxyy - 86yyyx - 150xxxy
===

Then, choosing to use 44 teaspoons of butterscotch and 56 teaspoons of cinnamon
(because the amounts of each ingredient must add up to 100) would result in a
cookie with the following properties:

A capacity of 44*-1 + 56*2 = 68
A durability of 44*-2 + 56*3 = 80
A flavor of 44*6 + 56*-2 = 152
A texture of 44*3 + 56*-1 = 76


Multiplying these together (68 * 80 * 152 * 76, ignoring calories for now)
results in a total score of 62842880, which happens to be the best score
possible given these ingredients. If any properties had produced a negative
total, it would have instead become zero, causing the whole score to multiply
to zero.
