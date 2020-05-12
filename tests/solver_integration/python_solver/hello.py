import CoSimIO
return_info = CoSimIO.Hello()

print(return_info)
major_version = return_info.GetInt("major_version")
minor_version = return_info.GetInt("minor_version")
patch_version = return_info.GetString("patch_version")

assert major_version is 1
assert minor_version is 0

print("CoSimIO version", str(major_version)+"."+str(minor_version)+"."+patch_version)