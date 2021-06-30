num_supps = 0
suppressions = []

supp_started = False
current_supp = ""

with open("MemoryChecker.xxx.log") as val_file: # Use file to refer to the file object
    lines = val_file.readlines()
    for line in lines:
        if not line.startswith("=="):
            current_supp += line

            if line.startswith("{"):
                num_supps +=1
                if supp_started:
                    raise Exception("cannot start twice!")
                supp_started = True

            elif line.startswith("}"):
                if not supp_started:
                    raise Exception("not yet sterted!")
                suppressions.append(current_supp)
                supp_started = False
                current_supp = ""

print(f"Number of suppressions initially: {num_supps}")

suppressions = list(set(suppressions))

print(f"Number of suppressions after removing duplicates: {len(suppressions)}")

with open("cleaned.supp", "w") as val_file: # Use file to refer to the file object
    val_file.writelines(suppressions)