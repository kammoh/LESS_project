# The LESS project repository

This repository contains the software artifacts related to the LESS 
signature algorithm.

The *reference_code* folder contains a portable C11 implementation
of the LESS signature algorithm for the three parameter sets specified
in the extended paper reference. The reference implementation provides
a functionality test binary, *less_test*, and a microbenchmark for
x86_64 platforms, *less_benchmark*.

The *attacks* folder contains Sage scripts to simulate codewords
enumerating methods to attack the Code Equivalence Problem; scripts
to estimate the attacks cost are provided as well.


## Paper references (reverse chronological order)

* _Alessandro Barenghi, Jean-François Biasse, Tran Ngo, Edoardo Persichetti, Paolo Santini_:
**Advanced signature functionalities from the code equivalence problem.** Int. 
J. Comput. Math. Comput. Syst. Theory 7(2) (2022) [link](https://doi.org/10.1080/23799927.2022.2048206)

* _Alessandro Barenghi, Jean-François Biasse, Edoardo Persichetti, Paolo Santini_:  
**LESS-FM: Fine-Tuning Signatures from the Code Equivalence Problem**. PQCrypto 2021 [link](https://dblp.org/db/conf/pqcrypto/pqcrypto2021.html#BarenghiBPS21)
 (Extended version available at [https://eprint.iacr.org/2021/396.pdf])

* _Jean-François Biasse, Giacomo Micheli, Edoardo Persichetti, Paolo Santini_:
**LESS is More: Code-Based Signatures Without Syndromes.** Africacrypt 2020 [link](https://doi.org/10.1007/978-3-030-51938-4_3)

## Performance figures from the reference implementation on an Intel Core i7-12700k 

| Parameter set | Keygen (kilocycles) | Sign (kilocycles) | Verify (kilocycles) |
|:--------------|:--------------------:|:-------------------:|:---------------------:|
| Min. Pub. Key  size |    3205.45     |    311449.36      |       310232.29    |
| Min. Signature size |    28823.12    |    122002.57      |       122002.57    |
| Min. PK+Sig    size |    5617.63     |    510400.97      |       512253.13    |


### Building Reference Implementation

- Clone and build [XKCP](https://github.com/XKCP/XKCP)
```
git clone https://github.com/XKCP/XKCP.git
cd XKCP
make <target-arch>/libXKCP.a
```
`taget-arch` should be either `reference` or target machine architecture.
For example for optimized ARMv8 build run:
```
make ARMv8A/libXKCP.a
```

- build
```
cmake -B build -DKECCAK_LIB=<PATH-TO-XKCP> -DCMAKE_BUILD_TYPE=Release
cmake --build build
```