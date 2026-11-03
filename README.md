# UNIX Shell with Priority-Based Batch Scheduling

CS 5600 Computer Systems Final Project, based on OSTEP Project G.2.

## Language

This project is written in **C**.

## Build and Run

Build the shell:

    make

Run the shell:

    ./shell

Build and run the scheduler test program:

    make scheduler-demo
    ./scheduler-demo

Run the executor tests:

    make test

Remove build files:

    make clean

## Usage

Interactive mode. Type a command and it runs immediately:

    myshell> ls -l

Batch mode. Submit jobs with a priority from 1 (low) to 3 (high), then run them with a scheduling policy:

    myshell> submit 1 echo low
    myshell> submit 3 echo high
    myshell> run priority

Use `run fifo` to run the same jobs in submission order. Type `exit` to quit.

## Team Contribution Tracker

[View Team Contribution Tracker](https://docs.google.com/spreadsheets/d/1VAaoLCXTdNVwtM9zk3o1P9Id3Qjeg_ShEKmaY1Y56eM/edit?usp=sharing)

## Team Members

- Zihan Guo
- Changming Qiu
- Parker McKillop: exempt from Sprint 2 by instructor accommodation
- Rowan Lowden: no longer on the team

## Sprint 2 Deliverables

- [Annotated Bibliography](docs/bibliography.md), with the Research Extension Update at the top
- [Glossary](docs/glossary.md)
- [AI Reflections](docs/ai-reflections.md)
- [Sprint 2 Design Note](docs/architecture/sprint2-design-note.md)
- Skeleton code in `src/`

## Sprint 1 Deliverables

- [Research Proposal (PDF)](proposal/Research_Proposal_Sprint1.pdf)
- [Shell Exploration Report](docs/exploration.md) and [screenshots](docs/screenshots/)
- UML Sequence Diagrams:
  - [Basic Command Execution](docs/architecture/diagram1_basic_command.png)
  - [Pipeline Execution](docs/architecture/diagram2_pipeline_execution.png)
  - [Priority-Based Batch Scheduling](docs/architecture/diagram3_priority_batch_scheduling.png)

## Project Structure

- `src/` : shell, executor, and the FIFO and priority schedulers
- `tests/` : executor tests
- `benchmarks/` : workload generator for scheduling experiments
- `docs/` : bibliography, glossary, AI reflections, exploration report, and UML diagrams
- `proposal/` : research proposal

## Open-Source Code

No third-party code is included in this repository. Sources we studied are listed in the bibliography.