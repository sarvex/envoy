#!/usr/bin/env python
from __future__ import print_function

import os
import signal
import sys
import time

# The number of seconds to wait for children to gracefully exit after
# propagating SIGTERM before force killing children.
# NOTE: If using a shutdown mechanism such as runit's `force-stop` which sends
# a KILL after a specified timeout period, it's important to ensure that this
# constant is smaller than the KILL timeout
TERM_WAIT_SECONDS = 30

restart_epoch = 0
pid_list = []


def term_all_children():
  """ Iterate through all known child processes, send a TERM signal to each of
  them, and then wait up to TERM_WAIT_SECONDS for them to exit gracefully,
  exiting early if all children go away. If one or more children have not
  exited after TERM_WAIT_SECONDS, they will be forcibly killed """

  # First uninstall the SIGCHLD handler so that we don't get called again.
  signal.signal(signal.SIGCHLD, signal.SIG_DFL)

  global pid_list
  for pid in pid_list:
    print(f"sending TERM to PID={pid}")
    try:
      os.kill(pid, signal.SIGTERM)
    except OSError:
      print(f"error sending TERM to PID={pid} continuing")

  all_exited = False

  # wait for TERM_WAIT_SECONDS seconds for children to exit cleanly
  retries = 0
  while not all_exited and retries < TERM_WAIT_SECONDS:
    for pid in list(pid_list):
      ret_pid, exit_status = os.waitpid(pid, os.WNOHANG)
      if ret_pid == 0 and exit_status == 0:
        # the child is still running
        continue

      pid_list.remove(pid)

    if len(pid_list) == 0:
      all_exited = True
    else:
      retries += 1
      time.sleep(1)

  if all_exited:
    print("all children exited cleanly")
  else:
    for pid in pid_list:
      print(f"child PID={pid} did not exit cleanly, killing")
    force_kill_all_children()
    sys.exit(1)  # error status because a child did not exit cleanly


def force_kill_all_children():
  """ Iterate through all known child processes and force kill them. Typically
  term_all_children() should be attempted first to give child processes an
  opportunity to clean up state before exiting """

  global pid_list
  for pid in pid_list:
    print(f"force killing PID={pid}")
    try:
      os.kill(pid, signal.SIGKILL)
    except OSError:
      print(f"error force killing PID={pid} continuing")

  pid_list = []


def shutdown():
  """ Attempt to gracefully shutdown all child Envoy processes and then exit.
  See term_all_children() for further discussion. """
  term_all_children()
  sys.exit(0)


def sigterm_handler(signum, frame):
  """ Handler for SIGTERM. """
  print("got SIGTERM")
  shutdown()


def sigint_handler(signum, frame):
  """ Handler for SIGINT (ctrl-c). The same as the SIGTERM handler. """
  print("got SIGINT")
  shutdown()


def sighup_handler(signum, frame):
  """ Handler for SIGUP. This signal is used to cause the restarter to fork and exec a new
      child. """

  print("got SIGHUP")
  fork_and_exec()


def sigusr1_handler(signum, frame):
  """ Handler for SIGUSR1. Propagate SIGUSR1 to all of the child processes """

  global pid_list
  for pid in pid_list:
    print(f"sending SIGUSR1 to PID={pid}")
    try:
      os.kill(pid, signal.SIGUSR1)
    except OSError:
      print(f"error in SIGUSR1 to PID={pid} continuing")


def sigchld_handler(signum, frame):
  """ Handler for SIGCHLD. Iterates through all of our known child processes and figures out whether
      the signal/exit was expected or not. Python doesn't have any of the native signal handlers
      ability to get the child process info directly from the signal handler so we need to iterate
      through all child processes and see what happened."""

  print("got SIGCHLD")

  kill_all_and_exit = False
  global pid_list
  pid_list_copy = list(pid_list)
  for pid in pid_list_copy:
    ret_pid, exit_status = os.waitpid(pid, os.WNOHANG)
    if ret_pid == 0 and exit_status == 0:
      # This child is still running.
      continue

    pid_list.remove(pid)

    # Now we see how the child exited.
    if os.WIFEXITED(exit_status):
      exit_code = os.WEXITSTATUS(exit_status)
      print(f"PID={ret_pid} exited with code={exit_code}")
      if exit_code != 0:
        # Something bad happened. We need to tear everything down so that whoever started the
        # restarter can know about this situation and restart the whole thing.
        kill_all_and_exit = True
    elif os.WIFSIGNALED(exit_status):
      print(f"PID={ret_pid} was killed with signal={os.WTERMSIG(exit_status)}")
      kill_all_and_exit = True
    else:
      kill_all_and_exit = True

  if kill_all_and_exit:
    print("Due to abnormal exit, force killing all child processes and exiting")

    # First uninstall the SIGCHLD handler so that we don't get called again.
    signal.signal(signal.SIGCHLD, signal.SIG_DFL)

    force_kill_all_children()

  # Our last child died, so we have no purpose. Exit.
  if not pid_list:
    print("exiting due to lack of child processes")
    sys.exit(1 if kill_all_and_exit else 0)


def fork_and_exec():
  """ This routine forks and execs a new child process and keeps track of its PID. Before we fork,
      set the current restart epoch in an env variable that processes can read if they care. """

  global restart_epoch
  os.environ['RESTART_EPOCH'] = str(restart_epoch)
  print(f"forking and execing new child process at epoch {restart_epoch}")
  restart_epoch += 1

  child_pid = os.fork()
  if child_pid == 0:
    # Child process
    os.execl(sys.argv[1], sys.argv[1])
  else:
    # Parent process
    print(f"forked new child process with PID={child_pid}")
    pid_list.append(child_pid)


def main():
  """ Script main. This script is designed so that a process watcher like runit or monit can watch
      this process and take corrective action if it ever goes away. """

  print(f"starting hot-restarter with target: {sys.argv[1]}")

  signal.signal(signal.SIGTERM, sigterm_handler)
  signal.signal(signal.SIGINT, sigint_handler)
  signal.signal(signal.SIGHUP, sighup_handler)
  signal.signal(signal.SIGCHLD, sigchld_handler)
  signal.signal(signal.SIGUSR1, sigusr1_handler)

  # Start the first child process and then go into an endless loop since everything else happens via
  # signals.
  fork_and_exec()
  while True:
    time.sleep(60)


if __name__ == '__main__':
  main()
