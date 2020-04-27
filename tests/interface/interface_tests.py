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

import sys

# import CoSimIO

def my_test_for_info():
    pass


registered_tests = {
    "test_Info" : my_test_for_info
}


def RunTest(test_name):
    if test_name in registered_tests:
        registered_tests[test_name]()
    else:
        raise Exception('Test name is unknown: "{}"'.format(test_name))


if __name__ == '__main__':
    if len(sys.argv) != 2:
        raise Exception("One input argument required: Name of the test to execute");

    test_name = sys.argv[1]
    RunTest(test_name)
