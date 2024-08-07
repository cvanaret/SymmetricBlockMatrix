# SymmetricBlockMatrix

This is a C++ attempt at forming symmetric bloc matrices and condensing them at compile time using [Schur complements](https://en.wikipedia.org/wiki/Schur_complement#Application_to_solving_linear_equations). By condensation, we mean eliminating certain row blocks and substituting in the other block equations.  

The goal is to manipulate KKT/augmented/saddle-point systems symbolically to obtain equivalent systems (typically: unsymmetric, symmetrized, and normal equations) that can be tackled by various numerical solvers.

An $n \times n$ symmetric block matrix is represented as a sequence of blocks in the upper triangular part of the matrix:  
```
A_{11} A_{12} ... A_{1n}
       A_{22} ... A_{2n}
              ...
                  A_{nn}
```

*Contributions are welcome!*
