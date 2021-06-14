# Parallel-Computing-Assignment---Strassen-Algorithm

## Member in Team
|NAME|ID Student|
|---|---|
|[Nguyen Huu Thien Phu](https://github.com/phupfoem)|1810433|
|[Nguyen Minh Anh](https://github.com/zoldabest-72)|1811431|
|[Doan Ngoc Thinh]()|1810542|
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