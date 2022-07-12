# 2022-03-03 Lecture on normals and the chain rule

Consider a shape that is a level curve of a function $f: \mathbb{R}^2 \to \mathbb{R}$. \\
Suppose that curve is parametrized by $(x, y) = (X(t), Y(t))$. \\
That is, $f(x, y) = c$ for some $c \in \mathbb{r}$. \\
Recall that the gradient of $f$ is normal to the its level curves. \\
Thus, the tangent vector on the curve $T = \left\langle \frac{dX}{dt}, \frac{dY}{dt}\right\rangle$ is normal to the gradient $\vec{\mathrm{grad}}(f) = \nabla f = \left\langle \frac{\partial f}{\partial x}, \frac{\partial f}{\partial y} \right\rangle$. \\
Observe $\nabla f \cdot T =  \frac{\partial f}{\partial x} \frac{dX}{dt} + \frac{\partial f}{\partial y} \frac{dY}{dt}$. \\
On the level curve $f(x, y) = c$, $x = X(t)$ and $y = y(t)$, so $\nabla f \cdot T = \frac{\partial f}{\partial x} \frac{dx}{dt} + \frac{\partial f}{\partial y} \frac{dy}{dt} = \frac{df}{dt}$. \\
On the level curve $f(x, y) = f(X(t), Y(t)) = c$, $f$ is constant, so $\frac{df}{dt} = 0$. \\
Thus $\nabla f \cdot T = 0$, so the gradient is perpendicular to the level curve. \\

# 2022-04-05 Raytracing a torus and solving a quartic
First, we parametrize a torus with big radius $R$ and little radius $r$ ($R > r > 0$):
$$
\begin{pmatrix}
    x \\
    y \\
    z \\
\end{pmatrix}
=
\begin{pmatrix}
    (R + r\cos\phi) \cdot \cos\theta \\
    (R + r\cos\phi) \cdot \sin\theta \\
    r\sin\phi
\end{pmatrix}
$$
and $\theta, \phi \in [0, \tau)$.

Let's reverse parametrize the torus.
$$\begin{aligned}
    \tan\theta &= \frac{y}{x} & \\
    \theta &= \arctan \left( y, x \right) & \text{(And then adjust to be in $[0, \tau)$)} \\
    z &= r\sin\phi & \\
    \sqrt{x^2 + y^2} - R &= r\cos\phi & \\
    \phi &= \arctan \left( z, \sqrt{x^2 + y^2} - R \right) & \text{(Again, adjust the angle)} \\
\end{aligned}$$

Then we derive the implicit equation:
$$\begin{aligned}
    x^2 + y^2 &= \left( (R + r\cos\phi) \cdot \cos\theta \right)^2 +  \left( (R + r\cos\phi) \cdot \sin\theta \right)^2 & \\
    &= (R + r\cos\phi)^2 \left( \cos^2 \theta + \sin^2 \theta \right) & \\
    &= (R + r\cos\phi)^2 & \\
    \sqrt{x^2 + y^2} &= R + r\cos\phi & \text{(Both sides are positive)} \\
    \sqrt{x^2 + y^2} - R &= r\cos\phi & \\
    \left( \sqrt{x^2 + y^2} - R \right)^2 + z^2 &= \left( r\cos\phi \right)^2 + \left( r\sin\phi \right)^2 & \\
    \left( \sqrt{x^2 + y^2} - R \right)^2 + z^2 &= r^2 \left( \cos^2 \phi + \sin^2 \phi \right)^2 & \\
    \left( \sqrt{x^2 + y^2} - R \right)^2 + z^2 &= r^2. & \\
\end{aligned}$$

This corresponds to the level surface
$$
    0 = f(x, y) = \left( \sqrt{x^2 + y^2} - R \right)^2 + z^2 - r^2
$$

Now find the gradient:
$$
\nabla  f = \begin{pmatrix}
    \frac{\partial f}{\partial x} \\
    \frac{\partial f}{\partial y} \\
    \frac{\partial f}{\partial z} \\
\end{pmatrix} = \begin{pmatrix}
    2\left( \sqrt{x^2 + y^2} - R \right) \cdot \frac{1}{2\sqrt{x^2 + y^2}} \cdot 2x \\
    2\left( \sqrt{x^2 + y^2} - R \right) \cdot \frac{1}{2\sqrt{x^2 + y^2}} \cdot 2y \\
    2z \\
\end{pmatrix} = \begin{pmatrix}
    2x \left( 1 - \frac{R}{\sqrt{x^2 + y^2}} \right)\\
    2y \left( 1 - \frac{R}{\sqrt{x^2 + y^2}} \right)\\
    2z \\
\end{pmatrix}.
$$

