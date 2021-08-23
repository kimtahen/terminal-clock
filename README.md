# terminal-clock
Digital clock with stopwatch and timer function
* ncurses library used for displaying character on terminal
* thread library used for switching functions

# Documentation
## How to run
1. build
```make
make
```
2. run
```
./main.out -v [refresh interval of 1~1000]
```
3. clean
```
make clean
```

## Addtional information
This program uses linux threads, so default refresh interval setting of 1 milliseconds for stopwatch and timer can use quite long cpu time. By using -v flag, the refresh interval can be adjusted. 1 is the highest and 1000 is the lowest (1 milliseconds, 1000 milliseconds = 1 seconds)

# Snapshot
![clock](https://user-images.githubusercontent.com/52111798/130354601-c6fe95f5-3a36-4190-a947-c2fc0eff9b87.jpg)
![stopwatch](https://user-images.githubusercontent.com/52111798/130354610-954de18b-fa09-4d4b-b8c9-6546aa1448ed.jpg)
![timer](https://user-images.githubusercontent.com/52111798/130354618-4708d0e8-299a-44ce-a687-ddc4a22152aa.jpg)
![timer](https://user-images.githubusercontent.com/52111798/130354620-b01c2194-a229-4075-a83e-62897235fe3c.jpg)

