# Sprint 2

## Changming Qiu

I used Claude for concept explanation, overall architecture guidance, Git workflow (especially moving the repo from a personal account to an organization structure), and environment troubleshooting.

Claude accelerated my work in several ways. It explained fork, execvp, and waitpid with a clear analogy of a parent cloning itself and the clone becoming the target program, which made process creation click faster than the textbook alone. It also broke the scheduler work into small, ordered steps, so I built job.h, the FIFO and priority schedulers, and the workload generator one piece at a time instead of all at once. When my VM lost DNS resolution, Claude helped trace the cause to a network switch from a phone hotspot to home WiFi, where the VM kept the old DNS.

Claude was not always right. At one point it miscounted the commits in my pull request and told me a commit was missing, which sent me to verify something that was actually fine. This reminded me to confirm claims against the real state myself.

## Zihan Guo

I used ChatGPT during Sprint 2 to help understand UNIX process management, plan the implementation of the interactive shell, troubleshoot errors, and design tests. I used it as a learning and debugging assistant while implementing the REPL loop, command parsing, and the executor using `fork()`, `execvp()`, and `waitpid()`.

ChatGPT helped me work faster by breaking the shell implementation into smaller tasks. One useful example was integrating batch job submission with the FIFO and priority schedulers. It helped me think through how to preserve command arguments, validate user input, and test execution order. I also learned how to use AddressSanitizer to check for memory errors and verified the shell with different priorities, invalid commands, and input limits.

However, AI assistance still required careful verification. During development, a typo in the input-handling code caused a compilation problem, and I had to correct it and rebuild the program. This experience reminded me that AI-generated suggestions are not automatically reliable. I learned to verify changes through compilation, testing, and reviewing the actual program output.

# Sprint 3

## Changming Qiu

I used Claude as my main AI tool in Sprint 3.

Where it helped: It turned the Sprint 3 rubric into a gap analysis and a task split for our team. It helped me design the benchmark rig. The key idea was to measure each job from outside the shell, with a wrapper script that logs its own timestamps. It drafted the benchmark scripts, the README, and my paper sections. I ran every script, checked each output, and verified the numbers by hand. It also caught a real problem: our LaTeX table rounded small standard deviations to zero.

Where it fell short: It gave me a Mac file path when I was working on the VM. It assumed Parker would share the LXC device, but he had sent a user invite, so the setup steps changed. Twice it predicted the wrong output for a check command. It also suggested extra work, such as a Piazza post and a second experiment, that our sprint did not need. I learned to check each suggestion against the rubric and the real output before acting on it.
