import subprocess
import json
import os
import pytest

TEST_CASE_DIR = "tests/cases"
EXPECTED_DIR = "tests/expected"
ACTUAL_DIR = "tests/output"
EXECUTABLE = "./bin/dsa"

# This file runs each test in the /analyzer/tests/cases folder
# and compares it with the expected output in /analyzer/tests/expected


def get_test_cases():
    return [f.replace(".c", "") for f in os.listdir(TEST_CASE_DIR) if f.endswith(".c")]


@pytest.mark.parametrize("test_case", get_test_cases())
def test_dsa(test_case):
    os.makedirs(ACTUAL_DIR, exist_ok=True)

    output = os.path.join(ACTUAL_DIR, f"{test_case}.json")

    subprocess.run(
        [EXECUTABLE, os.path.join(TEST_CASE_DIR, f"{test_case}.c"), "-o", output],
        check=True,
    )

    with (
        open(os.path.join(EXPECTED_DIR, f"{test_case}.json"), "r") as expected,
        open(output, "r") as actual,
    ):
        assert json.load(actual) == json.load(expected)
