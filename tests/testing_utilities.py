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

# This file contains functionalities used for the testing of CoSimIO

# python imports
import unittest, sys, os
import json
import subprocess


def _GetFilePath(fileName):
    return os.path.join(os.path.dirname(os.path.realpath(__file__)), fileName)


with open(_GetFilePath("compiled_config.json")) as json_file:
    compiled_config = json.load(json_file)


def IsOptionEnabled(option):
    return compiled_config[option] == "ON"


class SubprocessWrapper(object):

    def __init__(self, cmd, args, cwd, shell=False):
        self.sp = subprocess.Popen(
            [cmd, *args],
            stdout=subprocess.PIPE,
            cwd=cwd,
            shell=shell)

    def GetReturnCode(self):
        process_stdout, process_stderr = self.sp.communicate()
        if process_stdout:
            print(process_stdout.decode('ascii'), file=sys.stdout)
        if process_stderr:
            print(process_stderr.decode('ascii'), file=sys.stderr)

        return self.sp.returncode


class RunTestRunner(SubprocessWrapper):
    def __init__(self, cmd, args=None):
        cwd = os.path.join(os.path.dirname(os.path.abspath(__file__)), os.pardir, "bin")

        if os.name=="nt":
            cmd = cmd + ".exe"
            shell=True
        else:
            cmd = "./" + cmd
            shell=False

        if args is None:
            args = []

        super().__init__(cmd, args, cwd, shell)