Rewrite the implicit equation:
$$\begin{aligned}
    0 &= \left( \sqrt{x^2 + y^2} - R \right)^2 + z^2 - r^2 \\
    &= x^2 + y^2 - 2R\sqrt{x^2 + y^2} + R^2 + z^2 - r^2 \\
    2R\sqrt{x^2 + y^2} &= x^2 + y^2 + R^2 + z^2 - r^2 \\
    4R^2 (x^2 + y^2) &= (x^2 + y^2 + R^2 + z^2 - r^2)^2 \\
    0 &= \left( (x^2 + y^2 + z^2) + (R^2 - r^2) \right)^2 - 4R^2 (x^2 + y^2) \\
    &= (x^2 + y^2 + z^2)^2 + 2(R^2 - r^2) \cdot (x^2 + y^2 + z^2) + (R^2 - r^2)^2 - 4R^2 (x^2 + y^2) \\
    &= \left( x^4 + y^4 + z^4 + 2 \left( x^2 y^2 + y^2 z^2 + z^2 x^2 \right) \right) + 2(R^2 - r^2) \cdot (x^2 + y^2 + z^2) + (R^2 - r^2)^2 - 4R^2 (x^2 + y^2) \\
\end{aligned}$$

Take a point on the ray:
$$
\begin{pmatrix}
    x \\
    y \\
    z \\
\end{pmatrix} = \begin{pmatrix}
    x_0 + t \Delta x \\
    y_0 + t \Delta y \\
    z_0 + t \Delta z \\
\end{pmatrix}.
$$

