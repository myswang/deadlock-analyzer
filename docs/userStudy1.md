## User Study 1

See `user-study-procedure.md` for context.

## User Background Information from Starting Questions

-   User understands in an abstract sense what mutexes, threads, and race conditions are
-   User knows what deadlocks are and was able to give an example (multiple threads waiting for each other in a "circular chain")
-   User was familiar with C programming, although had not used the `pthread.c` library, which is what the examples were based upon
-   User had not previously used any software to understand deadlock in code better; however, used has usedprogram analysis tools in the past, such as valgrind

## Example 1

### What They Found Easy

-   Code was clear and the example was simple (only 2 threads)

### What They Found Difficult

-   Since the user had not previously used the library for threads, they had to read through the documentation for the library
-   User believed to have found a race condition that did not exist, possibly due to their low knowledge of the library
-   Could not figure out some aspects of the code until the second example, when the user was able to the compare programs
-   Gave the wrong answer (though that the program would produce a deadlock)

## Example 2

### What They Found Easy

-   User was easily able to spot the difference between the first and second examples
-   User has developed a good intuitive sense of threads and how the library works, which led to a correct prediction of program behavior (a deadlock)

### What They Found Difficult

-   Still a little bit of syntactic difficulties (e.g., thought that the second mutex was not initialized at first)

## Example 3

### What They Found Easy

-   Example was still somewhat similar to the first two (same context and structs)
-   Was able to eventually give a correct answer after some time

### What They Found Difficult

-   Codebase became more complex (3 functions, 3 mutexes)
-   User needed to go up and down the codebase to mentally create a graph-like structure in their head
-   User sometimes made a mental mistake when creating their mental model, but eventually corrected themself (this slowed their thinking down)
-   Somewhat difficult to keep track of print statements and execution order

## What Did You Learn From Your Users: User Thoughts on Visualization Tools from Visualization Questions

-   The way that the user created their own mental model of deadlock analysis was very similar to how a resource allocation graph looks, making the graph intuitive to use
-   User was able to match a given resource allocation graph with a code snippet based on only skimming an article about resource allocation graphs due to its intuitiveness
-   User believed that resource allocation graphs would be more useful in complex codebases where it would be difficult to mentally juggle resources/mutexes as there could be a tradeoff in small programs where the time it takes to run the program analysis could be higher than the time it would have taken to mentally figure the deadlock out
-   If the user had a choice, they would use resource allocation graphs to help debug deadlocks

## What Would You Have Done Differently

-   If resources and time permitted, more users could have been studied, especially within our target audience, "Developers working on multithreaded, concurrent systems that involve shared resources and synchronization using mutexes."
-   If resources and time permitted, it would have been good to have two groups: one not given a resource allocation graph for the examples, and one given a resource allocation graph; then, internal validity could be established better
-   User stated that the third example was still a bit too simple to fully illustrate the need for a resource allocation graph. Therefore, a more complex example may have been better; however, this would take a lot of time
