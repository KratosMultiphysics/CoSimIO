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
    def test_basics_int(self):
        info = CoSimIO.Info()

        self.asserFalse(info.Has("echo_level"))

        info.SetInt("echo_level", 1)

        self.assertTrue(info.Has("echo_level"))

        self.assertEqual(info.GetInt("echo_level"), 1)

    def test_basics_double(self):
        info = CoSimIO.Info()

        self.asserFalse(info.Has("tolerance"))

        info.SetDouble("tolerance", 1.5)

        self.assertTrue(info.Has("tolerance"))

        self.assertAlmostEqual(info.GetDouble("tolerance"), 1.5)

    def test_basics_bool(self):
        info = CoSimIO.Info()

        self.asserFalse(info.Has("print_sth"))

        info.SetBool("print_sth", False)

        self.assertTrue(info.Has("print_sth"))

        self.assertFalse(info.GetBool("print_sth"))

    def test_basics_string(self):
        info = CoSimIO.Info()

        self.asserFalse(info.Has("identifier"))

        info.SetString("identifier", "pressure")

        self.assertTrue(info.Has("identifier"))

        self.assertEqual(info.GetString("identifier"), "pressure")


if __name__ == '__main__':
    unittest.main()
