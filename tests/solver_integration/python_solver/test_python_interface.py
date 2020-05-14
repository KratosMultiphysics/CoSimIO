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

# tests for the python interface of the CoSimIO

# python imports
import unittest, sys, os
import subprocess

class PythonInterfaceTests(unittest.TestCase):
    def test_hello(self):
        subprocess.run(['python3', 'hello.py'], check=True,stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL, cwd=os.path.dirname(os.path.realpath(__file__)))

    def test_connect_disconnect(self):
        self.__RunScripts("connect_disconnect.py", "connect_disconnect.py")

    def test_import_export_data(self):
        self.__RunScripts("export_data.py", "import_data.py")

    def test_import_export_mesh(self):
        self.__RunScripts("export_mesh.py", "import_mesh.py")

    def __RunScripts(self, script_1, script_2):
        p1 = subprocess.Popen(['python3', script_1],stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL, cwd=os.path.dirname(os.path.realpath(__file__)))
        p2 = subprocess.Popen(['python3', script_2],stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL, cwd=os.path.dirname(os.path.realpath(__file__)))
        p1.wait(5)
        p2.wait(5)

    def setUp(self):
        # removing leftover files
        for file_name in os.listdir(os.path.dirname(os.path.realpath(__file__))):
            if "CoSimIO_" in file_name:
                os.remove(file_name)


if __name__ == '__main__':
    unittest.main()
