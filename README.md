# Simulation of A Bicycle Rental System

## Overview

Analyzing the bicycle sharing system of a Singapore Theme Park, which is described in the `/doc/bicycleRentalSystem.pdf`


## How to Run

**Generate the input data of our model:**

```sh
cd data
sh generateData.sh
```

Above command will generate 12 data files in the subfolders under data/ directory.
You may check the generated data distribution by:

```sh
python plot.py create/a
...
```

**Complie the C++ file**

```sh
# At the root of the repo.
g++ -O2 main.cpp
```

You will see some 'warnings', which doesn't mater at all and you just ignore them.

**Run a.out:**

```sh
./a.out
```
## How to Read Output

**Printed output result:**

You will see a lot of out put like this:

```
At second 7200000: 0 A(12) 1008 -> (3) -> 0 B(9) 1786 -> (3) -> 230 C(9) 0 -> (1) -> 0 D(11) 2092 -> (3) -> A

```

It means:

```
At second [the second now, from start]: [number of people in queue to go into A] A(the number of people in A) [number of people in queue out A] -> ([number of people in path])...
```

**Result in file:**
In the file `Tourists.txt`, you will see:

```
Tourist Id 0 arrive at 379: 0 D(2332) 0 -> (387) -> 0 A(1748) 0 -> (797) -> 0 B(1904) 0 -> (669) -> 0 C(2258) 0 -> (0) -> Leave at 10474, totally 10095 seconds. 4 sites 
```

It means, tourist with id 0 arrived at the theme park at the 379th second. He firstly stayed at D, which is where he first arrived, for 2332 seconds, and waited 0 seconds to get a bicycle and spent 387 seconds to ride to the next site A, where he stayed for 1748 seconds. The rest is just like this.


## What to Do Next

**Parameters in main.cpp:**

You will see at line 11-13 in file  `main.cpp`:

```cpp
#define NUM_DOCKER 80
#define NUM_BICYCLE 80
#define MAX_SECONDS 3600000 * 2
``` 

Adjust these parameters will change the initial number of dockers and bicyles, and the maxmium simulation days. 
For example, the 3600000 * 2 seconds is 83 days. Note that if you change this number even bigger, you may need to change the
data parameter in `data/generateData.sh` to have more input data, which is very easy.

**Search for n, m:**

We will search for n, m (the best number of docker and bicycle), by writing a .sh file that loop the ./a.out for many times with different `NUM_DOCKER` and 
`NUM_BICYCLE`. To do this you might need to change the C++ code and use the `argv`.   

Notice that you should update to a new random data set in /data before you run each `./a.out`. `a.out` takes the `data/create/*`, `data/pathtime/*` and `data/playtime/*`
as its model input directly. To achieve this, you may include the `data/generateData.sh` into your new `.sh` file.

**Random Seed:**

If you you want to  generating different data with the last runing of `generateData.sh`, you should specify different random seed in `generateData.sh`.

  
