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

def LaunchSubprocess(cmd, *args):
    path_bin = os.path.join(os.path.dirname(os.path.abspath(__file__)), os.pardir, "bin")

    if os.name=="nt":
        cmd = cmd + ".exe"
        use_shell=True
    else:
        cmd = "./" + cmd
        use_shell=False

    return subprocess.Popen(
        [cmd, *args],
        stdout=subprocess.PIPE,
        cwd=path_bin,
        shell=use_shell)

def CollectSubprocessResult(sp):
    process_stdout, process_stderr = sp.communicate()
    if process_stdout:
        print(process_stdout.decode('ascii'), file=sys.stdout)
    if process_stderr:
        print(process_stderr.decode('ascii'), file=sys.stderr)

    return sp.returncode
