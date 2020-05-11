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

# tests for the python exposure of CoSimIO::Info

# python imports
import unittest, sys, os

import CoSimIO

class CoSimIO_Info_Tests(unittest.TestCase):
    maxDiff = None # to display all the diff

    def test_basics_int(self):
        info = CoSimIO.Info()

        self.assertFalse(info.Has("echo_level"))

        info.SetInt("echo_level", 1)

        self.assertTrue(info.Has("echo_level"))

        self.assertEqual(info.GetInt("echo_level"), 1)

    def test_basics_double(self):
        info = CoSimIO.Info()

        self.assertFalse(info.Has("tolerance"))

        info.SetDouble("tolerance", 1.5)

        self.assertTrue(info.Has("tolerance"))

        self.assertAlmostEqual(info.GetDouble("tolerance"), 1.5)

    def test_basics_bool(self):
        info = CoSimIO.Info()

        self.assertFalse(info.Has("print_sth"))

        info.SetBool("print_sth", False)

        self.assertTrue(info.Has("print_sth"))

        self.assertFalse(info.GetBool("print_sth"))

    def test_basics_string(self):
        info = CoSimIO.Info()

        self.assertFalse(info.Has("identifier"))

        info.SetString("identifier", "pressure")

        self.assertTrue(info.Has("identifier"))

        self.assertEqual(info.GetString("identifier"), "pressure")

    def test_non_existing_key(self):
        info = CoSimIO.Info()

        self.assertFalse(info.Has("identifier"))

        with self.assertRaisesRegex(Exception, 'Error: '): # TODO improve this once errors are working better
            info.GetBool("identifier")

    def test_wrong_type(self):
        info = CoSimIO.Info()

        self.assertFalse(info.Has("identifier"))

        info.SetString("identifier", "pressure")

        self.assertTrue(info.Has("identifier"))

        with self.assertRaisesRegex(Exception, 'Error: '): # TODO improve this once errors are working better
            info.GetBool("identifier")

    def test_many_values(self):
        info = CoSimIO.Info()

        self.assertFalse(info.Has("identifier"))
        self.assertFalse(info.Has("is_converged"))
        self.assertFalse(info.Has("tol"))
        self.assertFalse(info.Has("echo_level"))

        info.SetString("identifier", "velocity_interface")
        info.SetBool("is_converged", True)
        info.SetDouble("tol", 0.008)
        info.SetInt("echo_level", 2)

        self.assertTrue(info.Has("identifier"))
        self.assertTrue(info.Has("is_converged"))
        self.assertTrue(info.Has("tol"))
        self.assertTrue(info.Has("echo_level"))

        self.assertEqual(info.GetString("identifier"), "velocity_interface")
        self.assertTrue(info.GetBool("is_converged"))
        self.assertAlmostEqual(info.GetDouble("tol"), 0.008)
        self.assertEqual(info.GetInt("echo_level"), 2)

    def test_set_alreay_existing(self):
        info = CoSimIO.Info()

        self.assertFalse(info.Has("identifier"))
        info.SetString("identifier", "velocity_interface")
        self.assertTrue(info.Has("identifier"))
        self.assertEqual(info.GetString("identifier"), "velocity_interface")

        # now overwriting the already existing value
        info.SetString("identifier", "pressure")
        self.assertEqual(info.GetString("identifier"), "pressure")

    def test_set_alreay_existing_different_data_type(self):
        info = CoSimIO.Info()

        self.assertFalse(info.Has("identifier"))
        info.SetString("identifier", "velocity_interface")
        self.assertTrue(info.Has("identifier"))
        self.assertEqual(info.GetString("identifier"), "velocity_interface")

        # now overwriting the already existing value with a different type
        info.SetInt("identifier", 15)
        self.assertEqual(info.GetInt("identifier"), 15)

    def test_size(self):
        info = CoSimIO.Info()
        self.assertTrue(info.Size() == 0)

        info.SetString("identifier", "velocity_interface")
        info.SetBool("is_converged", True)
        info.SetDouble("tol", 0.008)
        info.SetInt("echo_level", 2)

        self.assertEqual(info.Size(), 4)
        info.SetInt("echo_level", 6)
        self.assertEqual(info.Size(), 4)

    def test_clear(self):
        info = CoSimIO.Info()
        self.assertTrue(info.Size() == 0)

        info.SetString("identifier", "velocity_interface")
        info.SetBool("is_converged", True)
        info.SetDouble("tol", 0.008)
        info.SetInt("echo_level", 2)

        self.assertEqual(info.Size(), 4)
        info.SetInt("echo_level", 6)
        self.assertEqual(info.Size(), 4)

        info.Clear()
        self.assertEqual(info.Size(), 0)

    def test_erase(self):
        info = CoSimIO.Info()
        self.assertTrue(info.Size() == 0)

        info.SetString("identifier", "velocity_interface")
        self.assertTrue(info.Has("identifier"))
        self.assertTrue(info.Size() == 1)

        info.Erase("identifier")
        self.assertFalse(info.Has("identifier"))
        self.assertTrue(info.Size() == 0)

        # erasing non-existing keys does not throw
        info.Erase("identifier")
        info.Erase("whatever")

    def test_print(self):
        info = CoSimIO.Info()

        info.SetString("keyword", "awesome")
        info.SetBool("is_converged", True)
        info.SetString("keyword", "awesome")
        info.SetDouble("tol", 0.008)
        info.SetInt("echo_level", 2)
        info.SetInt("checking", 22)

        exp_string = "CoSimIO-Info; containing 5 entries\n  name: checking | value: 22 | type: int\n  name: echo_level | value: 2 | type: int\n  name: is_converged | value: 1 | type: bool\n  name: keyword | value: awesome | type: string\n  name: tol | value: 0.008 | type: double\n"

        self.assertMultiLineEqual(str(info), exp_string)


if __name__ == '__main__':
    unittest.main()
