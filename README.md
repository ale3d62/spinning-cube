# spinning-cube
Spinning cube animation in terminal with ASCII

![spinning-cube](https://github.com/ale3d62/spinning-cube/blob/main/preview.gif)

## How it works
For each face of the cube (which has a character assigned), we take every point and calculate its position in 3d space after applying a rotation in every axis. Once rotated, we check where that point would land on the screen by simulating perspective. This is explained further in the [donut.c explanation page](https://www.a1k0n.net/2011/07/20/donut-math.html) by [ak0n](https://github.com/a1k0n/). The rotation is calculated via simple matrix multiplications

$$
X Axis:
\begin{pmatrix}
x' \\
y' \\
z'
\end{pmatrix}
=
\begin{pmatrix}
x \\
y \\
z
\end{pmatrix}
\begin{pmatrix}
 1 & 0 & 0 \\
 0 & cos(\theta_x) & -sin(\theta_x) \\
 0 & sin(\theta_x) & cos(\theta_x)
\end{pmatrix}
$$

$$
Y Axis:
\begin{pmatrix}
x' \\
y' \\
z'
\end{pmatrix}
=
\begin{pmatrix}
x \\
y \\
z
\end{pmatrix}
\begin{pmatrix}
 cos(\theta_y) & 0 & sin(\theta_y) \\
 0 & 1 & 0 \\
 -sin(\theta_y) & 0 & cos(\theta_y)
\end{pmatrix}
$$

$$
Z Axis:
\begin{pmatrix}
x' \\
y' \\
z'
\end{pmatrix}
=
\begin{pmatrix}
x \\
y \\
z
\end{pmatrix}
\begin{pmatrix}
 cos(\theta_z) & -sin(\theta_z) & 0 \\
 sin(\theta_z) & cos(\theta_z) & 0 \\
 0 & 0 & 1
\end{pmatrix}
$$

Then, every frame, each axis's θ is increased by a random amount.

## How to build
Built in *Ubuntu 22.04.5LTS* through *WSL* using *cmake 3.22.1*

    cd build
    cmake ..
    make