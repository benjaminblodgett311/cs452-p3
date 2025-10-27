# Project 3 - Multithreaded Sorting Application

- Name: Benjamin Blodgett
- Email: benjaminblodgett311@u.boisestate.edu
- Class: 452-002

## Known Bugs or Issues

``make test`` will cry that there are two entry points. The makefile doesn't expect us to write a main.c *and* a test_sort.c but the point of this project is to write a standalone program to generate a graph, so I guess that the testing suite is just for giggles? ChatGPT wrote all of it anyways.

## Experience

This project was neat. AI helped me work out a lot of the implementation, but I worked on most of the ``mergesort_mt`` function myself based on in-class discussions. I like this solution because it splits the workload evenly and allows us to join in a simple way as well as assign work in a very straightforward way.

## Analysis

I believe my laptop has more cores available to the programmer than Onyx, so it didn't hit the marginal utility part of the graph until around 16 threads. This is because running ``nproc`` on my system yields 16 cores. The graph eventually hits diminishing returns. After 16 threads it hits *negative* returns also. I think what I was able to generate was actually a little smoother than the example. At a certain point the program doesn't benefit from more threads. The optimum number of threads is kinda arbitrary and depends on the opportunity cost function for using additional threads, however the program ran fastest with 11 threads. The slowest number of threads was 1 thread on my machine. Trying to use the full 16 threads is difficult because I have a lot of stuff running that likely uses some of my other threads.

This is the part of my analysis where I talk more about my project because I need to fulfill a word count requirement. I think I could have seen something a little more similar to the example graph on Onyx if the graph range went up to 64 threads, because that's about double the current and my laptop graph bottomed out at 10 whereas the example bottomed out at 5. I was surprised how consistent my graph was but also how slowly it approached diminishing returns compared to the Onyx graph. It really shows that there are many cases where spinning up a dozen or so threads might be beneficial for some machines rather than just spinning up 2 or 4 threads.

Also something I realize is that ``nproc`` reports logical CPUs not physical cores. So I don't know what effect that has. I suppose there is a way to query in C for core count and spin up a corresponding amount of threads, but that was also somewhat discouraged against in class. I would like to try this threading approach with my Minecraft-clone project. It was very expensive to generate new world geometry when loading in new chunks and whenever the player did so it would cause one huge stutter. Multithreading is essentially mandatory for resolving this issue from what I understand from other people's videos. In my case I might try to spin up a thread for every chunk. There were other issues with my implementation that might make the stutters less powerful but that's beside the point.

I might be losing out on performance and thereby influencing the graph because of my naive joining solution. I really don't want to figure out how to merge blocks as they are joined though. I assume it might not make a big difference because the blocks all provide a relatively equal workload to each thread. However, we can't assume that the threads will take a similar amount of time to finish even if their workload is hypothetically similar. I wonder if there is a way to partition work in a fancy way so that when threads finish early we can estimate how early they are and give them additional work accordingly.

![Performance Graph](scripts/student_plot.png)
