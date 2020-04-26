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
from abc import ABCMeta, abstractmethod


from interface_tests import RunTest

if __name__ == '__main__':
    # necessary to import from parent dir
    sys.path.append(os.pardir)

from testing_utilities import IsOptionEnabled, LaunchSubprocess, CollectSubprocessResult


class TestInterfaces(object):
    class BaseTests(unittest.TestCase, metaclass=ABCMeta):
        '''wrapping in an extra class to avoid discovery of the base-test
        see https://stackoverflow.com/a/25695512
        '''

        def __GetTestName(self):
            return self.id().split(".")[-1]

        def setUp(self):
            self.sp = self._RunTestWithName(self.__GetTestName())

        def tearDown(self):
            self.assertEqual(CollectSubprocessResult(self.sp), 0)

        @abstractmethod
        def _RunTestWithName(self, test_name): pass

        def test_Info(self):
            print("My_id:", self.id())
            pass


@unittest.skipUnless(IsOptionEnabled("BUILD_TESTS"), "This test can only be executed if the tests are built")
class TestInterfaces_Cpp(TestInterfaces.BaseTests):
    def _RunTestWithName(self, test_name):
        return LaunchSubprocess('test_runner_cpp', test_name)


# @unittest.skipUnless(IsOptionEnabled("BUILD_PYTHON"), "This test can only be executed if CoSimIO for Python is built")
# class TestInterfaces_Python(TestInterfaces.BaseTests):
#     def _RunTestWithName(self, test_name):
#         RunTest(test_name)


@unittest.skipUnless(IsOptionEnabled("BUILD_C"), "This test can only be executed if CoSimIO for C is built")
@unittest.skipUnless(IsOptionEnabled("BUILD_TESTS"), "This test can only be executed if the tests are built")
class TestInterfaces_C(TestInterfaces.BaseTests):
    def _RunTestWithName(self, test_name):
        return LaunchSubprocess('test_runner_c', test_name)


@unittest.skipUnless(IsOptionEnabled("BUILD_FORTRAN"), "This test can only be executed if CoSimIO for Fortran is built")
@unittest.skipUnless(IsOptionEnabled("BUILD_TESTS"), "This test can only be executed if the tests are built")
class TestInterfaces_Fortran(TestInterfaces.BaseTests):
    def _RunTestWithName(self, test_name):
        return LaunchSubprocess('test_runner_fortran', test_name)

    # @unittest.skip("Not yet inmplemented")
    def test_Info(self):
        pass


if __name__ == '__main__':
    unittest.main()
