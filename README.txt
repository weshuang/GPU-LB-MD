GPU-LB-MD
=========
This is a project of Lattice Boltzmann(LB) coupled with molecular dynamics(MD) for complex fluid simulation

Platform and Device
=========
LINUX
NVDIA
CUDA

Milestones
=========
1. parallelized LB code with bouncing-back boundary condition.
2. parallelized MD code
  2-1. particle model
  2-2. polymer chain model.
  2-3. deformable particle (DP) model.  
3. Couple LB and MD code

Specifications
=========
1. LB
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

2. MD
  → particle motion satisfying Langevin equation
  → model
    ．for all models: 
        wall force, fluid force, external force
    ．particle model:
        force: exclusion force, thermal flunctuation
    ．polymer chain model:
        force: exclusion force, spring force, bending force, thermal flunctuation
    ．DP model:
        force: exclusion force, spring force, face-face bending force, area-conserve force, volume-conserve force, thermal flunctuation
  → output file format:
      model 2-1, 2-1 : .xyz
      model 2-1, 2-2, 2-3 : .vtk
  → already-available libraries
    (a) OpenMM : https://simtk.org/home/openmm
      (CUDA-accelrated, has existing models for 2-1, 2-2)
    (b) Espresso : http://www.quantum-espresso.org/?p=466
      (multi-CPU-accelrated)
    
Future goal for improvement
==========
1. complex shape of boundary in LB simulation (circular, user-define)
2. elastic wall

Schedule
==========
