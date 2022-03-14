from miniecs_test_package import miniecs
from time import sleep

def main():
    connection = miniecs.MiniECS()
    print(connection.create_container("Ubuntu"))
    print(connection.create_container("Fedora"))
    print(connection.create_container("Mint"))
    sleep(5)
    print(connection.create_container("Kali"))
    print(connection.list_containers())
    print(connection.stop_container("Mint"))
    print()
    print(connection.list_containers())
    print(connection.delete_instance("Fedora"))
    print()
    print(connection.list_containers())

    connection.stop()

main()