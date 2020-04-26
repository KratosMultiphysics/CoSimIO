#     ______     _____ _           ________
#    / ____/___ / ___/(_)___ ___  /  _/ __ |
#   / /   / __ \\__ \/ / __ `__ \ / // / / /
#  / /___/ /_/ /__/ / / / / / / // // /_/ /
#  \____/\____/____/_/_/ /_/ /_/___/\____/
#  Kratos CoSimulationApplication
#
#  License:         BSD License, see license.txt
#
#  Main authors:    Philipp Bucher (https://github.com/philbucher)
#

# python imports
import unittest, sys, os
import subprocess


# This test is not strictly necessary, since the test-runner can also be executed directly
# Hence it is only for better integration with the remaining tests, such that they can be run alltogether
class CoSimIOTests(unittest.TestCase):
    def test_with_testrunner(self):
        if os.name=="nt":
            cmd = "Debug/test_runner_co_sim_io_tests.exe"
        else:
            cmd = "./test_runner_co_sim_io_tests"

        sp = subprocess.Popen(
            [cmd],
            stdout=subprocess.PIPE,
            cwd=os.path.join(os.path.dirname(os.path.abspath(__file__)), "../../", "bin"))

        process_stdout, process_stderr = sp.communicate()
        if process_stdout:
            print(process_stdout.decode('ascii'), file=sys.stdout)
        if process_stderr:
            print(process_stderr.decode('ascii'), file=sys.stderr)

        self.assertEqual(sp.returncode, 0)


if __name__ == '__main__':
    unittest.main()
