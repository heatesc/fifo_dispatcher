# Simple Toy Dispatcher

This is a simple first come first serve toy dispatcher implemented in c. I've written this to aid my
learning of some operating systems stuff, so maybe if you're learning about operating systems,
you would find this useful as an educational resource. It works as follows:

- You require a job schedule file, where there is one line per job. You could could use the one provided (schedules/sched1)
- A line is of the form (arrival time), (service time)
- The dispatcher will execute a dummy program on each dispatch (src/program.c)

## Installation

This isn't the most interesting project, but if for whatever reason you would like
to try running it yourself, simply download the project and run the following.

```bash
# compile
make
make build_dummy_prog

# run
./disp <path to job schedule file>
```

## Video

https://github.com/user-attachments/assets/2ba6b1b8-23b4-4395-8af9-f2004acb6a5b
