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
    for file in input_files:
        if file.endswith('.ifj19'):
            actual_output_file = file[:-6] + act_out_ext
            output_file = file[:-6] + out_ext
        elif file.endswith('.src'):
            actual_output_file = file[:-4] + act_out_ext
            output_file = file[:-4] + out_ext

        command = "./scanner_test_app {} > {}".format(file, actual_output_file)
        subprocess.run(command, shell=True, universal_newlines=True, check=True)

        actual_output = (open(actual_output_file).read()).splitlines()
        expected_output = (open(output_file).read()).splitlines()
        test_file_name = str(file.split('/')[-1])

        if actual_output == expected_output:
            print(Bcolors.OKGREEN + Bcolors.BOLD + 'PASSED ' + Bcolors.ENDC + test_file_name)
        else:
            print(Bcolors.FAIL + Bcolors.BOLD + 'FAILED ' + Bcolors.ENDC + test_file_name)
            for line1, line2 in zip(expected_output, actual_output):
                if line1 != line2:
                    print(Bcolors.BOLD +
                          "Expected:\n" + Bcolors.ENDC +
                          "{}\n".format(line1) + Bcolors.BOLD +
                          "Got:\n" + Bcolors.ENDC +
                          "{}\n".format(line2))
        if len(expected_output) != len(actual_output):
            larger_file, larger_file_name = (expected_output, str(output_file.split('/')[-1])) \
                    if expected_output > actual_output else (actual_output, str(actual_output_file.split('/')[-1]))
            print( Bcolors.BOLD + "Extra lines in " + larger_file_name + Bcolors.ENDC)
            for line in larger_file[-(abs(len(actual_output) - len(expected_output))):]:
                print("+" + line)
        os.remove(actual_output_file)


if __name__ == '__main__':
    sys.exit(main())
