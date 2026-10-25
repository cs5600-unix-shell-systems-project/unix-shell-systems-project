## Changming Qiu

I used Claude for concept explanation, overall architecture guidance, Git workflow (especially moving the repo from a personal account to an organization structure), and environment troubleshooting.

Claude accelerated my work in several ways. It explained fork, execvp, and waitpid with a clear analogy of a parent cloning itself and the clone becoming the target program, which made process creation click faster than the textbook alone. It also broke the scheduler work into small, ordered steps, so I built job.h, the FIFO and priority schedulers, and the workload generator one piece at a time instead of all at once. When my VM lost DNS resolution, Claude helped trace the cause to a network switch from a phone hotspot to home WiFi, where the VM kept the old DNS.

Claude was not always right. At one point it miscounted the commits in my pull request and told me a commit was missing, which sent me to verify something that was actually fine. This reminded me to confirm claims against the real state myself.