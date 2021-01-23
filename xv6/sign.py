import sys


# def create_test_file(filename, num_bytes):
#     file_content = bytearray(b'\xdd') * num_bytes
#     with open(filename, 'wb') as test_file:
#         test_file.write(file_content)


def bootloader_512(filename):
    with open(filename, 'rb') as f:
        bytes_arr = bytearray(f.read())

    num_extra_bytes = 510 - len(bytes_arr)

    if num_extra_bytes < 0:
        print('ERROR. The code of bootloader is too long.')
        sys.exit(1)

    bytes_arr += bytearray(b'\x00') * num_extra_bytes + bytearray(b'\x55\xAA')
    with open(filename, 'wb') as f:
        f.write(bytes_arr)


# create_test_file('test_file', 200)
bootloader_512(sys.argv[1])