For reference, compute (with Pascal's Triangle):
$$\begin{aligned}
    x^2 =& (x_0 + t \Delta x)^2 \\
    =& x_0^2 + 2 x_0 t \Delta x + t^2 \Delta x^2 \\
    x^2 y^2 =& \left( x_0^2 + 2 x_0 t \Delta x + t^2 \Delta x^2 \right) \cdot \left( y_0^2 + 2 y_0 t \Delta y + t^2 \Delta y^2 \right) \\
    =& x_0^2 y_0^2 + x_0^2 \cdot 2 y_0 t \Delta y + x_0^2 t^2 \Delta y^2 \\
    &+ 2 x_0 t \Delta x y_0^2 + 2 x_0 t \Delta x \cdot 2 y_0 t \Delta y + 2 x_0 t \Delta x t^2 \Delta y^2 \\
    &+ t^2 \Delta x^2 y_0^2 + t^2 \Delta x^2 \cdot 2 y_0 t \Delta y + t^2 \Delta x^2 t^2 \Delta y^2 \\
    =& (x_0 y_0)^2 + t \cdot 2(x_0^2 y_0 \Delta y + y_0^2 x_0 \Delta x) + t^2 (x_0^2 \Delta y^2 + 4 x_0 y_0 \Delta x \Delta y + y_0^2 \Delta x^2) + t^3 \cdot 2(x_0 \Delta x \Delta y^2 + y_0 \Delta y \Delta x^2) + t^4 (\Delta x \Delta y)^2 \\
    x^4 =& (x_0 + t \Delta x)^4 \\
    =& x_0^4 + 4 x_0^3 t \Delta x + 6 x_0^2 t^2 \Delta x^2 + 4 x_0 t^3 \Delta x^3 + t^4 \Delta x^4 \\
\end{aligned}$$
We can compute the rest by cyclic permutation of variables: $x \to y \to z \to x$.
Plug these into the prior equation and hope that I didn't make any mistakes that I haven't found yet:
$$\begin{aligned}
    0 =& x^4 + y^4 + z^4 + 2 x^2 y^2 + 2 y^2 z^2 + 2 z^2 x^2 + 2(R^2 - r^2) \cdot (x^2 + y^2 + z^2) + (R^2 - r^2)^2 - 4R^2 (x^2 + y^2) \\
    =& (x_0^4 + 4 x_0^3 t \Delta x + 6 x_0^2 t^2 \Delta x^2 + 4 x_0 t^3 \Delta x^3 + t^4 \Delta x^4) \\
    &+ (y_0^4 + 4 y_0^3 t \Delta y + 6 y_0^2 t^2 \Delta y^2 + 4 y_0 t^3 \Delta y^3 + t^4 \Delta y^4) \\
    &+ (z_0^4 + 4 z_0^3 t \Delta z + 6 z_0^2 t^2 \Delta z^2 + 4 z_0 t^3 \Delta z^3 + t^4 \Delta z^4) \\
    &+ 2 ((x_0 y_0)^2 + t \cdot 2(x_0^2 y_0 \Delta y + y_0^2 x_0 \Delta x) + t^2 (x_0^2 \Delta y^2 + 4 x_0 y_0 \Delta x \Delta y + y_0^2 \Delta x^2) + t^3 \cdot 2(x_0 \Delta x \Delta y^2 + y_0 \Delta y \Delta x^2) + t^4 (\Delta x \Delta y)^2) \\
    &+ 2 ((y_0 z_0)^2 + t \cdot 2(y_0^2 z_0 \Delta z + z_0^2 y_0 \Delta y) + t^2 (y_0^2 \Delta z^2 + 4 y_0 z_0 \Delta y \Delta z + z_0^2 \Delta y^2) + t^3 \cdot 2(y_0 \Delta y \Delta z^2 + z_0 \Delta z \Delta y^2) + t^4 (\Delta y \Delta z)^2) \\
    &+ 2 ((z_0 x_0)^2 + t \cdot 2(z_0^2 x_0 \Delta x + x_0^2 z_0 \Delta z) + t^2 (z_0^2 \Delta x^2 + 4 z_0 x_0 \Delta z \Delta x + x_0^2 \Delta z^2) + t^3 \cdot 2(z_0 \Delta z \Delta x^2 + x_0 \Delta x \Delta z^2) + t^4 (\Delta z \Delta x)^2) \\
    &+ 2(R^2 - r^2) \cdot ((x_0^2 + 2 x_0 t \Delta x + t^2 \Delta x^2) + (y_0^2 + 2 y_0 t \Delta y + t^2 \Delta y^2) + (z_0^2 + 2 z_0 t \Delta z + t^2 \Delta z^2)) \\
    &+ (R^2 - r^2)^2 \\
    &- 4R^2 ((x_0^2 + 2 x_0 t \Delta x + t^2 \Delta x^2) + (y_0^2 + 2 y_0 t \Delta y + t^2 \Delta y^2)) \\
    =& (R^2 - r^2)^2 + x_0^4 + y_0^4 + z_0^4 + 2 ((x_0 y_0)^2 + (y_0 z_0)^2 + (z_0 x_0)^2) + 2 (R^2 - r^2) (x_0^2 + y_0^2 + z_0^2) - 4R^2 (x_0^2 + y_0^2) \\
    &+ t \cdot 4 (x_0^3 \Delta x + y_0^3 \Delta y + z_0^3 \Delta z + x_0^2 y_0 \Delta y + y_0^2 x_0 \Delta x + y_0^2 z_0 \Delta z + z_0^2 y_0 \Delta y + z_0^2 x_0 \Delta x + x_0^2 z_0 \Delta z + (R^2 - r^2) (x_0 \Delta x + y_0 \Delta y + z_0 \Delta z) - 2R^2 (x_0 \Delta x + y_0 \Delta y)) \\
    &+ t^2 \cdot \left( 6 (x_0^2 \Delta x^2 + y_0^2 \Delta y^2 + z_0^2 \Delta z^2) + 2 (x_0^2 \Delta y^2 + 4 x_0 y_0 \Delta x \Delta y + y_0^2 \Delta x^2 + y_0^2 \Delta z^2 + 4 y_0 z_0 \Delta y \Delta z + z_0^2 \Delta y^2 + z_0^2 \Delta x^2 + 4 z_0 x_0 \Delta z \Delta x + x_0^2 \Delta z^2) + 2 (R^2 - r^2) (\Delta x^2 + \Delta y^2 + \Delta z^2) - 4R^2 (\Delta x^2 + \Delta y^2) \right) \\
    &+ t^3 \cdot 4 (x_0 \Delta x^3 + y_0 \Delta y^3 + z_0 \Delta z^3 + x_0 \Delta x \Delta y^2 + y_0 \Delta y \Delta x^2 + y_0 \Delta y \Delta z^2 + z_0 \Delta z \Delta y^2 + z_0 \Delta z \Delta x^2 + x_0 \Delta x \Delta z^2) \\
    &+ t^4 \cdot \left( \Delta x^4 + \Delta y^4 + \Delta z^4 + 2 ((\Delta x \Delta y)^2 + (\Delta y \Delta z)^2 + (\Delta z \Delta x)^2) \right) \\
\end{aligned}$$
Now we can read off the coefficients and plug them into a polynomial solver.
I'll make some operations a bit more explicit, for convenience of translating directly into code:
    $$\begin{aligned}
        0 =& ((R)^2 - (r)^2)^2 + (x_0)^4 + (y_0)^4 + (z_0)^4 + 2 \cdot ((x_0 \cdot y_0)^2 + (y_0 \cdot z_0)^2 + (z_0 \cdot x_0)^2) + 2 \cdot ((R)^2 - (r)^2) \cdot ((x_0)^2 + (y_0)^2 + (z_0)^2) - 4 \cdot (R)^2 \cdot ((x_0)^2 + (y_0)^2) \\
        &+ t \cdot 4 \cdot ((x_0)^3 \cdot \Delta x + (y_0)^3 \cdot \Delta y + (z_0)^3 \cdot \Delta z + (x_0)^2 \cdot y_0 \cdot \Delta y + (y_0)^2 \cdot x_0 \cdot \Delta x + (y_0)^2 \cdot z_0 \cdot \Delta z + (z_0)^2 \cdot y_0 \cdot \Delta y + (z_0)^2 \cdot x_0 \cdot \Delta x + (x_0)^2 \cdot z_0 \cdot \Delta z + ((R)^2 - (r)^2) \cdot (x_0 \cdot \Delta x + y_0 \cdot \Delta y + z_0 \cdot \Delta z) - 2 \cdot (R)^2 \cdot (x_0 \cdot \Delta x + y_0 \cdot \Delta y)) \\
        &+ t^2 \cdot \left( 6 \cdot ((x_0 \cdot \Delta x)^2 + (y_0 \cdot \Delta y)^2 + (z_0 \cdot \Delta z)^2) + 2 \cdot ((x_0 \cdot \Delta y)^2 + 4 \cdot x_0 \cdot y_0 \cdot \Delta x \cdot \Delta y + (y_0 \cdot \Delta x)^2 + (y_0 \cdot \Delta z)^2 + 4 \cdot y_0 \cdot z_0 \cdot \Delta y \cdot \Delta z + (z_0 \cdot \Delta y)^2 + (z_0 \cdot \Delta x)^2 + 4 \cdot z_0 \cdot x_0 \cdot \Delta z \cdot \Delta x + (x_0 \cdot \Delta z)^2) + 2 \cdot ((R)^2 - (r)^2) \cdot ((\Delta x)^2 + (\Delta y)^2 + (\Delta z)^2) - 4 \cdot (R)^2 \cdot ((\Delta x)^2 + (\Delta y)^2) \right) \\
        &+ t^3 \cdot 4 \cdot (x_0 \cdot (\Delta x)^3 + y_0 \cdot (\Delta y)^3 + z_0 \cdot (\Delta z)^3 + x_0 \cdot \Delta x \cdot (\Delta y)^2 + y_0 \cdot \Delta y \cdot (\Delta x)^2 + y_0 \cdot \Delta y \cdot (\Delta z)^2 + z_0 \cdot \Delta z \cdot (\Delta y)^2 + z_0 \cdot \Delta z \cdot (\Delta x)^2 + x_0 \cdot \Delta x \cdot (\Delta z)^2) \\
        &+ t^4 \cdot \left( (\Delta x)^4 + (\Delta y)^4 + (\Delta z)^4 + 2 \cdot ((\Delta x \cdot \Delta y)^2 + (\Delta y \cdot \Delta z)^2 + (\Delta z \cdot \Delta x)^2) \right) \\
    \end{aligned}$$
This should be somewhat straightforward to translate into code:
- Remove the preceding power of _t_ and alignment junk.
- Replace `\cdot` with `*`.
- Remove all `\left` and `\right` (but not the associated parentheses).
- Replace `x_0` and `\Delta x` and so on as needed.
- All the powers are wrapped in parentheses, which might be turned into functions calls.

# 2022-04-07 Solving a cubic

## History
del Ferro solved depressed cubics.
Fior learned from del Ferro.
Tartaglia solved cubics with no linear ($t$) term.
Cardano (1539) solved the general cubic.
Ferrare solved the quartic.

## Example of Del Ferro's method
Consider a cubic $y^3 - 9y + 28 = 0$. \\
Let $y = z - \frac{-9}{3z} = z + \frac{3}{z}$. \\
Then $\left( z + \frac{3}{z} \right)^3 - 9 \left( z + \frac{3}{z} \right) + 28 = 0$. \\
$\left[ z^3 + 3 z^2 \frac{3}{z} + 3 z \left( \frac{3}{z} \right)^2 + \left( \frac{3}{z} \right)^3 \right] - 9z - \frac{27}{z} + 28 = 0$. \\
$z^3 + 9z + \frac{27}{z} + \frac{27}{z^3} - 9z - \frac{27}{z} + 28 = 0$. \\
$z^3 + \frac{27}{z^3}+ 28 = 0$. \\
$z^6 + 28z^3 + 27 = 0$. \\
Let $w = z^3$. \\
$w^2 + 28w + 27 = 0$. \\
Factor: $(w + 27) (w + 1) = 0$. \\
$w = -1 \text{ or } -27$. \\
Look at $w = -1$. \\
$\to z = \sqrt[3]{w} = \sqrt[3]{-1} = -1$. \\
$\to y = z + \frac{3}{z} = -1 + \frac{3}{-1} = -4$. \\
Look at $w = -27$. \\
$\to z = \sqrt[3]{w} = \sqrt[3]{-27} = -3$. \\
$\to y = z + \frac{3}{z} = -3 + \frac{3}{-3} = -4$. \\

## Cardano's method
Consider a polynomial $f(t) = At^3 + Bt^2 + Ct + D = 0$. \\
Rewrite: $t^3 + \frac{B}{A}t^2 + \frac{C}{A}t + \frac{D}{A} = 0$. \\
Let $b = \frac{B}{A}, c = \frac{C}{A}, d = \frac{D}{A}$. \\
Let $t = y - \frac{b}{3}$. \\
Then the polynomial becomes
    $\left( y - \frac{b}{3} \right)^3 + b \left( y - \frac{b}{3} \right)^2 + c \left( y - \frac{b}{3} \right) + d = 0$. \\
$y^3 - 3y^2 \frac{b}{3} + 3y \left( \frac{b}{3} \right)^2 - \left( \frac{b}{3} \right)^3 + b \left[ y^2 - 2y \frac{b}{3} + \left( \frac{b}{3} \right)^2 \right] + c \left[ y - \frac{b}{3} \right] + d = 0$. \\
$y^3 \color{red}{- by^2} + \frac{b^2 y}{3} - \frac{b^3}{27} \color{red}{+ by^2} - \frac{2b^2y}{3} + \frac{b^3}{9} + cy - \frac{bc}{3} + d = 0$. \\
$y^3 + y \left( \frac{b^2}{3} - \frac{2b^2}{3} + c \right) + \left( -\frac{b^3}{27} + \frac{b^3}{9} - \frac{bc}{3} + d \right )= 0$. \\
$y^3 + y \left( -\frac{b^2}{3} + c \right) + \left( \frac{2b^3}{27} - \frac{bc}{3} + d \right )= 0$. \\
Then solve by Del Ferro's method.

## Generalizing del Ferro
Consider a cubic $y^3 + by + c = 0$. \\
Let $y = z - \frac{b}{3z}$. \\
Then $\left( z - \frac{b}{3z} \right)^3 + b \left( z - \frac{b}{3z} \right) + c = 0$. \\
$\left[ z^3 - 3 z^2 \frac{b}{3z} + 3 z \left( \frac{b}{3z} \right)^2 - \left( \frac{b}{3z} \right)^3 \right] + \left[ bz - b\frac{b}{3z} \right] + c = 0$. \\
$ z^3 \color{red}{- bz + \frac{b^2}{3z}} - \frac{b^3}{27 z^3} \color{red}{+ bz - \frac{b^2}{3z}} + c = 0$. \\
$z^3 - \frac{b^3}{27 z^3} + c = 0$. \\
$z^6 + c z^3 - \frac{b^3}{27} = 0$. \\
Let $w = z^3$. \\
$w^2 + cw - \frac{b^3}{27} = 0$. \\
Apply quadratic formula:
    $w = \frac{-c \pm \sqrt{c^2 + \frac{4b^3}{27}}}{2}$. \\

$\to z = \sqrt[3]{w}$. \\
$$\begin{aligned}
    y &= z - \frac{b}{3z} \\
    &= \sqrt[3]{w} - \frac{b}{3 \sqrt[3]{w}} \\
    &= \sqrt[3]{w} - \sqrt[-3]{ \frac{27 w}{b^3}} \\
    &= \sqrt[3]{\frac{-c \pm \sqrt{c^2 + \frac{4b^3}{27}}}{2}} - \sqrt[-3]{ \frac{27 \left(-c \pm \sqrt{c^2 + \frac{4b^3}{27}} \right)}{2 b^3}} \\
    &= \sqrt[3]{\frac{-c \pm \sqrt{c^2 + \frac{4b^3}{27}}}{2}} - \sqrt[-3]{\frac{\frac{-27c}{b^3} \pm \sqrt{\frac{27^2}{b^6} \left( c^2 + \frac{4b^3}{27} \right)}}{2}} \quad \text{(We don't lose the sign of $b^3$ when moving it into the square root because of the $\pm$.} \\
    &= \sqrt[3]{\frac{-c \pm \sqrt{c^2 + \frac{4b^3}{27}}}{2}} - \sqrt[-3]{\frac{\frac{-27c}{b^3} \pm \sqrt{\left( \frac{27 c}{b^3} \right)^2 + \frac{4 \cdot 27}{b^3}}}{2}} \\
\end{aligned}$$

Alternate path (bad reverse proof) of equality

$$\begin{aligned}
    y_+ &= y_- \\
    z_+ - \frac{b}{3z_+} &= z_- - \frac{b}{3z_-} \\
    \sqrt[3]{w_+} - \frac{b}{3\sqrt[3]{w_+}} &= \sqrt[3]{w_-} - \frac{b}{3\sqrt[3]{w_-}} \\
    3 \sqrt[3]{w_+^2 w_-} - b \sqrt[3]{w_-} &= 3 \sqrt[3]{w_+ w_-^2} - b \sqrt[3]{w_+} \\
    0 &= b \sqrt[3]{w_-}  + 3 \sqrt[3]{w_+ w_-^2} - 3 \sqrt[3]{w_+^2 w_-} - b \sqrt[3]{w_+} \\
\end{aligned}$$

## Complex Analysis

From page 2:
$x^3 + bx + c = 0$ has solution
$x = \sqrt[3]{-\frac{c}{2} + \sqrt{\frac{c^2}{4} + \frac{b^3}{27}}} + \sqrt[3]{-\frac{c}{2} - \sqrt{\frac{c^2}{4} + \frac{b^3}{27}}}$.

# 2022-04-09 Anaglyph 3D methods

## Naive
Render the scene.
Duplicate the image.
Shift the copies left/right.
Color filter.
Merge.
This produces a stereo image from the same perspective.

## Less Naive
At the end of the view matrix sequence, shift opposite the displacement of the eye.
This produces two perspectives, but they have different centers of interest.


## Decent
At the end of the view matrix sequence (from standard eye space):
- Translate by $-\text{CoI}$.
- Rotate by the negative of the eye angle.
- Translate by $\text{CoI}$.

This works reasonably well, but the distance from the eye to the center of interest ($\left| \vec{EC} \right|$) is significant.

## More decent
Consider the world space eye $E$, center of interest $C$, and up point $U$.
Consider the vectors $\vec{EC}$ and $\vec{EU}$.
In a right-handed world, $\vec{EC} \times \vec{EU}$ points right.
Translate the $E$ and $U$ by some signed amount in the direction of $\vec{EC} \times \vec{EU}$ to find the left/right eye while keeping $U$ above.
A problem: it seems that this operation "rotates" the up point a different amount than the eye, potentially rotating the perspective. I haven't seen this problem manifest.

## Clever
Let's "solve" the previous problem.
Translating the eye, as in the previous method, is akin to making it more distant from the center of interest and rotating by some amount.
Call the translated distance $\Delta x$ and the new eye $E^\prime$.
At the end of the view matrix sequence (in standard eye space):
- $\left| \vec{E^\prime C} \right|^2 = \left| \vec{EC} \right|^2 + \left| \vec{\Delta x} \right|^2$.
- $\Delta x = \left| \vec{E^\prime C} \right| \sin\theta$.
- $\left| \vec{EC} \right| = \left| \vec{E^\prime C} \right| \cos\theta$.
- To keep the right up points:
- Translate by $-C$.
- Rotate by $\theta$.
- Translate by $C \frac{\left| \vec{E^\prime C} \right|}{\left| \vec{EC} \right|}$.

In practice, this seems to create the problem it wanted to solve.

## Questionable

Recenter at CoI.
Shear eye and up point so eye moves the needed amount.
Map new eye and up point back from standard eye-space.
Recompute view matrix with new eye and up point.

# 2022-04-10 Brainstorming refraction
Snell's law: $n_1 \sin\theta_1 = n_2 \sin\theta_2$.
Typical refractive index:
- Air/vacuum: $n = 1$
- Water: $n = 1.33$
- Glass: $n = 1.5$

Each object gets a refractive index.
The sign of the level curve can indicate inside (-) vs outside (+).
$\cos\theta = \frac{v \cdot w}{|v|\cdot|w|}$.

What about chromatic aberration? Reflection and transmission coefficients (energy, amplitude)

# 2022-03? Notes
We describe an object in object space with a parametrization or implicit form/level surface $f(x, y) = 0$.
We map a world-space light ray head and tail into object space (inverse object transformation matrix $Q$).
Find the intersection in object space.
Map the intersection point back to world space.
The normal to a level curve is the gradient (the vector of partial derivatives).
Chain rule: $\frac{df}{du} = \frac{\partial f}{\partial x} \frac{dx}{du} + \frac{\partial f}{\partial y} \frac{dy}{du}$.
$$\begin{aligned}
    \frac{df}{du} &= \lim_{u \to U} \frac{f(U) - f(u)}{U - u} \\
    &= \lim_{u \to U} \frac{f(x(U), y(U)) - f(x(u), y(u))}{U - u} \\
\end{aligned}$$

Interesting links:
[Phong shading](https://en.wikipedia.org/wiki/Phong_shading) and [Postcard raytracer](https://fabiensanglard.net/postcard_pathtracer/).

# 2022-03-15 Proof that the intersection algorithm works
Consider two world-space rays from $A$ to $B$: $Q_1 = A + T_1(B - A)$, $Q_2 = A + T_2(B - A)$.
Map these into different object spaces: $P_1 = A_1 + t_1(B_1 - A_1)$, $P_2 = A_2 + t_2(B_2 - A_2)$.
Map these back to world space:
$Q_1 = M_1 P_1 = M_1 (A_1 + t_1(B_1 - A_1) = A + t_1(B - A))$, $Q_2 = M_2 P_2 = ...$
Therefore $T_1 = t_1$ and $T_2 = t_2$.

# Finding normals
Find the object space intersection point $P_O$.
Find the gradient at $P_O$.
This is the object-space normal $N_O = \nabla f(P_O)$.
Let $M$ be the matrix mapping from object space to world space and $Q = M^{-1}$.
Then the world space normal (in 2D) is $N_W = \begin{pmatrix}
    Q_{00} & Q_{10} \\
    Q_{01} & Q_{11} \\
\end{pmatrix} N_O$
In 3D, use the transpose of the $3 \times 3$ upper left corner of $Q$.

# General matrix inversion algorithm
Reverse sort on $|\text{column 1}|$.
Divide to normalize the pivot.
Clear the column.
Sort on the next column and repeat.

# TNB (tangent, normal, binormal)
Consider functions $\mathrm{eye}(t)$ and $\mathrm{coi}(t) = \mathrm{eye}(t) + \mathrm{eye}^\prime(t)$.
$N$ is the normal to the center of a circle, which is the intersection of the perpendicular bisectors of two chords on the circle found as the forward and backward (approximate) tangent vectors.
Then $B = T \times N$.

Labs:
- Movie player
- Z-buffer
- 2D Raytracer
- 3D Raytracer with reflections, textures, and shadows
- Final project: Something cool, graded on
    - Technical ambition
    - Execution
    - 10-15 minute explanation, quality of explanation
    - Artistic merit

What about a church?
With refraction
Multiple lights

People have done
- something visually cool
- more shapes
- 3D glasses
- Refraction: start in 2D
- Splines (parabola connection line segment midpoints, and its 3D analog)

# Rotated objects
Consider a curve $y = f(x)$.
Rotate it around the $x$-axis.
Then $y^2 + z^2 = f(x)^2$.

Consider the torus: $y^2 + z^2 = (R \pm sqrt(r^2 - x^2))^2$.
$\sqrt{y^2 + z^2} = R \pm sqrt(r^2 - x^2)$
$\sqrt{y^2 + z^2} - R = \pm sqrt(r^2 - x^2)$
$(\sqrt{y^2 + z^2} - R)^2 = r^2 - x^2$

Current projects
- Torus (cubic/quartic solver) ($\checkmark$)
- refraction ($\checkmark$)
- anaglyph rendering ($\checkmark$)
- Raytrace space station
- triangle shape
- Moar Lights

Stolen:
- Pineapple
- View matrix movie
- spiral + checkerball
- lumpy square sphere

# 2022-04-17 Depressing a quartic
Start with $0 = y^4 + by^3 + cy^2 + dy + e$. \\
Substitute $y = x - \frac{b}{4}$. \\
$$\begin{aligned}
    0 &= y^4 + by^3 + cy^2 + dy + e \\
    &= \left( x - \frac{b}{4} \right)^4 + b \left( x - \frac{b}{4} \right)^3 + c \left( x - \frac{b}{4} \right)^2 + d \left( x - \frac{b}{4} \right) + e \\
    &= \left[ x^4 - 4 x^3 \frac{b}{4} + 6 x^2 \frac{b^2}{4^2} - 4 x \frac{b^3}{4^3} + \frac{b^4}{4^4} \right]
    + b \left[ x^3 - 3 x^2 \frac{b}{4} + 3 x \frac{b^2}{4^2} - \frac{b^3}{4^3} \right]
    + c \left[ x^2 - 2 x \frac{b}{4} + \frac{b^2}{4^2} \right]
    + d\left[ x - \frac{b}{4} \right]
    + e \\
    &= x^4
    + x^3 \left( -b + b \right)
    + x^2 \left( \frac{6b^2}{16} - \frac{3b^2}{4} + c \right)
    + x \left( -\frac{b^3}{16} + \frac{3b^3}{16} - \frac{bc}{2} + d \right)
    + \left( \frac{b^4}{256} - \frac{b^4}{64} + \frac{b^2 c}{16} - \frac{bd}{4} + e \right) \\
    &= x^4
    + x^2 \left( -\frac{3b^2}{8} + c \right)
    + x \left( \frac{b^3}{8} - \frac{bc}{2} + d \right)
    + \left( -\frac{3b^4}{256} + \frac{b^2 c}{16} - \frac{bd}{4} + e \right) \\
\end{aligned}$$

# 2022-04-17 Ferrarri's Method (from _Polynomia_, p. 75)
Start with a depressed quartic $x^4 = ax^2 + bx + c$. \\
Add $2 x^2 y + y^2$ to both sides. \\
$\left( x^2 + y \right)^2 = \left( a + 2y \right) x^2 + bx + \left( c + y^2 \right)$. \\
To make the RHS a perfect square, we want the discriminant to be zero:
$ \Delta_2 = b^2 - 4 (a + 2y) (c + y^2) = 0$. \\
Rewrite as $8y^3 + 4ay^2 + 8cy - b^2 + 4ac = 0$. \\
Solve this cubic with Cardano's method to get a real root $y$.
Then both sides of the previous equation are perfect squares, $w^2$ and $z^2$ respectively, so $w = \pm z$. \\
That is, $x^2 + y = \pm \sqrt{\left( a + 2y \right) x^2 + bx + \left( c + y^2 \right)}$. \\
Write the RHS term $\left( a + 2y \right) x^2 + bx + \left( c + y^2 \right)$ as $A(x + B)^2$ with $A = (a + 2y)$ and $B = \frac{b}{2A}$. \\
Then $x^2 + y = \pm \sqrt{A} (x + B)$. \\
$x^2 + x \left( \pm \sqrt{A} \right) + \left( y \pm B \sqrt{A} \right) = 0$. \\
Solve these two quadratics $(\pm)$ to find all four solutions.

# 2022-04-19 Jeff's alternate solution to depressed quartic
Take $ y^4 - my^2  - ny - p = 0$. \\
Factor into $(y^2 + fy + g) (y^2 - fy + i)$. \\
This is $y^4 + iy^2 - f^2 y^2 + fiy + gy^2 - fgy + gi$. \\
This is $y^4 + (i - f^2 + g) y^2 + f(i - g) y + gi$. \\
Then $i + g = f^2 - m$ and $i - g = -\frac{n}{f}$. \\
$(+)$: $2i = f^2 - m - \frac{n}{f}$. \\
$(-)$: $2g = f^2 - m + \frac{n}{f}$. \\
Then $4ig = (f^2 - m)^2 - (\frac{n}{f})^2$. \\
$-4p = (f^2 - m)^2 - (\frac{n}{f})^2$. \\
Let $F = f^2$. \\
$-4p = (F-m)^2 - \frac{n^2}{F}$. \\
$-4pF = F(F-m)^2 - n^2$. \\
$0 = F(F-m)^2 + 4pF - n^2$. \\

$0 = F^3 - 2mF^2 + (m^2 + 4p) F - n^2$. \\
Solve cubic. \\
$f = \pm \sqrt{F}$. Which? \\
$i = \frac{1}{2} (f^2 - m - \frac{n}{f})$. \\
$g = \frac{1}{2} (f^2 - m + \frac{n}{f})$. \\
Solve quadratics. \\

Mobius strip is a cubic?

# From the repo

## 2022-04-12: Raytracing triangles
A point $P$ on the line from $S$ to $E$ is described by $P = S + t \cdot (E - S)$. \\
A point on the triangle $ABC$ is described by $P = A + u \cdot (B - A) + v \cdot (C - A)$. \\
Solve this system:
$$\begin{aligned}
S + t \cdot (E - S) &= A + u \cdot (B - A) + v \cdot (C - A) \\
S - A &= u \cdot (B - A) + v \cdot (C - A) - t \cdot (E - S) \\
\end{aligned}$$
This is a linear system with 3 equations (for $x$, $y$, $z$) and 3 unknowns ($t$, $u$, $v$). \\
We will reject any solution with $u < 0$, $v < 0$, $u > 1$, $v > 1$, $u + v > 1$, or $t < 0$. \\
The triangle's normal is $\vec{N} = \vec{AC} \times \vec{AB}$. \\

### Cramer's Rule
We can solve a $3 \times 3$ matrix easily.
Consider solution $\begin{pmatrix}u \\ v \\ w\end{pmatrix} = \begin{pmatrix}1 \\ 2 \\ 4\end{pmatrix}$. \\
Construct a system of equations:
$$\begin{Bmatrix}
    3u + 7v + 2t = 25 \\
    1u + 3v + 1t = 11 \\
    2u + 1v + 5t = 24 \\
\end{Bmatrix}$$
The system has a matrix $M = \begin{pmatrix} 3& 7& 2 \\ 1& 3& 1 \\ 2& 1& 5 \end{pmatrix}$ with determinant $d = \det(M) = 11$. \\
The system is inconsistent when $\det(M) = 0$. \\
Otherwise we have
$$\begin{aligned}
    u \cdot d = \det(\text{the matrix M with the $u$ column replaced with the constant column}) = \begin{vmatrix}25& 7& 2 \\ 11& 3& 1 \\ 24& 1& 5\end{vmatrix} \\
    v \cdot d = \det(\text{the matrix M with the $v$ column replaced with the constant column}) = \begin{vmatrix}3& 25& 2 \\ 1& 11& 1 \\ 2& 24& 5\end{vmatrix} \\
    t \cdot d = \det(\text{the matrix M with the $t$ column replaced with the constant column}) = \begin{vmatrix}3& 7& 25 \\ 1& 3& 11 \\ 2& 1& 24\end{vmatrix} \\
\end{aligned}$$

# 2022-04-26 Placing objects using the view matrix
In world space, start at $\begin{pmatrix}xs \\ ys \\ zs\end{pmatrix}$ and end at $\begin{pmatrix}xe \\ ye \\ ze\end{pmatrix}$. \\
Connect these points with a cylinder.
## Z-buffer/raytracer strategy
A cylinder is $\begin{pmatrix}\cos(\pi u) \\ v \\ \sin( \pi u)\end{pmatrix}$
- TY 1 (to be above the $xz$-plane)
- SY len(s, e) / 2 (to get the right length)
- SX radius (to get to the right width)
- SZ radius (to get to the right width)
- RX 90° (to be on the +z-axis)

M3d_view(V, V_i, $S$, $E$, up)
Use V_i to move the cylinder from standard position to the arbitrary position we want.

## Alternate Z-buffer strategy
Let $V = \vec{SE} = S - E$.
We can find $W = \vec{SP} = P - S}$ for an arbitrary $P$ not on the line.
Then $A = V \times W$ is perpendicular to $V$ and $W$.
Then $B = A \times V$ is perpendicular to $A$ and $V$.
Let $v \in [0, 1]$ and $u \in [0,2]$.
$v \cdot V$ gets anywhere from $S$ to $E$, plus
 $\cos(\pi u) \cdot A + \sin(\pi u) \cdot B$ gets anywhere around the circle around that line.
