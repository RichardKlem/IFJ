import sys
import os
import subprocess


class Bcolors:
    HEADER = '\033[95m'
    OKBLUE = '\033[94m'
    OKGREEN = '\033[92m'
    WARNING = '\033[93m'
    FAIL = '\033[91m'
    ENDC = '\033[0m'
    BOLD = '\033[1m'
    UNDERLINE = '\033[4m'


def main():

    path = os.getcwd()
    input_files = []
    expected_output_files = []
    src_ext = ('.ifj19', '.src')
    out_ext = '.out'

    # r=root, d=directories, f = files
    for r, d, f in os.walk(path):
        for file in f:
            if file.endswith(src_ext):
                input_files.append(os.path.join(r, file))

    for r, d, f in os.walk(path):
        for file in f:
            if file.endswith(out_ext):
                expected_output_files.append(os.path.join(r, file))

    for file in input_files:
        actual_output_file = file[:-5] + "actout"
        output_file = file[:-5] + "out"

        command = "./a {} > {}".format(file, actual_output_file)
        subprocess.run(command, shell=True, universal_newlines=True, check=True)

        actual_output = (open(actual_output_file).read()).splitlines()
        expected_output = (open(output_file).read()).splitlines()
        test_file_name = str(file.split('/')[-1])

        if actual_output == expected_output:
            print(Bcolors.OKGREEN + Bcolors.BOLD + 'PASSED ' + Bcolors.ENDC + test_file_name)
        else:
            print(Bcolors.FAIL + Bcolors.BOLD + 'FAILED ' + Bcolors.ENDC + test_file_name)
            for line1, line2 in zip(expected_output, actual_output):
                if line1.strip() and line2.strip() and line1 != line2:
                    print(Bcolors.BOLD +
                          "Expected:\n" + Bcolors.ENDC +
                          "{}\n".format(line1) + Bcolors.BOLD +
                          "Got:\n" + Bcolors.ENDC +
                          "{}\n".format(line2))
        #os.remove(actual_output_file)


if __name__ == '__main__':
    sys.exit(main())
