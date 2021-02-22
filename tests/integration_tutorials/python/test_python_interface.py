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
import unittest, os, sys
import subprocess

class PythonInterfaceTests(unittest.TestCase):
    CWD = os.path.dirname(os.path.realpath(__file__))
    USE_SHELL = False #os.name=="nt" # using shell on win
    PYTHON_CMD = os.getenv("PYTHON_CMD", sys.executable)
    if not PYTHON_CMD:
        raise Exception("Python command could not be determined!")

    def setUp(self):
        # removing leftover files
        for file_name in os.listdir(PythonInterfaceTests.CWD):
            if "CoSimIO_" in file_name:
                os.remove(os.path.join(PythonInterfaceTests.CWD,file_name))

    def test_hello(self):
        self.__RunScript("hello.py")

    def test_connect_disconnect(self):
        self.__RunScripts("connect_disconnect_a.py", "connect_disconnect_b.py")

    def test_import_export_info(self):
        self.__RunScripts("export_info.py", "import_info.py")

    def test_import_export_data(self):
        self.__RunScripts("export_data.py", "import_data.py")

    def test_import_export_mesh(self):
        self.__RunScripts("export_mesh.py", "import_mesh.py")

    def test_run(self):
        self.__RunScripts("run.py", "runner.py")

    def __RunScript(self, script_name):
        p = subprocess.Popen([PythonInterfaceTests.PYTHON_CMD, script_name], stdout=subprocess.PIPE, cwd=PythonInterfaceTests.CWD)
        try:
            p_out = p.communicate(timeout=5)
            p_timeout_reached = False
        except subprocess.TimeoutExpired: # Timeout reached
            p_timeout_reached = True
            p.kill()
            p_out = p.communicate()

        self.assertEqual(p.returncode, 0, msg=GetErrMsg(script_name, p_out, p_timeout_reached))

    def __RunScripts(self, script_name_1, script_name_2):
        p1 = subprocess.Popen([PythonInterfaceTests.PYTHON_CMD, script_name_1], stdout=subprocess.PIPE, cwd=PythonInterfaceTests.CWD)
        p2 = subprocess.Popen([PythonInterfaceTests.PYTHON_CMD, script_name_2], stdout=subprocess.PIPE, cwd=PythonInterfaceTests.CWD)

        try:
            p1_out = p1.communicate(timeout=5)
            p1_timeout_reached = False
        except subprocess.TimeoutExpired: # Timeout reached
            p1_timeout_reached = True
            p1.kill()
            p1_out = p1.communicate()

        try:
            p2_out = p2.communicate(timeout=5)
            p2_timeout_reached = False
        except subprocess.TimeoutExpired: # Timeout reached
            p2_timeout_reached = True
            p2.kill()
            p2_out = p2.communicate()

        self.assertEqual(p1.returncode, 0, msg=GetErrMsg(script_name_1, p1_out, p1_timeout_reached))
        self.assertEqual(p2.returncode, 0, msg=GetErrMsg(script_name_2, p2_out, p2_timeout_reached))

def GetErrMsg(script_name, p_out, timeout_reached):
    err_msg = '\nRunning script "{}" was not successful! (timeout reached: {})'.format(script_name, timeout_reached)
    p_stdout = p_out[0]
    p_stderr = p_out[1]

    if p_stdout:
        err_msg += "\nSTDOUT:\n" + p_stdout.decode('ascii')
    if p_stderr:
        err_msg += "\nSTDERR:\n" + p_stderr.decode('ascii')

    return err_msg


if __name__ == '__main__':
    unittest.main()
