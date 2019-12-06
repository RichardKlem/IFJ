import sys
import os
import subprocess
import re



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

    path = os.getcwd() + "/"
    input_files = []
    expected_output_files = []
    src_ext = ('.ifj19', '.src')
    out_ext = '.out'
    act_out_ext = '.actout'

    # r=root, d=directories, f = files
    for r, d, f in os.walk(path):
        for file in f:
            if file.endswith(src_ext):
                input_files.append(os.path.join(r, file))

    for r, d, f in os.walk(path):
        for file in f:
            if file.endswith(out_ext):
                expected_output_files.append(os.path.join(r, file))

    actual_output_file = ""
    output_file = ""
    passed_count = 0
    failed_count = 0
    error_code = 0
    for file in input_files:
        if file.endswith('.ifj19'):
            error_code = file[-6]

        elif file.endswith('.src'):
            error_code = file[-5]

        command = subprocess.run("./../../compiler < {}".format(file), shell=True, stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL)

        test_file_name = str(file.split('/')[-1])

        if command.returncode == int(error_code):
            print(Bcolors.OKGREEN + Bcolors.BOLD + 'PASSED ' + Bcolors.ENDC + test_file_name)
        else:
            print(Bcolors.FAIL + Bcolors.BOLD + 'FAILED ' + Bcolors.ENDC + test_file_name)
            print("Expected return code for FILE {}: {} ".format(test_file_name, error_code))
            print("Got: {}".format(command.returncode))


if __name__ == '__main__':
    sys.exit(main())
