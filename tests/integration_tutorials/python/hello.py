#     ______     _____ _           ________
#    / ____/___ / ___/(_)___ ___  /  _/ __ |
#   / /   / __ \\__ \/ / __ `__ \ / // / / /
#  / /___/ /_/ /__/ / / / / / / // // /_/ /
#  \____/\____/____/_/_/ /_/ /_/___/\____/
#  Kratos CoSimulationApplication
#
#  License:         BSD License, see license.txt
#
#  Main authors:    Pooyan Dadvand
#

import CoSimIO

def cosimio_check_greater_equal(a, b):
    assert a >= b

info = CoSimIO.Hello()

print(info)
major_version = info.GetInt("major_version")
minor_version = info.GetInt("minor_version")
patch_version = info.GetString("patch_version")

cosimio_check_greater_equal(major_version,2)
cosimio_check_greater_equal(minor_version,0)

print("CoSimIO version", str(major_version)+"."+str(minor_version)+"."+patch_version)