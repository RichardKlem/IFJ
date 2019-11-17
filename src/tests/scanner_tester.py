import sys
import os
import subprocess

def compare(file1, file2):
    with open(file1, 'r') as f:
        d = set(f.readlines())

    with open(file2, 'r') as f:
        e = set(f.readlines())

    open('file3.txt', 'w').close()

    with open('file3.txt', 'a') as f:
        for line in list(d-e):
            f.write(line)


def main():

    path = os.getcwd()
    input_files = []
    expected_output_files = []

    # r=root, d=directories, f = files
    for r, d, f in os.walk(path):
        for file in f:
            if file.endswith('ifj19'):
                input_files.append(os.path.join(r, file))

    for r, d, f in os.walk(path):
        for file in f:
            if file.endswith('.out'):
                expected_output_files.append(os.path.join(r, file))

    for file in input_files:
        output_name = file[:-5] + "actout"
        command = "a.exe {}".format(file)# > {}".format(file, output_name)
        results = subprocess.run(
            command, shell=True, universal_newlines=True, check=True)
        output = results.stdout
        print(output)
        if output == open((file[:-5] + "out")).read():
            print('PASSED')
        else:
            print('FAILED')


if __name__ == '__main__':
    sys.exit(main())
