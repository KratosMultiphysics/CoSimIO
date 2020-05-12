import CoSimIO
info = CoSimIO.Hello()

def cosimio_check_equal(a, b):
    assert a is b


print(info)
major_version = info.GetInt("major_version")
minor_version = info.GetInt("minor_version")
patch_version = info.GetString("patch_version")

cosimio_check_equal(major_version,1)
cosimio_check_equal(minor_version,0)

print("CoSimIO version", str(major_version)+"."+str(minor_version)+"."+patch_version)