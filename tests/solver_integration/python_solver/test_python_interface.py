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
import unittest, os
import subprocess

class PythonInterfaceTests(unittest.TestCase):
    CWD = os.path.dirname(os.path.realpath(__file__))
    USE_SHELL = os.name=="nt" # using shell on win
    PYTHON_CMD = os.getenv("PYTHON_CMD", "python3") # use "python3" unless specified otherwise

    def setUp(self):
        # removing leftover files
        for file_name in os.listdir(PythonInterfaceTests.CWD):
            if "CoSimIO_" in file_name:
                os.remove(os.path.join(PythonInterfaceTests.CWD,file_name))

    def test_hello(self):
        self.__RunScript("hello.py")

    def test_connect_disconnect(self):
        self.__RunScripts("connect_disconnect.py", "connect_disconnect.py")

    def test_import_export_data(self):
        self.__RunScripts("export_data.py", "import_data.py")

    def test_import_export_mesh(self):
        self.__RunScripts("export_mesh.py", "import_mesh.py")

    def __RunScript(self, script_name):
        p = subprocess.Popen([PythonInterfaceTests.PYTHON_CMD, script_name], stdout=subprocess.PIPE, cwd=PythonInterfaceTests.CWD, shell=PythonInterfaceTests.USE_SHELL)
        p.wait(5)
        p_out = p.communicate()
        self.assertEqual(p.returncode, 0, msg=GetErrMsg(script_name, p_out))

    def __RunScripts(self, script_name_1, script_name_2):
        p1 = subprocess.Popen([PythonInterfaceTests.PYTHON_CMD, script_name_1], stdout=subprocess.PIPE, cwd=PythonInterfaceTests.CWD, shell=PythonInterfaceTests.USE_SHELL)
        p2 = subprocess.Popen([PythonInterfaceTests.PYTHON_CMD, script_name_2], stdout=subprocess.PIPE, cwd=PythonInterfaceTests.CWD, shell=PythonInterfaceTests.USE_SHELL)
        p1.wait(5)
        p2.wait(5)

        p1_out = p1.communicate()
        p2_out = p2.communicate()

        self.assertEqual(p1.returncode, 0, msg=GetErrMsg(script_name_1, p1_out))
        self.assertEqual(p2.returncode, 0, msg=GetErrMsg(script_name_2, p2_out))

def GetErrMsg(script_name, p_out):
    err_msg = '\nRunning script "{}" was not successful!'.format(script_name)
    p_stdout = p_out[0]
    p_stderr = p_out[1]
    if p_stdout:
        err_msg += "\nSTDOUT:\n" + p_stdout.decode('ascii')
    if p_stderr:
        err_msg += "\nSTDERR:\n" + p_stderr.decode('ascii')

    return err_msg


if __name__ == '__main__':
    unittest.main()
