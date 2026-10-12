# Preliminary Exploration Report

## Project G.2: UNIX Shell

### 1. Introduction

The goal of this exploration is to study existing UNIX shell implementations before designing our own shell.

We selected three shells for comparison: Bash, Dash, and Zsh. All three were tested on an Ubuntu 24.04.5 LTS virtual machine.

We focused on basic command execution, pipelines, output redirection, and background process management.

These features are important for our project because we plan to build a UNIX shell with a priority-based batch scheduling extension.

By comparing existing shells, we want to understand how they handle processes and identify which features and mechanisms we should consider in our own implementation.

### 2. Experimental Environment

- Operating System: Ubuntu 24.04.5 LTS
- Architecture: ARM64 (aarch64)
- Virtualization: VirtualBox
- Shell 1: GNU Bash 5.2.21
- Shell 2: Dash 0.5.12-6ubuntu5
- Shell 3: Zsh 5.9

All experiments were performed inside the Ubuntu virtual machine.

### 3. Existing Shell Implementations

#### 3.1 GNU Bash

Bash is a widely used UNIX shell that supports command execution, pipelines, redirection, scripting, and job control.

In our experiments, Bash successfully executed basic commands, processed a multi-stage pipeline, and redirected output to a file.

We also tested background process management using `sleep 60 &`, `jobs -l`, and `kill %1`.

Bash allowed us to start a background process, check its running status, and terminate it.

#### 3.2 Dash

Dash is a lightweight POSIX-compatible shell commonly used for system scripts.

Our tests showed that Dash supports basic command execution, pipelines, and output redirection.

Dash also successfully handled background processes. We were able to start a background job, view its status using `jobs -l`, and terminate it using `kill %1`.

This shows that even a lightweight shell can provide basic process management features.

#### 3.3 Zsh

Zsh is a UNIX shell that provides scripting capabilities and advanced interactive features.

In our experiments, Zsh successfully executed basic commands, pipelines, and output redirection.

We also tested its background job management.

Zsh allowed us to start a background process and inspect its status. After terminating the process, Zsh displayed a termination notification.

This behavior was slightly different from what we observed in Bash and Dash.
