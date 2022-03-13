from miniecs_test_package import miniecs
from time import sleep

def main():
    connection = miniecs.MiniECS()
    print(connection.create_container("Test_1"))
    print(connection.create_container("Test_2"))
    print(connection.create_container("Test_3"))
    sleep(5)
    print(connection.create_container("Test_4"))
    connection.stop()

main()