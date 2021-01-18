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
from abc import ABCMeta, abstractmethod

try:
    import numpy as np
    numpy_available = True
except:
    numpy_available = False

import CoSimIO


class CoSimIO_Vector:
    class BaseTests(unittest.TestCase, metaclass=ABCMeta):
        maxDiff = None # to display all the diff
        @abstractmethod
        def _CreateVector(self, *args): pass

        def test_basics(self):
            vec = self._CreateVector()
            self.assertEqual(vec.size(), 0)
            self.assertEqual(len(vec), 0)

            vec.append(15)
            self.assertEqual(vec.size(), 1)
            self.assertEqual(len(vec), 1)
            self.assertAlmostEqual(vec[0], 15)

            vec.append(-3)
            self.assertEqual(vec.size(), 2)
            self.assertEqual(len(vec), 2)
            self.assertAlmostEqual(vec[0], 15)
            self.assertAlmostEqual(vec[1], -3)

        def test_resize(self):
            vec = self._CreateVector()
            self.assertEqual(vec.size(), 0)

            vec.resize(10)
            self.assertEqual(vec.size(), 10)

            vec.resize(5)
            self.assertEqual(vec.size(), 5)

            vec.resize(23)
            self.assertEqual(vec.size(), 23)

        def test_iterators(self):
            vec = self._CreateVector()
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

        def test_copy_constructor(self):
            vec = self._CreateVector()
            for i in range(10):
                vec.append(i)

            vec_copy = self._CreateVector(vec)

            self.assertEqual(vec.size(), vec_copy.size())
            for v, v_c in zip(vec, vec_copy):
                self.assertAlmostEqual(v, v_c)

            # make sure that it is a deep and not a shallow copy
            vec[0] = 123456
            self.assertAlmostEqual(vec_copy[0], 0) # copy is unchanged

            vec.resize(5)
            self.assertEqual(vec_copy.size(), 10)

        def test_construction_from_list(self):
            init_list = [1, 2, 3, -4, 17]

            vec = self._CreateVector(init_list)
            self.assertEqual(vec.size(), len(init_list))

            for l, v in zip(init_list, vec):
                self.assertAlmostEqual(l,v)


class CoSimIO_DoubleVector(CoSimIO_Vector.BaseTests):
    def _CreateVector(self, *args):
        return CoSimIO.DoubleVector(*args)

    def test_print(self):
        vec = self._CreateVector([1.2, 2.5, 3.3, -4.1, 17])

        self.assertMultiLineEqual(str(vec), "[1.2, 2.5, 3.3, -4.1, 17]")

    def test_doubles(self):
        # general tests are done with ints, hence explicitly checking with doubles
        vec = self._CreateVector()
        for i in range(10):
            vec.append(i*1.5)

        self.assertEqual(vec.size(), 10)

        for i in range(10):
            self.assertAlmostEqual(vec[i], i*1.5)

    @unittest.skipUnless(numpy_available, "this test requries numpy")
    def test_construction_from_numpy_array(self):
        arr = np.array([1.1,2.2,-3.2,4,5,6.78,7,8.78,9], dtype=np.double)

        vec = self._CreateVector(arr)
        self.assertEqual(vec.size(), arr.size)

        for a, v in zip(arr, vec):
            self.assertAlmostEqual(a,v)

class CoSimIO_IntVector(CoSimIO_Vector.BaseTests):
    def _CreateVector(self, *args):
        return CoSimIO.IntVector(*args)

    def test_print(self):
        vec = self._CreateVector([1, 2, 3, -4, 17])

        self.assertMultiLineEqual(str(vec), "[1, 2, 3, -4, 17]")

    @unittest.skipUnless(numpy_available, "this test requries numpy")
    def test_construction_from_numpy_array(self):
        arr = np.array([10,2,-3,4,5,6,7,8,9], dtype=np.intc)

        vec = self._CreateVector(arr)
        self.assertEqual(vec.size(), arr.size)

        for a, v in zip(arr, vec):
            self.assertEqual(a,v)


# test with numpy => See Kratos: add_matrix_to_python.cpp => buffer_info

# strcpy for C-connection_name


# Paper:
# - Not restricted to data (should be serializable)
# - data transer - mapping - orchestrator are separeted => each can be exchnaged independently => can be adapted easily for many usecases
# - show case



if __name__ == '__main__':
    unittest.main()
