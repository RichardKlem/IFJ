def foo():
    x = 1
    return x
def main():
    # assigment
    x, y, z = 'foo', 14, 21.1
    print(x, y, z)

    # for i in range(start, stop, step)
    for i in range(1, 6):
        print(i)

    #

    # function call
    y = foo()

if __name__ == '__main__':
    # main
    main()
