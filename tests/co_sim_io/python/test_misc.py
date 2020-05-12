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

# miscellaneous for the CoSimIO

# python imports
import unittest

import CoSimIO


class CoSimIO_Misc(unittest.TestCase):

    def test_version(self):
        self.assertTrue(hasattr(CoSimIO, "__version__"))
        self.assertNotEqual(CoSimIO.__version__, "")


if __name__ == '__main__':
    unittest.main()
