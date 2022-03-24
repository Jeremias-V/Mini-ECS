from miniecs_test_package import miniecs
from time import sleep

def main():
    connection = miniecs.MiniECS(port=7070)
    print(connection.create_container("test1"))
    print(connection.create_container("test2"))
    print(connection.create_container("test3"))
    print(connection.create_container("test4"))
    print(connection.list_containers())
    print(connection.stop_container("test4"))
    print()
    print(connection.list_containers())
    print(connection.delete_instance("test3"))
    print()
    print(connection.list_containers())

    connection.stop()

main()