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

# tests for the python exposure of CoSimIO::Vector
# (which is a small python wrapper fro std::vector<double>)

# python imports
import unittest

import CoSimIO


class CoSimIO_Vector(unittest.TestCase):
    maxDiff = None # to display all the diff

    def test_basics(self):
        vec = CoSimIO.Vector()
        self.assertEqual(vec.size(), 0)
        self.assertEqual(len(vec), 0)

        vec.append(15)
        self.assertEqual(vec.size(), 1)
        self.assertEqual(len(vec), 1)
        self.assertAlmostEqual(vec[0], 15)

        vec.append(-1.45)
        self.assertEqual(vec.size(), 2)
        self.assertEqual(len(vec), 2)
        self.assertAlmostEqual(vec[0], 15)
        self.assertAlmostEqual(vec[1], -1.45)

    def test_construction_from_objects(self):
        init_list = [1.0, 2.5, 3.3, -4.1, 17]

        vec_from_list = CoSimIO.Vector(init_list)
        self.assertEqual(vec_from_list.size(), len(init_list))

        for l, v in zip(init_list, vec_from_list):
            self.assertAlmostEqual(l,v)

    def test_copy_constructor(self):
        vec = CoSimIO.Vector()
        for i in range(10):
            vec.append(i)

        vec_copy = CoSimIO.Vector(vec)

        self.assertEqual(vec.size(), vec_copy.size())
        for v, v_c in zip(vec, vec_copy):
            self.assertAlmostEqual(v, v_c)

        # make sure that it is a deep and not a shallow copy
        vec[0] = 123456
        self.assertAlmostEqual(vec_copy[0], 0) # copy is unchanged

        vec.resize(5)
        self.assertEqual(vec_copy.size(), 10)

    def test_resize(self):
        vec = CoSimIO.Vector()
        self.assertEqual(vec.size(), 0)

        vec.resize(10)
        self.assertEqual(vec.size(), 10)

        vec.resize(5)
        self.assertEqual(vec.size(), 5)

        vec.resize(23)
        self.assertEqual(vec.size(), 23)

    def test_iterators(self):
        vec = CoSimIO.Vector()
        for i in range(10):
            vec.append(i)

        self.assertEqual(vec.size(), 10)

        counter = 0
        for i, v in enumerate(vec):
            self.assertAlmostEqual(i,v)
            counter += 1
        self.assertEqual(counter, 10)

        # check for after resize to smaller and larger
        counter = 0
        vec.resize(5)
        for i, v in enumerate(vec):
            self.assertAlmostEqual(i,v)
            counter += 1
        self.assertEqual(counter, 5)

        counter = 0
        vec.resize(23)
        for i, v in enumerate(vec):
            counter += 1

        self.assertEqual(counter, 23)

    def test_print(self):
        vec = CoSimIO.Vector([1.2, 2.5, 3.3, -4.1, 17])

        self.assertMultiLineEqual(str(vec), "[1.2, 2.5, 3.3, -4.1, 17]")


if __name__ == '__main__':
    unittest.main()
