GPU-LB-MD
=========
This is a project of Lattice Boltzmann(LB) coupled with molecular dynamics(MD) for complex fluid simulation

Platform and Device
=========
1. LINUX
2. NVDIA, CPU
3. CUDA

Key features
=========
1. parallelized LB code with bouncing-back boundary condition.
2. parallelized MD code
  2-1. particle model --- (model A)
  2-2. polymer chain model --- (model B)
  2-3. deformable particle model --- (model C)  
3. Couple LB and MD code

Specifications
=========
0. Code for single CPU and single GPU 
1. Suitable for three major simulation systems
  (a) LB for fluid simulation only
  (b) MD for particle simulation with any combination of model A,B,C
  (c) LB coupled with MD simulation

2. LB
  → one kind of fluid only
  → D3Q19 lattice model
  → boundary condition
    ．periodic boundary condition
    ．bouncing-back boundary condition
  → flunctuation force
  → output file format: .vtk
  → reference:
    (a) Palabos website : http://www.palabos.org/documentation/tutorial/
    (b) http://www.icp.uni-stuttgart.de/~icp/mediawiki/images/2/26/Roehm_diplomathesis.pdf
    (c) http://www.sciencedirect.com/science/article/pii/S0898122109006361

3. MD
  → particle motion satisfying Langevin equation
  → model
    ．for all models: 
        wall force, fluid force, external force
    ．particle model (model A):
        force: exclusion force, thermal flunctuation
    ．polymer chain model (model B):
        force: exclusion force, spring force, bending force, thermal flunctuation
    ．DP model (model C):
        force: exclusion force, spring force, face-face bending force, area-conserve force, volume-conserve force, thermal flunctuation
  → output file format:
      model A, B : .xyz
      model A, B, C : .vtk
  → already-available libraries
    (a) OpenMM : https://simtk.org/home/openmm
      (CUDA-accelrated, has preexisting model for model A, model B)
    (b) Espresso : http://www.quantum-espresso.org/?p=466
      (multi-CPU-accelrated)

Future goal
==========
1. complex shape of boundary in LB simulation (circular, user-define)
2. elastic wall

Schedule
=========
1. ~ 2013/4/19
  (1) Review
  (2) Get acquianted with CUDA programming environment on linux server, learn github
  (3) Discussion on algorithm and structure
2. 
  (a) Write a skeleton code for our purpose
  (b) Write kernel code (LB)
  ??(c-1) Write kernel code (MD) -- model A, model B
  ??(c-2) Get started with openMM library
3.
  (1) bench mark testing for LB code (poiseuile flow, shear flow)

[Final Project Presentation]

4. 
  (a) couple MD and LB
  (b) Write model C
