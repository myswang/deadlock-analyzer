# Group 4 Check-Ins

## Table of contents

- [Check-In 1 Report](#check-in-1-report)
- [Check-In 2 Report](#check-in-2-report)
- [Check-In 3 Report](#check-in-3-report)
- [User Study 1 Results](#user-study-1)
- [Check-In 4 Report](#check-in-4-report)
- [Check-In 5 Report](#check-in-5-report)
- [User Study 2 Results](#user-study-2)

## Check-In 1 Report

Brief description of your discussions within your team so far, and any current candidate ideas for your project. You should talk with your TA/Caroline as soon as possible about these ideas; due to the project start mid-week it's OK if you have not yet done this, but make sure to note the progress you have made so far.

- We are leaning towards static analysis, but currently we are open to both static or dynamic analysis. We have not decided on the language yet.
- Currently we have the following ideas
  - deadlock static analysis tool, visualized as a graph
  - visualization for classes that extend/inherit from each other/coupled, visualized as a graph
  - profiler, for seeing what parts of the code take the most time/least efficient, visualized

Any planned follow-up tasks for the next week.

- We will want to decide on a type of analysis as well as the target language and the analysis language.
- Additionally, we want to have a general understanding of how to divide up the work.

## Check-in 2 Report

Brief description of your planned program analysis (and visualisation, if applicable) ideas.

- Our tool performs static analysis of lock dependencies in multithreaded programs by visualizing them as a directed conflict graph. Each node represents a lock, and edges indicate processes/threads acquiring them in a specific order. Cycles in the graph suggest potential deadlocks. The tool helps programmers verify deadlock risks and promote safe concurrency practices.

Notes of any important changes/feedback from TA discussion.

- We were unsure about the scope of the project, such as whether we have to support everything (like joining threads, etc), but our TA told us that we can make some assumptions about the input program.
- TA told us the course does not prefer to have user annotations on the input programs, so our input programs will not have annotations.

Any planned follow-up tasks or features still to design.

- We have a general idea of what the visualization(s) would be, but likely our idea will change as we mould it in the upcoming days/weeks.
- We need to figure out resulting JSON outputed by static analyzer used by the visualization frontend so both groups can work in parallel.

Planned division of main responsibilities between team members.

- Current idea is Ryan, Mike, Yingkai will focus on the cpp side (ast visitor, and getting the static analysis working) and Alan, Sunny will focus on visualization.

Summary of progress so far.

- We have decided on our target language, visualization language, and planned program analysis.
- We have not started implementation yet.

Roadmap for what should be done when, including specific goals for completion by future check-in (propose at least three such goals per future check-in, along with who will work on them; you can revise these later as needed).

- Check-in 3:
  - User study results uploaded (likely done by multiple people, undecided yet)
  - Mockup of planned visualization (Alan, Sunny)
  - Good idea on JSON format outputted by static analyzer and used by frontend (whole team)
  - AST visitor pattern working on the Clang AST (Ryan, Yingkai, Mike)
- Check-in 4
  - Visualization(s) can display data, though may be ugly/in-progress (Alan, Sunny)
  - Basic tests passing with respect to the static analysis, able to detect trivial deadlock cases (Mike, Yingkai, Ryan)
  - Plan of questions written for upcoming user study (unsure who will do this yet)
  - Have plan of timeline for video and remaining tasks and who will work on them (whole team)

## Check-in 3 Report

Mockup of how your project is planned to operate (as used for your first user study). Include any sketches/examples/scenarios.

- Our project will be used to analyze programs that interact with the `pthread` library and output a visualization similar to a [Resource Allocation Graph](https://www.geeksforgeeks.org/resource-allocation-graph-rag-in-operating-system/)
- As part of our user study, we will present the user with some examples of programs that may or may not have deadlocks, askng them to try and identify them on their own. We will then use our visualizations to ask whether they may be helpful for the user to more proactively identify such issues.
- For example, we have a multi-threaded transaction management system for a video shop, with various configurations of threads and mutexes, to illustrate the utility of our static analysis and visualization components.

Notes about first user study results.

- The details of the user study have been completed, and we are running it with one user.
- We are on track to have the user study done by the next check-in.

Any changes to original design.

- We encountered several issues with setting up the environment and tooling for the project on different machines. While these issues have been largely resolved, we are still concerned about other configuration issues that may occur.
  - To remedy this, we are considering moving the project into a Docker container with preinstalled LLVM/Clang libraries.
- We are also considering expanding the scope of the project (time permitting) to track resource dependencies on heap objects.
- We are considering reducing our false-positive rate by taking into account symbolic execution to separate different paths a program can take.

Progress against the timeline planned for your team, including the specific goals you defined (as part of Check-in 2) for Check-in 3; any revisions to future Check-in goals.

- We are working on the user study and expect to have the results uploaded by the next check-in.
- We have a basic mockup of the planned visualization for select cases of cycles and how they could be displayed on the frontend, and our frontend implementation has D3.js set up and can read input.json.
- We have a general understanding of the the JSON format outputted that would be used by the frontend, see input.json in `frontend/src/input.json`
- We have basic code that works with LLVM/Clang to visit the AST of simple programs, including identifying usages of the `pthread` library.

## User Study 1

See `user-study-procedure.md` for context.

## User Background Information from Starting Questions

- User understands in an abstract sense what mutexes, threads, and race conditions are
- User knows what deadlocks are and was able to give an example (multiple threads waiting for each other in a "circular chain")
- User was familiar with C programming, although had not used the `pthread.c` library, which is what the examples were based upon
- User had not previously used any software to understand deadlock in code better; however, used has usedprogram analysis tools in the past, such as valgrind

## Example 1

### What They Found Easy

- Code was clear and the example was simple (only 2 threads)

### What They Found Difficult

- Since the user had not previously used the library for threads, they had to read through the documentation for the library
- User believed to have found a race condition that did not exist, possibly due to their low knowledge of the library
- Could not figure out some aspects of the code until the second example, when the user was able to the compare programs
- Gave the wrong answer (though that the program would produce a deadlock)

## Example 2

### What They Found Easy

- User was easily able to spot the difference between the first and second examples
- User has developed a good intuitive sense of threads and how the library works, which led to a correct prediction of program behavior (a deadlock)

### What They Found Difficult

- Still a little bit of syntactic difficulties (e.g., thought that the second mutex was not initialized at first)

## Example 3

### What They Found Easy

- Example was still somewhat similar to the first two (same context and structs)
- Was able to eventually give a correct answer after some time

### What They Found Difficult

- Codebase became more complex (3 functions, 3 mutexes)
- User needed to go up and down the codebase to mentally create a graph-like structure in their head
- User sometimes made a mental mistake when creating their mental model, but eventually corrected themself (this slowed their thinking down)
- Somewhat difficult to keep track of print statements and execution order

## What Did You Learn From Your Users: User Thoughts on Visualization Tools from Visualization Questions

- The way that the user created their own mental model of deadlock analysis was very similar to how a resource allocation graph looks, making the graph intuitive to use
- User was able to match a given resource allocation graph with a code snippet based on only skimming an article about resource allocation graphs due to its intuitiveness
- User believed that resource allocation graphs would be more useful in complex codebases where it would be difficult to mentally juggle resources/mutexes as there could be a tradeoff in small programs where the time it takes to run the program analysis could be higher than the time it would have taken to mentally figure the deadlock out
- If the user had a choice, they would use resource allocation graphs to help debug deadlocks

## What Would You Have Done Differently

- If resources and time permitted, more users could have been studied, especially within our target audience, "Developers working on multithreaded, concurrent systems that involve shared resources and synchronization using mutexes."
- If resources and time permitted, it would have been good to have two groups: one not given a resource allocation graph for the examples, and one given a resource allocation graph; then, internal validity could be established better
- User stated that the third example was still a bit too simple to fully illustrate the need for a resource allocation graph. Therefore, a more complex example may have been better; however, this would take a lot of time

## Check-in 4 Report

Status of implementation so far.

- We have a basic MVP for the analyzer in place. It can handle simple C programs that utilize threads and mutexes. It can also detect whether threads are running concurrently, and whether or not two threads will definitely or possibly deadlock.
- We have implemented a JSON format that can represent a graph of lock dependencies, threads and shared objects.
- Visualization can read and display data. The visualization can display the nodes and the edges of the directed graph, but currently there is no interaction and there are no details added. There are also visual issues, such as overlapping arrows, text not being displayed, etc.

Plans for the final user study.

- We plan to run a controlled experiment with two users to evaluate whether our program analyzer is useful for detecting and visualizing deadlocks.
- The procedure will be similar to the first user study.
  - We will give each user a program that contains a deadlock. One of them will be able to use our deadlock visualization program.
  - Each user will be timed on how quickly they are able to find the deadlock.
  - In addition, we will gather each user's subjective opinions on the difficulty of the task. For the user that used our program analyzer, we will ask them whether they believed the visualization made it easier for them to spot the deadlocks.

Planned timeline for the remaining days.

- Over the next week the visualization should be polished, and interaction and more detail added. Visualization issues will be patched, such as cases where there is overlapping text/arrows, etc.
- We will work on integrating the frontend and backend together. Specifically, we will pass the JSON output from the analyzer to our frontend program, which will output a URL that takes the user to the deadlock visualization.
- We will also extend our program analysis to work on loops, conditionals and recursion.

Progress against the timeline planned for your team, including the specific goals you defined (originally as part of Check-in 2) for Check-in 4; any revisions to Check-in 5 goals.

- On the visualization side, we are on track with check-in 2. We have some data displayed (the nodes and the edges for the directed graph), however the displayed data is very crude and there is no interaction yet.
- Our program analyzer can handle simple cases of deadlock, and we have some basic tests here to show for it.
- The first user study is complete. We are currently working on the test procedure for the final user study.
- We have not started working on the video yet, as we are working on improving the program analysis to handle more complex cases.
  - Once this is complete, we will have a good idea of what to include in the video.

## Check-in 5 Report

Status of final user study; any feedback and changes planned:

- We have finalized the user study procedure and will be administering it in the coming days.
- As part of the user study, we will ask for feedback on whether the information density of the visualization is appropriate, and incorporate changes to the analysis and frontend to improve it.

Plans for final video:

- Our video will follow a similar format to the first project:
- We will include a demo showing how users can input multithreaded C programs, along with the various features of our visualization

Planned timeline for the remaining days:

- We will prioritize work on adding more information to our program analysis, including the control flow graph, call graph and line numbers.
- We will update the JSON format accordingly and add more visualization features to display the data.
- Time permitting, we will implement a Docker container that will make it easy for users to deploy tne analyzer and frontend website.

Progress against the timeline planned for your team, including the specific goals you defined (originally as part of Check-in 2) for Check-in 5.

- While we didn't have specific goals for check-in 5, we are pretty behind where we need to be.
  - The delay was mainly due to figuring out how detailed our analysis/visualization should be, along with some other implementation issues.
- The remaining tasks are mostly intended to refine the analysis and user experience, so they won't require as much time as the prior tasks.

## User Study 2

For our second user study, we utilize the completed project with the code samples that were provided previously as our study from user student #1. This time, we have two different subjects. One subject will be given the three programs and asked to determine if there is a deadlock, and if there is where the deadlock may be if so. They will not have access to our tool. 

The second user will have access to the tool but may need to be wary of false positives (i.e. our tool is reporting that there is a deadlock when there isn't). For this reason, while they have access to the tool, they must be confident and report accurately if there is a deadlock or not and only use the tool as an aid to finding the deadlock and not as the sole source of reasoning. 

Our goal is to compare the speeds and accuracy of the two users to verify if our tool can help programmers out. Does our tool save a decent amount of time from the programmers’ debug time? Three more concrete questions that we hope to answer as follows:
- Does our tool increase accuracy of deadlock reports?
- Does our tool make it faster to find deadlocks?
- Does our tool increase the likelihood to respond yes to potential deadlocks in the case of a false-positives? 

Again, note that the same examples have been carried over from study #1 but this time in a more applied setting. Both programmers studied are 4th-year Computer Science students with decent multithreaded and concurrent programming experience and are on a relative playing field. If more time permits, increasing the sample size and normalizing the times between the two subjects will help us develop a better understanding of the effectiveness of our tool.

Also note that participants are unable to run the code. While deadlocks are an inherently dynamic-based runtime problem, we wanted to focus on the debugging aspect where programmers often look at the code and trace locks manually (in a more static fashion). 

### Example 1: No Deadlock

#### User 1 (no program)

The time it took for the first programmer to find the deadlock was 1:24.70. They were accurate and correctly indicated the deadlock.

#### User 2 (access to program)

The second user took a total of 1:47.49 which was roughly ~23 seconds longer than the tester without access to the program. We hypothesize while this tester was able to identify that there was a "potential" deadlock thanks to the cycle that appeared in our program, it took them longer to verify and become certain about this was a correct conclusion.

Also, there was a bit of a learning curve for the user to get used to the program (and bouncing back and forth with the code it was analyzing it). The ability to click on nodes and edges to view more information like line numbers was something that they remarked should have been more clearly communicated. It helped narrow down the places to look in the program to ensure that the correct conclusion was made. 

### Example 2: Simple Deadlock

#### User 1 (no program)

While it took the user 1:42.41 which was relatively fast compared to the expectation of this program, they came to an incorrect conclusion thinking that there was no deadlock. 

After showing the user, the graph generated by our program they, quickly realized their mistake as a blunder and was immediately able point out where the deadlock that they missed was. This was a particularly interesting test to note, as it validated the idea of a program that at least points engineers in the right direction as something valuable. They are then able to reconcile mistakes faster, something helpful during fast development cycles. 

#### User 2 (access to program)

It took the user 56.08 seconds to report the issue and the fact that this program had a deadlock. At this point, the programmer was much more familiar with how to use our program and was able to narrow down the issue far faster than in the first example. The deadlock cycle that was reported clearly in the program and gave the engineer an informed area to look within the program, helping narrow down the scope tremendously.

### Example 3: Complex Deadlock

#### User 1 (no program)

With this one, it took the user 1:54.23. They were correct about the deadlock and location. There was nothing particularly of note with this one, aside from the fact that it was hard to trace mentally, and a pencil and paper perhaps would have helped to allow the user to draw a graph, like the type that our program would report. 

#### User 2 (access to program)

It took the user with access to the program 1:39.69 and were correct. Again, they reported that the program helped narrow it down far faster as the larger cycle with multiple dependencies was able to help narrow the scope of where to look for dependencies and waits when referring to the code. For this one, the graph generated was particularly helpful, although longer time was needed for correct verification of the program output.

# Feedback

We also asked our participants for feedback regarding the visualization (the first participant after writing the code and seeing what the visualization would have shown if it was accessible during the programming stage) and the second who was able to see it as it went on.

The main point of discussion was adding more instructions on where to click within the graph to see the additional details. To remedy this, we added a more thought-out guide to the sidebar. We also ensured that bugs that users experienced like nodes floating off the screen when adjusting the graph was fixed. 

The way to use the tool was a bit unclear from further levels. Setting up and running the Docker container, while simple, had a high overhead on developers machines (taking a few gigabytes of space to install the LLVM toolchains and NPM packages) along with a lot of RAM usage when running. 

Another point worth mention is the fact that we are very open ended and broad in our interpretation of "potential deadlocks" lead programmers to doubt the results and led to a lot of wasted time checking to make sure we weren't reporting false positives. The first example conducted during our user studies is a good example of this. It resulted in the tester who did not use our program to find the deadlock faster than the one who did use it, as the second person was wary that it was an incorrect report (and had to go back and forth with the code to double check). Because of this, we figured that our idea to add a control-flow-graph to our visualizations would help programmers go back to the code and pinpoint whether the warning was valid or not and proceed accordingly. 

# Conclusion

Overall, our findings indicated that being able to represent dependencies and threads in program as a graph helped programmers and provided a helpful second way to look at programs. Using it as a debugging tool as opposed to tracing code out by hand statically seems to be useful, especially if a program tends to exhibit undefined and non-deterministic behaviour during runtime (which many programs with concurrency issues tend to do). 