# Parallel-Computing-Assignment---Strassen-Algorithm

## Problem Statement

Multiply 2 Matrix using [Strassen algorithm](https://en.wikipedia.org/wiki/Strassen_algorithm) with [OpenMP](https://en.wikipedia.org/wiki/OpenMP) in parallelization

Special: dimension of matrix is not necessarily form of power 2

## Member in Team
|NAME|ID Student|
|---|---|
|[Nguyen Huu Thien Phu](https://github.com/phupfoem)|1810433|
|[Nguyen Minh Anh](https://github.com/zoldabest-72)|1811431|
|[Doan Ngoc Thinh](https://github.com/nngthinh)|1810542|
|[Truong Cong Thanh](https://github.com/truongcongthanh2000)|1810766|

## Usage
    open terminal and type
    $ ./test.sh
    
    # Matrix A have size (m x p)
    # Matrix B have size (p x n)
    # Matrix C = A * B have size (m x n)
    # Output is recorded in output_dac.txt and output_dac_OpenMP.txt

## Run measurement test file.
    open teminal and type
    $ ./run_dac.sh
    $ ./run_dac_OpenMP.sh 
    # Attention: file run_dac_OpenMP.sh is takes very long time to run
    please remove the case dimension = 10000 if neccessary.

    # Result is recorded in result_dac.txt and result_dac_OpenMP.txt

## Measurement results chart

[Result](https://github.com/truongcongthanh2000/Parallel-Computing-Assignment---Strassen-Algorithm/blob/main/PC_Strassen_Chart.xlsx)
    
