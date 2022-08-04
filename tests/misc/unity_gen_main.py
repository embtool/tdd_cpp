#!/usr/bin/env python3

import sys

if len(sys.argv) != 3:
    print("Usage: unity_gen_main.py RUNNER MAIN")
    sys.exit(-1)

runner_filename = sys.argv[1]
main_filename = sys.argv[2]

# Read runner code
with open(runner_filename, "r") as fp:
    runner = fp.readlines()

# Obtain the list with runners
runner = [f"{line.strip()};" for line in runner if "TEST_GROUP_RUNNER(" in line]
runner = "\n    ".join(runner)

# Change declaration to calling the runner
runner = runner.replace("TEST_GROUP_RUNNER(", "RUN_TEST_GROUP(")

# Insert runners in the main code
main = f"""\
#include "unity_fixture.h"

static void runAllTests(void)
{{
    {runner}
}}

int main(int argc, const char *argv[])
{{
    return UnityMain(argc, argv, runAllTests);
}}
"""

with open(main_filename, "w") as fp:
    fp.write(main)
