
# Burgers computer assisted proof code

The code is used for computer-assisted proof of the existence of a periodic orbit and its attraction for the non-autonomous Burgers equation with fractional Laplasian:

$$u_t = -(-\Delta)^{\alpha} u + \delta(u^2)_{x} + (A_1\sin(2 \pi t)+B_1)\sin(x) +(A_2\sin(2 \pi t)+B_2)\sin(2x),$$

where the solution $u(x,t):\mathbb{R}\times[t_0,T]\to \mathbb{R}$ satisfies odd, $2\pi$-periodic boundary coditions that is $u(x+2\pi,t) = u(x,t)$ and $u(t,x)=u(t,-x).$ We assume that  $A_1,B_1,A_2,B_2\in\mathbb{R}$ and $\delta\in \mathbb{R}$ and $\alpha\in(\frac{1}{2},1]$.

We work in the Fourier base, which means we write every point from the phase space in the form

$$ u^0 = \sum_{i=1}^\infty u_i^0\sin(ix).$$ 
The operator  (-\Delta)^{\alpha} is defined in following way
$$ (-\Delta)^{\alpha} u^0 = \sum_{i=1}^\infty i^{2\alpha}u_i^0\sin(ix).$$ 


The code contains program:

- `Burgers/CAProof`,


## Burgers\CAProof.cpp

This program is designed to prove the existence of a periodic orbit and demonstrate its local attraction. It does so by checking whether a defined set $X^0$ satisfies the following condition:

$$ \varphi(1,0,X^0) \subset X^0 $$

It contstruct this set, by firstly finding numerically fixed point of map $\varphi(1,0,\cdot)$ and then $X^0$ is some neighborhood of this point.

If this condition is satisfied, it confirms the existence of a periodic orbit. The program utilizes a rigorous C0 algorithm for integrating partial differential equations (PDEs) to compute the image. Additionally, it attempts to prove that the orbit is locally attracting by verifying:

$$
|\left|\frac{\partial \varphi}{\partial x}(1,0,X_0)\right||_{H^2(-\pi,\pi)} < 1.
$$


The computation of derivatives employs a rigorous C1 integration algorithm.
We do this prosecude for $\alpha = \frac{1}{2} +\frac{k}{32}$, $k = 32,\ldots, 1$ starting with $k=32.$ We go down with the $k$ to see how close we can get to the critical value of $\alpha = \frac{1}{2}.$ If estimates blow-up we increase the number of explicitly represented modes. If this number get bigger then $200$ we end the prosedure,

Parameters ${\alpha,\omega,\delta,A_1,B_1,A_2,B_2}$ are taken from file `ChafeeInfante/textFiles/params.txt` in the form 

```
{1,6.28318530718,0.5,0.5,1,0,0}
-------------
{alfa,omega,delta,A1,B1,A2,B2}

```
$\omega$ is addidtional parametr hardcoded to be equal $2\pi$, but it has to be on the list of parameters. Also $\alpha$ has to be provided but does not change the output of program.

## Code Information

- The programs are using the [CAPD library](http://capd.ii.uj.edu.pl/index.php) - a tool for nonrigorous and validated numerics for dynamical systems.
  - Used version of the library: 6.0.0,
  - The Makefile assumes that the CAPD library is located in the following position relative to the main directory:
   ```
   # directory where capd scripts are (e.g. capd-config)
   CAPDBINDIR = ../CAPD/build/bin/
   
   ```

- Folder DissipativePDE contains tools for rigorous integration of PDEs:
  - Folder DissipativePDE\Algebra contains the structure infinite series used in the algorithm and implementation of operations on them,
  - Folder DissipativePDE\Set contains structures for sets which are used in rigorous integration,
  - Folder DissipativePDE\VectorField contains structures for VectorFields used in rigorous integration,
  - Folder DissipativePDE\VectorFieldMaker contains additional methods that allow producing vector field fields for Gallerkin projection in string form, which is used in CAPD IMap class,
  - Folder DissipativePDE\SolverPDE contains methods and structs for rigorous C0 and C1 integrations.

- Folder Utils mainly contains Input/Output settings.

- Folder Burgers contains code specifically dedicated to Burgers:
   - Folder Burgers\ChafeeInfanteVecField containing C0 and C1 vector fields used in rigrous integration
   - Folder Burgers\GallerkinProjections containing method to produce strings, used in IMap class, for Gallerkin projection of the Burgers equation.





  



