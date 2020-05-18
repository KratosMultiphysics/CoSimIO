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
import os
import json


def _GetFilePath(fileName):
    return os.path.join(os.path.dirname(os.path.realpath(__file__)), fileName)


with open(_GetFilePath("compiled_config.json")) as json_file:
    compiled_config = json.load(json_file)


def IsOptionEnabled(option):
    return compiled_config[option] == "ON"
