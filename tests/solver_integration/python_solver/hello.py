import CoSimIO
return_info = CoSimIO.Hello()

def cosimio_check_equal(a, b):
    assert a is b


print(return_info)
major_version = return_info.GetInt("major_version")
minor_version = return_info.GetInt("minor_version")
patch_version = return_info.GetString("patch_version")

cosimio_check_equal(major_version,1)
cosimio_check_equal(minor_version,0)

print("CoSimIO version", str(major_version)+"."+str(minor_version)+"."+patch_version)