# MatMul


MatMul is a project inspired by the research detailed in [Nature's](https://www.nature.com/articles/s41586-022-05172-4) publication. Unlike the original machine learning-based approach, MatMul leverages Integer Linear Programming (ILP) using the Gurobi solver to tackle the problem of matrix multiplication.


---

## Overview
Standard matrix multiplication for \( n × n × n \) matrices can be performed in \( O(n^3) \) time, but there are known algorithms (like Strassen’s algorithm) that can perform the multiplication with fewer than \( n^3 \) multiplications. 

In this project, we try to formulate the problem of finding such a multiplication scheme into ILP, which is solved using Gurobi via a **modified ScaLP** library. However, due to the complexity of the search space, it can be extremely difficult for large matrix dimensions. Even 3×3×3 can be prohibitively slow.

---

## Installation

### Prerequisites
- Ensure that CMake version 3.0 or higher is installed.
- Install Gurobi and ensure that you have a valid license.
- Set up a compatible C++ compiler (e.g., GCC or Clang).
- Clone the necessary repositories as described below.

### Install Modified ScaLP
1. Clone the modified ScaLP repository:
   ```bash
   git clone https://gitlab.uni-kassel.de/uk050765/scalp_modified.git
   ```

2. Navigate to the cloned directory:
   ```bash
   cd scalp_modified
   ```

3. Use CMake with the following parameters:
   ```bash
   cmake -DOLD_CXX_ABI=ON -DGUROBI_LIBRARIES="path/to/Gurobi/Library" -DGUROBI_INCLUDE_DIRS="path/to/Gurobi/Header" .
   make
   ```

This will produce the `libScaLP` library.


### Install MatMul
1. Clone the MatMul repository:
   ```bash
   git clone git@github.com:alnugro/MatMul.git
   ```

2. Navigate to the cloned directory:
   ```bash
   cd MatMul
   ```

3. Use CMake with the following parameters:
   ```bash
   cmake -DOLD_CXX_ABI=ON -DSCALP_H="path/to/src/folder/in/ScaLP_modified" -DSCALP_LIB ="Path/to/libScaLP.so" -DCMAKE_PREFIX_PATH="path/to/ScaLP_modified" .
   make
   ```

This will produce the `MatrixMult` executable.

---

## Usage

Run the `MatrixMult` executable with the `--help` flag to see available options and parameters. Depending on your setup, you can specify input matrix dimensions, solver parameters, etc.

   ```bash
   .\MatrixMult --help
   ```

---

## References

1. **Project Report for Detailed Formulation and Implementation**:  
   https://drive.google.com/file/d/16n0uhNSgFpNslAt1oPodC9JzZU0JzlAv/view?usp=sharing

2. **Modified ScaLP Repository**:  
   https://gitlab.uni-kassel.de/uk050765/scalp_modified

3. **Original ScaLP Repository** (Kumm et al.):  
   https://digidev.digi.e-technik.uni-kassel.de/scalp/

4. **MatMul Repository**:  
   https://gitlab.uni-kassel.de/uk050765/matrixmult


---

